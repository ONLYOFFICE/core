#ifndef CDOCUMENTSTYLE_H
#define CDOCUMENTSTYLE_H

#include "CssCalculator_global.h"
#include "CCompiledStyle.h"

namespace NSCSS
{
    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::wstring m_sStyle;
    public:
        CDocumentStyle();
        CDocumentStyle(NSCSS::CCompiledStyle& oStyle);
        ~CDocumentStyle();

        void WriteStyle( NSCSS::CCompiledStyle& oStyle);
        std::wstring GetStyle();

    };
}
#endif // CDOCUMENTSTYLE_H
