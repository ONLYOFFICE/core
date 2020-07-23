#ifndef CGETDATA_H
#define CGETDATA_H

#include "../katana-parser/src/katana.h"
#include "../katana-parser/src/parser.h"
#include "iostream"
#include <vector>
#include <map>
#include "CElement.h"

class CGetData
{
    std::vector<CElement*> m_arData;

    void GetStylesheet(KatanaParser* oParser, KatanaStylesheet* oStylesheet, CElement* elementRule = NULL);
    void GetRule(KatanaParser* oParser, KatanaRule* oRule, CElement* elementRule = NULL);

    CElement* GetStyleRule(KatanaParser* oParser, KatanaStyleRule* oRule, CElement* elementRule = NULL);
    void GetImportRule(KatanaParser* parser, KatanaImportRule* rule, CElement* elementRule = NULL);
    void GetFontFaceRule(KatanaParser* parser, KatanaFontFaceRule* rule, CElement* elementRule = NULL);
    void GetKeyframesRule(KatanaParser* parser, KatanaKeyframesRule* rule, CElement* elementRule = NULL);
    void GetMediaRule(KatanaParser* parser, KatanaMediaRule* rule, CElement* elementRule = NULL);

    std::wstring GetMediaList(KatanaParser* parser, KatanaArray* medias);
    std::wstring GetMediaQuery(KatanaParser* parser, KatanaMediaQuery* query);
    std::wstring GetMediaQueryExp(KatanaParser* parser, KatanaMediaQueryExp* exp);

    std::wstring GetValueList(KatanaParser* parser, KatanaArray* values);

    CElement* GetKeyframe(KatanaParser* parser, KatanaKeyframe* keyframe);

    std::vector<std::wstring> GetSelectorList(KatanaParser* oParser, KatanaArray* oSelectors);
    std::wstring GetSelector(KatanaParser* oParser, KatanaSelector* oSelector);

    std::vector<std::pair<std::wstring, std::wstring>> GetDeclarationList(KatanaParser* oParser, KatanaArray* oDeclarations);
    std::pair<std::wstring, std::wstring> GetDeclaration(KatanaParser* oParser, KatanaDeclaration* oDecl);

public:
    CGetData();
    ~CGetData();

    void GetOutputData(KatanaOutput* oOutput);

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(std::wstring sSelector);

    void Print();
};

#endif // CGETDATA_H
