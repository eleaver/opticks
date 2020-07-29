/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AppVerify.h"
#include "Blob.h"
#include "DataDescriptor.h"
#include "DataElement.h"
#include "DesktopServices.h"
#include "DynamicObject.h"
#include "Endian.h"
#include "EngrdaWidget.h"
#include "NitfConstants.h"
#include "NitfUtilities.h"
#include "PlugInRegistration.h"
#include "SessionManager.h"
#include "switchOnEncoding.h"
#include "NitfPropertiesQWidgetWrapper.h"

#include <boost/any.hpp>
#include <limits>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>

REGISTER_PLUGIN(OpticksNitfCommonTre, EngrdaWidget, Nitf::PropertiesQWidgetWrapper<EngrdaWidget>());

namespace
{
   template<typename T>
   void getDataValue(T* pData, size_t idx, double& val, Endian& endian)
   {
      T tmp = pData[idx];
      endian.swapValue(tmp);
      val = static_cast<double>(tmp);
   }
}

EngrdaWidget::EngrdaWidget(QWidget *pParent) :
      QWidget(pParent)
{
   mpTres = new QTreeWidget(this);
   mpTres->setHeaderHidden(true);
   mpTres->setSortingEnabled(true);
   mpTres->sortByColumn(0, Qt::AscendingOrder);
   mpTres->setColumnCount(1);

   mpValueType = new QLineEdit(this);
   mpValueType->setReadOnly(true);
   
   mpDataUnits = new QLineEdit(this);
   mpDataUnits->setReadOnly(true);

   mpData = new QTableWidget(this);
   mpData->setEditTriggers(QAbstractItemView::NoEditTriggers);

   QHBoxLayout* pTopLevel = new QHBoxLayout(this);
   pTopLevel->addWidget(mpTres);

   QGridLayout* pDataLayout = new QGridLayout();
   pTopLevel->addLayout(pDataLayout, 1);
   pDataLayout->addWidget(new QLabel("Value Type:"), 0, 0);
   pDataLayout->addWidget(mpValueType, 0, 1);
   pDataLayout->addWidget(new QLabel("Data Units:"), 1, 0);
   pDataLayout->addWidget(mpDataUnits, 1, 1);
   pDataLayout->addWidget(mpData, 2, 0, 1, 2);
   pDataLayout->setRowStretch(2, 1);

   VERIFYNR(connect(mpTres, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
                    this, SLOT(updateData(QTreeWidgetItem*))));

   mpValueType->setEnabled(false);
   mpDataUnits->setEnabled(false);
   mpData->setEnabled(false);
}

EngrdaWidget::~EngrdaWidget()
{
}

bool EngrdaWidget::initialize(SessionItem* pSessionItem)
{
   DataElement* pElement = dynamic_cast<DataElement*>(pSessionItem);
   DataDescriptor* pDesc = (pElement == NULL) ? NULL : pElement->getDataDescriptor();
   const DynamicObject* pMeta = (pDesc == NULL) ? NULL : pDesc->getMetadata();
   return initialize(pMeta);
}

bool EngrdaWidget::initialize(const DynamicObject* pMeta)
{
   VERIFY(pMeta);
   mpMeta->clear();
   QString baseTrePath = QString::fromStdString(Nitf::NITF_METADATA + "/" + Nitf::TRE_METADATA + "/ENGRDA");
   const DynamicObject* pEngrda = dv_cast<DynamicObject>(
      &pMeta->getAttributeByPath(baseTrePath.toStdString()));
   if (pEngrda == NULL)
   {
      return false;
   }
   mpMeta->merge(pEngrda);
   std::vector<std::string> treNames;
   mpMeta->getAttributeNames(treNames);
   if (treNames.empty())
   {
      return false;
   }
   for (std::vector<std::string>::const_iterator treName = treNames.begin(); treName != treNames.end(); ++treName)
   {
      try
      {
         DynamicObject& tre = dv_cast<DynamicObject>(mpMeta->getAttribute(*treName));
         std::string lbl = dv_cast<std::string>(tre.getAttribute(Nitf::TRE::ENGRDA::RESRC));
         QTreeWidgetItem* pTopItem = new QTreeWidgetItem(mpTres);
         pTopItem->setText(0, QString::fromStdString(lbl));
         pTopItem->setData(0, Qt::UserRole, qVariantFromValue(reinterpret_cast<void*>(&tre)));

         std::vector<std::string> recNames;
         tre.getAttributeNames(recNames);
         for (std::vector<std::string>::const_iterator recName = recNames.begin();
                  recName != recNames.end(); ++recName)
         {
            DynamicObject& rec = dv_cast<DynamicObject>(tre.getAttribute(*recName));
            std::string recLbl = dv_cast<std::string>(rec.getAttribute(Nitf::TRE::ENGRDA::ENGLBL));
            QTreeWidgetItem* pRecItem = new QTreeWidgetItem(pTopItem);
            pRecItem->setText(0, QString::fromStdString(recLbl));
            pRecItem->setData(0, Qt::UserRole, qVariantFromValue(reinterpret_cast<void*>(&rec)));
         }
      }
      catch (const std::bad_cast&)
      {
         continue;
      }
   }
   return true;
}

bool EngrdaWidget::applyChanges()
{
   return true;
}

