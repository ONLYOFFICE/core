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

    size_t CElement::EmptyPrevElements() const
    {
        return m_arPrevElements.empty();
    }

    std::map<std::wstring, std::wstring> CElement::GetStyle() const
    {
        return m_mStyle;
    }

    std::map<std::wstring, std::wstring> CElement::GetFullStyle(const std::vector<CNode>& arSelectors) const
    {
        if (arSelectors.empty())
            return std::map<std::wstring, std::wstring>();

        std::map<std::wstring, std::wstring> mStyle;
        for (const CNode& oNode : arSelectors)
        {
            const CElement *oElement = FindPrevElement(oNode.m_sName);
            if (NULL != oElement)
            {
                std::wcout << L"***" << oElement->GetSelector() << std::endl;
                const std::map<std::wstring, std::wstring>& mTempMap = oElement->GetStyle();
                mStyle.insert(mTempMap.begin(), mTempMap.end());
            }
        }
        return m_mStyle;
    }

    CElement *CElement::FindPrevElement(const std::wstring &sSelector) const
    {
        if (sSelector.empty())
            return NULL;

        for (CElement* oElement : m_arPrevElements)
        {
            if (oElement->m_sSelector == sSelector)
                return oElement;
        }
        return NULL;
    }

    void CElement::Print() const
    {
        std::wcout << L"Selector: " << m_sSelector << std::endl;
        std::wcout << L"===========STYLE(" << m_mStyle.size() << ")===========" << std::endl;
        for (const std::pair<std::wstring, std::wstring>& oPropertie : m_mStyle)
            std::wcout << oPropertie.first << L" - " << oPropertie.second << std::endl;
        std::wcout << L"===========================" << std::endl;

        for (CElement* oElement : m_arPrevElements)
            oElement->Print();
    }

}

