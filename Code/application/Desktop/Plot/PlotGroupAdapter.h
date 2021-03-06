/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PLOTGROUPADAPTER_H
#define PLOTGROUPADAPTER_H

#include "PlotGroup.h"
#include "PlotGroupImp.h"

class PlotGroupAdapter : public PlotGroup, public PlotGroupImp PLOTGROUPADAPTEREXTENSION_CLASSES
{
public:
   PlotGroupAdapter(PlotViewImp* pPlot, bool bPrimary);
   ~PlotGroupAdapter();

   // TypeAwareObject
   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   PLOTGROUPADAPTER_METHODS(PlotGroupImp)

private:
   PlotGroupAdapter(const PlotGroupAdapter& rhs);
};

#endif
