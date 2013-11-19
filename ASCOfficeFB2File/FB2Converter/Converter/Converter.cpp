#include "Converter.h"
#include "./../files.h"
#include "./../msxml/XML.h"



namespace FB2Converter
{
	Converter::Converter(bool bGenerateContent):m_bGenerateContent(bGenerateContent)
	{
	}


	bool Converter::Convert(const FB2Document& fb2, DocxDocument& docx)
	{
		//делаем как в stdu: в первом body каждый элемент на отдельной странице
		//В следующих body нет разрывов страниц.
		bool bFirstBody = true;
		MSXML2::IXMLDOMNodeListPtr ins_elems = fb2.m_pXmlDoc->selectNodes( L"FictionBook/*" );
		while( const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode() )
        {
			std::wstring elementName = child->GetnodeName();
			if (elementName == L"description")
				ConvertDescription(child, docx);
			else if (elementName == L"body")
			{
				if( true == bFirstBody )
				{
					bFirstBody = false;
					docx.m_document.m_bAddPageBreak = true;
				}
				else
					docx.m_document.m_bAddPageBreak = false;
				ConvertBody(child, docx);
			}
			else if (elementName == L"binary")
				ConvertBinary(child, docx);
        }

		return true;
	}


	void Converter::ConvertDescription(const MSXML2::IXMLDOMNodePtr description, DocxDocument& docx)
	{
		MSXML2::IXMLDOMNodeListPtr ins_elems = description->GetchildNodes();
		while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
        {
			std::wstring childElemName = child->GetnodeName();
			if (childElemName == L"title-info")
			{
				//cover
				MSXML2::IXMLDOMNodePtr cover = child->selectSingleNode( L"./coverpage/image" );
				MSXML2::IXMLDOMElementPtr coverEl = cover;
				if( NULL != cover)
				{
					coverEl->setAttribute( L"AVSCover", (const _variant_t &)L"true" );
					docx.AddElement(cover);
				}
				//other
				ConvertElementsFromDescription(child, docx);
			}
        }
		if(m_bGenerateContent)
			docx.AddContent();
	}


	void Converter::ConvertBody(const MSXML2::IXMLDOMNodePtr body, DocxDocument& docx)
	{		
		MSXML2::IXMLDOMNodeListPtr ins_elems = body->GetchildNodes();
		while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
        {
			std::wstring name = child->GetnodeName();
			MSXML2::IXMLDOMNamedNodeMapPtr attrs = child->Getattributes();

			docx.m_document.m_bFirstTitle = true;
			docx.AddElement(child);
        }
	}


	void Converter::ConvertBinary(const MSXML2::IXMLDOMNodePtr binary, DocxDocument& docx)
	{
		std::wstring contentType = XML::getAttr(binary, L"content-type");
		std::wstring id = XML::getAttr(binary, L"id");
		docx.AddImage(binary, contentType, id);
	}
	

	void Converter::ConvertElementsFromDescription(const MSXML2::IXMLDOMNodePtr bodyChildElem, DocxDocument& docx)
	{
		std::wstring elemName = bodyChildElem->GetnodeName();
		MSXML2::IXMLDOMNodeListPtr ins_elems = bodyChildElem->GetchildNodes();
		while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
		{
			std::wstring childElemName = child->GetnodeName();
			//if (elemName == L"author" && (childElemName == L"first-name" || childElemName == L"last-name"))			
			//	docx.AddParagraph(child, L"title");
			//else if (childElemName == L"book-title")
			//	docx.AddParagraph(child, L"title");
			//else 
			if (childElemName == L"annotation")
				docx.AddAnnotation(child);
			//else if (childElemName == L"image")
			//	docx.AddImage(child);
			else
				ConvertElementsFromDescription(child, docx);
		}
	}
}