#pragma once

#include <atlutil.h>
#include <atlstr.h>

namespace PathHelpers
{
	static inline BOOL UnescapeURL (LPCWSTR szStringIn, LPWSTR szStringOut, LPDWORD pdwStrLen, DWORD dwMaxLength)
	{
		ATLENSURE(szStringIn != NULL);
		ATLENSURE(szStringOut != NULL);
		/// convert to local code page
		BOOL bRet = FALSE;

		int nSrcLen =	(int) wcslen(szStringIn);
		int nCnt	=	WideCharToMultiByte(CP_ACP, 0, szStringIn, nSrcLen, NULL, 0, NULL, NULL); // AtlUnicodeToACP(szStringIn, nSrcLen, NULL, 0);
		if (nCnt != 0)
		{
			nCnt++;
			CHeapPtr<char> szIn;

			char szInBuf[ATL_URL_MAX_URL_LENGTH];
			char *pszIn = szInBuf;

			if (nCnt <= 0)
			{
				return FALSE;
			}

			// try to avoid allocation
			if (nCnt > ATL_URL_MAX_URL_LENGTH)
			{
				if (!szIn.AllocateBytes(nCnt))
				{
					// out of memory
					return FALSE;
				}
				pszIn = szIn;
			}

			nCnt = WideCharToMultiByte(CP_ACP, 0, szStringIn, nSrcLen, pszIn, nCnt, NULL, NULL); // AtlUnicodeToACP(szStringIn, nSrcLen, pszIn, nCnt);
			ATLASSERT( nCnt != 0 );

			pszIn[nCnt] = '\0';

			char szOutBuf[ATL_URL_MAX_URL_LENGTH];
			char *pszOut = szOutBuf;
			CHeapPtr<char> szTmp;

			// try to avoid allocation
			if (dwMaxLength > ATL_URL_MAX_URL_LENGTH)
			{
				if (!szTmp.AllocateBytes(dwMaxLength))
				{
					// out of memory
					return FALSE;
				}
				pszOut = szTmp;
			}

			DWORD dwStrLen = 0;
			bRet = AtlUnescapeUrl(pszIn, pszOut, &dwStrLen, dwMaxLength);

			if (bRet != FALSE)
			{
				// it is now safe to convert using any codepage, since there
				// are no non-ASCII characters
				_ATLTRY
				{
					Checked::wmemcpy_s(szStringOut, dwMaxLength, CA2W( pszOut ), dwStrLen);
				}
				_ATLCATCHALL()
				{
					bRet = FALSE;
				}
			}
			if (pdwStrLen)
			{
				*pdwStrLen = dwStrLen;
			}
		}

		return bRet;
	}

	static inline CString ResourceFileSystemPath (const CString& toPath)
	{
		CString sPathName	=	toPath;			

		int lIndex			=	-1;		
		lIndex				=	sPathName.Find(_T("file:///"));
		if (0 == lIndex)
			sPathName		=	sPathName.Mid(8);

		lIndex				=	sPathName.Find(_T("file://"));
		if (0 == lIndex)
			sPathName		=	sPathName.Mid(7);

		lIndex				=	sPathName.Find(_T("file:\\"));
		if (0 == lIndex)
			sPathName		=	sPathName.Mid(6);

		DWORD dwLength		=	0;
		CString filePath;

		if (UnescapeURL (sPathName, filePath.GetBuffer(4096), &dwLength, 4096))
			sPathName		=	filePath;

		filePath.ReleaseBuffer();

		return sPathName;
	}

	static inline CString ResourceFileSystemPathXml (const CString& toPath)
	{
		CString sPathName	=	ResourceFileSystemPath (toPath);			

		sPathName.Replace ( _T("&"),  _T("&amp;") );
		sPathName.Replace ( _T("'"),  _T("&apos;") );
		sPathName.Replace ( _T("<"),  _T("&lt;") );
		sPathName.Replace ( _T(">"),  _T("&gt;") );
		sPathName.Replace ( _T("\""), _T("&quot;") );

		return sPathName;
	}
}