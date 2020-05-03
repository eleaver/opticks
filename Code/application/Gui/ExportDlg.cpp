/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from
 * http://www.gnu.org/licenses/lgpl.html
 */

#include <QtCore/QDir>
#include <QListView>
#include <QMessageBox>

#include "AppVerify.h"
#include "ConfigurationSettings.h"
#include "DataElement.h"
#include "ExportDlg.h"
#include "ExportOptionsDlg.h"
#include "FileDescriptor.h"

#include <string>
using namespace std;

ExportDlg::ExportDlg(ExporterResource& pExporter,
                     const vector<PlugInDescriptor*>& availablePlugins,
                     QWidget* pParent) :
   FilePlugInDlg(availablePlugins, ConfigurationSettings::getSettingPluginWorkingDirectoryKey("Exporter"), pParent),
   mpExporter(pExporter),
   mValidated(false)
{
   // Initializtion
   setWindowTitle("Export");
   setFileMode(QFileDialog::AnyFile);
   setAcceptMode(QFileDialog::AcceptSave);
   setConfirmOverwrite(true);
   setLabelText(QFileDialog::LookIn, "Save in:");
   setPlugInLabel("Exporter");
   enableOptions(true);

   // Set the initial directory
   Service<ConfigurationSettings> pSettings;
   const Filename* pWorkingDir = pSettings->getSetting(
      ConfigurationSettings::getSettingPluginWorkingDirectoryKey("Exporter")).getPointerToValue<Filename>();
   if (pWorkingDir == NULL)
   {
      pWorkingDir = ConfigurationSettings::getSettingExportPath();
   }

   string directory;
   if (pWorkingDir != NULL)
   {
      directory = pWorkingDir->getFullPathAndName();
   }

   if (!directory.empty())
   {
      setDirectory(QString::fromStdString(directory));
   }

   if (isDefaultPlugIn())
   {
      setSelectedPlugIn("Ice Exporter");
   }
   updateFromExporter(getSelectedPlugIn());

   // Set the initially selected file
   QString strFilename;

   SessionItem* pSessionItem = mpExporter->getItem();
   if (pSessionItem != NULL)
   {
      DataElement* pDataElement = dynamic_cast<DataElement*>(pSessionItem);
      if (pDataElement != NULL)
      {
         strFilename = QString::fromStdString(pDataElement->getFilename());
      }

      if (strFilename.isEmpty() == true)
      {
         strFilename = QString::fromStdString(pSessionItem->getName());
      }

      strFilename = updateExtension(strFilename);
      selectFile(strFilename);
   }

   updateFromFile(strFilename);

   // Connections
   VERIFYNR(connect(this, SIGNAL(plugInSelected(const QString&)), this, SLOT(updateFromExporter(const QString&))));
   VERIFYNR(connect(this, SIGNAL(optionsClicked()), this, SLOT(invokeOptionsDialog())));
}

ExportDlg::~ExportDlg()
{}

ExporterResource& ExportDlg::getExporterResource() const
{
   return mpExporter;
}

void ExportDlg::accept()
{
   // Check for a valid plug-in
   QString strPlugIn = getSelectedPlugIn();
   if (strPlugIn.isEmpty() == true)
   {
      QMessageBox::critical(this, windowTitle(), "The selected exporter is invalid!");
      return;
   }

   // Validate with the exporter
   bool validating = true;
   while (validating == true)
   {
      string errorMessage;

      // Update the file descriptor with the selected file
      QString strFilename = getExportFile();
      if (QFileInfo(strFilename).isDir())
      {
         break;
      }
      updateFromFile(updateExtension(strFilename, true, false));

      // Only validate with the exporter if the options have not yet been validated
      if (mValidated == false)
      {
         switch (mpExporter->validate(errorMessage))
         {
         case VALIDATE_SUCCESS:
            validating = false;
            break;
         case VALIDATE_FAILURE:
            if (errorMessage.empty())
            {
               errorMessage = "Unable to validate inputs.";
            }
            QMessageBox::critical(this, windowTitle(), QString::fromStdString(errorMessage));
            return;
         case VALIDATE_INFO:
            // Exporter's returning VALIDATE_INFO must provide a value
            // for error message...not providing a value is a programmer error
            VERIFYNRV(!errorMessage.empty());
            switch (QMessageBox::warning(this, windowTitle(), QString::fromStdString(errorMessage),
               "OK", "Options...", "Cancel", 0, 2))
            {
            case 0: // Ok
               validating = false;
               break;
            case 1: // Options
               if (invokeOptionsDialog())
               {
                  break;
               }
               // fall through
            case 2: // Cancel
               return;
            }
            break;
         case VALIDATE_INPUT_REQUIRED:
            if (errorMessage.empty() == false)
            {
               QMessageBox::critical(this, windowTitle(), QString::fromStdString(errorMessage));
            }
            if (!invokeOptionsDialog())
            {
               return;
            }
            break;
         default:
            VERIFYNRV_MSG(false, "An invalid verification type was returned.");
         }
      }
      else
      {
         validating = false;
      }
   }

   FilePlugInDlg::accept();
}

