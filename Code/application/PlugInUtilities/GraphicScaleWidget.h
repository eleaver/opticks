/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GRAPHICSCALEWIDGET_H
#define GRAPHICSCALEWIDGET_H

#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QWidget>

class GraphicScaleWidget : public QWidget
{
   Q_OBJECT

public:
   GraphicScaleWidget(QWidget* pParent = NULL);
   ~GraphicScaleWidget();

   double getScale() const;

public slots:
   void setScale(double scale);

signals:
   void scaleChanged(double scale);

private:
   GraphicScaleWidget(const GraphicScaleWidget& rhs);
   GraphicScaleWidget& operator=(const GraphicScaleWidget& rhs);
   QDoubleSpinBox* mpScaleSpin;
};

#endif
