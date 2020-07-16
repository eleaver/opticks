/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AppVerify.h"
#include "DmsFormatTypeComboBox.h"
#include "StringUtilities.h"

#include <string>

DmsFormatTypeComboBox::DmsFormatTypeComboBox(QWidget* pParent) :
   QComboBox(pParent)
{
   setEditable(false);

   addItem(QString::fromStdString(StringUtilities::toDisplayString(DMS_FULL)));
   addItem(QString::fromStdString(StringUtilities::toDisplayString(DMS_FULL_DECIMAL)));
   addItem(QString::fromStdString(StringUtilities::toDisplayString(DMS_MINUTES_DECIMAL)));

   VERIFYNR(connect(this, SIGNAL(currentIndexChanged(const QString&)), this,
      SLOT(translateIndexChanged(const QString&))));
}

DmsFormatTypeComboBox::~DmsFormatTypeComboBox()
{}

void DmsFormatTypeComboBox::setCurrentValue(DmsFormatType value)
{
   if (value != getCurrentValue())
   {
      QString strValue = QString::fromStdString(StringUtilities::toDisplayString(value));
      int index = findText(strValue);
      setCurrentIndex(index);
   }
}

DmsFormatType DmsFormatTypeComboBox::getCurrentValue() const
{
   if (currentIndex() != -1)
   {
      std::string curText = currentText().toStdString();
      if (curText.empty() == false)
      {
         return StringUtilities::fromDisplayString<DmsFormatType>(curText);
      }
   }

   return DmsFormatType();
}

void DmsFormatTypeComboBox::translateIndexChanged(const QString& text)
{
   DmsFormatType formatType;
   if (text.isEmpty() == false)
   {
      formatType = StringUtilities::fromDisplayString<DmsFormatType>(text.toStdString());
   }

   emit valueChanged(formatType);
}