void ExportDlg::hideEvent(QHideEvent* pEvent)
{
   // Set the selected directory to the current directory
   QDir dir = directory();
   QDir::setCurrent(dir.absolutePath());

   FilePlugInDlg::hideEvent(pEvent);
}

QString ExportDlg::getExportFile() const
{
   QStringList filenames = selectedFiles();
   if (filenames.empty() == false)
   {
      return filenames.front();
   }

   return QString();
}

void ExportDlg::updateFromFile(const QString& strFilename)
{
   FileDescriptor* pFileDescriptor = mpExporter->getFileDescriptor();
   if (pFileDescriptor != NULL)
   {
      string filename;
      if (strFilename.isEmpty() == false)
      {
         QFileInfo fileInfo(strFilename);
         if (fileInfo.isDir() == false)
         {
            filename = fileInfo.absoluteFilePath().toStdString();
         }
      }

      pFileDescriptor->setFilename(filename);
   }
}

QString ExportDlg::updateExtension(const QString& strFilename,
                                   bool preserveFullPath,
                                   bool alwaysReplaceExtension)
{
   if (strFilename.isEmpty())
   {
      return strFilename;
   }

   QFileInfo fileInfo(strFilename);
   if (fileInfo.isDir())
   {
      return "";
   }
   QString orgFilename = (preserveFullPath ? strFilename : fileInfo.fileName());

#if HAVE_QT5
   QString strCurrentFilter = selectedNameFilter();
#else
   QString strCurrentFilter = selectedFilter();
#endif
   int iOpenParen = strCurrentFilter.indexOf("(");
   int iCloseParen = strCurrentFilter.indexOf(")");

   QString strFilterExtension;
   if (iOpenParen != -1 && iCloseParen != -1)
   {
      strFilterExtension = strCurrentFilter.mid(iOpenParen + 1, iCloseParen - iOpenParen - 1);
      int filterStart = strFilterExtension.indexOf(".");
      int filterEnd = strFilterExtension.indexOf(" ");
      if (filterEnd == -1)
      {
         filterEnd = strFilterExtension.length();
      }
      if (filterStart != -1)
      {
         strFilterExtension = strFilterExtension.mid(filterStart, filterEnd - filterStart);
      }
      strFilterExtension = strFilterExtension.trimmed();
   }

   if (strFilterExtension.isEmpty() || strFilterExtension == "*")
   {
      return orgFilename;
   }
   QString newFilename;
   QString existingExtension = fileInfo.completeSuffix();
   QString orgFilenameWithoutExtension = fileInfo.baseName();
   if (alwaysReplaceExtension || existingExtension.isEmpty())
   {
      newFilename = orgFilenameWithoutExtension + strFilterExtension;
   }
   else
   {
      newFilename = orgFilenameWithoutExtension + "." + existingExtension;
   }
   return (preserveFullPath ? fileInfo.path() + "/" + newFilename : newFilename);
}

void ExportDlg::updateFromExporter(const QString& strExporter)
{
   // Update the exporter
   if (strExporter.isEmpty() == false)
   {
      selectFile(updateExtension(getExportFile()));
      updateFromFile(getExportFile());
      string exporter = strExporter.toStdString();
      mpExporter->setPlugIn(exporter);
   }
   else
   {
      mpExporter->setPlugIn(NULL);
   }
}

bool ExportDlg::invokeOptionsDialog()
{
   // Update the file descriptor with the selected file
   QString strFilename = getExportFile();
   updateFromFile(strFilename);

   // Display the dialog
   ExportOptionsDlg optionsDlg(mpExporter, this);
   if (optionsDlg.exec() == QDialog::Accepted)
   {
      // Selecting file because the filename can be changed in Export Options widget (ie: ShapeFileOptionsWidget).
      if (mpExporter.get() != NULL)
      {
         FileDescriptor* pFileDesc = mpExporter->getFileDescriptor();
         if (pFileDesc != NULL)
         {
            std::string newFilename = pFileDesc->getFilename().getFullPathAndName();
            selectFile(QString::fromStdString(newFilename));
         }
      }

      mValidated = true;
      return true;
   }

   mValidated = false;
   return false;
}
