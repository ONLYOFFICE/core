#ifndef CCOMPILEDSTYLE_H
#define CCOMPILEDSTYLE_H

#include "CssCalculator_global.h"
#include "ConstValues.h"

#include <map>
#include <set>
#include <vector>
#include <string>

namespace NSCSS
{
    class CSSCALCULATOR_EXPORT CCompiledStyle
    {
        std::set<std::wstring> m_arParentsStyles;
        std::wstring m_sId;

        unsigned short int m_nDpi;
        UnitMeasure m_UnitMeasure;

        std::wstring ConvertUnitMeasure(const std::wstring& sValue, const float& fPreviousValue) const;

        std::wstring ConvertPx(const std::wstring& sValue) const;
        std::wstring ConvertPxToCm(const float& dValue) const;
        std::wstring ConvertPxToIn(const float& dValue) const;
        std::wstring ConvertPxToMm(const float& dValue) const;
        std::wstring ConvertPxToPt(const float& dValue) const;
        std::wstring ConvertPxToPc(const float& dValue) const;

        std::wstring ConvertCm(const std::wstring& sValue) const;
        std::wstring ConvertCmToPx(const float& dValue) const;
        std::wstring ConvertCmToIn(const float& dValue) const;
        std::wstring ConvertCmToMm(const float& dValue) const;
        std::wstring ConvertCmToPt(const float& dValue) const;
        std::wstring ConvertCmToPc(const float& dValue) const;

        std::wstring ConvertMm(const std::wstring& sValue) const;
        std::wstring ConvertMmToPx(const float& dValue) const;
        std::wstring ConvertMmToIn(const float& dValue) const;
        std::wstring ConvertMmToCm(const float& dValue) const;
        std::wstring ConvertMmToPt(const float& dValue) const;
        std::wstring ConvertMmToPc(const float& dValue) const;

        std::wstring ConvertIn(const std::wstring& sValue) const;
        std::wstring ConvertInToPx(const float& dValue) const;
        std::wstring ConvertInToMm(const float& dValue) const;
        std::wstring ConvertInToCm(const float& dValue) const;
        std::wstring ConvertInToPt(const float& dValue) const;
        std::wstring ConvertInToPc(const float& dValue) const;

        std::wstring ConvertPt(const std::wstring& sValue) const;
        std::wstring ConvertPtToPx(const float& dValue) const;
        std::wstring ConvertPtToMm(const float& dValue) const;
        std::wstring ConvertPtToCm(const float& dValue) const;
        std::wstring ConvertPtToIn(const float& dValue) const;
        std::wstring ConvertPtToPc(const float& dValue) const;

        std::wstring ConvertPc(const std::wstring& sValue) const;
        std::wstring ConvertPcToPx(const float& dValue) const;
        std::wstring ConvertPcToMm(const float& dValue) const;
        std::wstring ConvertPcToCm(const float& dValue) const;
        std::wstring ConvertPcToIn(const float& dValue) const;
        std::wstring ConvertPcToPt(const float& dValue) const;

    public:

        NSConstValues::NSCssProperties::Font        m_pFont;
        NSConstValues::NSCssProperties::Margin      m_pMargin;
        NSConstValues::NSCssProperties::Background  m_pBackground;
        NSConstValues::NSCssProperties::Text        m_pText;
        NSConstValues::NSCssProperties::Border      m_pBorder;

        CCompiledStyle();
        explicit CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle);
        CCompiledStyle(const CCompiledStyle& oStyle);

        ~CCompiledStyle();

        void SetDpi(const unsigned short& uiDpi);
        void SetUnitMeasure(const UnitMeasure& enUnitMeasure);

        bool Empty() const;

        void AddPropSel(const std::wstring& sProperty, const std::wstring& sValue, const unsigned int unLevel, const bool& bHardMode = true);
        void AddStyle(const std::map<std::wstring, std::wstring>& mStyle, const unsigned int unLevel, const bool& bHardMode = true);
        void AddStyle(const std::wstring& sStyle, const unsigned int unLevel, const bool& bHardMode = false);
        void AddParent(const std::wstring& sParentName);

        std::vector<std::wstring> GetParentsName() const;

        void SetID(const std::wstring& sId);
        std::wstring GetId() const;

        void ClearImportants();

        CCompiledStyle& operator+= (const CCompiledStyle& oElement);
        CCompiledStyle& operator=  (const CCompiledStyle& oElement);
        bool operator==            (const CCompiledStyle& oElement) const;

        static void StyleEquation(CCompiledStyle& oFirstStyle, CCompiledStyle& oSecondStyle);
    };
}

#endif // CCOMPILEDSTYLE_H
