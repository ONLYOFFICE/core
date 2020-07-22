#ifndef CGETDATA_H
#define CGETDATA_H

#include "../Katana/katana-parser/src/katana.h"
#include "../Katana/katana-parser/src/parser.h"
#include "iostream"
#include <vector>
#include <map>

class CGetData
{
    std::vector<std::pair<std::vector<std::wstring>, std::vector<std::pair<std::wstring, std::wstring>>>> m_arData;
//              selectors                            declarations

public:
    CGetData();

    void GetOutputData(KatanaOutput* oOutput);
    void GetStylesheet(KatanaParser* oParser, KatanaStylesheet* oStylesheet);
        void GetRule(KatanaParser* oParser, KatanaRule* oRule);
        void GetStyleRule(KatanaParser* oParser, KatanaStyleRule* oRule);

        std::vector<std::wstring> GetSelectorList(KatanaParser* oParser, KatanaArray* oSelectors);
        std::wstring GetSelector(KatanaParser* oParser, KatanaSelector* oSelector);

        std::vector<std::pair<std::wstring, std::wstring>> GetDeclarationList(KatanaParser* oParser, KatanaArray* oDeclarations);
        std::pair<std::wstring, std::wstring> GetDeclaration(KatanaParser* oParser, KatanaDeclaration* oDecl);

        std::wstring SelectorToString(KatanaSelector* oSelector);

    void Print();
};

#endif // CGETDATA_H