void EngrdaWidget::updateData(QTreeWidgetItem* pItem)
{
   if (pItem != NULL && pItem->childCount() == 0)
   {
      mpValueType->setEnabled(true);
      mpDataUnits->setEnabled(true);
      mpData->setEnabled(true);
      DynamicObject* pDo = reinterpret_cast<DynamicObject*>(qvariant_cast<void*>(pItem->data(0, Qt::UserRole)));
      try
      {
         if (pDo != NULL)
         {
            Endian endian(BIG_ENDIAN_ORDER);
            unsigned int cols = dv_cast<unsigned int>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGMTXC));
            unsigned int rows = dv_cast<unsigned int>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGMTXR));
            std::string type = dv_cast<std::string>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGTYP));
            std::string units = dv_cast<std::string>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGDATU));
            unsigned int dts = dv_cast<unsigned int>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGDTS));
            unsigned int datc = dv_cast<unsigned int>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGDATC));
            if (dts == 0)
            {
               return;
            }
            const std::vector<unsigned char>& data2 = dv_cast<Blob>(pDo->getAttribute(Nitf::TRE::ENGRDA::ENGDATA));
            if (data2.empty())
            {
               return;
            }
            const void* pData = reinterpret_cast<const void*>(&data2.front());

            mpValueType->setText(QString("%1 byte %2").arg(dts).arg(QString::fromStdString(type)));
            mpDataUnits->setText(QString::fromStdString(units));
            mpData->setColumnCount(cols);
            mpData->setRowCount(rows);

            bool isAscii = false;
            EncodingType encoding;
            if (type == "S" && dts == 1)
            {
               encoding = INT1SBYTE;
            }
            else if (type == "S" && dts == 2)
            {
               encoding = INT2SBYTES;
            }
            else if (type == "S" && dts == 4)
            {
               encoding = INT4SBYTES;
            }
            else if (type == "R" && dts == 4)
            {
               encoding = FLT4BYTES;
            }
            else if (type == "R" && dts == 8)
            {
               encoding = FLT8BYTES;
            }
            else if (type == "C" && dts == 8)
            {
               encoding = FLT8COMPLEX;
            }
            else if (type == "I" && dts == 1)
            {
               encoding = INT1UBYTE;
            }
            else if (type == "I" && dts == 2)
            {
               encoding = INT2UBYTES;
            }
            else if (type == "I" && dts == 4)
            {
               encoding = INT4UBYTES;
            }
            else if (type == "A")
            {
               isAscii = true;
               mpData->setColumnCount(1);
               if (dts != 1)
               {
                  return;
               }
            }
            if (!encoding.isValid() && !isAscii)
            {
               // something is invalid
               mpData->setEnabled(false);
               return;
            }
            else if (datc != (rows * cols))
            {
               // can't handle multiple items per cell
               // I haven't actually seen this in real data
               // so we're likely safe ignoring this condition for now
               // The spec is unclear if this is even allowed but
               // we are checking it just to be safe.
               mpData->setEnabled(false);
               Service<DesktopServices>()->showMessageBox("Unable to display data",
                     "This ENGRDA contains more than one data item per matrix cell. This condition is not supported.",
                     "Ok");
               return;
            }
            for (size_t row = 0; row < rows ; ++row)
            {
               if (isAscii)
               {
                  // this is really a string per row.
                  size_t idx = row * cols;
                  const QByteArray buf(reinterpret_cast<const char*>(pData) + idx, cols);
                  QTableWidgetItem* pItem2 = new QTableWidgetItem(QString(buf));
                  mpData->setItem(row, 0, pItem2);
               }
               else 
               {
                  for (size_t col = 0; col < cols; ++col)
                  {
                     if (encoding == FLT8COMPLEX)
                     {
                        // Complex hasn't been tested due to lack of test data
                        //
                        size_t idx = (col * 2) + (row * cols * 2); // 2 floats per item
                        double real = std::numeric_limits<double>::quiet_NaN();
                        switchOnEncoding(FLT4BYTES, getDataValue, pData, idx, real, endian);
                        idx++;
                        double imag = std::numeric_limits<double>::quiet_NaN();
                        switchOnEncoding(FLT4BYTES, getDataValue, pData, idx, imag, endian);
                        QTableWidgetItem* pItem3 = new QTableWidgetItem(
                           QString::number(real, 'g', 12) + ", " + QString::number(imag, 'g', 12));
                        mpData->setItem(row, col, pItem3);
                     }
                     else
                     {
                        size_t idx = col + row * cols;
                        double val = std::numeric_limits<double>::quiet_NaN();
                        switchOnEncoding(encoding, getDataValue, pData, idx, val, endian);
                        QTableWidgetItem* pItem3 = new QTableWidgetItem(QString::number(val, 'g', 12));
                        mpData->setItem(row, col, pItem3);
                     }
                  }
               }
            }
         }
         return;
      }
      catch(const std::bad_cast&)
      {
      }
   }

   mpValueType->setText(QString());
   mpDataUnits->setText(QString());
   mpData->clear();
   mpValueType->setEnabled(false);
   mpDataUnits->setEnabled(false);
   mpData->setEnabled(false);
}

bool EngrdaWidget::canDisplayMetadata(const DynamicObject& treMetadata) const
{
   return true;
}
