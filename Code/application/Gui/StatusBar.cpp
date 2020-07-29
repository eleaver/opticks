/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "StatusBar.h"
#include "ConfigurationSettings.h"
#include "DataVariant.h"
#include "Slot.h"
#include "Units.h"

#include <limits>
#include <string>
using namespace std;

StatusBar::StatusBar(QWidget* parent) :
   QStatusBar(parent),
   m_pResultValue_Label(NULL),
   m_pCubeValue_Label(NULL),
   m_pCoordinates_Label(NULL),
   m_pGeoCoordinates_Label(NULL),
   m_pRotation_Label(NULL),
   m_pElevation_Label(NULL)
{
   // Results Values
   m_pResultValue_Label = new QLabel(this);
   m_pResultValue_Label->setMinimumWidth(32);
   m_pResultValue_Label->setAlignment(Qt::AlignHCenter);
   m_pResultValue_Label->setToolTip("Results Values");
   addPermanentWidget(m_pResultValue_Label);

   // Cube Values
   m_pCubeValue_Label = new QLabel(this);
   m_pCubeValue_Label->setMinimumWidth(32);
   m_pCubeValue_Label->setAlignment(Qt::AlignHCenter);
   m_pCubeValue_Label->setToolTip("Data Values");
   addPermanentWidget(m_pCubeValue_Label);

   // Rotation Value
   m_pRotation_Label = new QLabel(this);
   m_pRotation_Label->setMinimumWidth(32);
   m_pRotation_Label->setAlignment(Qt::AlignHCenter);
   m_pRotation_Label->setToolTip("Rotation");
   addPermanentWidget(m_pRotation_Label);

   // Geographic Coordinates
   m_pGeoCoordinates_Label = new QLabel(this);
   m_pGeoCoordinates_Label->setMinimumWidth(32);
   m_pGeoCoordinates_Label->setAlignment(Qt::AlignHCenter);
   m_pGeoCoordinates_Label->setToolTip("Geocoordinates");
   addPermanentWidget(m_pGeoCoordinates_Label);

   // Pixel Coordinates
   m_pCoordinates_Label = NULL;
   m_pCoordinates_Label = new QLabel(this);
   m_pCoordinates_Label->setMinimumWidth(32);
   m_pCoordinates_Label->setAlignment(Qt::AlignHCenter);
   m_pCoordinates_Label->setToolTip("Pixel Coordinates");
   addPermanentWidget(m_pCoordinates_Label);

   // Elevation Information
   m_pElevation_Label = new QLabel(this);
   m_pElevation_Label->setMinimumWidth(32);
   m_pElevation_Label->setAlignment(Qt::AlignHCenter);
   m_pElevation_Label->setToolTip("Elevation");
   addPermanentWidget(m_pElevation_Label);

   // Initialization
   Service<ConfigurationSettings> pSettings;
   pSettings->attach(SIGNAL_NAME(ConfigurationSettings, SettingModified), Slot(this, &StatusBar::optionsModified));
}

StatusBar::~StatusBar()
{
   Service<ConfigurationSettings> pSettings;
   pSettings->detach(SIGNAL_NAME(ConfigurationSettings, SettingModified), Slot(this, &StatusBar::optionsModified));
}

void StatusBar::attached(Subject &subject, const string &signal, const Slot &slot)
{
   if (NN(dynamic_cast<ConfigurationSettings*>(&subject)))
   {
      updateDisplayedWidgets();
   }
}

void StatusBar::optionsModified(Subject& subject, const string& signal, const boost::any& value)
{
   if (NN(dynamic_cast<ConfigurationSettings*>(&subject)))
   {
      VERIFYNR(signal == SIGNAL_NAME(ConfigurationSettings, SettingModified));
      string key = boost::any_cast<string>(value);
      if (key.find("StatusBar/ShowStatusBar") == 0)
      {
         updateDisplayedWidgets();
      }
   }
}

bool StatusBar::arePixelCoordsShown()
{
   bool bShown = m_pCoordinates_Label->isVisible();
   return bShown;
}

