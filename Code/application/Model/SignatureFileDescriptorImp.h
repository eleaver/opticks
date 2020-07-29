/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SIGNATUREFILEDESCRIPTORIMP_H
#define SIGNATUREFILEDESCRIPTORIMP_H

#include "FileDescriptorImp.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class Units;

using XERCES_CPP_NAMESPACE_QUALIFIER DOMNode;

class SignatureFileDescriptorImp : public FileDescriptorImp
{
public:
   SignatureFileDescriptorImp();
   virtual ~SignatureFileDescriptorImp();

   void setUnits(const std::string& name, const Units* pUnits);
   const Units* getUnits(const std::string& name) const;
   std::set<std::string> getUnitNames() const;

   virtual bool clone(const FileDescriptor* pFileDescriptor);

   virtual bool toXml(XMLWriter* pXml) const;
   virtual bool fromXml(DOMNode* pDocument, unsigned int version);
   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   static void getFileDescriptorTypes(std::vector<std::string>& classList);
   static bool isKindOfFileDescriptor(const std::string& className);

protected:
   void notifyUnitsModified(Subject& subject, const std::string& signal, const boost::any& data);

private:
   SignatureFileDescriptorImp& operator=(const SignatureFileDescriptorImp& fileDescriptor);
   void clearUnits();

   std::map<std::string, Units*> mUnits;
};

#define SIGNATUREFILEDESCRIPTORADAPTEREXTENSION_CLASSES \
   FILEDESCRIPTORADAPTEREXTENSION_CLASSES

#define SIGNATUREFILEDESCRIPTORADAPTER_METHODS(impClass) \
   FILEDESCRIPTORADAPTER_METHODS(impClass) \
   void setUnits(const std::string& name, const Units* pUnits) \
   { \
      impClass::setUnits(name, pUnits); \
   } \
   const Units* getUnits(const std::string& name) const \
   { \
      return impClass::getUnits(name); \
   } \
   std::set<std::string> getUnitNames() const \
   { \
      return impClass::getUnitNames(); \
   }

#endif
