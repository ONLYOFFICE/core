/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "Path.h"
#include "File.h"
#include <vector>
#include <cwctype>

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

	template<class CHAR, class STRING = std::basic_string<CHAR, std::char_traits<CHAR>, std::allocator<CHAR>>>
	STRING NormalizePathTemplate(const STRING& strFileName, const bool& canHead = false)
	{
		const CHAR* pData = strFileName.c_str();
		int nLen          = (int) strFileName.length();

		CHAR* pDataNorm       = new CHAR[nLen + 1];
		int*  pSlashPoints    = new int[nLen + 1];

		int nStart          = 0;
		int nCurrent        = 0;
		int nCurrentSlash   = -1;
		int nCurrentW       = 0;
		bool bIsUp          = false;

		if (canHead)
		{
			nCurrentSlash = 0;
			pSlashPoints[0] = 0;
		}

		if (pData[nCurrent] == '/' || pData[nCurrent] == '\\')
		{
#if !defined(_WIN32) && !defined (_WIN64)
			pDataNorm[nCurrentW++] = pData[nCurrent];
#endif
			++nCurrentSlash;
			pSlashPoints[nCurrentSlash] = nCurrentW;
		}

		while (nCurrent < nLen)
		{
			if (pData[nCurrent] == '/' || pData[nCurrent] == '\\')
			{
				if (nStart < nCurrent)
				{
					bIsUp = false;
					if ((nCurrent - nStart) == 2)
					{
						if (pData[nStart] == (CHAR)'.' && pData[nStart + 1] == (CHAR)'.')
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
						pDataNorm[nCurrentW++] = (CHAR)'/';
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

		pDataNorm[nCurrentW] = (CHAR)'\0';

		STRING result = STRING(pDataNorm, nCurrentW);

		delete[] pDataNorm;
		delete[] pSlashPoints;

		return result;
	}

	std::string NormalizePath(const std::string& strFileName, const bool& canHead)
	{
		return NormalizePathTemplate<char>(strFileName, canHead);
	}
	std::wstring NormalizePath(const std::wstring& strFileName, const bool& canHead)
	{
		return NormalizePathTemplate<wchar_t>(strFileName, canHead);
	}

	static bool IsPathSeparator(const wchar_t& wchSymbol)
	{
		return L'/' == wchSymbol || L'\\' == wchSymbol;
	}

	static bool IsDriveLetter(const wchar_t& wchSymbol)
	{
		return (L'a' <= wchSymbol && wchSymbol <= L'z') || (L'A' <= wchSymbol && wchSymbol <= L'Z');
	}

	static size_t GetRootLength(const std::wstring& wsPath)
	{
		const size_t nLength = wsPath.length();

		if (0 == nLength)
			return 0;

		if (nLength > 1 && L':' == wsPath[1] && IsDriveLetter(wsPath[0]))
			return (nLength > 2 && IsPathSeparator(wsPath[2])) ? 3 : 2;

		if (IsPathSeparator(wsPath[0]))
			return (nLength > 1 && IsPathSeparator(wsPath[1])) ? 2 : 1;

		return 0;
	}

	struct TNormalizedPath
	{
		std::wstring              m_wsRoot;
		std::vector<std::wstring> m_arParts;
		size_t                    m_nLeadingUp;
		bool                      m_bAbsolute;

		TNormalizedPath() : m_nLeadingUp(0), m_bAbsolute(false) {}
	};

	static TNormalizedPath SplitNormalized(const std::wstring& wsPath)
	{
		TNormalizedPath oResult;

		const size_t nRootLength = GetRootLength(wsPath);

		for (size_t i = 0; i < nRootLength; ++i)
			oResult.m_wsRoot += IsPathSeparator(wsPath[i]) ? L'/' : wsPath[i];

		oResult.m_bAbsolute = !oResult.m_wsRoot.empty() &&
		                      IsPathSeparator(oResult.m_wsRoot[oResult.m_wsRoot.length() - 1]);

		std::wstring wsToken;

		for (size_t i = nRootLength; i <= wsPath.length(); ++i)
		{
			if (i < wsPath.length() && !IsPathSeparator(wsPath[i]))
			{
				wsToken += wsPath[i];
				continue;
			}

			if (L".." == wsToken)
			{
				if (!oResult.m_arParts.empty())
					oResult.m_arParts.pop_back();
				else if (!oResult.m_bAbsolute)
					++oResult.m_nLeadingUp;
			}
			else if (L"." != wsToken && !wsToken.empty())
				oResult.m_arParts.push_back(wsToken);

			wsToken.clear();
		}

		return oResult;
	}

	static bool EqualComponent(const std::wstring& wsFirst, const std::wstring& wsSecond)
	{
		#if defined(_WIN32) || defined(_WIN64)
		if (wsFirst.length() != wsSecond.length())
			return false;

		for (size_t i = 0; i < wsFirst.length(); ++i)
			if (towlower(wsFirst[i]) != towlower(wsSecond[i]))
				return false;

		return true;
		#else
		return wsFirst == wsSecond;
		#endif
	}

	std::wstring ShortenPath(const std::wstring &wsPath, const bool& bRemoveExternalPath)
	{
		const TNormalizedPath oPath{SplitNormalized(wsPath)};

		std::wstring wsNewPath = oPath.m_wsRoot;
		size_t nWritten = 0;

		if (!bRemoveExternalPath)
		{
			for (size_t i = 0; i < oPath.m_nLeadingUp; ++i)
			{
				if (0 != nWritten++)
					wsNewPath += L'/';

				wsNewPath += L"..";
			}
		}

		for (size_t i = 0; i < oPath.m_arParts.size(); ++i)
		{
			if (0 != nWritten++)
				wsNewPath += L'/';

			wsNewPath += oPath.m_arParts[i];
		}

		return wsNewPath;
	}

	bool IsPathInDirectory(const std::wstring& wsDirectory, const std::wstring& wsPath)
	{
		if (wsDirectory.empty())
			return false;

		const TNormalizedPath oDirectory{SplitNormalized(wsDirectory)};
		const TNormalizedPath oPath     {SplitNormalized(wsPath)};

		if (oDirectory.m_bAbsolute != oPath.m_bAbsolute)
			return false;

		if (!EqualComponent(oDirectory.m_wsRoot, oPath.m_wsRoot))
			return false;

		if (0 != oDirectory.m_nLeadingUp || 0 != oPath.m_nLeadingUp)
			return false;

		if (oPath.m_arParts.size() < oDirectory.m_arParts.size())
			return false;

		for (size_t i = 0; i < oDirectory.m_arParts.size(); ++i)
			if (!EqualComponent(oDirectory.m_arParts[i], oPath.m_arParts[i]))
				return false;

		return true;
	}

	std::wstring ResolveInDirectory(const std::wstring& wsBoundaryDirectory, const std::wstring& wsBaseDirectory, const std::wstring& wsRelativePath)
	{
		if (wsBoundaryDirectory.empty() || wsRelativePath.empty())
			return std::wstring();

		size_t nStart = 0;
		while (nStart < wsRelativePath.length() && IsPathSeparator(wsRelativePath[nStart]))
			++nStart;

		const std::wstring wsDocumentPath = wsRelativePath.substr(nStart);

		if (wsDocumentPath.empty() || 0 != GetRootLength(wsDocumentPath))
			return std::wstring();

		const std::wstring wsFullPath{ShortenPath(Combine(wsBaseDirectory, wsDocumentPath))};

		if (!IsPathInDirectory(wsBoundaryDirectory, wsFullPath))
			return std::wstring();

		return wsFullPath;
	}
	
}
