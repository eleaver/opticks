/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef ICERASTERELEMENTIMPORTER_H
#define ICERASTERELEMENTIMPORTER_H

#include "IceImporterShell.h"

class IceRasterElementImporter : public IceImporterShell
{
public:
   IceRasterElementImporter();
   ~IceRasterElementImporter();

private:
   IceRasterElementImporter& operator=(const IceRasterElementImporter& rhs);
};

#endif
