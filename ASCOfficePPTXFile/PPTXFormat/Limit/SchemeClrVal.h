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
#pragma once
#ifndef PPTX_LIMIT_SCHEMECLRVAL_INCLUDE_H_
#define PPTX_LIMIT_SCHEMECLRVAL_INCLUDE_H_

#include "BaseLimit.h"
#if !defined(_WIN32) && !defined (_WIN64)
#include "../../../DesktopEditor/common/Types.h"
#endif


namespace PPTX
{
	namespace Limit
	{
		class SchemeClrVal : public BaseLimit
		{
		public:
			SchemeClrVal()
			{
				m_strValue = _T("accent1");
	 		}

			_USE_STRING_OPERATOR
				
			virtual void set(const std::wstring& strValue)
			{
				if ((_T("accent1") == strValue) || // (Accent Color 1)  Extra scheme color 1 
					(_T("accent2") == strValue) || // (Accent Color 2)  Extra scheme color 2 
					(_T("accent3") == strValue) || // (Accent Color 3)  Extra scheme color 3 
					(_T("accent4") == strValue) || // (Accent Color 4)  Extra scheme color 4 
					(_T("accent5") == strValue) || // (Accent Color 5)  Extra scheme color 5 
					(_T("accent6") == strValue) || // (Accent Color 6)  Extra scheme color 6 
					(_T("bg1") == strValue) || // (Background Color 1)  Semantic background color 
					(_T("bg2") == strValue) || // (Background Color 2)  Semantic additional background color 
					(_T("dk1") == strValue) || // (Dark Color 1)  Main dark color 1 
					(_T("dk2") == strValue) || // (Dark Color 2)  Main dark color 2 
					(_T("folHlink") == strValue) || // (Followed Hyperlink Color)  Followed Hyperlink Color 
					(_T("hlink") == strValue) || // (Hyperlink Color)  Regular Hyperlink Color 
					(_T("lt1") == strValue) || // (Light Color 1)  Main Light Color 1 
					(_T("lt2") == strValue) || // (Light Color 2)  Main Light Color 2 
					(_T("phClr") == strValue) || // (Style Color)  A color used in theme definitions which means to use 
					(_T("tx1") == strValue) || // (Text Color 1)  Semantic text color 
					(_T("tx2") == strValue)) // (Text Color 2)  Semantic additional text color 
				{
					m_strValue = strValue;
				}
			}

			virtual BYTE GetBYTECode() const
			{
				if (_T("accent1") == m_strValue)
					return 0;
				if (_T("accent2") == m_strValue)
					return 1;
				if (_T("accent3") == m_strValue)
					return 2;
				if (_T("accent4") == m_strValue)
					return 3;
				if (_T("accent5") == m_strValue)
					return 4;
				if (_T("accent6") == m_strValue)
					return 5;

				if (_T("bg1") == m_strValue)
					return 6;
				if (_T("bg2") == m_strValue)
					return 7;

				if (_T("dk1") == m_strValue)
					return 8;
				if (_T("dk2") == m_strValue)
					return 9;

				if (_T("folHlink") == m_strValue)
					return 10;
				if (_T("hlink") == m_strValue)
					return 11;

				if (_T("lt1") == m_strValue)
					return 12;
				if (_T("lt2") == m_strValue)
					return 13;
				if (_T("phClr") == m_strValue)
					return 14;
				if (_T("tx1") == m_strValue)
					return 15;
				if (_T("tx2") == m_strValue)
					return 16;

				return 0;
			}
		};
	} // namespace Limit
} // namespace PPTX

#endif // PPTX_LIMIT_SCHEMECLRVAL_INCLUDE_H_
