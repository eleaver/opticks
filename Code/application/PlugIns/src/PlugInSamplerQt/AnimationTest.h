/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef ANIMATIONTEST_H
#define ANIMATIONTEST_H

#include <QtWidgets/QDialog>

#include "AnimationController.h"
#include "AttachmentPtr.h"
#include "ViewerShell.h"

#include <vector>

class Animation;
class QComboBox;
class QDateTimeEdit;
class QSpinBox;
class QStackedWidget;

class AnimationTestPlugIn : public ViewerShell
{
public:
   AnimationTestPlugIn();
   ~AnimationTestPlugIn();

   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);
   bool abort();

protected:
   QWidget* getWidget() const;

private:
   AnimationTestPlugIn(const AnimationTestPlugIn& rhs);
   AnimationTestPlugIn& operator=(const AnimationTestPlugIn& rhs);
   QDialog* mpDialog;
};

class AnimationTestDlg : public QDialog
{
   Q_OBJECT

public:
   AnimationTestDlg(PlugIn* pPlugIn, QWidget* pParent = 0);
   ~AnimationTestDlg();

   void animationAdded(Subject& subject, const std::string& signal, const boost::any& value);
   void animationRemoved(Subject& subject, const std::string& signal, const boost::any& value);
   void controllerDeleted(Subject& subject, const std::string& signal, const boost::any& value);

protected slots:
   void createController(QAction* pAction);
   void destroyController();
   void createAnimations();
   void clearAnimation();
   void destroyAnimation();
   void viewFrames();
   void viewAllFrames();
   void toggleTimeDisplay();
   void toggleCanDropFrames();
   void destroyAnimations();

protected:
   void closeEvent(QCloseEvent* pEvent);
   void viewFrames(const std::vector<Animation*>& animations);

private:
   AnimationTestDlg(const AnimationTestDlg& rhs);
   AnimationTestDlg& operator=(const AnimationTestDlg& rhs);
   PlugIn* mpPlugIn;
   AttachmentPtr<AnimationController> mpController;
   QSpinBox* mpNumAnimations;
   QSpinBox* mpNumFrames;
   QStackedWidget* mpStack;
   QDateTimeEdit* mpMinFrameTime;
   QDateTimeEdit* mpMaxFrameTime;
   QSpinBox* mpHoursSpin;
   QSpinBox* mpMinutesSpin;
   QSpinBox* mpSecondsSpin;
   QComboBox* mpAnimationList;
   unsigned int mAnimationNumber;
};

#endif
