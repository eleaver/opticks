/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef FILLSTYLECOMBOBOX_H
#define FILLSTYLECOMBOBOX_H

#include "TypesFile.h"

#include <QtWidgets/QComboBox>

class FillStyleComboBox : public QComboBox
{
   Q_OBJECT

public:
   FillStyleComboBox(QWidget* pParent);
   void setCurrentValue(FillStyle value);
   FillStyle getCurrentValue() const;

signals: 
   void valueChanged(FillStyle value);

private:
   FillStyleComboBox(const FillStyleComboBox& rhs);
   FillStyleComboBox& operator=(const FillStyleComboBox& rhs);

private slots:
   void translateActivated(int newIndex);
};

#endif
