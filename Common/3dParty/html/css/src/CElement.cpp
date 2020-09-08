#include "CElement.h"
#include <iostream>
#include <algorithm>
#include <cwctype>

CElement::CElement() {}

CElement::~CElement()
{
    for (CElement* oChildren : m_arChildrens)
        delete oChildren;

    m_arChildrens.clear();
    m_arSelectors.clear();
    m_arDeclarations.clear();
}

/*
std::wstring CElement::GetText() const
{
    std::wstring sText;

    if(m_arSelectors.size() > 0)
    {
        for (const std::wstring& sSelector : m_arSelectors)
            sText += sSelector + L", ";

        sText.erase(sText.length() - 2, 2);
    }
    if (m_arDeclarations.size() != 0 ||
        m_arChildrens.size() != 0)
    {
        sText += L"{\n";

        if (m_arChildrens.size() != 0)
            for (const CElement* oChildren : m_arChildrens)
                sText += oChildren->GetText();

        for (const auto& pDeclaration : m_arDeclarations)
            sText += L"   " + pDeclaration.first + L": " + pDeclaration.second + L";\n";
        sText += L"};\n";
    }
    else
        sText += L";\n";
    return sText;
}
*/

void CElement::AddChildren(CElement *oChildren)
{
    m_arChildrens.push_back(oChildren);
}

void CElement::AddSelector(const std::wstring& sSelector)
{
    std::wstring sTemp(sSelector);
    sTemp.erase(std::remove_if(sTemp.begin(), sTemp.end(), [] (wchar_t ch) { return std::iswspace(ch); }), sTemp.end());
    m_arSelectors.push_back(sTemp);
}

/*
void CElement::AddDeclaration(const std::pair<std::wstring, std::wstring>& pDeclaration)
{
    m_arDeclarations.insert(pDeclaration);
}
*/

void CElement::AddSelectors(const std::vector<std::wstring>& arSelectors)
{
    m_arSelectors.insert(m_arSelectors.end(), arSelectors.begin(), arSelectors.end());
}

void CElement::AddDeclarations(const std::map<std::wstring, std::wstring>& arDeclarations)
{
    m_arDeclarations.insert(arDeclarations.begin(), arDeclarations.end());
}

/*
void CElement::SetDeclaratins(const std::map<std::wstring, std::wstring>& arDeclarations)
{
    m_arDeclarations = arDeclarations;
}
*/

int CElement::GetCountSelectors() const
{
    return m_arSelectors.size();
}

int CElement::GetCountDeclarations() const
{
    return m_arDeclarations.size();
}

/*
int CElement::GetCountChildrens() const
{
    return m_arChildrens.size();
}
*/

bool CElement::FindSelector(const std::wstring& sSelector) const
{
    return std::find(m_arSelectors.begin(), m_arSelectors.end(), sSelector) != m_arSelectors.cend();
}

/*
std::vector<std::wstring> CElement::GetSelectors() const
{
    return m_arSelectors;
}

std::map<std::wstring, std::wstring> CElement::GetDeclarations() const
{
    return m_arDeclarations;
}
*/

std::map<std::wstring, std::map<std::wstring, std::wstring>> CElement::GetDeclarations(const std::wstring& sSelector, const std::vector<std::wstring>& arParents) const
{
    std::map<std::wstring, std::map<std::wstring, std::wstring>> arElement;

    if(std::find(m_arSelectors.begin(), m_arSelectors.end(), sSelector) != m_arSelectors.end())
    {
        std::wstring sTempSelectors;
        for (const std::wstring& sParent : arParents)
            sTempSelectors += sParent;
        sTempSelectors = sTempSelectors.empty() ? sSelector : sTempSelectors + L" -> " + sSelector;
        arElement.insert(std::make_pair(sTempSelectors, m_arDeclarations));
    }
    for (const CElement* oElement : m_arChildrens)
    {
        const std::vector<std::wstring>& sSelectors = oElement->m_arSelectors;
        if (std::find(sSelectors.begin(), sSelectors.end(), sSelector) != sSelectors.cend())
        {
            const std::map<std::wstring, std::map<std::wstring, std::wstring>>& TempArElement = oElement->GetDeclarations(sSelector, m_arSelectors);
            arElement.insert(TempArElement.begin(), TempArElement.end());
        }
    }
    return arElement;
}

/*
std::vector<CElement*> CElement::GetChildrens() const
{
    return m_arChildrens;
}
*/

CElement& CElement::operator= (const CElement &oElement)
{
    m_arChildrens.clear();
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
