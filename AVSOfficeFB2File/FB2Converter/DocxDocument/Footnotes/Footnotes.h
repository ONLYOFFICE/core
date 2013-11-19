#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class Footnotes : public File
	{
	public:
		Footnotes(const CString filepath);
		virtual ~Footnotes();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	public:
		bool AddNote(const MSXML2::IXMLDOMNodePtr note, const int id);
		
	private:
		void AddParagraphToNote(const MSXML2::IXMLDOMNodePtr note, MSXML2::IXMLDOMNodePtr endnote_node);
		bool AddParagraph(const MSXML2::IXMLDOMNodePtr paragraph, const std::wstring& parentName,  MSXML2::IXMLDOMNodePtr endnote_node);
		const MSXML2::IXMLDOMElementPtr AddRun(const MSXML2::IXMLDOMNodePtr Fb2run, bool isFirstParagraph);

	private:
		MSXML2::IXMLDOMElementPtr AddText(const std::wstring& text);

	private:
		const std::wstring getStyleName(const std::wstring& tagName);

	private:
		MSXML2::IXMLDOMElementPtr m_notesElems;

	private:
		CString		m_filepath;
	};
}