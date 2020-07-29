/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "Endian.h"
#include "FileDescriptor.h"
#include "FileDescriptorImp.h"
#include "Filename.h"
#include "ObjectResource.h"
#include "StringUtilities.h"
#include "Subject.h"

XERCES_CPP_NAMESPACE_USE
using namespace std;

FileDescriptorImp::FileDescriptorImp() :
   mEndian(Endian::getSystemEndian())
{}

FileDescriptorImp::~FileDescriptorImp()
{}

void FileDescriptorImp::setFilename(const string& filename)
{
   if (filename != mFilename.getFullPathAndName())
   {
      mFilename.setFullPathAndName(filename);
      notify(SIGNAL_NAME(FileDescriptor, FilenameChanged), boost::any(&mFilename));
   }
}

void FileDescriptorImp::setFilename(const Filename& filename)
{
   if (filename != mFilename)
   {
      mFilename = filename;
      notify(SIGNAL_NAME(FileDescriptor, FilenameChanged), boost::any(&mFilename));
   }
}

const Filename& FileDescriptorImp::getFilename() const
{
   return mFilename;
}

void FileDescriptorImp::setDatasetLocation(const string& datasetLocation)
{
   if (datasetLocation != mDatasetLocation)
   {
      mDatasetLocation = datasetLocation;
      notify(SIGNAL_NAME(FileDescriptor, DatasetLocationChanged), boost::any(mDatasetLocation));
   }
}

const string& FileDescriptorImp::getDatasetLocation() const
{
   return mDatasetLocation;
}

void FileDescriptorImp::setEndian(EndianType endian)
{
   if (endian != mEndian)
   {
      mEndian = endian;
      notify(SIGNAL_NAME(FileDescriptor, EndianChanged), boost::any(mEndian));
   }
}

EndianType FileDescriptorImp::getEndian() const
{
   return mEndian;
}

bool FileDescriptorImp::clone(const FileDescriptor* pFileDescriptor)
{
   if (pFileDescriptor == NULL)
   {
      return false;
   }

   if (dynamic_cast<const FileDescriptorImp*>(pFileDescriptor) != this)
   {
      setFilename(pFileDescriptor->getFilename());
      setDatasetLocation(pFileDescriptor->getDatasetLocation());
      setEndian(pFileDescriptor->getEndian());
   }

   return true;
}

void FileDescriptorImp::addToMessageLog(Message* pMessage) const
{
   if (pMessage == NULL)
   {
      return;
   }

   // Filename
   pMessage->addProperty("Filename", mFilename);

   // Data set location
   pMessage->addProperty("Data Set Location", mDatasetLocation);

   // Endian
   pMessage->addProperty("Endian", StringUtilities::toDisplayString(mEndian));
}

bool FileDescriptorImp::toXml(XMLWriter* pXml) const
{
   if (pXml == NULL)
   {
      return false;
   }

   // Filename
   pXml->addAttr("filename", static_cast<string>(
      StringUtilities::toXmlString(dynamic_cast<const Filename*>(&mFilename))));
   pXml->addAttr("datasetLocation", mDatasetLocation);
   pXml->addAttr("endian", mEndian);
   pXml->addAttr("type", "FileDescriptor");

   return true;
}

bool FileDescriptorImp::fromXml(DOMNode* pDocument, unsigned int version)
{
   if (pDocument == NULL)
   {
      return false;
   }

   DOMElement* pElement = static_cast<DOMElement*>(pDocument);

   // Filename
   bool error;
   FactoryResource<Filename> pFilename(StringUtilities::fromXmlString<Filename*>(
      A(pElement->getAttribute(X("filename"))), &error));
   if (pFilename.get() == NULL)
   {
      return false;
   }
   mFilename = *(pFilename.get());
   // Dataset location
   if (!error)
   {
      mDatasetLocation = A(pElement->getAttribute(X("datasetLocation")));
      mEndian = StringUtilities::fromXmlString<EndianType>(A(pElement->getAttribute(X("endian"))), &error);
   }

   notify(SIGNAL_NAME(Subject, Modified));
   return !error;
}

const string& FileDescriptorImp::getObjectType() const
{
   static string sType("FileDescriptorImp");
   return sType;
}

bool FileDescriptorImp::isKindOf(const string& className) const
{
   if ((className == getObjectType()) || (className == "FileDescriptor"))
   {
      return true;
   }

   return false;
}

void FileDescriptorImp::getFileDescriptorTypes(vector<string>& classList)
{
   classList.push_back("FileDescriptor");
}

bool FileDescriptorImp::isKindOfFileDescriptor(const string& className)
{
   if ((className == "FileDescriptorImp") || (className == "FileDescriptor"))
   {
      return true;
   }

   return false;
}
