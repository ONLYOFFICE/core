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
        Default = 0,
        Pixel,
        Point,
        Cantimeter,
        Millimeter,
        Inch,
        Peak
    } UnitMeasure;

    struct CNode
    {
        std::wstring m_sName;  // Имя тэга
        std::wstring m_sClass; // Класс тэга
        std::wstring m_sId;    // Id тэга
        std::wstring m_sStyle; // Стиль тэга

        CNode(){};
        CNode(std::wstring sName, std::wstring sClass, std::wstring sId) : m_sName(sName), m_sClass(sClass), m_sId(sId){};

        bool Empty() const
        {
            return m_sName.empty() && m_sClass.empty() && m_sId.empty() && m_sStyle.empty();
        }

        std::vector<std::wstring> GetData() const
        {
            std::vector<std::wstring> arValues;
            if (!m_sClass.empty())
                arValues.push_back(m_sClass);
            if (!m_sName.empty())
                arValues.push_back(m_sName);
            return arValues;
        }

        bool operator< (const CNode& oNode) const
        {
            if(m_sName != oNode.m_sName)
                return m_sName < oNode.m_sName;

            if(m_sClass != oNode.m_sClass)
                return m_sClass < oNode.m_sClass;

            if(m_sId != oNode.m_sId)
                return m_sId < oNode.m_sId;

            if(m_sStyle != oNode.m_sStyle)
                return m_sStyle < oNode.m_sStyle;

            return false;
        }

        bool operator== (const CNode& oNode) const
        {
            return((m_sId == oNode.m_sId)       &&
                   (m_sName == oNode.m_sName)   &&
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

        CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure = Default) const;

        // void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles        (const std::string&  sStyle);
        void AddStyles        (const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(const unsigned short int& nValue);

        UnitMeasure GetUnitMeasure() const;
        std::wstring GetEncoding()   const;
        unsigned short int GetDpi()  const;

        void Clear();
    };
}

#endif // CCSSCALCULATOR_H
