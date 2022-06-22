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
#ifndef OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_

#include "../../Common/SimpleTypes_Shared.h"
#include "../../Common/SimpleTypes_Word.h"

#include "./../WritingElement.h"

#include "RunProperty.h"
#include "SectionProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// NumPr 17.13.1.19 (Part 1)
		//--------------------------------------------------------------------------------
		class CNumPr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CNumPr)
			CNumPr()
			{
			}
			virtual ~CNumPr()
			{
			}

		public:
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:numPr" != oNode.GetName() && L"w:listPr" != oNode.GetName())
					return;

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:ilvl", oChild ) )
					m_oIlvl = oChild;

				if ( oNode.GetNode( L"w:ins", oChild ) )
					m_oIns = oChild;

				if ( oNode.GetNode( L"w:numId", oChild ) )
					m_oNumID = oChild;
				
				if ( oNode.GetNode( L"w:ilfo", oChild ) )
					m_oNumID = oChild;
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					
					if ( L"w:ilvl" == sName )
						m_oIlvl = oReader;
					else if ( L"w:ins" == sName )
						m_oIns = oReader;
					else if ( L"w:numId" == sName || L"w:ilfo" == sName )
						m_oNumID = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:numPr>";

				if ( m_oIlvl.IsInit() )
				{
					sResult += L"<w:ilvl ";
					sResult += m_oIlvl->ToString();
					sResult += L"/>";
				}

				if ( m_oIns.IsInit() )
				{
					sResult += L"<w:ins ";
					sResult += m_oIns->ToString();
					sResult += L"/>";
				}

				if ( m_oNumID.IsInit() )
				{
					sResult += L"<w:numId ";
					sResult += m_oNumID->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:numPr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_numPr;
			}
		public:

			nullable<ComplexTypes::Word::CDecimalNumber > m_oIlvl;
			nullable<ComplexTypes::Word::CTrackChange   > m_oIns;
			nullable<ComplexTypes::Word::CDecimalNumber > m_oNumID;

		};

		//--------------------------------------------------------------------------------
		// PBdr 17.13.1.24 (Part 1)
		//--------------------------------------------------------------------------------
		class CPBdr : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CPBdr)
			CPBdr()
			{
			}
			virtual ~CPBdr()
			{
			}
		public:

			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:pBdr" != oNode.GetName() )
					return;

				XmlUtils::CXmlNode oChild;

				if ( oNode.GetNode( L"w:bar", oChild ) )
					m_oBar = oChild;

				if ( oNode.GetNode( L"w:between", oChild ) )
					m_oBetween = oChild;

				if ( oNode.GetNode( L"w:bottom", oChild ) )
					m_oBottom = oChild;

				if ( oNode.GetNode( L"w:left", oChild ) )
					m_oLeft = oChild;

				if ( oNode.GetNode( L"w:right", oChild ) )
					m_oRight = oChild;

				if ( oNode.GetNode( L"w:top", oChild ) )
					m_oTop = oChild;

			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:bar" == sName )
						m_oBar = oReader;
					else if ( L"w:between" == sName )
						m_oBetween = oReader;
					else if ( L"w:bottom" == sName )
						m_oBottom = oReader;
					else if ( L"w:left" == sName )
						m_oLeft = oReader;
					else if ( L"w:right" == sName )
						m_oRight = oReader;
					else if ( L"w:top" == sName )
						m_oTop = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = L"<w:pBdr>";

				if ( m_oBar.IsInit() )
				{
					sResult += L"<w:bar ";
					sResult += m_oBar->ToString();
					sResult += L"/>";
				}

				if ( m_oBetween.IsInit() )
				{
					sResult += L"<w:between ";
					sResult += m_oBetween->ToString();
					sResult += L"/>";
				}

				if ( m_oBottom.IsInit() )
				{
					sResult += L"<w:bottom ";
					sResult += m_oBottom->ToString();
					sResult += L"/>";
				}

				if ( m_oLeft.IsInit() )
				{
					sResult += L"<w:left ";
					sResult += m_oLeft->ToString();
					sResult += L"/>";
				}

				if ( m_oRight.IsInit() )
				{
					sResult += L"<w:right ";
					sResult += m_oRight->ToString();
					sResult += L"/>";
				}

				if ( m_oTop.IsInit() )
				{
					sResult += L"<w:top ";
					sResult += m_oTop->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:pBdr>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_pBdr;
			}
			static const CPBdr Merge(const CPBdr& oPrev, const CPBdr& oCurrent)
			{
				CPBdr oProperties;
				oProperties.m_oBar            = Merge( oPrev.m_oBar,            oCurrent.m_oBar );
				oProperties.m_oBetween            = Merge( oPrev.m_oBetween,            oCurrent.m_oBetween );
				oProperties.m_oBottom            = Merge( oPrev.m_oBottom,            oCurrent.m_oBottom );
				oProperties.m_oLeft            = Merge( oPrev.m_oLeft,            oCurrent.m_oLeft );
				oProperties.m_oRight            = Merge( oPrev.m_oRight,            oCurrent.m_oRight );
				oProperties.m_oTop            = Merge( oPrev.m_oTop,            oCurrent.m_oTop );
				return oProperties;
			}
			template<typename Type>
			static nullable<Type>     Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
		public:

			nullable<ComplexTypes::Word::CBorder > m_oBar;
			nullable<ComplexTypes::Word::CBorder > m_oBetween;
			nullable<ComplexTypes::Word::CBorder > m_oBottom;
			nullable<ComplexTypes::Word::CBorder > m_oLeft;
			nullable<ComplexTypes::Word::CBorder > m_oRight;
			nullable<ComplexTypes::Word::CBorder > m_oTop;

		};
		//--------------------------------------------------------------------------------
		// PPrChange 17.13.5.29 (Part 1)
		//--------------------------------------------------------------------------------
		class CParagraphProperty;
		class CPPrChange : public WritingElement
		{
		public:
			CPPrChange();
			CPPrChange(XmlUtils::CXmlNode& oNode);
			CPPrChange(XmlUtils::CXmlLiteReader& oReader);
			const CPPrChange& operator =(const XmlUtils::CXmlNode &oNode);
			const CPPrChange& operator =(const XmlUtils::CXmlLiteReader& oReader);
			virtual ~CPPrChange();

		public:

			virtual void         fromXML(XmlUtils::CXmlNode& oNode);
			virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring      toXML() const;
			virtual EElementType getType() const;

		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader);

		public:

			nullable<std::wstring                       > m_sAuthor;
			nullable<SimpleTypes::CDateTime        > m_oDate;
			nullable<SimpleTypes::CDecimalNumber<> > m_oId;
			nullable<std::wstring                       > m_sUserId;

			nullable<OOX::Logic::CParagraphProperty> m_pParPr;

		};
		//--------------------------------------------------------------------------------
		// Tabs 17.3.1.38 (Part 1)
		//--------------------------------------------------------------------------------
		class CTabs : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CTabs)
			CTabs(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
			}
			virtual ~CTabs()
			{
				for ( unsigned int nIndex = 0; nIndex < m_arrTabs.size(); nIndex++ )
				{
					if ( m_arrTabs[nIndex] ) delete m_arrTabs[nIndex];
					m_arrTabs[nIndex] = NULL;
				}
				m_arrTabs.clear();
			}
			CTabs(const CTabs& oSrc)
			{
				*this = oSrc;
			}
			const CTabs& operator =(const CTabs &oTabs)
			{
				for(size_t i = 0; i < oTabs.m_arrTabs.size(); ++i)
				{
					m_arrTabs.push_back(new ComplexTypes::Word::CTabStop(*oTabs.m_arrTabs[i]));
				}
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				if ( L"w:tabs" != oNode.GetName() )
					return;

				XmlUtils::CXmlNodes oTabs;
				if ( oNode.GetNodes( L"w:tab", oTabs ) )
				{
					XmlUtils::CXmlNode oTab;
					for ( int nIndex = 0; nIndex < oTabs.GetCount(); nIndex++ )
					{
						if ( oTabs.GetAt( nIndex, oTab ) )
						{
							ComplexTypes::Word::CTabStop *oTabStop = new ComplexTypes::Word::CTabStop(oTab);
							if (oTabStop) m_arrTabs.push_back( oTabStop );
						}
					}
				}
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader) 
			{
				if ( oReader.IsEmptyNode() )
					return;

				int nParentDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nParentDepth ) )
				{
					std::wstring sName = oReader.GetName();
					if ( L"w:tab" == sName )
					{
						ComplexTypes::Word::CTabStop *oTabStop = new ComplexTypes::Word::CTabStop(oReader);
						if (oTabStop) m_arrTabs.push_back( oTabStop );
					}
				}
			}
			virtual std::wstring      toXML() const
			{
				std::wstring sResult = L"<w:tabs>";

				for (unsigned int nIndex = 0; nIndex < m_arrTabs.size(); nIndex++ )
				{
					sResult += L"<w:tab ";
					if (m_arrTabs[nIndex])
						sResult += m_arrTabs[nIndex]->ToString();
					sResult += L"/>";
				}

				sResult += L"</w:tabs>";

				return sResult;
			}

			virtual EElementType getType() const
			{
				return et_w_tabs;
			}
		public:

			std::vector<ComplexTypes::Word::CTabStop*> m_arrTabs;
		};
		//--------------------------------------------------------------------------------
		// CParagraphProperty
		//--------------------------------------------------------------------------------
		class CParagraphProperty : public WritingElement
		{
		public:
			CParagraphProperty(OOX::Document *pMain = NULL) : WritingElement(pMain)
			{
				m_bPPrChange = false;
			}
			CParagraphProperty(XmlUtils::CXmlNode& oNode) : WritingElement(NULL)
			{
				m_bPPrChange = false;
				fromXML( oNode );
			}
			CParagraphProperty(XmlUtils::CXmlLiteReader& oReader) : WritingElement(NULL)
			{
				m_bPPrChange = false;
				fromXML( oReader );
			}
			virtual ~CParagraphProperty() {}
			const CParagraphProperty& operator =(const XmlUtils::CXmlNode &oNode)
			{
				fromXML( (XmlUtils::CXmlNode &)oNode );
				return *this;
			}
			const CParagraphProperty& operator =(const XmlUtils::CXmlLiteReader &oReader)
			{
				fromXML( (XmlUtils::CXmlLiteReader&)oReader );
				return *this;
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode);
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader);
			virtual std::wstring toXML() const;

			virtual EElementType getType() const
			{
				return et_w_pPr;
			}
			static const CParagraphProperty Merge(const CParagraphProperty& oPrev, const CParagraphProperty& oCurrent);
			template<typename Type>
			static nullable<Type> Merge(const nullable<Type> &oPrev, const nullable<Type> &oCurrent)
			{
				nullable<Type> oResult;

				if ( oCurrent.IsInit() )
					oResult = oCurrent;
				else if ( oPrev.IsInit() )
					oResult = oPrev;

				return oResult;
			}
