/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PLOTSETADAPTER_H
#define PLOTSETADAPTER_H

#include "PlotSet.h"
#include "PlotSetImp.h"

class PlotSetAdapter : public PlotSet, public PlotSetImp PLOTSETADAPTEREXTENSION_CLASSES
{
public:
   PlotSetAdapter(const std::string& id, const std::string& plotSetName, QWidget* pParent = NULL);
   virtual ~PlotSetAdapter();

   // TypeAwareObject
   virtual const std::string& getObjectType() const;
   virtual bool isKindOf(const std::string& className) const;

   PLOTSETADAPTER_METHODS(PlotSetImp)

private:
   PlotSetAdapter(const PlotSetAdapter& rhs);
   PlotSetAdapter& operator=(const PlotSetAdapter& rhs);
};

#endif
