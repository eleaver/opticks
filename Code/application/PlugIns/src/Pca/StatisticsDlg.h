/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */



#ifndef STATISTICSDLG_H
#define STATISTICSDLG_H

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>

#include <vector>
#include <string>

class StatisticsDlg : public QDialog
{
   Q_OBJECT

public:
   StatisticsDlg(const QString& strCaption, const std::vector<std::string>& aoiNames, QWidget* parent = 0);
   ~StatisticsDlg();

   int getRowFactor() const;
   int getColumnFactor() const;
   QString getAoiName() const;

private:
   StatisticsDlg(const StatisticsDlg& rhs);
   StatisticsDlg& operator=(const StatisticsDlg& rhs);

   QRadioButton* mpFactorRadio;
   QSpinBox* mpRowSpin;
   QSpinBox* mpColumnSpin;
   QRadioButton* mpAoiRadio;
   QComboBox* mpAoiCombo;
};

#endif
