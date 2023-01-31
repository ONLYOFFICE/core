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

#include "SchemeClrVal.h"

namespace PPTX
{
	namespace Limit
	{		
		SchemeClrVal::SchemeClrVal()
		{
			m_strValue = L"accent1";
		}
		void SchemeClrVal::set(const std::wstring& strValue)
		{
			if ((L"accent1" == strValue) || // (Accent Color 1)  Extra scheme color 1
				(L"accent2" == strValue) || // (Accent Color 2)  Extra scheme color 2
				(L"accent3" == strValue) || // (Accent Color 3)  Extra scheme color 3
				(L"accent4" == strValue) || // (Accent Color 4)  Extra scheme color 4
				(L"accent5" == strValue) || // (Accent Color 5)  Extra scheme color 5
				(L"accent6" == strValue) || // (Accent Color 6)  Extra scheme color 6
				(L"bg1" == strValue) || // (Background Color 1)  Semantic background color
				(L"bg2" == strValue) || // (Background Color 2)  Semantic additional background color
				(L"dk1" == strValue) || // (Dark Color 1)  Main dark color 1
				(L"dk2" == strValue) || // (Dark Color 2)  Main dark color 2
				(L"folHlink" == strValue) || // (Followed Hyperlink Color)  Followed Hyperlink Color
				(L"hlink" == strValue) || // (Hyperlink Color)  Regular Hyperlink Color
				(L"lt1" == strValue) || // (Light Color 1)  Main Light Color 1
				(L"lt2" == strValue) || // (Light Color 2)  Main Light Color 2
				(L"phClr" == strValue) || // (Style Color)  A color used in theme definitions which means to use
				(L"tx1" == strValue) || // (Text Color 1)  Semantic text color
				(L"tx2" == strValue)) // (Text Color 2)  Semantic additional text color
			{
				m_strValue = strValue;
			}
		}
		BYTE SchemeClrVal::GetBYTECode() const
		{
			if (L"accent1" == m_strValue)	return 0;
			if (L"accent2" == m_strValue)	return 1;
			if (L"accent3" == m_strValue)	return 2;
			if (L"accent4" == m_strValue)	return 3;
			if (L"accent5" == m_strValue)	return 4;
			if (L"accent6" == m_strValue)	return 5;
			if (L"bg1" == m_strValue)		return 6;
			if (L"bg2" == m_strValue)		return 7;
			if (L"dk1" == m_strValue)		return 8;
			if (L"dk2" == m_strValue)		return 9;
			if (L"folHlink" == m_strValue)	return 10;
			if (L"hlink" == m_strValue)		return 11;
			if (L"lt1" == m_strValue)		return 12;
			if (L"lt2" == m_strValue)		return 13;
			if (L"phClr" == m_strValue)		return 14;
			if (L"tx1" == m_strValue)		return 15;
			if (L"tx2" == m_strValue)		return 16;

			return 0;
		}
		void SchemeClrVal::SetBYTECode(const BYTE& code)
		{
			switch (code)
			{
				case 0:  m_strValue = L"accent1";  break;
				case 1:  m_strValue = L"accent2";  break;
				case 2:  m_strValue = L"accent3";  break;
				case 3:  m_strValue = L"accent4";  break;
				case 4:  m_strValue = L"accent5";  break;
				case 5:  m_strValue = L"accent6";  break;
				case 6:  m_strValue = L"bg1";  break;
				case 7:  m_strValue = L"bg2";  break;
				case 8:  m_strValue = L"dk1";  break;
				case 9:  m_strValue = L"dk2";  break;
				case 10:  m_strValue = L"folHlink";  break;
				case 11:  m_strValue = L"hlink";  break;
				case 12:  m_strValue = L"lt1";  break;
				case 13:  m_strValue = L"lt2";  break;
				case 14:  m_strValue = L"phClr";  break;
				case 15:  m_strValue = L"tx1";  break;
				case 16:  m_strValue = L"tx2";  break;
			}
		}
	} // namespace Limit
} // namespace PPTX

