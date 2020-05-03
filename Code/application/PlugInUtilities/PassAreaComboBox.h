/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PASSAREACOMBOBOX_H
#define PASSAREACOMBOBOX_H

#include "TypesFile.h"

#include <QComboBox>

class PassAreaComboBox : public QComboBox
{
   Q_OBJECT

public:
   PassAreaComboBox(QWidget* pParent);
   void setCurrentValue(PassArea value);
   PassArea getCurrentValue() const;

signals: 
   void valueChanged(PassArea value);

private:
   PassAreaComboBox(const PassAreaComboBox& rhs);
   PassAreaComboBox& operator=(const PassAreaComboBox& rhs);

private slots:
   void translateActivated(int newIndex);
};

#endif
