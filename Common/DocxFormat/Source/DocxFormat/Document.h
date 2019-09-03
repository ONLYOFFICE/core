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
#ifndef OOX_DOCUMENT_FILE_INCLUDE_H_
#define OOX_DOCUMENT_FILE_INCLUDE_H_

#include "../Base/Nullable.h"

#include "../Common/SimpleTypes_Word.h"
#include "../Common/SimpleTypes_Shared.h"

#include "WritingElement.h"
#include "File.h"
#include "FileTypes.h"
#include "IFileContainer.h"

#include "Logic/SectionProperty.h"
#include "Logic/Annotations.h"
#include "Logic/Paragraph.h"
#include "Logic/Sdt.h"
#include "Logic/Table.h"
#include "Math/oMathPara.h"
#include "Math/OMath.h"

#include "External/HyperLink.h"

namespace OOX
{
	namespace Logic
	{
		//--------------------------------------------------------------------------------
		// Background 17.2.1 (Part 1)
		//--------------------------------------------------------------------------------

		// TO DO: Когда будет реализован класс CDrawing добавить его обработку тут
		class CBackground : public WritingElement
		{
		public:
			WritingElement_AdditionConstructors(CBackground)
			CBackground()
			{
			}
			virtual ~CBackground()
			{
			}
			virtual void fromXML(XmlUtils::CXmlNode& oNode)
			{
				XmlMacroReadAttributeBase( oNode, _T("w:color"),      m_oColor );
				XmlMacroReadAttributeBase( oNode, _T("w:themeColor"), m_oThemeColor );
				XmlMacroReadAttributeBase( oNode, _T("w:themeShade"), m_oThemeShade );
				XmlMacroReadAttributeBase( oNode, _T("w:themeTint"),  m_oThemeTint );
			}
			virtual void fromXML(XmlUtils::CXmlLiteReader& oReader)
			{
				ReadAttributes( oReader );

				if ( oReader.IsEmptyNode() )
					return;

				int nCurDepth = oReader.GetDepth();
				while( oReader.ReadNextSiblingNode( nCurDepth ) )
				{
					std::wstring sName = oReader.GetName();

					if ( _T("w:drawing") == sName )
						m_oDrawing = oReader;
					else if ( _T("v:background") == sName )
						m_oBackground = oReader;
				}
			}
			virtual std::wstring toXML() const
			{
				std::wstring sResult = _T("<w:background ");

				if ( m_oColor.IsInit() )
				{
					sResult += _T("w:color=\"");
					sResult += m_oColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeColor.IsInit() )
				{
					sResult += _T("w:themeColor=\"");
					sResult += m_oThemeColor->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeShade.IsInit() )
				{
					sResult += _T("w:themeShade=\"");
					sResult += m_oThemeShade->ToString();
					sResult += _T("\" ");
				}

				if ( m_oThemeTint.IsInit() )
				{
					sResult += _T("w:themeTint=\"");
					sResult += m_oThemeTint->ToString();
					sResult += _T("\" ");
				}

				if ( m_oDrawing.IsInit() )
				{
					sResult += _T(">");
					sResult += m_oDrawing->toXML();
					sResult += _T("</w:background>");
				}
				else if (m_oBackground.IsInit())
				{//наличие атрибута Color обязательно
					sResult += m_oBackground->toXML();
					sResult += _T("</w:background>");
				}
				else
					sResult += _T("/>");

				return sResult;
			}

			virtual EElementType getType () const
			{
				return et_w_background;
			}
		
		private:

			void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
			{
				// Читаем атрибуты
				WritingElement_ReadAttributes_Start( oReader )

				WritingElement_ReadAttributes_Read_if     ( oReader, _T("w:color"),      m_oColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeColor"), m_oThemeColor )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeShade"), m_oThemeShade )
				WritingElement_ReadAttributes_Read_else_if( oReader, _T("w:themeTint"),  m_oThemeTint )

				WritingElement_ReadAttributes_End( oReader )
			}

		public:

			// Attributes
			nullable<SimpleTypes::CHexColor<>        >	m_oColor;
			nullable<SimpleTypes::CThemeColor<>      >	m_oThemeColor;
			nullable<SimpleTypes::CUcharHexNumber<>  >	m_oThemeShade;
			nullable<SimpleTypes::CUcharHexNumber<>  >	m_oThemeTint;

