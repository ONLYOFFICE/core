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

#include "DgmBuildType.h"

namespace PPTX
{
	namespace Limit
	{		
		DgmBuildType::DgmBuildType()
		{
			m_strValue = L"whole";
		}
		void DgmBuildType::set(const std::wstring& strValue)
		{
			if ((L"allAtOnce" == strValue) ||
				(L"breadthByLvl" == strValue) ||
				(L"breadthByNode" == strValue) ||
				(L"ccw" == strValue) ||
				(L"ccwIn" == strValue) ||
				(L"ccwOut" == strValue) ||
				(L"cust" == strValue) ||
				(L"cw" == strValue) ||
				(L"cwIn" == strValue) ||
				(L"cwOut" == strValue) ||
				(L"depthByBranch" == strValue) ||
				(L"depthByNode" == strValue) ||
				(L"down" == strValue) ||
				(L"inByRing" == strValue) ||
				(L"outByRing" == strValue) ||
				(L"up" == strValue) ||
				(L"whole" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char DgmBuildType::GetBYTECode() const
		{
			if (L"allAtOnce" == m_strValue)		return 0;
			if (L"breadthByLvl" == m_strValue)	return 1;
			if (L"breadthByNode" == m_strValue)	return 2;
			if (L"ccw" == m_strValue)			return 3;
			if (L"ccwIn" == m_strValue)			return 4;
			if (L"ccwOut" == m_strValue)		return 5;
			if (L"cust" == m_strValue)			return 6;
			if (L"cw" == m_strValue)			return 7;
			if (L"cwIn" == m_strValue)			return 8;
			if (L"cwOut" == m_strValue)			return 9;
			if (L"depthByBranch" == m_strValue)	return 10;
			if (L"depthByNode" == m_strValue)	return 11;
			if (L"down" == m_strValue)			return 12;
			if (L"inByRing" == m_strValue)		return 13;
			if (L"outByRing" == m_strValue)		return 14;
			if (L"up" == m_strValue)			return 15;
			if (L"whole" == m_strValue)			return 16;

			return 0;
		}
		void DgmBuildType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:		m_strValue = L"allAtOnce";		break;
			case 1:		m_strValue = L"breadthByLvl";	break;
			case 2:		m_strValue = L"breadthByNode";	break;
			case 3:		m_strValue = L"ccw";			break;
			case 4:		m_strValue = L"ccwIn";			break;
			case 5:		m_strValue = L"ccwOut";			break;
			case 6:		m_strValue = L"cust";			break;
			case 7:		m_strValue = L"cw";				break;
			case 8:		m_strValue = L"cwIn";			break;
			case 9:		m_strValue = L"cwOut";			break;
			case 10:	m_strValue = L"depthByBranch";	break;
			case 11:	m_strValue = L"depthByNode";	break;
			case 12:	m_strValue = L"down";			break;
			case 13:	m_strValue = L"inByRing";		break;
			case 14:	m_strValue = L"outByRing";		break;
			case 15:	m_strValue = L"up";				break;
			case 16:	m_strValue = L"whole";			break;
			default:	m_strValue = L"allAtOnce";
			}
		}
	} // namespace Limit
} // namespace PPTX
