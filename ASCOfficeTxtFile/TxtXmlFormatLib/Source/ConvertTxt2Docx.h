#pragma once
#ifndef TXT_2_DOCX_CONVERTER_INCLUDE_H_
#define TXT_2_DOCX_CONVERTER_INCLUDE_H_

#include "../../../Common/DocxFormat/Source/XML/Utils.h"
#include <string>

namespace TxtXml
{
	class ITxtXmlEvent;
}

namespace Txt2Docx
{
    class Converter_Impl;

	class Converter
	{
	public:
		Converter	(int encoding);
        ~Converter	();
		
		void convert(TxtXml::ITxtXmlEvent& Event);
        void read	(const std::wstring& path);
        void write	(XmlUtils::CStringWriter & stringWriter/*const std::wstring& path*/);

    private:
        Converter_Impl * converter_;
	};

} // namespace Txt2Docx

#endif // TXT_2_DOCX_CONVERTER_INCLUDE_H_
