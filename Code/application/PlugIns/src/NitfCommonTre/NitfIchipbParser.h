/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef NITFICHIPBPARSER_H
#define NITFICHIPBPARSER_H

#include "NitfTreParserShell.h"

namespace Nitf
{
   class IchipbParser : public TreParserShell
   {
   public:
      IchipbParser();

      bool toDynamicObject(std::istream&, size_t numBytes, DynamicObject&, 
         std::string &errorMessage) const;
      bool ossimTagToDynamicObject(const ossimNitfRegisteredTag& input, DynamicObject& output, 
         std::string &errorMessage) const;
      bool fromDynamicObject(const DynamicObject& input, std::ostream& output, size_t& numBytesWritten, 
         std::string &errorMessage) const;

      virtual TreState isTreValid(const DynamicObject& tre, std::ostream& reporter) const;

      TreExportStatus exportMetadata(const RasterDataDescriptor &descriptor, 
         const RasterFileDescriptor &exportDescriptor, DynamicObject &tre, 
         unsigned int & ownerIndex, std::string & tagType, std::string &errorMessage) const;

      // inherited from Testable
      bool runAllTests(Progress* pProgress, std::ostream& failure);
   };
}

#endif
