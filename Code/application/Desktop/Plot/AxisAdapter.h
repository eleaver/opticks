/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef AXISADAPTER_H
#define AXISADAPTER_H

#include "Axis.h"
#include "AxisImp.h"

class AxisAdapter : public Axis, public AxisImp
{
public:
   AxisAdapter(AxisPosition position, QWidget* pParent = 0);
   ~AxisAdapter();

   QWidget* getWidget();

   AXISADAPTER_METHODS(AxisImp)

private:
   AxisAdapter(const AxisAdapter& rhs);
   AxisAdapter& operator=(const AxisAdapter& rhs);
};

#endif
