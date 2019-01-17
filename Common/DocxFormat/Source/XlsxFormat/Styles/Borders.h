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
#ifndef OOX_BORDERS_FILE_INCLUDE_H_
#define OOX_BORDERS_FILE_INCLUDE_H_

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
				writer.WriteString(_T("<"));
				writer.WriteString(sName);
				WritingStringNullableAttrString(L"style", m_oStyle, m_oStyle->ToString());
				writer.WriteString(_T(">"));
				if(m_oColor.IsInit())
					m_oColor->toXML2(writer, _T("color"));
				writer.WriteString(_T("</"));
				writer.WriteString(sName);
				writer.WriteString(_T(">"));
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

					if ( _T("color") == sName )
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
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

					WritingElement_ReadAttributes_Read_if     ( oReader, _T("style"),      m_oStyle )

					WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CBorderStyle<>>	m_oStyle;
			nullable<CColor>									m_oColor;
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
				writer.WriteString(_T("<border"));
				if (m_oDiagonalDown.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalDown->GetValue())
				{
					WritingStringAttrString(L"diagonalDown", m_oDiagonalDown->ToString3(SimpleTypes::onofftostring1));
				}
				if (m_oDiagonalUp.IsInit() && SimpleTypes::onoffTrue == m_oDiagonalUp->GetValue())
				{
					WritingStringAttrString(L"diagonalUp", m_oDiagonalUp->ToString3(SimpleTypes::onofftostring1));
				}
				writer.WriteString(_T(">"));
				if(m_oStart.IsInit() && false == m_oStart->IsEmpty())
					m_oStart->toXML2(writer, _T("left"));
				else
					writer.WriteString(_T("<left/>"));
				if(m_oEnd.IsInit() && false == m_oEnd->IsEmpty())
					m_oEnd->toXML2(writer, _T("right"));
				else
					writer.WriteString(_T("<right/>"));
				if(m_oTop.IsInit() && false == m_oTop->IsEmpty())
					m_oTop->toXML2(writer, _T("top"));
				else
					writer.WriteString(_T("<top/>"));
				if(m_oBottom.IsInit() && false == m_oBottom->IsEmpty())
					m_oBottom->toXML2(writer, _T("bottom"));
				else
					writer.WriteString(_T("<bottom/>"));
				if(m_oDiagonal.IsInit() && false == m_oDiagonal->IsEmpty())
					m_oDiagonal->toXML2(writer, _T("diagonal"));
				else
					writer.WriteString(_T("<diagonal/>"));
				if(m_oVertical.IsInit() && false == m_oVertical->IsEmpty())
					m_oVertical->toXML2(writer, _T("vertical"));
				if(m_oHorizontal.IsInit() && false == m_oHorizontal->IsEmpty())
					m_oHorizontal->toXML2(writer, _T("horizontal"));
				writer.WriteString(_T("</border>"));
			}
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
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
					else if ( _T("vertical") == sName )
						m_oVertical = oReader;

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

			nullable<CBorderProp>			m_oBottom;
			nullable<CBorderProp>			m_oDiagonal;
			nullable<CBorderProp>			m_oEnd;
			nullable<CBorderProp>			m_oHorizontal;
			nullable<CBorderProp>			m_oStart;
			nullable<CBorderProp>			m_oTop;
			nullable<CBorderProp>			m_oVertical;
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
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int index = 0;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

					if ( L"border" == sName )
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

#endif // OOX_BORDERS_FILE_INCLUDE_H_
