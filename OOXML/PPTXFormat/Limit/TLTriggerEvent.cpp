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

#include "TLTriggerEvent.h"

namespace PPTX
{
	namespace Limit
	{
		TLTriggerEvent::TLTriggerEvent()
		{
			m_strValue = _T("begin");
		}
		void TLTriggerEvent::set(const std::wstring& strValue)
		{
			if ((_T("begin") == strValue) ||
				(_T("end") == strValue) ||
				(_T("onBegin") == strValue) ||
				(_T("onClick") == strValue) ||
				(_T("onDblClick") == strValue) ||
				(_T("onEnd") == strValue) ||
				(_T("onMouseOut") == strValue) ||
				(_T("onMouseOver") == strValue) ||
				(_T("onNext") == strValue) ||
				(_T("onPrev") == strValue) ||
				(_T("onStopAudio") == strValue))
			{
				m_strValue = strValue;
			}
		}
		BYTE TLTriggerEvent::GetBYTECode() const
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
		void TLTriggerEvent::SetBYTECode(const BYTE& src)
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
