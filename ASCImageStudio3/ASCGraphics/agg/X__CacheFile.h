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

enum ImageTypes
{
	_CXIMAGE_FORMAT_UNKNOWN	= 0,
	_CXIMAGE_FORMAT_BMP		= 1,
	_CXIMAGE_FORMAT_GIF		= 2,
	_CXIMAGE_FORMAT_JPG		= 3,
	_CXIMAGE_FORMAT_PNG		= 4,
	_CXIMAGE_FORMAT_ICO		= 5,
	_CXIMAGE_FORMAT_TIF		= 6,
	_CXIMAGE_FORMAT_TGA		= 7,
	_CXIMAGE_FORMAT_PCX		= 8,
	_CXIMAGE_FORMAT_WBMP	= 9,
	_CXIMAGE_FORMAT_WMF		= 10,
	_CXIMAGE_FORMAT_JP2		= 11,
	_CXIMAGE_FORMAT_JPC		= 12,
	_CXIMAGE_FORMAT_PGX		= 13,
	_CXIMAGE_FORMAT_PNM		= 14,
	_CXIMAGE_FORMAT_RAS		= 15,
	_CXIMAGE_FORMAT_JBG		= 16,
	_CXIMAGE_FORMAT_MNG		= 17,
	_CXIMAGE_FORMAT_SKA		= 18,
	_CXIMAGE_FORMAT_RAW		= 19
};

