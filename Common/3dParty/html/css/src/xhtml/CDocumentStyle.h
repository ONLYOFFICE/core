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
        bool m_bIsPStyle;
        std::wstring m_sId;
        CCompiledStyle m_oStyle;

        CStyleUsed(const CCompiledStyle& oStyle, const bool& bIsPStyle) : m_oStyle(oStyle), m_bIsPStyle(bIsPStyle) {}

        bool operator==(const CStyleUsed& oUsedStyle) const
        {
            return (m_oStyle == oUsedStyle.m_oStyle) && (m_bIsPStyle == oUsedStyle.m_bIsPStyle);
        }

        /*
        bool operator<(const CStyleUsed& oUsedStyle) const
        {
            return m_oStyle < oUsedStyle.m_oStyle;
        }
        */
    };

    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::wstring m_sStyle;
        std::wstring m_sId;

        std::vector<std::wstring> m_arStandardStylesUsed;
        std::vector<std::wstring> m_arStandardStyles;
        std::vector<CStyleUsed> m_arStyleUsed;

        void CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement);
        void CreateStandardStyle(const std::wstring& sNameStyle, CXmlElement& oElement);
        void ConvertStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle);

        void SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);
        void SetPStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);

    public:
        CDocumentStyle();
        ~CDocumentStyle();

        void WritePStyle(const NSCSS::CCompiledStyle& oStyle);
        void WriteRStyle(const NSCSS::CCompiledStyle& oStyle);

        void SetStyle(const std::wstring& sStyle);
        void SetId(const std::wstring& sId);

        std::wstring GetStyle() const;
        std::wstring GetIdAndClear();

        void Clear();
    };
}
#endif // CDOCUMENTSTYLE_H
