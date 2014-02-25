#pragma once

#include <boost_filesystem_version.h>

namespace Oox2Odf
{
	class Impl;
	class Converter
	{

	public:
		Converter(const std::wstring & path, const std::wstring & type);
        virtual ~Converter();

	public:
		void convert();
     
        void write(const std::wstring & path) const;

		Impl * get_impl() { return impl_; }

	private:
		Impl* impl_;
	};
} // namespace Oox2Odf:Convert
