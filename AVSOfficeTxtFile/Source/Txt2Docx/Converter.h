#pragma once
#ifndef TXT_2_DOCX_CONVERTER_INCLUDE_H_
#define TXT_2_DOCX_CONVERTER_INCLUDE_H_

#include <boost/filesystem.hpp>

namespace Txt
{
	class ITxtEvent;
}

namespace Txt2Docx
{
    class Converter_Impl;

	class Converter
	{
	public:
		Converter(const boost::filesystem::wpath& originPath, int encoding);
        ~Converter();
		void convert(Txt::ITxtEvent& Event);
    	void read(const boost::filesystem::wpath& path);
	    void write(const boost::filesystem::wpath& path) const;

    private:
        Converter_Impl * converter_;
	};

} // namespace Txt2Docx

#endif // TXT_2_DOCX_CONVERTER_INCLUDE_H_