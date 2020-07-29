/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef IMAGERESOLUTIONWIDGET_H
#define IMAGERESOLUTIONWIDGET_H

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class LabeledSection;

class ImageResolutionWidget : public QWidget
{
   Q_OBJECT

public:
   ImageResolutionWidget(QWidget* pParent = NULL);
   ~ImageResolutionWidget();

   void getResolution(unsigned int &width, unsigned int &height) const;
   void setResolution(unsigned int width, unsigned int height);
   bool getAspectRatioLock();
   void setAspectRatioLock(bool state);

signals:
   void ResolutionXChanged(const QString&);
   void ResolutionYChanged(const QString&);
   void aspectLockChanged(bool state);

private slots:
   void checkResolutionX(bool ignoreAspectRatio = false);
   void checkResolutionY(bool ignoreAspectRatio = false);

private:
   ImageResolutionWidget(const ImageResolutionWidget& rhs);
   ImageResolutionWidget& operator=(const ImageResolutionWidget& rhs);
   QLineEdit* mpResolutionX;
   QLineEdit* mpResolutionY;
   QPushButton* mpResolutionAspectLock;
   unsigned int mCurrentResolutionX;
   unsigned int mCurrentResolutionY;
   LabeledSection* mpSettingsSection;
   QIcon* mpLockIcon;
   QIcon* mpUnlockIcon;

};
#endif