bool StatusBar::areGeoCoordsShown()
{
   bool bShown = m_pGeoCoordinates_Label->isVisible();
   return bShown;
}

bool StatusBar::isCubeValueShown()
{
   bool bShown = m_pCubeValue_Label->isVisible();
   return bShown;
}

bool StatusBar::isElevationShown()
{
   bool bShown = m_pElevation_Label->isVisible();
   return bShown;
}

bool StatusBar::isResultValueShown()
{
   bool bShown = m_pResultValue_Label->isVisible();
   return bShown;
}

bool StatusBar::isRotationShown()
{
   bool bShown = m_pRotation_Label->isVisible();
   return bShown;
}

void StatusBar::setPixelCoords(int iXCoord, int iYCoord)
{
   QString strXCoord = QString::number(iXCoord + 1);   // Add 1 to account for zero-based coordinate
   QString strYCoord = QString::number(iYCoord + 1);
   m_pCoordinates_Label->setText(" Pixel: (" + strXCoord + ", " + strYCoord + ") ");
}

void StatusBar::setPixelCoords(const QPoint& ptPixel)
{
   int iX = ptPixel.x();
   int iY = ptPixel.y();

   setPixelCoords(iX, iY);
}

void StatusBar::clearPixelCoords()
{
   m_pCoordinates_Label->clear();
}

void StatusBar::showPixelCoords(bool bShow)
{
   m_pCoordinates_Label->setVisible(bShow);
}

void StatusBar::setGeoCoords(double xCoord, double yCoord, GeocoordType type, DmsFormatType format)
{
   LocationType coords(xCoord, yCoord);
   setGeoCoords(coords, type, format);
}

void StatusBar::setGeoCoords(LocationType latLonCoord, GeocoordType type, DmsFormatType format)
{
   string label = "";

   LatLonPoint latLonPoint(latLonCoord);
   if (type == GEOCOORD_LATLON)
   {
      label = " Geo: (" +
         (format == DMS_FULL_DECIMAL ? latLonPoint.getText(format, 6) : latLonPoint.getText(format)) + ")";
   }
   else if (type == GEOCOORD_UTM)
   {
      UtmPoint utmPoint(latLonPoint);
      label = " UTM: (" + utmPoint.getText() + ") ";
   }
   else if (type == GEOCOORD_MGRS)
   {
      MgrsPoint mgrsPoint(latLonPoint);
      label = " MGRS: (" + mgrsPoint.getText() + ") ";
   }

   m_pGeoCoordinates_Label->setText(QString::fromStdString(label));
}

void StatusBar::clearGeoCoords()
{
   m_pGeoCoordinates_Label->clear();
}

void StatusBar::showGeoCoords(bool bShow)
{
   m_pGeoCoordinates_Label->setVisible(bShow);
}

void StatusBar::setCubeValue(const QString& layerName, double gray, const Units* pUnits)
{
   QString unitText;
   if (pUnits != NULL)
   {
      unitText = QString::fromStdString(pUnits->getUnitName());
   }

   QString displayText = QString::number(gray, 'g', numeric_limits<double>::digits10);
   if (ConfigurationSettings::getSettingShowStatusBarCubeValueUnits())
   {
      displayText += " " + unitText;
   }
   setCubeValue(layerName, displayText);
}

void StatusBar::setCubeValue(const QString& layerName, const QString& strGray)
{
   QString nameText = " Raster";
   if (layerName.isEmpty() == false)
   {
      nameText += " (" + layerName + ")";
   }

   nameText += " - ";
   m_pCubeValue_Label->setText(nameText + "Gray: " + strGray + " ");
}

