/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef POINTCLOUDELEMENTADAPTER_H
#define POINTCLOUDELEMENTADAPTER_H

#include "PointCloudElement.h"
#include "PointCloudElementImp.h"

class PointCloudElementAdapter : public PointCloudElement, public PointCloudElementImp POINTCLOUDELEMENTADAPTEREXTENSION_CLASSES
{
public:
   PointCloudElementAdapter(const DataDescriptorImp& descriptor, const std::string& id);
   ~PointCloudElementAdapter();

   // TypeAwareObject
   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   POINTCLOUDELEMENTADAPTER_METHODS(PointCloudElementImp)
};

#endif
