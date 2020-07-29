/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AddFeatureDlg.h"
#include "AnnotationLayer.h"
#include "AppVerify.h"
#include "ConfigurationSettings.h"
#include "DataVariant.h"
#include "DynamicObject.h"
#include "GraphicGroup.h"
#include "GraphicObject.h"
#include "StringUtilities.h"
#include "TypesFile.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>

using namespace std;

AddFeatureDlg::AddFeatureDlg(const vector<GraphicElement*>& graphicElements,
   ShapefileTypes::ShapeType shapeType, QWidget* pParent) :
   QDialog(pParent)
{
   // Element list
   QLabel* pElementLabel = new QLabel("Data Elements:", this);

   QStringList columnNames;
   columnNames.append("Name");
   columnNames.append("Type");

   mpElementTree = new QTreeWidget(this);
   mpElementTree->setColumnCount(columnNames.count());
   mpElementTree->setHeaderLabels(columnNames);
   mpElementTree->setSelectionBehavior(QAbstractItemView::SelectRows);
   mpElementTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
   mpElementTree->setRootIsDecorated(true);
   mpElementTree->setAllColumnsShowFocus(true);
   mpElementTree->setSortingEnabled(true);
   mpElementTree->sortByColumn(0, Qt::AscendingOrder);

   QHeaderView* pHeader = mpElementTree->header();
   if (pHeader != NULL)
   {
      pHeader->setSortIndicatorShown(true);
      pHeader->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
   }

   mpFeatureClassCheck = new QCheckBox("Apply feature class:", this);
   mpFeatureClassCheck->setChecked(false);

   mpFeatureClassCombo = new QComboBox(this);
   mpFeatureClassCombo->setEditable(false);
   mpFeatureClassCombo->setEnabled(false);

   Service<ConfigurationSettings> pSettings;
   const DataVariant& featureClasses = pSettings->getSetting("ShapeFileExporter/FeatureClasses");

   const DynamicObject* pFeatureClasses = featureClasses.getPointerToValue<DynamicObject>();
   if (pFeatureClasses != NULL)
   {
      vector<string> classNames;
      pFeatureClasses->getAttributeNames(classNames);
      for (vector<string>::const_iterator iter = classNames.begin(); iter != classNames.end(); ++iter)
      {
         QString className = QString::fromStdString(*iter);
         if (className.isEmpty() == false)
         {
            mpFeatureClassCombo->addItem(className);
         }
      }
   }

   if (mpFeatureClassCombo->count() == 0)
   {
      mpFeatureClassCheck->setEnabled(false);
   }

   // Horizontal line
   QFrame* pHLine = new QFrame(this);
   pHLine->setFrameStyle(QFrame::HLine | QFrame::Sunken);

   // Dialog buttons
   QPushButton* pOkButton = new QPushButton("&OK", this);
   QPushButton* pCancelButton = new QPushButton("&Cancel", this);

   // Layout
   QHBoxLayout* pLayout = new QHBoxLayout();
   pLayout->setMargin(0);
   pLayout->setSpacing(5);
   pLayout->addStretch(10);
   pLayout->addWidget(pOkButton);
   pLayout->addWidget(pCancelButton);

   QGridLayout* pGrid = new QGridLayout(this);
   pGrid->setMargin(10);
   pGrid->setSpacing(10);
   pGrid->addWidget(pElementLabel, 0, 0, 1, 2);
   pGrid->addWidget(mpElementTree, 1, 0, 1, 2);
   pGrid->addWidget(mpFeatureClassCheck, 2, 0);
   pGrid->addWidget(mpFeatureClassCombo, 2, 1);
   pGrid->addWidget(pHLine, 3, 0, 1, 2);
   pGrid->addLayout(pLayout, 4, 0, 1, 2);
   pGrid->setRowStretch(1, 10);
   pGrid->setColumnStretch(1, 10);

   // Initialization
   setWindowTitle("Add Feature");
   setModal(true);
   resize(500, 300);

   // Populate the list with graphic elements and their objects that are supported by the shape type
   for (vector<GraphicElement*>::const_iterator iter = graphicElements.begin(); iter != graphicElements.end(); ++iter)
   {
      GraphicElement* pElement = *iter;
      if (pElement != NULL)
      {
         QTreeWidgetItem* pElementItem = new QTreeWidgetItem(mpElementTree);
         pElementItem->setText(0, QString::fromStdString(pElement->getName()));
         pElementItem->setText(1, "Element");
         pElementItem->setData(1, Qt::UserRole, QVariant::fromValue(pElement));

         GraphicGroup* pGroup = pElement->getGroup();
         if (pGroup != NULL)
         {
            std::list<GraphicObject*> objects;
            // If annotation layer, get only the selected objects.
            AnnotationLayer* pAnnotationLayer = dynamic_cast<AnnotationLayer*>(pGroup->getLayer());
            if (pAnnotationLayer != NULL)
            {
               pAnnotationLayer->getSelectedObjects(objects);
            }
            // If no objects selected, or this is an AOI layer, get all objects.
            if (objects.empty())
            {
               objects = pGroup->getObjects();
            }

            for (list<GraphicObject*>::const_iterator objectIter = objects.begin();
               objectIter != objects.end();
               ++objectIter)
            {
               GraphicObject* pObject = *objectIter;
               if (pObject != NULL)
               {
                  if (pObject->getDrawMode() == DRAW)
                  {
                     GraphicObjectType objectType = pObject->getGraphicObjectType();

                     bool addItem = true;
                     if (shapeType == ShapefileTypes::POLYLINE_SHAPE)
                     {
                        if ((objectType != LINE_OBJECT) && (objectType != POLYLINE_OBJECT) &&
                           (objectType != HLINE_OBJECT) && (objectType != VLINE_OBJECT))
                        {
                           addItem = false;
                        }
                     }
                     else if (shapeType == ShapefileTypes::POLYGON_SHAPE)
                     {
                        if ((objectType != RECTANGLE_OBJECT) &&
                           (objectType != POLYGON_OBJECT) &&
                           (objectType != ARC_OBJECT) &&
                           (objectType != ROUNDEDRECTANGLE_OBJECT) &&
                           (objectType != TRIANGLE_OBJECT) &&
                           (objectType != ELLIPSE_OBJECT))
                        {
                           addItem = false;
                        }
                     }

                     if (addItem == true)
                     {
                        QTreeWidgetItem* pObjectItem = new QTreeWidgetItem(pElementItem);
                        pObjectItem->setText(0, QString::fromStdString(pObject->getName()));
                        pObjectItem->setText(1, QString::fromStdString(StringUtilities::toDisplayString(objectType)));
                        pObjectItem->setData(1, Qt::UserRole, QVariant::fromValue(pObject));
                     }
                  }
               }
            }
         }
      }
   }

   mpElementTree->expandAll();

   for (int i = 0; i < columnNames.count(); ++i)
   {
      mpElementTree->resizeColumnToContents(i);
   }

   mpElementTree->collapseAll();

   // Connections
   VERIFYNR(connect(mpFeatureClassCheck, SIGNAL(toggled(bool)), mpFeatureClassCombo, SLOT(setEnabled(bool))));
   VERIFYNR(connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept())));
   VERIFYNR(connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject())));
}

