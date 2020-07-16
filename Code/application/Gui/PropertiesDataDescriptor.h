/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PROPERTIESDATADESCRIPTOR_H
#define PROPERTIESDATADESCRIPTOR_H

#include "DataDescriptor.h"
#include "ObjectResource.h"
#include "PropertiesShell.h"

class SessionItem;

class PropertiesDataDescriptor : public PropertiesShell
{
public:
   PropertiesDataDescriptor();
   virtual ~PropertiesDataDescriptor();

   virtual bool initialize(SessionItem* pSessionItem);
   virtual bool applyChanges();

protected:
   virtual QWidget* createWidget();

private:
   DataDescriptorResource<DataDescriptor> mpDescriptor;
};

#endif
