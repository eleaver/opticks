/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef FLICKERCONTROLS_H
#define FLICKERCONTROLS_H

#include "DockWindowShell.h"

class FlickerControls : public DockWindowShell
{
public:
   FlickerControls();
   virtual ~FlickerControls();

   virtual bool serialize(SessionItemSerializer& serializer) const;
   virtual bool deserialize(SessionItemDeserializer& deserializer);

protected:
   QAction* createAction();
   QWidget* createWidget();

private:
   FlickerControls(const FlickerControls& rhs);
   FlickerControls& operator=(const FlickerControls& rhs);
};

#endif
