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

#include "TableStyles.h"

#include "../../XlsbFormat/Biff12_records/CommonRecords.h"
#include "../../XlsbFormat/Biff12_unions/TABLESTYLES.h"
#include "../../XlsbFormat/Biff12_unions/TABLESTYLE.h"
namespace OOX
{
	namespace Spreadsheet
	{
		CTableStyleElement::CTableStyleElement()
		{
		}
		CTableStyleElement::~CTableStyleElement()
		{
		}
		void CTableStyleElement::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTableStyleElement::toXML() const
		{
			return _T("");
		}
		void CTableStyleElement::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_oType.IsInit() && m_oDxfId.IsInit())
			{
				writer.WriteString(L"<tableStyleElement");
				WritingStringAttrString(L"type", m_oType->ToString());
				WritingStringNullableAttrInt(L"size", m_oSize, m_oSize->GetValue());
				WritingStringAttrInt(L"dxfId", m_oDxfId->GetValue());
				writer.WriteString(L"/>");
			}
		}
		void CTableStyleElement::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( !oReader.IsEmptyNode() )
				oReader.ReadTillEnd();
		}
		void CTableStyleElement::fromBin(XLS::BaseObjectPtr& obj)
		{
			ReadAttributes(obj);
		}
		EElementType CTableStyleElement::getType () const
		{
			return et_x_TableStyleElement;
		}
		void CTableStyleElement::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
							// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("size"),      m_oSize )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

				WritingElement_ReadAttributes_End( oReader )
		}
		void CTableStyleElement::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::TableStyleElement*>(obj.get());
			if(ptr != nullptr)
			{
				m_oDxfId                = ptr->index;
				m_oSize                 = ptr->size;

				switch(ptr->tseType)
				{
					case 0:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeWholeTable; break;
					case 1:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeHeaderRow; break;
					case 2:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeTotalRow; break;
					case 3:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstColumn; break;
					case 4:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeLastColumn; break;
					case 5:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstRowStripe; break;
					case 6:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondRowStripe; break;
					case 7:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstColumnStripe; break;
					case 8:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondColumnStripe; break;
					case 9:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstHeaderCell; break;
					case 10:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeLastHeaderCell; break;
					case 11:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstTotalCell; break;
					case 12:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeLastTotalCell; break;
					case 13:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstSubtotalColumn; break;
					case 14:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondSubtotalColumn; break;
					case 15:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeThirdSubtotalColumn; break;
					case 16:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstSubtotalRow; break;
					case 17:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondSubtotalRow; break;
					case 18:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeThirdSubtotalRow; break;
					case 19:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeBlankRow; break;
					case 20:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstColumnSubheading; break;
					case 21:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondColumnSubheading; break;
					case 22:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeThirdColumnSubheading; break;
					case 23:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeFirstRowSubheading; break;
					case 24:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeSecondRowSubheading; break;
					case 25:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeThirdRowSubheading; break;
					case 26:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypePageFieldLabels; break;
					case 27:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypePageFieldValues; break;
					default:
						m_oType = SimpleTypes::Spreadsheet::ETableStyleType::tablestyletypeBlankRow; break;

				}

			}
		}

		CTableStyle::CTableStyle()
		{
		}
		CTableStyle::~CTableStyle()
		{
		}
		void CTableStyle::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTableStyle::toXML() const
		{
			return _T("");
		}
		void CTableStyle::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			if(m_oName.IsInit() && m_arrItems.size() > 0)
			{
				writer.WriteString(_T("<tableStyle"));
				WritingStringNullableAttrEncodeXmlString(L"name", m_oName, m_oName.get());
				WritingStringNullableAttrBool(L"table", m_oTable);
				WritingStringNullableAttrBool(L"pivot", m_oPivot);
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(_T(">"));

				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (  m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString(_T("</tableStyle>"));
			}

		}
		void CTableStyle::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("tableStyleElement") == sName )
					m_arrItems.push_back( new CTableStyleElement( oReader ));
			}
		}
		void CTableStyle::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::TABLESTYLE*>(obj.get());
			ReadAttributes(ptr->m_BrtBeginTableStyle);

			for(auto &tableStyleElement : ptr->m_arBrtTableStyleElement)
			{
				m_arrItems.push_back(new CTableStyleElement(tableStyleElement));
			}

		}
		EElementType CTableStyle::getType () const
		{
			return et_x_TableStyle;
		}
		void CTableStyle::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),       m_oName )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("pivot"),      m_oPivot )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("table"),      m_oTable )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("displayName"),m_oDisplayName )

			WritingElement_ReadAttributes_End( oReader )
		}
		void CTableStyle::ReadAttributes(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::BeginTableStyle*>(obj.get());
			if(ptr != nullptr)
			{
				m_oCount                = ptr->ctse;
				m_oPivot                = ptr->fIsPivot;
				m_oTable                = ptr->fIsTable;
				m_oName                 = ptr->rgchName;
				m_oDisplayName          = ptr->rgchName;
			}
		}

		CTableStyles::CTableStyles()
		{
		}
		CTableStyles::~CTableStyles()
		{
		}
		void CTableStyles::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CTableStyles::toXML() const
		{
			return _T("");
		}
		void CTableStyles::toXML(NSStringUtils::CStringBuilder& writer) const
		{
			writer.WriteString(_T("<tableStyles"));
			WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
			WritingStringNullableAttrEncodeXmlString(L"defaultTableStyle", m_oDefaultTableStyle, m_oDefaultTableStyle.get());
			WritingStringNullableAttrEncodeXmlString(L"defaultPivotStyle", m_oDefaultPivotStyle, m_oDefaultPivotStyle.get());

			if(!m_arrItems.empty())
			{
				writer.WriteString(_T(">"));

				for ( size_t i = 0; i < m_arrItems.size(); ++i)
				{
					if (  m_arrItems[i] )
					{
						m_arrItems[i]->toXML(writer);
					}
				}

				writer.WriteString(_T("</tableStyles>"));
			}
			else
				writer.WriteString(_T("/>"));
		}
		void CTableStyles::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( _T("tableStyle") == sName )
					m_arrItems.push_back( new CTableStyle( oReader ));
			}
		}
		void CTableStyles::fromBin(XLS::BaseObjectPtr& obj)
		{
			auto ptr = static_cast<XLSB::TABLESTYLES*>(obj.get());
			ReadAttributes(ptr->m_BrtBeginTableStyles);

			for(auto &tableStyle : ptr->m_arTABLESTYLE)
			{
				m_arrItems.push_back(new CTableStyle(tableStyle));
			}

		}
		EElementType CTableStyles::getType () const
		{
			return et_x_TableStyles;
		}
		void CTableStyles::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultPivotStyle"),      m_oDefaultPivotStyle )
				WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultTableStyle"),      m_oDefaultTableStyle )

				WritingElement_ReadAttributes_End( oReader )
		}
		void CTableStyles::ReadAttributes(XLS::BaseObjectPtr& obj)
			{
				auto ptr = static_cast<XLSB::BeginTableStyles*>(obj.get());
				if(ptr != nullptr)
				{
					m_oCount                = ptr->cts;
					m_oDefaultPivotStyle    = ptr->rgchDefPivotStyle;
					m_oDefaultTableStyle    = ptr->rgchDefTableStyle;
				}
			}

	} //Spreadsheet
} // namespace OOX
