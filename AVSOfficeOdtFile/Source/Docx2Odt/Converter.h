#pragma once
#ifndef DOCX_2_ODT_CONVERTER_INCLUDE_H_
#define DOCX_2_ODT_CONVERTER_INCLUDE_H_

#include <boost_filesystem_version.h>


namespace Odt {
    class IOdtEvent;
    namespace Meta {
        class File;
    }
}

namespace OOX {
    class IOdtEvent;
    class App;
}

namespace Docx2Odt
{
    class Converter_Impl;
	class Converter
	{

	public:
		Converter(const boost::filesystem::wpath& originPath);
        virtual ~Converter();

	public:
		void convert(Odt::IOdtEvent& Event);
        void read(const boost::filesystem::wpath& path);
        void write(const boost::filesystem::wpath& path) const;

	private:
		void app2meta(const OOX::App& app, Odt::Meta::File& meta) const;

    private:
        Converter_Impl * converter_;
	};
} // namespace Docx2Odt

#endif // DOCX_2_ODT_CONVERTER_INCLUDE_H_