AddFeatureDlg::~AddFeatureDlg()
{}

map<GraphicElement*, vector<GraphicObject*> > AddFeatureDlg::getFeatureItems() const
{
   map<GraphicElement*, vector<GraphicObject*> > featureItems;

   int numElements = mpElementTree->topLevelItemCount();
   for (int i = 0; i < numElements; ++i)
   {
      QTreeWidgetItem* pElementItem = mpElementTree->topLevelItem(i);
      if (pElementItem != NULL)
      {
         GraphicElement* pGraphicElement = pElementItem->data(1, Qt::UserRole).value<GraphicElement*>();
         if (pGraphicElement != NULL)
         {
            if (pElementItem->isSelected() == true)
            {
               // An empty vector indicates that all objects in the graphic element should be used
               featureItems[pGraphicElement] = vector<GraphicObject*>();
            }
            else
            {
               vector<GraphicObject*> objects;
               for (int j = 0; j < pElementItem->childCount(); ++j)
               {
                  QTreeWidgetItem* pObjectItem = pElementItem->child(j);
                  if ((pObjectItem != NULL) && (pObjectItem->isSelected() == true))
                  {
                     GraphicObject* pObject = pObjectItem->data(1, Qt::UserRole).value<GraphicObject*>();
                     if (pObject != NULL)
                     {
                        objects.push_back(pObject);
                     }
                  }
               }

               if (objects.empty() == false)
               {
                  featureItems[pGraphicElement] = objects;
               }
            }
         }
      }
   }

   return featureItems;
}

QString AddFeatureDlg::getFeatureClass() const
{
   if (mpFeatureClassCheck->isChecked() == true)
   {
      return mpFeatureClassCombo->currentText();
   }

   return QString();
}

void AddFeatureDlg::accept()
{
   if (getFeatureItems().empty() == true)
   {
      QMessageBox::critical(this, windowTitle(), "Please select at least one item for a feature.");
      return;
   }

   QDialog::accept();
}
