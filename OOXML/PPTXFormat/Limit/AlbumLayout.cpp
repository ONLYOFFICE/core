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

#include "AlbumLayout.h"

namespace PPTX
{
	namespace Limit
	{		
		AlbumLayout::AlbumLayout()
		{
			m_strValue = L"fitToSlide";
		}
		void AlbumLayout::set(const std::wstring& strValue)
		{
			if ((L"1pic" == strValue) ||
				(L"1picTitle" == strValue) ||
				(L"2pic" == strValue) ||
				(L"2picTitle" == strValue) ||
				(L"4pic" == strValue) ||
				(L"4picTitle" == strValue) ||
				(L"fitToSlide" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char AlbumLayout::GetBYTECode() const
		{
			if (L"1pic" == m_strValue)			return 0;
			if (L"1picTitle" == m_strValue)		return 1;
			if (L"2pic" == m_strValue)			return 2;
			if (L"2picTitle" == m_strValue)		return 3;
			if (L"4pic" == m_strValue)			return 4;
			if (L"4picTitle" == m_strValue)		return 5;
			if (L"fitToSlide" == m_strValue)	return 6;

			return 0;
		}
		void AlbumLayout::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:		m_strValue = L"1pic";			break;
			case 1:		m_strValue = L"1picTitle";		break;
			case 2:		m_strValue = L"2pic";			break;
			case 3:		m_strValue = L"2picTitle";		break;
			case 4:		m_strValue = L"4pic";			break;
			case 5:		m_strValue = L"4picTitle";		break;
			case 6:		m_strValue = L"fitToSlide";		break;
			default:	m_strValue = L"1pic";
			}
		}
	} // namespace Limit
} // namespace PPTX
