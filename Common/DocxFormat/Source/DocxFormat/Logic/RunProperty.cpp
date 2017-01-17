/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

			oNode.ReadAttributeBase( _T("w:author"), m_sAuthor );
			oNode.ReadAttributeBase( _T("w:date"),   m_oDate );
			oNode.ReadAttributeBase( _T("w:id"),     m_oId );
			oNode.ReadAttributeBase( _T("oouserid"), m_sUserId );

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
			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth )  )
			{
                std::wstring sName = oReader.GetName();
				WritingElement *pItem = NULL;

				if ( _T("w:b") == sName )
					m_oBold = oReader;
				else if ( _T("w:bCs") == sName )
					m_oBoldCs = oReader;
				else if ( _T("w:bdr") == sName )
					m_oBdr = oReader;
				else if ( _T("w:caps") == sName )
					m_oCaps = oReader;
				else if ( _T("w:color") == sName )
					m_oColor = oReader;
				else if ( _T("w:cs") == sName )
					m_oCs = oReader;
				else if ( _T("w:del") == sName )
					m_oDel = oReader;
				else if ( _T("w:dstrike") == sName )
					m_oDStrike = oReader;
				else if ( _T("w:eastAsianLayout") == sName )
					m_oEastAsianLayout = oReader;
				else if ( _T("w:effect") == sName )
					m_oEffect = oReader;
				else if ( _T("w:em") == sName )
					m_oEm = oReader;
				else if ( _T("w:emboss") == sName )
					m_oEmboss = oReader;
				else if ( _T("w:fitText") == sName )
					m_oFitText = oReader;
				else if ( _T("w:highlight") == sName )
					m_oHighlight = oReader;
				else if ( _T("w:ins") == sName )
					m_oIns = oReader;
				else if ( _T("w:i") == sName )
					m_oItalic = oReader;
				else if ( _T("w:iCs") == sName )
					m_oItalicCs = oReader;
				else if ( _T("w:imprint") == sName )
					m_oImprint = oReader;
				else if ( _T("w:kern") == sName )
					m_oKern = oReader;
				else if ( _T("w:lang") == sName )
					m_oLang = oReader;
				else if ( _T("w:noProof") == sName )
					m_oNoProof = oReader;
				else if ( _T("m:oMath") == sName )
					m_oMath = oReader;
				else if ( _T("w:outline") == sName )
					m_oOutline = oReader;
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
					m_oRtL = oReader;
				else if ( _T("w:shadow") == sName )
					m_oShadow = oReader;
				else if ( _T("w:shd") == sName )
					m_oShd = oReader;
				else if ( _T("w:smallCaps") == sName )
					m_oSmallCaps = oReader;
				else if ( _T("w:snapToGrid") == sName )
					m_oSnapToGrid = oReader;
				else if ( _T("w:spacing") == sName )
					m_oSpacing = oReader;
				else if ( _T("w:specVanish") == sName )
					m_oSpecVanish = oReader;
				else if ( _T("w:strike") == sName )
					m_oStrike = oReader;
				else if ( _T("w:sz") == sName )
					m_oSz = oReader;
				else if ( _T("w:szCs") == sName )
					m_oSzCs = oReader;
				else if ( _T("w:u") == sName )
					m_oU = oReader;
				else if ( _T("w:vanish") == sName )
					m_oVanish = oReader;
				else if ( _T("w:vertAlign") == sName )
					m_oVertAlign = oReader;
				else if ( _T("w:w") == sName )
					m_oW = oReader;
				else if ( _T("w:webHidden") == sName )
					m_oWebHidden = oReader;
				else if (_T("w14:textOutline") == sName)
					m_sTextOutline = oReader.GetOuterXml();
				else if (_T("w14:textFill") == sName)
				{
					m_sTextFill = oReader.GetOuterXml();

					std::wstring strXml = _T("<xml xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" >");
							strXml += m_sTextFill.get();
							strXml += _T("</xml>");
							
					XmlUtils::CXmlLiteReader oSubReader;//нам нужны xml и сами объекты 
					bool result = oSubReader.FromString(strXml);
					
					result = oSubReader.ReadNextNode();//root
					result = oSubReader.ReadNextNode();//textFill
					result = oSubReader.ReadNextNode();//...Fill

					std::wstring sName = XmlUtils::GetNameNoNS(oSubReader.GetName());
						
					if (_T("gradFill") == sName)
					{
						m_oGradFill = oSubReader;
					}
				}
			}
		}
	} // Logic
} // ComplexTypes


