#pragma once
#include "stdAfx.h"

#include "Converter.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

namespace Oox2Odf
{
	Converter::Converter(const std::wstring & path) 
    {
		std::wstring  type = L"spreadsheet";

		if (type == L"text")			impl_ = new DocxConverter(path);
		if (type == L"spreadsheet")		impl_ = new XlsxConverter(path);
	}

	Converter::~Converter() 
    {
		if (impl_ )delete impl_ ;
	}
    void Converter::convert()
    {
		if (!impl_)return;
        impl_->convert();
    }
    void Converter::write(const std::wstring & path) const
    {
        return impl_->write(path);
    }

} // namespace Docx2Odt