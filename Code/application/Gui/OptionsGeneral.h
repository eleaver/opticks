/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef OPTIONSGENERAL_H
#define OPTIONSGENERAL_H

#include <QtWidgets/QWidget>

#include "AppVersion.h"

class QCheckBox;
class QComboBox;
class QRadioButton;
class QSpinBox;

class OptionsGeneral : public QWidget
{
   Q_OBJECT

public:
   OptionsGeneral();
   ~OptionsGeneral();

   void applyChanges();

   static const std::string& getName()
   {
      static std::string var = "General Options";
      return var;
   }

   static const std::string& getOptionName()
   {
      static std::string var = "General";
      return var;
   }

   static const std::string& getDescription()
   {
      static std::string var = "Widget to display general options for the application";
      return var;
   }

   static const std::string& getShortDescription()
   {
      static std::string var = "Widget to display general options for the application";
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
      static std::string var = "{028D6D49-3F98-4432-8FF2-68E33AA0DF00}";
      return var;
   }

private:
   OptionsGeneral(const OptionsGeneral& rhs);
   OptionsGeneral& operator=(const OptionsGeneral& rhs);
   QSpinBox* mpMruFilesSpin;
   QSpinBox* mpBufferSpin;
   QSpinBox* mpThreadSpin;
   QRadioButton* mpStatsAutoRadio;
   QRadioButton* mpStatsFullRadio;
   QRadioButton* mpStatsSkipRadio;
   QSpinBox* mpStatsSkipSpin;
   QCheckBox* mpProgressClose;
   QCheckBox* mpMouseWheelZoom;
   QComboBox* mpClassificationPositionPlotView;
   QComboBox* mpClassificationPositionProductView;
   QComboBox* mpClassificationPositionSpatialView;
};

#endif
