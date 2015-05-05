#pragma once
#ifndef DOCX_2_TXT_CONVERTER_INCLUDE_H_
#define DOCX_2_TXT_CONVERTER_INCLUDE_H_

#include <vector>
#include <string>

namespace TxtXml
{
	class ITxtXmlEvent;
}

namespace Docx2Txt
{
    class Converter_Impl;
    class Converter
    {
	public:
		Converter();
		~Converter();

		void convert(TxtXml::ITxtXmlEvent& Event);
    	
        void read	(const std::wstring& path);
        void write	(const std::wstring& path);

        void writeUtf8		(const std::wstring& path) const;
        void writeUnicode	(const std::wstring& path) const;
        void writeBigEndian	(const std::wstring& path) const;
        void writeAnsi		(const std::wstring& path) const;

    private:
        Converter_Impl * converter_;
    
    };

	
} // namespace Docx2Txt

#endif // DOCX_2_TXT_CONVERTER_INCLUDE_H_
