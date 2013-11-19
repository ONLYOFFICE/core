#include "Endnotes.h"
#include <string>
#include "./../../msxml/XML.h"

namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\endnotes.xml");

	Endnotes::Endnotes(const CString filepath)
		:m_filepath(filepath)
	{
		m_notesElems = m_pXmlDoc->createElement("w:endnotes");
	}

	Endnotes::~Endnotes()
	{
	}


	bool Endnotes::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool Endnotes::Write(const CString filepath)
	{
		MSXML2::IXMLDOMElementPtr endnote_node = m_pXmlDoc->selectSingleNode(L"w:endnotes");
		MSXML2::IXMLDOMNodeListPtr endnotes  = m_notesElems->selectNodes( L"w:endnote" );
        while( MSXML2::IXMLDOMNodePtr endnote = endnotes->nextNode() )
			endnote_node->appendChild(endnote);

		m_pXmlDoc->removeChild(endnote_node);
		m_pXmlDoc->appendChild(endnote_node);

		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}


	bool Endnotes::AddNote(const MSXML2::IXMLDOMNodePtr note, const int id)
	{
		MSXML2::IXMLDOMElementPtr endnote_node = m_pXmlDoc->createElement(L"w:endnote");
		endnote_node->setAttribute(L"w:id", (const _variant_t &)id);
		std::wstring name = note->GetbaseName();

		AddParagraphToNote(note, endnote_node);
		m_notesElems->appendChild(endnote_node);

		return true;
	}


	void Endnotes::AddParagraphToNote(const MSXML2::IXMLDOMNodePtr note, MSXML2::IXMLDOMNodePtr endnote_node)
	{
		std::wstring elemName = note->GetnodeName();
		MSXML2::IXMLDOMNodeListPtr ins_elems = note->GetchildNodes();
		while(const MSXML2::IXMLDOMNodePtr child = ins_elems->nextNode())
		{
			std::wstring childElemName = child->GetnodeName();
			if ( childElemName == L"p" )
				AddParagraph(child, elemName, endnote_node);
			else 
				AddParagraphToNote(child, endnote_node);
		}
	}


	bool Endnotes::AddParagraph(const MSXML2::IXMLDOMNodePtr paragraphFb2, const std::wstring& parentName,  MSXML2::IXMLDOMNodePtr note_node)
	{
		std::wstring styleName = getStyleName(parentName);
		if (styleName != L"")
		{
			int count = note_node->GetchildNodes()->Getlength();
			MSXML2::IXMLDOMElementPtr paragraph = m_pXmlDoc->createElement(L"w:p");
			MSXML2::IXMLDOMElementPtr paragraphPr = m_pXmlDoc->createElement(L"w:pPr");
			MSXML2::IXMLDOMElementPtr pStyle = m_pXmlDoc->createElement(L"w:pStyle");
			pStyle->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
			paragraphPr->appendChild(pStyle);
			paragraph->appendChild(paragraphPr);

			if (count == 1)
			{
				MSXML2::IXMLDOMElementPtr firstParagraph = note_node->selectSingleNode(L"w:p");
				MSXML2::IXMLDOMNodeListPtr runs = firstParagraph->selectNodes(L"w:r");
				while( MSXML2::IXMLDOMNodePtr run_node = runs->nextNode() )
					paragraph->appendChild(run_node);
				note_node->removeChild(firstParagraph);
			}

			MSXML2::IXMLDOMElementPtr run_elem = NULL;
			paragraphFb2->QueryInterface( __uuidof(MSXML2::IXMLDOMElement), (void**) & run_elem );
			run_elem->setAttribute( L"xml:space", (const _variant_t &)L"preserve" );

			MSXML2::IXMLDOMNodeListPtr runs = paragraphFb2->GetchildNodes();
			while( MSXML2::IXMLDOMNodePtr run_node = runs->nextNode() )
			{				
				MSXML2::IXMLDOMElementPtr run;
				if (count == 0)
					run = AddRun(run_node, true);				
				else
					run = AddRun(run_node, false);
				if (run != NULL)
					paragraph->appendChild(run);
			}	
			note_node->appendChild( paragraph );        
		}
		
		return true;
	}


	const MSXML2::IXMLDOMElementPtr Endnotes::AddRun(const MSXML2::IXMLDOMNodePtr Fb2run, bool isFirstParagraph)
	{
		/*std::wstring runName = Fb2run->GetnodeName();
		MSXML2::IXMLDOMElementPtr run;

		if (runName == L"#text")
		{			
			run = m_pXmlDoc->createElement(L"w:r");
			run->appendChild(AddText((std::wstring)Fb2run->Gettext()));				
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
				if (runName == L"title" || runName == L"subtitle")
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
		return run;*/
		std::wstring runName = Fb2run->GetnodeName();
		MSXML2::IXMLDOMElementPtr run;

		std::wstring runStyleName;
		if (isFirstParagraph)
			runStyleName = getStyleName(L"a");
		else
			runStyleName = getStyleName(runName);

		MSXML2::IXMLDOMElementPtr rPr = m_pXmlDoc->createElement(L"w:rPr");
		MSXML2::IXMLDOMElementPtr rStyle = m_pXmlDoc->createElement(L"w:rStyle");
		rStyle->setAttribute(L"w:val", (const _variant_t &)runStyleName.c_str());
		rPr->appendChild(rStyle);
		
		if (runStyleName != L"")
			if (runName == L"title" || runName == L"subtitle")
			{
			}
			else
			{
				run = m_pXmlDoc->createElement(L"w:r");
				run->appendChild(rPr);
				run->appendChild(AddText((std::wstring)Fb2run->Gettext()));
			}
		else if (runName == L"#text")
		{
			run = m_pXmlDoc->createElement(L"w:r");
			run->appendChild(AddText((std::wstring)Fb2run->Gettext()));	
		}

		return run;
	}


	MSXML2::IXMLDOMElementPtr Endnotes::AddText(const std::wstring& text)
	{
		MSXML2::IXMLDOMElementPtr text_elem = m_pXmlDoc->createElement(L"w:t");
		MSXML2::IXMLDOMTextPtr text_node = m_pXmlDoc->createTextNode(static_cast<std::wstring>(text).c_str());
		text_elem->setAttribute(L"xml:space", (const _variant_t &)L"preserve");
		text_elem->appendChild(text_node);
		return text_elem;
	}	


	const std::wstring Endnotes::getStyleName(const std::wstring& tagName)
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
			return L"EndnoteReference";
		else if ( tagName == L"section" || tagName == L"body")
			return L"ParagraphStyle";
		return L"";
	}

}