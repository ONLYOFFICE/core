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

#include "TLTriggerEvent.h"

namespace PPTX
{
	namespace Limit
	{
		TLTriggerEvent::TLTriggerEvent()
		{
			m_strValue = L"begin";
		}
		void TLTriggerEvent::set(const std::wstring& strValue)
		{
			if ((L"begin" == strValue) ||
				(L"end" == strValue) ||
				(L"onBegin" == strValue) ||
				(L"onClick" == strValue) ||
				(L"onDblClick" == strValue) ||
				(L"onEnd" == strValue) ||
				(L"onMouseOut" == strValue) ||
				(L"onMouseOver" == strValue) ||
				(L"onNext" == strValue) ||
				(L"onPrev" == strValue) ||
				(L"onStopAudio" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char TLTriggerEvent::GetBYTECode() const
		{
			if (L"begin" == m_strValue)		return 0;
			if (L"end" == m_strValue)		return 1;
			if (L"onBegin" == m_strValue)	return 2;
			if (L"onClick" == m_strValue)	return 3;
			if (L"onDblClick" == m_strValue)	return 4;
			if (L"onEnd" == m_strValue)			return 5;
			if (L"onMouseOut" == m_strValue)	return 6;
			if (L"onMouseOver" == m_strValue)	return 7;
			if (L"onNext" == m_strValue)		return 8;
			if (L"onPrev" == m_strValue)		return 9;
			if (L"onStopAudio" == m_strValue)	return 10;

			return 0;
		}
		void TLTriggerEvent::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"begin"; break;
			case 1: m_strValue = L"end"; break;
			case 2: m_strValue = L"onBegin"; break;
			case 3: m_strValue = L"onClick"; break;
			case 4: m_strValue = L"onDblClick"; break;
			case 5: m_strValue = L"onEnd"; break;
			case 6: m_strValue = L"onMouseOut"; break;
			case 7: m_strValue = L"onMouseOver"; break;
			case 8: m_strValue = L"onNext"; break;
			case 9: m_strValue = L"onPrev"; break;
			case 10: m_strValue = L"onStopAudio"; break;
			}
		}
	} // namespace Limit
} // namespace PPTX
