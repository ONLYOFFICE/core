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

    struct CNode
    {
        std::wstring m_sName;  // Имя тэга
        std::wstring m_sId;    // Id тэга
        std::wstring m_sClass; // Класс тэга
        std::wstring m_sStyle; // Стиль тэга
    };

    class CCssCalculator_Private;
    class CSSCALCULATOR_EXPORT CCssCalculator
    {
    protected:
        CCssCalculator_Private* m_pInternal;
    public:
        CCssCalculator();
        ~CCssCalculator();

        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(const std::wstring& sSelector) const;

        CCompiledStyle GetCompiledStyle(const std::vector<std::string> arSelectors, const UnitMeasure unitMeasure = Default) const;
        CCompiledStyle GetCompiledStyle(const CNode& oNode, const std::vector<CNode>& oParents = std::vector<CNode>(), const UnitMeasure unitMeasure = Default) const;

        void AddStyle(const std::vector<std::string> sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStyles(const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure nType);
        void SetDpi(const int nValue);

        UnitMeasure GetUnitMeasure() const;
        std::wstring GetEncoding() const;
        int GetDpi() const;

        void Print() const;
        void Clear();
    };
}

#endif // CCSSCALCULATOR_H
