#ifndef CGETDATA_H
#define CGETDATA_H

#include "../katana-parser/src/katana.h"
#include "../katana-parser/src/parser.h"
#include "iostream"
#include <vector>
#include <map>
#include "CElement.h"

typedef enum {
    Defoult,
    Cantimeter,
    Inch,
    Millimeter,
    Pixel,
    Point,
    Peak
} UnitMeasure;

class CGetData
{
    std::vector<CElement*> m_arData;

    int m_nDpi = 96;

    UnitMeasure m_UnitMeasure = Millimeter;

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

    std::wstring ConvertUnitMeasure(std::wstring sValue);

    std::wstring ConvertPx(std::wstring sValue);
    std::wstring ConvertPxToCm(double dValue);
    std::wstring ConvertPxToIn(double dValue);
    std::wstring ConvertPxToMm(double dValue);
    std::wstring ConvertPxToPt(double dValue);
    std::wstring ConvertPxToPc(double dValue);

    std::wstring ConvertCm(std::wstring sValue);
    std::wstring ConvertCmToPx(double dValue);
    std::wstring ConvertCmToIn(double dValue);
    std::wstring ConvertCmToMm(double dValue);
    std::wstring ConvertCmToPt(double dValue);
    std::wstring ConvertCmToPc(double dValue);

    std::wstring ConvertMm(std::wstring sValue);
    std::wstring ConvertMmToPx(double dValue);
    std::wstring ConvertMmToIn(double dValue);
    std::wstring ConvertMmToCm(double dValue);
    std::wstring ConvertMmToPt(double dValue);
    std::wstring ConvertMmToPc(double dValue);

    std::wstring ConvertIn(std::wstring sValue);
    std::wstring ConvertInToPx(double dValue);
    std::wstring ConvertInToMm(double dValue);
    std::wstring ConvertInToCm(double dValue);
    std::wstring ConvertInToPt(double dValue);
    std::wstring ConvertInToPc(double dValue);

    std::wstring ConvertPt(std::wstring sValue);
    std::wstring ConvertPtToPx(double dValue);
    std::wstring ConvertPtToMm(double dValue);
    std::wstring ConvertPtToCm(double dValue);
    std::wstring ConvertPtToIn(double dValue);
    std::wstring ConvertPtToPc(double dValue);

    std::wstring ConvertPc(std::wstring sValue);
    std::wstring ConvertPcToPx(double dValue);
    std::wstring ConvertPcToMm(double dValue);
    std::wstring ConvertPcToCm(double dValue);
    std::wstring ConvertPcToIn(double dValue);
    std::wstring ConvertPcToPt(double dValue);
public:
    CGetData();
    ~CGetData();

    void GetOutputData(KatanaOutput* oOutput);

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(std::wstring sSelector);

    std::map<std::string, std::string> GetStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure = Defoult);
    std::map<std::wstring, std::wstring> GetStyleW(std::vector<std::string> arSelectors, UnitMeasure unitMeasure = Defoult);

    std::vector<int> GetWeightSelector(std::string sSelector);
    std::vector<int> GetWeightSelector(std::wstring sSelector);

    void AddStyle(std::vector<std::string> sSelectors, std::string sStyle);

    void SetUnitMeasure(UnitMeasure nType);
    void SetDpi(int nValue);

    UnitMeasure GetUnitMeasure();
    int GetDpi();

    void Print();
};

#endif // CGETDATA_H
