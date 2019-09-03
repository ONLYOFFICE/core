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
#pragma once
#ifndef OOX_TABLESTYLES_FILE_INCLUDE_H_
#define OOX_TABLESTYLES_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CTableStyleElement : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTableStyleElement)
			CTableStyleElement()
			{
			}
			virtual ~CTableStyleElement()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_x_TableStyleElement;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("dxfId"),      m_oDxfId )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("size"),      m_oSize )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("type"),      m_oType )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oDxfId;
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oSize;
			nullable<SimpleTypes::Spreadsheet::CTableStyleType<>>	m_oType;
		};
		class CTableStyle : public WritingElementWithChilds<CTableStyleElement>
		{
		public:
			WritingElement_AdditionConstructors(CTableStyle)
			CTableStyle()
			{
			}
			virtual ~CTableStyle()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_TableStyle;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
								// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("name"),       m_oName )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("pivot"),      m_oPivot )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("table"),      m_oTable )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("displayName"),m_oDisplayName )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			nullable<std::wstring>									m_oName;
			nullable<SimpleTypes::COnOff<>>						m_oPivot;
			nullable<SimpleTypes::COnOff<>>						m_oTable;
			nullable<std::wstring>									m_oDisplayName; // Используется только для дефалтовых стилей
		};
		class CTableStyles : public WritingElementWithChilds<CTableStyle>
		{
		public:
			WritingElement_AdditionConstructors(CTableStyles)
			CTableStyles()
			{
			}
			virtual ~CTableStyles()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return _T("");
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
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
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
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

			virtual EElementType getType () const
			{
				return et_x_TableStyles;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("count"),      m_oCount )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultPivotStyle"),      m_oDefaultPivotStyle )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("defaultTableStyle"),      m_oDefaultTableStyle )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			nullable<std::wstring>									m_oDefaultPivotStyle;
			nullable<std::wstring>									m_oDefaultTableStyle;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TABLESTYLES_FILE_INCLUDE_H_
