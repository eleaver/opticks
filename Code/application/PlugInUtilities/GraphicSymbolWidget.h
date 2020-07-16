/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GRAPHICSYMBOLWIDGET_H
#define GRAPHICSYMBOLWIDGET_H

#include <QtCore/QString>
#include <QtWidgets/QWidget>
#include <string>

class QLabel;
class QListWidget;
class QListWidgetItem;
class QSpinBox;

class GraphicSymbolWidget : public QWidget
{
   Q_OBJECT

public:
   GraphicSymbolWidget(QWidget* pParent = NULL);
   ~GraphicSymbolWidget();

   void setSymbolName(const QString &name);
   QString getSymbolName() const;

   void setSymbolSize(unsigned int size);
   unsigned int getSymbolSize() const;

protected slots:
   void updateSelection();
   void notifySizeChange(int size);

signals:
   void nameChanged(const QString& name);
   void sizeChanged(unsigned int size);

private:
   GraphicSymbolWidget(const GraphicSymbolWidget& rhs);
   GraphicSymbolWidget& operator=(const GraphicSymbolWidget& rhs);

   static const QString NO_SYMBOL;

   QListWidgetItem *getSelectedItem() const;

   QListWidget* mpSymbolList;
   QLabel* mpSymbolPreview;
   QSpinBox* mpSymbolSizeSpin;
};

#endif
