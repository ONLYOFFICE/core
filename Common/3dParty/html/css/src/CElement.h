#ifndef CELEMENT_H
#define CELEMENT_H

#include <vector>
#include <string>

class CElement
{
    std::vector<CElement*> m_arChildrens;
    std::vector<std::wstring> m_arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> m_arDeclarations;
public:
    CElement();
    ~CElement();

    void AddSelector(const std::wstring& sSelector);
    void AddSelectors(const std::vector<std::wstring>& arSelectors);
    void AddChildren(CElement* oChildren);
    void AddDeclaration(const std::pair<std::wstring, std::wstring>& pDeclaration);
    void AddDeclarations(const std::vector<std::pair<std::wstring, std::wstring>>& arDeclarations);

    void SetDeclaratins(const std::vector<std::pair<std::wstring, std::wstring>>& arDeclarations);

    int GetCountSelectors() const;
    int GetCountDeclarations() const;
    int GetCountChildrens() const;

    bool FindSelector(std::wstring sSelector);

    std::vector<std::wstring> GetSelectors() const;
    std::vector<std::pair<std::wstring, std::wstring>> GetDeclarations() const;
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(const std::wstring sSelector,
                                                                                                             const std::vector<std::wstring> arParents) const;

    std::vector<CElement*> GetChildrens() const;

    std::wstring GetText() const;

    CElement& operator= (const CElement& oElement);
};

#endif // CELEMENT_H
