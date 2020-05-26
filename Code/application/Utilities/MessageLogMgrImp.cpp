/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "ConfigurationSettings.h"
#include "Filename.h"
#include "MessageLogAdapter.h"
#include "MessageLogMgrImp.h"
#include "SessionManager.h"

#include <QtCore/QTemporaryFile>

using namespace std;

MessageLogMgrImp* MessageLogMgrImp::spInstance = NULL;
bool MessageLogMgrImp::mDestroyed = false;

MessageLogMgrImp::MessageLogMgrImp() :
   mpJournal(NULL)
{
   const Filename* pMessageLogPath = ConfigurationSettings::getSettingMessageLogPath();
   if (pMessageLogPath != NULL)
   {
      mLogPath = pMessageLogPath->getFullPathAndName();
   }

   if (mLogPath.empty())
   {
      const Filename* pTempPath = ConfigurationSettings::getSettingTempPath();
      if (pTempPath != NULL)
      {
         mLogPath = pTempPath->getFullPathAndName();
      }
   }

   std::string journalFilename = (mLogPath + "/journal");

#if HAVE_QSAVEFILE
   mpJournal = new QFILE(QString::fromStdString(journalFilename));
#else
   QTemporaryFile* pTempFile = new QTemporaryFile(QString::fromStdString(journalFilename));
   mpJournal = pTempFile;
   if (pTempFile != NULL)
   {
       pTempFile->setAutoRemove(false); // Debug only?
   }
#endif
   if ((mpJournal == NULL) || !mpJournal->open(QIODevice::WriteOnly))
   {
         string msg("Unable to open journal file ");
         msg += journalFilename;
         msg += " for writing ";
         // As logFile hasn't been created yet either, write message to stderr
         std::cerr << msg << std::endl;
   }
   else
   {
#if HAVE_QSAVEFILE
       mpJournal->setPermissions(QFileDevice::ReadUser | QFileDevice::WriteUser);
#else
       mpJournal->setPermissions(QFile::ReadUser | QFile::WriteUser);
#endif
   }
   // Create a default session log
   createLog(Service<SessionManager>()->getName());
}

void MessageLogMgrImp::finalize()
{
    for( std::map<std::string, MessageLog*>::iterator liter=mLogMap.begin(); liter!= mLogMap.end(); ++liter )
    {
        const std::string & logName = liter->first;
        MessageLog* pLog = liter->second;
        if(pLog)
        {
            for( std::vector<Message*>::iterator miter=pLog->begin(); miter!=pLog->end(); ++miter)
            {
                Message* pMsg = *miter;
                if( pMsg && !pMsg->isFinalized() )
                {
                    pMsg->finalize();
                }
            }
        }
    }
}

MessageLogMgrImp::~MessageLogMgrImp()
{
   finalize();
   notify(SIGNAL_NAME(Subject, Deleted));
   clear();

#if HAVE_QSAVEFILE
   mpJournal->commit();
   unlink(mpJournal->fileName().toStdString().c_str());
#else
   mpJournal->close();
   mpJournal->remove();
#endif
   delete mpJournal;
   mpJournal = NULL;
}

MessageLogMgrImp* MessageLogMgrImp::instance()
{
   if (spInstance == NULL)
   {
      if (mDestroyed)
      {
         throw std::logic_error("Attempting to use MessageLogMgr after destroying it.");
      }

      spInstance = new MessageLogMgrImp;
   }

   return spInstance;
}

void MessageLogMgrImp::destroy()
{
   if (mDestroyed)
   {
      throw std::logic_error("Attempting to destroy MessageLogMgr after destroying it.");
   }

   delete spInstance;
   spInstance = NULL;
   mDestroyed = true;
}

void MessageLogMgrImp::setPath(const string& path)
{
   if ((path.empty() == false) && (path != mLogPath))
   {
      mLogPath = path;
      notify(SIGNAL_NAME(MessageLogMgr, LogPathChanged), mLogPath);
   }
}

MessageLog* MessageLogMgrImp::createLog(const string& logName)
{
   if (getLog(logName) != NULL)
   {
      return NULL;
   }

   MessageLog* pLog = new MessageLogAdapter(logName.c_str(), mLogPath.c_str(), mpJournal);
   mLogMap.insert(pair<string, MessageLog*>(logName, pLog));
   notify(SIGNAL_NAME(MessageLogMgr, LogAdded), pLog);

   return pLog;
}

MessageLog* MessageLogMgrImp::getLog(const string& logName) const
{
   if (logName.empty() == true)
   {
      return NULL;
   }

   map<string, MessageLog*>::const_iterator iter = mLogMap.find(logName);
   if (iter != mLogMap.end())
   {
      return iter->second;
   }

   return NULL;
}

MessageLog* MessageLogMgrImp::getLog() const
{
   return getLog(Service<SessionManager>()->getName());
}

void MessageLogMgrImp::clear()
{
   while (mLogMap.empty() == false)
   {
      map<string, MessageLog*>::iterator iter = mLogMap.begin();
      MessageLog* pLog = iter->second;
      mLogMap.erase(iter);
      notify(SIGNAL_NAME(MessageLogMgr, LogRemoved), pLog);
      delete dynamic_cast<MessageLogAdapter*>(pLog);
   }
}

vector<MessageLog*> MessageLogMgrImp::getLogs() const
{
   vector<MessageLog*> logs;
   for (map<string, MessageLog*>::const_iterator iter = mLogMap.begin(); iter != mLogMap.end(); ++iter)
   {
      MessageLog* pLog = iter->second;
      if (pLog != NULL)
      {
         logs.push_back(pLog);
      }
   }

   return logs;
}

const string& MessageLogMgrImp::getObjectType() const
{
   static string sType("MessageLogMgrImp");
   return sType;
}

bool MessageLogMgrImp::isKindOf(const string& className) const
{
   if ((className == getObjectType()) || (className == "MessageLogMgr"))
   {
      return true;
   }

   return false;
}
