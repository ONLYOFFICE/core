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
#include "Document.h"

namespace OOX
{
	namespace Logic
	{
		void CBackground::fromXML(XmlUtils::CXmlNode& oNode)
		{
			XmlMacroReadAttributeBase(oNode, L"w:color", m_oColor);
			XmlMacroReadAttributeBase(oNode, L"w:themeColor", m_oThemeColor);
			XmlMacroReadAttributeBase(oNode, L"w:themeShade", m_oThemeShade);
			XmlMacroReadAttributeBase(oNode, L"w:themeTint", m_oThemeTint);
		}
		void CBackground::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			ReadAttributes(oReader);

			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:drawing" == sName)
				{
					m_oDrawing = new OOX::Logic::CDrawing(m_pMainDocument);
					m_oDrawing->fromXML(oReader);
				}
				else if (L"v:background" == sName)
				{
					m_oBackground = OOX::Vml::CBackground(m_pMainDocument);
					m_oBackground->fromXML(oReader);
				}
			}
		}
		std::wstring CBackground::toXML() const
		{
			std::wstring sResult = L"<w:background ";

			if (m_oColor.IsInit())
			{
				sResult += L"w:color=\"";
				sResult += m_oColor->ToString();
				sResult += L"\" ";
			}

			if (m_oThemeColor.IsInit())
			{
				sResult += L"w:themeColor=\"";
				sResult += m_oThemeColor->ToString();
				sResult += L"\" ";
			}

			if (m_oThemeShade.IsInit())
			{
				sResult += L"w:themeShade=\"";
				sResult += m_oThemeShade->ToString();
				sResult += L"\" ";
			}

			if (m_oThemeTint.IsInit())
			{
				sResult += L"w:themeTint=\"";
				sResult += m_oThemeTint->ToString();
				sResult += L"\" ";
			}

			if (m_oDrawing.IsInit())
			{
				sResult += L">";
				sResult += m_oDrawing->toXML();
				sResult += L"</w:background>";
			}
			else if (m_oBackground.IsInit())
			{//наличие атрибута Color обязательно
				sResult += m_oBackground->toXML();
				sResult += L"</w:background>";
			}
			else
				sResult += L"/>";

			return sResult;
		}
		void CBackground::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:color", m_oColor)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeColor", m_oThemeColor)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeShade", m_oThemeShade)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:themeTint", m_oThemeTint)
			WritingElement_ReadAttributes_End(oReader)
		}

		void CBgPict::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				std::wstring sName = oReader.GetName();

				if (L"w:background" == sName)
				{
					ReadAttributes(oReader);
				}
				else if (L"v:background" == sName)
					m_oBackground = oReader;
				else if (L"w:binData" == sName)
					m_oBinData = oReader;
			}
		}
		void CBgPict::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			WritingElement_ReadAttributes_Start(oReader)
				WritingElement_ReadAttributes_Read_if(oReader, L"w:bgcolor", m_oColor)
				WritingElement_ReadAttributes_Read_else_if(oReader, L"w:background", m_oBackgroundType)
				WritingElement_ReadAttributes_End(oReader)
		}
	}
