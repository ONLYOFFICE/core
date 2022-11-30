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

#include "BevelType.h"

namespace PPTX
{
	namespace Limit
	{		
		BevelType::BevelType()
		{
			m_strValue = L"circle";
		}
		void BevelType::set(const std::wstring& strValue)
		{
			m_strValue = strValue;
		}
		BYTE BevelType::GetBYTECode() const
		{
			if (L"angle" == m_strValue)			return 0;
			if (L"artDeco" == m_strValue)		return 1;
			if (L"circle" == m_strValue)		return 2;
			if (L"convex" == m_strValue)		return 3;
			if (L"coolSlant" == m_strValue)		return 4;
			if (L"cross" == m_strValue)			return 5;
			if (L"divot" == m_strValue)			return 6;
			if (L"hardEdge" == m_strValue)		return 7;
			if (L"relaxedInset" == m_strValue)	return 8;
			if (L"riblet" == m_strValue)		return 9;
			if (L"slope" == m_strValue)			return 10;
			if (L"softRound" == m_strValue)		return 11;
			return 0;
		}
		void BevelType::SetBYTECode(const BYTE& val)
		{
			switch (val)
			{
				case 0:		m_strValue = L"angle";		break;
				case 1:		m_strValue = L"artDeco";	break;
				case 2:		m_strValue = L"circle";		break;
				case 3:		m_strValue = L"convex";		break;
				case 4:		m_strValue = L"coolSlant";	break;
				case 5:		m_strValue = L"cross";		break;
				case 6:		m_strValue = L"divot";		break;
				case 7:		m_strValue = L"hardEdge";	break;
				case 8:		m_strValue = L"relaxedInset";break;
				case 9:		m_strValue = L"riblet";		break;
				case 10:	m_strValue = L"slope";		break;
				case 11:	m_strValue = L"softRound";	break;
				default:	m_strValue = L"angle";
			}
		}
	} // namespace Limit
} // namespace PPTX