void StatusBar::setCubeValue(const QString& layerName, double red, double green, double blue,
   const Units* pRedUnits, const Units* pGreenUnits, const Units* pBlueUnits)
{
   string redText;
   string greenText;
   string blueText;

   if (pRedUnits != NULL)
   {
      redText = pRedUnits->getUnitName();
   }

   if (pGreenUnits != NULL)
   {
      greenText = pGreenUnits->getUnitName();
   }

   if (pBlueUnits != NULL)
   {
      blueText = pBlueUnits->getUnitName();
   }

   // if units available for all channels and they all have same unit name, set unit text
   QString unitText("Units vary by channel");
   if (!redText.empty()&& !greenText.empty() && !blueText.empty() && redText == greenText && greenText == blueText)
   {
      unitText = QString::fromStdString(redText);
   }

   QString displayText = QString::number(blue, 'g', numeric_limits<double>::digits10);
   if (ConfigurationSettings::getSettingShowStatusBarCubeValueUnits())
   {
      displayText += " " + unitText;
   }

   setCubeValue(layerName, QString::number(red, 'g', numeric_limits<double>::digits10),
      QString::number(green, 'g', numeric_limits<double>::digits10), displayText);
}

void StatusBar::setCubeValue(const QString& layerName, const QString& strRed, const QString& strGreen,
   const QString& strBlue)
{
   QString nameText = " Raster";
   if (layerName.isEmpty() == false)
   {
      nameText += " (" + layerName + ")";
   }

   nameText += " - ";
   m_pCubeValue_Label->setText(nameText + "R,G,B: " + strRed + ", " + strGreen + ", " + strBlue + " ");
}

void StatusBar::clearCubeValue()
{
   m_pCubeValue_Label->clear();
}

void StatusBar::showCubeValue(bool bShow)
{
   m_pCubeValue_Label->setVisible(bShow);
}

void StatusBar::setElevationValue(double value, const Units* pUnits)
{
   QString unitText;
   if (pUnits != NULL)
   {
      unitText = QString::fromStdString(pUnits->getUnitName()) + " ";
   }

   m_pElevation_Label->setText(" Elevation: " + QString::number(value) + " " + unitText);
}

void StatusBar::clearElevationValue()
{
   m_pElevation_Label->clear();
}

void StatusBar::showElevationValue(bool bShow)
{
   m_pElevation_Label->setVisible(bShow);
}

void StatusBar::setResultValue(const QString& layerName, double value, const Units* pUnits)
{
   QString nameText = " Result";
   if (layerName.isEmpty() == false)
   {
      nameText += " (" + layerName + ")";
   }

   nameText += ": ";

   QString unitText;
   if (pUnits != NULL)
   {
      unitText = QString::fromStdString(pUnits->getUnitName()) + " ";
   }

   m_pResultValue_Label->setText(nameText + QString::number(value) + " " + unitText);
}

void StatusBar::clearResultValue()
{
   m_pResultValue_Label->clear();
}

void StatusBar::showResultValue(bool bShow)
{
   m_pResultValue_Label->setVisible(bShow);
}

void StatusBar::setRotationValue(double value)
{
   m_pRotation_Label->setText(" Rotation: " + QString::number(value) + " ");
}

void StatusBar::clearRotationValue()
{
   m_pRotation_Label->clear();
}

void StatusBar::showRotationValue(bool bShow)
{
   m_pRotation_Label->setVisible(bShow);
}

void StatusBar::clearValues()
{
   clearPixelCoords();
   clearGeoCoords();
   clearCubeValue();
   clearElevationValue();
   clearResultValue();
   clearRotationValue();
}

void StatusBar::updateDisplayedWidgets()
{
   bool bPixelCoords = ConfigurationSettings::getSettingShowStatusBarPixelCoords();
   bool bGeoCoords = ConfigurationSettings::getSettingShowStatusBarGeoCoords();
   bool bCubeValue = ConfigurationSettings::getSettingShowStatusBarCubeValue();
   bool bResultValue = ConfigurationSettings::getSettingShowStatusBarResultValue();
   bool bRotationValue = ConfigurationSettings::getSettingShowStatusBarRotationValue();
   bool bElevationValue = ConfigurationSettings::getSettingShowStatusBarElevationValue();

   showPixelCoords(bPixelCoords);
   showGeoCoords(bGeoCoords);
   showCubeValue(bCubeValue);
   showResultValue(bResultValue);
   showRotationValue(bRotationValue);
   showElevationValue(bElevationValue);
}
