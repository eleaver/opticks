/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */
 


#ifndef MODELITEMS_H
#define MODELITEMS_H

#include "WizardItems.h"

class ModelItems : public WizardItems
{
public:
   ModelItems();
   ~ModelItems();

   virtual bool setBatch();
   virtual bool setInteractive();
};

#endif   // MODELITEMS_H

 
