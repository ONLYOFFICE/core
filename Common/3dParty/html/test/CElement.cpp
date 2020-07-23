#include "CElement.h"
#include <iostream>

CElement::CElement()
{

}

CElement::~CElement()
{
    for (size_t i = 0; i < m_arChildrens.size(); i++)
        delete m_arChildrens[i];

    m_arChildrens.clear();
}

std::wstring CElement::GetText()
{
    std::wstring sText;
    for (size_t i =0 ; i < m_arSelectors.size(); i++)
        sText += m_arSelectors[i] + L" ";
    if (m_arDeclarations.size() != 0 ||
        m_arChildrens.size() != 0)
    {
        sText += L"{\n";

        if (m_arChildrens.size() != 0)
            for (size_t i = 0; i < m_arChildrens.size(); i++)
                sText += m_arChildrens[i]->GetText();

        for (size_t i = 0; i < m_arDeclarations.size(); i++)
        {
            sText += L"   " + m_arDeclarations[i].first + L": " + m_arDeclarations[i].second + L";\n";
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

void CElement::AddSelector(std::wstring sSelector)
{
    m_arSelectors.push_back(sSelector);
}

void CElement::AddDeclaration(std::pair<std::wstring, std::wstring> pDeclaration)
{
    m_arDeclarations.push_back(pDeclaration);
}

void CElement::AddSelectors(std::vector<std::wstring> arSelectors)
{
    for (size_t i = 0; i < arSelectors.size(); i++)
        m_arSelectors.push_back(arSelectors[i]);
}

void CElement::AddDeclarations(std::vector<std::pair<std::wstring, std::wstring> > arDeclarations)
{
    for (size_t i = 0; i < arDeclarations.size(); i++)
        m_arDeclarations.push_back(arDeclarations[i]);
}

int CElement::GetCountSelectors()
{
    return m_arSelectors.size();
}

int CElement::GetCountDeclarations()
{
    return m_arDeclarations.size();
}

int CElement::GetCountChildrens()
{
    return m_arChildrens.size();
}

std::vector<std::wstring> CElement::GetSelectors()
{
    return m_arSelectors;
}

std::vector<std::pair<std::wstring, std::wstring>> CElement::GetDeclarations()
{
    return m_arDeclarations;
}

std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CElement::GetDeclarations(std::wstring sSelector,
                                                                                                                   std::vector<std::wstring> pParent)
{
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arElement;
    std::vector<std::wstring> arSelectors = GetSelectors();

    for (size_t i = 0; i < arSelectors.size(); i++)
    {
        if (arSelectors[i] == sSelector)
        {
            std::wstring sTempSelectors;
            for (size_t j = 0; j < pParent.size(); j++)
                sTempSelectors += pParent[j];
            if (!sTempSelectors.empty())
                sTempSelectors += L" -> " + sSelector;
            else
                sTempSelectors = sSelector;
            arElement.push_back(std::make_pair(sTempSelectors, m_arDeclarations));
        }
    }

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> TempArElement;

    for (int i = 0; i < GetCountChildrens(); i++)
    {
        TempArElement = m_arChildrens[i]->GetDeclarations(sSelector, GetSelectors());
        for (size_t  j = 0; j < TempArElement.size(); j++)
            arElement.push_back(TempArElement[j]);
    }

    return arElement;
}

std::vector<CElement*> CElement::GetChildrens()
{
    return m_arChildrens;
}
