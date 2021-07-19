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
#include "../DocxFlat.h"
#include "../HeaderFooter.h"

#include "SectionProperty.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// SectPrChange 
		//--------------------------------------------------------------------------------	

		CSectPrChange::CSectPrChange()
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlNode& oNode)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oNode );
		}
		CSectPrChange::CSectPrChange(XmlUtils::CXmlLiteReader& oReader)
		{
			m_pSecPr.Init();
			m_pSecPr->m_bSectPrChange = true;

			fromXML( oReader );
		}
		CSectPrChange::~CSectPrChange()
		{
		}
		void CSectPrChange::fromXML(XmlUtils::CXmlNode& oNode)
		{
            XmlMacroReadAttributeBase( oNode, _T("w:author"), m_sAuthor );
            XmlMacroReadAttributeBase( oNode, _T("w:date"),   m_oDate );
            XmlMacroReadAttributeBase( oNode, _T("w:id"),     m_oId );
            XmlMacroReadAttributeBase( oNode, _T("oouserid"), m_sUserId );

			XmlUtils::CXmlNode oNode_sectPr;

			if ( m_pSecPr.IsInit() && oNode.GetNode( _T("w:sectPr"), oNode_sectPr ) )
				m_pSecPr->fromXML( oNode_sectPr );

		}
		void CSectPrChange::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( _T("w:sectPr") == sName )
					m_pSecPr->fromXML( oReader );
			}
		}
		std::wstring CSectPrChange::toXML() const
		{			
			std::wstring sResult = _T("<w:sectPrChange ");

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

			if ( m_pSecPr.IsInit() )
				sResult += m_pSecPr->toXML();

			sResult += _T("</w:sectPrChange>");

			return sResult;
		}
		EElementType CSectPrChange::getType() const
		{
			return et_w_sectPrChange;
		}
		void CSectPrChange::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:author"), m_sAuthor )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:date"),   m_oDate )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:id"),     m_oId )
			WritingElement_ReadAttributes_Read_else_if( oReader, _T("oouserid"), m_sUserId )
			WritingElement_ReadAttributes_End( oReader )
		}
		void CSectionProperty::fromXML(XmlUtils::CXmlNode &oNode)
		{
			if ( (L"w:sectPr") != oNode.GetName() )
				return;

			XmlMacroReadAttributeBase( oNode, (L"w:rsidDel"),  m_oRsidDel );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidR"),    m_oRsidR );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidRPr"),  m_oRsidRPr );
			XmlMacroReadAttributeBase( oNode, (L"w:rsidSect"), m_oRsidSect );

			XmlUtils::CXmlNode oChild;

			if ( oNode.GetNode( (L"w:bidi"), oChild ) )
				m_oBidi = oChild;

			if ( oNode.GetNode( (L"w:cols"), oChild ) )
				m_oCols = oChild;

			if ( oNode.GetNode( (L"w:docGrid"), oChild ) )
				m_oDocGrid = oChild;

			if ( oNode.GetNode( (L"w:endnotePr"), oChild ) )
				m_oEndnotePr = oChild;

			if ( !m_bSectPrChange )
			{
				XmlUtils::CXmlNodes oNodes;
				if ( oNode.GetNodes( (L"w:footerReference"), oNodes ) )
				{
					XmlUtils::CXmlNode oFooterNode;
					for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
					{
						if ( oNodes.GetAt( nIndex, oFooterNode ) )
						{
							ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oFooterNode);
							if (oFooter) m_arrFooterReference.push_back( oFooter );
						}
					}
				}
			}

			if ( oNode.GetNode( (L"w:footnotePr"), oChild ) )
				m_oFootnotePr = oChild;

			if ( oNode.GetNode( (L"w:formProt"), oChild ) )
				m_oFormProt = oChild;

			if ( !m_bSectPrChange )
			{
				XmlUtils::CXmlNodes oNodes;
				if ( oNode.GetNodes( (L"w:headerReference"), oNodes ) )
				{
					XmlUtils::CXmlNode oHeaderNode;
					for ( int nIndex = 0; nIndex < oNodes.GetCount(); nIndex++ )
					{
						if ( oNodes.GetAt( nIndex, oHeaderNode ) )
						{
							ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef(oHeaderNode);
							if (oHeader) m_arrHeaderReference.push_back( oHeader );
						}
					}
				}
			}

			if ( oNode.GetNode( (L"w:lnNumType"), oChild ) )
				m_oLnNumType = oChild;

			if ( oNode.GetNode( (L"w:noEndnote"), oChild ) )
				m_oNoEndnote = oChild;

			if ( oNode.GetNode( (L"w:paperSrc"), oChild ) )
				m_oPaperSrc = oChild;

			if ( oNode.GetNode( (L"w:pgBorders"), oChild ) )
				m_oPgBorders = oChild;

			if ( oNode.GetNode( (L"w:pgMar"), oChild ) )
				m_oPgMar = oChild;

			if ( oNode.GetNode( (L"w:pgNumType"), oChild ) )
				m_oPgNumType = oChild;

			if ( oNode.GetNode( (L"w:pgSz"), oChild ) )
				m_oPgSz = oChild;

			if ( oNode.GetNode( (L"w:printerSettings"), oChild ) )
				m_oPrinterSettings = oChild;

			if ( oNode.GetNode( (L"w:rtlGutter"), oChild ) )
				m_oRtlGutter = oChild;

			if ( !m_bSectPrChange && oNode.GetNode( (L"w:sectPrChange"), oChild ) )
				m_oSectPrChange = oChild;

			if ( oNode.GetNode( (L"w:textDirection"), oChild ) )
				m_oTextDirection = oChild;

			if ( oNode.GetNode( (L"w:titlePg"), oChild ) )
				m_oTitlePg = oChild;

			if ( oNode.GetNode( (L"w:type"), oChild ) )
				m_oType = oChild;

			if ( oNode.GetNode( (L"w:vAlign"), oChild ) )
				m_oVAlign = oChild;
		}
		void CSectionProperty::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes( oReader );

			if ( oReader.IsEmptyNode() )
				return;

			OOX::Document* document = WritingElement::m_pMainDocument;
			
			int nParentDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nParentDepth ) )
			{
				std::wstring sName = oReader.GetName();
				if ( L"w:bidi" == sName )
					m_oBidi = oReader;
				else if ( L"w:cols" == sName )
					m_oCols = oReader;
				else if ( L"w:docGrid" == sName )
					m_oDocGrid = oReader;
				else if ( L"w:endnotePr" == sName )
					m_oEndnotePr = oReader;
				else if ( !m_bSectPrChange && L"w:footerReference" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *oFooter = new ComplexTypes::Word::CHdrFtrRef(oReader);
					if (oFooter) m_arrFooterReference.push_back( oFooter );
				}
				else if ( L"w:footnotePr" == sName )
					m_oFootnotePr = oReader;
				else if ( L"w:formProt" == sName )
					m_oFormProt = oReader;
				else if ( !m_bSectPrChange && L"w:headerReference" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *oHeader = new ComplexTypes::Word::CHdrFtrRef( oReader);
					if (oHeader) m_arrHeaderReference.push_back( oHeader );
				}
				else if ( L"w:lnNumType" == sName )
					m_oLnNumType = oReader;
				else if ( L"w:noEndnote" == sName )
					m_oNoEndnote = oReader;
				else if ( (L"w:paperSrc") == sName )
					m_oPaperSrc = oReader;
				else if ( L"w:pgBorders" == sName )
					m_oPgBorders = oReader;
				else if ( L"w:pgMar" == sName )
					m_oPgMar = oReader;
				else if ( L"w:pgNumType" == sName )
					m_oPgNumType = oReader;
				else if ( L"w:pgSz" == sName )
					m_oPgSz = oReader;
				else if ( L"w:printerSettings" == sName )
					m_oPrinterSettings = oReader;
				else if ( L"w:rtlGutter" == sName )
					m_oRtlGutter = oReader;
				else if ( !m_bSectPrChange && L"w:sectPrChange" == sName )
					m_oSectPrChange = oReader;
				else if ( L"w:textDirection" == sName )
					m_oTextDirection = oReader;
				else if ( L"w:titlePg" == sName )
					m_oTitlePg = oReader;
				else if ( L"w:type" == sName )
					m_oType = oReader;
				else if ( L"w:vAlign" == sName )
					m_oVAlign = oReader;
				else if ( L"w:hdr" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *pHeaderRef = new ComplexTypes::Word::CHdrFtrRef();
					OOX::CHdrFtr* pHeader = new OOX::CHdrFtr(document);
					if (pHeaderRef && pHeader)
					{
						pHeader->fromXML(oReader);

						pHeaderRef->m_oId	= L"hdrId" + std::to_wstring(m_arrHeaderReference.size() + 1);
						pHeaderRef->m_oType = pHeader->m_oType;

						m_arrHeaderReference.push_back( pHeaderRef );
						
						CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
						if (docx_flat)
						{
							docx_flat->m_mapHeadersFooters.insert(std::make_pair(pHeaderRef->m_oId->ToString(), pHeader));
						}
					}
				}
				else if ( L"w:ftr" == sName )
				{
					ComplexTypes::Word::CHdrFtrRef *pFooterRef = new ComplexTypes::Word::CHdrFtrRef();
					OOX::CHdrFtr* pFooter = new OOX::CHdrFtr(document);
					if (pFooter && pFooterRef)
					{
						pFooter->fromXML(oReader);

						pFooterRef->m_oId	= L"ftrId" + std::to_wstring(m_arrFooterReference.size() + 1);
						pFooterRef->m_oType = pFooter->m_oType;
						
						m_arrFooterReference.push_back( pFooterRef );
						
						CDocxFlat* docx_flat = dynamic_cast<CDocxFlat*>(document);
						if (docx_flat)
						{
							docx_flat->m_mapHeadersFooters.insert(std::make_pair(pFooterRef->m_oId->ToString(), pFooter));
						}
					}
				}
			}
		}
		std::wstring CSectionProperty::toXML() const
		{
            std::wstring sResult = (L"<w:sectPr ");

			if ( m_oRsidDel.IsInit() )
			{
				sResult += (L"w:rsidDel=\"");
				sResult += m_oRsidDel->ToString();
				sResult += (L"\" ");						
			}

			if ( m_oRsidR.IsInit() )
			{
				sResult += (L"w:rsidR=\"");
				sResult += m_oRsidR->ToString();
				sResult += (L"\" ");						
			}

			if ( m_oRsidRPr.IsInit() )
			{
				sResult += (L"w:rsidRPr=\"");
				sResult += m_oRsidRPr->ToString();
				sResult += (L"\" ");						
			}

			if ( m_oRsidSect.IsInit() )
			{
				sResult += (L"w:rsidSect=\"");
				sResult += m_oRsidSect->ToString();
				sResult += (L"\" ");						
			}

			sResult += (L">");

			if ( m_oBidi.IsInit() )
			{
				sResult += (L"<w:bidi ");
				sResult += m_oBidi->ToString();
				sResult += (L"/>");
			}

			if ( m_oCols.IsInit() )
				sResult += m_oCols->toXML();

			if ( m_oDocGrid.IsInit() )
			{
				sResult += (L"<w:docGrid ");
				sResult += m_oDocGrid->ToString();
				sResult += (L"/>");
			}

			if ( m_oEndnotePr.IsInit() )
				sResult += m_oEndnotePr->toXML();

			if ( !m_bSectPrChange )
			{
				for (unsigned int nIndex = 0; nIndex < m_arrFooterReference.size(); nIndex++ )
				{
					sResult += (L"<w:footerReference ");
					if (m_arrFooterReference[nIndex])
						sResult += m_arrFooterReference[nIndex]->ToString();
					sResult += (L"/>");
				}
			}

			if ( m_oFootnotePr.IsInit() )
				sResult += m_oFootnotePr->toXML();

			if ( m_oFormProt.IsInit() )
			{
				sResult += (L"<w:formProt ");
				sResult += m_oFormProt->ToString();
				sResult += (L"/>");
			}

			if ( !m_bSectPrChange )
			{
				for (unsigned int nIndex = 0; nIndex < m_arrHeaderReference.size(); nIndex++ )
				{
					sResult += (L"<w:headerReference ");
					if (m_arrHeaderReference[nIndex])
						sResult += m_arrHeaderReference[nIndex]->ToString();
					sResult += (L"/>");
				}
			}

			if ( m_oLnNumType.IsInit() )
			{
				sResult += (L"<w:lnNumType ");
				sResult += m_oLnNumType->ToString();
				sResult += (L"/>");
			}

			if ( m_oNoEndnote.IsInit() )
			{
				sResult += (L"<w:noEndnote ");
				sResult += m_oNoEndnote->ToString();
				sResult += (L"/>");
			}

			if ( m_oPaperSrc.IsInit() )
			{
				sResult += (L"<w:paperSrc ");
				sResult += m_oPaperSrc->ToString();
				sResult += (L"/>");
			}

			if ( m_oPgBorders.IsInit() )
				sResult += m_oPgBorders->toXML();

			if ( m_oPgMar.IsInit() )
			{
				sResult += (L"<w:pgMar ");
				sResult += m_oPgMar->ToString();
				sResult += (L"/>");
			}

			if ( m_oPgNumType.IsInit() )
			{
				sResult += (L"<w:pgNumType ");
				sResult += m_oPgNumType->ToString();
				sResult += (L"/>");
			}

			if ( m_oPgSz.IsInit() )
			{
				sResult += (L"<w:pgSz ");
				sResult += m_oPgSz->ToString();
				sResult += (L"/>");
			}

			if ( m_oPrinterSettings.IsInit() )
			{
				sResult += (L"<w:printerSettings ");
				sResult += m_oPrinterSettings->ToString();
				sResult += (L"/>");
			}

			if ( m_oRtlGutter.IsInit() )
			{
				sResult += (L"<w:rtlGutter ");
				sResult += m_oRtlGutter->ToString();
				sResult += (L"/>");
			}

			if ( !m_bSectPrChange && m_oSectPrChange.IsInit() )
				sResult += m_oSectPrChange->toXML();

			if ( m_oTextDirection.IsInit() )
			{
				sResult += (L"<w:textDirection ");
				sResult += m_oTextDirection->ToString();
				sResult += (L"/>");
			}

			if ( m_oTitlePg.IsInit() )
			{
				sResult += (L"<w:titlePg ");
				sResult += m_oTitlePg->ToString();
				sResult += (L"/>");
			}

			if ( m_oType.IsInit() )
			{
				sResult += (L"<w:type ");
				sResult += m_oType->ToString();
				sResult += (L"/>");
			}

			if ( m_oVAlign.IsInit() )
			{
				sResult += (L"<w:vAlign ");
				sResult += m_oVAlign->ToString();
				sResult += (L"/>");
			}

			sResult += (L"</w:sectPr>");

			return sResult;
		}


	} // Logic
} 
