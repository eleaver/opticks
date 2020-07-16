/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef BITRATEWIDGET_H
#define BITRATEWIDGET_H

#include <QtWidgets/QWidget>

class QLabel;
class QSlider;

class BitrateWidget : public QWidget
{
   Q_OBJECT

public:
   BitrateWidget(QWidget* pParent = NULL);
   ~BitrateWidget();

   void setBitrate(int bitrate);
   int getBitrate() const;

protected slots:
   void bitrateChanged(int value);

private:
   BitrateWidget(const BitrateWidget& rhs);
   BitrateWidget& operator=(const BitrateWidget& rhs);
   QSlider* mpBitrateSlider;
   QLabel* mpBitrateLabel;
};

#endif
