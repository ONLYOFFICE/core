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
			if ( _T("w:rPrChange") != oNode.GetName() )
				return;

            XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
            XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
            XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_rPr;

			if ( m_pRunPr.IsInit() && oNode.GetNode( _T("w:rPr"), oNode_rPr ) )
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

				if ( _T("w:rPr") == sName )
					m_pRunPr->fromXML( oReader );
			}
		}
		std::wstring CRPrChange::toXML() const
		{			
			std::wstring sResult = _T("<w:rPrChange ");

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

			if ( m_pRunPr.IsInit() )
				sResult += m_pRunPr->toXML();

			sResult += _T("</w:rPrChange>");

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
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),	 m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
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

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth )  )
			{
				std::wstring sName = oReader.GetName();

				if ( _T("w:b") == sName )
					FROM_XML_ELEM(m_oBold, oReader, pRun)
				else if ( _T("w:bCs") == sName )
					FROM_XML_ELEM(m_oBoldCs, oReader, pRun)
				else if ( _T("w:bdr") == sName )
					m_oBdr = oReader;
				else if ( _T("w:caps") == sName )
					FROM_XML_ELEM(m_oCaps, oReader, pRun)
				else if ( _T("w:color") == sName )
					m_oColor = oReader;
				else if ( _T("w:cs") == sName )
					FROM_XML_ELEM(m_oCs, oReader, pRun)
				else if ( _T("w:del") == sName )
					m_oDel = oReader;
				else if ( _T("w:dstrike") == sName )
					FROM_XML_ELEM(m_oDStrike, oReader, pRun)
				else if ( _T("w:eastAsianLayout") == sName )
					m_oEastAsianLayout = oReader;
				else if ( _T("w:effect") == sName )
					m_oEffect = oReader;
				else if ( _T("w:em") == sName )
					m_oEm = oReader;
				else if ( _T("w:emboss") == sName )
					FROM_XML_ELEM(m_oEmboss, oReader, pRun)
				else if ( _T("w:fitText") == sName )
					m_oFitText = oReader;
				else if ( _T("w:highlight") == sName )
					m_oHighlight = oReader;
				else if ( _T("w:ins") == sName )
					m_oIns = oReader;
				else if ( _T("w:i") == sName )
					FROM_XML_ELEM(m_oItalic, oReader, pRun)
				else if ( _T("w:iCs") == sName )
					FROM_XML_ELEM(m_oItalicCs, oReader, pRun)
				else if ( _T("w:imprint") == sName )
					FROM_XML_ELEM(m_oImprint, oReader, pRun)
				else if ( _T("w:kern") == sName )
					m_oKern = oReader;
				else if ( _T("w:lang") == sName )
					FROM_XML_ELEM(m_oLang, oReader, pRun)
				else if ( _T("w:noProof") == sName )
					FROM_XML_ELEM(m_oNoProof, oReader, pRun)
				else if ( _T("m:oMath") == sName )
					FROM_XML_ELEM(m_oMath, oReader, pRun)
				else if ( _T("w:moveFrom") == sName )
					m_oMoveFrom = oReader;
				else if ( _T("w:moveTo") == sName )
					m_oMoveTo = oReader;
				else if ( _T("w:outline") == sName )
					FROM_XML_ELEM(m_oOutline, oReader, pRun)
				else if ( _T("w:position") == sName )
					m_oPosition = oReader;
				else if ( _T("w:rFonts") == sName )
					m_oRFonts = oReader;
				else if ( !m_bRPRChange && _T("w:rPrChange") == sName )
					m_oRPrChange = oReader;
				// В спецификации почему-то написано pStyle, хотя по смыслы, по ссылке в самой
				// же спецификации и, в конце концов, по алфавиту тут толжно быть rStyle
				else if ( _T("w:rStyle") == sName )
					m_oRStyle = oReader;
				else if ( !m_oRStyle.IsInit() && _T("w:pStyle") == sName )
					m_oRStyle = oReader;
				else if ( _T("w:rtl") == sName )
					FROM_XML_ELEM(m_oRtL, oReader, pRun)
				else if ( _T("w:shadow") == sName )
					FROM_XML_ELEM(m_oShadow, oReader, pRun)
				else if ( _T("w:shd") == sName )
					m_oShd = oReader;
				else if ( _T("w:smallCaps") == sName )
					FROM_XML_ELEM(m_oSmallCaps, oReader, pRun)
				else if ( _T("w:snapToGrid") == sName )
					FROM_XML_ELEM(m_oSnapToGrid, oReader, pRun)
				else if ( _T("w:spacing") == sName )
					m_oSpacing = oReader;
				else if ( _T("w:specVanish") == sName )
					FROM_XML_ELEM(m_oSpecVanish, oReader, pRun)
				else if ( _T("w:strike") == sName )
					FROM_XML_ELEM(m_oStrike, oReader, pRun)
				else if ( _T("w:sz") == sName )
					m_oSz = oReader;
				else if ( _T("w:szCs") == sName )
					m_oSzCs = oReader;
				else if ( _T("w:u") == sName )
					m_oU = oReader;
				else if ( _T("w:vanish") == sName )
					FROM_XML_ELEM(m_oVanish, oReader, pRun)
				else if ( _T("w:vertAlign") == sName )
					m_oVertAlign = oReader;
				else if ( _T("w:w") == sName )
					m_oW = oReader;
				else if ( _T("w:webHidden") == sName )
					FROM_XML_ELEM(m_oWebHidden, oReader, pRun)
				else if (_T("w14:textOutline") == sName)
					m_oTextOutline = oReader;
				else if (_T("w14:textFill") == sName)
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
		}

		void CRunProperty::fromXML(XmlUtils::CXmlNode& oNode)
		{
			if ( _T("w:rPr") != oNode.GetName() )
				return;

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( _T("w:b"), oChild ) )
				m_oBold = oChild;

			if ( oNode.GetNode( _T("w:bCs"), oChild ) )
			{
				m_oBoldCs = oChild;
				
				XmlUtils::CXmlNode oChild1;
				if (oChild.GetNode(L"w:t", oChild1))
				{
					m_pText = new CText(oChild1);	//XpertdocOnlineDemoEn.docx
				}
			}
			if ( oNode.GetNode( _T("w:bdr"), oChild ) )
				m_oBdr = oChild;

			if ( oNode.GetNode( _T("w:caps"), oChild ) )
				m_oCaps = oChild;

			if ( oNode.GetNode( _T("w:color"), oChild ) )
				m_oColor = oChild;

			if ( oNode.GetNode( _T("w:cs"), oChild ) )
				m_oCs = oChild;

			if ( oNode.GetNode( _T("w:del"), oChild ) )
				m_oDel = oChild;

			if ( oNode.GetNode( _T("w:dstrike"), oChild ) )
				m_oDStrike = oChild;

			if ( oNode.GetNode( _T("w:eastAsianLayout"), oChild ) )
				m_oEastAsianLayout = oChild;

			if ( oNode.GetNode( _T("w:effect"), oChild ) )
				m_oEffect = oChild;

			if ( oNode.GetNode( _T("w:em"), oChild ) )
				m_oEm = oChild;

			if ( oNode.GetNode( _T("w:emboss"), oChild ) )
				m_oEmboss = oChild;

			if ( oNode.GetNode( _T("w:fitText"), oChild ) )
				m_oFitText = oChild;

			if ( oNode.GetNode( _T("w:highlight"), oChild ) )
				m_oHighlight = oChild;

			if ( oNode.GetNode( _T("w:ins"), oChild ) )
				m_oIns = oChild;

			if ( oNode.GetNode( _T("w:i"), oChild ) )
				m_oItalic = oChild;

			if ( oNode.GetNode( _T("w:iCs"), oChild ) )
				m_oItalicCs = oChild;

			if ( oNode.GetNode( _T("w:imprint"), oChild ) )
				m_oImprint = oChild;

			if ( oNode.GetNode( _T("w:kern"), oChild ) )
				m_oKern = oChild;

			if ( oNode.GetNode( _T("w:lang"), oChild ) )
				m_oLang = oChild;

			if ( oNode.GetNode( _T("w:noProof"), oChild ) )
				m_oNoProof = oChild;

			if ( oNode.GetNode( _T("m:oMath"), oChild ) )
				m_oMath = oChild;

			if ( oNode.GetNode( _T("w:outline"), oChild ) )
				m_oOutline = oChild;

			if ( oNode.GetNode( _T("w:position"), oChild ) )
				m_oPosition = oChild;

			if ( oNode.GetNode( _T("w:rFonts"), oChild ) )
				m_oRFonts = oChild;

			if ( !m_bRPRChange && oNode.GetNode( _T("w:rPrChange"), oChild ) )
				m_oRPrChange = oChild;

			// В спецификации почему-то написано pStyle, хотя по смыслы, по ссылке в самой
			// же спецификации и, в конце концов, по алфавиту тут толжно быть rStyle
			if ( oNode.GetNode( _T("w:rStyle"), oChild ) )
				m_oRStyle = oChild;

			if ( !m_oRStyle.IsInit() && oNode.GetNode( _T("w:pStyle"), oChild ) )
				m_oRStyle = oChild;

			if ( oNode.GetNode( _T("w:rtl"), oChild ) )
				m_oRtL = oChild;

			if ( oNode.GetNode( _T("w:shadow"), oChild ) )
				m_oShadow = oChild;

			if ( oNode.GetNode( _T("w:shd"), oChild ) )
				m_oShd = oChild;

			if ( oNode.GetNode( _T("w:smallCaps"), oChild ) )
				m_oSmallCaps = oChild;

			if ( oNode.GetNode( _T("w:snapToGrid"), oChild ) )
				m_oSnapToGrid = oChild;

			if ( oNode.GetNode( _T("w:spacing"), oChild ) )
				m_oSpacing = oChild;

			if ( oNode.GetNode( _T("w:specVanish"), oChild ) )
				m_oSpecVanish = oChild;

			if ( oNode.GetNode( _T("w:strike"), oChild ) )
				m_oStrike = oChild;

			if ( oNode.GetNode( _T("w:sz"), oChild ) )
				m_oSz = oChild;

			if ( oNode.GetNode( _T("w:szCs"), oChild ) )
				m_oSzCs = oChild;

			if ( oNode.GetNode( _T("w:u"), oChild ) )
				m_oU = oChild;

			if ( oNode.GetNode( _T("w:vanish"), oChild ) )
				m_oVanish = oChild;

			if ( oNode.GetNode( _T("w:vertAlign"), oChild ) )
				m_oVertAlign = oChild;

			if ( oNode.GetNode( _T("w:w"), oChild ) )
				m_oW = oChild;

			if ( oNode.GetNode( _T("w:webHidden"), oChild ) )
				m_oWebHidden = oChild;

			if ( oNode.GetNode( _T("w:moveFrom"), oChild ) )
				m_oMoveFrom = oChild;

			if ( oNode.GetNode( _T("w:moveTo"), oChild ) )
				m_oMoveTo = oChild;
		}

	} // Logic
} // ComplexTypes


