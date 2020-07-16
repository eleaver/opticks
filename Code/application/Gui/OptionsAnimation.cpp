/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AnimationController.h"
#include "AnimationCycleButton.h"
#include "AnimationToolBar.h"
#include "LabeledSection.h"
#include "OptionsAnimation.h"

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLayout>

#include <algorithm>

using namespace std;

OptionsAnimation::OptionsAnimation() :
   QWidget(NULL)
{
   // Animation
   mpCanDropFrames = new QCheckBox("Can Drop Frames", this);
   mpCanDropFrames->setStatusTip("Determines if frames should be dropped to preserve the frame speed");
   mpCanDropFrames->setToolTip("Determines if frames should be dropped to preserve the frame speed");
   mpCanDropFrames->setChecked(AnimationController::getSettingCanDropFrames());

   QLabel* pAnimationCycleSettingLabel = new QLabel("Default Animation Cycle:", this);
   mpCycle = new AnimationCycleButton(this);
   mpCycle->setStatusTip("Specifies the play behavior when the end of the animation is reached");
   mpCycle->setToolTip("Animation Cycle");
   mpCycle->setCurrentValue(AnimationController::getSettingAnimationCycleSelection());

   mpResetOnStop = new QCheckBox("Reset on Stop", this);
   mpResetOnStop->setStatusTip("If checked, resets the animation to the first frame when the "
      "animation is stopped.  If unchecked, remains on the current frame when the animation is stopped.");
   mpResetOnStop->setToolTip("If checked, resets the animation to the first frame when the "
      "animation is stopped.  If unchecked, remains on the current frame when the animation is stopped.");
   mpResetOnStop->setChecked(AnimationController::getSettingResetOnStop());

   mpConfirmDelete = new QCheckBox("Confirm delete", this);
   mpConfirmDelete->setToolTip("Displays a dialog to confirm deleting selected animation controllers in "
      "the Session Explorer.");
   mpConfirmDelete->setChecked(AnimationController::getSettingConfirmDelete());

   mpFrameSpeedList = new QListWidget(this);
   QLabel* pFrameSpeedLabel = new QLabel("Default Frame Speeds:", this);
   mFrameSpeeds = AnimationToolBar::getSettingFrameSpeeds();
   for (vector<double>::iterator iter = mFrameSpeeds.begin(); iter < mFrameSpeeds.end(); ++iter)
   {
      mpFrameSpeedList->addItem(QString::number(*iter));
   }

   QWidget* pAnimationLayoutWidget = new QWidget();
   QPushButton* pAddButton = new QPushButton("Add", pAnimationLayoutWidget);
   QPushButton* pRemoveButton = new QPushButton("Remove", pAnimationLayoutWidget);

   QGridLayout* pAnimationLayout = new QGridLayout(pAnimationLayoutWidget);
   pAnimationLayout->setMargin(0);
   pAnimationLayout->setSpacing(5);
   pAnimationLayout->addWidget(mpCanDropFrames, 0, 0, 1, 2);
   pAnimationLayout->addWidget(mpResetOnStop, 1, 0, 1, 2);
   pAnimationLayout->addWidget(mpConfirmDelete, 2, 0, 1, 2);
   pAnimationLayout->addWidget(pAnimationCycleSettingLabel, 3, 0);
   pAnimationLayout->addWidget(mpCycle, 3, 1, Qt::AlignLeft);
   pAnimationLayout->addWidget(pFrameSpeedLabel, 4, 0, 1, 2);
   pAnimationLayout->addWidget(mpFrameSpeedList, 5, 0, 2, 2);
   pAnimationLayout->addWidget(pAddButton, 5, 2);
   pAnimationLayout->addWidget(pRemoveButton, 6, 2, Qt::AlignTop);
   pAnimationLayout->setColumnStretch(1, 20);
   pAnimationLayout->setRowStretch(6, 20);
   LabeledSection* pAnimationSection = new LabeledSection(pAnimationLayoutWidget, "Animation", this);

   VERIFYNR(connect(pAddButton, SIGNAL(clicked()), this, SLOT(addFrameSpeed())));
   VERIFYNR(connect(pRemoveButton, SIGNAL(clicked()), this, SLOT(removeFrameSpeed())));
   VERIFYNR(connect(mpFrameSpeedList, SIGNAL(itemChanged(QListWidgetItem*)), this,
      SLOT(editFrameSpeedFinished(QListWidgetItem*))));

   // Dialog layout
   QVBoxLayout* pLayout = new QVBoxLayout(this);
   pLayout->setMargin(0);
   pLayout->setSpacing(10);
   pLayout->addWidget(pAnimationSection, 1000);
   pLayout->addStretch(1);
}

OptionsAnimation::~OptionsAnimation()
{}

void OptionsAnimation::applyChanges()
{
   AnimationController::setSettingCanDropFrames(mpCanDropFrames->isChecked());
   AnimationController::setSettingResetOnStop(mpResetOnStop->isChecked());
   AnimationController::setSettingConfirmDelete(mpConfirmDelete->isChecked());
   AnimationController::setSettingAnimationCycleSelection(mpCycle->getCurrentValue());
   AnimationToolBar::setSettingFrameSpeeds(mFrameSpeeds);
}

void OptionsAnimation::addFrameSpeed()
{
   mpFrameSpeedList->addItem(QString());
   mpFrameSpeedList->setCurrentRow(mpFrameSpeedList->count() - 1);
   mpFrameSpeedList->openPersistentEditor(mpFrameSpeedList->currentItem());
   mpFrameSpeedList->editItem(mpFrameSpeedList->currentItem());
}

void OptionsAnimation::editFrameSpeedFinished(QListWidgetItem *pCurrentItem)
{
   if (pCurrentItem == NULL)
   {
      return;
   }
   mpFrameSpeedList->closePersistentEditor(pCurrentItem);
   vector<double>::iterator iter; 

   iter = std::find(mFrameSpeeds.begin(), mFrameSpeeds.end(), pCurrentItem->text().toDouble());
   if (iter == mFrameSpeeds.end() && pCurrentItem->text().toDouble() > 0.0)
   {
      mFrameSpeeds.push_back(pCurrentItem->text().toDouble());

      sort(mFrameSpeeds.begin(), mFrameSpeeds.end());
      mpFrameSpeedList->clear();
      for (iter = mFrameSpeeds.begin(); iter < mFrameSpeeds.end(); ++iter)
      {
         mpFrameSpeedList->addItem(QString::number(*iter));
      }
   }
   else
   {
      mpFrameSpeedList->takeItem(mpFrameSpeedList->row(pCurrentItem));
   }
}

void OptionsAnimation::removeFrameSpeed()
{
   if (mpFrameSpeedList->count() > 0 && mpFrameSpeedList->currentItem() != NULL)
   {
      if (mpFrameSpeedList->currentItem()->text().toDouble() > 0.0)  
      {
         mFrameSpeeds.erase(std::find(mFrameSpeeds.begin(), mFrameSpeeds.end(), 
                            mpFrameSpeedList->currentItem()->text().toDouble()));
         mpFrameSpeedList->takeItem(mpFrameSpeedList->row(mpFrameSpeedList->currentItem()));
      }
   }
}
