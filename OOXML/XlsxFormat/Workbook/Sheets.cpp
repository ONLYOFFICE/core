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

#include "Sheets.h"
#include "../../XlsbFormat/Biff12_records/BundleSh.h"

namespace OOX
{
	namespace Spreadsheet
	{
		CSheet::CSheet(OOX::Document *pMain) : WritingElement(pMain)
		{
		}
		CSheet::~CSheet()
		{
		}
		void CSheet::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheet::toXML() const
		{
			return (L"");
		}
		void CSheet::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<sheet"));
			WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
			WritingStringNullableAttrInt(L"sheetId", m_oSheetId, m_oSheetId->GetValue());
			WritingStringNullableAttrString(L"state", m_oState, m_oState->ToString());
			WritingStringNullableAttrString(L"r:id", m_oRid, m_oRid->ToString());
			writer.WriteString((L"/>"));
		}
		void CSheet::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CSheet::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CSheet::getType () const
		{
			return et_x_Sheet;
		}
		void CSheet::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, (L"r:id"),				m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"relationships:id"),	m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"name"),				m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"sheetId"),			m_oSheetId )
				WritingElement_ReadAttributes_Read_else_if( oReader, (L"state"),			m_oState )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CSheet::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BundleSh*>(obj.get());
			m_oRid                = ptr->strRelID.value.value();
			m_oName               = ptr->strName.value();
			m_oSheetId            = ptr->iTabID;

			switch(ptr->hsState)
			{
				case XLSB::BundleSh::ST_SheetState::VISIBLE:
					m_oState = SimpleTypes::Spreadsheet::EVisibleType::visibleVisible;
					break;
				case XLSB::BundleSh::ST_SheetState::HIDDEN:
					m_oState = SimpleTypes::Spreadsheet::EVisibleType::visibleHidden;
					break;
				case XLSB::BundleSh::ST_SheetState::VERYHIDDEN:
					m_oState = SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden;
					break;
			}

		}

		CSheets::CSheets(OOX::Document *pMain) : WritingElementWithChilds<CSheet>(pMain)
		{
		}
		CSheets::~CSheets()
		{
		}
		void CSheets::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CSheets::toXML() const
		{
			return (L"");
		}
		void CSheets::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString((L"<sheets>"));

			for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (  m_arrItems[i] )
				{
					m_arrItems[i]->toXML(writer);
				}
			}

			writer.WriteString((L"</sheets>"));
		}
		void CSheets::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();

			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( (L"sheet") == sName )
				{
					CSheet* pSheet = new CSheet( oReader );

					m_arrItems.push_back( pSheet );
				}

			}
		}
		void CSheets::fromBin(std::vector<XLS::BaseObjectPtr>& obj)
		{
			//ReadAttributes(obj);

			if (obj.empty())
				return;

			for(auto &sheet : obj)
			{
				m_arrItems.push_back(new CSheet(sheet));
			}
		}
		EElementType CSheets::getType () const
		{
			return et_x_Sheets;
		}
		void CSheets::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}

	} //Spreadsheet
} // namespace OOX
