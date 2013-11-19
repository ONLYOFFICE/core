#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class Styles : public File
	{
	public:
		Styles(const CString filepath);
		virtual ~Styles();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	private:
		void AddStyles();
		void AddTitleStyle(const std::wstring& styleName);
		void AddEpigraphStyle(const std::wstring& styleName);
		void AddStrongStyle(const std::wstring& styleName);
		void AddEmphasisStyle(const std::wstring& styleName);
		void AddPoemLineStyle(const std::wstring& styleName);
		void AddParagraphStyle(const std::wstring& styleName);

	private:
		MSXML2::IXMLDOMElementPtr m_styleElems;

	private:
		CString		m_filepath;
	};
}