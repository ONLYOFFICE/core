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
#include "../Document.h"
#include "ParagraphProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// PPrChange
		//--------------------------------------------------------------------------------
		CPPrChange::CPPrChange()
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;
		}
		CPPrChange::CPPrChange(XmlUtils::CXmlNode &oNode)
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;

			fromXML( oNode );
		}
		CPPrChange::CPPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pParPr.Init();
			m_pParPr->m_bPPrChange = true;

			fromXML( oReader );
		}
		const CPPrChange& CPPrChange::operator =(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode&)oNode );
			return *this;
		}
		const CPPrChange& CPPrChange::operator =(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
		}
		CPPrChange::~CPPrChange()
		{
		}

		void CPPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:pPrChange" != oNode.GetName() )
				return;

            XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
            XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
            XmlMacroReadAttributeBase( oNode, L"w:id",     m_oId );
            XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );

			XmlUtils::CXmlNode oNode_pPr;

			if ( m_pParPr.IsInit() && oNode.GetNode(L"w:pPr", oNode_pPr ) )
				m_pParPr->fromXML( oNode_pPr );
		}

		void CPPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());
				if ( L"pPr" == sName )
					m_pParPr->fromXML( oReader );
			}
		}

		std::wstring CPPrChange::toXML() const
		{
			std::wstring sResult = L"<w:pPrChange ";

			if ( m_sAuthor.IsInit() )
			{
				sResult += L"w:author=\"";
                sResult += m_sAuthor.get2();
				sResult += L"\" ";
			}

			if ( m_oDate.IsInit() )
			{
				sResult += L"w:date=\"";
				sResult += m_oDate->ToString();
				sResult += L"\" ";
			}

			if ( m_oId.IsInit() )
			{
				sResult += L"w:id=\"";
				sResult += m_oId->ToString();
				sResult += L"\" ";
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += L"oouserid=\"";
                sResult += m_sUserId.get2();
				sResult += L"\" ";
			}

			sResult += L">";

			if ( m_pParPr.IsInit() )
				sResult += m_pParPr->toXML();

			sResult += L"</w:pPrChange>";

			return sResult;
		}
		EElementType CPPrChange::getType() const
		{
			return et_w_pPrChange;
		}
		void CPPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start_No_NS( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"date",   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"id",	 m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End_No_NS( oReader )
		}
		void CParagraphProperty::fromXML(XmlUtils::CXmlNode& oNode)
		{//??? где используется ?
			if ( L"w:pPr" != oNode.GetName() )
				return;

			XmlUtils::CXmlNodes oNodes;

			if (oNode.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oChild;
					oNodes.GetAt(i, oChild);

					std::wstring strName = oChild.GetName();

					if (L"w:adjustRightInd" == strName)			m_oAdjustRightInd = oChild;
					if (L"w:autoSpaceDE" == strName)			m_oAutoSpaceDE = oChild;
					if (L"w:autoSpaceDN" == strName)			m_oAutoSpaceDN = oChild;
					if (L"w:bidi" == strName)					m_oBidi = oChild;
					if (L"w:cnfStyle" == strName)				m_oCnfStyle = oChild;
					if (L"w:contextualSpacing" == strName)		m_oContextualSpacing = oChild;
					if (L"w:divId" == strName)					m_oDivID = oChild;
					if (L"w:framePr" == strName)				m_oFramePr = oChild;
					if (L"w:ind" == strName)					m_oInd = oChild;
					if (L"w:jc" == strName)						m_oJc = oChild;
					if (L"w:keepLines" == strName)				m_oKeepLines = oChild;
					if (L"w:keepNext" == strName)				m_oKeepNext = oChild;
					if (L"w:kinsoku" == strName)				m_oKinsoku = oChild;
					if (L"w:mirrorIndents" == strName)			m_oMirrorIndents = oChild;
					if (L"w:numPr" == strName)					m_oNumPr = oChild;
					if (L"w:listPr" == strName)					m_oNumPr = oChild;
					if (L"w:outlineLvl" == strName)				m_oOutlineLvl = oChild;
					if (L"w:overflowPunct" == strName)			m_oOverflowPunct = oChild;
					if (L"w:pageBreakBefore" == strName)		m_oPageBreakBefore = oChild;
					if (L"w:pBdr" == strName)					m_oPBdr = oChild;
					if (!m_bPPrChange && L"w:pPrChange" == strName)	m_oPPrChange = oChild;
					if (L"w:pStyle" == strName)					m_oPStyle = oChild;
					if (!m_bPPrChange && L"w:rPr" == strName)	m_oRPr = oChild;
					if (!m_bPPrChange && L"w:sectPr" == strName)m_oSectPr = oChild;
					if (L"w:shd" == strName)					m_oShd = oChild;
					if (L"w:snapToGrid" == strName)				m_oSnapToGrid = oChild;
					if (L"w:spacing" == strName)				m_oSpacing = oChild;
					if (L"w:suppressAutoHyphens" == strName)	m_oSuppressAutoHyphens = oChild;
					if (L"w:suppressLineNumbers" == strName)	m_oSuppressLineNumbers = oChild;
					if (L"w:suppressOverlap" == strName)		m_oSuppressOverlap = oChild;
					if (L"w:tabs" == strName)					m_oTabs = oChild;
					if (L"w:textAlignment" == strName)			m_oTextAlignment = oChild;
					if (L"w:textboxTightWrap" == strName)		m_oTextboxTightWrap = oChild;
					if (L"w:textDirection" == strName)			m_oTextDirection = oChild;
					if (L"w:topLinePunct" == strName)			m_oTopLinePunct = oChild;
					if (L"w:widowControl" == strName)			m_oWidowControl = oChild;
					if (L"w:wordWrap" == strName)				m_oWordWrap = oChild;
				}
			}
		}
		void CParagraphProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = XmlUtils::GetNameNoNS(oReader.GetName());

				if ( L"adjustRightInd" == sName )
					m_oAdjustRightInd = oReader;
				else if ( L"autoSpaceDE" == sName )
					m_oAutoSpaceDE = oReader;
				else if ( L"autoSpaceDN" == sName )
					m_oAutoSpaceDN = oReader;
				else if ( L"bidi" == sName )
					m_oBidi = oReader;
				else if ( L"cnfStyle" == sName )
					m_oCnfStyle = oReader;
				else if ( L"contextualSpacing" == sName )
					m_oContextualSpacing = oReader;
				else if ( L"divId" == sName )
					m_oDivID = oReader;
				else if ( L"framePr" == sName )
					m_oFramePr = oReader;
				else if ( L"ind" == sName )
					m_oInd = oReader;
				else if ( L"jc" == sName )
					m_oJc = oReader;
				else if ( L"keepLines" == sName )
					m_oKeepLines = oReader;
				else if ( L"keepNext" == sName )
					m_oKeepNext = oReader;
				else if ( L"kinsoku" == sName )
					m_oKinsoku = oReader;
				else if ( L"mirrorIndents" == sName )
					m_oMirrorIndents = oReader;
				else if ( L"numPr" == sName || L"listPr" == sName)
					m_oNumPr = oReader;
				else if ( L"outlineLvl" == sName )
					m_oOutlineLvl = oReader;
				else if ( L"overflowPunct" == sName )
					m_oOverflowPunct = oReader;
				else if ( L"pageBreakBefore" == sName )
					m_oPageBreakBefore = oReader;
				else if ( L"pBdr" == sName )
					m_oPBdr = oReader;
				else if ( !m_bPPrChange && L"pPrChange" == sName )
					m_oPPrChange = oReader;
				else if ( L"pStyle" == sName )
					m_oPStyle = oReader;
				else if ( !m_bPPrChange && L"rPr" == sName )
					m_oRPr = oReader;
				else if ( !m_bPPrChange && L"sectPr" == sName )
				{
					m_oSectPr = new CSectionProperty(document);
					m_oSectPr->fromXML(oReader);
//------------------------------------------------------------------------------------
					OOX::CDocx *docx = dynamic_cast<OOX::CDocx*>(document);
					if (docx)
					{
						OOX::CDocument *doc = docx->m_bGlossaryRead ? docx->m_oGlossary.document : docx->m_oMain.document;

						OOX::CDocument::_section section;
						section.sect = m_oSectPr.GetPointer();
						section.start_elm = doc->m_arrItems.size() + 1; // следующий после текущего

						if (false == doc->m_arrSections.empty())
						{
							doc->m_arrSections.back().end_elm = doc->m_arrItems.size() + 1; //активный рутовый еще не добавлен
						}
						doc->m_arrSections.push_back(section);
					}
//------------------------------------------------------------------------------------
				}
				else if ( L"shd" == sName )
					m_oShd = oReader;
				else if ( L"snapToGrid" == sName )
					m_oSnapToGrid = oReader;
				else if ( L"spacing" == sName )
					m_oSpacing = oReader;
				else if ( L"suppressAutoHyphens" == sName )
					m_oSuppressAutoHyphens = oReader;
				else if ( L"suppressLineNumbers" == sName )
					m_oSuppressLineNumbers = oReader;
				else if ( L"suppressOverlap" == sName )
					m_oSuppressOverlap = oReader;
				else if ( L"tabs" == sName )
					m_oTabs = oReader;
				else if ( L"textAlignment" == sName )
					m_oTextAlignment = oReader;
				else if ( L"textboxTightWrap" == sName )
					m_oTextboxTightWrap = oReader;
				else if ( L"textDirection" == sName )
					m_oTextDirection = oReader;
				else if ( L"topLinePunct" == sName )
					m_oTopLinePunct = oReader;
				else if ( L"widowControl" == sName )
					m_oWidowControl = oReader;
				else if ( L"wordWrap" == sName )
					m_oWordWrap = oReader;
			}
		}
		std::wstring CParagraphProperty::toXML() const
		{
			std::wstring sResult = L"<w:pPr>";

			if ( m_oAdjustRightInd.IsInit() ) 
			{
				sResult += L"<w:adjustRightInd ";
				sResult += m_oAdjustRightInd->ToString();
				sResult += L"/>";
			}

			if ( m_oAutoSpaceDE.IsInit() ) 
			{ 
				sResult += L"<w:autoSpaceDE ";
				sResult += m_oAutoSpaceDE->ToString();
				sResult += L"/>";				
			}

			if ( m_oAutoSpaceDN.IsInit() ) 
			{ 
				sResult += L"<w:autoSpaceDN ";
				sResult += m_oAutoSpaceDN->ToString();
				sResult += L"/>";	
			}

			if ( m_oBidi.IsInit() ) 
			{ 
				sResult += L"<w:bidi ";
				sResult += m_oBidi->ToString();	
				sResult += L"/>";		
			}

			if ( m_oCnfStyle.IsInit() ) 
			{ 
				sResult += L"<w:cnfStyle "; 
				sResult += m_oCnfStyle->ToString();
				sResult += L"/>";	
			}

			if ( m_oContextualSpacing.IsInit() ) 
			{ 
				sResult += L"<w:contextualSpacing ";
				sResult += m_oContextualSpacing->ToString();	
				sResult += L"/>";		
			}

			if ( m_oDivID.IsInit() ) 
			{ 
				sResult += L"<w:divId "; 
				sResult += m_oDivID->ToString();	
				sResult += L"/>";		
			}

			if ( m_oFramePr.IsInit() )
			{ 
				sResult += L"<w:framePr "; 
				sResult += m_oFramePr->ToString();	
				sResult += L"/>";	
			}

			if ( m_oInd.IsInit() )
			{ 
				sResult += L"<w:ind "; 
				sResult += m_oInd->ToString();		
				sResult += L"/>";	
			}

			if ( m_oJc.IsInit() ) 
			{ 
				sResult += L"<w:jc ";
				sResult += m_oJc->ToString();		
				sResult += L"/>";	
			}

			if ( m_oKeepLines.IsInit() )
			{ 
				sResult += L"<w:keepLines ";
				sResult += m_oKeepLines->ToString();	
				sResult += L"/>";			
			}

			if ( m_oKeepNext.IsInit() ) 
			{ 
				sResult += L"<w:keepNext "; 
				sResult += m_oKeepNext->ToString();
				sResult += L"/>";	
			}

			if ( m_oKinsoku.IsInit() )
			{
				sResult += L"<w:kinsoku "; 
				sResult += m_oKinsoku->ToString();	
				sResult += L"/>";	
			}

			if ( m_oMirrorIndents.IsInit() ) 
			{ 
				sResult += L"<w:mirrorIndents "; 
				sResult += m_oMirrorIndents->ToString();	
				sResult += L"/>";			
			}

			if ( m_oNumPr.IsInit() ) 
				sResult += m_oNumPr->toXML();	

			if ( m_oOutlineLvl.IsInit() ) 
			{ 
				sResult += L"<w:outlineLvl ";
				sResult += m_oOutlineLvl->ToString();		
				sResult += L"/>";		
			}

			if ( m_oOverflowPunct.IsInit() ) 
			{ 					
				sResult += L"<w:overflowPunct "; 
				sResult += m_oOverflowPunct->ToString();	
				sResult += L"/>";			
			}

			if ( m_oPageBreakBefore.IsInit() ) 
			{ 
				sResult += L"<w:pageBreakBefore "; 
				sResult += m_oPageBreakBefore->ToString();
				sResult += L"/>";		
			}

			if ( m_oPBdr.IsInit() )
				sResult += m_oPBdr->toXML();	

			if ( !m_bPPrChange && m_oPPrChange.IsInit() )
				sResult += m_oPPrChange->toXML();	

			if ( m_oPStyle.IsInit() )
			{
				sResult += L"<w:pStyle ";
				sResult += m_oPStyle->ToString();
				sResult += L"/>";
			}

			if ( !m_bPPrChange && m_oRPr.IsInit() )
				sResult += m_oRPr->toXML();		

			if ( !m_bPPrChange && m_oSectPr.IsInit() ) 
				sResult += m_oSectPr->toXML();	

			if ( m_oShd.IsInit() ) 
			{ 
				sResult += L"<w:shd "; 
				sResult += m_oShd->ToString();		
				sResult += L"/>";	
			}

			if ( m_oSnapToGrid.IsInit() ) 
			{ 
				sResult += L"<w:snapToGrid ";
				sResult += m_oSnapToGrid->ToString();	
				sResult += L"/>";		
			}

			if ( m_oSpacing.IsInit() ) 
			{
				sResult += L"<w:spacing "; 
				sResult += m_oSpacing->ToString();		
				sResult += L"/>";		
			}

			if ( m_oSuppressAutoHyphens.IsInit() ) 
			{ 
				sResult += L"<w:suppressAutoHyphens ";
				sResult += m_oSuppressAutoHyphens->ToString();		
				sResult += L"/>";		
			}

			if ( m_oSuppressLineNumbers.IsInit() ) 
			{ 
				sResult += L"<w:suppressLineNumbers "; 
				sResult += m_oSuppressLineNumbers->ToString();		
				sResult += L"/>";			
			}

			if ( m_oSuppressOverlap.IsInit() ) 
			{ 
				sResult += L"<w:suppressOverlap "; 
				sResult += m_oSuppressOverlap->ToString();		
				sResult += L"/>";			
			}

			if ( m_oTabs.IsInit() ) 
				sResult += m_oTabs->toXML();	

			if ( m_oTextAlignment.IsInit() ) 
			{
				sResult += L"<w:textAlignment "; 
				sResult += m_oTextAlignment->ToString();	
				sResult += L"/>";		
			}

			if ( m_oTextboxTightWrap.IsInit() ) 
			{
				sResult += L"<w:textboxTightWrap ";
				sResult += m_oTextboxTightWrap->ToString();	
				sResult += L"/>";			
			}

			if ( m_oTextDirection.IsInit() )
			{ 
				sResult += L"<w:textDirection "; 
				sResult += m_oTextDirection->ToString();	
				sResult += L"/>";		
			}

			if ( m_oTopLinePunct.IsInit() )
			{ 
				sResult += L"<w:topLinePunct ";
				sResult += m_oTopLinePunct->ToString();	
				sResult += L"/>";			
			}

			if ( m_oWidowControl.IsInit() ) 
			{ 
				sResult += L"<w:widowControl "; 
				sResult += m_oWidowControl->ToString();
				sResult += L"/>";		
			}

			if ( m_oWordWrap.IsInit() ) 
			{
				sResult += L"<w:wordWrap "; 
				sResult += m_oWordWrap->ToString();	
				sResult += L"/>";		
			}

			sResult += L"</w:pPr>";

			return sResult;
		}
		const CParagraphProperty CParagraphProperty::Merge(const CParagraphProperty& oPrev, const CParagraphProperty& oCurrent)
		{
			CParagraphProperty oProperties;

			oProperties.m_oAdjustRightInd = Merge(oPrev.m_oAdjustRightInd, oCurrent.m_oAdjustRightInd);
			oProperties.m_oAutoSpaceDE = Merge(oPrev.m_oAutoSpaceDE, oCurrent.m_oAutoSpaceDE);
			oProperties.m_oAutoSpaceDN = Merge(oPrev.m_oAutoSpaceDN, oCurrent.m_oAutoSpaceDN);
			oProperties.m_oBidi = Merge(oPrev.m_oBidi, oCurrent.m_oBidi);
			oProperties.m_oCnfStyle = Merge(oPrev.m_oCnfStyle, oCurrent.m_oCnfStyle);
			oProperties.m_oContextualSpacing = Merge(oPrev.m_oContextualSpacing, oCurrent.m_oContextualSpacing);
			oProperties.m_oDivID = Merge(oPrev.m_oDivID, oCurrent.m_oDivID);
			oProperties.m_oFramePr = Merge(oPrev.m_oFramePr, oCurrent.m_oFramePr);

			if (oCurrent.m_oInd.IsInit() && oPrev.m_oInd.IsInit())
				oProperties.m_oInd = ComplexTypes::Word::CInd::Merge(oPrev.m_oInd.get(), oCurrent.m_oInd.get());
			else
				oProperties.m_oInd = Merge(oPrev.m_oInd, oCurrent.m_oInd);

			oProperties.m_oJc = Merge(oPrev.m_oJc, oCurrent.m_oJc);
			oProperties.m_oKeepLines = Merge(oPrev.m_oKeepLines, oCurrent.m_oKeepLines);
			oProperties.m_oKeepNext = Merge(oPrev.m_oKeepNext, oCurrent.m_oKeepNext);
			oProperties.m_oKinsoku = Merge(oPrev.m_oKinsoku, oCurrent.m_oKinsoku);
			oProperties.m_oMirrorIndents = Merge(oPrev.m_oMirrorIndents, oCurrent.m_oMirrorIndents);
			oProperties.m_oNumPr = Merge(oPrev.m_oNumPr, oCurrent.m_oNumPr);
			oProperties.m_oOutlineLvl = Merge(oPrev.m_oOutlineLvl, oCurrent.m_oOutlineLvl);
			oProperties.m_oOverflowPunct = Merge(oPrev.m_oOverflowPunct, oCurrent.m_oOverflowPunct);
			oProperties.m_oPageBreakBefore = Merge(oPrev.m_oPageBreakBefore, oCurrent.m_oPageBreakBefore);

			if (oCurrent.m_oPBdr.IsInit() && oPrev.m_oPBdr.IsInit())
				oProperties.m_oPBdr = OOX::Logic::CPBdr::Merge(oPrev.m_oPBdr.get(), oCurrent.m_oPBdr.get());
			else
				oProperties.m_oPBdr = Merge(oPrev.m_oPBdr, oCurrent.m_oPBdr);

			oProperties.m_oPPrChange = Merge(oPrev.m_oPPrChange, oCurrent.m_oPPrChange);
			oProperties.m_oPStyle = Merge(oPrev.m_oPStyle, oCurrent.m_oPStyle);
			oProperties.m_oRPr = Merge(oPrev.m_oRPr, oCurrent.m_oRPr);
			oProperties.m_oSectPr = Merge(oPrev.m_oSectPr, oCurrent.m_oSectPr);
			oProperties.m_oShd = Merge(oPrev.m_oShd, oCurrent.m_oShd);
			oProperties.m_oSnapToGrid = Merge(oPrev.m_oSnapToGrid, oCurrent.m_oSnapToGrid);

			if (oCurrent.m_oSpacing.IsInit() && oPrev.m_oSpacing.IsInit())
				oProperties.m_oSpacing = ComplexTypes::Word::CSpacing::Merge(oPrev.m_oSpacing.get(), oCurrent.m_oSpacing.get());
			else
				oProperties.m_oSpacing = Merge(oPrev.m_oSpacing, oCurrent.m_oSpacing);

			oProperties.m_oSuppressAutoHyphens = Merge(oPrev.m_oSuppressAutoHyphens, oCurrent.m_oSuppressAutoHyphens);
			oProperties.m_oSuppressLineNumbers = Merge(oPrev.m_oSuppressLineNumbers, oCurrent.m_oSuppressLineNumbers);
			oProperties.m_oSuppressOverlap = Merge(oPrev.m_oSuppressOverlap, oCurrent.m_oSuppressOverlap);
			oProperties.m_oTabs = Merge(oPrev.m_oTabs, oCurrent.m_oTabs);
			oProperties.m_oTextAlignment = Merge(oPrev.m_oTextAlignment, oCurrent.m_oTextAlignment);
			oProperties.m_oTextboxTightWrap = Merge(oPrev.m_oTextboxTightWrap, oCurrent.m_oTextboxTightWrap);
			oProperties.m_oTextDirection = Merge(oPrev.m_oTextDirection, oCurrent.m_oTextDirection);
			oProperties.m_oTopLinePunct = Merge(oPrev.m_oTopLinePunct, oCurrent.m_oTopLinePunct);
			oProperties.m_oWidowControl = Merge(oPrev.m_oWidowControl, oCurrent.m_oWidowControl);
			oProperties.m_oWordWrap = Merge(oPrev.m_oWordWrap, oCurrent.m_oWordWrap);

			return oProperties;
		}


	} // Logic
} // OOX