			// Childs
			nullable<OOX::Logic::CDrawing            >	m_oDrawing;
			nullable<OOX::Vml::CBackground>				m_oBackground; 
		};
	}
}

namespace OOX
{

// TO DO: Нехватающие классы:
//        <w:customXml>
//        <w:moveFrom>
//        <w:moveTo>

	//--------------------------------------------------------------------------------
	// CDocument 17.2.3 (Part 1)
	//--------------------------------------------------------------------------------
	class CDocument : public OOX::File, public IFileContainer
	{
	public:
		CDocument(OOX::Document *pMain) : File(pMain), IFileContainer(pMain)
		{
			m_bMacroEnabled = false;
			
			CDocx* docx = dynamic_cast<CDocx*>(pMain);
			if (docx) docx->m_pDocument = this;
		}
		CDocument(OOX::Document *pMain, const CPath& oRootPath, const CPath& oPath) : File(pMain), IFileContainer(pMain)
		{
			m_bMacroEnabled = false;

			CDocx* docx = dynamic_cast<CDocx*>(pMain);
			if (docx) docx->m_pDocument = this;
			
			read( oRootPath, oPath );
		}
		virtual ~CDocument()
		{
			ClearItems();
		}

		virtual void read(const CPath& oPath)
		{
			//don't use this. use read(const CPath& oRootPath, const CPath& oFilePath)
			CPath oRootPath;
			read(oRootPath, oPath);
		}
		
		void CreateElements(XmlUtils::CXmlLiteReader &oReader, int Depth)
		{
			while ( oReader.ReadNextSiblingNode( Depth ) )
			{
				std::wstring sName = oReader.GetName();
				
				WritingElement *pItem = NULL;

				if ( _T("w:altChunk") == sName )
					pItem = new Logic::CAltChunk( oReader );
				else if ( _T("w:bookmarkEnd") == sName )
					pItem = new Logic::CBookmarkEnd( oReader );
				else if ( _T("w:bookmarkStart") == sName )
					pItem = new Logic::CBookmarkStart( oReader );
				else if ( _T("w:commentRangeEnd") == sName )
					pItem = new Logic::CCommentRangeEnd( oReader );
				else if ( _T("w:commentRangeStart") == sName )
					pItem = new Logic::CCommentRangeStart( oReader );
				//else if ( _T("w:customXml") == sName )
				//	pItem = new Logic::CCustomXml( oReader );
				else if ( _T("w:customXmlDelRangeEnd") == sName )
					pItem = new Logic::CCustomXmlDelRangeEnd( oReader );
				else if ( _T("w:customXmlDelRangeStart") == sName )
					pItem = new Logic::CCustomXmlDelRangeStart( oReader );
				else if ( _T("w:customXmlInsRangeEnd") == sName )
					pItem = new Logic::CCustomXmlInsRangeEnd( oReader );
				else if ( _T("w:customXmlInsRangeStart") == sName )
					pItem = new Logic::CCustomXmlInsRangeStart( oReader );
				else if ( _T("w:customXmlMoveFromRangeEnd") == sName ) 
					pItem = new Logic::CCustomXmlMoveFromRangeEnd( oReader );
				else if ( _T("w:customXmlMoveFromRangeStart") == sName )
					pItem = new Logic::CCustomXmlMoveFromRangeStart( oReader );
				else if ( _T("w:customXmlMoveToRangeEnd") == sName ) 
					pItem = new Logic::CCustomXmlMoveToRangeEnd( oReader );
				else if ( _T("w:customXmlMoveToRangeStart") == sName )
					pItem = new Logic::CCustomXmlMoveToRangeStart( oReader );
				else if ( _T("w:del") == sName )
					pItem = new Logic::CDel( oReader );
				else if ( _T("w:ins") == sName )
					pItem = new Logic::CIns( oReader );
				else if ( _T("w:moveFrom") == sName )
					pItem = new Logic::CMoveFrom( oReader );
				else if ( _T("w:moveFromRangeEnd") == sName )
					pItem = new Logic::CMoveFromRangeEnd( oReader );
				else if ( _T("w:moveFromRangeStart") == sName )
					pItem = new Logic::CMoveFromRangeStart( oReader );
				else if ( _T("w:moveTo") == sName )
					pItem = new Logic::CMoveTo( oReader );
				else if ( _T("w:moveToRangeEnd") == sName )
					pItem = new Logic::CMoveToRangeEnd( oReader );
				else if ( _T("w:moveToRangeStart") == sName )
					pItem = new Logic::CMoveToRangeStart( oReader );
				else if ( _T("m:oMath") == sName )
					pItem = new Logic::COMath( oReader );
				else if ( _T("m:oMathPara") == sName )
					pItem = new Logic::COMathPara( oReader );
				else if ( _T("w:p") == sName )
					pItem = new Logic::CParagraph( oReader );
				else if ( _T("w:permEnd") == sName )
					pItem = new Logic::CPermEnd( oReader );
				else if ( _T("w:permStart") == sName )
					pItem = new Logic::CPermStart( oReader );
				else if ( _T("w:proofErr") == sName )
					pItem = new Logic::CProofErr( oReader );
				else if ( _T("w:sdt") == sName )
					pItem = new Logic::CSdt( oReader );
				else if ( _T("w:sectPr") == sName )
					m_oSectPr = oReader;
				else if ( _T("w:tbl") == sName )
					pItem = new Logic::CTbl( oReader );
				else if ( _T("wx:sect") == sName && !oReader.IsEmptyNode())
				{
					int nWxSectDepth = oReader.GetDepth();							
					CreateElements(oReader, nWxSectDepth);
				}
				else if ( _T("wx:pBdrGroup") == sName && !oReader.IsEmptyNode())
				{
					int nWxBdrGroupDepth = oReader.GetDepth();							
					CreateElements(oReader, nWxBdrGroupDepth);
				}

				if ( pItem )
				{
					m_arrItems.push_back( pItem );
				}
			}
		}

