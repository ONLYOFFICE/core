/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
			WritingElementSpreadsheet_AdditionConstructors(CTableStyleElement)
			CTableStyleElement()
			{
			}
			virtual ~CTableStyleElement()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oType.IsInit() && m_oDxfId.IsInit())
				{
					CString sXml;
					if(m_oSize.IsInit())
						sXml.Format(_T("<tableStyleElement type=\"%ls\" size=\"%d\" dxfId=\"%d\"/>"), m_oType->ToString(), m_oSize->GetValue(), m_oDxfId->GetValue());
					else
						sXml.Format(_T("<tableStyleElement type=\"%ls\" dxfId=\"%d\"/>"), m_oType->ToString(), m_oDxfId->GetValue());
					writer.WriteString(sXml);
				}
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
					oReader.ReadTillEnd();
			}

			virtual EElementType getType () const
			{
				return et_TableStyleElement;
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
			WritingElementSpreadsheet_AdditionConstructors(CTableStyle)
			CTableStyle()
			{
			}
			virtual ~CTableStyle()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				if(m_oName.IsInit() && m_arrItems.size() > 0)
				{
					writer.WriteString(_T("<tableStyle"));
					if(m_oName.IsInit())
					{
						CString sName;
						sName.Append(_T(" name=\""));
						sName.Append(XmlUtils::EncodeXmlString(m_oName.get2()));
						sName.Append(_T("\""));
						writer.WriteString(sName);
					}
					if((m_oPivot.IsInit() && true == m_oPivot->ToBool()) || (m_oTable.IsInit() && false == m_oTable->ToBool()))
						writer.WriteString(_T(" table=\"0\""));
					else
						writer.WriteString(_T(" pivot=\"0\""));
					if(m_oCount.IsInit())
					{
						CString sCount;
						sCount.Format(_T(" count=\"%d\""), m_oCount->GetValue());
						writer.WriteString(sCount);
					}
					writer.WriteString(_T(">"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</tableStyle>"));
				}

			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("tableStyleElement") == sName )
						m_arrItems.push_back( new CTableStyleElement( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_TableStyle;
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
			nullable<CString>									m_oName;
			nullable<SimpleTypes::COnOff<>>						m_oPivot;
			nullable<SimpleTypes::COnOff<>>						m_oTable;
			nullable<CString>									m_oDisplayName; // Используется только для дефалтовых стилей
		};
		class CTableStyles : public WritingElementWithChilds<CTableStyle>
		{
		public:
			WritingElementSpreadsheet_AdditionConstructors(CTableStyles)
			CTableStyles()
			{
			}
			virtual ~CTableStyles()
			{
			}

		public:
			virtual CString      toXML() const
			{
				return _T("");
			}
			virtual void toXML(XmlUtils::CStringWriter& writer) const
			{
				writer.WriteString(_T("<tableStyles"));
				if(m_oCount.IsInit())
				{
					CString sVal;sVal.Format(_T(" count=\"%d\""), m_oCount->GetValue());
					writer.WriteString(sVal);
				}
				if(m_oDefaultTableStyle.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" defaultTableStyle=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oDefaultTableStyle.get()));
					sVal.Append(_T("\""));
					writer.WriteString(sVal);
				}
				if(m_oDefaultPivotStyle.IsInit())
				{
					CString sVal;
					sVal.Append(_T(" defaultPivotStyle=\""));
					sVal.Append(XmlUtils::EncodeXmlString(m_oDefaultPivotStyle.get()));
					sVal.Append(_T("\""));
					writer.WriteString(sVal);
				}
				if(m_arrItems.size() >  0)
				{
					writer.WriteString(_T(">"));
					for(unsigned int i = 0, length = m_arrItems.size(); i < length; ++i)
						m_arrItems[i]->toXML(writer);
					writer.WriteString(_T("</tableStyles>"));
				}
				else
					writer.WriteString(_T("/>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					CString sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( _T("tableStyle") == sName )
						m_arrItems.push_back( new CTableStyle( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_TableStyles;
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
			nullable<CString>									m_oDefaultPivotStyle;
			nullable<CString>									m_oDefaultTableStyle;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_TABLESTYLES_FILE_INCLUDE_H_