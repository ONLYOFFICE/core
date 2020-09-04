#include "CElement.h"
#include <iostream>
#include <algorithm>

CElement::CElement()
{

}

CElement::~CElement()
{
    for (CElement* oChildren : m_arChildrens)
        delete oChildren;

    m_arChildrens.clear();
}

std::wstring CElement::GetText() const
{
    std::wstring sText;

    if(m_arSelectors.size() > 0)
    {
        for (int i = 0; i < (int)(m_arSelectors.size() - 1); i++)
            sText += m_arSelectors[i] + L", ";

        sText += m_arSelectors[m_arSelectors.size() - 1];
    }
    if (m_arDeclarations.size() != 0 ||
        m_arChildrens.size() != 0)
    {
        sText += L"{\n";

        if (m_arChildrens.size() != 0)
            for (const CElement* oChildren : m_arChildrens)
                sText += oChildren->GetText();

        for (const std::pair<std::wstring, std::wstring>& pDeclaration : m_arDeclarations)
        {
            sText += L"   " + pDeclaration.first + L": " + pDeclaration.second + L";\n";
        }
        sText += L"};\n";
    }
    else
        sText += L";\n";
    return sText;
}

void CElement::AddChildren(CElement *oChildren)
{
    m_arChildrens.push_back(oChildren);
}

void CElement::AddSelector(const std::wstring sSelector)
{
    if (sSelector.find(L' ') != std::wstring::npos)
    {
        std::wstring sTempSelector;
        for (const wchar_t& wc : sSelector)
            if (!iswspace(wc))
                sTempSelector += wc;

        m_arSelectors.push_back(sTempSelector);
    }
    else
        m_arSelectors.push_back(sSelector);
}

void CElement::AddDeclaration(const std::pair<std::wstring, std::wstring> pDeclaration)
{
    m_arDeclarations.push_back(pDeclaration);
}

void CElement::AddSelectors(const std::vector<std::wstring> arSelectors)
{
    m_arSelectors.insert(m_arSelectors.end(), arSelectors.begin(), arSelectors.end());
}

void CElement::AddDeclarations(const std::vector<std::pair<std::wstring, std::wstring> > arDeclarations)
{
    m_arDeclarations.insert(m_arDeclarations.end(), arDeclarations.begin(), arDeclarations.end());
}

void CElement::SetDeclaratins(const std::vector<std::pair<std::wstring, std::wstring> > arDeclarations)
{
    m_arDeclarations = arDeclarations;
}

const int& CElement::GetCountSelectors() const
{
    return m_arSelectors.size();
}

const int& CElement::GetCountDeclarations() const
{
    return m_arDeclarations.size();
}

const int& CElement::GetCountChildrens() const
{
    return m_arChildrens.size();
}

bool CElement::FindSelector(std::wstring sSelector)
{
    if (std::find(m_arSelectors.begin(), m_arSelectors.end(), sSelector) != m_arSelectors.cend())
        return true;

    return false;
}

const std::vector<std::wstring>& CElement::GetSelectors() const
{
    return m_arSelectors;
}

const std::vector<std::pair<std::wstring, std::wstring>>& CElement::GetDeclarations() const
{
    return m_arDeclarations;
}

std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CElement::GetDeclarations(const std::wstring sSelector,
                                                                                                                   const std::vector<std::wstring> arParents) const
{
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arElement;

    for (const std::wstring& sValueSelector : m_arSelectors)
    {
        if (sValueSelector == sSelector)
        {
            std::wstring sTempSelectors;

            for (const std::wstring& sParent : arParents)
                sTempSelectors += sParent;

            if (!sTempSelectors.empty())
                sTempSelectors += L" -> " + sSelector;
            else
                sTempSelectors = sSelector;

            arElement.push_back(std::make_pair(sTempSelectors, m_arDeclarations));
        }
    }

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> TempArElement;

    for (const CElement* oElement : m_arChildrens)
    {
        const std::vector<std::wstring>& sSelectors = oElement->GetSelectors();

        if (std::find(sSelectors.begin(), sSelectors.end(), sSelector) != sSelectors.cend())
        {
            TempArElement = oElement->GetDeclarations(sSelector, GetSelectors());
            arElement.insert(arElement.end(), TempArElement.begin(), TempArElement.end());
            TempArElement.clear();
        }
    }

    return arElement;
}

std::vector<CElement*> CElement::GetChildrens() const
{
    return m_arChildrens;
}

CElement& CElement::operator= (const CElement &oElement)
{
    m_arChildrens.clear();
//    m_arSelectors.clear();
//    m_arDeclarations.clear();

    m_arSelectors = oElement.m_arSelectors;
    m_arDeclarations = oElement.m_arDeclarations;

    for (const CElement *oChildren : oElement.m_arChildrens)
    {
        CElement *oTempChildren = new CElement();
        *oTempChildren = *oChildren;
        m_arChildrens.push_back(oTempChildren);
    }

    return *this;
}
