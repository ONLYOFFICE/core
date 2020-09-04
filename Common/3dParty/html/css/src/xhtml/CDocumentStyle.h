#ifndef CDOCUMENTSTYLE_H
#define CDOCUMENTSTYLE_H

#include "../CssCalculator_global.h"
#include "../CCompiledStyle.h"
#include "CXmlElement.h"

#include <iostream>
namespace NSCSS
{
    class CStyleUsed
    {
    public:
        CCompiledStyle m_oStyle;
        bool m_bIsPStyle;

        std::wstring m_sId;

        CStyleUsed(CCompiledStyle oStyle, const bool bIsPStyle) : m_oStyle(oStyle), m_bIsPStyle(bIsPStyle) {}

        bool operator==(const CStyleUsed& oUsedStyle) const
        {
            return ((m_oStyle == oUsedStyle.m_oStyle) && (m_bIsPStyle == oUsedStyle.m_bIsPStyle));
        }

        bool operator<(const CStyleUsed& oUsedStyle) const
        {
            return m_oStyle < oUsedStyle.m_oStyle;
        }
    };

    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::wstring m_sStyle;
        std::wstring m_sId;
        std::vector<std::wstring> m_arStandardStyles;

        std::vector<CStyleUsed> m_arStyleUsed;

        std::vector<std::wstring> m_arStandardStylesUsed;

        CXmlElement CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles);
        CXmlElement CreateStandardStyle(const std::wstring& sNameStyle);
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
