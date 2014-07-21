/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef COLORMAPEDITOR_H
#define COLORMAPEDITOR_H

#include "ColorMap.h"

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <string>
#include <vector>

class CustomColorButton;
class HistogramPlotImp;
class Subject;
class QGridLayout;
class QTabWidget;

namespace boost
{
   class any;
}

class ColormapEditor : public QDialog
{
   Q_OBJECT

public:
   ColormapEditor(HistogramPlotImp &parent);
   ~ColormapEditor();

public slots:
   void accept();
   void distributeUniformly();
   void primaryPositionChanged(int newValue);
   void rangePositionChanged(int newValue);
   void saveColormap();
   void loadColormap();
   void applyColormap();

private slots:
   void updateColormap();
   void numPrimariesChanged(int newCount);
   void primaryColorChanged(const QColor &clrNew);

private:
   ColormapEditor(const ColormapEditor& rhs);
   ColormapEditor& operator=(const ColormapEditor& rhs);
   void histogramDeleted(Subject &subject, const std::string &signal, const boost::any& v);
   ColorMap::Gradient makeGradient() const;
   ColorMap makeCubehelix(const std::string& name) const;

   QTabWidget* mpTypeTab;

   QLabel* mpPrimariesLabel;
   QSpinBox* mpPrimariesSpinBox;
   QLabel* mpIndicesLabel;
   QSpinBox* mpIndicesSpinBox;
   QGridLayout* mpPrimaryLayout;
   QWidget* mpPrimaryView;
   QWidget* mpCubehelixView;
   QSlider* mpRangeMinSlider;
   QLabel* mpDisplay;
   QSlider* mpRangeMaxSlider;
   QPushButton* mpSaveButton;
   QPushButton* mpApplyButton;
   QPushButton* mpLoadButton;
   QSpacerItem* mpSpacerItem1;
   QPushButton* mpOkButton;
   QPushButton* mpCloseButton;
   QPushButton* mpUniformButton;
   QPushButton* mpLowerTransparent;
   QPushButton* mpUpperTransparent;

   QSlider* mpCHStartColor;
   QDoubleSpinBox* mpCHNumberOfRotations;
   QDoubleSpinBox* mpCHGamma;
   QButtonGroup* mpCHDirection;
   QDoubleSpinBox* mpCHSaturation;

   struct Primary
   {
      CustomColorButton* mpButton;
      QSlider* mpSlider;
      int mValue;
      QColor mColor;
   };
   std::vector<Primary> mPrimaries;
   HistogramPlotImp& mHistogram;
   static const int mInitialIndices = 250;
   static const int mSliderRange = 10000;
   ColorMap mColormap;
   std::string mName;
   bool mIsApplied;
   bool mNeedDetach;
};
#endif
