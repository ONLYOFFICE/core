#include "Document.h"
#include <string>
#include "./../../msxml/XML.h"
#include "GdiPlusEx.h"
#define NOMINMAX


namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\document.xml");

	Document::Document(const CString filepath, bool setContent, bool bGenerateContent)
		:m_filepath(filepath),
		  m_footnotes(filepath),
		  m_endnotes(filepath),
		  m_documentRels(filepath),
		  m_theme(filepath),
		  m_settings(filepath),
		  m_webSettings(filepath),
		  m_styles(filepath),
		  m_fontTable(filepath),
		  m_hasContent(setContent),
		  m_countBookmark(0),
		  m_bAddPageBreak(true),
		  m_bFirstElement(true),
		  m_bFirstTitlePar(true),
		  m_bPrevTitle(false),
		  m_bFirstTitle(true),
		  m_bGenerateContent(bGenerateContent)

	{
		m_bodyElems = m_pXmlDoc->createElement("w:body");
if(m_bGenerateContent)
{
		m_content   = m_pXmlDoc->createElement("w:sdtContent");

		std::wstring text = L"Contents";
		std::wstring style = L"TOC";
		AddParagraph(text, style, m_content);
		
		MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
		paragraph->appendChild(AddFldChar(L"begin"));			
		paragraph->appendChild(AddInstrText(" TOC \\o \"1-3\" \\h \\z \\u "));
		paragraph->appendChild(AddFldChar(L"separate"));
		m_content->appendChild(paragraph);
}
	}

	Document::~Document()
	{
	}


	bool Document::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE
			&& m_footnotes.Read()
			&& m_endnotes.Read()
			&& m_documentRels.Read()
			&& m_theme.Read()
			&& m_settings.Read()
			&& m_webSettings.Read()
			&& m_styles.Read()
			&& m_fontTable.Read())
			return true;
		return false;
	}


	bool Document::Write(const CString filepath)
	{
		MSXML2::IXMLDOMElementPtr document_node = m_pXmlDoc->selectSingleNode(L"w:document");
		MSXML2::IXMLDOMElementPtr body_node = document_node->selectSingleNode(L"w:body");
		MSXML2::IXMLDOMElementPtr sectPr_node = body_node->selectSingleNode(L"w:sectPr");

		m_pXmlDoc->removeChild(document_node);
		document_node->removeChild(body_node);
		m_bodyElems->appendChild(sectPr_node);

		MSXML2::IXMLDOMNodeListPtr elems  = m_bodyElems->GetchildNodes();
		while( MSXML2::IXMLDOMNodePtr elem = elems->nextNode() )
			CopyElements(elem, body_node, filepath);

		document_node->appendChild(body_node);
		m_pXmlDoc->appendChild(document_node);
		m_notesIdTable;

		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK
			&& m_footnotes.Write(filepath)
			&& m_endnotes.Write(filepath)
			&& m_documentRels.Write(filepath)
			&& m_styles.Write(filepath)
			&& m_theme.Write(filepath)
			&& m_settings.Write(filepath)
			&& m_webSettings.Write(filepath)
			&& m_fontTable.Write(filepath))
			return true;
		return false;
	}


	bool Document::CopyElements(const MSXML2::IXMLDOMNodePtr elementFrom, MSXML2::IXMLDOMNodePtr elementTo, const CString filepath)
	{
		std::wstring elemName = elementFrom->GetnodeName();
		if (elemName == L"image")
		{
			std::wstring imageHref = XML::getAttr(elementFrom, L"href");
			MSXML2::IXMLDOMElementPtr parWithImage = CreateImageElem(elementFrom, imageHref, true);
			if (parWithImage != NULL)
			{
				std::wstring filename = imageHref.substr(1, imageHref.length());
				CStringW srcPath = m_filepath + _T("\\word\\media\\") + filename.c_str();
				CStringW dstPath = filepath + _T("\\word\\media\\") + filename.c_str();
				BOOL res = CopyFileW(srcPath, dstPath, FALSE );
				elementTo->appendChild(parWithImage);
			}
		}
		else if (elemName == L"w:tbl" || elemName == L"w:tr" || elemName == L"w:tc")
		{
			MSXML2::IXMLDOMElementPtr nodeTo = m_pXmlDoc->createElement((_bstr_t)(elemName.c_str()));
			MSXML2::IXMLDOMNodeListPtr elems  = elementFrom->GetchildNodes();
			while(MSXML2::IXMLDOMNodePtr elem = elems->nextNode())
				CopyElements(elem, nodeTo, filepath);
			elementTo->appendChild(nodeTo);
		}	
		else if (elemName == L"content" && m_bGenerateContent )
		{
			if (m_hasContent)
			{
				MSXML2::IXMLDOMElementPtr paragraphPBr_node1 = m_pXmlDoc->createElement(L"w:p");
				paragraphPBr_node1->appendChild(AddPageBreak());
				MSXML2::IXMLDOMElementPtr paragraphPBr_node2 = m_pXmlDoc->createElement(L"w:p");
				paragraphPBr_node2->appendChild(AddPageBreak());

				MSXML2::IXMLDOMElementPtr content_node = m_pXmlDoc->createElement(L"w:sdt");
				content_node->appendChild(AddSdtPr());

				MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
				paragraph->appendChild(AddFldChar(L"end"));
				m_content->appendChild(paragraph);

				content_node->appendChild(m_content);
				elementTo->appendChild(paragraphPBr_node1);
				elementTo->appendChild(content_node);
				elementTo->appendChild(paragraphPBr_node2);
			}
		}
		else
		{
			MSXML2::IXMLDOMNodeListPtr inlineimage_nodes = elementFrom->selectNodes( L"image" );
			if(NULL != inlineimage_nodes && inlineimage_nodes->length > 0)
			{
				while( const MSXML2::IXMLDOMNodePtr inlineimage_node = inlineimage_nodes->nextNode() )
				{
					std::wstring imageHref = XML::getAttr(inlineimage_node, L"href");
					MSXML2::IXMLDOMElementPtr parWithImage = CreateImageElem(inlineimage_node, imageHref, false);
					if (parWithImage != NULL)
					{
						std::wstring filename = imageHref.substr(1, imageHref.length());
						CStringW srcPath = m_filepath + _T("\\word\\media\\") + filename.c_str();
						CStringW dstPath = filepath + _T("\\word\\media\\") + filename.c_str();
						BOOL res = CopyFileW(srcPath, dstPath, FALSE );

						MSXML2::IXMLDOMElementPtr oParentNode = inlineimage_node->parentNode;
						if(NULL != oParentNode)
							oParentNode->replaceChild(parWithImage, inlineimage_node);
					}
				}
			}
			elementTo->appendChild(elementFrom);
		}
		return true;
	}


	
	bool Document::AddElement(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentElemName)
	{
		return AddElement(element, parentElemName, m_bodyElems);
	}


	bool Document::AddElement(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentElemName, MSXML2::IXMLDOMElementPtr elems)
	{
		std::wstring elemName = element->GetnodeName();
		if (elemName == L"p" || elemName == L"text-author")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddParagraph(element, parentElemName, elems);
		}
		else if (elemName == L"empty-line")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddEmptyLine(elems);
		}
		else if (elemName == L"table")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddTable(element, elems);
		}
		else if (elemName == L"#text")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddParagraph((std::wstring)element->Gettext(), parentElemName, elems);
		}
		else if (elemName == L"poem")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddPoem(element, elems);
		}
		else if (elemName == L"image")
		{
			m_bFirstElement = false;m_bPrevTitle=false;
			AddImage(element, elems);
		}
		else if (elemName == L"section")
		{
			std::wstring id = L"";
if(m_bGenerateContent)
			id = XML::getAttr(element, L"id");
			if (id != L"")
			{
				AddMarkedParagraph(element, id);
			}				
			else
			{
				MSXML2::IXMLDOMNodeListPtr ins_elems = element->GetchildNodes();
				while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
					AddElement(child, elemName, elems);
			}				
		}
		else if( elemName == L"title" )
		{
			if( false == m_bFirstElement && true == m_bAddPageBreak && false == m_bPrevTitle )
				AddPageBreak( elems );
			MSXML2::IXMLDOMNodeListPtr ins_elems = element->GetchildNodes();
			m_bFirstTitlePar = true;
			while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
				AddElement(child, L"title", elems);
			m_bFirstTitle = false;
			m_bPrevTitle = true;
		}
		else if( elemName == L"subtitle" )
		{
			MSXML2::IXMLDOMNodeListPtr ins_elems = element->GetchildNodes();
			m_bFirstTitlePar = true;
			while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
				AddElement(child, L"subtitle", elems);
		}
		else
		{
			MSXML2::IXMLDOMNodeListPtr ins_elems = element->GetchildNodes();
			while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
				AddElement(child, elemName, elems);
		}

		return true;
	}


	bool Document::AddContent()
	{
		MSXML2::IXMLDOMElementPtr content = m_pXmlDoc->createElement(L"content");
		m_bodyElems->appendChild(content);
		return true;
	}
	

	bool Document::AddParagraph(const MSXML2::IXMLDOMNodePtr paragraphFb2, const std::wstring& parentName)
	{				
		return AddParagraph(paragraphFb2, parentName, m_bodyElems);
	}


	bool Document::AddParagraph(const MSXML2::IXMLDOMNodePtr paragraphFb2, const std::wstring& parentName, MSXML2::IXMLDOMElementPtr elems)
	{
		std::wstring styleName = getStyleName(parentName);
		if (styleName != L"")
		{
			MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
			MSXML2::IXMLDOMElementPtr paragraphPr = m_pXmlDoc->createElement(L"w:pPr");
			MSXML2::IXMLDOMElementPtr pStyle = m_pXmlDoc->createElement(L"w:pStyle");
			pStyle->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
			paragraphPr->appendChild(pStyle);
			paragraph->appendChild(paragraphPr);

			MSXML2::IXMLDOMElementPtr run_elem = NULL;
			paragraphFb2->QueryInterface( __uuidof(MSXML2::IXMLDOMElement), (void**) & run_elem );
			run_elem->setAttribute( L"xml:space", (const _variant_t &)L"preserve" );

			if (parentName == L"title" || parentName == L"subtitle")
			{
if(m_bGenerateContent)
{
				std::string nameMarkName;
				if( true == m_bFirstTitlePar )
				{
					m_bFirstTitlePar = false;
					if( parentName == L"title" && true == m_bFirstTitle )
						nameMarkName = setMarkToContent(paragraphFb2, L"title");
					else
						nameMarkName = setMarkToContent(paragraphFb2, L"subtitle");
				MSXML2::IXMLDOMElementPtr bookmarkStart = m_pXmlDoc->createElement(L"w:bookmarkStart");
				bookmarkStart->setAttribute(L"w:id", (const _variant_t &)m_countBookmark);
				bookmarkStart->setAttribute(L"w:name", (const _variant_t &)nameMarkName.c_str());
				MSXML2::IXMLDOMElementPtr bookmarkEnd = m_pXmlDoc->createElement(L"w:bookmarkEnd");
				bookmarkEnd->setAttribute(L"w:id", (const _variant_t &)m_countBookmark);
				paragraph->appendChild(bookmarkStart);
				paragraph->appendChild(bookmarkEnd);
				m_countBookmark++;
				}
}
			}

			MSXML2::IXMLDOMNodeListPtr runs = paragraphFb2->GetchildNodes();
			while( MSXML2::IXMLDOMNodePtr run_node = runs->nextNode() )
			{
				MSXML2::IXMLDOMElementPtr run = AddRun(run_node);
				if (run != NULL)
					paragraph->appendChild(run);
			}	
			elems->appendChild( paragraph );
		}
		
		return true;
	}


	bool Document::AddParagraph(const std::wstring paragraphFb2, const std::wstring& parentName)
	{
		return AddParagraph(paragraphFb2, parentName, m_bodyElems);
	}



	bool Document::AddParagraph(const std::wstring paragraphFb2, const std::wstring& parentName, MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"paragraph");
		MSXML2::IXMLDOMTextPtr text_node = m_pXmlDoc->createTextNode(static_cast<std::wstring>(paragraphFb2).c_str());
		paragraph->appendChild(text_node);
		/*std::wstring styleName = getStyleName(parentName);
		if (styleName != L"")
		{
			MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
			MSXML2::IXMLDOMElementPtr paragraphPr = m_pXmlDoc->createElement(L"w:pPr");
			MSXML2::IXMLDOMElementPtr pStyle = m_pXmlDoc->createElement(L"w:pStyle");
			pStyle->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
			paragraphPr->appendChild(pStyle);
			paragraph->appendChild(paragraphPr);
			if (parentName == L"title" || parentName == L"subtitle")
			{
				std::string nameMarkName = setMarkToContent(paragraphFb2, parentName);
				MSXML2::IXMLDOMElementPtr bookmarkStart = m_pXmlDoc->createElement(L"w:bookmarkStart");
				bookmarkStart->setAttribute(L"w:id", (const _variant_t &)m_countBookmark);
				bookmarkStart->setAttribute(L"w:name", (const _variant_t &)nameMarkName.c_str());
				MSXML2::IXMLDOMElementPtr bookmarkEnd = m_pXmlDoc->createElement(L"w:bookmarkEnd");
				bookmarkEnd->setAttribute(L"w:id", (const _variant_t &)m_countBookmark);
				paragraph->appendChild(bookmarkStart);
				paragraph->appendChild(bookmarkEnd);
				m_countBookmark++;
			}
			paragraph->appendChild(AddRun(paragraphFb2));
			elems->appendChild( paragraph );        
		}*/
		AddParagraph(paragraph, parentName, elems);

		return true;
	}


	bool Document::AddTable(const MSXML2::IXMLDOMNodePtr table)
	{
		return AddTable(table, m_bodyElems);
	}


	bool Document::AddTable(const MSXML2::IXMLDOMNodePtr table, MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMElementPtr table_node = m_pXmlDoc->createElement(L"w:tbl");
		MSXML2::IXMLDOMElementPtr tablePr_node = m_pXmlDoc->createElement(L"w:tblPr");
		MSXML2::IXMLDOMElementPtr tableStyle_node = m_pXmlDoc->createElement(L"w:tblStyle");
		tableStyle_node->setAttribute(L"w:val", (const _variant_t &)"TableGrid");
		MSXML2::IXMLDOMElementPtr tableWidth_node = m_pXmlDoc->createElement(L"w:tblWidth");
		tableWidth_node->setAttribute(L"w:w", (const _variant_t &)"0");
		tableWidth_node->setAttribute(L"w:type", (const _variant_t &)"auto");
		MSXML2::IXMLDOMElementPtr tableLook_node = m_pXmlDoc->createElement(L"w:tblLook");
		tableLook_node->setAttribute(L"w:val", (const _variant_t &)"04A0");
		MSXML2::IXMLDOMElementPtr tableGrid_node = m_pXmlDoc->createElement(L"w:tblGrid");

		tablePr_node->appendChild(tableStyle_node);
		tablePr_node->appendChild(tableWidth_node);
		tablePr_node->appendChild(tableLook_node);
		table_node->appendChild(tablePr_node);
		table_node->appendChild(tableGrid_node);

		MSXML2::IXMLDOMNodeListPtr rows = table->GetchildNodes();
		while(MSXML2::IXMLDOMNodePtr row = rows->nextNode())
		{
			MSXML2::IXMLDOMElementPtr row_node = m_pXmlDoc->createElement(L"w:tr");
			MSXML2::IXMLDOMNodeListPtr cells = row->GetchildNodes();
			while(MSXML2::IXMLDOMNodePtr cell = cells->nextNode())
			{
				MSXML2::IXMLDOMElementPtr cell_node = m_pXmlDoc->createElement(L"w:tc");
				MSXML2::IXMLDOMElementPtr cellPr_node = m_pXmlDoc->createElement(L"w:tcPr");
				MSXML2::IXMLDOMElementPtr cellWidth_node = m_pXmlDoc->createElement(L"w:tcW");
				cellWidth_node->setAttribute(L"w:w", (const _variant_t &)"0");
				cellWidth_node->setAttribute(L"w:type", (const _variant_t &)"auto");
				cellPr_node->appendChild(cellWidth_node);
				cell_node->appendChild(cellPr_node);
				MSXML2::IXMLDOMNodeListPtr content = cell->GetchildNodes();
				while(MSXML2::IXMLDOMNodePtr item = content->nextNode())
				{
					AddElement(item, L"body", cell_node);
				}
				row_node->appendChild(cell_node);
			}
			table_node->appendChild(row_node);
		}
		elems->appendChild(table_node);

		return true;
	}

	
	bool Document::AddNote(const MSXML2::IXMLDOMNodePtr note, const std::wstring& href)
	{
		int id = GetId(L"#" + href);
		return m_endnotes.AddNote(note, id);
		//return m_footnotes.AddNote(note, id);
	}


	bool Document::AddMarkedParagraph(const MSXML2::IXMLDOMNodePtr element, const std::wstring& id)
	{
		MSXML2::IXMLDOMElementPtr block = m_pXmlDoc->createElement(L"block");
		MSXML2::IXMLDOMNodeListPtr content = element->GetchildNodes();
		while(MSXML2::IXMLDOMNodePtr item = content->nextNode())
		{
			AddElement(item, L"section", block);
		}

		MSXML2::IXMLDOMElementPtr bookmarkStart_node = m_pXmlDoc->createElement(L"w:bookmarkStart");
		bookmarkStart_node->setAttribute(L"w:id", (const _variant_t &)id.c_str());
		bookmarkStart_node->setAttribute(L"w:name", (const _variant_t &)id.c_str());
		MSXML2::IXMLDOMElementPtr bookmarkEnd_node = m_pXmlDoc->createElement(L"w:bookmarkEnd");
		bookmarkEnd_node->setAttribute(L"w:id", (const _variant_t &)id.c_str());
		block->selectSingleNode(L"w:p")->appendChild(bookmarkStart_node);
		block->selectSingleNode(L"w:p")->appendChild(bookmarkEnd_node);

		MSXML2::IXMLDOMNodeListPtr block_content = block->GetchildNodes();
		while(MSXML2::IXMLDOMNodePtr item = block_content->nextNode())
		{
			m_bodyElems->appendChild(item);
		}

		return true;
	}


	bool Document::AddPoem(const MSXML2::IXMLDOMNodePtr poem)
	{
		return AddPoem(poem, m_bodyElems);
	}


	bool Document::AddPoem(const MSXML2::IXMLDOMNodePtr poem, MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMNodeListPtr childs = poem->GetchildNodes();
		while( MSXML2::IXMLDOMNodePtr child_node = childs->nextNode() )
		{
			std::wstring childName = child_node->GetnodeName();
			if (childName == L"stanza")
			{
				AddStranza(child_node, elems);
				AddEmptyLine(elems);
			}
			else
			{
				AddParagraph(child_node, childName, elems);
			}
		}	

		return true;
	}	


	bool Document::AddStranza(const MSXML2::IXMLDOMNodePtr Fb2Stranza)
	{
		return AddStranza(Fb2Stranza, m_bodyElems);
	}


	bool Document::AddStranza(const MSXML2::IXMLDOMNodePtr Fb2Stranza, MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMNodeListPtr childs = Fb2Stranza->GetchildNodes();
		while( MSXML2::IXMLDOMNodePtr child_node = childs->nextNode() )
		{
			std::wstring childName = child_node->GetnodeName();
			if (childName == L"stanza")
			{
				AddStranza(child_node, elems);
				AddEmptyLine(elems);
			}
			else
			{
				AddParagraph(child_node, childName, elems);
			}
		}
		return true;
	}	


	bool Document::AddEmptyLine()
	{
		return AddEmptyLine(m_bodyElems);
	}


	bool Document::AddEmptyLine(MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
		MSXML2::IXMLDOMElementPtr paragraphPr = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr spacing = m_pXmlDoc->createElement(L"w:spacing");
		spacing->setAttribute(L"w:after", (const _variant_t &)"0");
		paragraphPr->appendChild(spacing);
		paragraph->appendChild(paragraphPr);
		elems->appendChild(paragraph); 
		return true;
	}	


	bool Document::AddImage(const MSXML2::IXMLDOMNodePtr image)
	{
		return AddImage(image, m_bodyElems);
	}



	bool Document::AddImage(const MSXML2::IXMLDOMNodePtr image, MSXML2::IXMLDOMElementPtr elems)
	{
		elems->appendChild(image);		
		return true;
	}
	

	bool Document::AddImage(const MSXML2::IXMLDOMNodePtr binary, const std::wstring& type, const std::wstring& id)
	{
		std::wstring data = binary->Gettext();
		BYTE* bytes;
		int size;
		GetFileInBinary(data, bytes, size);

		CString path = m_filepath + L"\\word\\media\\" + id.c_str();

		if (SaveFile(bytes, size, path))
		{
			m_documentRels.AddImage(id);
			return true;
		}		
		return false;
	}


	bool Document::AddPageBreak(MSXML2::IXMLDOMElementPtr elems)
	{
		MSXML2::IXMLDOMElementPtr PageBreak = m_pXmlDoc->createElement(L"w:p");
		PageBreak->appendChild(AddPageBreak());
		elems->appendChild(PageBreak);
		return true;
	}
	bool Document::AddAnnotation( const MSXML2::IXMLDOMNodePtr annotation  )
	{
		std::wstring sAnnotationText = L"Annotation";
		AddParagraph(sAnnotationText, L"title", m_bodyElems);

		MSXML2::IXMLDOMNodeListPtr ins_elems = annotation->GetchildNodes();
		const std::wstring parentName = annotation->GetnodeName();
		while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
		{
			std::wstring childElemName = child->GetnodeName();
			if( L"p" == childElemName )
				AddParagraph(child, parentName);
		}
		return true;
	}
	const MSXML2::IXMLDOMElementPtr Document::CreateImageElem(MSXML2::IXMLDOMElementPtr image, const std::wstring& href, bool bParagraph)
	{
		bool bIsCover = false;
		MSXML2::IXMLDOMNamedNodeMapPtr piAttrMap;
		if( SUCCEEDED( image->get_attributes( &piAttrMap ) ) )
		{
			MSXML2::IXMLDOMNodePtr piNode = piAttrMap->getNamedItem( L"AVSCover" );
			if(  NULL != piNode )
			{
				CString sCoverAttr = piNode->GetnodeValue().bstrVal;
				if( _T("true") == sCoverAttr )
					bIsCover = true;
			}
		}
		MSXML2::IXMLDOMElementPtr parWithImage;
		std::wstring rId = m_documentRels.GetRId(href.substr(1, href.length()));
		if (rId != L"")
		{			
			int nWidth = 0;
			int nHeight = 0;
			GdiplusStartupInput  oInput;
			GdiplusStartupOutput oOutput;
			ULONG dwToken = 0;
			Status eStatus = GdiplusStartup( &dwToken, &oInput, &oOutput );
			if ( eStatus == Ok )
			{
				std::wstring filename = href.substr(1, href.length());
				Bitmap oImage(m_filepath + _T("\\word\\media\\") + filename.c_str());						
				nWidth = oImage.GetWidth();
				nHeight = oImage.GetHeight();
			}
			GdiplusShutdown( dwToken );
			if (nWidth != 0 && nHeight != 0)
			{
				int new_width = 0;
				int new_height = 0;
				int dpi = 96;
				if( true ==  bIsCover)
				{
					//с учетом, что в DocxTemplate.docx
					//<w:pgSz w:w="11906" w:h="16838"/>
					//<w:pgMar w:top="0" w:right="0" w:bottom="0" w:left="0" w:header="0" w:footer="0" w:gutter="0"/>
					//реальная ширина страницы с учетом margin страницы в twips: width - 11906, height - 16838
					//в пунктах (twips = 20*pt) : width = ( 9355 / 20 ) = 595.3, height - 841.9
					//в пикселях если считать dpi 96: width = ( 593,3 * 96 / 72 ) = 791, height - 1123
					Resize(791, 1123, new_width, new_height, dpi);
				}
				else
					Resize(nWidth, nHeight, new_width, new_height, dpi);
				if(true == bParagraph)
				{
					parWithImage = m_pXmlDoc->createElement(L"w:p");
					MSXML2::IXMLDOMElementPtr pPr_node = m_pXmlDoc->createElement(L"w:pPr");
					MSXML2::IXMLDOMElementPtr jc_node = m_pXmlDoc->createElement(L"w:jc");
					jc_node->setAttribute(L"w:val", (const _variant_t &)L"center");
					pPr_node->appendChild(jc_node);
					if( true ==  bIsCover)
					{
						//долбавляем конец секции
						MSXML2::IXMLDOMDocumentPtr m_pXmlDoc( L"Msxml2.DOMDocument");
						BSTR bstrSectXml = L"<w:sectPr xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\"><w:pgSz w:w=\"11906\" w:h=\"16838\"/><w:pgMar w:top=\"0\" w:right=\"0\" w:bottom=\"0\" w:left=\"0\" w:header=\"0\" w:footer=\"0\" w:gutter=\"0\"/><w:cols w:space=\"708\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr>";
						if( VARIANT_TRUE == m_pXmlDoc->loadXML( bstrSectXml ) )
						{
							MSXML2::IXMLDOMNodePtr piSectNode =  m_pXmlDoc->selectSingleNode( L"w:sectPr" );
							if( NULL != piSectNode )
								pPr_node->appendChild( piSectNode );
						}

					}
					parWithImage->appendChild(pPr_node);
				}
				MSXML2::IXMLDOMElementPtr run_node = m_pXmlDoc->createElement(L"w:r");
				MSXML2::IXMLDOMElementPtr drawing_node = m_pXmlDoc->createElement(L"w:drawing");
				MSXML2::IXMLDOMElementPtr inline_node = m_pXmlDoc->createElement(L"wp:inline");
				inline_node->setAttribute(L"distT", (const _variant_t &)L"0");
				inline_node->setAttribute(L"distB", (const _variant_t &)L"0");
				inline_node->setAttribute(L"distL", (const _variant_t &)L"0");
				inline_node->setAttribute(L"distR", (const _variant_t &)L"0");
				MSXML2::IXMLDOMElementPtr extent_node = m_pXmlDoc->createElement(L"wp:extent");
				extent_node->setAttribute(L"cx", (const _variant_t &)new_width);
				extent_node->setAttribute(L"cy", (const _variant_t &)new_height);
				MSXML2::IXMLDOMElementPtr docPr_node = m_pXmlDoc->createElement(L"wp:docPr");
				docPr_node->setAttribute(L"id", (const _variant_t &)L"0");
				docPr_node->setAttribute(L"name", (const _variant_t &)L"");
				MSXML2::IXMLDOMElementPtr framePr_node = m_pXmlDoc->createElement(L"wp:cNvGraphicFramePr");
				MSXML2::IXMLDOMElementPtr frameLocks_node = m_pXmlDoc->createElement(L"a:graphicFrameLocks");
				frameLocks_node->setAttribute(L"noChangeAspect", (const _variant_t &)L"1");
				MSXML2::IXMLDOMElementPtr graphic_node = m_pXmlDoc->createElement(L"a:graphic");
				MSXML2::IXMLDOMElementPtr graphicData_node = m_pXmlDoc->createElement(L"a:graphicData");
				graphicData_node->setAttribute(L"uri", (const _variant_t &)L"http://schemas.openxmlformats.org/drawingml/2006/picture");
				MSXML2::IXMLDOMElementPtr pic_node = m_pXmlDoc->createElement(L"pic:pic");
				MSXML2::IXMLDOMElementPtr nvPicPr_node = m_pXmlDoc->createElement(L"pic:nvPicPr");
				MSXML2::IXMLDOMElementPtr cNvPr_node = m_pXmlDoc->createElement(L"pic:cNvPr");
				cNvPr_node->setAttribute(L"id", (const _variant_t &)L"0");
				cNvPr_node->setAttribute(L"name", (const _variant_t &)L"");
				MSXML2::IXMLDOMElementPtr cNvPicPr_node = m_pXmlDoc->createElement(L"pic:cNvPicPr");
				MSXML2::IXMLDOMElementPtr blipFill_node = m_pXmlDoc->createElement(L"pic:blipFill");
				MSXML2::IXMLDOMElementPtr blip_node = m_pXmlDoc->createElement(L"a:blip");
				blip_node->setAttribute(L"r:embed", (const _variant_t &)rId.c_str());
				MSXML2::IXMLDOMElementPtr stretch_node = m_pXmlDoc->createElement(L"a:stretch");
				MSXML2::IXMLDOMElementPtr fillRect_node = m_pXmlDoc->createElement(L"a:fillRect");
				MSXML2::IXMLDOMElementPtr spPr_node = m_pXmlDoc->createElement(L"pic:spPr");
				spPr_node->setAttribute(L"bwMode", (const _variant_t &)L"auto");
				MSXML2::IXMLDOMElementPtr xfrm_node = m_pXmlDoc->createElement(L"a:xfrm");
				MSXML2::IXMLDOMElementPtr off_node = m_pXmlDoc->createElement(L"a:off");
				off_node->setAttribute(L"x", (const _variant_t &)L"0");
				off_node->setAttribute(L"y", (const _variant_t &)L"0");
				MSXML2::IXMLDOMElementPtr ext_node = m_pXmlDoc->createElement(L"a:ext");
				ext_node->setAttribute(L"cx", (const _variant_t &)new_width);
				ext_node->setAttribute(L"cy", (const _variant_t &)new_height);
				MSXML2::IXMLDOMElementPtr prstGeom_node = m_pXmlDoc->createElement(L"a:prstGeom");				
				prstGeom_node->setAttribute(L"prst", (const _variant_t &)L"rect");
				MSXML2::IXMLDOMElementPtr avLst_node = m_pXmlDoc->createElement(L"a:avLst");

				nvPicPr_node->appendChild(cNvPr_node);
				nvPicPr_node->appendChild(cNvPicPr_node);
				pic_node->appendChild(nvPicPr_node);
				blipFill_node->appendChild(blip_node);
				stretch_node->appendChild(fillRect_node);
				blipFill_node->appendChild(stretch_node);
				pic_node->appendChild(blipFill_node);
				xfrm_node->appendChild(off_node);
				xfrm_node->appendChild(ext_node);
				spPr_node->appendChild(xfrm_node);
				prstGeom_node->appendChild(avLst_node);
				spPr_node->appendChild(prstGeom_node);
				pic_node->appendChild(spPr_node);
				graphicData_node->appendChild(pic_node);
				graphic_node->appendChild(graphicData_node);				
				inline_node->appendChild(extent_node);
				inline_node->appendChild(docPr_node);
				framePr_node->appendChild(frameLocks_node);
				inline_node->appendChild(framePr_node);
				inline_node->appendChild(graphic_node);
				drawing_node->appendChild(inline_node);
				run_node->appendChild(drawing_node);

				if(true == bParagraph)
					parWithImage->appendChild(run_node);
				else
					parWithImage = run_node;
				//std::wstring part1 = L"<w:p w:rsidR=\"00C13915\" w:rsidRPr=\"001B1E1D\" w:rsidRDefault=\"003F58C8\" w:rsidP=\"003F58C8\"></w:pPr><w:r><w:drawing><wp:inline distT="0" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"";
					//2219325
				//std::wstring part2 = L"\" cy=\""
					//1809750
				//std::wstring part3 = "\"/><wp:effectExtent l=\"19050\" t=\"0\" r=\"9525\" b=\"0\"/><wp:docPr id=\"0\" name=\"\" descr=\"" + imageHref.substr(1, imageHref.length()) + \"/><wp:cNvGraphicFramePr><a:graphicFrameLocks xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main" noChangeAspect="1"/></wp:cNvGraphicFramePr><a:graphic xmlns:a="http://schemas.openxmlformats.org/drawingml/2006/main"><a:graphicData uri="http://schemas.openxmlformats.org/drawingml/2006/picture"><pic:pic xmlns:pic="http://schemas.openxmlformats.org/drawingml/2006/picture"><pic:nvPicPr><pic:cNvPr id="0" name="error26.bmp"/><pic:cNvPicPr/></pic:nvPicPr><pic:blipFill><a:blip r:embed="rId6"/><a:stretch><a:fillRect/></a:stretch></pic:blipFill><pic:spPr><a:xfrm><a:off x="0" y="0"/><a:ext cx="2219325" cy="1809750"/></a:xfrm><a:prstGeom prst="rect"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r><w:r w:rsidR="001B1E1D"><w:rPr><w:lang w:val="en-US"/></w:rPr><w:t>ndfhfghdfg</w:t></w:r></w:p>
			}
		}
		return parWithImage;
	}


	void Document::Resize(int old_width, int old_height, int& new_width, int& new_height, int dpi)
	{
		double heightPage = 29.7;//in cm
		double widthPage  = 21;//in cm	
		double w = old_width / (widthPage / 2.54 * dpi);
		double h = old_height / (heightPage / 2.54 * dpi);
		double maximum = (w > h) ? w : h;//std::max(old_width / (widthPage / 2.54 * dpi), old_height / (heightPage / 2.54 * dpi));
		if (maximum < 1)
		{				
			new_width = (int)(old_width * 2.54 * 72 * 100* 1000 / 20 / 96);//from px to sx 
			new_height = (int)(old_height * 2.54 * 72 * 100* 1000 / 20 / 96);//from px to sx 
		}
		else
		{				
			new_width  = (int)(old_width  / maximum * 2.54 * 72 * 100* 1000 / 20 / 96);//from px to sx
			new_height = (int)(old_height / maximum * 2.54 * 72 * 100* 1000 / 20 / 96);//from px to sx
		}
	}



	void Document::GetFileInBinary(std::wstring& valueInBase64, BYTE*& data, int& size)
	{
		CString string(valueInBase64.c_str());
		int nStrLen = string.GetLength();

		if (/*data || */!nStrLen)
			return;

		size = Base64DecodeGetRequiredLength(nStrLen);

		data = new BYTE[size + 2];

		if (!data)
			return;

		CStringA cstr;
		cstr = string;
		char* strData = (char*)(LPCSTR)(cstr);

		BOOL success = Base64Decode(strData, nStrLen, data, &size);

		if (!success)
			delete[] data;
		else
		{
			data[size + 0] = 0;
			data[size + 1] = 0;
		}
		return;
	}


	bool Document::SaveFile(const BYTE* bytes, const int size,  CString path)
	{
		CStringA strA;
		strA = path;

		FILE* file = NULL;
	
		file = fopen(strA, "wb");

		if(file)
		{
			fwrite(bytes, sizeof(BYTE), size, file);
			fclose(file);
		}
		return true;
	}


	const std::string Document::setMarkToContent(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentName)
	{		
		char buf[10];
		std::string markName = "heading" + std::string(::itoa(m_countBookmark, buf, 10));
		std::wstring pStyleName;
		if (parentName == L"title")
			pStyleName = L"TOC1";
		else
			pStyleName = L"TOC2";

		MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
		MSXML2::IXMLDOMElementPtr paragraphPr = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr pStyle = m_pXmlDoc->createElement(L"w:pStyle");
		pStyle->setAttribute(L"w:val", (const _variant_t &)pStyleName.c_str());
		paragraphPr->appendChild(pStyle);
		MSXML2::IXMLDOMElementPtr tabs = m_pXmlDoc->createElement(L"w:tabs");
		MSXML2::IXMLDOMElementPtr tab1 = m_pXmlDoc->createElement(L"w:tab");
		tab1->setAttribute(L"w:val", (const _variant_t &)L"right");
		tab1->setAttribute(L"w:leader", (const _variant_t &)L"dot");
		tab1->setAttribute(L"w:pos", (const _variant_t &)L"9345");
		tabs->appendChild(tab1);
		paragraphPr->appendChild(tabs);
		paragraph->appendChild(paragraphPr);
		
		MSXML2::IXMLDOMElementPtr hyperlink = m_pXmlDoc->createElement(L"w:hyperlink");
		hyperlink->setAttribute( L"w:anchor", (const _variant_t &)markName.c_str() );

		MSXML2::IXMLDOMElementPtr run_elem = NULL;
		element->QueryInterface( __uuidof(MSXML2::IXMLDOMElement), (void**) & run_elem );
		run_elem->setAttribute( L"xml:space", (const _variant_t &)L"preserve" );
		
		MSXML2::IXMLDOMNodeListPtr runs = element->GetchildNodes();
		while( MSXML2::IXMLDOMNodePtr run_node = runs->nextNode() )
		{
			MSXML2::IXMLDOMElementPtr run = AddRun(run_node);
			if (run != NULL)
				hyperlink->appendChild(run);
		}

		hyperlink->appendChild(AddTab());
		hyperlink->appendChild(AddFldChar(L"begin"));
		hyperlink->appendChild(AddInstrText(" PAGEREF " + markName + " \\h "));
		hyperlink->appendChild(AddFldChar(L"separate"));
		hyperlink->appendChild(AddFldChar(L"end"));
		paragraph->appendChild(hyperlink);

		m_content->appendChild(paragraph);

		return markName;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddRun(const MSXML2::IXMLDOMNodePtr Fb2run)
	{
		std::wstring runName = Fb2run->GetnodeName();
		MSXML2::IXMLDOMElementPtr run;

		if (runName == L"#text")
		{			
			run = m_pXmlDoc->createElement(L"w:r");
			run->appendChild(AddText((std::wstring)Fb2run->Gettext()));				
		}
		else if(runName == L"strong" || runName == L"sup" || runName == L"sub")
		{
			run = m_pXmlDoc->createElement(L"w:r");
			MSXML2::IXMLDOMElementPtr rPr_node = m_pXmlDoc->createElement(L"w:rPr");
			if(runName == L"strong")
			{
				MSXML2::IXMLDOMElementPtr rstyle_node = m_pXmlDoc->createElement(L"w:b");
				rPr_node->appendChild(rstyle_node);
			}
			else if(runName == L"sup")
			{
				MSXML2::IXMLDOMElementPtr rstyle_node = m_pXmlDoc->createElement(L"w:vertAlign");
				rstyle_node->setAttribute(L"w:val", (const _variant_t &)L"superscript");
				rPr_node->appendChild(rstyle_node);
			}
			else if(runName == L"sub")
			{
				MSXML2::IXMLDOMElementPtr rstyle_node = m_pXmlDoc->createElement(L"w:vertAlign");
				rstyle_node->setAttribute(L"w:val", (const _variant_t &)L"subscript");
				rPr_node->appendChild(rstyle_node);
			}
			run->appendChild(rPr_node);
			run->appendChild(AddText((std::wstring)Fb2run->Gettext()));				
		}
		else if (runName == L"image")
		{
			run = Fb2run;
		}
		else if (runName != L"")
		{
			std::wstring runStyleName = getStyleName(runName);
			if (runStyleName != L"")
			{
				MSXML2::IXMLDOMElementPtr rPr = m_pXmlDoc->createElement(L"w:rPr");
				MSXML2::IXMLDOMElementPtr rStyle = m_pXmlDoc->createElement(L"w:rStyle");
				rStyle->setAttribute(L"w:val", (const _variant_t &)runStyleName.c_str());
				rPr->appendChild(rStyle);
				if (runName == L"a")
				{
					std::wstring type = XML::getAttr(Fb2run, L"type");
					std::wstring href = XML::getAttr(Fb2run, L"href");
					/*if (type == L"note" && href != L"")
					{						
						run = m_pXmlDoc->createElement(L"w:r");
						MSXML2::IXMLDOMElementPtr reference = m_pXmlDoc->createElement(L"w:endnoteReference");
						//MSXML2::IXMLDOMElementPtr reference = m_pXmlDoc->createElement(L"w:footnoteReference");
						reference->setAttribute(L"w:customMarkFollows", (const _variant_t &)L"1");
						int id = GetId(href);
						reference->setAttribute(L"w:id", (const _variant_t &)id);
						
						run->appendChild(rPr);
						run->appendChild(reference);
						run->appendChild(AddText((std::wstring)Fb2run->Gettext()));

					}	
					else if (type == L"" && href.find(L"#") == std::wstring::npos)
					{*/						 
						run = m_pXmlDoc->createElement(L"w:hyperlink");
						if (href.find(L"#") == std::wstring::npos)
						{
							std::string rId = m_documentRels.AddHyperlink(href);
							run->setAttribute(L"r:id", (const _variant_t &)rId.c_str());
						}
						else
						{
							std::wstring bookmarkId = href.substr(1, href.length() - 1);
							run->setAttribute(L"w:anchor", (const _variant_t &)bookmarkId.c_str());
						}

						run->setAttribute(L"w:history", (const _variant_t &)"1");
						MSXML2::IXMLDOMElementPtr run_node = m_pXmlDoc->createElement(L"w:r");
						MSXML2::IXMLDOMElementPtr rPr_node = m_pXmlDoc->createElement(L"w:rPr");
						MSXML2::IXMLDOMElementPtr rstyle_node = m_pXmlDoc->createElement(L"w:rStyle");
						rstyle_node->setAttribute(L"w:val", (const _variant_t &)L"Hyperlink");
						rPr_node->appendChild(rstyle_node);
						run_node->appendChild(rPr_node);
						run_node->appendChild(AddText((std::wstring)Fb2run->Gettext()));
						run->appendChild(run_node);
					/*}
					else
					{
						run = m_pXmlDoc->createElement(L"w:r");
						//run->appendChild(rPr);
						run->appendChild(AddText((std::wstring)Fb2run->Gettext()));
					}*/
				}
				else if (runName == L"title" || runName == L"subtitle")
				{
				}
				else
				{
					run = m_pXmlDoc->createElement(L"w:r");
					run->appendChild(rPr);
					run->appendChild(AddText((std::wstring)Fb2run->Gettext()));
				}				
			}						
		}
		return run;
	}	


	const MSXML2::IXMLDOMElementPtr Document::AddRun(const std::wstring& Fb2run)
	{
		MSXML2::IXMLDOMElementPtr run = m_pXmlDoc->createElement(L"w:r");
		run->appendChild(AddText(Fb2run));
		return run;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddText(const std::wstring& text)
	{
		MSXML2::IXMLDOMElementPtr text_elem = m_pXmlDoc->createElement(L"w:t");
		MSXML2::IXMLDOMTextPtr text_node = m_pXmlDoc->createTextNode(static_cast<std::wstring>(text).c_str());
		text_elem->setAttribute(L"xml:space", (const _variant_t &)L"preserve");
		text_elem->appendChild(text_node);
		return text_elem;
	}	


	const MSXML2::IXMLDOMElementPtr Document::AddFldChar(const std::wstring& name)
	{
		MSXML2::IXMLDOMElementPtr run = m_pXmlDoc->createElement(L"w:r");
		MSXML2::IXMLDOMElementPtr fldChar = m_pXmlDoc->createElement(L"w:fldChar");
		fldChar->setAttribute( L"w:fldCharType", (const _variant_t &)name.c_str());
		run->appendChild(fldChar);
		return run;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddInstrText(const std::string& text)	
	{
		MSXML2::IXMLDOMElementPtr run = m_pXmlDoc->createElement(L"w:r");
		MSXML2::IXMLDOMElementPtr instrText = m_pXmlDoc->createElement(L"w:instrText");
		instrText->setAttribute( L"xml:space", (const _variant_t &)L"preserve" );
		MSXML2::IXMLDOMTextPtr text_node = m_pXmlDoc->createTextNode(static_cast<std::string>(text).c_str());
		instrText->appendChild(text_node);
		run->appendChild(instrText);
		return run;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddTab()
	{
		MSXML2::IXMLDOMElementPtr run = m_pXmlDoc->createElement(L"w:r");
		MSXML2::IXMLDOMElementPtr tab = m_pXmlDoc->createElement(L"w:tab");
		run->appendChild(tab);
		return run;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddPageBreak()
	{
		MSXML2::IXMLDOMElementPtr run = m_pXmlDoc->createElement(L"w:r");
		MSXML2::IXMLDOMElementPtr br = m_pXmlDoc->createElement(L"w:br");
		br->setAttribute(L"w:type", (const _variant_t &)"page");
		run->appendChild(br);
		return run;
	}


	const MSXML2::IXMLDOMElementPtr Document::AddSdtPr()
	{
		MSXML2::IXMLDOMElementPtr content_node = m_pXmlDoc->createElement(L"w:sdt");
		MSXML2::IXMLDOMElementPtr sdtPr_node = m_pXmlDoc->createElement(L"w:sdtPr");
		MSXML2::IXMLDOMElementPtr id_node = m_pXmlDoc->createElement(L"w:id");
		id_node->setAttribute(L"w:val", (const _variant_t &)"15801580");
		MSXML2::IXMLDOMElementPtr docPartObj_node = m_pXmlDoc->createElement(L"w:docPartObj");
		MSXML2::IXMLDOMElementPtr docPartGallery_node = m_pXmlDoc->createElement(L"w:docPartGallery");
		docPartGallery_node->setAttribute(L"w:val", (const _variant_t &)"Table of Contents");
		MSXML2::IXMLDOMElementPtr docPartUnique_node = m_pXmlDoc->createElement(L"w:docPartUnique");
		docPartObj_node->appendChild(docPartGallery_node);
		docPartObj_node->appendChild(docPartUnique_node);

		sdtPr_node->appendChild(id_node);
		sdtPr_node->appendChild(docPartObj_node);
		return sdtPr_node;
	}


	const std::wstring Document::getStyleName(const std::wstring& tagName)
	{		
		if ( tagName == L"epigraph" )
			return L"EpigraphStyle";
		else if ( tagName == L"title" )
			return L"Heading1";
		else if ( tagName == L"subtitle" )
			return L"Heading2";
		else if ( tagName == L"annotation" )
			return L"AnnotationStyle";
		else if ( tagName == L"strong" )
			return L"StrongStyle";
		else if ( tagName == L"emphasis" )
			return L"EmphasisStyle";
		else if ( tagName == L"a" )
			//return L"EndnoteReference";
			return L"FootnoteReference";
		else if ( tagName == L"section" || tagName == L"body" || tagName == L"paragraph")
			return L"ParagraphStyle";
		else if ( tagName == L"v" )
			return L"PoemLineStyle";
		else if ( tagName == L"TOC" )
			return L"TOCHeading";
		return L"";
	}


	const int Document::GetId(const std::wstring& href)
	{
		int id = 0;
		std::map<std::wstring, int>::const_iterator iter = m_notesIdTable.find(href);
		if (iter != m_notesIdTable.end())
		{
			id = iter->second;
		}
		else
		{
			id = m_notesIdTable.size() + 2;
			m_notesIdTable.insert(std::make_pair(href, id));
		}

		return id;
	}


	
}