#pragma once
#include <string>
#include <map>
#include "./../File.h"
#include "./../Footnotes/Footnotes.h"
#include "./../Endnotes/Endnotes.h"
#include "./../DocumentRels/DocumentRels.h"
#include "./../Styles/Styles.h"
#include "./../FontTable/FontTable.h"
#include "./../Theme/Theme.h"
#include "./../WebSettings/WebSettings.h"
#include "./../Settings/Settings.h"

#include "gdiplus.h"

namespace FB2Converter
{
	class Document : public File
	{
	public:
		Document(const CString filepath, bool setContent, bool bGenerateContent);
		virtual ~Document();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

		bool CopyElements(const MSXML2::IXMLDOMNodePtr elementFrom, MSXML2::IXMLDOMNodePtr elementTo, const CString filepath);

		bool AddElement(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentElemName, MSXML2::IXMLDOMElementPtr elems);
		bool AddElement(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentElemName);

	public:
		bool AddContent			();
		bool AddParagraph		(const MSXML2::IXMLDOMNodePtr paragraph, const std::wstring& parentName, MSXML2::IXMLDOMElementPtr elems);
		bool AddParagraph		(const MSXML2::IXMLDOMNodePtr paragraph, const std::wstring& parentName);
		bool AddParagraph		(const std::wstring paragraphFb2, const std::wstring& parentName, MSXML2::IXMLDOMElementPtr elems);
		bool AddParagraph		(const std::wstring paragraphFb2, const std::wstring& parentName);
		bool AddNote			(const MSXML2::IXMLDOMNodePtr note, const std::wstring& href);
		bool AddMarkedParagraph	(const MSXML2::IXMLDOMNodePtr element, const std::wstring& id);
		bool AddEmptyLine		(MSXML2::IXMLDOMElementPtr elems);
		bool AddEmptyLine		();
		bool AddTable			(const MSXML2::IXMLDOMNodePtr table, MSXML2::IXMLDOMElementPtr elems);
		bool AddTable			(const MSXML2::IXMLDOMNodePtr table);
		bool AddPoem			(const MSXML2::IXMLDOMNodePtr poem, MSXML2::IXMLDOMElementPtr elems);
		bool AddPoem			(const MSXML2::IXMLDOMNodePtr poem);
		bool AddStranza			(const MSXML2::IXMLDOMNodePtr Fb2Stranza, MSXML2::IXMLDOMElementPtr elems);
		bool AddStranza			(const MSXML2::IXMLDOMNodePtr Fb2Stranza);
		bool AddImage			(const MSXML2::IXMLDOMNodePtr image, MSXML2::IXMLDOMElementPtr elems);
		bool AddImage			(const MSXML2::IXMLDOMNodePtr image);
		bool AddImage			(const MSXML2::IXMLDOMNodePtr binary, const std::wstring& type, const std::wstring& id);
		bool AddPageBreak		(MSXML2::IXMLDOMElementPtr elems);
		bool AddAnnotation		(const MSXML2::IXMLDOMNodePtr annotation);			 

	private:
		void GetFileInBinary(std::wstring& valueInBase64, BYTE*& data, int& size);		
		bool SaveFile(const BYTE* bytes, const int size, CString path);
		void Resize(int old_width, int old_height, int& new_width, int& new_height, int dpi);

	private:
		const std::string setMarkToContent(const MSXML2::IXMLDOMNodePtr element, const std::wstring& parentName);

	private:
		const MSXML2::IXMLDOMElementPtr AddRun(const std::wstring& Fb2run);
		const MSXML2::IXMLDOMElementPtr AddRun(const MSXML2::IXMLDOMNodePtr Fb2run);
		const MSXML2::IXMLDOMElementPtr AddText(const std::wstring& text);	

		const MSXML2::IXMLDOMElementPtr AddFldChar(const std::wstring& name);
		const MSXML2::IXMLDOMElementPtr AddInstrText(const std::string& text);
		const MSXML2::IXMLDOMElementPtr AddTab();
		const MSXML2::IXMLDOMElementPtr AddPageBreak();
		const MSXML2::IXMLDOMElementPtr AddSdtPr();

	private:
		const std::wstring getStyleName(const std::wstring& tagName);
		const int GetId(const std::wstring& href);
		const MSXML2::IXMLDOMElementPtr CreateImageElem(MSXML2::IXMLDOMElementPtr image, const std::wstring& href, bool bParagraph);
		
	public: 
		bool		m_bAddPageBreak;// флаг, если первый элемент в документе title, то не делать разрыв страницы
		bool		m_bFirstTitle;// флаг, все вложеные титулы воспринимаются как subtitle
	private:
		MSXML2::IXMLDOMElementPtr	m_bodyElems;
		MSXML2::IXMLDOMElementPtr	m_content;

	private:
		int m_countBookmark;
		bool m_hasContent;

	private:
		Footnotes					m_footnotes;
		Endnotes					m_endnotes;
		DocumentRels				m_documentRels;	
		Theme						m_theme;
		Settings					m_settings;
		WebSettings					m_webSettings;
		Styles						m_styles;
		FontTable					m_fontTable;

	private:
		std::map<std::wstring, int> m_notesIdTable;

	private:
		CString		m_filepath;
		bool		m_bFirstElement; // флаг, если первый элемент в документе title, то не делать разрыв страницы
		bool        m_bPrevTitle;// флаг, если предыдущий элемент был титульником, то не ставим разрыв страницы
		bool		m_bFirstTitlePar; // флаг, в contents попадает только первый параграф
		bool		m_bGenerateContent;
	};
}