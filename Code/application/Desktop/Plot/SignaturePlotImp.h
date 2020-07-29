/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef SIGNATUREPLOTIMP_H
#define SIGNATUREPLOTIMP_H

#include <QtWidgets/QAction>

#include "CartesianPlotImp.h"
#include "TypesFile.h"

class SignaturePlotImp : public CartesianPlotImp
{
   Q_OBJECT

public:
   SignaturePlotImp(const std::string& id, const std::string& viewName, QGLContext* pDrawContext = 0,
      QWidget* pParent = 0);
   ~SignaturePlotImp();

   using CartesianPlotImp::setIcon;
   using CartesianPlotImp::setName;

   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   static bool isKindOfView(const std::string& className);
   static void getViewTypes(std::vector<std::string>& classList);

   View* copy(QGLContext* pDrawContext = 0, QWidget* pParent = 0) const;
   bool copy(View* pView) const;

   PlotType getPlotType() const;

protected:
   SignaturePlotImp& operator=(const SignaturePlotImp& signaturePlot);

private:
   SignaturePlotImp(const SignaturePlotImp& rhs);
};

#define SIGNATUREPLOTADAPTEREXTENSION_CLASSES \
   CARTESIANPLOTADAPTEREXTENSION_CLASSES

#define SIGNATUREPLOTADAPTER_METHODS(impClass) \
   CARTESIANPLOTADAPTER_METHODS(impClass)

#endif