		virtual void read(const CPath& oRootPath, const CPath& oPath)
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
			if ( _T("w:document") == sName || _T("w:wordDocument") == sName)
			{
				ReadAttributes( oReader );

				if ( !oReader.IsEmptyNode() )
				{
					int nDocumentDepth = oReader.GetDepth();
					while ( oReader.ReadNextSiblingNode( nDocumentDepth ) )
					{
						sName = oReader.GetName();

						if ( _T("w:body") == sName && !oReader.IsEmptyNode() )
						{
							int nBodyDepth = oReader.GetDepth();
							
							CreateElements(oReader, nBodyDepth);
						}
						else if ( _T("w:background") == sName )
							m_oBackground = oReader;
					}
				}
			}			
		}
		virtual void write(const CPath& oPath, const CPath& oDirectory, CContentTypes& oContent) const
		{
			std::wstring sXml = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document";

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

			sXml += _T("<w:body>");

            for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
                if ( m_arrItems[i] )
				{
                    sXml += m_arrItems[i]->toXML();
				}
			}

			if ( m_oSectPr.IsInit() )
				sXml += m_oSectPr->toXML();

			sXml += _T("</w:body></w:document>");

			CDirectory::SaveToFile( oPath.GetPath(), sXml );

			oContent.Registration( type().OverrideType(), oDirectory, oPath );
			IFileContainer::Write( oPath, oDirectory, oContent );
		}
		virtual const OOX::FileType type() const
		{
			if (m_bMacroEnabled)	return FileTypes::DocumentMacro;
			else					return FileTypes::Document;
		}
		virtual const CPath DefaultDirectory() const
		{
			return type().DefaultDirectory();
		}
		virtual const CPath DefaultFileName() const
		{
			return type().DefaultFileName();
		}
		const CPath& GetReadPath()
		{
			return m_oReadPath;
		}
		void ClearItems()
		{
            for ( size_t i = 0; i < m_arrItems.size(); ++i)
			{
                if ( m_arrItems[i] )delete m_arrItems[i];
			}
			m_arrItems.clear();
		}
		void AddImage(const CPath& oImagePath, const long lWidth, const long lHeight)
		{
			//// TO DO: Сделать добавление Image

			//OOX::Image* pImage = new OOX::Image( oImagePath );
			//const RId rId = add( smart_ptr<OOX::File>(pImage) );

			//WritingElement *pPara = new OOX::Logic::CParagraph();

			//// Добавить добавление картинки в параграф

			//m_arrItems.push_back( pPara );
		}
		void AddImage(const CPath& oImagePath, const long lEmuX, const std::wstring& sHRelativeFrom, const long lEmuY, const std::wstring& sVRelativeFrom, const long lWidthEmu, const long lHeightEmu)
		{
			//// TO DO: Сделать добавление Image

			//OOX::Image* pImage = new OOX::Image( oImagePath );
			//const RId rId = add( smart_ptr<OOX::File>(pImage) );

			//WritingElement *pPara = new OOX::Logic::CParagraph();

			//// Добавить добавление картинки в параграф

			//m_arrItems.push_back( pPara );
		}
		void AddImageInBegin(const CPath& oImagePath, const long lWidth, const long lHeight)
		{
			//// TO DO: Сделать добавление Image

			//ClearItems();

			//OOX::Image* pImage = new OOX::Image( oImagePath );
			//const RId rId = add( smart_ptr<OOX::File>(pImage) );

			//WritingElement *pPara = new OOX::Logic::CParagraph();

			//// Добавить добавление картинки в параграф

			//m_arrItems.push_back( pPara );
		}
		
		void AddSpaceToLast(const int nCount)
		{
			if ( (!m_arrItems.empty()) && (et_w_p == m_arrItems.back()->getType()) )
			{
				OOX::Logic::CParagraph* pPara = (OOX::Logic::CParagraph*)m_arrItems.back();
				pPara->AddSpace( nCount );
			}
		}
		void AddPageBreak()
		{
			WritingElement *pNewElement = new Logic::CParagraph();
			if ( !pNewElement )
				return;

			Logic::CParagraph* pPara = (Logic::CParagraph*)pNewElement;
			pPara->AddBreak( SimpleTypes::brtypePage );

			m_arrItems.push_back( pNewElement );
		}
		void AddText(std::wstring& sText)
		{
			WritingElement *pNewElement = new Logic::CParagraph();
			if ( !pNewElement )
				return;

			Logic::CParagraph* pPara = (Logic::CParagraph*)pNewElement;
			pPara->AddText( sText );

			m_arrItems.push_back( pNewElement );
		}
		void AddTextToLast(std::wstring& sText)
		{
			if ( (!m_arrItems.empty()) && (et_w_p == m_arrItems.back()->getType()) )
			{
				OOX::Logic::CParagraph* pPara = (OOX::Logic::CParagraph*)m_arrItems.back();
				pPara->AddText( sText );
			}
		}
		void AddHyperlink (std::wstring& sNameHref, std::wstring& sText)
		{
			WritingElement *pNewElement = new Logic::CParagraph;
			if ( !pNewElement )
				return;

			Logic::CParagraph* pPara = (Logic::CParagraph*)pNewElement;

			smart_ptr<OOX::File> oHyperlink = smart_ptr<OOX::File>( new OOX::HyperLink(File::m_pMainDocument, sNameHref ) );
			const OOX::RId rId = Add( oHyperlink );

			// TO DO: Сделать добавление гиперссылок в параграфах
			//pPara->AddHyperlink( rId, sText );

			m_arrItems.push_back( pNewElement );
		}
		void AddHyperlinkToLast(std::wstring& sNameHref, std::wstring& sText)
		{
			if ( (!m_arrItems.empty()) && (et_w_p == m_arrItems.back()->getType()) )
			{
				OOX::Logic::CParagraph* pPara = (OOX::Logic::CParagraph*)m_arrItems.back();

				smart_ptr<OOX::File> oHyperlink = smart_ptr<OOX::File>( new OOX::HyperLink( File::m_pMainDocument, sNameHref ) );
				const OOX::RId rId = Add( oHyperlink );

				// TO DO: Сделать добавление гиперссылок в параграфах
				//pPara->AddHyperlink( rId, sText );
			}		
		}

	private:
		void ReadAttributes(XmlUtils::CXmlLiteReader& oReader)
		{
			// Читаем атрибуты
			WritingElement_ReadAttributes_Start( oReader )
			WritingElement_ReadAttributes_ReadSingle( oReader, _T("w:conformance"), m_oConformance )
			WritingElement_ReadAttributes_End( oReader )
		}
	public:
		bool									m_bMacroEnabled;
		CPath									m_oReadPath;
		// Attributes
		SimpleTypes::CConformanceClass<SimpleTypes::conformanceclassTransitional> m_oConformance;

		// Childs
		nullable<OOX::Logic::CSectionProperty> m_oSectPr;
		nullable<OOX::Logic::CBackground     > m_oBackground;

        std::vector<WritingElement *>			m_arrItems;
	};

} // namespace OOX

#endif // OOX_DOCUMENT_FILE_INCLUDE_H_
