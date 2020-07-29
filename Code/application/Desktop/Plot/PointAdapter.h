/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef POINTADAPTER_H
#define POINTADAPTER_H

#include "Point.h"
#include "PointImp.h"

class PointAdapter : public Point, public PointImp POINTADAPTEREXTENSION_CLASSES
{
public:
   PointAdapter(PlotViewImp* pPlot, bool bPrimary, bool bQuiet);
   PointAdapter(PlotViewImp* pPlot, bool bPrimary, LocationType point, bool bQuiet);
   PointAdapter(PlotViewImp* pPlot, bool bPrimary, double dX, double dY, bool bQuiet);
   ~PointAdapter();

   // TypeAwareObject
   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   POINTADAPTER_METHODS(PointImp)

private:
   PointAdapter(const PointAdapter& rhs);
};

#endif
