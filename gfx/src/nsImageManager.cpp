/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 *
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#include "nsIImageManager.h"
#include "libimg.h"
#include "nsCRT.h"
#include "nsImageNet.h"

static NS_DEFINE_IID(kIImageManagerIID, NS_IIMAGEMANAGER_IID);

class ImageManagerImpl : public nsIImageManager {
public:
  ImageManagerImpl();
  virtual ~ImageManagerImpl();

  nsresult Init();

  NS_DECL_AND_IMPL_ZEROING_OPERATOR_NEW

  NS_DECL_ISUPPORTS

  virtual void SetCacheSize(PRInt32 aCacheSize);
  virtual PRInt32 GetCacheSize(void);
  virtual PRInt32 ShrinkCache(void);
  NS_IMETHOD FlushCache(void);
 // virtual nsImageType GetImageType(const char *buf, PRInt32 length);

private:
  ilISystemServices *mSS;
};

// The singleton image manager
// XXX make this a service
static ImageManagerImpl*   gImageManager;

ImageManagerImpl::ImageManagerImpl()
{
  NS_NewImageSystemServices(&mSS);
  NS_ADDREF(mSS);
  IL_Init(mSS);
  IL_SetCacheSize(2048L * 1024L);
}

ImageManagerImpl::~ImageManagerImpl()
{
  IL_Shutdown();
  NS_RELEASE(mSS);
}

NS_IMPL_ISUPPORTS1(ImageManagerImpl, nsIImageManager); 

nsresult 
ImageManagerImpl::Init()
{
  return NS_OK;
}

void  
ImageManagerImpl::SetCacheSize(PRInt32 aCacheSize)
{
  IL_SetCacheSize(aCacheSize);
}

PRInt32 
ImageManagerImpl::GetCacheSize()
{
  return IL_GetCacheSize();
}
 
PRInt32 
ImageManagerImpl::ShrinkCache(void)
{
  return IL_ShrinkCache();
}

NS_IMETHODIMP
ImageManagerImpl::FlushCache(void)
{
  IL_FlushCache();
  return NS_OK;
}

extern "C" NS_GFX_(nsresult)
NS_NewImageManager(nsIImageManager **aInstancePtrResult)
{
  NS_PRECONDITION(nsnull != aInstancePtrResult, "null ptr");
  if (nsnull == aInstancePtrResult) {
    return NS_ERROR_NULL_POINTER;
  }
  if (nsnull == gImageManager) {
    gImageManager = new ImageManagerImpl();
    //neeti
    NS_IF_ADDREF(gImageManager);
  }
  if (nsnull == gImageManager) {
    return NS_ERROR_OUT_OF_MEMORY;
  }
  return gImageManager->QueryInterface(kIImageManagerIID,
                                       (void **)aInstancePtrResult);
}
extern "C" NS_GFX_(void)
NS_FreeImageManager()
{
  NS_IF_RELEASE(gImageManager);
}
