#ifndef CDOCUMENTSTYLE_H
#define CDOCUMENTSTYLE_H

#include "../CssCalculator_global.h"
#include "../CCompiledStyle.h"
#include "CXmlElement.h"

namespace NSCSS
{
    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::wstring m_sStyle;
        std::wstring m_sId;
        std::vector<std::wstring> m_arStandardStyles;
        std::vector<std::wstring> m_arUsedStyles;

        CXmlElement AddParentsStyle(NSCSS::CCompiledStyle& oStyle);

    public:
        CDocumentStyle();
        ~CDocumentStyle();

        void WriteStyle(NSCSS::CCompiledStyle& oStyle);

        void SetStyle(const std::wstring& sStyle);
        void SetId(const std::wstring& sId);

        std::wstring GetStyle();
        std::wstring GetId();

        void Clear();
    };
}
#endif // CDOCUMENTSTYLE_H
