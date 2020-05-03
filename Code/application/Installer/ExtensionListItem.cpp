/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "Aeb.h"
#include "ExtensionListItem.h"
#include "InstallerServices.h"
#include <QDialogButtonBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

ExtensionListItem::ExtensionListItem(bool editor, bool showUpdateInfo, QWidget* pParent) :
      QWidget(pParent),
      mUseUninstallHelper(false)
{
   //KIP - Fix up the layout of this widget.
   QVBoxLayout* pTopLevel = new QVBoxLayout(this);

   QHBoxLayout* pInfo = new QHBoxLayout;
   pTopLevel->addLayout(pInfo);
   mpIcon = new QLabel("pix", this);
   pInfo->addWidget(mpIcon);
   pInfo->addSpacing(10);

   QVBoxLayout* pText = new QVBoxLayout;
   pInfo->addLayout(pText);
   pInfo->addStretch();

   QHBoxLayout* pFirst = new QHBoxLayout;
   pText->addLayout(pFirst);
   mpName = new QLabel(this);
   mpName->setWordWrap(true);
   QFont boldFont = mpName->font();
   boldFont.setBold(true);
   mpName->setFont(boldFont);
   pFirst->addWidget(mpName);
   mpVersion = new QLabel(this);
   pFirst->addWidget(mpVersion);

   mpDescription = new QLabel(this);
   mpDescription->setWordWrap(true);
   pText->addWidget(mpDescription);

   if (showUpdateInfo)
   {
      mpUpdateInfo = new QLabel(this);
      mpUpdateInfo->setFont(boldFont);
      pText->addWidget(mpUpdateInfo);
   }
   else
   {
      mpUpdateInfo = NULL;
   }

   if (editor)
   {
      mpButtons = new QDialogButtonBox(this);
      mpButtons->addButton("Disable", QDialogButtonBox::ActionRole)->setEnabled(false);
      mpUninstallButton = mpButtons->addButton("Uninstall", QDialogButtonBox::DestructiveRole);
      connect(mpUninstallButton, SIGNAL(clicked()), this, SLOT(uninstall()));
      mpButtons->addButton("About", QDialogButtonBox::HelpRole);
      mpButtons->addButton("Update", QDialogButtonBox::ApplyRole)->setEnabled(false);
      pTopLevel->addWidget(mpButtons);
      connect(mpButtons, SIGNAL(helpRequested()), this, SLOT(about()));
   }
   else
   {
      mpButtons = NULL;
      mpUninstallButton = NULL;
   }
   pTopLevel->addStretch();
}

ExtensionListItem::~ExtensionListItem()
{
}

QString ExtensionListItem::getName() const
{
   return mpName->text();
}

QString ExtensionListItem::getDescription() const
{
   return mpDescription->text();
}

QString ExtensionListItem::getVersion() const
{
   return mpVersion->text();
}

QPixmap ExtensionListItem::getIcon() const
{
   const QPixmap* pPix = mpIcon->pixmap();
   return pPix == NULL ? QPixmap() : *pPix;
}

QString ExtensionListItem::getId() const
{
   return mExtensionId;
}

QString ExtensionListItem::getUpdateInfo() const
{
   if (mpUpdateInfo == NULL)
   {
      return QString();
   }
   return mpUpdateInfo->text();
}

bool ExtensionListItem::getUninstallable() const
{
   if (mpUninstallButton == NULL)
   {
      return true;
   }
   return mpUninstallButton->isEnabled();
}

bool ExtensionListItem::getUseUninstallHelper() const
{
   return mUseUninstallHelper;
}

void ExtensionListItem::setName(const QString& v)
{
   mpName->setText(v);
}

void ExtensionListItem::setDescription(const QString& v)
{
   mpDescription->setText(v);
}

void ExtensionListItem::setVersion(const QString& v)
{
   mpVersion->setText(v);
}

void ExtensionListItem::setIcon(const QPixmap& v)
{
   const QPixmap* pPix = mpIcon->pixmap();
   if (pPix != NULL)
   {
      mpIcon->setPixmap(v.scaled(pPix->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
   }
   else
   {
      mpIcon->setPixmap(v);
   }
}

void ExtensionListItem::setId(const QString& v)
{
   mExtensionId = v;
}

void ExtensionListItem::setUpdateInfo(const QString& v)
{
   if (mpUpdateInfo != NULL)
   {
      mpUpdateInfo->setText(v);
   }
}

void ExtensionListItem::about()
{
   const Aeb* pExtension = Service<InstallerServices>()->getAeb(mExtensionId.toStdString());
   if (pExtension != NULL && pExtension->validate())
   {
      QString name = QString::fromStdString(pExtension->getName());
      QString version = QString::fromStdString(pExtension->getVersion().toString());
      QString description = QString::fromStdString(pExtension->getDescription());
      QString creator = QString::fromStdString(pExtension->getCreator());
      QString homepageUrl = QString::fromStdString(pExtension->getHomepageURL());

      QString caption = QString("About %1").arg(name);
      QString message = QString("<h3>%1</h3>Version %2<br>%3<hr><b>Created By:</b><br>%4").arg(name).arg(version)
         .arg(description).arg(creator);
      if (homepageUrl.isEmpty() == false)
      {
         message += QString("<br><a href=\"%5\">Visit Home Page</a>").arg(homepageUrl);
      }

      QMessageBox::about(this, caption, message);
   }
}

void ExtensionListItem::uninstall()
{
   std::string errMsg;
   if (Service<InstallerServices>()->uninstallExtension(getId().toStdString(), errMsg))
   {
      setUninstallable(false);
   }
   else
   {
      setUseUninstallHelper(true);
      setUninstallable(false);
   }
}

void ExtensionListItem::setUninstallable(bool v)
{
   if (mpUninstallButton == NULL)
   {
      return;
   }
   bool cur = mpUninstallButton->isEnabled();
   if (mpUninstallButton != NULL)
   {
      mpUninstallButton->setEnabled(v);
   }
   if (!v)
   {
      if (mUseUninstallHelper)
      {
         setUpdateInfo("This extension will be uninstalled when this dialog is closed.");
      }
      else
      {
         setUpdateInfo("This extension will be uninstalled when Opticks is restarted.");
      }
   }
   else
   {
      setUpdateInfo("");
   }
   if (cur != v)
   {
      emit modified();
   }
   update();
}

void ExtensionListItem::setUseUninstallHelper(bool v)
{
   mUseUninstallHelper = v;
}