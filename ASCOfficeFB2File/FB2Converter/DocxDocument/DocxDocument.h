#pragma once
#include <string>
#include <vector>
#include "./../files.h"
#include "App/App.h"
#include "Core/Core.h"
#include "DocxRels/DocxRels.h"
#include "Document/Document.h"
#include "ContentTypes/ContentTypes.h"

namespace FB2Converter
{
	class DocxDocument
	{
	public:
		DocxDocument(const CString filepath, bool setContent, bool bGenerateContent);

	public:
		bool Read();
		bool Write(const CString filepath);

	public:
		bool AddElement(const MSXML2::IXMLDOMNodePtr element);

	public:
		bool AddContent		();
		bool AddParagraph	(const MSXML2::IXMLDOMNodePtr paragraph, const std::wstring& parentName);
		bool AddParagraph	(const std::wstring paragraph, const std::wstring& parentName);
		bool AddEmptyLine	();
		bool AddTable		(const MSXML2::IXMLDOMNodePtr table);
		bool AddNote		(const MSXML2::IXMLDOMNodePtr note, const std::wstring& href);
		bool AddPoem		(const MSXML2::IXMLDOMNodePtr poem);
		bool AddImage		(const MSXML2::IXMLDOMNodePtr image);
		bool AddImage		(const MSXML2::IXMLDOMNodePtr binary, const std::wstring& type, const std::wstring& id);
		bool AddAnnotation	(const MSXML2::IXMLDOMNodePtr annotation);

	private:
		MSXML2::IXMLDOMElementPtr AddText(const std::wstring& text);

	private:
		CString		m_filepath;

	private:
		ContentTypes		m_contentTypes;
		DocxRels			m_docxRels;
		App					m_app;
		Core				m_core;
	public:
		Document			m_document;
	};
}