static DWORD GetImageFileType(const CString &sFilePath, BOOL& bError)
{
	bError = FALSE;
	BYTE	pBuffer[ 44 ];
	DWORD	dwBytesRead;
	HANDLE	hFile;

	hFile = ::CreateFile(sFilePath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile) 
	{
		bError = TRUE;
		return FALSE;
	}
	::ReadFile(hFile, pBuffer, 44, &dwBytesRead, NULL);
	::CloseHandle(hFile); 

	//jpeg	
	// Hex: FF D8 FF
	if ( (3 <= dwBytesRead) && (0xFF == pBuffer[0]) && (0xD8 == pBuffer[1]) && (0xFF == pBuffer[2]) )
	{
		return _CXIMAGE_FORMAT_JPG;
	}

	//bmp ( http://ru.wikipedia.org/wiki/BMP )
	//Hex: 42 4D
	//ASCII: BM
	//Hex (position 6) : 00 00
	//Hex (position 26): 01 00
	//Hex (position 28): 00 || 01 || 04 || 08 || 10 || 18 || 20
	//Hex (position 29): 00
	//Hex (position 30): 00 || 01 || 02 || 03 || 04 || 05
	//Hex (position 31): 00 00 00
	if ( (34 <= dwBytesRead) && (0x42 == pBuffer[0]) && (0x4D == pBuffer[1]) && 
		 (0x00 == pBuffer[6]) && (0x00 == pBuffer[7]) && (0x01 == pBuffer[26]) && (0x00 == pBuffer[27]) && 
		 ( (0x00 == pBuffer[28]) || (0x01 == pBuffer[28]) || (0x04 == pBuffer[28]) || (0x08 == pBuffer[28]) ||
		   (0x10 == pBuffer[28]) || (0x18 == pBuffer[28]) || (0x20 == pBuffer[28]) ) && (0x00 == pBuffer[29]) && 
		 ( (0x00 == pBuffer[30]) || (0x01 == pBuffer[30]) || (0x02 == pBuffer[30]) || (0x03 == pBuffer[30]) || 
		   (0x04 == pBuffer[30]) || (0x05 == pBuffer[30]) ) && (0x00 == pBuffer[31]) && (0x00 == pBuffer[32]) && (0x00 == pBuffer[33]) )
	{
		return _CXIMAGE_FORMAT_BMP;
	}

	//gif 
	//Hex: 47 49 46 38
	//ASCII: GIF8
	//or for GIF87a...
	//Hex: 47 49 46 38 37 61
	//ASCII: GIF87a
	//or for GIF89a...
	//Hex: 47 49 46 38 39 61
	//ASCII: GIF89a
	if ( (4 <= dwBytesRead) && (0 == strncmp( (char *)pBuffer, "GIF8", 4)))
		return _CXIMAGE_FORMAT_GIF;
	if ( (6 <= dwBytesRead) && ( (0 == strncmp( (char *)pBuffer, "GIF87a", 6)) || (0 == strncmp((char *)pBuffer, "GIF89a", 6)) ) )
		return _CXIMAGE_FORMAT_GIF;

	//png 
	//Hex: 89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
	//ASCII: .PNG........IHDR
	if ( (16 <= dwBytesRead) && (0x89 == pBuffer[0]) && (0x50 == pBuffer[1]) && (0x4E == pBuffer[2]) && (0x47 == pBuffer[3])
		&& (0x0D == pBuffer[4]) && (0x0A == pBuffer[5]) && (0x1A == pBuffer[6]) && (0x0A == pBuffer[7])
		&& (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10]) && (0x0D == pBuffer[11])
		&& (0x49 == pBuffer[12]) && (0x48 == pBuffer[13]) && (0x44 == pBuffer[14]) && (0x52 == pBuffer[15]))
	{
		return _CXIMAGE_FORMAT_PNG;
	}

	//CR2
	//Hex: 49 49 2A 00 10 00 00 00 43 52
	//ASCII: II*.....CR
	if ( (10 <= dwBytesRead ) && (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2])
		&& (0x00 == pBuffer[3])  && (0x10 == pBuffer[4]) && (0x00 == pBuffer[5]) && (0x00 == pBuffer[6])
		&& (0x00 == pBuffer[7]) && (0x43 == pBuffer[8]) && (0x52 == pBuffer[9]) )
		return TRUE;

	//tiff
	//Hex: 49 49 2A 00
	//ASCII:
	//or for big endian
	//Hex: 4D 4D 00 2A
	//ASCII: MM.*
	//or for little endian
	//Hex: 49 49 2A 00
	//ASCII: II*
	if ( 4 <= dwBytesRead )
	{
		if ( ( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ) ||
			( (0x4D == pBuffer[0]) && (0x4D == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x2A == pBuffer[3]) ) ||
			( (0x49 == pBuffer[0]) && (0x49 == pBuffer[1]) && (0x2A == pBuffer[2]) && (0x00 == pBuffer[3]) ))
			return _CXIMAGE_FORMAT_TIF;
	}

	//wmf
	//Hex: D7 CD C6 9A 00 00
	//or for Windows 3.x
	//Hex: 01 00 09 00 00 03
	if ( 6 <= dwBytesRead )
	{
		if ( ((0xD7 == pBuffer[0]) && (0xCD == pBuffer[1]) && (0xC6 == pBuffer[2]) && (0x9A == pBuffer[3])&& (0x00 == pBuffer[4]) && (0x00 == pBuffer[5]) ) || 
			((0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x09 == pBuffer[2]) && (0x00 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x03 == pBuffer[5]) ))
			return TRUE;
	}

	//emf ( http://wvware.sourceforge.net/caolan/ora-wmf.html )
	//Hex: 01 00 00 00
	//Hex (position 40): 20 45 4D 46
	if ( (44 <= dwBytesRead) && (0x01 == pBuffer[0]) && (0x00 == pBuffer[1]) && (0x00 == pBuffer[2]) && (0x00 == pBuffer[3]) && 
		 (0x20 == pBuffer[40]) && (0x45 == pBuffer[41]) && (0x4D == pBuffer[42]) && (0x46 == pBuffer[43]) )
		return TRUE;

	//pcx ( http://www.fileformat.info/format/pcx/corion.htm )
	//Hex (position 0): 0A
	//Hex (position 1): 00 || 01 || 02 || 03 || 04 || 05
	//Hex (position 3): 01 || 02 || 04 || 08 ( Bytes per pixel )
	if ( (4 <= dwBytesRead) && (0x0A == pBuffer[0]) && (0x00 == pBuffer[1] || 0x01 == pBuffer[1] ||
			0x02 == pBuffer[1] || 0x03 == pBuffer[1] || 0x04 == pBuffer[1] || 0x05 == pBuffer[1] ) && 
		  ( 0x01 == pBuffer[3] || 0x02 == pBuffer[3] || 0x04 == pBuffer[3] || 0x08 == pBuffer[3] ))
		return _CXIMAGE_FORMAT_PCX;

	//tga ( http://www.fileformat.info/format/tga/corion.htm )
	//DATA TYPE 1-COLOR-MAPPED IMAGES								: Hex (position 1) : 01 01
	//DATA TYPE 2-TRUE-COLOR IMAGES									: Hex (position 1) : 00 02
	//DATA TYPE 3-BLACK AND WHITE(UNMAPPED) IMAGES					: Hex (position 1) : 00 03
	//DATA TYPE 9-RUN-LENGTH ENCODED(RLE),COLOR-MAPPED IMAGES		: Hex (position 1) : 01 09
	//DATA TYPE 10-RUN-LENGTH ENCODED(RLE),TRUE-COLOR IMAGES		: Hex (position 1) : 00 0A
	//DATA TYPE 11-RUN-LENGTH ENCODED(RLE),BLACK AND WHITE IMAGES	: Hex (position 1) : 00 0B
	// + Bytes per pixel											: Hex (position 16): 0x08 || 0x10 || 0x18 || 0x20
	if ( (17 <= dwBytesRead) && ( (0x01 == pBuffer[1] && 0x01 == pBuffer[2]) || (0x00 == pBuffer[1] && 0x02 == pBuffer[2]) ||
								 (0x00 == pBuffer[1] && 0x03 == pBuffer[2]) || (0x01 == pBuffer[1] && 0x09 == pBuffer[2]) ||
								 (0x00 == pBuffer[1] && 0x0A == pBuffer[2]) || (0x00 == pBuffer[1] && 0x0B == pBuffer[2]) )
							&& ( 0x08 == pBuffer[16] || 0x10 == pBuffer[16] || 0x18 == pBuffer[16] || 0x20 == pBuffer[16] ))
		return _CXIMAGE_FORMAT_TGA;

	//ras
	//Hex: 59 A6 6A 95
	//ASCII: Y
	if ( (4 <= dwBytesRead ) && (0x59 == pBuffer[0]) && (0xA6 == pBuffer[1]) && (0x6A == pBuffer[2])&& (0x95 == pBuffer[3]))
		return _CXIMAGE_FORMAT_RAS;

	//ipod 
	//(None or Unknown)

	//psd
	//Hex: 38 42 50 53 00 01 00 00 00 00 00 00 00
	//ASCII: 8BPS
	if ( (13 <= dwBytesRead ) && (0x38 == pBuffer[0]) && (0x42 == pBuffer[1]) && (0x50 == pBuffer[2])
		&& (0x53 == pBuffer[3]) && (0x00 == pBuffer[4]) && (0x01 == pBuffer[5]) && (0x00 == pBuffer[6])
		&& (0x00 == pBuffer[7]) && (0x00 == pBuffer[8]) && (0x00 == pBuffer[9]) && (0x00 == pBuffer[10])
		&& (0x00 == pBuffer[11]) && (0x00 == pBuffer[12]))
		return 0;

	return 0;
}


extern X__CacheFile Z__CacheFile;


#endif // !defined(AFX_X__CACHEFILE_H__9056C787_C29B_4E79_A6CD_29768C0CEBA7__INCLUDED_)
