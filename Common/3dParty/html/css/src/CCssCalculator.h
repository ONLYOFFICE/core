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

        bool operator< (const CNode& oNode) const
        {
            return (m_sId.length() + m_sName.length() + m_sClass.length() + m_sStyle.length())
                    <
                   (oNode.m_sId.length() + oNode.m_sName.length() + oNode.m_sClass.length() + oNode.m_sStyle.length());
        }

        bool operator== (const CNode& oNode) const
        {
            return((m_sId == oNode.m_sId) &&
                   (m_sName == oNode.m_sName) &&
                   (m_sClass == oNode.m_sClass) &&
                   (m_sStyle == oNode.m_sStyle));
        }
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

        CCompiledStyle GetCompiledStyle(const std::vector<std::string>& arSelectors, const UnitMeasure unitMeasure = Default) const;
        CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure = Default) const;

        void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStyles(const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(const int& nValue);

        const UnitMeasure& GetUnitMeasure() const;
        const std::wstring& GetEncoding() const;
        const int& GetDpi() const;

        void Print() const;
        void Clear();
    };
}

#endif // CCSSCALCULATOR_H
