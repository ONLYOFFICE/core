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

#include "LightRigType.h"

namespace PPTX
{
	namespace Limit
	{		
		LightRigType::LightRigType()
		{
			m_strValue = L"balanced";
		}
		void LightRigType::set(const std::wstring& strValue)
		{
			m_strValue = strValue;
		}
		BYTE LightRigType::GetBYTECode() const
		{
			if (L"balanced" == m_strValue)		return 0;
			if (L"brightRoom" == m_strValue)	return 1;
			if (L"chilly" == m_strValue)		return 2;
			if (L"contrasting" == m_strValue)	return 3;
			if (L"flat" == m_strValue)			return 4;
			if (L"flood" == m_strValue)			return 5;
			if (L"freezing" == m_strValue)		return 6;
			if (L"glow" == m_strValue)			return 7;
			if (L"harsh" == m_strValue)			return 8;
			if (L"legacyFlat1" == m_strValue)	return 9;
			if (L"legacyFlat2" == m_strValue)	return 10;
			if (L"legacyFlat3" == m_strValue)	return 11;
			if (L"legacyFlat4" == m_strValue)	return 12;
			if (L"legacyHarsh1" == m_strValue)	return 13;
			if (L"legacyHarsh2" == m_strValue)	return 14;
			if (L"legacyHarsh3" == m_strValue)	return 15;
			if (L"legacyHarsh4" == m_strValue)	return 16;
			if (L"legacyNormal1" == m_strValue)	return 17;
			if (L"legacyNormal2" == m_strValue)	return 18;
			if (L"legacyNormal3" == m_strValue)	return 19;
			if (L"legacyNormal4" == m_strValue)	return 20;
			if (L"morning" == m_strValue)		return 21;
			if (L"soft" == m_strValue)			return 22;
			if (L"sunrise" == m_strValue)		return 23;
			if (L"sunset" == m_strValue)		return 24;
			if (L"threePt" == m_strValue)		return 25;
			if (L"twoPt" == m_strValue)			return 26;
			return 0;
		}
		void LightRigType::SetBYTECode(const BYTE& val)
		{
			switch (val)
			{
				case 0:		m_strValue = L"balanced";	break;
				case 1:		m_strValue = L"brightRoom"; break;
				case 2:		m_strValue = L"chilly";		break;
				case 3:		m_strValue = L"contrasting"; break;
				case 4:		m_strValue = L"flat";		break;
				case 5:		m_strValue = L"flood";		break;
				case 6:		m_strValue = L"freezing";	break;
				case 7:		m_strValue = L"glow";		break;
				case 8:		m_strValue = L"harsh";		break;
				case 9:		m_strValue = L"legacyFlat1";	break;
				case 10:	m_strValue = L"legacyFlat2";	break;
				case 11:	m_strValue = L"legacyFlat3";	break;
				case 12:	m_strValue = L"legacyFlat4";	break;
				case 13:	m_strValue = L"legacyHarsh1";	break;
				case 14:	m_strValue = L"legacyHarsh2";	break;
				case 15:	m_strValue = L"legacyHarsh3";	break;
				case 16:	m_strValue = L"legacyHarsh4";	break;
				case 17:	m_strValue = L"legacyNormal1";	break;
				case 18:	m_strValue = L"legacyNormal2";	break;
				case 19:	m_strValue = L"legacyNormal3";	break;
				case 20:	m_strValue = L"legacyNormal4";	break;
				case 21:	m_strValue = L"morning";	break;
				case 22:	m_strValue = L"soft";		break;
				case 23:	m_strValue = L"sunrise";	break;
				case 24:	m_strValue = L"sunset";		break;
				case 25:	m_strValue = L"threePt";	break;
				case 26:	m_strValue = L"twoPt";		break;
				default:	m_strValue = L"balanced";

			}
		}
	} // namespace Limit
} // namespace PPTX
