#include "CElement.h"
#include <algorithm>

namespace NSCSS
{
    CElement::CElement()
    {

    }
    CElement::~CElement()
    {
        for (CElement* oElement : m_arPrevElements)
            if (oElement != NULL)
                delete oElement;
            else
                continue;

        m_mStyle.clear();

    }

    std::wstring CElement::GetSelector() const
    {
        return m_sSelector;
    }

    void CElement::SetSelector(const std::wstring &sSelector)
    {
        m_sSelector = sSelector;
    }

    void NSCSS::CElement::AddPropertie(const std::wstring &sName, const std::wstring& sValue)
    {
        m_mStyle[sName] = sValue;
    }

    void CElement::AddProperties(const std::map<std::wstring, std::wstring>& mProperties)
    {
        for (const std::pair<std::wstring, std::wstring>& pPropertie : mProperties)
            m_mStyle[pPropertie.first] = pPropertie.second;
    }

    void CElement::AddPrevElement(CElement *oPrevElement)
    {
        if (NULL == oPrevElement)
            return;

        if (std::find_if(m_arPrevElements.begin(), m_arPrevElements.end(),
                         [oPrevElement](CElement* oElement){ return  oElement->m_sSelector == oPrevElement->m_sSelector;}) == m_arPrevElements.end())
        {
            m_arPrevElements.push_back(oPrevElement);
        }
    }

    std::map<std::wstring, std::wstring> CElement::GetStyle() const
    {
        return m_mStyle;
    }

}

