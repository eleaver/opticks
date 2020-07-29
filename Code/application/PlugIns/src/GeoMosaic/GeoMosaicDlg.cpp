/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "DesktopServices.h"
#include "GeoMosaicDlg.h"
#include "LayerList.h"
#include "PlugInArgList.h"
#include "PlugInDescriptor.h"
#include "PlugInResource.h"
#include "Progress.h"
#include "RasterDataDescriptor.h"
#include "RasterElement.h"
#include "SpatialDataView.h"
#include "SpatialDataWindow.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

GeoMosaicDlg::GeoMosaicDlg(Progress* pProgress, QWidget* pParent) :
   QDialog(pParent)
{
   mProgressTracker.initialize(pProgress, "Executing GeoMosaic Plug-In", "app", "FD509668-9D9B-4C39-B3C4-3F5AE7B996F0");

   setWindowTitle("Geo-Mosaic Stitching");
   setModal(true);

   QLabel* pPrimaryDatasetLabel = new QLabel("Existing Data Set", this);
   mpPrimaryList = new QListWidget(this);
   mpPrimaryList->setSelectionMode(QAbstractItemView::ExtendedSelection);
   mpCreateAnimationCheckBox = new QCheckBox("Create Animation", this);
   mpDlgBtns = new QDialogButtonBox(this);
   QPushButton* pOkButton = mpDlgBtns->addButton(QDialogButtonBox::Ok);
   pOkButton->setEnabled(false);
   mpDlgBtns->addButton(QDialogButtonBox::Cancel);
   QPushButton* pBrowser = new QPushButton("Browse for Data", this);

   QGridLayout* pLayout = new QGridLayout(this);
   pLayout->addWidget(pPrimaryDatasetLabel, 0, 0, 1, 2);
   pLayout->addWidget(mpPrimaryList, 1, 0, 1, 2);
   pLayout->addWidget(mpCreateAnimationCheckBox, 2, 0);
   pLayout->addWidget(pBrowser, 2, 1);
   pLayout->setColumnStretch(0, 10);
   pLayout->addWidget(mpDlgBtns, 3, 0, 1, 2);

   // connections
   VERIFYNR(connect(pBrowser, SIGNAL(clicked()), this, SLOT(loadData())));
   VERIFYNR(connect(mpDlgBtns, SIGNAL(accepted()), this, SLOT(accept())));
   VERIFYNR(connect(mpDlgBtns, SIGNAL(rejected()), this, SLOT(reject())));
   VERIFYNR(connect(mpPrimaryList, SIGNAL(itemSelectionChanged()), this, SLOT(enableOK())));

   std::vector<Window*> windows;
   Service<DesktopServices>()->getWindows(SPATIAL_DATA_WINDOW, windows);
   for (std::vector<Window*>::iterator it = windows.begin(); it != windows.end(); ++it)
   {
      SpatialDataWindow* pWindow = dynamic_cast<SpatialDataWindow*>(*it);
      if (pWindow == NULL)
      {
         continue;
      }
      std::string windowName = pWindow->getName();
      if (windowName.empty())
      {
         continue;
      }
      QListWidgetItem* pItem = new QListWidgetItem(QString::fromStdString(windowName));
      pItem->setData(Qt::UserRole, false);
      mpPrimaryList->addItem(pItem);
   }
}

GeoMosaicDlg::~GeoMosaicDlg()
{}

void GeoMosaicDlg::enableOK()
{
   QPushButton* pOkButton = mpDlgBtns->button(QDialogButtonBox::Ok);
   if (pOkButton == NULL)
   {
      return;
   }
   if (!mpPrimaryList->selectedItems().empty())
   {
      pOkButton->setEnabled(true);
   }
   else
   {
      pOkButton->setEnabled(false);
   }
}

void GeoMosaicDlg::reject()
{
   QDialog::reject();
}

void GeoMosaicDlg::accept()
{
   QDialog::accept();
   batchStitch();
}

void GeoMosaicDlg::batchStitch()
{
   mProgressTracker.report("Mosaic...", 1, NORMAL);
   std::vector<PlugIn*> instances = Service<PlugInManagerServices>()->getPlugInInstances("Mosaic Manager");
   VERIFYNR(!instances.empty());
   MosaicManager* pManager = dynamic_cast<MosaicManager*>(instances.front());
   VERIFYNR(pManager != NULL);

   QList<QListWidgetItem*> items = mpPrimaryList->selectedItems();
   MosaicManager::MosaicData* pData = new MosaicManager::MosaicData();
   for (int i = 0; i < items.count(); ++i)
   {
      QListWidgetItem* pItem = items[i];
      if (pItem == NULL)
      {
         continue;
      }
      if (pItem->data(Qt::UserRole).toBool() == true)
      {
         ImporterResource autoImporter("Auto Importer", mProgressTracker.getCurrentProgress(), true);
         if (autoImporter.get() == NULL)
         {
            return;
         }
         std::string filename = pItem->text().toStdString();
         autoImporter->setFilename(filename);
         if (!autoImporter->execute())
         {
            mProgressTracker.report("Unable to load " + filename, 0, ERRORS, true);
            return;
         }
         std::vector<DataElement*> elements = autoImporter->getImportedElements();
         for (unsigned int i2 = 0; i2 < elements.size(); ++i2)
         {
            RasterElement* pRaster = dynamic_cast<RasterElement*>(elements[i2]);
            if (pRaster != NULL)
            {
               if (pRaster->getParent() == NULL)
               {
                  pData->mpRasters.push_back(pRaster);
               }
            }
         }
      }
      else
      {
         SpatialDataWindow* pWindow = 
            dynamic_cast<SpatialDataWindow*>(Service<DesktopServices>()->getWindow(pItem->text().toStdString(), SPATIAL_DATA_WINDOW));
         if (pWindow == NULL)
         {
            continue;
         }
         SpatialDataView* pView = pWindow->getSpatialDataView();
         if (pView == NULL)
         {
            continue;
         }
         RasterElement* pElement = pView->getLayerList()->getPrimaryRasterElement();
         if (pElement != NULL)
         {
            pData->mpRasters.push_back(pElement);
         }
      }
   }

   pData->createAnimation = mpCreateAnimationCheckBox->isChecked();

   if (!(pManager->geoStitch(pData, mProgressTracker.getCurrentProgress())))
   {
      mProgressTracker.report("The scenes could not be used to produce a mosaic", 0, ERRORS, true);
   }
   else
   {
      mProgressTracker.report("Completed.", 100, NORMAL, true);
      mProgressTracker.upALevel();
   }
}

void GeoMosaicDlg::loadData()
{
   QString strDirectory;
   const Filename* pWorkingDir = NULL;
   pWorkingDir = ConfigurationSettings::getSettingImportPath();
   if (pWorkingDir != NULL)
   {
      strDirectory = QString::fromStdString(pWorkingDir->getFullPathAndName());
   }
   QString filters = "All Files (*.*)";
   PlugInDescriptor* pDescriptor = Service<PlugInManagerServices>()->getPlugInDescriptor("Auto Importer");
   if (pDescriptor != NULL)
   {
      filters = QString::fromStdString(pDescriptor->getFileExtensions());
   }
   QStringList filenames = QFileDialog::getOpenFileNames(this, "Select files", strDirectory, filters);
   for (int i = 0; i < filenames.size(); ++i)
   {
      QListWidgetItem* pItem = new QListWidgetItem(filenames[i]);
      pItem->setData(Qt::UserRole, true);
      mpPrimaryList->addItem(pItem);
      pItem->setSelected(true);
   }
}
