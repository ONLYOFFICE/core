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

#include "Sheets.h"
#include "../../XlsbFormat/Biff12_records/BundleSh.h"

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Spreadsheet.h"

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
		XLS::BaseObjectPtr CSheet::toBin()
		{
			auto ptr(new XLSB::BundleSh);
			XLS::BaseObjectPtr objectPtr(ptr);

			ptr->strRelID.value = m_oRid->GetValue();
			ptr->strName = m_oName.get();
			ptr->iTabID = m_oSheetId->GetValue();

			if(m_oState == SimpleTypes::Spreadsheet::EVisibleType::visibleVisible)
				ptr->hsState = XLSB::BundleSh::ST_SheetState::VISIBLE;
			else if(m_oState == SimpleTypes::Spreadsheet::EVisibleType::visibleHidden)
				ptr->hsState = XLSB::BundleSh::ST_SheetState::HIDDEN;
			else if(m_oState == SimpleTypes::Spreadsheet::EVisibleType::visibleVeryHidden)
				ptr->hsState = XLSB::BundleSh::ST_SheetState::VERYHIDDEN;
            else
                ptr->hsState = XLSB::BundleSh::ST_SheetState::VISIBLE;

			return objectPtr;
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
			WritingElement_ReadAttributes_Start_No_NS( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"id",		m_oRid )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"name",		m_oName )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"sheetId",	m_oSheetId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"state",	m_oState )
			WritingElement_ReadAttributes_End_No_NS( oReader )
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
					CSheet* pSheet = new CSheet();
					*pSheet = oReader;

					m_arrItems.push_back( pSheet );
				}

			}
			auto sheetIndex = 0;
			for(auto i:m_arrItems)
			{	if(i->m_oName.IsInit())
					AddSheetRef(i->m_oName.get(), sheetIndex);
				sheetIndex++;
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
		std::vector<XLS::BaseObjectPtr> CSheets::toBin()
		{
			std::vector<XLS::BaseObjectPtr> objectVector;
			for(auto i: m_arrItems)
				objectVector.push_back(i->toBin());
			return objectVector;
		}
		EElementType CSheets::getType () const
		{
			return et_x_Sheets;
		}
		void CSheets::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		void CSheets::AddSheetRef(const std::wstring& link, const _INT32&  sheetIndex)
		{
			XLS::GlobalWorkbookInfo::_xti val_1;
			val_1.iSup		= sheetIndex;
			val_1.itabFirst = sheetIndex;
			val_1.itabLast = sheetIndex;
			val_1.link = link;
				
			XLS::GlobalWorkbookInfo::arXti_External_static.push_back(val_1);
		}

	} //Spreadsheet
} // namespace OOX
