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

        CSizeWindow m_oSourceWindow;
        CSizeWindow m_oDeviceWindow;

        std::wstring ConvertUnitMeasure(const std::wstring& sValue, const float& fPreviousValue, ScalingDirection enScalingDirection = ScalingDirectionNone) const;

        void Scale(int &nValue, ScalingDirection enScalingDirection) const;

        int ConvertPx(const std::wstring& sValue) const;
        int ConvertPxToCm(const float& dValue) const;
        int ConvertPxToIn(const float& dValue) const;
        int ConvertPxToMm(const float& dValue) const;
        int ConvertPxToPt(const float& dValue) const;
        int ConvertPxToPc(const float& dValue) const;

        int ConvertCm(const std::wstring& sValue) const;
        int ConvertCmToPx(const float& dValue) const;
        int ConvertCmToIn(const float& dValue) const;
        int ConvertCmToMm(const float& dValue) const;
        int ConvertCmToPt(const float& dValue) const;
        int ConvertCmToPc(const float& dValue) const;

        int ConvertMm(const std::wstring& sValue) const;
        int ConvertMmToPx(const float& dValue) const;
        int ConvertMmToIn(const float& dValue) const;
        int ConvertMmToCm(const float& dValue) const;
        int ConvertMmToPt(const float& dValue) const;
        int ConvertMmToPc(const float& dValue) const;

        int ConvertIn(const std::wstring& sValue) const;
        int ConvertInToPx(const float& dValue) const;
        int ConvertInToMm(const float& dValue) const;
        int ConvertInToCm(const float& dValue) const;
        int ConvertInToPt(const float& dValue) const;
        int ConvertInToPc(const float& dValue) const;

        int ConvertPt(const std::wstring& sValue) const;
        int ConvertPtToPx(const float& dValue) const;
        int ConvertPtToMm(const float& dValue) const;
        int ConvertPtToCm(const float& dValue) const;
        int ConvertPtToIn(const float& dValue) const;
        int ConvertPtToPc(const float& dValue) const;

        int ConvertPc(const std::wstring& sValue) const;
        int ConvertPcToPx(const float& dValue) const;
        int ConvertPcToMm(const float& dValue) const;
        int ConvertPcToCm(const float& dValue) const;
        int ConvertPcToIn(const float& dValue) const;
        int ConvertPcToPt(const float& dValue) const;

    public:

        NSConstValues::NSCssProperties::Font        m_pFont;
        NSConstValues::NSCssProperties::Margin      m_pMargin;
        NSConstValues::NSCssProperties::Background  m_pBackground;
        NSConstValues::NSCssProperties::Text        m_pText;
        NSConstValues::NSCssProperties::Border      m_pBorder;
        NSConstValues::NSCssProperties::Display     m_pDisplay;

        CCompiledStyle();
        explicit CCompiledStyle(const std::map<std::wstring, std::wstring>& mStyle);
        CCompiledStyle(const CCompiledStyle& oStyle);

        ~CCompiledStyle();

        void SetDpi(const unsigned short& uiDpi);
        void SetUnitMeasure(const UnitMeasure& enUnitMeasure);
        void SetSizeSourceWindow(const CSizeWindow& oSizeWindow);
        void SetSizeDeviceWindow(const CSizeWindow& oSizeWindow);

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
