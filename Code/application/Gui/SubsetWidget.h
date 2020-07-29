/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SUBSETWIDGET_H
#define SUBSETWIDGET_H

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpinBox>
#include <QtCore/QStringListModel>
#include <QtWidgets/QWidget>

#include "DimensionDescriptor.h"

#include <vector>

class QRadioButton;

class SubsetWidget : public QWidget
{
   Q_OBJECT

public:
   SubsetWidget(QWidget* pParent = NULL);
   ~SubsetWidget();

   void setExportMode(bool enableExportMode);

   void setRows(const std::vector<DimensionDescriptor>& rows,
                const std::vector<DimensionDescriptor>& selectedRows);
   const std::vector<DimensionDescriptor>& getRows() const;
   std::vector<DimensionDescriptor> getSubsetRows() const;
   unsigned int getSubsetRowCount() const;
   unsigned int getSubsetRowSkipFactor() const;

   void setColumns(const std::vector<DimensionDescriptor>& columns,
                   const std::vector<DimensionDescriptor>& selectedColumns);
   const std::vector<DimensionDescriptor>& getColumns() const;
   std::vector<DimensionDescriptor> getSubsetColumns() const;
   unsigned int getSubsetColumnCount() const;
   unsigned int getSubsetColumnSkipFactor() const;

   void setBands(const std::vector<DimensionDescriptor>& bands,
      const std::vector<std::string>& bandNames = std::vector<std::string>(),
      const std::vector<DimensionDescriptor>& selectedBands = std::vector<DimensionDescriptor>());
   const std::vector<DimensionDescriptor>& getBands() const;
   std::vector<DimensionDescriptor> getSubsetBands() const;
   unsigned int getSubsetBandCount() const;

   void setBadBandFileDirectory(const QString& strDirectory);

   QSize sizeHint() const;

signals:
   void subsetRowsChanged(const std::vector<DimensionDescriptor>& rows);
   void subsetColumnsChanged(const std::vector<DimensionDescriptor>& columns);
   void subsetBandsChanged(const std::vector<DimensionDescriptor>& bands);
   void modified();

protected slots:
   void customBandSelection();

private:
   SubsetWidget(const SubsetWidget& rhs);
   SubsetWidget& operator=(const SubsetWidget& rhs);
   bool compareDimensionDescriptors(DimensionDescriptor left, DimensionDescriptor right);

   bool mExportMode;
   QComboBox* mpStartRowCombo;
   QComboBox* mpEndRowCombo;
   QSpinBox* mpRowSkipSpin;
   QStringListModel* mpRowModel;

   QComboBox* mpStartColumnCombo;
   QComboBox* mpEndColumnCombo;
   QSpinBox* mpColumnSkipSpin;
   QStringListModel* mpColumnModel;

   QListView* mpBandList;
   QStringListModel* mpBandModel;
   QString mBadBandFileDir;

   std::vector<DimensionDescriptor> mRows;
   std::vector<DimensionDescriptor> mColumns;
   std::vector<DimensionDescriptor> mBands;

private slots:
   void notifyOfRowsChange();
   void notifyOfColumnsChange();
   void notifyOfBandsChange();
};

//private QDialog sub-class to allow user to perform custom band selections
class BandCustomSelectionDialog : public QDialog
{
   Q_OBJECT

public:
   BandCustomSelectionDialog(QWidget* pParent, QStringListModel* pBandList, QString badBandsDir);
   virtual ~BandCustomSelectionDialog();

   bool isSubsetSelected() const;
   QString getBadBandFile() const;
   void setStartBandIndex(int index);
   int getStartBandIndex() const;
   void setStopBandIndex(int index);
   int getStopBandIndex() const;
   void setSkipFactor(int skipFactor);
   int getSkipFactor() const;

private slots:
   void selectBadBandFile();
   void selectionChanged();

private:
   BandCustomSelectionDialog(const BandCustomSelectionDialog& rhs);
   BandCustomSelectionDialog& operator=(const BandCustomSelectionDialog& rhs);

   QComboBox* mpStartBandCombo;
   QComboBox* mpEndBandCombo;
   QSpinBox* mpBandSkipSpin;
   QLineEdit* mpBadBandFile;
   QPushButton* mpBadBandFileBtn;

   QRadioButton* mpSubsetSelection;
   QRadioButton* mpBadBandsSelection;

   QStringListModel* mpBandListModel;
   QString mBadBandDir;
};

#endif
