/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef EXPORTDATASET_H
#define EXPORTDATASET_H

#include "DesktopItems.h"

#include <string>

class DataElement;
class FileDescriptor;
class Filename;
class Layer;
class View;

class ExportDataSet : public DesktopItems
{
public:
   ExportDataSet();
   ~ExportDataSet();

   bool setBatch();
   bool getInputSpecification(PlugInArgList*& pArgList);
   bool getOutputSpecification(PlugInArgList*& pArgList);
   bool execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList);

protected:
   bool extractInputArgs(PlugInArgList* pInArgList);

private:
   View* mpView;
   Layer* mpLayer;
   DataElement* mpElement;
   unsigned int mOutputWidth;
   unsigned int mOutputHeight;
   Filename* mpFilename;
   FileDescriptor* mpFileDescriptor;
   std::string mExporterName;
};

#endif
