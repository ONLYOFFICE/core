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
#ifndef OOX_FILLS_FILE_INCLUDE_H_
#define OOX_FILLS_FILE_INCLUDE_H_

#include "../CommonInclude.h"

#include "rPr.h"

namespace OOX
{
	namespace Spreadsheet
	{
		class CPatternFill : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPatternFill)
			CPatternFill()
			{
			}
			virtual ~CPatternFill()
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
				writer.WriteString(L"<patternFill");
				WritingStringNullableAttrString(L"patternType", m_oPatternType, m_oPatternType->ToString());
				if(m_oBgColor.IsInit() || m_oFgColor.IsInit())
				{
					writer.WriteString(L">");
					if(m_oBgColor.IsInit() && m_oFgColor.IsInit())
					{
						m_oFgColor->toXML2(writer, L"fgColor");
						m_oBgColor->toXML2(writer, L"bgColor");
					}
					else if(m_oFgColor.IsInit())
					{
						m_oFgColor->toXML2(writer, L"fgColor");
						m_oFgColor->toXML2(writer, L"bgColor");
					}
					else if(m_oBgColor.IsInit())
					{
						m_oBgColor->toXML2(writer, L"fgColor");
						m_oBgColor->toXML2(writer, L"bgColor");
					}
					
					writer.WriteString(L"</patternFill>");
				}
				else
					writer.WriteString(L"/>");
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

					if ( L"bgColor" == sName )
						m_oBgColor = oReader;
					else if ( L"fgColor" == sName )
						m_oFgColor = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_PatternFill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"patternType", m_oPatternType )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::Spreadsheet::CPatternType<>>	m_oPatternType;
			nullable<CColor>									m_oBgColor;
			nullable<CColor>									m_oFgColor;
		};
		class CGradientStop : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CGradientStop)
			CGradientStop()
			{
			}
			virtual ~CGradientStop()
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
				writer.WriteString(L"<stop");
				WritingStringNullableAttrDouble(L"position", m_oPosition, m_oPosition->GetValue());
				writer.WriteString(L">");
				if (m_oColor.IsInit())
				{
					m_oColor->toXML(writer);
				}
				writer.WriteString(L"</stop>");
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
				return et_x_GradientStop;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"position", m_oPosition )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDouble>		m_oPosition;
			nullable<CColor>					m_oColor;
		};
		class CGradientFill : public WritingElementWithChilds<CGradientStop>
		{
		public:
			WritingElement_AdditionConstructors(CGradientFill)
			CGradientFill()
			{
			}
			virtual ~CGradientFill()
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
				writer.WriteString(L"<gradientFill");
				WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
				WritingStringNullableAttrDouble(L"left", m_oLeft, m_oLeft->GetValue());
				WritingStringNullableAttrDouble(L"right", m_oRight, m_oRight->GetValue());
				WritingStringNullableAttrDouble(L"top", m_oTop, m_oTop->GetValue());
				WritingStringNullableAttrDouble(L"bottom", m_oBottom, m_oBottom->GetValue());
				WritingStringNullableAttrDouble(L"degree", m_oDegree, m_oDegree->GetValue());
				writer.WriteString(L">");
				for(size_t i = 0; i < m_arrItems.size(); ++i)
				{
					m_arrItems[i]->toXML(writer);
				}
				writer.WriteString(L"</gradientFill>");
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

					if ( L"stop" == sName )
						m_arrItems.push_back( new CGradientStop( oReader ));
				}
			}

			virtual EElementType getType () const
			{
				return et_x_GradientFill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"bottom",	m_oBottom )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"degree",	m_oDegree )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"left",	m_oLeft )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"right",	m_oRight )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"top",	m_oTop )
					WritingElement_ReadAttributes_Read_if     ( oReader, L"type",	m_oType )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CDouble>		m_oBottom;
			nullable<SimpleTypes::CDouble>		m_oDegree;
			nullable<SimpleTypes::CDouble>		m_oLeft;
			nullable<SimpleTypes::CDouble>		m_oRight;
			nullable<SimpleTypes::CDouble>		m_oTop;
			nullable<SimpleTypes::Spreadsheet::CGradientType<>>		m_oType;
		};
		class CFill : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CFill)
			CFill()
			{
			}
			virtual ~CFill()
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
				writer.WriteString(L"<fill>");
				if(m_oPatternFill.IsInit())
					m_oPatternFill->toXML(writer);
				if(m_oGradientFill.IsInit())
					m_oGradientFill->toXML(writer);
				writer.WriteString(L"</fill>");
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

					if ( L"gradientFill" == sName )
						m_oGradientFill = oReader;
					else if ( L"patternFill" == sName )
						m_oPatternFill = oReader;
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Fill;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
			}
		public:
			nullable<CGradientFill>		m_oGradientFill;
			nullable<CPatternFill>		m_oPatternFill;
		};
		class CFills : public WritingElementWithChilds<CFill>
		{
		public:
			WritingElement_AdditionConstructors(CFills)
			CFills()
			{
			}
			virtual ~CFills()
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
				writer.WriteString(L"<fills");
				WritingStringNullableAttrInt(L"count", m_oCount, m_oCount->GetValue());
				writer.WriteString(L">");

                for ( size_t i = 0; i < m_arrItems.size(); ++i)
                {
                    if (  m_arrItems[i] )
                    {
                        m_arrItems[i]->toXML(writer);
                    }
                }
				
				writer.WriteString(L"</fills>");
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

					if ( L"fill" == sName )
					{
						CFill* pFill = new CFill( oReader );
						m_arrItems.push_back( pFill );
						m_mapFills.insert(std::make_pair(index++, pFill));
					}
				}
			}

			virtual EElementType getType () const
			{
				return et_x_Fills;
			}

		private:
			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if ( oReader, L"count", m_oCount )
				WritingElement_ReadAttributes_End( oReader )
			}
		public:
			nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCount;
			std::map<int, CFill*>								m_mapFills;
		};
	} //Spreadsheet
} // namespace OOX

#endif // OOX_FILLS_FILE_INCLUDE_H_
