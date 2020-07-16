/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef METADATAFILTERDLG_H
#define METADATAFILTERDLG_H

#include <QtCore/QRegExp>
#include <QtCore/QString>
#include <QtWidgets/QDialog>

class FilterWidget;
class QLineEdit;
class QShowEvent;

class MetadataFilterDlg : public QDialog
{
   Q_OBJECT

public:
   MetadataFilterDlg(QWidget* pParent = NULL);
   virtual ~MetadataFilterDlg();

   void setFilterName(const QString& name);
   void setNameFilter(const QRegExp& filter);
   void setValueFilter(const QRegExp& filter);

   QString getFilterName() const;
   QRegExp getNameFilter() const;
   QRegExp getValueFilter() const;

public slots:
   virtual void accept();

protected:
   void showEvent(QShowEvent* pEvent);

private:
   MetadataFilterDlg(const MetadataFilterDlg& rhs);
   MetadataFilterDlg& operator=(const MetadataFilterDlg& rhs);
   QLineEdit* mpFilterNameEdit;
   FilterWidget* mpNameFilter;
   FilterWidget* mpValueFilter;
};

#endif
