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
			if ( _T("w:pPrChange") != oNode.GetName() )
				return;

            XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
            XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
            XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_pPr;

			if ( m_pParPr.IsInit() && oNode.GetNode( _T("w:pPr"), oNode_pPr ) )
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
				std::wstring sName = oReader.GetName();
				if ( _T("w:pPr") == sName )
					m_pParPr->fromXML( oReader );
			}
		}

		std::wstring CPPrChange::toXML() const
		{
			std::wstring sResult = _T("<w:pPrChange ");

			if ( m_sAuthor.IsInit() )
			{
				sResult += _T("w:author=\"");
                sResult += m_sAuthor.get2();
				sResult += _T("\" ");
			}

			if ( m_oDate.IsInit() )
			{
				sResult += _T("w:date=\"");
				sResult += m_oDate->ToString();
				sResult += _T("\" ");
			}

			if ( m_oId.IsInit() )
			{
				sResult += _T("w:id=\"");
				sResult += m_oId->ToString();
				sResult += _T("\" ");
			}

			if ( m_sUserId.IsInit() )
			{
				sResult += _T("oouserid=\"");
                sResult += m_sUserId.get2();
				sResult += _T("\" ");
			}

			sResult += _T(">");

			if ( m_pParPr.IsInit() )
				sResult += m_pParPr->toXML();

			sResult += _T("</w:pPrChange>");

			return sResult;
		}
		EElementType CPPrChange::getType() const
		{
			return et_w_pPrChange;
		}
		void CPPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),	 m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CParagraphProperty::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:pPr") != oNode.GetName() )
				return;

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( _T("w:adjustRightInd"), oChild ) )
				m_oAdjustRightInd = oChild;

			if ( oNode.GetNode( _T("w:autoSpaceDE"), oChild ) )
				m_oAutoSpaceDE = oChild;

			if ( oNode.GetNode( _T("w:autoSpaceDN"), oChild ) )
				m_oAutoSpaceDN = oChild;

			if ( oNode.GetNode( _T("w:bidi"), oChild ) )
				m_oBidi = oChild;

			if ( oNode.GetNode( _T("w:cnfStyle"), oChild ) )
				m_oCnfStyle = oChild;

			if ( oNode.GetNode( _T("w:contextualSpacing"), oChild ) )
				m_oContextualSpacing = oChild;

			if ( oNode.GetNode( _T("w:divId"), oChild ) )
				m_oDivID = oChild;

			if ( oNode.GetNode( _T("w:framePr"), oChild ) )
				m_oFramePr = oChild;

			if ( oNode.GetNode( _T("w:ind"), oChild ) )
				m_oInd = oChild;

			if ( oNode.GetNode( _T("w:jc"), oChild ) )
				m_oJc = oChild;

			if ( oNode.GetNode( _T("w:keepLines"), oChild ) )
				m_oKeepLines = oChild;

			if ( oNode.GetNode( _T("w:keepNext"), oChild ) )
				m_oKeepNext = oChild;

			if ( oNode.GetNode( _T("w:kinsoku"), oChild ) )
				m_oKinsoku = oChild;

			if ( oNode.GetNode( _T("w:mirrorIndents"), oChild ) )
				m_oMirrorIndents = oChild;

			if ( oNode.GetNode( _T("w:numPr"), oChild ) )
				m_oNumPr = oChild;
		
			if ( oNode.GetNode( _T("w:listPr"), oChild ) )
				m_oNumPr = oChild;

			if ( oNode.GetNode( _T("w:outlineLvl"), oChild ) )
				m_oOutlineLvl = oChild;

			if ( oNode.GetNode( _T("w:overflowPunct"), oChild ) )
				m_oOverflowPunct = oChild;

			if ( oNode.GetNode( _T("w:pageBreakBefore"), oChild ) )
				m_oPageBreakBefore = oChild;

			if ( oNode.GetNode( _T("w:pBdr"), oChild ) )
				m_oPBdr = oChild;

			if ( !m_bPPrChange && oNode.GetNode( _T("w:pPrChange"), oChild ) )
				m_oPPrChange = oChild;

			if ( oNode.GetNode( _T("w:pStyle"), oChild ) )
				m_oPStyle = oChild;

			if ( !m_bPPrChange && oNode.GetNode( _T("w:rPr"), oChild ) )
				m_oRPr = oChild;

			if ( !m_bPPrChange && oNode.GetNode( _T("w:sectPr"), oChild ) )
				m_oSectPr = oChild;

			if ( oNode.GetNode( _T("w:shd"), oChild ) )
				m_oShd = oChild;

			if ( oNode.GetNode( _T("w:snapToGrid"), oChild ) )
				m_oSnapToGrid = oChild;

			if ( oNode.GetNode( _T("w:spacing"), oChild ) )
				m_oSpacing = oChild;

			if ( oNode.GetNode( _T("w:suppressAutoHyphens"), oChild ) )
				m_oSuppressAutoHyphens = oChild;

			if ( oNode.GetNode( _T("w:suppressLineNumbers"), oChild ) )
				m_oSuppressLineNumbers = oChild;

			if ( oNode.GetNode( _T("w:suppressOverlap"), oChild ) )
				m_oSuppressOverlap = oChild;

			if ( oNode.GetNode( _T("w:tabs"), oChild ) )
				m_oTabs = oChild;

			if ( oNode.GetNode( _T("w:textAlignment"), oChild ) )
				m_oTextAlignment = oChild;

			if ( oNode.GetNode( _T("w:textboxTightWrap"), oChild ) )
				m_oTextboxTightWrap = oChild;

			if ( oNode.GetNode( _T("w:textDirection"), oChild ) )
				m_oTextDirection = oChild;

			if ( oNode.GetNode( _T("w:topLinePunct"), oChild ) )
				m_oTopLinePunct = oChild;

			if ( oNode.GetNode( _T("w:widowControl"), oChild ) )
				m_oWidowControl = oChild;

			if ( oNode.GetNode( _T("w:wordWrap"), oChild ) )
				m_oWordWrap = oChild;
		}
		void CParagraphProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();

				if ( _T("w:adjustRightInd") == sName )
					m_oAdjustRightInd = oReader;
				else if ( _T("w:autoSpaceDE") == sName )
					m_oAutoSpaceDE = oReader;
				else if ( _T("w:autoSpaceDN") == sName )
					m_oAutoSpaceDN = oReader;
				else if ( _T("w:bidi") == sName )
					m_oBidi = oReader;
				else if ( _T("w:cnfStyle") == sName )
					m_oCnfStyle = oReader;
				else if ( _T("w:contextualSpacing") == sName )
					m_oContextualSpacing = oReader;
				else if ( _T("w:divId") == sName )
					m_oDivID = oReader;
				else if ( _T("w:framePr") == sName )
					m_oFramePr = oReader;
				else if ( _T("w:ind") == sName )
					m_oInd = oReader;
				else if ( _T("w:jc") == sName )
					m_oJc = oReader;
				else if ( _T("w:keepLines") == sName )
					m_oKeepLines = oReader;
				else if ( _T("w:keepNext") == sName )
					m_oKeepNext = oReader;
				else if ( _T("w:kinsoku") == sName )
					m_oKinsoku = oReader;
				else if ( _T("w:mirrorIndents") == sName )
					m_oMirrorIndents = oReader;
				else if ( L"w:numPr" == sName || L"w:listPr" == sName)
					m_oNumPr = oReader;
				else if ( _T("w:outlineLvl") == sName )
					m_oOutlineLvl = oReader;
				else if ( _T("w:overflowPunct") == sName )
					m_oOverflowPunct = oReader;
				else if ( _T("w:pageBreakBefore") == sName )
					m_oPageBreakBefore = oReader;
				else if ( _T("w:pBdr") == sName )
					m_oPBdr = oReader;
				else if ( !m_bPPrChange && _T("w:pPrChange") == sName )
					m_oPPrChange = oReader;
				else if ( _T("w:pStyle") == sName )
					m_oPStyle = oReader;
				else if ( !m_bPPrChange && _T("w:rPr") == sName )
					m_oRPr = oReader;
				else if ( !m_bPPrChange && _T("w:sectPr") == sName )
				{
					m_oSectPr = new CSectionProperty(document);
					m_oSectPr->fromXML(oReader);
				}
				else if ( _T("w:shd") == sName )
					m_oShd = oReader;
				else if ( _T("w:snapToGrid") == sName )
					m_oSnapToGrid = oReader;
				else if ( _T("w:spacing") == sName )
					m_oSpacing = oReader;
				else if ( _T("w:suppressAutoHyphens") == sName )
					m_oSuppressAutoHyphens = oReader;
				else if ( _T("w:suppressLineNumbers") == sName )
					m_oSuppressLineNumbers = oReader;
				else if ( _T("w:suppressOverlap") == sName )
					m_oSuppressOverlap = oReader;
				else if ( _T("w:tabs") == sName )
					m_oTabs = oReader;
				else if ( _T("w:textAlignment") == sName )
					m_oTextAlignment = oReader;
				else if ( _T("w:textboxTightWrap") == sName )
					m_oTextboxTightWrap = oReader;
				else if ( _T("w:textDirection") == sName )
					m_oTextDirection = oReader;
				else if ( _T("w:topLinePunct") == sName )
					m_oTopLinePunct = oReader;
				else if ( _T("w:widowControl") == sName )
					m_oWidowControl = oReader;
				else if ( _T("w:wordWrap") == sName )
					m_oWordWrap = oReader;
			}
		}
		std::wstring CParagraphProperty::toXML() const
		{
			std::wstring sResult = _T("<w:pPr>");

			if ( m_oAdjustRightInd.IsInit() ) 
			{
				sResult += _T("<w:adjustRightInd ");
				sResult += m_oAdjustRightInd->ToString();
				sResult += _T("/>");
			}

			if ( m_oAutoSpaceDE.IsInit() ) 
			{ 
				sResult += _T("<w:autoSpaceDE ");
				sResult += m_oAutoSpaceDE->ToString();
				sResult += _T("/>");				
			}

			if ( m_oAutoSpaceDN.IsInit() ) 
			{ 
				sResult += _T("<w:autoSpaceDN ");
				sResult += m_oAutoSpaceDN->ToString();
				sResult += _T("/>");	
			}

			if ( m_oBidi.IsInit() ) 
			{ 
				sResult += _T("<w:bidi ");
				sResult += m_oBidi->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oCnfStyle.IsInit() ) 
			{ 
				sResult += _T("<w:cnfStyle "); 
				sResult += m_oCnfStyle->ToString();
				sResult += _T("/>");	
			}

			if ( m_oContextualSpacing.IsInit() ) 
			{ 
				sResult += _T("<w:contextualSpacing ");
				sResult += m_oContextualSpacing->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oDivID.IsInit() ) 
			{ 
				sResult += _T("<w:divId "); 
				sResult += m_oDivID->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oFramePr.IsInit() )
			{ 
				sResult += _T("<w:framePr "); 
				sResult += m_oFramePr->ToString();	
				sResult += _T("/>");	
			}

			if ( m_oInd.IsInit() )
			{ 
				sResult += _T("<w:ind "); 
				sResult += m_oInd->ToString();		
				sResult += _T("/>");	
			}

			if ( m_oJc.IsInit() ) 
			{ 
				sResult += _T("<w:jc ");
				sResult += m_oJc->ToString();		
				sResult += _T("/>");	
			}

			if ( m_oKeepLines.IsInit() )
			{ 
				sResult += _T("<w:keepLines ");
				sResult += m_oKeepLines->ToString();	
				sResult += _T("/>");			
			}

			if ( m_oKeepNext.IsInit() ) 
			{ 
				sResult += _T("<w:keepNext "); 
				sResult += m_oKeepNext->ToString();
				sResult += _T("/>");	
			}

			if ( m_oKinsoku.IsInit() )
			{
				sResult += _T("<w:kinsoku "); 
				sResult += m_oKinsoku->ToString();	
				sResult += _T("/>");	
			}

			if ( m_oMirrorIndents.IsInit() ) 
			{ 
				sResult += _T("<w:mirrorIndents "); 
				sResult += m_oMirrorIndents->ToString();	
				sResult += _T("/>");			
			}

			if ( m_oNumPr.IsInit() ) 
				sResult += m_oNumPr->toXML();	

			if ( m_oOutlineLvl.IsInit() ) 
			{ 
				sResult += _T("<w:outlineLvl ");
				sResult += m_oOutlineLvl->ToString();		
				sResult += _T("/>");		
			}

			if ( m_oOverflowPunct.IsInit() ) 
			{ 					
				sResult += _T("<w:overflowPunct "); 
				sResult += m_oOverflowPunct->ToString();	
				sResult += _T("/>");			
			}

			if ( m_oPageBreakBefore.IsInit() ) 
			{ 
				sResult += _T("<w:pageBreakBefore "); 
				sResult += m_oPageBreakBefore->ToString();
				sResult += _T("/>");		
			}

			if ( m_oPBdr.IsInit() )
				sResult += m_oPBdr->toXML();	

			if ( !m_bPPrChange && m_oPPrChange.IsInit() )
				sResult += m_oPPrChange->toXML();	

			if ( m_oPStyle.IsInit() )
			{
				sResult += _T("<w:pStyle ");
				sResult += m_oPStyle->ToString();
				sResult += _T("/>");
			}

			if ( !m_bPPrChange && m_oRPr.IsInit() )
				sResult += m_oRPr->toXML();		

			if ( !m_bPPrChange && m_oSectPr.IsInit() ) 
				sResult += m_oSectPr->toXML();	

			if ( m_oShd.IsInit() ) 
			{ 
				sResult += _T("<w:shd "); 
				sResult += m_oShd->ToString();		
				sResult += _T("/>");	
			}

			if ( m_oSnapToGrid.IsInit() ) 
			{ 
				sResult += _T("<w:snapToGrid ");
				sResult += m_oSnapToGrid->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oSpacing.IsInit() ) 
			{
				sResult += _T("<w:spacing "); 
				sResult += m_oSpacing->ToString();		
				sResult += _T("/>");		
			}

			if ( m_oSuppressAutoHyphens.IsInit() ) 
			{ 
				sResult += _T("<w:suppressAutoHyphens ");
				sResult += m_oSuppressAutoHyphens->ToString();		
				sResult += _T("/>");		
			}

			if ( m_oSuppressLineNumbers.IsInit() ) 
			{ 
				sResult += _T("<w:suppressLineNumbers "); 
				sResult += m_oSuppressLineNumbers->ToString();		
				sResult += _T("/>");			
			}

			if ( m_oSuppressOverlap.IsInit() ) 
			{ 
				sResult += _T("<w:suppressOverlap "); 
				sResult += m_oSuppressOverlap->ToString();		
				sResult += _T("/>");			
			}

			if ( m_oTabs.IsInit() ) 
				sResult += m_oTabs->toXML();	

			if ( m_oTextAlignment.IsInit() ) 
			{
				sResult += _T("<w:textAlignment "); 
				sResult += m_oTextAlignment->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oTextboxTightWrap.IsInit() ) 
			{
				sResult += _T("<w:textboxTightWrap ");
				sResult += m_oTextboxTightWrap->ToString();	
				sResult += _T("/>");			
			}

			if ( m_oTextDirection.IsInit() )
			{ 
				sResult += _T("<w:textDirection "); 
				sResult += m_oTextDirection->ToString();	
				sResult += _T("/>");		
			}

			if ( m_oTopLinePunct.IsInit() )
			{ 
				sResult += _T("<w:topLinePunct ");
				sResult += m_oTopLinePunct->ToString();	
				sResult += _T("/>");			
			}

			if ( m_oWidowControl.IsInit() ) 
			{ 
				sResult += _T("<w:widowControl "); 
				sResult += m_oWidowControl->ToString();
				sResult += _T("/>");		
			}

			if ( m_oWordWrap.IsInit() ) 
			{
				sResult += _T("<w:wordWrap "); 
				sResult += m_oWordWrap->ToString();	
				sResult += _T("/>");		
			}

			sResult += _T("</w:pPr>");

			return sResult;
		}


	} // Logic
} // OOX
