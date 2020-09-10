#include "CElement.h"

#include <iostream>
#include <algorithm>

CElement::CElement(std::wstring sNameElement, std::wstring sValue, std::wstring sNamespace, bool bNamespaceArguments)
    : m_sValue(sValue),
      m_sNameElement(sNameElement),
      m_sNamespace(sNamespace),
      m_bNamespaceArguments(bNamespaceArguments)
{
    m_bEmpty = true;
}

CElement::~CElement()
{
    Clear();
}

bool CElement::IsEmpty()
{
    return m_bEmpty;
}

int CElement::GetCountChildrens()
{
    return (int)m_arChildren.size();
}

int CElement::GetCountArguments()
{
    return (int)m_arArguments.size();
}

CElement* CElement::GetChildren(const int nIndex)
{
    if (nIndex < 0 || nIndex > GetCountChildrens())
        return NULL;
    return m_arChildren[nIndex];
}

std::pair<std::wstring, std::wstring>* CElement::GetArgument(const int nIndex)
{
    if (nIndex < 0 || nIndex > GetCountChildrens())
        return NULL;
    return &m_arArguments[nIndex];
}

std::wstring CElement::GetElementXML()
{
    std::wstring sText = L"<";

    if (m_sNamespace != L"")
        sText += m_sNamespace + L":";

    sText += m_sNameElement;

    sText += GetArguments();
    if (m_arClosingTag[m_sNameElement] == false)
        return (sText + L"/>");

    sText += L">";

    sText += m_sValue;

    for (int i = 0; i <(int)m_arChildren.size(); i++)
        sText += m_arChildren[i]->GetElementXML();

    sText += L"</";
    if (m_sNamespace != L"")
        sText += m_sNamespace + L":";

    sText += m_sNameElement + L">";
    return sText;
}

std::wstring CElement::GetArguments()
{
    if (m_arArguments.size() <= 0)
        return L"";

    std::wstring sText;

    for (int i = 0; i < (int)m_arArguments.size(); i++)
    {
        sText += L" ";

        if (m_sNamespace != L"" &&
            std::find(m_arNamespaceBan.begin(), m_arNamespaceBan.end(), m_sNamespace) == std::end(m_arNamespaceBan) &&
            m_bNamespaceArguments)
            sText += m_sNamespace + L":";

        sText += m_arArguments[i].first + L"=\"" + m_arArguments[i].second + L"\"";
    }
    return sText;
}

void CElement::AddArgument(std::wstring sNameArgument, std::wstring sValueArgument)
{
    m_bEmpty = false;

    m_arArguments.push_back(std::make_pair(sNameArgument, sValueArgument));
}

void CElement::DeleteChildren(CElement *oChildren)
{
    if (oChildren == nullptr)
        return;
    m_arChildren.erase(std::remove(m_arChildren.begin(), m_arChildren.end(), oChildren), m_arChildren.end());
}

void CElement::DeleteChildren(std::wstring sNameChildren)
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        if (m_arChildren[i]->GetName() == sNameChildren)
        {
            DeleteChildren(m_arChildren[i]);
            return;
        }
    }
}

void CElement::DeleteArgument(std::wstring sNameArgument)
{
    for (auto iter = m_arArguments.begin(); iter != m_arArguments.end(); iter++)
        if (iter->first == sNameArgument)
        {
            m_arArguments.erase(iter);
            return;
        }
}

void CElement::Clear(bool Children, bool NameElement, bool Value, bool Arguments)
{
    if (Children)
    {
        for (CElement* element : m_arChildren)
            delete element;
        m_arChildren.clear();
    }
    if (NameElement)
        m_sNameElement = L"";
    if (Value)
        m_sValue = L"";
    if (Arguments)
        m_arArguments.clear();

    m_bEmpty = true;
}

void CElement::EditBoolNamespace(bool bNamespace)
{
    m_bNamespaceArguments = bNamespace;
}

void CElement::AddChildren(CElement* oChildren, int nIndex)
{
    m_bEmpty = false;

    if (nIndex < 0 || nIndex > (int)m_arChildren.size())
        m_arChildren.push_back(oChildren);
    else
        m_arChildren.insert(m_arChildren.begin() + nIndex, oChildren);
}

void CElement::setValue(std::wstring sValue)
{
    m_sValue = sValue;
    m_bEmpty = false;
}

void CElement::SetDefoult()
{
}

std::wstring CElement::GetName()
{
    return m_sNameElement;
}

std::wstring CElement::GetValue()
{
    return  m_sValue;
}
