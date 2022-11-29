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

#include "FromTo.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CFromTo::CFromTo()
		{
		}
		CFromTo::~CFromTo()
		{
		}
		void CFromTo::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CFromTo::toXML() const
		{
			return L"";
		}
		void CFromTo::toXML(NSStringUtils::CStringBuilder& writer) const
		{
		}
		void CFromTo::toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
		{
			writer.WriteString(L"<");
			writer.WriteString(sName);
			writer.WriteString(L">");
			if(m_oCol.IsInit())
			{
				WritingStringValInt(L"xdr:col", m_oCol->GetValue());
			}
			if(m_oColOff.IsInit())
			{
				WritingStringValInt64(L"xdr:colOff", m_oColOff->ToEmu());
			}
			if(m_oRow.IsInit())
			{
				WritingStringValInt(L"xdr:row", m_oRow->GetValue());
			}
			if(m_oRowOff.IsInit())
			{
				WritingStringValInt64(L"xdr:rowOff", m_oRowOff->ToEmu());
			}
			writer.WriteString(L"</");
			writer.WriteString(sName);
			writer.WriteString(L">");
		}
		void CFromTo::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"col" == sName )
					m_oCol = oReader.GetText2();
				else if ( L"colOff" == sName )
					m_oColOff = oReader.GetText2();
				else if ( L"row" == sName )
					m_oRow = oReader.GetText2();
				else if ( L"rowOff" == sName )
					m_oRowOff = oReader.GetText2();
			}
		}
		EElementType CFromTo::getType () const
		{
			return et_x_FromTo;
		}
		void CFromTo::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
		}

		CExt::CExt()
		{
		}
		CExt::~CExt()
		{
		}
		void CExt::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CExt::toXML() const
		{
			return L"";
		}
		void CExt::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(L"<xdr:ext");
			WritingStringNullableAttrInt64(L"cx", m_oCx, m_oCx->ToEmu());
			WritingStringNullableAttrInt64(L"cy", m_oCy, m_oCy->ToEmu());
			writer.WriteString(L"/>");
		}
		void CExt::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		EElementType CExt::getType () const
		{
			return et_x_Ext;
		}
		void CExt::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if( oReader, L"cx", m_oCx )
					WritingElement_ReadAttributes_Read_if( oReader, L"cy", m_oCy )
				WritingElement_ReadAttributes_End( oReader )
			}

	} //Spreadsheet
} // namespace OOX

