#ifndef CELEMENT_H
#define CELEMENT_H

#include <map>
#include <vector>
#include "CCssCalculator.h"

namespace NSCSS
{
    class CElement
    {
        std::map<std::wstring, std::wstring> m_mStyle;

        std::vector<CElement*> m_arPrevElements;

        std::wstring m_sSelector;

    public:
        CElement();
        ~CElement();

        std::wstring GetSelector() const;

        void SetSelector(const std::wstring& sSelector);
        void AddPropertie(const std::wstring& sName, const std::wstring& sValue);
        void AddProperties(const std::map<std::wstring, std::wstring>& mProperties);
        void AddPrevElement(CElement* oPrevElement);

        size_t EmptyPrevElements() const;

        std::map<std::wstring, std::wstring> GetStyle() const;
        std::map<std::wstring, std::wstring> GetFullStyle(const std::vector<CNode>& arSelectors) const;
        std::map<std::wstring, std::wstring> GetConvertStyle(const std::vector<CNode>& arNodes) const;

        CElement *FindPrevElement(const std::wstring& sSelector) const;

        void Print() const;
    };
}


#endif // CELEMENT_H
