/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "LatLonLayerAdapter.h"
#include "LayerUndo.h"
#include "RasterElement.h"
#include "SpatialDataView.h"

using namespace std;

LatLonLayerAdapter::LatLonLayerAdapter(const string& id, const string& layerName, RasterElement* pRaster) :
   LatLonLayerImp(id, layerName, pRaster)
{
}

LatLonLayerAdapter::~LatLonLayerAdapter()
{
   SpatialDataView* pView = dynamic_cast<SpatialDataView*>(getView());
   if (pView != NULL)
   {
      pView->addUndoAction(new DeleteLayer(pView, this));
   }

   notify(SIGNAL_NAME(Subject, Deleted));
}

// TypeAwareObject
const string& LatLonLayerAdapter::getObjectType() const
{
   static string type("LatLonLayerAdapter");
   return type;
}

bool LatLonLayerAdapter::isKindOf(const string& className) const
{
   if ((className == getObjectType()) || (className == "LatLonLayer"))
   {
      return true;
   }

   return LatLonLayerImp::isKindOf(className);
}

// Layer
Layer* LatLonLayerAdapter::copy(const string& layerName, bool bCopyElement, DataElement* pParent) const
{
   // Get the layer name
   string name = layerName;
   if (name.empty() == true)
   {
      name = getName();
   }

   RasterElement* pRasterElement = NULL;

   RasterElement* pCurrentRasterElement = dynamic_cast<RasterElement*>(getDataElement());
   if (bCopyElement == true && pCurrentRasterElement != NULL)
   {
      pRasterElement = dynamic_cast<RasterElement*>(pCurrentRasterElement->copy(name, pParent));
   }
   else
   {
      pRasterElement = pCurrentRasterElement;
   }

   if (pRasterElement == NULL)
   {
      return NULL;
   }

   // Create the new layer
   LatLonLayerAdapter* pLayer = new LatLonLayerAdapter(SessionItemImp::generateUniqueId(), name, pRasterElement);
   if (pLayer != NULL)
   {
      *pLayer = *this;
   }

   return pLayer;
}
