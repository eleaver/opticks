/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PAPERSIZEDLG_H
#define PAPERSIZEDLG_H

#include <QDialog>
#include <QComboBox>
#include <QRadioButton>

class PaperSizeDlg : public QDialog
{
   Q_OBJECT

public:
   PaperSizeDlg(QWidget* pParent = 0);
   PaperSizeDlg(double dWidth, double dHeight, QWidget* pParent = 0);
   ~PaperSizeDlg();

   void getSize(double& dWidth, double& dHeight) const;

private:
   PaperSizeDlg(const PaperSizeDlg& rhs);
   PaperSizeDlg& operator=(const PaperSizeDlg& rhs);
   QComboBox* mpTypeCombo;
   QRadioButton* mpPortraitRadio;
   QRadioButton* mpLandscapeRadio;
};

#endif
