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

#include "SysClrVal.h"

namespace PPTX
{
	namespace Limit
	{		
		SysClrVal::SysClrVal()
		{
			m_strValue = L"window";
		}
		void SysClrVal::set(const std::wstring& strValue)
		{
			if ((L"3dDkShadow" == strValue) ||
				(L"3dLight" == strValue) ||
				(L"activeBorder" == strValue) ||
				(L"activeCaption" == strValue) ||
				(L"appWorkspace" == strValue) ||
				(L"background" == strValue) ||
				(L"btnFace" == strValue) ||
				(L"btnHighlight" == strValue) ||
				(L"btnShadow" == strValue) ||
				(L"btnText" == strValue) ||
				(L"captionText" == strValue) ||
				(L"gradientActiveCaption" == strValue) ||
				(L"gradientInactiveCaption" == strValue) ||
				(L"grayText" == strValue) ||
				(L"highlight" == strValue) ||
				(L"highlightText" == strValue) ||
				(L"hotLight" == strValue) ||
				(L"inactiveBorder" == strValue) ||
				(L"inactiveCaption" == strValue) ||
				(L"inactiveCaptionText" == strValue) ||
				(L"infoBk" == strValue) ||
				(L"infoText" == strValue) ||
				(L"menu" == strValue) ||
				(L"menuBar" == strValue) ||
				(L"menuHighlight" == strValue) ||
				(L"menuText" == strValue) ||
				(L"scrollBar" == strValue) ||
				(L"window" == strValue) ||
				(L"windowFrame" == strValue) ||
				(L"windowText" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char SysClrVal::GetBYTECode() const
		{
			//not using
			return 0;
		}
		void SysClrVal::SetBYTECode(const unsigned char& src)
		{
			//not using
		}
	} // namespace Limit
} // namespace PPTX
