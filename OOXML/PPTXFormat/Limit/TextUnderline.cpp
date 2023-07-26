/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "TextUnderline.h"

namespace PPTX
{
	namespace Limit
	{		
		TextUnderline::TextUnderline()
		{
			m_strValue = L"sng";
		}
		void TextUnderline::set(const std::wstring& strValue)
		{
			if ((L"dash" == strValue) ||
				(L"dashHeavy" == strValue) ||
				(L"dashLong" == strValue) ||
				(L"dashLongHeavy" == strValue) ||
				(L"dbl" == strValue) ||
				(L"dotDash" == strValue) ||
				(L"dotDashHeavy" == strValue) ||
				(L"dotDotDash" == strValue) ||
				(L"dotDotDashHeavy" == strValue) ||
				(L"dotted" == strValue) ||
				(L"dottedHeavy" == strValue) ||
				(L"heavy" == strValue) ||
				(L"none" == strValue) ||
				(L"sng" == strValue) ||
				(L"wavy" == strValue) ||
				(L"wavyDbl" == strValue) ||
				(L"wavyHeavy" == strValue) ||
				(L"words" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char TextUnderline::GetBYTECode() const
		{
			if (L"dash" == m_strValue)
				return 0;
			if (L"dashHeavy" == m_strValue)
				return 1;
			if (L"dashLong" == m_strValue)
				return 2;
			if (L"dashLongHeavy" == m_strValue)
				return 3;
			if (L"dbl" == m_strValue)
				return 4;
			if (L"dotDash" == m_strValue)
				return 5;
			if (L"dotDashHeavy" == m_strValue)
				return 6;
			if (L"dotDotDash" == m_strValue)
				return 7;
			if (L"dotDotDashHeavy" == m_strValue)
				return 8;
			if (L"dotted" == m_strValue)
				return 9;
			if (L"dottedHeavy" == m_strValue)
				return 10;
			if (L"heavy" == m_strValue)
				return 11;
			if (L"none" == m_strValue)
				return 12;
			if (L"sng" == m_strValue)
				return 13;
			if (L"wavy" == m_strValue)
				return 14;
			if (L"wavyDbl" == m_strValue)
				return 15;
			if (L"wavyHeavy" == m_strValue)
				return 16;
			if (L"words" == m_strValue)
				return 17;
			return 13;
		}
		void TextUnderline::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
				case 0:		m_strValue = L"dash"; break;
				case 1:		m_strValue = L"dashHeavy"; break;
				case 2:		m_strValue = L"dashLong"; break;
				case 3:		m_strValue = L"dashLongHeavy"; break;
				case 4:		m_strValue = L"dbl"; break;
				case 5:		m_strValue = L"dotDash"; break;
				case 6:		m_strValue = L"dotDashHeavy"; break;
				case 7:		m_strValue = L"dotDotDash"; break;
				case 8:		m_strValue = L"dotDotDashHeavy"; break;
				case 9:		m_strValue = L"dotted"; break;
				case 10:	m_strValue = L"dottedHeavy"; break;
				case 11:	m_strValue = L"heavy"; break;
				case 12:	m_strValue = L"none"; break;
				case 13:	m_strValue = L"sng"; break;
				case 14:	m_strValue = L"wavy"; break;
				case 15:	m_strValue = L"wavyDbl"; break;
				case 16:	m_strValue = L"wavyHeavy"; break;
				case 17:	m_strValue = L"words"; break;
			default:
				m_strValue = L"sng";
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
