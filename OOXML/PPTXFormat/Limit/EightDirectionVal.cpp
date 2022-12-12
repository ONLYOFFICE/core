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

#include "EightDirectionVal.h"

namespace PPTX
{
	namespace Limit
	{		
		EightDirectionVal::EightDirectionVal()
		{
			m_strValue = _T("l");
		}
		void EightDirectionVal::set(const std::wstring& strValue)
		{
			if ((_T("ld") == strValue) ||
				(_T("lu") == strValue) ||
				(_T("rd") == strValue) ||
				(_T("ru") == strValue) ||
				(_T("d") == strValue) ||
				(_T("l") == strValue) ||
				(_T("r") == strValue) ||
				(_T("u") == strValue))
			{
				m_strValue = strValue;
			}
		}
		BYTE EightDirectionVal::GetBYTECode() const
		{
			if (L"ld" == m_strValue)	return 0;
			if (L"lu" == m_strValue)	return 1;
			if (L"rd" == m_strValue)	return 2;
			if (L"ru" == m_strValue)	return 3;
			if (L"d" == m_strValue)		return 4;
			if (L"l" == m_strValue)		return 5;
			if (L"r" == m_strValue)		return 6;
			if (L"u" == m_strValue)		return 7;
			return 0;
		}
		void EightDirectionVal::SetBYTECode(const BYTE& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"ld"; break;
			case 1: m_strValue = L"lu"; break;
			case 2: m_strValue = L"rd"; break;
			case 3: m_strValue = L"ru"; break;
			case 4: m_strValue = L"d"; break;
			case 5: m_strValue = L"l"; break;
			case 6: m_strValue = L"r"; break;
			case 7: m_strValue = L"u"; break;
			}
		}
	} // namespace Limit
} // namespace PPTX
