/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include <boost/algorithm/string.hpp>

#include <stdlib.h>

#include "../../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../../DesktopEditor/graphics/GraphicsPath.h"

const int		ShapeSizeVML	= 21600;
const double	RadKoef			= M_PI/10800000.0;

#ifndef		pow2_16
#define		pow2_16			65536
#endif

#ifndef		pow3_16
#define		pow3_16			60000
#endif

namespace NSStringUtils
{
	#define IS_ALPHA(c) (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
	#define IS_DIGIT(c) (((c >= '0') && (c <= '9')) || (c == '-'))

	static bool IsDigit(const TCHAR& c)
	{
		return (((c >= '0') && (c <= '9')) || (c == '-'));
	}
	static bool IsAlpha(const TCHAR& c)
	{
		return (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')));
	}
	static bool IsNumber(std::wstring str)
	{
		for (int nIndex = 0; nIndex < str.length(); ++nIndex)
		{
			if (!IsDigit(str[nIndex]))
			{
				return false;
			}
		}
		return true;
	}

	static std::wstring ToString(LONG val)
	{
#if defined(_WIN32) || defined (_WIN64)
            wchar_t buff[32] ={};
            _itow(val, buff, 10);
            return std::wstring(buff);
#else
            return (std::to_wstring(val));
#endif
	}
	
	static void ParseString(std::wstring strDelimeters, std::wstring strSource, 
		std::vector<std::wstring>& pArrayResults, bool bIsCleared = true)
	{
		if (bIsCleared)
			pArrayResults.clear();


		boost::algorithm::split(pArrayResults, strSource, boost::algorithm::is_any_of(strDelimeters), boost::algorithm::token_compress_on);
		
		while (pArrayResults.size() > 0)
		{
			if (pArrayResults[pArrayResults.size() - 1].empty())
				pArrayResults.erase(pArrayResults.begin() + pArrayResults.size() - 1);
			else
				break;
		}

	}
	//static void ParseString(std::vector<char>& pArrayDelimeters, std::wstring strSource, 
	//	std::vector<std::wstring> & pArrayResults, bool bIsCleared = true)
	//{
	//	std::wstring strDelimeters = _T("");
	//	for (int nIndex = 0; nIndex < pArrayDelimeters.size(); ++nIndex)
	//		strDelimeters += pArrayDelimeters[nIndex];

	//	return ParseString(strDelimeters, strSource, pArrayResults, bIsCleared);
	//}
	static void ParsePath(std::wstring strSource, std::vector<std::wstring> pArrayResults, bool bIsCleared = true)
	{
		std::wstring strPath = strSource;
		//strPath.Replace(_T(" "), _T(","));
		for (int nIndex = 0; nIndex < strPath.length(); ++nIndex)
		{
			if (nIndex == (strPath.length() - 1))
				continue;

			if (IsAlpha(strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
				strPath.insert(strPath.begin() + nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && (',' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, '0');
				++nIndex;
			}
			else if ((',' == strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, '0');
				++nIndex;
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && IsDigit(strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('@' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('#' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsDigit(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('$' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if (IsAlpha(strPath[nIndex]) && ('?' == strPath[nIndex + 1]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
			else if ((IsAlpha(strPath[nIndex]) && IsAlpha(strPath[nIndex + 1])) && ('x' == strPath[nIndex]))
			{
				strPath.insert(strPath.begin() + nIndex + 1, ',');
				++nIndex;
			}
		}

		ParseString(_T(","), strPath, pArrayResults, bIsCleared);
		return;
	}

	static void ParsePath2(std::wstring strSource, std::vector<std::wstring>* pArrayResults, bool bIsCleared = true)
    {
        if (NULL == pArrayResults)
            return;
        
        std::wstring strPath = strSource;
		int nLength = strPath.length();
        //strPath.Replace(_T(" "), _T(","));
		if (strPath.find(TCHAR('h')) != -1)
		{
			TCHAR* pBuff = new TCHAR[nLength + 1];
			int nCur = 0;
			for (int i = 1; i < nLength; ++i)
			{
				TCHAR _c = strPath[i - 1];
				if (_c != TCHAR('h'))
				{
					pBuff[nCur++] = _c;
				}
				else
				{
					TCHAR _c1 = strPath[i];
					if (_c1 == TCHAR('a') ||
						_c1 == TCHAR('b') ||
						_c1 == TCHAR('c') ||
						_c1 == TCHAR('d') ||
						_c1 == TCHAR('e') ||
						_c1 == TCHAR('f') ||
						_c1 == TCHAR('g') ||
						_c1 == TCHAR('h') ||
						_c1 == TCHAR('i'))
					{
						++i;
					}
				}
			}

			if (nLength > 0)
				pBuff[nCur++] = strPath[nLength - 1];

			pBuff[nCur] = 0;

			strPath = std::wstring(pBuff, nCur);
			nLength = strPath.length();

			if (NULL != pBuff)
			{
				delete []pBuff;
				pBuff = NULL;
			}
		}

		if (nLength > 0 && strPath[nLength - 1] == TCHAR(','))
		{
			strPath += _T("0");
			++nLength;
		}

        int nIndexOld = 0;
        for (int nIndex = 0; nIndex < nLength; ++nIndex)
        {
            if (nIndex == (nLength - 1))
			{
				pArrayResults->push_back(strPath.substr(nIndexOld));
				//continue;
			}

			TCHAR _c	= strPath[nIndex];
			TCHAR _c1	= strPath[nIndex + 1];

			if (_c1 == ',')
			{
				if (',' == _c)
				{
					pArrayResults->push_back(_T("0"));
				}
				else if (IS_ALPHA(_c))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));
					pArrayResults->push_back(_T("0"));
				}
				else if (IS_DIGIT(_c))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));
				}
			}
			else if (',' == _c)
			{
				if (IS_ALPHA(_c1))
				{
					pArrayResults->push_back(_T("0"));
					nIndexOld = nIndex + 1;
				}
				else if (IS_DIGIT(_c1))
				{
					nIndexOld = nIndex + 1;
				}
			}
			else
			{
				bool _isA = IS_ALPHA(_c);
				bool _isD = _isA ? false : IS_DIGIT(_c);

				bool _isA1 = IS_ALPHA(_c1);
				bool _isD1 = _isA1 ? false : IS_DIGIT(_c1);

				if (_isA && _isD1)
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && _isA1)
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));
					nIndexOld = nIndex + 1;
				}
				else if (_isD && ('@' == _c1))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isD && ('#' == _c1))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('@' == _c1))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (_isA && ('#' == _c1))
				{
					pArrayResults->push_back(strPath.substr(nIndexOld, nIndex - nIndexOld + 1));

					++nIndex;
					nIndexOld = nIndex;
				}
				else if (('x' == _c) && _isA1)
				{
					pArrayResults->push_back(_T("x"));
					nIndexOld = nIndex + 1;
				}				
			}            
        }

        //ParseString(_T(","), strPath, pArrayResults, bIsCleared);
        return;
    }

	static void CheckPathOn_Fill_Stroke(std::wstring& strPath, bool& bFill, bool& bStroke)
	{
		bFill = true;
		bStroke = true;
		int nIndex = strPath.find(_T("nf"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.erase(nIndex, 2);
				nIndex = strPath.find(_T("nf"));
			}
		}
		nIndex = strPath.find(_T("ns"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.erase(nIndex, 2);
				nIndex = strPath.find(_T("ns"));
			}
		}

		nIndex = strPath.find(_T("F"));
		if (-1 != nIndex)
		{
			bFill = false;
			while (-1 != nIndex)
			{
				strPath.erase(nIndex, 2);
				nIndex = strPath.find(_T("F"));
			}
		}

		nIndex = strPath.find(_T("S"));
		if (-1 != nIndex)
		{
			bStroke = false;
			while (-1 != nIndex)
			{
				strPath.erase(nIndex, 2);
				nIndex = strPath.find(_T("S"));
			}
		}
	}
}
