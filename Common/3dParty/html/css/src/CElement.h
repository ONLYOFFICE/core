#ifndef CELEMENT_H
#define CELEMENT_H

#include <vector>

class CElement
{
    std::vector<CElement*> m_arChildrens;
    std::vector<std::wstring> m_arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> m_arDeclarations;
public:
    CElement();
    ~CElement();

    void AddSelector(std::wstring sSelector);
    void AddSelectors(std::vector<std::wstring> arSelectors);
    void AddChildren(CElement* oChildren);
    void AddDeclaration(std::pair<std::wstring, std::wstring> pDeclaration);
    void AddDeclarations(std::vector<std::pair<std::wstring, std::wstring>> arDeclarations);

    int GetCountSelectors();
    int GetCountDeclarations();
    int GetCountChildrens();

    std::vector<std::wstring> GetSelectors();
    std::vector<std::pair<std::wstring, std::wstring>> GetDeclarations();
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(std::wstring sSelector,
                                                                                                             std::vector<std::wstring> sParent);

    std::vector<CElement*> GetChildrens();

    std::wstring GetText();

    CElement& operator= (const CElement& oElement);
};

#endif // CELEMENT_H
