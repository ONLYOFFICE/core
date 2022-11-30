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

#include "Material.h"

namespace PPTX
{
	namespace Limit
	{		
		Material::Material()
		{
			m_strValue = L"clear";
		}
		void Material::set(const std::wstring& strValue)
		{
			m_strValue = strValue;
		}
		BYTE Material::GetBYTECode() const
		{
			if (L"clear" == m_strValue)
				return 0;
			if (L"dkEdge" == m_strValue)
				return 1;
			if (L"flat" == m_strValue)
				return 2;
			if (L"legacyMatte" == m_strValue)
				return 3;
			if (L"legacyMetal" == m_strValue)
				return 4;
			if (L"legacyPlastic" == m_strValue)
				return 5;
			if (L"legacyWireframe" == m_strValue)
				return 6;
			if (L"matte" == m_strValue)
				return 7;
			if (L"metal" == m_strValue)
				return 8;
			if (L"plastic" == m_strValue)
				return 9;
			if (L"powder" == m_strValue)
				return 10;
			if (L"softEdge" == m_strValue)
				return 11;
			if (L"softmetal" == m_strValue)
				return 12;
			if (L"translucentPowder" == m_strValue)
				return 13;
			if (L"warmMatte" == m_strValue)
				return 14;

			return 0;
		}
		void Material::SetBYTECode(const BYTE& val)
		{
			switch (val)
			{
				case 0:		m_strValue = L"clear";			break;
				case 1:		m_strValue = L"dkEdge";			break;
				case 2:		m_strValue = L"flat";			break;
				case 3:		m_strValue = L"legacyMatte";	break;
				case 4:		m_strValue = L"legacyMetal";	break;
				case 5:		m_strValue = L"legacyPlastic";	break;
				case 6:		m_strValue = L"legacyWireframe";break;
				case 7:		m_strValue = L"matte";			break;
				case 8:		m_strValue = L"metal";			break;
				case 9:		m_strValue = L"plastic";		break;
				case 10:	m_strValue = L"powder";			break;
				case 11:	m_strValue = L"softEdge";		break;
				case 12:	m_strValue = L"softmetal";		break;
				case 13:	m_strValue = L"translucentPowder";break;
				case 14:	m_strValue = L"warmMatte";		break;
				default:	m_strValue = L"clear";
			}
		}
	} // namespace Limit
} // namespace PPTX
