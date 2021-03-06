/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PICTURESVIEWEXPORTER_H
#define PICTURESVIEWEXPORTER_H

#include "PicturesExporter.h"

class PicturesViewExporter : public PicturesExporter
{
public:
   PicturesViewExporter(PicturesDetails *pDetails);
   ~PicturesViewExporter();

   bool getInputSpecification(PlugInArgList*& pArgList);
   bool extractInputArgs(const PlugInArgList* pInArgList);
   bool generateImage(QImage &image);
};

#endif
