#pragma once
#ifndef XML_2_DOCX_CONVERTER_INCLUDE_H_
#define XML_2_DOCX_CONVERTER_INCLUDE_H_

#include "AbstractConverter.h"
#include "XmlFormat.h"
#include "Docx.h"
#include "XmlEvent.h"
#include <vector>


namespace Xml2Docx
{
	class Converter : protected/*public*/ AbstractConverter<XmlFormat::File, Docx::Folder>
	{
	public:
		Converter(const boost::filesystem::wpath& originPath);
		void convert(XmlFormat::IXmlEvent& Event);
		void read(const boost::filesystem::wpath& path, bool space);
		void write(const boost::filesystem::wpath& path) const;
	private:
		bool AddElement(const XML::XElement& element, std::vector<OOX::Logic::TextItem>& document, int level, int percent, int step, XmlFormat::IXmlEvent& Event)const;
		void AddAttribute(const XML::XAttribute& attribute, OOX::Logic::Paragraph& paragraph)const;
		void AddNamespace(const XML::XNamespace& NameSpace, OOX::Logic::Paragraph& paragraph)const;
		void AddText(const XML::XText& text, std::vector<OOX::Logic::TextItem>& document, int level)const;
		void AddComment(const XML::XComment& comment, std::vector<OOX::Logic::TextItem>& document, int level)const;

		OOX::Logic::RunProperty rprSSS;
		OOX::Logic::RunProperty rprES;
		OOX::Logic::RunProperty rprTS;
		OOX::Logic::RunProperty rprCS;
		OOX::Logic::RunProperty rprNSS;
		OOX::Logic::RunProperty rprXMLNS;
		OOX::Logic::ParagraphProperty ppr;
	};
} // namespace Xml2Docx

#endif // XML_2_DOCX_CONVERTER_INCLUDE_H_