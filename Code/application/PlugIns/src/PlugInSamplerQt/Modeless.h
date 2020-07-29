/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef MODELESS_H
#define MODELESS_H

#include <memory>

#include <QtWidgets/QDialog>
#include <QtGui/QIcon>

#include "ViewerShell.h"

class ModelessPlugIn : public ViewerShell
{
public:
   ModelessPlugIn();
   ~ModelessPlugIn();

   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);
   bool abort();
   const QIcon& getIcon() const;

protected:
   QWidget* getWidget() const;

private:
   ModelessPlugIn(const ModelessPlugIn& rhs);
   ModelessPlugIn& operator=(const ModelessPlugIn& rhs);

   QDialog* mpDialog;
   int mRuns;
   bool mSessionClosed;
   mutable std::auto_ptr<QIcon> mMenuIcon;

   void aboutToClose(Subject& subject, const std::string& signal, const boost::any &args);
   void updateConfigSettings(Subject& subject, const std::string& signal, const boost::any &args);
   void sessionClosed(Subject& subject, const std::string& signal, const boost::any& value);
};


class ModelessDlg : public QDialog
{
   Q_OBJECT

public:
   ModelessDlg(PlugIn* pPlugIn, QWidget* pParent = 0);
   ~ModelessDlg();

protected:
   void closeEvent(QCloseEvent* pEvent);

private:
   ModelessDlg(const ModelessDlg& rhs);
   ModelessDlg& operator=(const ModelessDlg& rhs);
   PlugIn* mpPlugIn;
};

#endif
