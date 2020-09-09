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
				toXMLWithNS(writer, L"", L"patternFill", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				WritingStringNullableAttrString(L"patternType", m_oPatternType, m_oPatternType->ToString());
				if(m_oBgColor.IsInit() || m_oFgColor.IsInit())
				{
					writer.EndAttributes();
					if(m_oBgColor.IsInit() && m_oFgColor.IsInit())
					{
						m_oFgColor->toXMLWithNS(writer, child_ns, L"fgColor", child_ns);
						m_oBgColor->toXMLWithNS(writer, child_ns, L"bgColor", child_ns);
					}
					else if(m_oFgColor.IsInit())
					{
						m_oFgColor->toXMLWithNS(writer, child_ns, L"fgColor", child_ns);
						m_oFgColor->toXMLWithNS(writer, child_ns, L"bgColor", child_ns);
					}
					else if(m_oBgColor.IsInit())
					{
						m_oBgColor->toXMLWithNS(writer, child_ns, L"fgColor", child_ns);
						m_oBgColor->toXMLWithNS(writer, child_ns, L"bgColor", child_ns);
					}

					writer.EndNodeWithNS(node_ns, node_name);
				}
				else
					writer.EndAttributesAndNode();
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
				toXMLWithNS(writer, L"", L"stop", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				WritingStringNullableAttrDouble(L"position", m_oPosition, m_oPosition->GetValue());
				writer.EndAttributes();
				if (m_oColor.IsInit())
				{
					m_oColor->toXMLWithNS(writer, child_ns, L"color", child_ns);
				}
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
				toXMLWithNS(writer, L"", L"gradientFill", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				WritingStringNullableAttrString(L"type", m_oType, m_oType->ToString());
				WritingStringNullableAttrDouble(L"left", m_oLeft, m_oLeft->GetValue());
				WritingStringNullableAttrDouble(L"right", m_oRight, m_oRight->GetValue());
				WritingStringNullableAttrDouble(L"top", m_oTop, m_oTop->GetValue());
				WritingStringNullableAttrDouble(L"bottom", m_oBottom, m_oBottom->GetValue());
				WritingStringNullableAttrDouble(L"degree", m_oDegree, m_oDegree->GetValue());
				writer.EndAttributes();
				for(size_t i = 0; i < m_arrItems.size(); ++i)
				{
					m_arrItems[i]->toXMLWithNS(writer, child_ns, L"stop", child_ns);
				}
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
				toXMLWithNS(writer, L"", L"fill", L"");
			}
			void toXMLWithNS(NSStringUtils::CStringBuilder& writer, const std::wstring &node_ns, const std::wstring &node_name, const std::wstring &child_ns) const
			{
				writer.StartNodeWithNS(node_ns, node_name);
				writer.StartAttributes();
				writer.EndAttributes();
				if(m_oPatternFill.IsInit())
					m_oPatternFill->toXMLWithNS(writer, child_ns, L"patternFill", child_ns);
				if(m_oGradientFill.IsInit())
					m_oGradientFill->toXMLWithNS(writer, child_ns, L"gradientFill", child_ns);
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
				if ( oReader.GetAttributesCount() <= 0) return;

				nullable_string sColor, sPatternColor, sPattern;

				WritingElement_ReadAttributes_Start( oReader )
					WritingElement_ReadAttributes_Read_if     ( oReader, _T("ss:PatternColor"), sPatternColor )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("ss:Pattern"), sPattern )
					WritingElement_ReadAttributes_Read_else_if( oReader, _T("ss:Color"), sColor )
				WritingElement_ReadAttributes_End( oReader )
				
				if (sColor.IsInit() == false && sPattern.IsInit() == false && sPatternColor.IsInit() == false) return;
				
				m_oPatternFill.Init();
				if (sColor.IsInit())
				{
					m_oPatternFill->m_oFgColor.Init(); m_oPatternFill->m_oFgColor->m_oRgb.Init();
					m_oPatternFill->m_oFgColor->m_oRgb->FromString(*sColor);
				}
				if (sPatternColor.IsInit())
				{
					m_oPatternFill->m_oBgColor.Init(); m_oPatternFill->m_oBgColor->m_oRgb.Init();
					m_oPatternFill->m_oBgColor->m_oRgb->FromString(*sPatternColor);
				}
				m_oPatternFill->m_oPatternType.Init();
				if (sPattern.IsInit())
				{
					m_oPatternFill->m_oPatternType->FromString(*sPattern);
				}
				else m_oPatternFill->m_oPatternType->FromString(L"Solid");
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
