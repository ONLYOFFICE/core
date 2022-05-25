#ifndef CCSSCALCULATOR_H
#define CCSSCALCULATOR_H

#include "CssCalculator_global.h"
#include "CCompiledStyle.h"
#include "ConstValues.h"
#include <iostream>
#include <map>
#include <vector>

namespace NSCSS
{
    class CCssCalculator_Private;
    class CSSCALCULATOR_EXPORT CCssCalculator
    {
    protected:
        CCssCalculator_Private* m_pInternal;
    public:
        CCssCalculator();
        ~CCssCalculator();

        CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const bool& bIsSettings = false, const UnitMeasure& unitMeasure = Default) const;

        // void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles        (const std::string&  sStyle);
        void AddStyles        (const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(const unsigned short int& nValue);
        void SetBodyTree(const CTree &oTree);

        void SetSizeSourceWindow(const CSizeWindow& oSizeWindow);
        void SetSizeDeviceWindow(const CSizeWindow& oSizeWindow);

        CSizeWindow GetSizeSourceWindow() const;
        CSizeWindow GetSizeDeviceWindow() const;

        UnitMeasure GetUnitMeasure() const;
        std::wstring GetEncoding()   const;
        unsigned short int GetDpi()  const;

        void Clear();
    };
}

#endif // CCSSCALCULATOR_H
