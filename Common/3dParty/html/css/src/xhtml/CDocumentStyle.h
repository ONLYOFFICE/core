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
        CCompiledStyle m_oStyle;
        bool m_bIsPStyle;
        std::wstring m_sId;

    public:
        CStyleUsed(const CCompiledStyle& oStyle, bool bIsPStyle) : m_oStyle(oStyle), m_bIsPStyle(bIsPStyle) {}

        bool operator==(const CStyleUsed& oUsedStyle) const
        {
            return (m_bIsPStyle == oUsedStyle.m_bIsPStyle) && (m_oStyle == oUsedStyle.m_oStyle);
        }

        std::wstring getId()
        {
            return m_sId;
        }

        void setId(const std::wstring& sId)
        {
            m_sId = sId;
        }

    };

    class CSSCALCULATOR_EXPORT CDocumentStyle
    {
        std::list<std::wstring> m_arStandardStylesUsed;
        std::list<std::wstring> m_arStandardStyles;
        std::list<CStyleUsed>   m_arStyleUsed;

        std::wstring m_sStyle;
        std::wstring m_sId;

        void CombineStandardStyles(const std::vector<std::wstring>& arStandartedStyles, CXmlElement& oElement);
        void CreateStandardStyle  (const std::wstring& sNameStyle, CXmlElement& oElement);
        void ConvertStyle         (const NSCSS::CCompiledStyle& oStyle, CXmlElement& oElement, bool bIsPStyle);

        void SetRStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);
        void SetPStyle(const NSCSS::CCompiledStyle& oStyle, CXmlElement& oXmlElement);

    public:
        CDocumentStyle();
        ~CDocumentStyle();

        void WritePStyle(const NSCSS::CCompiledStyle& oStyle);
        void WriteRStyle(const NSCSS::CCompiledStyle& oStyle);
        void WriteLitePStyle(const NSCSS::CCompiledStyle& oStyle);
        void WriteLiteRStyle(const NSCSS::CCompiledStyle& oStyle);

        void SetStyle(const std::wstring& sStyle);
        void SetId   (const std::wstring& sId);

        std::wstring GetStyle() const;
        std::wstring GetIdAndClear();

        void Clear();
    };
}
#endif // CDOCUMENTSTYLE_H
