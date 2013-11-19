#pragma once
#include <string>
#include "./../File.h"

namespace FB2Converter
{
	class DocumentRels : public File
	{
	public:
		DocumentRels(const CString filepath);
		virtual ~DocumentRels();

	public:
		virtual bool Read();
		virtual bool Write(const CString filepath);

	public:
		void AddImage(const std::wstring& name);
		const std::string AddHyperlink(const std::wstring& href);
		const std::wstring GetRId(const std::wstring& name);

	private:
		MSXML2::IXMLDOMElementPtr	m_relsElems;

	private:
		CString		m_filepath;
	};
}