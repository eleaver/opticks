/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef LINKOPTIONSPAGE_H
#define LINKOPTIONSPAGE_H

#include "TypesFile.h"

#include <QtCore/QMap>
#include <QtWidgets/QWidget>

#include <vector>

class Layer;
class QCheckBox;
class QRadioButton;
class QStackedWidget;
class QTreeWidget;
class QTreeWidgetItem;
class View;

class LinkOptionsPage : public QWidget
{
   Q_OBJECT

public:
   LinkOptionsPage(QWidget* pParent = NULL);
   virtual ~LinkOptionsPage();

   // Views
   std::vector<View*> getViewLinks() const;
   LinkType getLinkType() const;
   bool isTwoWayLink() const;

   // Layers
   std::vector<Layer*> getLayerLinks() const;
   bool areLayersDuplicated() const;

public slots:
   void setLinkObjects(const QString& strDataset);

protected slots:
   void activateOptions();

private:
   LinkOptionsPage(const LinkOptionsPage& rhs);
   LinkOptionsPage& operator=(const LinkOptionsPage& rhs);

   QTreeWidget* mpLinksTree;
   QMap<QTreeWidgetItem*, View*> mViews;
   QMap<QTreeWidgetItem*, Layer*> mLayers;

   QString mstrDataset;

   QStackedWidget* mpStack;
   QWidget* mpViewWidget;
   QRadioButton* mpAutolinkRadio;
   QRadioButton* mpMirrorRadio;
   QRadioButton* mpGeoRadio;
   QRadioButton* mpUnlinkRadio;
   QCheckBox* mpTwoWayCheck;
   QWidget* mpLayerWidget;
   QCheckBox* mpDuplicateCheck;
};

#endif
