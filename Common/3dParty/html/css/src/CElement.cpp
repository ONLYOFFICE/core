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

    std::wstring CElement::GetFullSelector() const
    {
        return m_sFullSelector;
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
        for (std::pair<std::wstring, std::wstring> pPropertie : mProperties)
        {
            std::transform(pPropertie.first.begin(), pPropertie.first.end(), pPropertie.first.begin(), tolower);
            std::transform(pPropertie.second.begin(), pPropertie.second.end(), pPropertie.second.begin(), tolower);

            if (!m_mStyle[pPropertie.first].empty() && m_mStyle[pPropertie.first].find(L'!') != std::wstring::npos)
                continue;

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

        m_arPrevElements.push_back(oPrevElement);
        oPrevElement->m_sFullSelector += L' ' + m_sFullSelector;
    }

    void CElement::AddKinElement(CElement *oKinElement)
    {
        if (NULL == oKinElement)
            return;

        m_arKinElements.push_back(oKinElement);
        oKinElement->m_sFullSelector += m_sFullSelector;
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
        if (arNodes.empty() || (m_mStyle.empty() && m_arPrevElements.empty()))
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
                        mStyle[pPropertie.first] = pPropertie.second;
//                    const std::map<std::wstring, std::wstring> mTempStyle = oElement->GetFullStyle((arNodes.size() > 1) ? std::vector<std::wstring>(&arNodes[1], &arNodes[arNodes.size() - 1]) : std::vector<std::wstring>());
//                    mStyle.insert(mTempStyle.begin(), mTempStyle.end());
                }
            }
        }

        return mStyle;
    }

    std::vector<CElement *> CElement::GetAllElements(const std::vector<std::wstring> &arNodes) const
    {
        if (arNodes.empty())
            return std::vector<CElement*>({const_cast<CElement*>(this)});

        std::vector<CElement *> arElements;

        if (!m_mStyle.empty())
            arElements.push_back(const_cast<CElement*>(this));

        std::vector<std::wstring> arKins;
        for (std::vector<std::wstring>::const_reverse_iterator sNode = arNodes.rbegin(); sNode != arNodes.rend(); ++sNode)
        {
            arKins.push_back(*sNode);
            if ((*sNode)[0] != L'#' && (*sNode)[0] != L'.')
                break;
        }

        for (const CElement* oElement : m_arKinElements)
        {
            for (std::vector<std::wstring>::const_reverse_iterator sNode = arKins.rbegin(); sNode != arKins.rend(); ++sNode)
            {
                if (oElement->m_sSelector == *sNode)
                {
                    std::vector<std::wstring> arWords = arNodes;
                        arWords.pop_back();
                    const std::vector<CElement*> arTemp = oElement->GetAllElements(arWords);
                    arElements.insert(arElements.end(), arTemp.begin(), arTemp.end());
                }
            }
        }

        for (const CElement* oElement : m_arPrevElements)
        {
            for (std::vector<std::wstring>::const_reverse_iterator sNode = arNodes.rbegin(); sNode != arNodes.rend(); ++sNode)
            {
                if (oElement->m_sSelector == *sNode)
                {
                    std::vector<std::wstring> arWords = arNodes;
                        arWords.pop_back();

                    const std::vector<CElement*> arTemp = oElement->GetAllElements(arWords);
                    arElements.insert(arElements.end(), arTemp.begin(), arTemp.end());
                }
            }
        }

        return arElements;
    }

    std::vector<CElement *> CElement::GetNextOfKin(const std::wstring &sName, const std::vector<std::wstring>& arClasses) const
    {
        if (m_arKinElements.empty())
            return std::vector<CElement*>();

        std::vector<CElement*> arElements;
        for (CElement* oElement : m_arKinElements)
        {
            if (!sName.empty() && oElement->m_sSelector == sName)
            {
                arElements.push_back(oElement);
                continue;
            }

            for (const std::wstring& sClass : arClasses)
            {
                if (oElement->m_sSelector == sClass)
                    arElements.push_back(oElement);
            }
        }
        return arElements;
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

    std::vector<unsigned short> CElement::GetWeight()
    {
        if (m_arWeight.empty())
            m_arWeight = NS_STATIC_FUNCTIONS::GetWeightSelector(m_sFullSelector);

        return m_arWeight;
    }

    void CElement::IncreasedWeight()
    {
        if (!m_arWeight.empty())
        {
            m_arWeight = NS_STATIC_FUNCTIONS::GetWeightSelector(m_sFullSelector);
            ++m_arWeight[3];
        }
    }

    void CElement::Print() const
    {
        std::wcout << L"Selector: " << m_sSelector << std::endl;
        std::wcout << L"Full Selector: " << m_sFullSelector << std::endl;
        std::wcout << L"===========STYLE(" << m_mStyle.size() << ")===========" << std::endl;
        for (const std::pair<std::wstring, std::wstring>& oPropertie : m_mStyle)
            std::wcout << oPropertie.first << L" - " << oPropertie.second << std::endl;
        std::wcout << L"===========================" << std::endl;

        std::wcout << m_arKinElements.size() << L" --- " << m_arPrevElements.size() << std::endl;

        if (!m_arKinElements.empty())
        {
            std::wcout << L"\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/" << std::endl;
            for (CElement* oElement : m_arKinElements)
                oElement->Print();
            std::wcout << L"/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\/\\" << std::endl;
        }
        for (CElement* oElement : m_arPrevElements)
            oElement->Print();

    }

}

