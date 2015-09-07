#ifndef _SERVER_COMPONENTS_UNICODE_CONVERTER_H
#define _SERVER_COMPONENTS_UNICODE_CONVERTER_H

#ifndef UNICODECONVERTER_USE_DYNAMIC_LIBRARY
#define UNICODECONVERTER_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define UNICODECONVERTER_DECL_EXPORT Q_DECL_EXPORT
#endif

#include <string>

namespace NSUnicodeConverter
{
    class CUnicodeConverter_Private;
    class UNICODECONVERTER_DECL_EXPORT CUnicodeConverter
	{
	public:
        CUnicodeConverter();
        ~CUnicodeConverter();

    public:
        std::string fromUnicode(const std::wstring& sSrc, const char* sCodePage);
        std::wstring toUnicode(const std::string& sSrc, const char* sCodePage);

    private:
        CUnicodeConverter_Private* m_pInternal;
	};
}

#endif // _SERVER_COMPONENTS_UNICODE_CONVERTER_H
