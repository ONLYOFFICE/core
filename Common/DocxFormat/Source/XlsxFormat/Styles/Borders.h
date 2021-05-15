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

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CBorderProp : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorderProp)
			CBorderProp()
			{
			}
			virtual ~CBorderProp()
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
			}
			void toXML2(NSStringUtils::CStringBuilder& writer, const std::wstring& sName) const
			{
				toXMLWithNS(writer, L"", sName, L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				WritingStringNullableAttrString(L"style", m_oStyle, m_oStyle->ToString());
				writer.EndAttributes();
				if(m_oColor.IsInit())
					m_oColor->toXMLWithNS(writer, child_ns, L"color", child_ns);
				writer.EndNodeWithNS(node_ns, node_name);
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

					if ( L"color" == sName )
						m_oColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_BorderProp;
			}
			bool IsEmpty()
			{
				return !(m_oStyle.IsInit() || m_oColor.IsInit());
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				nullable_int iWeight;
				nullable_string sColor, sLineStyle;

				WritingElement_ReadAttributes_Start(oReader)
					WritingElement_ReadAttributes_Read_if(oReader, L"style", m_oStyle)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:Color", sColor)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:LineStyle", sLineStyle)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:Position", m_oType)
					WritingElement_ReadAttributes_Read_else_if(oReader, L"ss:Weight", iWeight)
				WritingElement_ReadAttributes_End(oReader)

				if (sColor.IsInit())
				{
					m_oColor.Init(); m_oColor->m_oRgb.Init();
					m_oColor->m_oRgb->FromString(*sColor);
				}
				if (sLineStyle.IsInit())
				{
					if (*sLineStyle == L"Dot") 
						m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleDotted));
					else if (*sLineStyle == L"Dash") 
						m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleDashed));
					else if (*sLineStyle == L"Double")
						m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleDouble));
					else if (*sLineStyle == L"Continuous")
					{
						m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleThin));
						bBorderContinuous = true;
					}
				}
				if (iWeight.IsInit())
				{
					switch (*iWeight)
					{
						case 1:	 //Thin
						{
							m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleThin));
						}break;
						case 3: //Thick
						{
							m_oStyle.reset(new SimpleTypes::Spreadsheet::CBorderStyle<>(SimpleTypes::Spreadsheet::borderstyleThick));
						}break;
						default://2: //Medium
						{
						}break;
					}
				}
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CBorderStyle<>>	m_oStyle;
			nullable<CColor>									m_oColor;
			nullable_string										m_oType;

			bool bBorderContinuous = false; // merge cells border (2003)
		};

		class CBorder : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBorder)
			CBorder()
			{
			}
			virtual ~CBorder()
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
				toXMLWithNS(writer, L"", L"border", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				if (m_oDiagonalDown.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalDown->GetValue())
				{
					WritingStringAttrString(L"diagonalDown", m_oDiagonalDown->ToString3(SimpleTypes::onofftostring1));
				}
				if (m_oDiagonalUp.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalUp->GetValue())
				{
					WritingStringAttrString(L"diagonalUp", m_oDiagonalUp->ToString3(SimpleTypes::onofftostring1));
				}
				writer.EndAttributes();
				if(m_oStart.IsInit() && false == m_oStart->IsEmpty())
					m_oStart->toXMLWithNS(writer, child_ns, _T("left"), child_ns);
				else
					writer.WriteString(_T("<left/>"));
				if(m_oEnd.IsInit() && false == m_oEnd->IsEmpty())
					m_oEnd->toXMLWithNS(writer, child_ns, _T("right"), child_ns);
				else
					writer.WriteString(_T("<right/>"));
				if(m_oTop.IsInit() && false == m_oTop->IsEmpty())
					m_oTop->toXMLWithNS(writer, child_ns, _T("top"), child_ns);
				else
					writer.WriteString(_T("<top/>"));
				if(m_oBottom.IsInit() && false == m_oBottom->IsEmpty())
					m_oBottom->toXMLWithNS(writer, child_ns, _T("bottom"), child_ns);
				else
					writer.WriteString(_T("<bottom/>"));
				if(m_oDiagonal.IsInit() && false == m_oDiagonal->IsEmpty())
					m_oDiagonal->toXMLWithNS(writer, child_ns, _T("diagonal"), child_ns);
				else
					writer.WriteString(_T("<diagonal/>"));
				if(m_oVertical.IsInit() && false == m_oVertical->IsEmpty())
					m_oVertical->toXMLWithNS(writer, child_ns, _T("vertical"), child_ns);
				if(m_oHorizontal.IsInit() && false == m_oHorizontal->IsEmpty())
					m_oHorizontal->toXMLWithNS(writer, child_ns, _T("horizontal"), child_ns);
				writer.EndNodeWithNS(node_ns, node_name);
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

					if ( _T("bottom") == sName )
						m_oBottom = oReader;
					else if ( _T("diagonal") == sName )
						m_oDiagonal = oReader;
					else if ( _T("end") == sName || _T("right") == sName )
						m_oEnd = oReader;
					else if ( _T("horizontal") == sName )
						m_oHorizontal = oReader;
					else if ( _T("start") == sName || _T("left") == sName )
						m_oStart = oReader;
					else if ( _T("top") == sName )
						m_oTop = oReader;
					else if ( L"vertical" == sName )
						m_oVertical = oReader;
					else if (L"Border" == sName)
					{
						CBorderProp* border = new CBorderProp(oReader);
						if ((border) && (border->m_oType.IsInit()))
						{
							if (*border->m_oType == L"Bottom")		m_oBottom	= border;
							else if (*border->m_oType == L"Top")	m_oTop		= border;
							else if (*border->m_oType == L"Left")	m_oStart	= border;
							else if (*border->m_oType == L"Right")	m_oEnd		= border;

							if (border->bBorderContinuous)
								bBorderContinuous = true;
						}
						else
						{
							delete border;
						}
					}

				}
			}

			virtual EElementType getType () const
			{
				return et_x_Border;
			}
		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("diagonalDown"),	m_oDiagonalDown )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("diagonalUp"),		m_oDiagonalUp )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("outline"),			m_oOutline )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::COnOff<>>	m_oDiagonalDown;
			nullable<SimpleTypes::COnOff<>>	m_oDiagonalUp;
			nullable<SimpleTypes::COnOff<>>	m_oOutline;

			nullable<CBorderProp>	m_oBottom;
			nullable<CBorderProp>	m_oDiagonal;
			nullable<CBorderProp>	m_oEnd;
			nullable<CBorderProp>	m_oHorizontal;
			nullable<CBorderProp>	m_oStart;
			nullable<CBorderProp>	m_oTop;
			nullable<CBorderProp>	m_oVertical;

			bool bBorderContinuous = false;
		};

		class CBorders : public WritingElementWithChilds<CBorder>
		{
		public:
			WritingElement_AdditionConstructors(CBorders)
			CBorders()
			{
			}
			virtual ~CBorders()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& node)
			{
			}
            virtual std::wstring toXML() const
			{
				return L"";
			}
			virtual void toXML(NSStringUtils::CStringBuilder& writer) const
			{
				writer.WriteString(L"<borders");
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(L">");
				
                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(L"</borders>");
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int index = 0;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"border" == sName || L"Border" == sName)
					{
						CBorder *pBorder = new CBorder( oReader );
						m_arrItems.push_back( pBorder );
						m_mapBorders.insert(std::make_pair(index++, pBorder));
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Borders;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oCount;
			std::map<int, CBorder*>							m_mapBorders;
		};
	} //Spreadsheet
} // namespace OOX
