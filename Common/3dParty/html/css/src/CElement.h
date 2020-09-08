#ifndef CELEMENT_H
#define CELEMENT_H

#include <vector>
#include <string>
#include <map>

class CElement
{
    std::vector<CElement*> m_arChildrens;
    std::vector<std::wstring> m_arSelectors;
    std::map<std::wstring, std::wstring> m_arDeclarations;
public:
    CElement();
    ~CElement();

    void AddSelector(const std::wstring& sSelector);
    void AddSelectors(const std::vector<std::wstring>& arSelectors);
    void AddChildren(CElement* oChildren);
    // void AddDeclaration(const std::pair<std::wstring, std::wstring>& pDeclaration);
    void AddDeclarations(const std::map<std::wstring, std::wstring>& arDeclarations);

    // void SetDeclaratins(const std::map<std::wstring, std::wstring>& arDeclarations);

    int GetCountSelectors() const;
    int GetCountDeclarations() const;
    // int GetCountChildrens() const;

    bool FindSelector(const std::wstring& sSelector) const;

    // std::vector<std::wstring> GetSelectors() const;
    // std::map<std::wstring, std::wstring> GetDeclarations() const;
    std::map<std::wstring, std::map<std::wstring, std::wstring>> GetDeclarations(const std::wstring& sSelector, const std::vector<std::wstring>& arParents) const;
    // std::vector<CElement*> GetChildrens() const;
    // std::wstring GetText() const;

    CElement& operator= (const CElement& oElement);
};

#endif // CELEMENT_H
