/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef GRAPHICOBJECTFACTORY_H
#define GRAPHICOBJECTFACTORY_H

#include "GraphicObject.h"
#include "GraphicObjectImp.h"
#include "LocationType.h"
#include "Resource.h"
#include "TypesFile.h"

#include <list>

class GraphicLayer;
class Progress;

/**
 *  Since the GraphicGroup had to change to no longer be static, the factory
 *  became necessary to produce the annotation objects that the group once created
 */
class GraphicObjectFactory
{
public:
   static GraphicObject* createObject(GraphicObjectType objectType, GraphicLayer* pLayer,
      LocationType pixelCoord = LocationType());
   static std::list<GraphicObject*> createObjects(unsigned int numObjects, GraphicObjectType objectType,
      GraphicLayer* pLayer, LocationType pixelCoord = LocationType(), Progress* pProgress = NULL);
};

template <typename T>
class GraphicObjectObject
{
public:
   class Args
   {
   public:
      Args(GraphicObjectType type, GraphicLayer* pLayer, LocationType pixel) :
         mType(type),
         mpLayer(pLayer),
         mPixel(pixel) {}

      GraphicObjectType mType;
      GraphicLayer* mpLayer;
      LocationType mPixel;
   };

   T* obtainResource(const Args& args) const
   {
      return dynamic_cast<T*>(GraphicObjectFactory::createObject(args.mType, args.mpLayer, args.mPixel));
   }

   void releaseResource(const Args& args, T* pObject) const
   {
      delete dynamic_cast<GraphicObjectImp*>(pObject);
   }
};

template<typename T = GraphicObject>
class GraphicResource : public Resource<T, GraphicObjectObject<T> >
{
public:
   GraphicResource(GraphicObjectType type, GraphicLayer* pLayer = NULL, LocationType pixel = LocationType()) :
      Resource<T, GraphicObjectObject<T> >(typename GraphicObjectObject<T>::Args(type, pLayer, pixel)) {}
};

#endif
