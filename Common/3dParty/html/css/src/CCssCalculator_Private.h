#ifndef CCSSCALCULATOR_PRIVATE_H
#define CCSSCALCULATOR_PRIVATE_H

#include <vector>
#include <map>
#include "CElement.h"
#include "CCssCalculator.h"

#include "../../katana-parser/src/katana.h"

class CCssCalculator_Private
{
public:
    std::vector<CElement*> m_arData;

    int m_nDpi = 96;
    std::wstring m_sEncoding = L"UTF-8";

    UnitMeasure m_UnitMeasure;

    CCssCalculator_Private();
    ~CCssCalculator_Private();

    void AddElement(CElement* oElement);
    size_t GetSize();

    void GetStylesheet(KatanaStylesheet* oStylesheet, CElement* oElementRule = NULL);
    void GetRule(KatanaRule* oRule, CElement* oElementRule = NULL);

    CElement* GetStyleRule(KatanaStyleRule* oRule, CElement* oElementRule = NULL);
    void GetImportRule(KatanaImportRule* oRule);
    void GetFontFaceRule(KatanaFontFaceRule* oRule);
    void GetKeyframesRule(KatanaKeyframesRule* oRule);
    void GetMediaRule(KatanaMediaRule* oRule);

    CElement* GetElement(const int& nIndex);

    std::wstring GetMediaList(KatanaArray* oMedias);
    std::wstring GetMediaQuery(KatanaMediaQuery* oQuery);
    std::wstring GetMediaQueryExp(KatanaMediaQueryExp* oExp);

    std::wstring GetValueList(KatanaArray* oValues);

    CElement* GetKeyframe(KatanaKeyframe* oKeyframe);

    std::vector<std::wstring> GetSelectorList(KatanaArray* oSelectors);
    std::wstring GetSelector(KatanaSelector* oSelector);

    std::vector<std::pair<std::wstring, std::wstring>> GetDeclarationList(KatanaArray* oDeclarations);
    std::pair<std::wstring, std::wstring> GetDeclaration(KatanaDeclaration* oDecl);

    std::wstring ConvertUnitMeasure(const std::wstring& sValue);

    std::wstring ConvertPx(const std::wstring& sValue);
    std::wstring ConvertPxToCm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPxToIn(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPxToMm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPxToPt(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPxToPc(const double& dValue, bool bAddUM = false);

    std::wstring ConvertCm(const std::wstring& sValue);
    std::wstring ConvertCmToPx(const double& dValue, bool bAddUM = false);
    std::wstring ConvertCmToIn(const double& dValue, bool bAddUM = false);
    std::wstring ConvertCmToMm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertCmToPt(const double& dValue, bool bAddUM = false);
    std::wstring ConvertCmToPc(const double& dValue, bool bAddUM = false);

    std::wstring ConvertMm(const std::wstring& sValue);
    std::wstring ConvertMmToPx(const double& dValue, bool bAddUM = false);
    std::wstring ConvertMmToIn(const double& dValue, bool bAddUM = false);
    std::wstring ConvertMmToCm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertMmToPt(const double& dValue, bool bAddUM = false);
    std::wstring ConvertMmToPc(const double& dValue, bool bAddUM = false);

    std::wstring ConvertIn(const std::wstring& sValue);
    std::wstring ConvertInToPx(const double& dValue, bool bAddUM = false);
    std::wstring ConvertInToMm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertInToCm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertInToPt(const double& dValue, bool bAddUM = false);
    std::wstring ConvertInToPc(const double& dValue, bool bAddUM = false);

    std::wstring ConvertPt(const std::wstring& sValue);
    std::wstring ConvertPtToPx(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPtToMm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPtToCm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPtToIn(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPtToPc(const double& dValue, bool bAddUM = false);

    std::wstring ConvertPc(const std::wstring& sValue);
    std::wstring ConvertPcToPx(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPcToMm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPcToCm(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPcToIn(const double& dValue, bool bAddUM = false);
    std::wstring ConvertPcToPt(const double& dValue, bool bAddUM = false);

    std::vector<int> GetWeightSelector(const std::string& sSelector);
    std::vector<int> GetWeightSelector(const std::wstring& sSelector);

    void GetOutputData(KatanaOutput* oOutput);

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(const std::wstring& sSelector);

    std::map<std::string, std::string> GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure = Defoult);
    std::map<std::wstring, std::wstring> GetCompiledStyleW(std::vector<std::string> arSelectors, UnitMeasure unitMeasure = Defoult);


    void AddStyle(std::vector<std::string> sSelectors, const std::string& sStyle);
    void AddStyle(const std::string& sStyle);
    void AddStyles(const std::wstring& sFileName);

    void SetUnitMeasure(const UnitMeasure nType);
    void SetDpi(const int nValue);

    UnitMeasure GetUnitMeasure();
    std::wstring GetEncoding();
    int GetDpi();

    void Print();
};

#endif // CCSSCALCULATOR_PRIVATE_H
