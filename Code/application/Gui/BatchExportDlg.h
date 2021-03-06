/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BATCHEXPORTDLG_H
#define BATCHEXPORTDLG_H

#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>

#include <vector>

class ExporterResource;
class PlugInDescriptor;

class BatchExportDlg : public QDialog
{
   Q_OBJECT

public:
   BatchExportDlg(ExporterResource& exporter, const std::vector<PlugInDescriptor*>& availablePlugIns,
      QWidget* pParent = NULL);
   ~BatchExportDlg();

   QString getExportDirectory() const;
   QString getExporterName() const;
   QString getFileExtension() const;

protected slots:
   void browse();
   void updateExporter(const QString& strExporter);
   void updateFileExtensions();

private:
   BatchExportDlg(const BatchExportDlg& rhs);
   BatchExportDlg& operator=(const BatchExportDlg& rhs);
   QLineEdit* mpDirectoryEdit;
   QComboBox* mpExporterCombo;
   QComboBox* mpExtensionCombo;

   ExporterResource& mpExporter;
   QMap<QString, QStringList> mFileExtensions;
};

#endif
