/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef JPEG2000IMPORTER_H
#define JPEG2000IMPORTER_H

#include "RasterElementImporterShell.h"

#include <openjpeg.h>

#include <map>
#include <string>
#include <vector>

// Opticks Dependencies/64/include/openjpeg/opj_config.h doesnt provide
// version numbers. But its pkg-config.pc claims its 2.0.0
// Otherwise, newer openjpeg versions do provide version numbers.
// (This stuff may be removed if we add version info to our opj_config.h)
#ifndef OPJ_VERSION_MAJOR
#define OPJ_VERSION_MAJOR 2
#endif
#ifndef OPJ_VERSION_MINOR
#define OPJ_VERSION_MINOR 0
#endif
#ifndef OPJ_VERSION_BUILD
#define OPJ_VERSION_BUILD 0
#endif
#ifndef OPJ_VERSION_NUMBER
#define OPJ_VERSION_NUMBER (100*100*(OPJ_VERSION_MAJOR) + 100*(OPJ_VERSION_MINOR) + (OPJ_VERSION_BUILD))
#endif

class RasterDataDescriptor;

class Jpeg2000Importer : public RasterElementImporterShell
{
public:
   Jpeg2000Importer();
   virtual ~Jpeg2000Importer();

   virtual unsigned char getFileAffinity(const std::string& filename);
   virtual std::vector<ImportDescriptor*> getImportDescriptors(const std::string& filename);
   virtual bool validate(const DataDescriptor* pDescriptor,
      const std::vector<const DataDescriptor*>& importedDescriptors, std::string& errorMessage) const;
   virtual QWidget* getPreview(const DataDescriptor* pDescriptor, Progress* pProgress);

   virtual bool createRasterPager(RasterElement* pRasterElement) const;

   static void defaultCallback(const char* pMessage, void* pClientData);   // These methods need to be static to be
   static void reportWarning(const char* pMessage, void* pClientData);     // properly used as callbacks in OpenJPEG
   static void reportError(const char* pMessage, void* pClientData);

protected:
   opj_image_t* getImageInfo(const std::string& filename, bool logErrors) const;
   bool populateDataDescriptor(RasterDataDescriptor* pDescriptor);

private:
   static std::map<std::string, std::vector<std::string> > msWarnings;
   static std::map<std::string, std::vector<std::string> > msErrors;
};

#endif