//--------------------------------------------------------------------------------------------------------
			bool                                                           m_bPPrChange;

			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oAdjustRightInd;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oAutoSpaceDE;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oAutoSpaceDN;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oBidi;
			nullable<ComplexTypes::Word::CCnf>								m_oCnfStyle;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oContextualSpacing;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oDivID;
			nullable<ComplexTypes::Word::CFramePr>							m_oFramePr;
			nullable<ComplexTypes::Word::CInd>								m_oInd;
			nullable<ComplexTypes::Word::CJc>								m_oJc;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oKeepLines;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oKeepNext;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oKinsoku;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oMirrorIndents;
			nullable<OOX::Logic::CNumPr>									m_oNumPr;
			nullable<ComplexTypes::Word::CDecimalNumber>					m_oOutlineLvl;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oOverflowPunct;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oPageBreakBefore;
			nullable<OOX::Logic::CPBdr>										m_oPBdr;
			nullable<OOX::Logic::CPPrChange>								m_oPPrChange;
			nullable<ComplexTypes::Word::String>							m_oPStyle;
			nullable<OOX::Logic::CRunProperty>								m_oRPr;
			nullable<OOX::Logic::CSectionProperty>							m_oSectPr;
			nullable<ComplexTypes::Word::CShading>							m_oShd;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oSnapToGrid;
			nullable<ComplexTypes::Word::CSpacing>							m_oSpacing;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oSuppressAutoHyphens;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oSuppressLineNumbers;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oSuppressOverlap;
			nullable<OOX::Logic::CTabs>										m_oTabs;
			nullable<ComplexTypes::Word::CTextAlignment>					m_oTextAlignment;
			nullable<ComplexTypes::Word::CTextboxTightWrap>					m_oTextboxTightWrap;
			nullable<ComplexTypes::Word::CTextDirection>					m_oTextDirection;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oTopLinePunct;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oWidowControl;
			nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>>	m_oWordWrap;
		};

	} // namespace Logic
} // namespace OOX
#endif // OOX_LOGIC_PARAGRAPH_PROPERTY_INCLUDE_H_
