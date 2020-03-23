/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef JPEG2000PAGER_H
#define JPEG2000PAGER_H

#include "CachedPager.h"

#include <openjpeg.h>
#include <stdio.h>
#include <string>

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

class Jpeg2000Pager : public CachedPager
{
public:
   Jpeg2000Pager();
   virtual ~Jpeg2000Pager();

   static std::string offsetArg();
   static std::string sizeArg();

   virtual bool getInputSpecification(PlugInArgList*& pArgList);
   virtual bool parseInputArgs(PlugInArgList* pArgList);

   virtual bool openFile(const std::string& filename);
   virtual CachedPage::UnitPtr fetchUnit(DataRequest* pOriginalRequest);

protected:
   virtual double getChunkSize() const;

   template <typename Out>
   CachedPage::UnitPtr populateImageData(const DimensionDescriptor& startRow, const DimensionDescriptor& startColumn,
      unsigned int concurrentRows, unsigned int concurrentColumns) const;

   opj_image_t* decodeImage(unsigned int originalStartRow, unsigned int originalStartColumn,
      unsigned int originalStopRow, unsigned int originalStopColumn, int decoderType) const;

private:
   static size_t msMaxCacheSize;

   FILE* mpFile;
   uint64_t mOffset;
   uint64_t mSize;
   std::string mFilename;
};

#endif
