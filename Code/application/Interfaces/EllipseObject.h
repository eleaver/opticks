/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef ELLIPSEOBJECT_H
#define ELLIPSEOBJECT_H

#include "GraphicObject.h"

/**
 * This class provides access to the display properties for an ellipse object.
 *
 * Possible GraphicObjectTypes: ELLIPSE_OBJECT.
 *
 *  This subclass of Subject will notify upon the following conditions:
 *  - All notifications documented in GraphicObject.
 */
class EllipseObject : public GraphicObject
{
protected:
   /**
    * This should be destroyed by calling GraphicLayer::removeObject.
    */
   virtual ~EllipseObject() {}
};

#endif
