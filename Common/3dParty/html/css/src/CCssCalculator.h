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

        friend bool operator> (const CNode& oLeftNode, const CNode& oRightNode)
        {
            return (oLeftNode.m_sId.length() + oLeftNode.m_sName.length() + oLeftNode.m_sClass.length() + oLeftNode.m_sStyle.length())
                    >
                   (oRightNode.m_sId.length() + oRightNode.m_sName.length() + oRightNode.m_sClass.length() + oRightNode.m_sStyle.length());
        }

        friend bool operator< (const CNode& oLeftNode, const CNode& oRightNode)
        {
            return (oLeftNode.m_sId.length() + oLeftNode.m_sName.length() + oLeftNode.m_sClass.length() + oLeftNode.m_sStyle.length())
                    <
                   (oRightNode.m_sId.length() + oRightNode.m_sName.length() + oRightNode.m_sClass.length() + oRightNode.m_sStyle.length());
        }

        friend bool operator== (const CNode& oLeftNode, const CNode& oRightNode)
        {
            return (oLeftNode.m_sId == oRightNode.m_sId) &&
                   (oLeftNode.m_sName == oRightNode.m_sName) &&
                   (oLeftNode.m_sClass == oRightNode.m_sClass) &&
                   (oLeftNode.m_sStyle == oRightNode.m_sStyle);
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
        CCompiledStyle GetCompiledStyle(const CNode& oNode, const std::vector<CNode>& oParents = std::vector<CNode>(), const UnitMeasure& unitMeasure = Default) const;

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
