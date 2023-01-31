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

#include "Pos.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CPos::CPos()
		{
		}
		CPos::~CPos()
		{
		}
		void CPos::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CPos::toXML() const
		{
			return _T("");
		}
		void CPos::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<xdr:pos"));
			WritingStringNullableAttrInt64(L"x", m_oX, m_oX->ToEmu());
			WritingStringNullableAttrInt64(L"y", m_oY, m_oY->ToEmu());
			writer.WriteString(_T("/>"));
		}
		void CPos::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CPos::getType () const
		{
			return et_x_FromTo;
		}
		void CPos::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("x"),      m_oX )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("y"),      m_oY )

			WritingElement_ReadAttributes_End( oReader )
		}
	} //Spreadsheet
} // namespace OOX
