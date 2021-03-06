/*
 * The information in this file is
 * Copyright(c) 2020 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef WORKSPACEWINDOWADAPTER_H
#define WORKSPACEWINDOWADAPTER_H

#include "WorkspaceWindow.h"
#include "WorkspaceWindowImp.h"

class WorkspaceWindowAdapter : public WorkspaceWindow, public WorkspaceWindowImp WORKSPACEWINDOWADAPTEREXTENSION_CLASSES
{
public:
   WorkspaceWindowAdapter(const std::string& id, const std::string& windowName, QWidget* parent = 0);
   ~WorkspaceWindowAdapter();

   // TypeAwareObject
   const std::string& getObjectType() const;
   bool isKindOf(const std::string& className) const;

   WORKSPACEWINDOWADAPTER_METHODS(WorkspaceWindowImp)

private:
   WorkspaceWindowAdapter(const WorkspaceWindowAdapter& rhs);
   WorkspaceWindowAdapter& operator=(const WorkspaceWindowAdapter& rhs);
};

#endif
