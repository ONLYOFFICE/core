#include "CDocumentStyle.h"

namespace NSCSS
{
    CDocumentStyle::CDocumentStyle()
    {
    }

    CDocumentStyle::~CDocumentStyle()
    {
    }

    CDocumentStyle::CDocumentStyle(NSCSS::CCompiledStyle& oStyle)
    {
        WriteStyle(oStyle);
    }

    std::wstring CDocumentStyle::GetStyle()
    {
        return m_sStyle;
    }

    void CDocumentStyle::WriteStyle(NSCSS::CCompiledStyle &oStyle)
    {
        // пофиксить появление пробелов
        std::wstring sId = oStyle.GetId();
        if (sId.find(L" ") != std::wstring::npos)
            sId = sId.substr(0, sId.find(L" "));

        m_sStyle = L"<w:style w:styleId=\"" + sId + L"\" w:type=\"paragraph\">";

        m_sStyle += L"<w:name w:val=\"" + sId + L"\"/>";

        m_sStyle += L"<w:pPr>";

        if (!oStyle.GetColor().empty())
            m_sStyle += L"<w:color w:val=\"" + oStyle.GetColor() + L"\"/>";

        if (!oStyle.GetFontSize().empty())
        {
            std::wstring sFontSize = oStyle.GetFontSize();
            if (isdigit(sFontSize[0]))
                m_sStyle += L"<w:sz w:val=\"" + oStyle.GetFontSize() + L"\"/>";
        }
        if (!oStyle.GetFontFamily().empty())
        {
            std::wstring sFontFamily = oStyle.GetFontFamily();
            m_sStyle += L"<w:rFonts w:ascii=\"" + sFontFamily + L"\" w:hAnsi=\"" + sFontFamily + L"\"/>";
        }

        if (!oStyle.GetTextAlign().empty())
        {
            std::wstring sTextAlign = oStyle.GetTextAlign();
            if (sTextAlign != L"justify")
                m_sStyle += L"<w:jc w:val=\"" + oStyle.GetTextAlign() + L"\"/>";
            else if (sTextAlign == L"justify")
                m_sStyle += L"<w:jc w:val=\"both\"/>";
        }
        if (!oStyle.GetFontStyle().empty())
        {
            std::wstring sFontStyle = oStyle.GetFontStyle();
            if (sFontStyle == L"italic")
                m_sStyle += L"<w:i w:val=\"true\"/>";
        }

        if (!oStyle.GetFontWeight().empty())
        {
            std::wstring sFontWeight = oStyle.GetFontWeight();
            if (sFontWeight == L"bold")
                m_sStyle += L"<w:b w:val=\"true\"/>";
        }

        if (!oStyle.GetTextIndent().empty())
            m_sStyle += L"<w:ind w:firstLine=\"" + oStyle.GetTextIndent() + L"\"/>";

        if (!oStyle.GetPadding().empty())
        {
            m_sStyle += L"<w:ind w:firstLine=\"" + oStyle.GetTextIndent() + L"\"/>";

        }
        if (!oStyle.GetTextDecoration().empty())
        {
            std::wstring sTextDecoration = oStyle.GetTextDecoration();
            if (sTextDecoration == L"underline")
                m_sStyle += L"<w:u w:val=\"single\"/>";
        }

        if (!oStyle.GetBackgroundColor().empty())
            m_sStyle += L"<w:background w:color=\"" + oStyle.GetBackgroundColor() + L"\"/>";

        m_sStyle += L"</w:pPr>";
        m_sStyle += L"</w:style>";
    }

}
