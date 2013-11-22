// X__CacheFile.h: interface for the X__CacheFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_X__CACHEFILE_H__9056C787_C29B_4E79_A6CD_29768C0CEBA7__INCLUDED_)
#define AFX_X__CACHEFILE_H__9056C787_C29B_4E79_A6CD_29768C0CEBA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

#define defMAX_X__CacheFile_Size (64*1024*1024)

BOOL xx_Get_RGBA_from_file(LPCWSTR szwFilename, DWORD *pdwWidth, DWORD *pdwHeight, void **ppBuff);

class X__CacheFile  
{
public:
	X__CacheFile();
	virtual ~X__CacheFile();

 LPVOID GetCreateItemBuff(LPCWSTR szwFilename, DWORD *pdwRetWidth, DWORD *pdwRetHeight);
 LPVOID GetCreateCopy(LPVOID pSourceBuff, DWORD dwWidth, DWORD dwHeight);

 void Release(LPVOID pBuff);
 bool EnsureFreeSpace(DWORD dwSize);

 inline DWORD GetCount() const { return((DWORD)m_cache_items.size()); }
 inline DWORD GetBuffSize() const { return(m_dwBuffSizeTotal); }
 inline DWORD GetMaxBuffSize() const { return(defMAX_X__CacheFile_Size); }
protected:
 DWORD m_dwBuffSizeTotal;

 struct CKey
  {
   DWORD m_dwFileSizeHigh, m_dwFileSizeLow;
   FILETIME m_ftCreationTime, m_ftLastWriteTime;
  };
 struct CItem
  {
   WCHAR m_wcFilename[MAX_PATH];
   CKey m_Key;

   LPVOID m_pBuff;

   //internals
   int m_nUsage;
   DWORD m_dwBuffSize;
   FILETIME m_ftLastRequested;

   DWORD m_dwWidth, m_dwHeight;
  };

/*
class CChain
{
public:
 CChain() : m_pPrev(NULL), m_pNext(NULL), nCount(0), m_pData(NULL) { }
 ~CChain()
  {
   if(m_pPrev) { delete(m_pPrev); }
  }

 void Add(void *pData)
  {
   CChain *m_pNew=new CChain;
   m_pNew->m_pData=pData;
   m_pNew->m_pNext=this;
   m_pNew->m_pPrev=m_pPrev;
   if(m_pPrev) m_pPrev->m_pNext=m_pNew;
   m_pPrev=m_pNew;
   nCount++;
  }
 void Remove(CChain *pDel)
  {
   pDel->get_out_from_chain();
   nCount--;
   delete(pDel);
  }
 inline CChain *GetPrev() const { return m_pPrev; }
 inline void *GetData() const { return(m_pData); }
 inline int GetSize() const { return(nCount); }
protected:
 void get_out_from_chain()
  {
   if(m_pPrev) { m_pPrev->m_pNext=m_pNext; }
   if(m_pNext) { m_pNext->m_pPrev=m_pPrev; }
   m_pPrev=m_pNext=NULL;
  }
 int nCount;
 CChain *m_pPrev, *m_pNext;
 void *m_pData;
};
 CChain m_cache_items;
 */

 std::vector<void *> m_cache_items;
};


extern X__CacheFile Z__CacheFile;


#endif // !defined(AFX_X__CACHEFILE_H__9056C787_C29B_4E79_A6CD_29768C0CEBA7__INCLUDED_)
