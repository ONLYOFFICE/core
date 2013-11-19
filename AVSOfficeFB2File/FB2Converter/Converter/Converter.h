#pragma once
#include <string>
#include "../DocxDocument/DocxDocument.h"
#include "../FB2Document/FB2Document.h"

namespace FB2Converter
{
	class Converter
	{
	public:
		Converter(bool bGenerateContent);

	public:
		bool Convert(const FB2Document& fb2, DocxDocument& docx);

	private:
		void ConvertDescription(const MSXML2::IXMLDOMNodePtr description, DocxDocument& docx);
		void ConvertBody(const MSXML2::IXMLDOMNodePtr body, DocxDocument& docx);
		void ConvertBinary(const MSXML2::IXMLDOMNodePtr binary, DocxDocument& docx);

		void ConvertElementsFromDescription(const MSXML2::IXMLDOMNodePtr bodyChildElem, DocxDocument& docx);
	private: 
		bool m_bGenerateContent;
	};
}