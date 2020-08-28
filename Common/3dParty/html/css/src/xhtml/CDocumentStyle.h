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

        std::vector<std::pair<CCompiledStyle, bool>> m_arStyleUsed;
        std::vector<std::wstring> m_arStandardStylesUsed;

        CXmlElement AddParentsStyle(const NSCSS::CCompiledStyle& oStyle);
        CXmlElement CombineStandardStyles(const std::vector<std::wstring> arStandartedStyles);
        CXmlElement CreateStandardStyle(const std::wstring sNameStyle);
        CXmlElement ConvertStyle(const NSCSS::CCompiledStyle& oStyle, bool bIsPStyle);

        void SetRStyle(NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);
        void SetPStyle(NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);

    public:
        CDocumentStyle();
        ~CDocumentStyle();

        void WritePStyle(NSCSS::CCompiledStyle& oStyle);
        void WriteRStyle(NSCSS::CCompiledStyle& oStyle);

        void SetStyle(const std::wstring& sStyle);
        void SetId(const std::wstring& sId);

        std::wstring GetStyle();
        std::wstring GetId() const;

        void Clear();
    };
}
#endif // CDOCUMENTSTYLE_H
