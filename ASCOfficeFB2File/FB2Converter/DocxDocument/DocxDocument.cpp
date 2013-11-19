#include "DocxDocument.h"
#define TITLE_STYLE			L"TitleStyle";
#define EPIGRAPH_STYLE      L"EpigraphStyle";
#define PARAGRAPH_STYLE     L"ParagraphStyle";
#define ANNOTATION_STYLE	L"AnnotationStyle";
#define EMPHASIS_STYLE		L"EmphasisStyle";
#define STRONG_STYLE		L"StrongStyle";


namespace FB2Converter
{
	DocxDocument::DocxDocument(const CString filepath, bool setContent, bool bGenerateContent)
		: m_filepath(filepath),
		  m_contentTypes(filepath),
		  m_docxRels(filepath),
		  m_app(filepath),
		  m_core(filepath),
		  m_document(filepath, setContent, bGenerateContent)
	{
	}


	bool DocxDocument::Read()
	{
		if (m_contentTypes.Read()
			&& m_docxRels.Read()
			&& m_app.Read()
			&& m_core.Read()
			&& m_document.Read())	
				return true;
		return false;
	}


	bool DocxDocument::Write(const CString filepath)
	{
		CreateDirectoryW(filepath + _T("\\word"), NULL);		
		CreateDirectoryW(filepath + _T("\\docProps"), NULL);		
		CreateDirectoryW(filepath + _T("\\_rels"), NULL);
		CreateDirectoryW(filepath + _T("\\word\\_rels"), NULL);
		CreateDirectoryW(filepath + _T("\\word\\theme"), NULL);
		CreateDirectoryW(filepath + _T("\\word\\media"), NULL);
	
		m_contentTypes.Write(filepath);
		m_docxRels.Write(filepath);
		m_app.Write(filepath);
		m_core.Write(filepath);
		m_document.Write(filepath);	

		return true;
	}


	bool DocxDocument::AddContent()
	{
		return m_document.AddContent();
	}


	bool DocxDocument::AddElement(const MSXML2::IXMLDOMNodePtr element)
	{
		return m_document.AddElement(element, L"body");
	}


	bool DocxDocument::AddParagraph(const MSXML2::IXMLDOMNodePtr paragraphFb2, const std::wstring& parentName)
	{
		return m_document.AddParagraph(paragraphFb2, parentName);
	}


	bool DocxDocument::AddParagraph(const std::wstring paragraph, const std::wstring& parentName)
	{
		return m_document.AddParagraph(paragraph, parentName);
	}


	bool DocxDocument::AddTable(const MSXML2::IXMLDOMNodePtr table)
	{
		return m_document.AddTable(table);
	}


	bool DocxDocument::AddEmptyLine()
	{
		return m_document.AddEmptyLine();
	}


	bool DocxDocument::AddNote(const MSXML2::IXMLDOMNodePtr note, const std::wstring& href)
	{
		return m_document.AddNote(note, href);
	}


	bool DocxDocument::AddPoem(const MSXML2::IXMLDOMNodePtr poem)
	{
		return m_document.AddPoem(poem);
	}


	bool DocxDocument::AddImage(const MSXML2::IXMLDOMNodePtr image)
	{
		return m_document.AddImage(image);
	}


	bool DocxDocument::AddImage(const MSXML2::IXMLDOMNodePtr binary, const std::wstring& type, const std::wstring& id)
	{
		return m_document.AddImage(binary, type, id);
	}

	bool DocxDocument::AddAnnotation(const MSXML2::IXMLDOMNodePtr annotation)
	{
		return m_document.AddAnnotation(annotation);
	}
}