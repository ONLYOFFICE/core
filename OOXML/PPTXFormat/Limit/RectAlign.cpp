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

#include "RectAlign.h"

namespace PPTX
{
	namespace Limit
	{		
		RectAlign::RectAlign()
		{
			m_strValue = L"tl";
		}
		void RectAlign::set(const std::wstring& strValue)
		{
			if ((L"b" == strValue) ||
				(L"bl" == strValue) ||
				(L"br" == strValue) ||
				(L"ctr" == strValue) ||
				(L"l" == strValue) ||
				(L"r" == strValue) ||
				(L"t" == strValue) ||
				(L"tl" == strValue) ||
				(L"tr" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char RectAlign::GetBYTECode() const
		{
			if (L"b" == m_strValue)
				return 0;
			if (L"bl" == m_strValue)
				return 1;
			if (L"br" == m_strValue)
				return 2;
			if (L"ctr" == m_strValue)
				return 3;
			if (L"l" == m_strValue)
				return 4;
			if (L"r" == m_strValue)
				return 5;
			if (L"t" == m_strValue)
				return 6;
			if (L"tl" == m_strValue)
				return 7;
			if (L"tr" == m_strValue)
				return 8;
			return 7;
		}
		void RectAlign::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:
				m_strValue = L"b";
				break;
			case 1:
				m_strValue = L"bl";
				break;
			case 2:
				m_strValue = L"br";
				break;
			case 3:
				m_strValue = L"ctr";
				break;
			case 4:
				m_strValue = L"l";
				break;
			case 5:
				m_strValue = L"r";
				break;
			case 6:
				m_strValue = L"t";
				break;
			case 7:
				m_strValue = L"tl";
				break;
			case 8:
				m_strValue = L"tr";
				break;
			default:
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
