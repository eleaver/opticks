/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef OPTIONSLATLONLAYER_H
#define OPTIONSLATLONLAYER_H

#include "AppVersion.h"

#include <QtWidgets/QWidget>

class CustomColorButton;
class LatLonStyleComboBox;
class LineWidthComboBox;
class QComboBox;
class QFontComboBox;

class OptionsLatLonLayer : public QWidget
{
   Q_OBJECT

public:
   OptionsLatLonLayer();
   virtual ~OptionsLatLonLayer();

   void applyChanges();

   static const std::string& getName()
   {
      static std::string var = "Lat/Lon Layer Options";
      return var;
   }

   static const std::string& getOptionName()
   {
      static std::string var = "Layers/Lat//Lon";
      return var;
   }

   static const std::string& getDescription()
   {
      static std::string var = getShortDescription();
      return var;
   }

   static const std::string& getShortDescription()
   {
      static std::string var = "Widget to display latitude/longitude layer related options for the application";
      return var;
   }

   static const std::string& getCreator()
   {
      static std::string var = "Ball Aerospace & Technologies Corp.";
      return var;
   }

   static const std::string& getCopyright()
   {
      static std::string var = APP_COPYRIGHT_MSG;
      return var;
   }

   static const std::string& getVersion()
   {
      static std::string var = APP_VERSION_NUMBER;
      return var;
   }

   static bool isProduction()
   {
      return APP_IS_PRODUCTION_RELEASE;
   }

   static const std::string& getDescriptorId()
   {
      static std::string var = "{772CE114-8903-4625-AFA4-BEB28F4AB3A7}";
      return var;
   }

private:
   OptionsLatLonLayer(const OptionsLatLonLayer& rhs);
   OptionsLatLonLayer& operator=(const OptionsLatLonLayer& rhs);

   LatLonStyleComboBox* mpStyle;
   LineWidthComboBox* mpLineWidth;
   QComboBox* mpFontSize;
   QFontComboBox* mpFont;
   CustomColorButton* mpColor;
};

#endif
