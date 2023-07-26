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

#include "SlideSize.h"

namespace PPTX
{
	namespace Limit
	{		
		SlideSize::SlideSize()
		{
			m_strValue = L"screen4x3";
		}
		void SlideSize::set(const std::wstring& strValue)
		{
			if ((L"35mm" == strValue) ||
				(L"A3" == strValue) ||
				(L"A4" == strValue) ||
				(L"B4ISO" == strValue) ||
				(L"B4JIS" == strValue) ||
				(L"B5ISO" == strValue) ||
				(L"B5JIS" == strValue) ||
				(L"banner" == strValue) ||
				(L"custom" == strValue) ||
				(L"hagakiCard" == strValue) ||
				(L"ledger" == strValue) ||
				(L"letter" == strValue) ||
				(L"overhead" == strValue) ||
				(L"screen16x10" == strValue) ||
				(L"screen16x9" == strValue) ||
				(L"screen4x3" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char SlideSize::GetBYTECode() const
		{
			if (L"35mm" == m_strValue) return 0;
			if (L"A3" == m_strValue) return 1;
			if (L"A4" == m_strValue) return 2;
			if (L"B4ISO" == m_strValue) return 3;
			if (L"B4JIS" == m_strValue) return 4;
			if (L"B5ISO" == m_strValue) return 5;
			if (L"B5JIS" == m_strValue) return 6;
			if (L"banner" == m_strValue) return 7;
			if (L"custom" == m_strValue) return 8;
			if (L"hagakiCard" == m_strValue) return 9;
			if (L"ledger" == m_strValue) return 10;
			if (L"letter" == m_strValue) return 11;
			if (L"overhead" == m_strValue) return 12;
			if (L"screen16x10" == m_strValue) return 13;
			if (L"screen16x9" == m_strValue) return 14;
			if (L"screen4x3" == m_strValue) return 15;

			return 15;
		}
		void SlideSize::SetBYTECode(const unsigned char& type)
		{
			switch (type)
			{
			case 0: m_strValue = L"35mm"; break;
			case 1: m_strValue = L"A3"; break;
			case 2: m_strValue = L"A4"; break;
			case 3: m_strValue = L"B4ISO"; break;
			case 4: m_strValue = L"B4JIS"; break;
			case 5: m_strValue = L"B5ISO"; break;
			case 6: m_strValue = L"B5JIS"; break;
			case 7: m_strValue = L"banner"; break;
			case 8: m_strValue = L"custom"; break;
			case 9: m_strValue = L"hagakiCard"; break;
			case 10: m_strValue = L"ledger"; break;
			case 11: m_strValue = L"letter"; break;
			case 12: m_strValue = L"overhead"; break;
			case 13: m_strValue = L"screen16x10"; break;
			case 14: m_strValue = L"screen16x9"; break;
			case 15: m_strValue = L"screen4x3"; break;
			default:
				m_strValue = L"screen4x3";
			}
		}
	} // namespace Limit
} // namespace PPTX
