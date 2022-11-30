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

#include "PresetShadowVal.h"

namespace PPTX
{
	namespace Limit
	{		
		PresetShadowVal::PresetShadowVal()
		{
			m_strValue = L"shdw1";
		}
		void PresetShadowVal::set(const std::wstring& strValue)
		{
			if ((L"shdw1" == strValue) ||
				(L"shdw2" == strValue) ||
				(L"shdw3" == strValue) ||
				(L"shdw4" == strValue) ||
				(L"shdw5" == strValue) ||
				(L"shdw6" == strValue) ||
				(L"shdw7" == strValue) ||
				(L"shdw8" == strValue) ||
				(L"shdw9" == strValue) ||
				(L"shdw10" == strValue) ||
				(L"shdw11" == strValue) ||
				(L"shdw12" == strValue) ||
				(L"shdw13" == strValue) ||
				(L"shdw14" == strValue) ||
				(L"shdw15" == strValue) ||
				(L"shdw16" == strValue) ||
				(L"shdw17" == strValue) ||
				(L"shdw18" == strValue) ||
				(L"shdw19" == strValue) ||
				(L"shdw20" == strValue))
			{
				m_strValue = strValue;
			}
		}
		BYTE PresetShadowVal::GetBYTECode() const
		{
			if (L"shdw1" == m_strValue)		return 0;
			if (L"shdw2" == m_strValue)		return 1;
			if (L"shdw3" == m_strValue)		return 2;
			if (L"shdw4" == m_strValue)		return 3;
			if (L"shdw5" == m_strValue)		return 4;
			if (L"shdw6" == m_strValue)		return 5;
			if (L"shdw7" == m_strValue)		return 6;
			if (L"shdw8" == m_strValue)		return 7;
			if (L"shdw9" == m_strValue)		return 8;
			if (L"shdw10" == m_strValue)	return 9;
			if (L"shdw11" == m_strValue)	return 10;
			if (L"shdw12" == m_strValue)	return 11;
			if (L"shdw13" == m_strValue)	return 12;
			if (L"shdw14" == m_strValue)	return 13;
			if (L"shdw15" == m_strValue)	return 14;
			if (L"shdw16" == m_strValue)	return 15;
			if (L"shdw17" == m_strValue)	return 16;
			if (L"shdw18" == m_strValue)	return 17;
			if (L"shdw19" == m_strValue)	return 18;
			if (L"shdw20" == m_strValue)	return 19;

			return 0;
		}
		void PresetShadowVal::SetBYTECode(const BYTE& src)
		{
			switch (src)
			{
				case 0: m_strValue = L"shdw1"; break;
				case 1: m_strValue = L"shdw2"; break;
				case 2: m_strValue = L"shdw3"; break;
				case 3: m_strValue = L"shdw4"; break;
				case 4: m_strValue = L"shdw5"; break;
				case 5: m_strValue = L"shdw6"; break;
				case 6: m_strValue = L"shdw7"; break;
				case 7: m_strValue = L"shdw8"; break;
				case 8: m_strValue = L"shdw9"; break;
				case 9: m_strValue = L"shdw10"; break;
				case 10: m_strValue = L"shdw11"; break;
				case 11: m_strValue = L"shdw12"; break;
				case 12: m_strValue = L"shdw13"; break;
				case 13: m_strValue = L"shdw14"; break;
				case 14: m_strValue = L"shdw15"; break;
				case 15: m_strValue = L"shdw16"; break;
				case 16: m_strValue = L"shdw17"; break;
				case 17: m_strValue = L"shdw18"; break;
				case 18: m_strValue = L"shdw19"; break;
				case 19: m_strValue = L"shdw20"; break;

			}
		}
	} // namespace Limit
} // namespace PPTX
