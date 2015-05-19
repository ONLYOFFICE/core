// X__CacheFile.cpp: implementation of the X__CacheFile class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ap_AggPlusTypes.h"
#include "X__CacheFile.h"
#include "ap_Mem.h"

#include "../CxImage/CxImage/ximage.h"

X__CacheFile Z__CacheFile;


BOOL if_file_exists(LPCWSTR szwFilename)
{
 HANDLE hFile;
 hFile=CreateFileW(szwFilename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if(hFile==INVALID_HANDLE_VALUE)
  {
   if(GetLastError()==ERROR_SHARING_VIOLATION) return(TRUE); //File exists but is open
   return(FALSE);
  }
 CloseHandle(hFile);
 return(TRUE);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

X__CacheFile::X__CacheFile()
{
 m_dwBuffSizeTotal=0;
 m_cache_items.reserve(255);
}

X__CacheFile::~X__CacheFile()
{
 for(unsigned int i=0;i<m_cache_items.size(); ++i)
  {
   CItem *pItm=(CItem *) m_cache_items.at(i);
   Aggplus::ap_free(pItm->m_pBuff);
#ifdef _DEBUG
   //assert(m_dwBuffSizeTotal>=pItm->m_dwBuffSize);
   m_dwBuffSizeTotal-=pItm->m_dwBuffSize;
#endif
   delete(pItm);
  }
 /*
 for(CChain *pc_item=m_cache_items.GetPrev(); pc_item; pc_item=pc_item->GetPrev())
  {
   CItem *pItm=(CItem *) pc_item->GetData();
   Aggplus::ap_free(pItm->m_pBuff);
#ifdef _DEBUG
   assert(m_dwBuffSizeTotal>=pItm->m_dwBuffSize);
   m_dwBuffSizeTotal-=pItm->m_dwBuffSize;
#endif
   delete(pItm);
  }
  */

}

LPVOID X__CacheFile::GetCreateItemBuff(LPCWSTR szwFilename, DWORD *pdwRetWidth, DWORD *pdwRetHeight)
{
 *pdwRetWidth=*pdwRetHeight=0;
 if(!if_file_exists(szwFilename)) return(NULL);

 WIN32_FILE_ATTRIBUTE_DATA FileInfo;
 if(!GetFileAttributesExW(szwFilename, GetFileExInfoStandard, &FileInfo)) return(NULL);

 CKey key_search;
 key_search.m_dwFileSizeLow=FileInfo.nFileSizeLow;
 key_search.m_dwFileSizeHigh=FileInfo.nFileSizeHigh;
 key_search.m_ftCreationTime=FileInfo.ftCreationTime;
 key_search.m_ftLastWriteTime=FileInfo.ftLastWriteTime;

 //Search Cache
 for(unsigned int i=0;i<m_cache_items.size(); ++i)
  {
   CItem *pItm=(CItem *) m_cache_items.at(i);
 //for(CChain *pc_item=m_cache_items.GetPrev(); pc_item; pc_item=pc_item->GetPrev())
 // {
 //  CItem *pItm=(CItem *) pc_item->GetData();
   if( memcmp(&(pItm->m_Key), &key_search, sizeof(CKey)) == 0
    && wcscmp(pItm->m_wcFilename, szwFilename)==0
    )
    {
     GetSystemTimeAsFileTime(&pItm->m_ftLastRequested);
     *pdwRetWidth=pItm->m_dwWidth;
     *pdwRetHeight=pItm->m_dwHeight;
     pItm->m_nUsage++;
     return(pItm->m_pBuff);
    }
  }
 

 DWORD dwWidth, dwHeight;
 void *ppBuff;
 if(xx_Get_RGBA_from_file(szwFilename, &dwWidth, &dwHeight, &ppBuff))
  {
   if(EnsureFreeSpace(dwWidth*dwHeight*4))
    {
     //Dodadi vo cache i vrati nazad params i buff
     CItem *pItm=new CItem;
     wcscpy(pItm->m_wcFilename, szwFilename);

     memcpy(&(pItm->m_Key), &key_search, sizeof(CKey));

     pItm->m_dwWidth=dwWidth;
     pItm->m_dwHeight=dwHeight;

     pItm->m_pBuff=ppBuff;
     pItm->m_dwBuffSize=dwWidth*dwHeight*4;
     m_dwBuffSizeTotal+=pItm->m_dwBuffSize;
     GetSystemTimeAsFileTime(&pItm->m_ftLastRequested);
     pItm->m_nUsage=1;
     //m_cache_items.Add(pItm);
     m_cache_items.push_back(pItm);

     *pdwRetWidth=pItm->m_dwWidth;
     *pdwRetHeight=pItm->m_dwHeight;
     //pItm->m_psMemBuff->AddUsage();
     //DbgAddLine("X__CacheFile: %.2f Mb %d items", (double(GetBuffSize())/1024.00)/1024.00, GetCount());
     return(pItm->m_pBuff);
    }
   else
    {
     //No free space, return ordinary pointer
     *pdwRetWidth=dwWidth;
     *pdwRetHeight=dwHeight;
     //DbgAddLine("X__CacheFile: %.2f Mb %d items (NO SPACE)", (double(GetBuffSize())/1024.00)/1024.00, GetCount());
     return(ppBuff);
    }
  }
 
 return(NULL);
}

void X__CacheFile::Release(LPVOID pBuff)
{
 if(!pBuff) return;
 //Search Cache
 for(unsigned int i=0;i<m_cache_items.size(); ++i)
  {
   CItem *pItm=(CItem *) m_cache_items.at(i);
 //for(CChain *pc_item=m_cache_items.GetPrev(); pc_item; pc_item=pc_item->GetPrev())
 // {
 //  CItem *pItm=(CItem *) pc_item->GetData();
   if(pItm->m_pBuff == pBuff)
    {
     pItm->m_nUsage--;
     //assert(pItm->m_nUsage>=0);
     //DbgAddLine("X__CacheFile: %.2f Mb %d items (Release usage %d)", (double(GetBuffSize())/1024.00)/1024.00, GetCount(), pItm->m_nUsage);
     return;
    }
  }

 //Invalid-not in cache
 Aggplus::ap_free(pBuff);
}

LPVOID X__CacheFile::GetCreateCopy(LPVOID pSourceBuff, DWORD dwWidth, DWORD dwHeight)
{
 if(!pSourceBuff) return(NULL);
 //Search Cache
 for(unsigned int i=0;i<m_cache_items.size(); ++i)
  {
   CItem *pItm=(CItem *) m_cache_items.at(i);
 //for(CChain *pc_item=m_cache_items.GetPrev(); pc_item; pc_item=pc_item->GetPrev())
 // {
 //  CItem *pItm=(CItem *) pc_item->GetData();
   if(pItm->m_pBuff == pSourceBuff)
    {
     pItm->m_nUsage++;
     //DbgAddLine("X__CacheFile: %.2f Mb %d items (Fnd New usage %d)", (double(GetBuffSize())/1024.00)/1024.00, GetCount(), pItm->m_nUsage);
     return(pSourceBuff);
    }
  }

 //Invalid-not in cache, return ordinary pointer and copy from source
 LPVOID pRet=Aggplus::ap_malloc16(4*dwWidth*dwHeight);
 memcpy(pRet, pSourceBuff, 4*dwWidth*dwHeight);
 //DbgAddLine("X__CacheFile: %.2f Mb %d items (NO Fnd found)", (double(GetBuffSize())/1024.00)/1024.00, GetCount());
 return(pRet);
}

bool X__CacheFile::EnsureFreeSpace(DWORD dwSize)
{
 while(GetBuffSize()+dwSize>GetMaxBuffSize())
  {
   //free some space if I can
   FILETIME ftLastRequested={0, 0};
   GetSystemTimeAsFileTime(&ftLastRequested);

   //CChain *pc_Found=NULL;
   //CItem *pc_Found=NULL;
   int nFound=-1;
   for(unsigned int i=0;i<m_cache_items.size(); ++i)
    {
     CItem *pItm=(CItem *) m_cache_items.at(i);
   //for(CChain *pc_item=m_cache_items.GetPrev(); pc_item; pc_item=pc_item->GetPrev())
   // {
   //  CItem *pItm=(CItem *) pc_item->GetData();
     if(pItm->m_nUsage==0)
      {
       if(CompareFileTime(&ftLastRequested, &(pItm->m_ftLastRequested))>0)
        {
         //pc_Found=pc_item;
         nFound=i;
         ftLastRequested=pItm->m_ftLastRequested;
        }
      }
    }

   //if(pc_Found)
   if(nFound>=0)
    {
     //CItem *pItm=(CItem *) pc_Found->GetData();
     CItem *pItm=(CItem *) m_cache_items.at(nFound);
     //DbgAddLine("X__CacheFile:  Delete item %d", nLastUsedIndex);
     //assert(m_dwBuffSizeTotal>=pItm->m_dwBuffSize);
     m_dwBuffSizeTotal-=pItm->m_dwBuffSize;
     
     Aggplus::ap_free(pItm->m_pBuff);
     delete(pItm);
     //m_cache_items.Remove(pc_Found);
     m_cache_items.erase(m_cache_items.begin() + nFound);
     //TRACE("XC:Remove Item\n");
    }
   else
    {
     return(false);
    }
  }
 return(true);
}



BOOL xx_Get_RGBA_from_file(LPCWSTR szwFilename, DWORD *pdwWidth, DWORD *pdwHeight, void **ppBuff)
{
 if(!szwFilename || wcslen(szwFilename)<4) return(FALSE);
 LPCWSTR wExt=szwFilename+(wcslen(szwFilename)-4);

 DWORD dwImgType=CXIMAGE_FORMAT_UNKNOWN;

 if(     _wcsicmp(wExt, L".tga")==0) dwImgType=CXIMAGE_FORMAT_TGA; //TGA
 else if(_wcsicmp(wExt, L".png")==0) dwImgType=CXIMAGE_FORMAT_PNG; //PNG
 else if(_wcsicmp(wExt, L".bmp")==0) dwImgType=CXIMAGE_FORMAT_BMP; //BMP
 else if(_wcsicmp(wExt, L".jpg")==0) dwImgType=CXIMAGE_FORMAT_JPG; //JPG
 else if(_wcsicmp(wExt, L".gif")==0) dwImgType=CXIMAGE_FORMAT_GIF; //GIF

 BOOL  retRes = FALSE;
 FILE* hFile = NULL;
  
 if(dwImgType!=CXIMAGE_FORMAT_UNKNOWN)
  {
   CxImage MXimage(dwImgType);
   if((hFile=_wfopen(szwFilename, L"rb"))==NULL)  
	   return FALSE;	// For UNICODE support

   if(MXimage.Decode(hFile,dwImgType))
    {
     DWORD dwWidth=MXimage.GetWidth();
     DWORD dwHeight=MXimage.GetHeight();
     if(!dwWidth || !dwHeight) 
	 {
		 goto endLbl;
	 }
     *pdwWidth=dwWidth;
     *pdwHeight=dwHeight;
     //(*ppBuff)=Aggplus::ap_malloc16(4*dwWidth*dwHeight);
     //MXimage.CopyToBuffRGBA32((*ppBuff));

	 (*ppBuff)=Aggplus::ap_malloc16(4*dwWidth*dwHeight);

	 LONG lSize = 0;
	 MXimage.Encode2RGBA2((BYTE*)(*ppBuff), lSize, true);

	 //BYTE* pSrc = MXimage.GetBits();
	 //memcpy(*ppBuff, pSrc, 4*dwWidth*dwHeight);
	 /*LONG lSize = 0;
	 BYTE* pData = NULL;
	 
	 MXimage.Encode2RGBA(pData, lSize, true);

	 *ppBuff = (void*)pData;*/

	 retRes = TRUE;
    }
   else
    {
     //TRACE(">>>Load Image Error:%s\n", MXimage.GetLastError());
    }
  }

endLbl:

  if (hFile)
	  fclose(hFile);

  return retRes;
}
