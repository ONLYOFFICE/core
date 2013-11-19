#pragma once
#ifndef DOCX_2_TXT_CONVERTER_INCLUDE_H_
#define DOCX_2_TXT_CONVERTER_INCLUDE_H_

#include <boost/filesystem.hpp>
#include <vector>
#include <string>

namespace Txt
{
	class ITxtEvent;
}

namespace Docx2Txt
{
    class Converter_Impl;
    class Converter
    {
	public:
		Converter();
		~Converter();

	public:
		void convert(Txt::ITxtEvent& Event);
    	void read(const boost::filesystem::wpath& path);
	    void write(const boost::filesystem::wpath& path) const;

	public:
		void writeUtf8(const boost::filesystem::wpath& path) const;
		void writeUnicode(const boost::filesystem::wpath& path) const;
		void writeBigEndian(const boost::filesystem::wpath& path) const;
		void writeAnsi(const boost::filesystem::wpath& path) const;

    private:
        Converter_Impl * converter_;
    
    };

	
} // namespace Docx2Txt

#endif // DOCX_2_TXT_CONVERTER_INCLUDE_H_