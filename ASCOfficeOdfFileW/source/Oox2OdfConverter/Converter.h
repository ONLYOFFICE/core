#pragma once

#include <boost_filesystem_version.h>

static std::wstring string2std_string(CString val)
{
	return std::wstring(val.GetBuffer());
}

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual void convert() = 0;
		virtual void write(const std::wstring & path) = 0;
		OoxConverter(){}
	};

	class Converter
	{

	public:
		Converter(const std::wstring & path);
        virtual ~Converter();

	public:
		void convert();
     
        void write(const std::wstring & path) const;

		OoxConverter * get_ooxConverter() { return impl_; }

	private:
		OoxConverter* impl_;
	};
} // namespace Oox2Odf:Convert
