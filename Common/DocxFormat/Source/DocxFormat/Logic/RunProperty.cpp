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
#include "RunProperty.h"
#include "RunContent.h"
#include "Run.h"

#define FROM_XML_ELEM(oElem, oReader, pRun) \
{ \
	oElem.Init(); \
	if(pRun) \
	{ \
		oElem->ReadAttributes(oReader); \
		pRun->fromXMLElems(oReader); \
	} \
	else \
	{ \
		oElem->FromXML(oReader); \
	} \
}

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// RPrChange 
		//--------------------------------------------------------------------------------	

		CRPrChange::CRPrChange()
		{
			m_pRunPr.Init();
			m_pRunPr->m_bRPRChange = true;
		}
		CRPrChange::CRPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pRunPr.Init();
			m_pRunPr->m_bRPRChange = true;

			fromXML( oNode );
		}
		CRPrChange::CRPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pRunPr.Init();
			m_pRunPr->m_bRPRChange = true;

			fromXML( oReader );
		}
		CRPrChange::~CRPrChange()
		{
		}
		void CRPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:rPrChange" != oNode.GetName() )
				return;

            XmlMacroReadAttributeBase( oNode, L"w:author", m_sAuthor );
            XmlMacroReadAttributeBase( oNode, L"w:date",   m_oDate );
            XmlMacroReadAttributeBase( oNode, L"w:id",     m_oId );
            XmlMacroReadAttributeBase( oNode, L"oouserid", m_sUserId );

			XmlUtils::CXmlNode oNode_rPr;

			if ( m_pRunPr.IsInit() && oNode.GetNode( L"w:rPr", oNode_rPr ) )
				m_pRunPr->fromXML( oNode_rPr );

		}
		void CRPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( L"w:rPr" == sName )
					m_pRunPr->fromXML( oReader );
			}
		}
		std::wstring CRPrChange::toXML() const
		{			
			std::wstring sResult = L"<w:rPrChange ";

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

			if ( m_pRunPr.IsInit() )
				sResult += m_pRunPr->toXML();

			sResult += L"</w:rPrChange>";

			return sResult;
		}
		EElementType CRPrChange::getType() const
		{
			return et_w_rPrChange;
		}
		void CRPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",	 m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		void CRunProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML(oReader, NULL);
		}
		void CRunProperty::fromXML(XmlUtils::CXmlLiteReader& oReader, CRun* pRun)
		{
			//pRun for Run object -> XpertdocOnlineDemoEn.docx
			if ( oReader.IsEmptyNode() )
				return;

			nullable<ComplexTypes::Word::String> m_oFontName;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth )  )
			{
				std::wstring sName = oReader.GetName();

				if ( L"w:b" == sName )
					FROM_XML_ELEM(m_oBold, oReader, pRun)
				else if ( L"w:bCs" == sName )
					FROM_XML_ELEM(m_oBoldCs, oReader, pRun)
				else if ( L"w:bdr" == sName )
					m_oBdr = oReader;
				else if ( L"w:caps" == sName )
					FROM_XML_ELEM(m_oCaps, oReader, pRun)
				else if ( L"w:color" == sName )
					m_oColor = oReader;
				else if ( L"w:cs" == sName )
					FROM_XML_ELEM(m_oCs, oReader, pRun)
				else if ( L"w:del" == sName )
					m_oDel = oReader;
				else if ( L"w:dstrike" == sName )
					FROM_XML_ELEM(m_oDStrike, oReader, pRun)
				else if ( L"w:eastAsianLayout" == sName )
					m_oEastAsianLayout = oReader;
				else if ( L"w:effect" == sName )
					m_oEffect = oReader;
				else if ( L"w:em" == sName )
					m_oEm = oReader;
				else if ( L"w:emboss" == sName )
					FROM_XML_ELEM(m_oEmboss, oReader, pRun)
				else if ( L"w:fitText" == sName )
					m_oFitText = oReader;
				else if ( L"w:highlight" == sName )
					m_oHighlight = oReader;
				else if ( L"w:ins" == sName )
					m_oIns = oReader;
				else if ( L"w:i" == sName )
					FROM_XML_ELEM(m_oItalic, oReader, pRun)
				else if ( L"w:iCs" == sName )
					FROM_XML_ELEM(m_oItalicCs, oReader, pRun)
				else if ( L"w:imprint" == sName )
					FROM_XML_ELEM(m_oImprint, oReader, pRun)
				else if ( L"w:kern" == sName )
					m_oKern = oReader;
				else if ( L"w:lang" == sName )
					FROM_XML_ELEM(m_oLang, oReader, pRun)
				else if ( L"w:noProof" == sName )
					FROM_XML_ELEM(m_oNoProof, oReader, pRun)
				else if ( L"m:oMath" == sName )
					FROM_XML_ELEM(m_oMath, oReader, pRun)
				else if ( L"w:moveFrom" == sName )
					m_oMoveFrom = oReader;
				else if ( L"w:moveTo" == sName )
					m_oMoveTo = oReader;
				else if ( L"w:outline" == sName )
					FROM_XML_ELEM(m_oOutline, oReader, pRun)
				else if ( L"w:position" == sName )
					m_oPosition = oReader;
				else if ( L"w:rFonts" == sName )
					m_oRFonts = oReader;
				else if (L"wx:font" == sName)
					m_oFontName = oReader;
				else if ( !m_bRPRChange && L"w:rPrChange" == sName )
					m_oRPrChange = oReader;
				// В спецификации почему-то написано pStyle, хотя по смыслы, по ссылке в самой
				// же спецификации и, в конце концов, по алфавиту тут толжно быть rStyle
				else if ( L"w:rStyle" == sName )
					m_oRStyle = oReader;
				else if ( !m_oRStyle.IsInit() && L"w:pStyle" == sName )
					m_oRStyle = oReader;
				else if ( L"w:rtl" == sName )
					FROM_XML_ELEM(m_oRtL, oReader, pRun)
				else if ( L"w:shadow" == sName )
					FROM_XML_ELEM(m_oShadow, oReader, pRun)
				else if ( L"w:shd" == sName )
					m_oShd = oReader;
				else if ( L"w:smallCaps" == sName )
					FROM_XML_ELEM(m_oSmallCaps, oReader, pRun)
				else if ( L"w:snapToGrid" == sName )
					FROM_XML_ELEM(m_oSnapToGrid, oReader, pRun)
				else if ( L"w:spacing" == sName )
					m_oSpacing = oReader;
				else if ( L"w:specVanish" == sName )
					FROM_XML_ELEM(m_oSpecVanish, oReader, pRun)
				else if ( L"w:strike" == sName )
					FROM_XML_ELEM(m_oStrike, oReader, pRun)
				else if ( L"w:sz" == sName )
					m_oSz = oReader;
				else if ( L"w:szCs" == sName || L"w:sz-cs" == sName)
					m_oSzCs = oReader;
				else if ( L"w:u" == sName )
					m_oU = oReader;
				else if ( L"w:vanish" == sName )
					FROM_XML_ELEM(m_oVanish, oReader, pRun)
				else if ( L"w:vertAlign" == sName )
					m_oVertAlign = oReader;
				else if ( L"w:w" == sName )
					m_oW = oReader;
				else if ( L"w:webHidden" == sName )
					FROM_XML_ELEM(m_oWebHidden, oReader, pRun)
				else if (L"w14:textOutline" == sName)
					m_oTextOutline = oReader;
				else if (L"w14:textFill" == sName)
				{
					if ( oReader.IsEmptyNode() )
						continue;

					int nParentDepth1 = oReader.GetDepth();
					while( oReader.ReadNextSiblingNode( nParentDepth1 )  )
					{
						m_oTextFill.fromXML(oReader);
						break;
					}
				}
			}

			if ((m_oFontName.IsInit() && !m_oRFonts.IsInit()) && m_oFontName->m_sVal.IsInit())
			{
				m_oRFonts.Init();
				m_oRFonts->m_sAscii = *m_oFontName->m_sVal;
			}
		}

		void CRunProperty::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( L"w:rPr" != oNode.GetName() )
				return;

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( L"w:b", oChild ) )
				m_oBold = oChild;

			if ( oNode.GetNode( L"w:bCs", oChild ) )
			{
				m_oBoldCs = oChild;
				
				XmlUtils::CXmlNode oChild1;
				if (oChild.GetNode(L"w:t", oChild1))
				{
					m_pText = new CText(oChild1);	//XpertdocOnlineDemoEn.docx
				}
			}
			if ( oNode.GetNode( L"w:bdr", oChild ) )
				m_oBdr = oChild;

			if ( oNode.GetNode( L"w:caps", oChild ) )
				m_oCaps = oChild;

			if ( oNode.GetNode( L"w:color", oChild ) )
				m_oColor = oChild;

			if ( oNode.GetNode( L"w:cs", oChild ) )
				m_oCs = oChild;

			if ( oNode.GetNode( L"w:del", oChild ) )
				m_oDel = oChild;

			if ( oNode.GetNode( L"w:dstrike", oChild ) )
				m_oDStrike = oChild;

			if ( oNode.GetNode( L"w:eastAsianLayout", oChild ) )
				m_oEastAsianLayout = oChild;

			if ( oNode.GetNode( L"w:effect", oChild ) )
				m_oEffect = oChild;

			if ( oNode.GetNode( L"w:em", oChild ) )
				m_oEm = oChild;

			if ( oNode.GetNode( L"w:emboss", oChild ) )
				m_oEmboss = oChild;

			if ( oNode.GetNode( L"w:fitText", oChild ) )
				m_oFitText = oChild;

			if ( oNode.GetNode( L"w:highlight", oChild ) )
				m_oHighlight = oChild;

			if ( oNode.GetNode( L"w:ins", oChild ) )
				m_oIns = oChild;

			if ( oNode.GetNode( L"w:i", oChild ) )
				m_oItalic = oChild;

			if ( oNode.GetNode( L"w:iCs", oChild ) )
				m_oItalicCs = oChild;

			if ( oNode.GetNode( L"w:imprint", oChild ) )
				m_oImprint = oChild;

			if ( oNode.GetNode( L"w:kern", oChild ) )
				m_oKern = oChild;

			if ( oNode.GetNode( L"w:lang", oChild ) )
				m_oLang = oChild;

			if ( oNode.GetNode( L"w:noProof", oChild ) )
				m_oNoProof = oChild;

			if ( oNode.GetNode( L"m:oMath", oChild ) )
				m_oMath = oChild;

			if ( oNode.GetNode( L"w:outline", oChild ) )
				m_oOutline = oChild;

			if ( oNode.GetNode( L"w:position", oChild ) )
				m_oPosition = oChild;

			if ( oNode.GetNode( L"w:rFonts", oChild ) )
				m_oRFonts = oChild;

			if ( !m_bRPRChange && oNode.GetNode( L"w:rPrChange", oChild ) )
				m_oRPrChange = oChild;

			// В спецификации почему-то написано pStyle, хотя по смыслы, по ссылке в самой
			// же спецификации и, в конце концов, по алфавиту тут толжно быть rStyle
			if ( oNode.GetNode( L"w:rStyle", oChild ) )
				m_oRStyle = oChild;

			if ( !m_oRStyle.IsInit() && oNode.GetNode( L"w:pStyle", oChild ) )
				m_oRStyle = oChild;

			if ( oNode.GetNode( L"w:rtl", oChild ) )
				m_oRtL = oChild;

			if ( oNode.GetNode( L"w:shadow", oChild ) )
				m_oShadow = oChild;

			if ( oNode.GetNode( L"w:shd", oChild ) )
				m_oShd = oChild;

			if ( oNode.GetNode( L"w:smallCaps", oChild ) )
				m_oSmallCaps = oChild;

			if ( oNode.GetNode( L"w:snapToGrid", oChild ) )
				m_oSnapToGrid = oChild;

			if ( oNode.GetNode( L"w:spacing", oChild ) )
				m_oSpacing = oChild;

			if ( oNode.GetNode( L"w:specVanish", oChild ) )
				m_oSpecVanish = oChild;

			if ( oNode.GetNode( L"w:strike", oChild ) )
				m_oStrike = oChild;

			if ( oNode.GetNode( L"w:sz", oChild ) )
				m_oSz = oChild;

			if ( oNode.GetNode( L"w:szCs", oChild ) )
				m_oSzCs = oChild;

			if ( oNode.GetNode( L"w:u", oChild ) )
				m_oU = oChild;

			if ( oNode.GetNode( L"w:vanish", oChild ) )
				m_oVanish = oChild;

			if ( oNode.GetNode( L"w:vertAlign", oChild ) )
				m_oVertAlign = oChild;

			if ( oNode.GetNode( L"w:w", oChild ) )
				m_oW = oChild;

			if ( oNode.GetNode( L"w:webHidden", oChild ) )
				m_oWebHidden = oChild;

			if ( oNode.GetNode( L"w:moveFrom", oChild ) )
				m_oMoveFrom = oChild;

			if ( oNode.GetNode( L"w:moveTo", oChild ) )
				m_oMoveTo = oChild;
		}

        std::wstring CRunProperty::toXML() const
		{
            std::wstring sResult = L"<w:rPr>";

			if ( m_oBold.IsInit() )
			{
				sResult += L"<w:b ";
				sResult += m_oBold->ToString();
				sResult += L"/>";
			}

			if ( m_oBoldCs.IsInit() )
			{
				sResult += L"<w:bCs ";
				sResult += m_oBoldCs->ToString();
				sResult += L"/>";
			}

			if ( m_oBdr.IsInit() )
			{
				sResult += L"<w:bdr ";
				sResult += m_oBdr->ToString();
				sResult += L"/>";
			}

			if ( m_oCaps.IsInit() )
			{
				sResult += L"<w:caps ";
				sResult += m_oCaps->ToString();
				sResult += L"/>";
			}

			if ( m_oColor.IsInit() )
			{
				sResult += L"<w:color ";
				sResult += m_oColor->ToString();
				sResult += L"/>";
			}

			if ( m_oCs.IsInit() )
			{
				sResult += L"<w:cs ";
				sResult += m_oCs->ToString();
				sResult += L"/>";
			}

			if ( m_oDel.IsInit() )
			{
				sResult += L"<w:del ";
				sResult += m_oDel->ToString();
				sResult += L"/>";
			}

			if ( (m_oDStrike.IsInit()) && (m_oDStrike->m_oVal.ToBool()) )
			{
				sResult += L"<w:dstrike ";
				sResult += m_oDStrike->ToString();
				sResult += L"/>";
			}

			if ( m_oEastAsianLayout.IsInit() )
			{
				sResult += L"<w:eastAsianLayout ";
				sResult += m_oEastAsianLayout->ToString();
				sResult += L"/>";
			}

			if ( m_oEffect.IsInit() )
			{
				sResult += L"<w:effect ";
				sResult += m_oEffect->ToString();
				sResult += L"/>";
			}

			if ( m_oEm.IsInit() )
			{
				sResult += L"<w:em ";
				sResult += m_oEm->ToString();
				sResult += L"/>";
			}

			if ( m_oEmboss.IsInit() )
			{
				sResult += L"<w:emboss ";
				sResult += m_oEmboss->ToString();
				sResult += L"/>";
			}

			if ( m_oFitText.IsInit() )
			{
				sResult += L"<w:fitText ";
				sResult += m_oFitText->ToString();
				sResult += L"/>";
			}

			if ( m_oHighlight.IsInit() )
			{
				sResult += L"<w:highlight ";
				sResult += m_oHighlight->ToString();
				sResult += L"/>";
			}

			if ( m_oIns.IsInit() )
			{
				sResult += L"<w:ins ";
				sResult += m_oIns->ToString();
				sResult += L"/>";
			}

			if ( m_oItalic.IsInit() )
			{
				sResult += L"<w:i ";
				sResult += m_oItalic->ToString();
				sResult += L"/>";
			}

			if ( m_oItalicCs.IsInit() )
			{
				sResult += L"<w:iCs ";
				sResult += m_oItalicCs->ToString();
				sResult += L"/>";
			}

			if ( m_oImprint.IsInit() )
			{
				sResult += L"<w:imprint ";
				sResult += m_oImprint->ToString();
				sResult += L"/>";
			}

			if ( m_oKern.IsInit() )
			{
				sResult += L"<w:kern ";
				sResult += m_oKern->ToString();
				sResult += L"/>";
			}

			if ( m_oLang.IsInit() )
			{
				sResult += L"<w:lang ";
				sResult += m_oLang->ToString();
				sResult += L"/>";
			}

			if ( m_oNoProof.IsInit() )
			{
				sResult += L"<w:noProof ";
				sResult += m_oNoProof->ToString();
				sResult += L"/>";
			}

			if ( m_oMath.IsInit() )
			{
				sResult += L"<m:oMath ";
				sResult += m_oMath->ToString();
				sResult += L"/>";
			}

			if ( m_oMoveFrom.IsInit() )
			{
				sResult += L"<w:moveFrom ";
				sResult += m_oMoveFrom->ToString();
				sResult += L"/>";
			}

			if ( m_oMoveTo.IsInit() )
			{
				sResult += L"<w:moveTo ";
				sResult += m_oMoveTo->ToString();
				sResult += L"/>";
			}

			if ( m_oOutline.IsInit() )
			{
				sResult += L"<w:outline ";
				sResult += m_oOutline->ToString();
				sResult += L"/>";
			}

			if ( m_oPosition.IsInit() )
			{
				sResult += L"<w:position ";
				sResult += m_oPosition->ToString();
				sResult += L"/>";
			}

			if ( m_oRFonts.IsInit() )
			{
				sResult += L"<w:rFonts ";
				sResult += m_oRFonts->ToString();
				sResult += L"/>";
			}

			if ( !m_bRPRChange && m_oRPrChange.IsInit() )
				sResult += m_oRPrChange->toXML();

			if ( m_oRStyle.IsInit() )
			{
				sResult += L"<w:rStyle ";
				sResult += m_oRStyle->ToString();
				sResult += L"/>";
			}

			if ( m_oRtL.IsInit() )
			{
				sResult += L"<w:rtl ";
				sResult += m_oRtL->ToString();
				sResult += L"/>";
			}

			if ( m_oShadow.IsInit() )
			{
				sResult += L"<w:shadow ";
				sResult += m_oShadow->ToString();
				sResult += L"/>";
			}

			if ( m_oShd.IsInit() )
			{
				sResult += L"<w:shd ";
				sResult += m_oShd->ToString();
				sResult += L"/>";
			}

			if ( m_oSmallCaps.IsInit() )
			{
				sResult += L"<w:smallCaps ";
				sResult += m_oSmallCaps->ToString();
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

			if ( m_oSpecVanish.IsInit() )
			{
				sResult += L"<w:specVanish ";
				sResult += m_oSpecVanish->ToString();
				sResult += L"/>";
			}

			if ( m_oStrike.IsInit() )
			{
				sResult += L"<w:strike ";
				sResult += m_oStrike->ToString();
				sResult += L"/>";
			}

			if ( m_oSz.IsInit() )
			{
				sResult += L"<w:sz ";
				sResult += m_oSz->ToString();
				sResult += L"/>";
			}

			if ( m_oSzCs.IsInit() )
			{
				sResult += L"<w:szCs ";
				sResult += m_oSzCs->ToString();
				sResult += L"/>";
			}

			if ( m_oU.IsInit() )
			{
				sResult += L"<w:u ";
				sResult += m_oU->ToString();
				sResult += L"/>";
			}

			if ( m_oVanish.IsInit() )
			{
				sResult += L"<w:vanish ";
				sResult += m_oVanish->ToString();
				sResult += L"/>";
			}

			if ( m_oVertAlign.IsInit() )
			{
				sResult += L"<w:vertAlign ";
				sResult += m_oVertAlign->ToString();
				sResult += L"/>";
			}

			if ( m_oW.IsInit() )
			{
				sResult += L"<w:vertAlign ";
				sResult += m_oW->ToString();
				sResult += L"/>";
			}

			if ( m_oWebHidden.IsInit() )
			{
				sResult += L"<w:webHidden ";
				sResult += m_oWebHidden->ToString();
				sResult += L"/>";
			}

			sResult += L"</w:rPr>";

			return sResult;
		}



	} // Logic
} // ComplexTypes


