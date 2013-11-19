#pragma once
#include <string>
#include "./../files.h"

namespace FB2Converter
{
	class FB2Document
	{
	public:
		CString m_strMeta;
	public:
		FB2Document();

	public:
		bool Read(const CString filepath);
		bool Write(const CString filepath);

	public:
		MSXML2::IXMLDOMDocumentPtr m_pXmlDoc;
	};
}