//------------------------------------------------------------------------------------------------------------------------
	CDocument::CDocument(OOX::Document *pMain, OOX::FileType type)
		: File(pMain), IFileContainer(pMain), WritingElement(pMain)
	{
		m_bMacroEnabled = false;
			
		CDocx* docx = dynamic_cast<CDocx*>(pMain);
		if (docx)
		{ 
			if (type == OOX::FileTypes::Document)
			{
				docx->m_oMain.document = this;
			}
			else if (type == FileTypes::DocumentMacro)
			{
				docx->m_oMain.document = this;
				m_bMacroEnabled = true;
			}
			else if (type == OOX::FileTypes::GlossaryDocument)
			{
				docx->m_oGlossary.document = this;
			}
			else
			{
				//????
			}
		}
	}
	CDocument::CDocument(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath, OOX::FileType type)
		: File(pMain), IFileContainer(pMain), WritingElement(pMain)
	{
		m_bMacroEnabled = false;

		CDocx* docx = dynamic_cast<CDocx*>(pMain);
		if (docx)
		{
			if (type == OOX::FileTypes::Document)
			{
				docx->m_oMain.document = this;
			}
			else if (type == FileTypes::DocumentMacro)
			{
				docx->m_oMain.document = this;
				m_bMacroEnabled = true;
			}
			else if (type == OOX::FileTypes::GlossaryDocument)
			{
				docx->m_oGlossary.document = this;
				docx->m_bGlossaryRead = true;
			}
			else
			{
				//???
			}
		}

		read(oRootPath, oPath);
		
		if (docx)
		{
			docx->m_bGlossaryRead = false;
		}
	}
	CDocument::CDocument(XmlUtils::CXmlNode& oNode) : File(NULL), IFileContainer(NULL), WritingElement(NULL)
	{
		m_bMacroEnabled = false;
			
		fromXML( oNode );
	}
	CDocument::CDocument(XmlUtils::CXmlLiteReader& oReader) : File(NULL), IFileContainer(NULL), WritingElement(NULL)
	{
		m_bMacroEnabled = false;
			
		fromXML( oReader );
	}
	void CDocument::CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth)
	{
		OOX::Document* document = WritingElement::m_pMainDocument;

		while ( oReader.ReadNextSiblingNode( Depth ) )
		{
			std::wstring sName = oReader.GetName();
				
			WritingElement *pItem = NULL;

			if (L"w:altChunk" == sName )
				pItem = new Logic::CAltChunk( document );
			else if (L"w:bookmarkEnd" == sName )
				pItem = new Logic::CBookmarkEnd( document );
			else if (L"w:bookmarkStart" == sName )
				pItem = new Logic::CBookmarkStart( document );
			else if (L"w:commentRangeEnd" == sName )
				pItem = new Logic::CCommentRangeEnd( document );
			else if (L"w:commentRangeStart" == sName )
				pItem = new Logic::CCommentRangeStart( document );
			//else if (L"w:customXml" == sName )
			//	pItem = new Logic::CCustomXml( document );
			else if (L"w:customXmlDelRangeEnd" == sName )
				pItem = new Logic::CCustomXmlDelRangeEnd( document );
			else if (L"w:customXmlDelRangeStart" == sName )
				pItem = new Logic::CCustomXmlDelRangeStart( document );
			else if (L"w:customXmlInsRangeEnd" == sName )
				pItem = new Logic::CCustomXmlInsRangeEnd( document );
			else if (L"w:customXmlInsRangeStart" == sName )
				pItem = new Logic::CCustomXmlInsRangeStart( document );
			else if (L"w:customXmlMoveFromRangeEnd" == sName ) 
				pItem = new Logic::CCustomXmlMoveFromRangeEnd( document );
			else if (L"w:customXmlMoveFromRangeStart" == sName )
				pItem = new Logic::CCustomXmlMoveFromRangeStart( document );
			else if (L"w:customXmlMoveToRangeEnd" == sName ) 
				pItem = new Logic::CCustomXmlMoveToRangeEnd( document );
			else if (L"w:customXmlMoveToRangeStart" == sName )
				pItem = new Logic::CCustomXmlMoveToRangeStart( document );
			else if (L"w:del" == sName )
				pItem = new Logic::CDel( document );
			else if (L"w:ins" == sName )
				pItem = new Logic::CIns( document );
			else if (L"w:moveFrom" == sName )
				pItem = new Logic::CMoveFrom( document );
			else if (L"w:moveFromRangeEnd" == sName )
				pItem = new Logic::CMoveFromRangeEnd( document );
			else if (L"w:moveFromRangeStart" == sName )
				pItem = new Logic::CMoveFromRangeStart( document );
			else if (L"w:moveTo" == sName )
				pItem = new Logic::CMoveTo( document );
			else if (L"w:moveToRangeEnd" == sName )
				pItem = new Logic::CMoveToRangeEnd( document );
			else if (L"w:moveToRangeStart" == sName )
				pItem = new Logic::CMoveToRangeStart( document );
			else if (L"m:oMath" == sName )
				pItem = new Logic::COMath( document );
			else if (L"m:oMathPara" == sName )
				pItem = new Logic::COMathPara( document );
			else if (L"w:p" == sName )
				pItem = new Logic::CParagraph( document );
			else if (L"w:permEnd" == sName )
				pItem = new Logic::CPermEnd( document );
			else if (L"w:permStart" == sName )
				pItem = new Logic::CPermStart( document );
			else if (L"w:proofErr" == sName )
				pItem = new Logic::CProofErr( document );
			else if (L"w:sdt" == sName )
				pItem = new Logic::CSdt( document );
			else if (L"w:sectPr" == sName )
			{
				m_oSectPr = new Logic::CSectionProperty( document );
				m_oSectPr->fromXML(oReader);
			}
			else if (L"w:tbl" == sName )
				pItem = new Logic::CTbl( document );
			else if (L"wx:sect" == sName && !oReader.IsEmptyNode())
			{
				int nWxSectDepth = oReader.GetDepth();							
				CreateElements(oReader, nWxSectDepth);
			}
			else if (L"wx:sub-section" == sName && !oReader.IsEmptyNode())
			{
				int nWxSubSectDepth = oReader.GetDepth();							
				CreateElements(oReader, nWxSubSectDepth);
			}
			else if (L"wx:pBdrGroup" == sName && !oReader.IsEmptyNode())
			{
				int nWxBdrGroupDepth = oReader.GetDepth();							
				CreateElements(oReader, nWxBdrGroupDepth);
			}

			if ( pItem )
			{
				m_arrItems.push_back( pItem );
				pItem->fromXML(oReader);
			}
		}
	}

	void CDocument::read(const CPath& oRootPath, const CPath& oPath)
	{
		m_oReadPath = oPath;
		IFileContainer::Read( oRootPath, oPath );
			
		CDocx* docx = dynamic_cast<CDocx*>(File::m_pMainDocument);
		if ( (docx ) && (docx->m_pVbaProject) )
		{
			m_bMacroEnabled = true;
		}	

		XmlUtils::CXmlLiteReader oReader;
			
		if ( !oReader.FromFile( oPath.GetPath() ) )
			return;

		if ( !oReader.ReadNextNode() )
			return;

		std::wstring sName = oReader.GetName();
		if ( L"w:document" == sName || L"w:wordDocument" == sName || L"w:glossaryDocument" == sName)
		{
			fromXML(oReader);
		}			
	}
	void CDocument::write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
	{
		std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
			
		sXml += toXML();

		CDirectory::SaveToFile( oPath.GetPath(), sXml );

		oContent.Registration( type().OverrideType(), oDirectory, oPath );
		IFileContainer::Write( oPath, oDirectory, oContent );
	}

	void CDocument::ClearItems()
	{
        for ( size_t i = 0; i < m_arrItems.size(); ++i)
		{
            if ( m_arrItems[i] )delete m_arrItems[i];
		}
		m_arrItems.clear();
	}
	
	void CDocument::ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
	{
		WritingElement_ReadAttributes_Start( oReader )
		WritingElement_ReadAttributes_Read_if		( oReader, L"w:conformance",	m_oConformance )
		WritingElement_ReadAttributes_Read_else_if	( oReader, L"xml:space",		m_oSpace )
		WritingElement_ReadAttributes_End( oReader )
	}
	void CDocument::fromXML(XmlUtils::CXmlLiteReader& oReader)
	{
		ReadAttributes( oReader );

		if ( oReader.IsEmptyNode() )
			return;

		int nDocumentDepth = oReader.GetDepth();
		while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
		{
			std::wstring sName = oReader.GetName();

			if ( L"w:body" == sName && !oReader.IsEmptyNode() )
			{
				int nBodyDepth = oReader.GetDepth();
					
				CreateElements(oReader, nBodyDepth);
			}
			else if ( L"w:background" == sName )
			{
				m_oBackground = new OOX::Logic::CBackground(WritingElement::m_pMainDocument);
				m_oBackground->fromXML(oReader);
			}
			else if ( L"wx:sect" == sName && !oReader.IsEmptyNode() )
			{
				int nWxSect = oReader.GetDepth();
				CreateElements(oReader, nWxSect);
			}
			else if ( L"wx:sub-section" == sName && !oReader.IsEmptyNode() )
			{
				int nWxSubSection = oReader.GetDepth();
				CreateElements(oReader, nWxSubSection);
			}
			else if (L"w:docParts" == sName && !oReader.IsEmptyNode())
			{
				WritingElement *pItem = new OOX::Logic::CDocParts(WritingElement::m_pMainDocument);
				m_arrItems.push_back(pItem);
				pItem->fromXML(oReader);
			}
		}
	}
    std::wstring CDocument::toXML() const
	{
		std::wstring sXml = L"<w:document";

		if ( SimpleTypes::conformanceclassTransitional != m_oConformance.GetValue() )
		{		
			sXml += L" w:conformance=\"";
			sXml += m_oConformance.ToString();
			sXml += L"\"";
		}
sXml += L" xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 wp14\">";

		if ( m_oBackground.IsInit() )
			sXml += m_oBackground->toXML();

		sXml += L"<w:body>";

        for ( size_t i = 0; i < m_arrItems.size(); ++i)
		{
            if ( m_arrItems[i] )
			{
                sXml += m_arrItems[i]->toXML();
			}
		}

		if ( m_oSectPr.IsInit() )
			sXml += m_oSectPr->toXML();

		sXml += L"</w:body></w:document>";

		return sXml;
	}

} // namespace OOX

