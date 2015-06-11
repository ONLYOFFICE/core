#pragma once

#include "../progressCallback.h"
#include <string>

namespace Oox2Odf
{
	class OoxConverter;

	class Converter
	{

	public:
        Converter(const std::wstring & path, const std::wstring  & type, const std::wstring & fontsPath, const ProgressCallback* ffCallBack);
        virtual ~Converter();

	public:
		void convert();
     
        void write(const std::wstring & path) const;

		OoxConverter * get_ooxConverter() { return impl_; }

	private:
		OoxConverter* impl_;
	
	};
} // namespace Oox2Odf
