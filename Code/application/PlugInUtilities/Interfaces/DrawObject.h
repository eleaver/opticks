/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H

#include "LocationType.h"
#include "SubjectAdapter.h"

#include <QtCore/qnamespace.h>
#include <QtCore/QPoint>

#include <vector>

class CustomLayer;

/**
 *  Performs custom drawing in a custom layer.
 *
 *  A custom layer provides the capability for a developer plug-in to draw in a spatial data view.
 *  This class provides the drawing instructions for rendering in a custom layer.
 *
 *  This subclass of SubjectAdapter will notify upon the following conditions:
 *  - The following method is called: setLayer().
 *  - Everything else documented in Subject.
 *
 *  @see     CustomLayer
 */
class DrawObject : public SubjectAdapter
{
public:
   /**
    *  Emitted with boost::any<\link CustomLayer\endlink*> when the layer object is changed.
    */
   SIGNAL_METHOD(DrawObject, LayerChanged)

   /**
    *  Default constructor.
    */
   DrawObject();

   /**
    *  Destroys the draw object.
    *
    *  The destructor resets the layer by calling setLayer(), passing in a
    *  \c NULL value.  The DrawObject for a CustomLayer is automatically
    *  destroyed when the layer is destroyed.
    *
    *  @notify  The destructor notifies signalDeleted() after the layer has
    *           been reset.
    */
   virtual ~DrawObject();

   /**
    *  This method sets the custom layer in which the draw object will execute its drawing instructions.
    *
    *  @param   pLayer
    *           The custom layer in which the draw object will draw.
    *
    *  @note    The custom layer will take ownership of the draw object.
    *           If the layer is destroyed, it will destroy the draw object.
    *
    *  @notify  This method will notify signalLayerChanged() with boost::any<\link CustomLayer\endlink*>.
    */
   void setLayer(CustomLayer* pLayer);

   /**
    *  This method returns a pointer to the custom layer.
    *
    *  @return  The custom layer.
    *
    *  @see     setLayer()
    */
   CustomLayer* getLayer();

   /**
    *  This method returns a const pointer to the custom layer.
    *
    *  @return  The custom layer.
    *
    *  @see     setLayer()
    */
   const CustomLayer* getLayer() const;

   /**
    *  This method performs the custom drawing in the custom layer.
    *
    *  @note    The drawing environment is setup in world coordinates.
    */
   virtual void draw() = 0;

   /**
    *  This method provides custom processing for a mouse press event in the custom layer.
    *
    *  @param   screenCoord
    *           The location in screen coordinates for the mouse press.
    *  @param   button
    *           The mouse button that was pressed.
    *  @param   buttons
    *           The QFlags associated with the mouse button that was pressed.
    *  @param   modifiers
    *           The keyboard modifiers (e.g. Shift key) that are pressed when the mouse button was pressed.
    *
    *  @return  Returns \c true if the method was able to process the mouse press event, otherwise it returns \c false.
    *
    *  @default The default implementation does nothing and returns \c false.
    */
   virtual bool processMousePress(const QPoint& screenCoord, Qt::MouseButton button, Qt::MouseButtons buttons,
      Qt::KeyboardModifiers modifiers);

   /**
    *  This method provides custom processing for a mouse move event in the custom layer.
    *
    *  @param   screenCoord
    *           The location in screen coordinates for the current mouse position.
    *  @param   button
    *           The mouse button that was pressed.
    *  @param   buttons
    *           The QFlags associated with the mouse button that was pressed.
    *  @param   modifiers
    *           The keyboard modifiers (e.g. Shift key) that are pressed when the mouse was moved.
    *
    *  @return  Returns \c true if the method was able to process the mouse move event, otherwise it returns \c false.
    *
    *  @default The default implementation does nothing and returns \c false.
    */
   virtual bool processMouseMove(const QPoint& screenCoord, Qt::MouseButton button, Qt::MouseButtons buttons,
      Qt::KeyboardModifiers modifiers);

   /**
    *  This method provides custom processing for a mouse release event in the custom layer.
    *
    *  @param   screenCoord
    *           The location in screen coordinates for the mouse release.
    *  @param   button
    *           The mouse button that was pressed.
    *  @param   buttons
    *           The QFlags associated with the mouse button that was pressed.
    *  @param   modifiers
    *           The keyboard modifiers (e.g. Shift key) that are pressed when the mouse button was released.
    *
    *  @return  Returns \c true if the method was able to process the mouse release event,
    *           otherwise it returns \c false.
    *
    *  @default The default implementation does nothing and returns \c false.
    */
   virtual bool processMouseRelease(const QPoint& screenCoord, Qt::MouseButton button, Qt::MouseButtons buttons,
      Qt::KeyboardModifiers modifiers);

   /**
    *  This method provides custom processing for a mouse double click event in the custom layer.
    *
    *  @param   screenCoord
    *           The location in screen coordinates for the mouse double click.
    *  @param   button
    *           The mouse button that was double clicked.
    *  @param   buttons
    *           The QFlags associated with the mouse button that was pressed.
    *  @param   modifiers
    *           The keyboard modifiers (e.g. Shift key) that are pressed when the mouse button was double clicked.
    *
    *  @return  Returns \c true if the method was able to process the mouse double click event,
    *           otherwise it returns \c false.
    *
    *  @default The default implementation does nothing and returns \c false.
    */
   virtual bool processMouseDoubleClick(const QPoint& screenCoord, Qt::MouseButton button, Qt::MouseButtons buttons,
      Qt::KeyboardModifiers modifiers);

   /**
    *  Retrieves the bounding box of the area within the custom layer to which
    *  the draw object will draw.
    *
    *  @param   x1
    *           The minimum coordinate value of the draw area in the X
    *           dimension.
    *  @param   y1
    *           The minimum coordinate value of the draw area in the Y
    *           dimension.
    *  @param   x4
    *           The maximum coordinate value of the draw area in the X
    *           dimension.
    *  @param   y4
    *           The maximum coordinate value of the draw area in the Y
    *           dimension.
    *
    *  @return  Returns \c true if the drawing extents are defined and the
    *           bounding box values are set in \em x1, \em y1, \em x4, and
    *           \em y4; otherwise returns \c false and the parameters are
    *           invalid.
    *
    *  @see     getExtents(std::vector<LocationType>&)<br>
    *           Layer::getExtents(double&, double&, double&, double&)
    */
   virtual bool getExtents(double& x1, double& y1, double& x4, double& y4) = 0;

   /**
    *  Retrieves area within the custom layer to which the draw object will
    *  draw.
    *
    *  @param   worldCoords
    *           The extent locations that comprise the area to which the draw
    *           object will draw.  This may be different from the bounding box
    *           draw area returned by
    *           getExtents(double&, double&, double&, double&) in that the
    *           coordinates representing individual data locations are added to
    *           the vector.
    *
    *  @return  Returns \c true if the drawing extents of the object were
    *           successfully retrieved; otherwise returns \c false and the
    *           parameter is invalid.
    *
    *  @default The default implementation calls
    *           getExtents(double&, double&, double&, double&) and adds the four
    *           corner locations derived from the bounding box to the vector.
    *
    *  @see     getExtents(double&, double&, double&, double&)<br>
    *           Layer::getExtents(std::vector<LocationType>&)
    */
   virtual bool getExtents(std::vector<LocationType>& worldCoords);

   /**
    *  This method performs any reset functions that may be required to reset the draw object.
    */
   virtual void reset() = 0;

private:
   CustomLayer* mpLayer;
};
#endif
