#pragma once
#include <string>
#include "./../files.h"

namespace FB2Converter
{
	class File
	{
	public:
		File();
		virtual ~File();

	public:
		virtual bool Read() = 0;
		virtual bool Write(const CString filepath) = 0;

	public:
		MSXML2::IXMLDOMDocumentPtr m_pXmlDoc;

	private:
		CString		m_filepath;
	};
}