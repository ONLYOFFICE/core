#include "CElement.h"
#include <algorithm>
#include <math.h>

#include "StaticFunctions.h"

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
        m_sFullSelector = m_sSelector;
    }

    void NSCSS::CElement::AddPropertie(const std::wstring &sName, const std::wstring& sValue)
    {
        m_mStyle[sName] = sValue;
    }

    void CElement::AddProperties(const std::map<std::wstring, std::wstring>& mProperties)
    {
        for (const std::pair<std::wstring, std::wstring>& pPropertie : mProperties)
        {
            if (pPropertie.second.substr(0, 3) == L"rgb")
                 m_mStyle[pPropertie.first] = NSCSS::NS_STATIC_FUNCTIONS::ConvertRgbToHex(pPropertie.second);
            else
                 m_mStyle[pPropertie.first] = pPropertie.second;
        }
    }

    void CElement::AddPrevElement(CElement *oPrevElement)
    {
        if (NULL == oPrevElement)
            return;

        switch (m_arPrevElements.size())
        {
            case 0:
            {
                m_arPrevElements.push_back(oPrevElement);
                oPrevElement->m_sFullSelector += m_sSelector;
                return;
            }
            case 1:
            {
                if (m_arPrevElements[0]->m_sSelector != oPrevElement->m_sSelector)
                {
                    m_arPrevElements.push_back(oPrevElement);
                    oPrevElement->m_sFullSelector += m_sSelector;
                }
                return;
            }
            case 2:
            {
                if (m_arPrevElements[0]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[1]->m_sSelector != oPrevElement->m_sSelector)
                {
                    m_arPrevElements.push_back(oPrevElement);
                    oPrevElement->m_sFullSelector += m_sSelector;
                }
                return;
            }
            case 3:
            {
                if (m_arPrevElements[0]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[1]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[2]->m_sSelector != oPrevElement->m_sSelector)
                {
                    m_arPrevElements.push_back(oPrevElement);
                    oPrevElement->m_sFullSelector += m_sSelector;
                }
                return;
            }
            case 4:
            {
                if (m_arPrevElements[0]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[1]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[2]->m_sSelector != oPrevElement->m_sSelector &&
                    m_arPrevElements[3]->m_sSelector != oPrevElement->m_sSelector)
                {
                    m_arPrevElements.push_back(oPrevElement);
                    oPrevElement->m_sFullSelector += m_sSelector;
                }
                return;
            }
            default:
            {
                if (std::find_if(m_arPrevElements.begin(), m_arPrevElements.end(),
                                 [oPrevElement](CElement* oElement){ return  oElement->m_sSelector == oPrevElement->m_sSelector;}) == m_arPrevElements.end())
                {
                    m_arPrevElements.push_back(oPrevElement);
                    oPrevElement->m_sFullSelector += m_sSelector;
                }
                return;
            }
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

        std::map<std::wstring, std::wstring> mStyle(m_mStyle);

//        Print();
//        std::wcout << L"++++++++++++++++++++++++++++++++++++++++" << std::endl;

        for (const CElement* oElement : m_arPrevElements)
        {
            for (std::vector<CNode>::const_reverse_iterator oNode = arSelectors.rbegin(); oNode != arSelectors.rend(); ++oNode)
            {
                std::map<std::wstring, std::wstring> mTempStyle;
                if (oElement->GetSelector() == L'.' + oNode->m_sClass ||
                    oElement->GetSelector() == oNode->m_sName)
                {
                    for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(arSelectors))
                        mStyle[pPropertie.first] = pPropertie.second;
//                    const std::map<std::wstring, std::wstring> mTempStyle = oElement->GetFullStyle(arSelectors);
//                    mStyle.insert(mTempStyle.begin(), mTempStyle.end());
                }
            }
        }

        return mStyle;
    }

    std::map<std::wstring, std::wstring> CElement::GetFullStyle(const std::vector<std::wstring> &arNodes) const
    {
        if (arNodes.empty())
            return std::map<std::wstring, std::wstring>();

//        Print();
//        std::wcout << L"++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::map<std::wstring, std::wstring> mStyle(m_mStyle);

        for (const CElement* oElement : m_arPrevElements)
        {
            for (std::vector<std::wstring>::const_reverse_iterator sNode = arNodes.rbegin(); sNode != arNodes.rend(); ++sNode)
            {
                if ((*sNode)[0] == L'.')
                {
                    for (const std::wstring& sClass : NS_STATIC_FUNCTIONS::GetWordsW(*sNode, L" "))
                    {
                        if (oElement->GetSelector() == sClass)
                        {
                            std::vector<std::wstring> sTempNodes = arNodes;
                            sTempNodes.pop_back();

//                            if (NS_STATIC_FUNCTIONS::GetWeightSelector(m_sSelector) <= NS_STATIC_FUNCTIONS::GetWeightSelector(oElement))
                            for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(sTempNodes))
                                mStyle[pPropertie.first] = pPropertie.second;
        //                    const std::map<std::wstring, std::wstring> mTempStyle = oElement->GetFullStyle((arNodes.size() > 1) ? std::vector<std::wstring>(&arNodes[1], &arNodes[arNodes.size() - 1]) : std::vector<std::wstring>());
        //                    mStyle.insert(mTempStyle.begin(), mTempStyle.end());
                        }
                    }
                }
                else if (oElement->GetSelector() == *sNode)
                {
                    std::vector<std::wstring> sTempNodes = arNodes;
                    sTempNodes.pop_back();
                    for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle(sTempNodes))
//                    for (const std::pair<std::wstring, std::wstring> pPropertie : oElement->GetFullStyle((arNodes.size() > 1) ? std::vector<std::wstring>(&arNodes[0], &arNodes[arNodes.size() - 2]) : std::vector<std::wstring>()))
                        mStyle[pPropertie.first] = pPropertie.second;
//                    const std::map<std::wstring, std::wstring> mTempStyle = oElement->GetFullStyle((arNodes.size() > 1) ? std::vector<std::wstring>(&arNodes[1], &arNodes[arNodes.size() - 1]) : std::vector<std::wstring>());
//                    mStyle.insert(mTempStyle.begin(), mTempStyle.end());
                }
            }
        }

        return mStyle;
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
        std::wcout << L"Full Selector: " << m_sFullSelector << std::endl;
        std::wcout << L"===========STYLE(" << m_mStyle.size() << ")===========" << std::endl;
        for (const std::pair<std::wstring, std::wstring>& oPropertie : m_mStyle)
            std::wcout << oPropertie.first << L" - " << oPropertie.second << std::endl;
        std::wcout << L"===========================" << std::endl;

        for (CElement* oElement : m_arPrevElements)
            oElement->Print();
    }

}

