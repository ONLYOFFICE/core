#ifndef CCSSCALCULATOR_H
#define CCSSCALCULATOR_H

#include "CssCalculator_global.h"
#include "CCompiledStyle.h"
#include <iostream>
#include <map>
#include <vector>

namespace NSCSS
{
    typedef enum {
        Default,
        Cantimeter,
        Inch,
        Millimeter,
        Pixel,
        Point,
        Peak
    } UnitMeasure;

    class CCssCalculator_Private;
    class CSSCALCULATOR_EXPORT CCssCalculator
    {
    protected:
        CCssCalculator_Private* m_pInternal;
    public:
        CCssCalculator();
        ~CCssCalculator();

        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(const std::wstring& sSelector);

        CCompiledStyle GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure = Default);

        void AddStyle(std::vector<std::string> sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure nType);
        void SetDpi(const int nValue);

        UnitMeasure GetUnitMeasure();
        std::wstring GetEncoding();
        int GetDpi();

        void Print();
        void Clear();
    };
}

#endif // CCSSCALCULATOR_H
