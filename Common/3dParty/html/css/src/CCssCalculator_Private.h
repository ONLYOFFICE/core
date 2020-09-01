#ifndef CCSSCALCULATOR_PRIVATE_H
#define CCSSCALCULATOR_PRIVATE_H

#include <vector>
#include <map>
#include "CElement.h"
#include "CCssCalculator.h"

#include "../../katana-parser/src/katana.h"

namespace NSCSS
{
    class CCssCalculator_Private
    {
        std::vector<CElement*> m_arData;
        std::vector<std::wstring> m_arFiles;

        std::vector<std::pair<CNode, std::pair<std::vector<CNode>, std::wstring>>> m_arUsedNode;

        int m_nDpi;
        std::wstring m_sEncoding;
        int m_nCountNodes;

        UnitMeasure m_UnitMeasure;

        void AddElement(CElement* oElement);
        size_t GetSize() const;

        void GetStylesheet(KatanaStylesheet* oStylesheet, CElement* oElementRule = NULL);
        void GetRule(KatanaRule* oRule, CElement* oElementRule = NULL);

        CElement* GetStyleRule(KatanaStyleRule* oRule, CElement* oElementRule = NULL);
        void GetImportRule(KatanaImportRule* oRule);
        void GetFontFaceRule(KatanaFontFaceRule* oRule);
        void GetKeyframesRule(KatanaKeyframesRule* oRule);
        void GetMediaRule(KatanaMediaRule* oRule);

        CElement* GetElement(const int& nIndex) const;

        std::wstring GetMediaList(KatanaArray* oMedias);
        std::wstring GetMediaQuery(KatanaMediaQuery* oQuery);
        std::wstring GetMediaQueryExp(KatanaMediaQueryExp* oExp);

        std::wstring GetValueList(KatanaArray* oValues);

        CElement* GetKeyframe(KatanaKeyframe* oKeyframe);

        std::vector<std::wstring> GetSelectorList(KatanaArray* oSelectors) const;
        std::wstring GetSelector(KatanaSelector* oSelector) const;

        std::vector<std::pair<std::wstring, std::wstring>> GetDeclarationList(KatanaArray* oDeclarations) const;
        std::pair<std::wstring, std::wstring> GetDeclaration(KatanaDeclaration* oDecl) const;

        std::wstring ConvertUnitMeasure(const std::wstring& sValue) const;

        std::wstring ConvertPx(const std::wstring& sValue) const;
        std::wstring ConvertPxToCm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToIn(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToMm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToPt(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToPc(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertCm(const std::wstring& sValue) const;
        std::wstring ConvertCmToPx(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToIn(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToMm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToPt(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToPc(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertMm(const std::wstring& sValue) const;
        std::wstring ConvertMmToPx(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToIn(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToCm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToPt(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToPc(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertIn(const std::wstring& sValue) const;
        std::wstring ConvertInToPx(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToMm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToCm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToPt(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToPc(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertPt(const std::wstring& sValue) const;
        std::wstring ConvertPtToPx(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToMm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToCm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToIn(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToPc(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertPc(const std::wstring& sValue) const;
        std::wstring ConvertPcToPx(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToMm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToCm(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToIn(const double& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToPt(const double& dValue, bool bAddUM = false) const;

        std::wstring ConvertEm(const std::wstring& sValue) const;

        std::vector<int> GetWeightSelector(const std::string& sSelector) const;
        std::vector<int> GetWeightSelector(const std::wstring& sSelector) const;

        void GetOutputData(KatanaOutput* oOutput);
    public:
        CCssCalculator_Private();
        ~CCssCalculator_Private();

        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> GetDeclarations(const std::wstring& sSelector) const;

        CCompiledStyle GetCompiledStyle(const std::vector<std::string> arSelectors, const UnitMeasure unitMeasure = Default);
        CCompiledStyle GetCompiledStyle(const CNode& oNode, const std::vector<CNode>& oParents = std::vector<CNode>(), const UnitMeasure& unitMeasure = Default);

        void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStyles(const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(const int& nValue);

        const UnitMeasure& GetUnitMeasure() const;
        const std::wstring& GetEncoding() const;
        const int& GetDpi() const;

        void Print() const;
        void Clear();
    };
}
#endif // CCSSCALCULATOR_PRIVATE_H
