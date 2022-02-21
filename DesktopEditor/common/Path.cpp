/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Path.h"
#include "File.h"

#if defined(_WIN32) || defined (_WIN64)
    #include <tchar.h>
#elif __linux__ || MAC
    #include <libgen.h>
#endif

namespace NSSystemPath
{
	std::wstring GetDirectoryName(const std::wstring& strFileName)
	{
		std::wstring sRes;
		//_wsplitpath return directory path, including trailing slash.
		//dirname() returns the string up to, but not including, the final '/',
#if defined(_WIN32) || defined (_WIN64)
		wchar_t tDrive[256];
		wchar_t tFolder[256];
		_wsplitpath( strFileName.c_str(), tDrive, tFolder, NULL, NULL );
		sRes.append(tDrive);
		sRes.append(tFolder);
		if(sRes.length() > 0)
			sRes.erase(sRes.length()-1);
#elif __linux__ || MAC
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
		char* pDirName = dirname((char*)pUtf8);
		sRes = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pDirName, strlen(pDirName));
		delete [] pUtf8;
#endif
        return sRes;
	}
	std::wstring GetFileName(const std::wstring& strFileName)
	{
		std::wstring sRes;
#if defined(_WIN32) || defined (_WIN64)
		wchar_t tFilename[256];
		wchar_t tExt[256];
		_wsplitpath( strFileName.c_str(), NULL, NULL, tFilename, tExt );
		sRes.append(tFilename);
		sRes.append(tExt);
		return sRes;
#elif __linux__ || MAC
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
		NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
		char* pBaseName = basename((char*)pUtf8);
		sRes = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)pBaseName, strlen(pBaseName));
		delete [] pUtf8;
#endif
		return sRes;
	}
	std::wstring Combine(const std::wstring& strLeft, const std::wstring& strRight)
	{
		std::wstring sRes;
		bool bLeftSlash = false;
		bool bRightSlash = false;
		if(strLeft.length() > 0)
		{
			wchar_t cLeft = strLeft[strLeft.length() - 1];
			bLeftSlash = ('/' == cLeft) || ('\\' == cLeft);
		}
		if(strRight.length() > 0)
		{
			wchar_t cRight = strRight[0];
			bRightSlash = ('/' == cRight) || ('\\' == cRight);
		}
		if(bLeftSlash && bRightSlash)
		{
			sRes = strLeft + strRight.substr(1);
		}
		else if(!bLeftSlash && !bRightSlash)
            sRes = strLeft + L"/" + strRight;
		else
			sRes = strLeft + strRight;
		return sRes;
	}
	std::string NormalizePath(const std::string& strFileName)
	{
		const char*   pData   = strFileName.c_str();
		int           nLen    = (int) strFileName.length();

		char* pDataNorm       = new char[nLen + 1];
		int*  pSlashPoints    = new int[nLen + 1];

		int nStart          = 0;
		int nCurrent        = 0;
		int nCurrentSlash   = -1;
		int nCurrentW       = 0;
		bool bIsUp          = false;

	#if !defined(_WIN32) && !defined (_WIN64)
		if (pData[nCurrent] == '/' || pData[nCurrent] == '\\')
			pDataNorm[nCurrentW++] = pData[nCurrent];
	#endif
		while (nCurrent < nLen)
		{
			if (pData[nCurrent] == '/' || pData[nCurrent] == '\\')
			{
				if (nStart < nCurrent)
				{
					bIsUp = false;
					if ((nCurrent - nStart) == 2)
					{
						if (pData[nStart] == (char)'.' && pData[nStart + 1] == (char)'.')
						{
							if (nCurrentSlash > 0)
							{
								--nCurrentSlash;
								nCurrentW = pSlashPoints[nCurrentSlash];
								bIsUp = true;
							}
						}
					}
					if (!bIsUp)
					{
						pDataNorm[nCurrentW++] = (char)'/';
						++nCurrentSlash;
						pSlashPoints[nCurrentSlash] = nCurrentW;
					}
				}
				nStart = nCurrent + 1;
				++nCurrent;
				continue;
			}
			pDataNorm[nCurrentW++] = pData[nCurrent];
			++nCurrent;
		}

		pDataNorm[nCurrentW] = (char)'\0';

		std::string result = std::string(pDataNorm, nCurrentW);

		delete[] pDataNorm;
		delete[] pSlashPoints;

		return result;
	}
}
