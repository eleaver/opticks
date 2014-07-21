/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "ColormapEditor.h"
#include "CustomColorButton.h"
#include "HistogramPlotImp.h"
#include "RasterLayer.h"

#include <QtGui/QButtonGroup>
#include <QtGui/QColorDialog>
#include <QtGui/QFileDialog>
#include <QtGui/QFormLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>

using namespace std;

ColormapEditor::ColormapEditor(HistogramPlotImp& parent) :
   QDialog(&parent),
   mHistogram(parent),
   mIsApplied(false),
   mNeedDetach(true)
{
   setAttribute(Qt::WA_DeleteOnClose);
   setWindowTitle("Colormap Editor");
   setObjectName("ColorMapEditor");
   resize(QSize(396, 244).expandedTo(minimumSizeHint()));

   mHistogram.attach(SIGNAL_NAME(Subject, Deleted), Slot(this, &ColormapEditor::histogramDeleted));

   QVBoxLayout* pVboxLayout = new QVBoxLayout(this);
   pVboxLayout->setSpacing(6);
   pVboxLayout->setMargin(9);
   pVboxLayout->setObjectName("pVboxLayout");
   QHBoxLayout* pHboxLayout = new QHBoxLayout();
   pHboxLayout->setSpacing(6);
   pHboxLayout->setMargin(0);
   pHboxLayout->setObjectName("pHboxLayout");
   mpPrimariesLabel = new QLabel(this);
   mpPrimariesLabel->setObjectName("mpPrimariesLabel");
   mpPrimariesLabel->setText("Control-Colors:");

   pHboxLayout->addWidget(mpPrimariesLabel);

   mpIndicesLabel = new QLabel(this);
   mpIndicesLabel->setObjectName("mpIndicesLabel");
   mpIndicesLabel->setText("Gradient Steps:");

   pHboxLayout->addWidget(mpIndicesLabel);

   mpIndicesSpinBox = new QSpinBox(this);
   mpIndicesSpinBox->setObjectName("mpIndicesSpinBox");
   mpIndicesSpinBox->setMaximum(1000);
   mpIndicesSpinBox->setMinimum(2);
   mpIndicesSpinBox->setValue(mInitialIndices);

   pHboxLayout->addWidget(mpIndicesSpinBox);

   pVboxLayout->addLayout(pHboxLayout);

   mpTypeTab = new QTabWidget(this);
   
   // standard tab
   mpPrimaryView = new QWidget(mpTypeTab);
   mpTypeTab->addTab(mpPrimaryView, "Standard");

   mpPrimaryLayout = new QGridLayout(mpPrimaryView);
   mpPrimaryLayout->setColumnStretch(0, 1);

   mpPrimariesSpinBox = new QSpinBox(mpPrimaryView);
   mpPrimariesSpinBox->setMaximum(20);
   mpPrimariesSpinBox->setMinimum(2);
   mpPrimariesSpinBox->setValue(2);

   mpPrimaryLayout->addWidget(mpPrimariesSpinBox, 0, 0);

   mpUniformButton = new QPushButton(mpPrimaryView);
   mpUniformButton->setText("Distribute");

   mpPrimaryLayout->addWidget(mpUniformButton, 0, 1);

   // cubehelix tab
   mpCubehelixView = new QFrame(mpTypeTab);
   mpTypeTab->addTab(mpCubehelixView, "Cubehelix");

   QFormLayout* pCubehelixLayout = new QFormLayout(mpCubehelixView);

   mpCHStartColor = new QSlider(Qt::Horizontal, mpCubehelixView);
   mpCHStartColor->setRange(0,200);
   mpCHStartColor->setSingleStep(1);
   mpCHStartColor->setValue(50);
   mpCHStartColor->setStyleSheet("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 255, 255), stop:0.5 rgba(255, 0, 0, 255), stop:1 rgba(0, 255, 0, 255));");
   pCubehelixLayout->addRow("Start Color", mpCHStartColor);
   mpCHNumberOfRotations = new QDoubleSpinBox(mpCubehelixView);
   mpCHNumberOfRotations->setSingleStep(0.1);
   mpCHNumberOfRotations->setRange(-10.,10.);
   mpCHNumberOfRotations->setValue(-1.5);
   mpCHNumberOfRotations->setToolTip("Positive=red to blue");
   pCubehelixLayout->addRow("Number of Rotations", mpCHNumberOfRotations);
   mpCHGamma = new QDoubleSpinBox(mpCubehelixView);
   mpCHGamma->setMinimum(0.1);
   mpCHGamma->setSingleStep(0.1);
   mpCHGamma->setValue(1.0);
   pCubehelixLayout->addRow("\\gamma", mpCHGamma);
   QWidget* pCHDirectionWidget = new QWidget(mpCubehelixView);
   mpCHDirection = new QButtonGroup(mpCubehelixView);
   QRadioButton* pBlack = new QRadioButton("Black->White", pCHDirectionWidget);
   QRadioButton* pWhite = new QRadioButton("White->Black", pCHDirectionWidget);
   mpCHDirection->addButton(pBlack, 0);
   mpCHDirection->addButton(pWhite, 1);
   pBlack->setChecked(true);
   QHBoxLayout* pCHDirectionLayout = new QHBoxLayout(pCHDirectionWidget);
   pCHDirectionLayout->addWidget(pBlack);
   pCHDirectionLayout->addWidget(pWhite);
   pCubehelixLayout->addRow(pCHDirectionWidget);
   mpCHSaturation = new QDoubleSpinBox(mpCubehelixView);
   mpCHSaturation->setMinimum(0.0);
   mpCHSaturation->setSingleStep(0.1);
   mpCHSaturation->setValue(1.2);
   pCubehelixLayout->addRow("Saturation", mpCHSaturation);

   ////
   pVboxLayout->addWidget(mpTypeTab);

   QHBoxLayout* pRangeMinLayout = new QHBoxLayout;
   pVboxLayout->addLayout(pRangeMinLayout);
   mpLowerTransparent = new QPushButton(this);
   mpLowerTransparent->setIcon(QIcon(":/icons/Transparency"));
   mpLowerTransparent->setCheckable(true);
   mpLowerTransparent->setToolTip("Force lowest value to be transparent");
   pRangeMinLayout->addWidget(mpLowerTransparent, 0);
   mpRangeMinSlider = new QSlider(this);
   mpRangeMinSlider->setObjectName("mpRangeMinSlider");
   mpRangeMinSlider->setMaximum(mSliderRange-1);
   mpRangeMinSlider->setSingleStep(mSliderRange/100);
   mpRangeMinSlider->setPageStep(mSliderRange/10);
   mpRangeMinSlider->setOrientation(Qt::Horizontal);
   pRangeMinLayout->addWidget(mpRangeMinSlider, 10);

   mpDisplay = new QLabel(this);
   mpDisplay->setObjectName("mpDisplay");
   mpDisplay->setScaledContents(true);
   mpDisplay->setMinimumHeight(20);
   QSizePolicy sizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
   sizePolicy.setHorizontalStretch(0);
   sizePolicy.setVerticalStretch(0);
   sizePolicy.setHeightForWidth(mpDisplay->sizePolicy().hasHeightForWidth());
   mpDisplay->setSizePolicy(sizePolicy);

   pVboxLayout->addWidget(mpDisplay);

   QHBoxLayout* pRangeMaxLayout = new QHBoxLayout;
   pVboxLayout->addLayout(pRangeMaxLayout);
   mpRangeMaxSlider = new QSlider(this);
   mpRangeMaxSlider->setObjectName("mpRangeMaxSlider");
   mpRangeMaxSlider->setMaximum(mSliderRange-1);
   mpRangeMaxSlider->setSingleStep(mSliderRange/100);
   mpRangeMaxSlider->setPageStep(mSliderRange/10);
   mpRangeMaxSlider->setValue(mSliderRange-1);
   mpRangeMaxSlider->setOrientation(Qt::Horizontal);
   pRangeMaxLayout->addWidget(mpRangeMaxSlider, 10);
   mpUpperTransparent = new QPushButton(this);
   mpUpperTransparent->setIcon(QIcon(":/icons/Transparency"));
   mpUpperTransparent->setCheckable(true);
   mpUpperTransparent->setToolTip("Force highest value to be transparent");
   pRangeMaxLayout->addWidget(mpUpperTransparent, 0);

   QHBoxLayout* pHboxLayout1 = new QHBoxLayout();
   pHboxLayout1->setSpacing(6);
   pHboxLayout1->setMargin(0);
   pHboxLayout1->setObjectName("pHboxLayout1");
   mpSaveButton = new QPushButton(this);
   mpSaveButton->setObjectName("mpSaveButton");
   mpSaveButton->setText("Save");

   pHboxLayout1->addWidget(mpSaveButton);

   mpLoadButton = new QPushButton(this);
   mpLoadButton->setObjectName("mpLoadButton");
   mpLoadButton->setText("Load");

   pHboxLayout1->addWidget(mpLoadButton);

   mpApplyButton = new QPushButton(this);
   mpApplyButton->setObjectName("mpApplyButton");
   mpApplyButton->setText("Apply");

   pHboxLayout1->addWidget(mpApplyButton);

   mpSpacerItem1 = new QSpacerItem(16, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

   pHboxLayout1->addItem(mpSpacerItem1);

   mpOkButton = new QPushButton(this);
   mpOkButton->setObjectName("mpOkButton");
   mpOkButton->setText("OK");

   pHboxLayout1->addWidget(mpOkButton);

   mpCloseButton = new QPushButton(this);
   mpCloseButton->setObjectName("mpCloseButton");
   mpCloseButton->setText("Close");

   pHboxLayout1->addWidget(mpCloseButton);

   pVboxLayout->addLayout(pHboxLayout1);

   VERIFYNR(connect(mpOkButton, SIGNAL(clicked()), this, SLOT(accept())));
   VERIFYNR(connect(mpCloseButton, SIGNAL(clicked()), this, SLOT(reject())));
   VERIFYNR(connect(mpPrimariesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(numPrimariesChanged(int))));
   VERIFYNR(connect(mpIndicesSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpRangeMinSlider, SIGNAL(valueChanged(int)), this, SLOT(rangePositionChanged(int))));
   VERIFYNR(connect(mpRangeMaxSlider, SIGNAL(valueChanged(int)), this, SLOT(rangePositionChanged(int))));
   VERIFYNR(connect(mpCHStartColor, SIGNAL(valueChanged(int)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpCHNumberOfRotations, SIGNAL(valueChanged(double)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpCHGamma, SIGNAL(valueChanged(double)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpCHDirection, SIGNAL(buttonClicked(int)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpCHSaturation, SIGNAL(valueChanged(double)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpTypeTab, SIGNAL(currentChanged(int)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpApplyButton, SIGNAL(clicked()), this, SLOT(applyColormap())));
   VERIFYNR(connect(mpSaveButton, SIGNAL(clicked()), this, SLOT(saveColormap())));
   VERIFYNR(connect(mpLoadButton, SIGNAL(clicked()), this, SLOT(loadColormap())));
   VERIFYNR(connect(mpUniformButton, SIGNAL(clicked()), this, SLOT(distributeUniformly())));
   VERIFYNR(connect(mpLowerTransparent, SIGNAL(toggled(bool)), this, SLOT(updateColormap())));
   VERIFYNR(connect(mpUpperTransparent, SIGNAL(toggled(bool)), this, SLOT(updateColormap())));

   numPrimariesChanged(2);
}

ColormapEditor::~ColormapEditor()
{
   if (mNeedDetach)
   {
      mHistogram.detach(SIGNAL_NAME(Subject, Deleted), Slot(this, &ColormapEditor::histogramDeleted));
   }
   mHistogram.setAlternateColormap(NULL);
}

void ColormapEditor::numPrimariesChanged(int newCount)
{
   if (newCount == static_cast<int>(mPrimaries.size()))
   {
      return;
   }
   else if (newCount > static_cast<int>(mPrimaries.size()))
   {
      while (newCount > static_cast<int>(mPrimaries.size()))
      {
         QSlider* pSlider = new QSlider(mpPrimaryView);
         pSlider->setOrientation(Qt::Horizontal);
         pSlider->setMaximum(mSliderRange-1);
         pSlider->setSingleStep(mSliderRange/100);
         pSlider->setPageStep(mSliderRange/10);
         if (!mPrimaries.empty())
         {
            pSlider->setValue(mSliderRange-1);
         }
         CustomColorButton* pColorChip = new CustomColorButton(mpPrimaryView);

         QColor newColor = Qt::white;
         if (mPrimaries.empty())
         {
            newColor = Qt::black;
         }
         pColorChip->setColor(newColor);
         mpPrimaryLayout->addWidget(pSlider, mPrimaries.size()+1, 0);
         mpPrimaryLayout->addWidget(pColorChip, mPrimaries.size()+1, 1);
         Primary primary = { pColorChip, pSlider, pSlider->value(), newColor };
         mPrimaries.push_back(primary);
         connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(primaryPositionChanged(int)));
         connect(pColorChip, SIGNAL(colorChanged(const QColor&)), this, SLOT(primaryColorChanged(const QColor&)));
      }
   }
   else // newCount < mPrimaries.size()
   {
      int currentCount = static_cast<int>(mPrimaries.size());
      while (newCount < currentCount)
      {
         delete mPrimaries[currentCount-1].mpButton;
         delete mPrimaries[currentCount-1].mpSlider;
         --currentCount;
      }
      mPrimaries.resize(newCount);
   }
   updateColormap();
}

void ColormapEditor::applyColormap()
{
   if (!mIsApplied)
   {
      RasterLayer* pLayer = dynamic_cast<RasterLayer*>(mHistogram.getLayer());
      VERIFYNRV(pLayer != NULL);
      string name = "Custom";
      if (!mName.empty())
      {
         name = mName;
      }

      try
      {
         // Need a try/catch block because ColorMap constructor throws on failure.
         // Need to create a ColorMap instead of using mColormap because mColormap's
         // name does not match the name to use and ColorMap does not have a setName method.
         pLayer->setColorMap(ColorMap(name, mColormap.getTable()));
      }
      catch (const std::runtime_error&)
      {
         VERIFYNRV_MSG(false, "Invalid colormap creation attempted");
      }

      mIsApplied = true;
   }
}

void ColormapEditor::saveColormap()
{
   QString filename = QFileDialog::getSaveFileName(this, "Save Colormap", QString(), "*.cgr");
   QString mapName = filename;
   if (filename.isNull())
   {
      return;
   }

   if (filename.endsWith(".cgr"))
   {
      mapName = filename.mid(0, filename.lastIndexOf('.'));
   }
   else
   {
      filename.append(".cgr");
   }
   mName = mapName.mid(mapName.lastIndexOf('/') + 1, mapName.length()).toStdString();
   ColorMap cmap;
   if (mpTypeTab->currentWidget() == mpPrimaryView)
   {
      cmap = ColorMap(mName, makeGradient());
   }
   else if (mpTypeTab->currentWidget() == mpCubehelixView)
   {
      cmap = makeCubehelix(mName);
   }
   else
   {
      VERIFYNR_MSG(false, "Invalid color map creator tab");
   }
   cmap.saveToFile(filename.toStdString());
}

void ColormapEditor::loadColormap()
{
   QString filename = QFileDialog::getOpenFileName(this, "Load Colormap", QString(), "*.cgr");
   if (filename.isNull())
   {
      return;
   }

   ColorMap::Gradient gradient;
   try
   {
      ColorMap cmap(filename.toStdString());

      const ColorMap::Gradient* pGradient = cmap.getGradientDefinition();
      if (pGradient == NULL)
      {
         QString message = "The selected file:\n" + filename + "\ncould not be loaded as a color gradient file.";
         QMessageBox::critical(this, "Error Loading Colormap", message, "Ok");
         return;
      }
      gradient = *pGradient;
   }
   catch (std::runtime_error&)
   {
      QString message = "The selected file:\n" + filename + "\ncould not be loaded as a colormap file.";
      QMessageBox::critical(this, "Error Loading Colormap", message, "Ok");
      return;
   }


   mpIndicesSpinBox->setValue(gradient.mNumIndices);
   mpPrimariesSpinBox->setValue(gradient.mControls.size());
   mpLowerTransparent->setChecked(gradient.mLowerTransparent);
   mpUpperTransparent->setChecked(gradient.mUpperTransparent);

   vector<Primary>::iterator pPrimary;
   for (pPrimary = mPrimaries.begin(); pPrimary != mPrimaries.end(); ++pPrimary)
   {
      disconnect(pPrimary->mpSlider, SIGNAL(valueChanged(int)), this, SLOT(primaryPositionChanged(int)));
   }

   int rangeMin = (gradient.mStartPosition * mSliderRange + (gradient.mNumIndices-1)/2) / (gradient.mNumIndices-1);
   int rangeMax = (gradient.mStopPosition * mSliderRange + (gradient.mNumIndices-1)/2) / (gradient.mNumIndices-1);
   mpRangeMinSlider->setValue(rangeMin);
   mpRangeMaxSlider->setValue(rangeMax);
   int sliderIndexRange = gradient.mStopPosition - gradient.mStartPosition;
   for (unsigned int i = 0; i < gradient.mControls.size(); ++i)
   {
      mPrimaries[i].mColor = QColor(gradient.mControls[i].mColor.mRed,
         gradient.mControls[i].mColor.mGreen, gradient.mControls[i].mColor.mBlue);
      mPrimaries[i].mpButton->setColor(mPrimaries[i].mColor);
      int pos = mSliderRange * (gradient.mControls[i].mPosition - gradient.mStartPosition) / sliderIndexRange;
      mPrimaries[i].mValue = pos;
      mPrimaries[i].mpSlider->setValue(pos);
   }

   for (pPrimary = mPrimaries.begin(); pPrimary != mPrimaries.end(); ++pPrimary)
   {
      connect(pPrimary->mpSlider, SIGNAL(valueChanged(int)), this, SLOT(primaryPositionChanged(int)));
   }

   updateColormap();
}

void ColormapEditor::primaryColorChanged(const QColor& clrNew)
{
   CustomColorButton* pButton = dynamic_cast<CustomColorButton*>(sender());
   VERIFYNRV(pButton != NULL);

   unsigned int i = 0;
   for (i = 0; i < mPrimaries.size(); ++i)
   {
      if (pButton == mPrimaries[i].mpButton)
      {
         break;
      }
   }

   VERIFYNRV(i < mPrimaries.size());

   mPrimaries[i].mColor = clrNew;

   updateColormap();
}

void ColormapEditor::primaryPositionChanged(int newValue)
{
   QSlider* pThisSlider = dynamic_cast<QSlider*>(sender());
   VERIFYNRV(pThisSlider != NULL);
   QSlider* pPrevSlider = NULL;
   QSlider* pNextSlider = NULL;
   unsigned int i = 0;
   for (i = 0; i < mPrimaries.size(); ++i)
   {
      if (mPrimaries[i].mpSlider == pThisSlider)
      {
         if (i != 0)
         {
            pPrevSlider = mPrimaries[i-1].mpSlider;
         }

         if (i != mPrimaries.size()-1)
         {
            pNextSlider = mPrimaries[i+1].mpSlider;
         }

         break;
      }
   }

   VERIFYNRV(i < mPrimaries.size());

   if (pPrevSlider != NULL && pThisSlider->value() < pPrevSlider->value())
   {
      pThisSlider->setValue(pPrevSlider->value());
   }
   if (pNextSlider != NULL && pThisSlider->value() > pNextSlider->value())
   {
      pThisSlider->setValue(pNextSlider->value());
   }

   mPrimaries[i].mValue = pThisSlider->value();

   updateColormap();
}

void ColormapEditor::rangePositionChanged(int newValue)
{
   QSlider* pThisSlider = dynamic_cast<QSlider*>(sender());
   VERIFYNRV(pThisSlider != NULL);

   if (pThisSlider == mpRangeMinSlider)
   {
      if (newValue > mpRangeMaxSlider->value())
      {
         pThisSlider->setValue(mpRangeMaxSlider->value());
      }
   }
   else if (pThisSlider == mpRangeMaxSlider)
   {
      if (newValue < mpRangeMinSlider->value())
      {
         pThisSlider->setValue(mpRangeMinSlider->value());
      }
   }
   else
   {
      // Only the range sliders should be attached to this slot
      VERIFYNRV(false);
   }

   updateColormap();
}

void ColormapEditor::distributeUniformly()
{
   // first, set them all to max, in reverse order - this keeps 
   // primaryPositionChanged from interfering
   vector<Primary>::reverse_iterator pPrimaryRev;
   for (pPrimaryRev = mPrimaries.rbegin(); pPrimaryRev != mPrimaries.rend(); ++pPrimaryRev)
   {
      disconnect(pPrimaryRev->mpSlider, SIGNAL(valueChanged(int)), this, SLOT(primaryPositionChanged(int)));
      pPrimaryRev->mpSlider->setValue(mSliderRange-1);
   }

   int index = 0;
   int maxIndex = mPrimaries.size() - 1;
   int max2 = maxIndex / 2;
   vector<Primary>::iterator pPrimary;
   for (pPrimary = mPrimaries.begin(); pPrimary != mPrimaries.end(); ++pPrimary, ++index)
   {
      pPrimary->mpSlider->setValue(((mSliderRange - 1) * index + max2) / maxIndex);
      connect(pPrimary->mpSlider, SIGNAL(valueChanged(int)), this, SLOT(primaryPositionChanged(int)));
      pPrimary->mValue = pPrimary->mpSlider->value();
   }

   updateColormap();
}

void ColormapEditor::accept()
{
   applyColormap();
   QDialog::accept();
}

void ColormapEditor::updateColormap()
{
   try
   {
      if (mpTypeTab->currentWidget() == mpPrimaryView)
      {
         mColormap = ColorMap("Custom", makeGradient());
      }
      else if (mpTypeTab->currentWidget() == mpCubehelixView)
      {
         mColormap = makeCubehelix("Custom");
      }
      else
      {
         VERIFYNR_MSG(false, "Invalid color map creator tab");
      }
   }
   catch (const std::runtime_error&)
   {
      VERIFYNRV_MSG(false, "Invalid colormap creation attempted");
   }

   const vector<ColorType>& colorMap = mColormap.getTable();
   int size = colorMap.size();

   // Create the display of the colormap
   QImage image(size, 1, QImage::Format_ARGB32);
   vector<unsigned int> value(size);
   for (int i = 0; i < size; ++i)
   {
      value[i] = qRgb(colorMap[i].mRed, colorMap[i].mGreen, colorMap[i].mBlue);
   }
   memcpy(image.bits(), &value[0], size * sizeof(unsigned int));

   // scale it ourselves non-smoothly to defeat Qt's hard-coded smooth rescaling
   image = image.scaled(max(size, 500), mpDisplay->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
   mpDisplay->setPixmap(QPixmap::fromImage(image));

   // Instruct the histogram to display the prospective colormap
   mHistogram.setAlternateColormap(&mColormap);

   mIsApplied = false;
}

void ColormapEditor::histogramDeleted(Subject& subject, const std::string& signal, const boost::any& v)
{
   mNeedDetach = false;
   delete this;
}

ColorMap::Gradient ColormapEditor::makeGradient() const
{
   int size = mpIndicesSpinBox->value();
   ColorMap::Gradient gradient;
   gradient.mNumIndices = mpIndicesSpinBox->value();
   int rangeMin = mpRangeMinSlider->value();
   int rangeMax = mpRangeMaxSlider->value();
   int sliderRange = rangeMax-rangeMin;
   gradient.mStartPosition = (rangeMin * (size - 1) + mSliderRange / 2) / mSliderRange;
   gradient.mStopPosition = (rangeMax * (size - 1) + mSliderRange / 2) / mSliderRange;
   vector<Primary>::const_iterator pPrimary;
   for (pPrimary = mPrimaries.begin(); pPrimary != mPrimaries.end(); ++pPrimary)
   {
      int index = gradient.mStartPosition + 
         (pPrimary->mValue * (gradient.mStopPosition - gradient.mStartPosition) + sliderRange / 2) / mSliderRange;

      ColorMap::Gradient::Control control;
      control.mColor = ColorType(pPrimary->mColor.red(), pPrimary->mColor.green(), pPrimary->mColor.blue());
      control.mPosition = index;
      gradient.mControls.push_back(control);
   }
   gradient.mLowerTransparent = mpLowerTransparent->isChecked();
   gradient.mUpperTransparent = mpUpperTransparent->isChecked();

   return gradient;
}

ColorMap ColormapEditor::makeCubehelix(const std::string& name) const
{
   std::vector<ColorType> colors;
   double startColor = mpCHStartColor->value() / 100.;
   double rotations = mpCHNumberOfRotations->value();
   double gamma = mpCHGamma->value();
   int direction = mpCHDirection->checkedId();
   double saturation = mpCHSaturation->value();
   int count = std::max(256, mpIndicesSpinBox->value());

   int rangeMin = mpRangeMinSlider->value();
   int rangeMax = mpRangeMaxSlider->value();
   int sliderRange = rangeMax-rangeMin;
   int cmap_start = (rangeMin * (count - 1) + mSliderRange / 2) / mSliderRange;
   int cmap_stop = (rangeMax * (count - 1) + mSliderRange / 2) / mSliderRange;

   colors.resize(count);
   
   // Fill with gray
   for (int i = 0; i < count; ++i)
   {
      int shade = (i*255 + (count-1)/2)/(count-1);
      colors[i] = ColorType(shade, shade, shade);
   }

   count = cmap_stop-cmap_start+1;

   for (int i = 0; i < count; i++)
   {
      double fract = i / (double)count;
      double angle = 2. * 3.1415926 * (startColor / 3. + rotations * fract + 1.);
      fract = std::pow(fract, gamma);
      double amp = saturation * fract * (1. - fract) / 2.;

      double red = fract + amp * (-0.14861 * std::cos(angle) + 1.78277 * std::sin(angle));
      double green = fract + amp * (-0.29227 * std::cos(angle) - 0.90649 * std::sin(angle));
      double blue = fract + amp * (1.97294 * std::cos(angle));

      red = std::max(0., std::min(red, 1.));
      green = std::max(0., std::min(green, 1.));
      blue = std::max(0., std::min(blue, 1.));

      ColorType color(red*255, green*255,  blue*255);
      if (direction == 0) // black->white
      {
         colors[i+cmap_start] = color;
      }
      else // white->black
      {
         colors[cmap_stop-i] = color;
      }
   }
   if (mpLowerTransparent->isChecked())
   {
      colors.front().mAlpha = 0;
   }
   if (mpUpperTransparent->isChecked())
   {
      colors.back().mAlpha = 0;
   }

   return ColorMap("Custom", colors);
}