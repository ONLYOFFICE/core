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

#include "TLNodeType.h"

namespace PPTX
{
	namespace Limit
	{
		TLNodeType::TLNodeType()
		{
			m_strValue = _T("tmRoot");
		}
		void TLNodeType::set(const std::wstring& strValue)
		{
			if ((_T("afterEffect") == strValue) ||
				(_T("afterGroup") == strValue) ||
				(_T("clickEffect") == strValue) ||
				(_T("clickPar") == strValue) ||
				(_T("interactiveSeq") == strValue) ||
				(_T("mainSeq") == strValue) ||
				(_T("tmRoot") == strValue) ||
				(_T("withEffect") == strValue) ||
				(_T("withGroup") == strValue))
			{
				m_strValue = strValue;
			}
		}
		BYTE TLNodeType::GetBYTECode() const
		{
			if (L"afterEffect" == m_strValue)	return 0;
			if (L"afterGroup" == m_strValue)	return 1;
			if (L"clickEffect" == m_strValue)	return 2;
			if (L"clickPar" == m_strValue)		return 3;
			if (L"interactiveSeq" == m_strValue)return 4;
			if (L"mainSeq" == m_strValue)		return 5;
			if (L"tmRoot" == m_strValue)		return 6;
			if (L"withEffect" == m_strValue)	return 7;
			if (L"withGroup" == m_strValue)		return 8;
			return 0;
		}
		void TLNodeType::SetBYTECode(const BYTE& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"afterEffect"; break;
			case 1: m_strValue = L"afterGroup"; break;
			case 2: m_strValue = L"clickEffect"; break;
			case 3: m_strValue = L"clickPar"; break;
			case 4: m_strValue = L"interactiveSeq"; break;
			case 5: m_strValue = L"mainSeq"; break;
			case 6: m_strValue = L"tmRoot"; break;
			case 7: m_strValue = L"withEffect"; break;
			case 8: m_strValue = L"withGroup"; break;
			}
		}
	} // namespace Limit
} // namespace PPTX
