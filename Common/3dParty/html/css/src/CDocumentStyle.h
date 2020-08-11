#ifndef CDOCUMENTSTYLE_H
#define CDOCUMENTSTYLE_H

#include "CssCalculator_global.h"
#include "CCompiledStyle.h"

namespace NSCSS
{
    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::wstring m_sStyle;
        std::wstring m_sId;
    public:
        CDocumentStyle();
        ~CDocumentStyle();

        void WriteStyle(NSCSS::CCompiledStyle& oStyle);
        std::wstring GetStyle();
        std::wstring GetId();
    };
}
#endif // CDOCUMENTSTYLE_H
