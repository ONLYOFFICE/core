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
		// StylisticSets
		//--------------------------------------------------------------------------------

		CStylisticSets::CStylisticSets(OOX::Document *pMain) : WritingElementWithChilds<ComplexTypes::Word::CStylisticSet>(pMain)
		{
		}
		CStylisticSets::~CStylisticSets() {}
		void CStylisticSets::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nParentDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nParentDepth))
			{
				std::wstring sName = oReader.GetNameNoNS();

				if (L"stylisticSet" == sName)
				{
					m_arrItems.push_back(new ComplexTypes::Word::CStylisticSet(oReader));
				}
			}
		}
		void CStylisticSets::fromXML(XmlUtils::CXmlNode& node)
		{
		}
		std::wstring CStylisticSets::toXML() const
		{
			std::wstring sResult = L"<w14:stylisticSets>";

			for (size_t i = 0; i < m_arrItems.size(); ++i)
			{
				if (m_arrItems[i])
				{
					sResult += m_arrItems[i]->ValNode(L"w14:stylisticSet");
				}
			}

			sResult += L"</w14:stylisticSets>";
			return sResult;
		}
		EElementType CStylisticSets::getType() const
		{
			return et_w_stylisticSets;
		}

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

			if (m_oId.IsInit())
			{
				sResult += L"w:id=\"";
				sResult += m_oId->ToString();
				sResult += L"\" ";
			}
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
			WritingElement_ReadAttributes_Start( oReader )
				WritingElement_ReadAttributes_Read_if     ( oReader, L"w:author", m_sAuthor )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:date",   m_oDate )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"w:id",	 m_oId )
				WritingElement_ReadAttributes_Read_else_if( oReader, L"oouserid", m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}

		//--------------------------------------------------------------------------------
		// RunProperty
		//--------------------------------------------------------------------------------

		CRunProperty::CRunProperty()
		{
			m_pText = NULL;
			m_bRPRChange = false;
		}
		CRunProperty::~CRunProperty()
		{
			RELEASEOBJECT(m_pText);
		}
		CRunProperty::CRunProperty(const XmlUtils::CXmlNode &oNode)
		{
			m_pText = NULL;
			m_bRPRChange = false;
			fromXML( (XmlUtils::CXmlNode &)oNode );
		}
		CRunProperty::CRunProperty(const XmlUtils::CXmlLiteReader& oReader)
		{
			m_pText = NULL;
			m_bRPRChange = false;
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
		}
		const CRunProperty& CRunProperty::operator=(const XmlUtils::CXmlNode &oNode)
		{
			fromXML( (XmlUtils::CXmlNode &)oNode );
			return *this;
		}
		const CRunProperty& CRunProperty::operator=(const XmlUtils::CXmlLiteReader& oReader)
		{
			fromXML( (XmlUtils::CXmlLiteReader&)oReader );
			return *this;
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
//---ext----------------------------------------------------------------------------------------------------
				else if (L"w14:textOutline" == sName)
					m_oTextOutline = oReader;
				else if (L"w14:ligatures" == sName)
					m_oLigatures = oReader;
				else if (L"w14:numSpacing" == sName)
					m_oNumSpacing = oReader;
				else if (L"w14:numForm" == sName)
					m_oNumForm = oReader;
				else if (L"w14:stylisticSets" == sName)
					m_oStylisticSets = oReader;
				else if (L"w14:cntxtAlts" == sName)
					m_oCntxtAlts = oReader;
				else if (L"w14:reflection" == sName)
					m_oReflection = oReader;
				else if (L"w14:glow" == sName)
					m_oGlow = oReader;
				else if (L"w14:shadow" == sName)
					m_oShadowExt = oReader;
				else if (L"w14:props3d" == sName)
					m_oProps3d = oReader;
				else if (L"w14:scene3d" == sName)
					m_oScene3d = oReader;
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
		void CRunProperty::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			if (!pWriter) return;
			pWriter->WriteString(toXML());
		}
        std::wstring CRunProperty::toXML() const
		{
            std::wstring sResult = L"<w:rPr>";

			if (m_oDel.IsInit())		sResult += m_oDel->ValNode(L"w:del");
			if (m_oIns.IsInit())		sResult += m_oIns->ValNode(L"w:ins");
			if (m_oMoveFrom.IsInit())	sResult += m_oMoveFrom->ValNode(L"w:moveFrom");
			if (m_oMoveTo.IsInit())		sResult += m_oMoveTo->ValNode(L"w:moveTo");
//---base----------------------------------------------------------------------------------------
			if (m_oRStyle.IsInit())		sResult += m_oRStyle->ValNode(L"w:rStyle");
			if (m_oRFonts.IsInit())		sResult += m_oRFonts->ValNode(L"w:rFonts");
			if (m_oBold.IsInit())		sResult += m_oBold->ValNode(L"w:b");
			if (m_oBoldCs.IsInit())		sResult += m_oBoldCs->ValNode(L"w:bCs");
			if (m_oItalic.IsInit())		sResult += m_oItalic->ValNode(L"w:i");
			if (m_oItalicCs.IsInit())	sResult += m_oItalicCs->ValNode(L"w:iCs");
			if (m_oCaps.IsInit())		sResult += m_oCaps->ValNode(L"w:caps");
			if (m_oSmallCaps.IsInit())	sResult += m_oSmallCaps->ValNode(L"w:smallCaps");
			if (m_oStrike.IsInit())		sResult += m_oStrike->ValNode(L"w:strike");
			if ((m_oDStrike.IsInit()) && (m_oDStrike->m_oVal.ToBool()))
										sResult += m_oDStrike->ValNode(L"w:dstrike");
			if (m_oOutline.IsInit())		sResult += m_oOutline->ValNode(L"w:outline");
			if (m_oShadow.IsInit())			sResult += m_oShadow->ValNode(L"w:shadow");
			if (m_oEmboss.IsInit())			sResult += m_oEmboss->ValNode(L"w:emboss");
			if (m_oImprint.IsInit())		sResult += m_oImprint->ValNode(L"w:imprint");
			if (m_oNoProof.IsInit())		sResult += m_oNoProof->ValNode(L"w:noProof");
			if (m_oSnapToGrid.IsInit())		sResult += m_oSnapToGrid->ValNode(L"w:snapToGrid");
			if (m_oVanish.IsInit())			sResult += m_oVanish->ValNode(L"w:vanish");
			if (m_oWebHidden.IsInit())		sResult += m_oWebHidden->ValNode(L"w:webHidden");
			if (m_oColor.IsInit())			sResult += m_oColor->ValNode(L"w:color");
			if (m_oSpacing.IsInit())		sResult += m_oSpacing->ValNode(L"w:spacing");
			if (m_oW.IsInit())				sResult += m_oW->ValNode(L"w:w");
			if (m_oKern.IsInit())			sResult += m_oKern->ValNode(L"w:kern");
			if (m_oPosition.IsInit())		sResult += m_oPosition->ValNode(L"w:position");
			if (m_oSz.IsInit())				sResult += m_oSz->ValNode(L"w:sz");
			if (m_oSzCs.IsInit())			sResult += m_oSzCs->ValNode(L"w:szCs");
			if (m_oHighlight.IsInit())		sResult += m_oHighlight->ValNode(L"w:highlight");
			if (m_oU.IsInit())				sResult += m_oU->ValNode(L"w:u");
			if (m_oEffect.IsInit())			sResult += m_oEffect->ValNode(L"w:effect");
			if (m_oBdr.IsInit())			sResult += m_oBdr->ValNode(L"w:bdr");
			if (m_oShd.IsInit())			sResult += m_oShd->ValNode(L"w:shd");
			if (m_oFitText.IsInit())		sResult += m_oFitText->ValNode(L"w:fitText");
			if (m_oVertAlign.IsInit())		sResult += m_oVertAlign->ValNode(L"w:vertAlign");
			if (m_oRtL.IsInit())			sResult += m_oRtL->ValNode(L"w:rtl");
			if (m_oCs.IsInit())			sResult += m_oCs->ValNode(L"w:cs");
			if (m_oEm.IsInit())				sResult += m_oEm->ValNode(L"w:em");
			if (m_oLang.IsInit())			sResult += m_oLang->ValNode(L"w:lang");
			if (m_oEastAsianLayout.IsInit())sResult += m_oEastAsianLayout->ValNode(L"w:eastAsianLayout");	
			if (m_oSpecVanish.IsInit())		sResult += m_oSpecVanish->ValNode(L"w:specVanish");
			if (m_oMath.IsInit())			sResult += m_oMath->ValNode(L"m:oMath");
//---ext----------------------------------------------------------------------------------------------------
			NSBinPptxRW::CXmlWriter xmlWriter;
			xmlWriter.m_lDocType = XMLWRITER_DOC_TYPE_WORDART;
			
			if (m_oGlow.IsInit())
			{
				m_oGlow->toXmlWriter(&xmlWriter);
			}
			if (m_oShadowExt.IsInit())
			{
				m_oShadowExt->m_name = L"shadow";
				m_oShadowExt->toXmlWriter(&xmlWriter);
			}
			if (m_oReflection.IsInit())
			{
				m_oReflection->toXmlWriter(&xmlWriter);
			}
			if (m_oTextOutline.IsInit())
			{
				m_oTextOutline->m_name = L"textOutline";
				m_oTextOutline->toXmlWriter(&xmlWriter);
			}
			if (m_oTextFill.is_init())
			{
				xmlWriter.StartNode(L"w14:textFill");
				xmlWriter.EndAttributes();
				m_oTextFill.toXmlWriter(&xmlWriter);
				xmlWriter.EndNode(L"w14:textFill");
			}
			if (m_oProps3d.IsInit())
			{
				m_oProps3d->m_name = L"props3d";
				m_oProps3d->toXmlWriter(&xmlWriter);
			}
			if (m_oScene3d.IsInit())
			{
				m_oScene3d->toXmlWriter(&xmlWriter);
			}
			sResult += xmlWriter.GetXmlString();

			if (m_oLigatures.IsInit())		sResult += m_oLigatures->ValNode(L"w14:ligatures");
			if (m_oNumForm.IsInit())		sResult += m_oNumForm->ValNode(L"w14:numForm");
			if (m_oNumSpacing.IsInit())		sResult += m_oNumSpacing->ValNode(L"w14:numSpacing");
			if (m_oStylisticSets.IsInit())	sResult += m_oStylisticSets->toXML();
			if (m_oCntxtAlts.IsInit())		sResult += m_oCntxtAlts->ValNode(L"w14:cntxtAlts");

			//----------------------------------------------------------------------------------------------------------			
			if (!m_bRPRChange && m_oRPrChange.IsInit())
				sResult += m_oRPrChange->toXML();

			sResult += L"</w:rPr>";
			return sResult;
		}
		void CRunProperty::Clear()
		{
			m_oBold.reset();
			m_oBoldCs.reset();
			m_oBdr.reset();
			m_oCaps.reset();
			m_oColor.reset();
			m_oCs.reset();
			m_oDel.reset();
			m_oDStrike.reset();
			m_oEastAsianLayout.reset();
			m_oEffect.reset();
			m_oEm.reset();
			m_oEmboss.reset();
			m_oFitText.reset();
			m_oHighlight.reset();
			m_oIns.reset();
			m_oItalic.reset();
			m_oItalicCs.reset();
			m_oImprint.reset();
			m_oKern.reset();
			m_oLang.reset();
			m_oNoProof.reset();
			m_oMath.reset();
			m_oMoveFrom.reset();
			m_oMoveTo.reset();
			m_oOutline.reset();
			m_oPosition.reset();
			m_oRFonts.reset();
			m_oRPrChange.reset();
			m_oRStyle.reset();
			m_oRtL.reset();
			m_oShadow.reset();
			m_oShd.reset();
			m_oSmallCaps.reset();
			m_oSnapToGrid.reset();
			m_oSpacing.reset();
			m_oSpecVanish.reset();
			m_oStrike.reset();
			m_oSz.reset();
			m_oSzCs.reset();
			m_oU.reset();
			m_oVanish.reset();
			m_oVertAlign.reset();
			m_oW.reset();
			m_oWebHidden.reset();
			m_oTextFill.Fill.reset();
			m_oTextOutline.reset();
			m_oLigatures.reset();
			m_oNumSpacing.reset();
			m_oNumForm.reset();
			m_oStylisticSets.reset();
			m_oCntxtAlts.reset();
			m_oReflection.reset();
			m_oGlow.reset();
			m_oShadowExt.reset();
			m_oProps3d.reset();
			m_oScene3d.reset();
		}
		bool CRunProperty::IsNoEmpty()
		{
			return (m_oBold.IsInit() || m_oBoldCs.IsInit() || m_oBdr.IsInit() || m_oCaps.IsInit() ||
				 m_oColor.IsInit() || m_oCs.IsInit() || m_oDel.IsInit() || m_oDStrike.IsInit() || m_oEastAsianLayout.IsInit() ||
				 m_oEffect.IsInit() || m_oEm.IsInit() || m_oEmboss.IsInit() || m_oFitText.IsInit() || m_oHighlight.IsInit() ||
				 m_oIns.IsInit() || m_oItalic.IsInit() || m_oItalicCs.IsInit() || m_oImprint.IsInit() ||
				 m_oKern.IsInit() || m_oLang.IsInit() || m_oNoProof.IsInit() || m_oMath.IsInit() || m_oMoveFrom.IsInit() ||
				 m_oMoveTo.IsInit() || m_oOutline.IsInit() || m_oPosition.IsInit() || m_oRFonts.IsInit() ||
				 m_oRPrChange.IsInit() || m_oRStyle.IsInit() || m_oRtL.IsInit() || m_oShadow.IsInit() || m_oShd.IsInit() ||
				 m_oSmallCaps.IsInit() || m_oSnapToGrid.IsInit() || m_oSpacing.IsInit() || m_oSpecVanish.IsInit() ||
				 m_oStrike.IsInit() || m_oSz.IsInit() || m_oSzCs.IsInit() || m_oU.IsInit() || m_oVanish.IsInit() ||
				 m_oVertAlign.IsInit() || m_oW.IsInit() || m_oWebHidden.IsInit() || m_oTextFill.is_init() ||
				 m_oTextOutline.IsInit() || m_oLigatures.IsInit() || m_oNumSpacing.IsInit() || m_oNumForm.IsInit() ||
				 m_oStylisticSets.IsInit() || m_oCntxtAlts.IsInit() || m_oReflection.IsInit() || m_oGlow.IsInit() ||
				 m_oShadowExt.IsInit() || m_oProps3d.IsInit() || m_oScene3d.IsInit());
		}
		const CRunProperty CRunProperty::Merge(const CRunProperty& oPrev, const CRunProperty& oCurrent)
		{
			CRunProperty oProperties;
			oProperties.m_oBold = Merge(oPrev.m_oBold, oCurrent.m_oBold);
			oProperties.m_oBoldCs = Merge(oPrev.m_oBoldCs, oCurrent.m_oBoldCs);
			oProperties.m_oBdr = Merge(oPrev.m_oBdr, oCurrent.m_oBdr);
			oProperties.m_oCaps = Merge(oPrev.m_oCaps, oCurrent.m_oCaps);
			oProperties.m_oColor = Merge(oPrev.m_oColor, oCurrent.m_oColor);
			oProperties.m_oCs = Merge(oPrev.m_oCs, oCurrent.m_oCs);
			oProperties.m_oDel = Merge(oPrev.m_oDel, oCurrent.m_oDel);
			oProperties.m_oDStrike = Merge(oPrev.m_oDStrike, oCurrent.m_oDStrike);
			oProperties.m_oEastAsianLayout = Merge(oPrev.m_oEastAsianLayout, oCurrent.m_oEastAsianLayout);
			oProperties.m_oEffect = Merge(oPrev.m_oEffect, oCurrent.m_oEffect);
			oProperties.m_oEm = Merge(oPrev.m_oEm, oCurrent.m_oEm);
			oProperties.m_oEmboss = Merge(oPrev.m_oEmboss, oCurrent.m_oEmboss);
			oProperties.m_oFitText = Merge(oPrev.m_oFitText, oCurrent.m_oFitText);
			oProperties.m_oHighlight = Merge(oPrev.m_oHighlight, oCurrent.m_oHighlight);
			oProperties.m_oIns = Merge(oPrev.m_oIns, oCurrent.m_oIns);
			oProperties.m_oItalic = Merge(oPrev.m_oItalic, oCurrent.m_oItalic);
			oProperties.m_oItalicCs = Merge(oPrev.m_oItalicCs, oCurrent.m_oItalicCs);
			oProperties.m_oImprint = Merge(oPrev.m_oImprint, oCurrent.m_oImprint);
			oProperties.m_oKern = Merge(oPrev.m_oKern, oCurrent.m_oKern);
			oProperties.m_oLang = Merge(oPrev.m_oLang, oCurrent.m_oLang);
			oProperties.m_oNoProof = Merge(oPrev.m_oNoProof, oCurrent.m_oNoProof);
			oProperties.m_oMath = Merge(oPrev.m_oMath, oCurrent.m_oMath);
			oProperties.m_oMoveFrom = Merge(oPrev.m_oMoveFrom, oCurrent.m_oMoveFrom);
			oProperties.m_oMoveTo = Merge(oPrev.m_oMoveTo, oCurrent.m_oMoveTo);
			oProperties.m_oOutline = Merge(oPrev.m_oOutline, oCurrent.m_oOutline);
			oProperties.m_oPosition = Merge(oPrev.m_oPosition, oCurrent.m_oPosition);
			oProperties.m_oRFonts = Merge(oPrev.m_oRFonts, oCurrent.m_oRFonts);
			oProperties.m_oRStyle = Merge(oPrev.m_oRStyle, oCurrent.m_oRStyle);
			oProperties.m_oRtL = Merge(oPrev.m_oRtL, oCurrent.m_oRtL);
			oProperties.m_oShadow = Merge(oPrev.m_oShadow, oCurrent.m_oShadow);
			oProperties.m_oShd = Merge(oPrev.m_oShd, oCurrent.m_oShd);
			oProperties.m_oSmallCaps = Merge(oPrev.m_oSmallCaps, oCurrent.m_oSmallCaps);
			oProperties.m_oSnapToGrid = Merge(oPrev.m_oSnapToGrid, oCurrent.m_oSnapToGrid);
			oProperties.m_oSpacing = Merge(oPrev.m_oSpacing, oCurrent.m_oSpacing);
			oProperties.m_oSpecVanish = Merge(oPrev.m_oSpecVanish, oCurrent.m_oSpecVanish);
			oProperties.m_oStrike = Merge(oPrev.m_oStrike, oCurrent.m_oStrike);
			oProperties.m_oSz = Merge(oPrev.m_oSz, oCurrent.m_oSz);
			oProperties.m_oSzCs = Merge(oPrev.m_oSzCs, oCurrent.m_oSzCs);
			oProperties.m_oU = Merge(oPrev.m_oU, oCurrent.m_oU);
			oProperties.m_oVanish = Merge(oPrev.m_oVanish, oCurrent.m_oVanish);
			oProperties.m_oVertAlign = Merge(oPrev.m_oVertAlign, oCurrent.m_oVertAlign);
			oProperties.m_oW = Merge(oPrev.m_oW, oCurrent.m_oW);
			oProperties.m_oWebHidden = Merge(oPrev.m_oWebHidden, oCurrent.m_oWebHidden);

			return oProperties;
		}
		EElementType CRunProperty::getType() const
		{
			return et_w_rPr;
		}


	} // Logic
} // ComplexTypes


