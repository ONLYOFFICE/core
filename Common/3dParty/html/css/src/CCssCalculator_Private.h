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
        unsigned int m_nDpi;
        unsigned int m_nCountNodes;
        std::wstring m_sEncoding;
        UnitMeasure m_UnitMeasure;

        std::vector<CElement*> m_arData;
        std::vector<std::wstring> m_arFiles;

        std::map<std::vector<CNode>, CCompiledStyle*> m_mUsedStyles;

        // void AddElement(CElement* oElement);
        // size_t GetSize() const;

        void GetStylesheet(const KatanaStylesheet* oStylesheet, CElement* oElementRule = NULL);
        void GetRule(const KatanaRule* oRule, CElement* oElementRule = NULL);

        CElement* GetStyleRule(const KatanaStyleRule* oRule, CElement* oElementRule = NULL);
        void GetImportRule(const KatanaImportRule* oRule);
        void GetFontFaceRule(const KatanaFontFaceRule* oRule);
        void GetKeyframesRule(const KatanaKeyframesRule* oRule);
        void GetMediaRule(const KatanaMediaRule* oRule);

        // CElement* GetElement(const int& nIndex) const;

        std::wstring GetMediaList(const KatanaArray* oMedias);
        std::wstring GetMediaQuery(const KatanaMediaQuery* oQuery);
        std::wstring GetMediaQueryExp(const KatanaMediaQueryExp* oExp);

        std::wstring GetValueList(const KatanaArray* oValues);

        CElement* GetKeyframe(const KatanaKeyframe* oKeyframe);

        std::vector<std::wstring> GetSelectorList(const KatanaArray* oSelectors) const;
        std::wstring GetSelector(const KatanaSelector* oSelector) const;

        std::map<std::wstring, std::wstring> GetDeclarationList(const KatanaArray* oDeclarations) const;
        std::pair<std::wstring, std::wstring> GetDeclaration(const KatanaDeclaration* oDecl) const;

        std::wstring ConvertUnitMeasure(const std::wstring& sValue) const;

        std::wstring ConvertPx(const std::wstring& sValue) const;
        std::wstring ConvertPxToCm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToIn(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToMm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToPt(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPxToPc(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertCm(const std::wstring& sValue) const;
        std::wstring ConvertCmToPx(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToIn(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToMm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToPt(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertCmToPc(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertMm(const std::wstring& sValue) const;
        std::wstring ConvertMmToPx(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToIn(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToCm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToPt(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertMmToPc(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertIn(const std::wstring& sValue) const;
        std::wstring ConvertInToPx(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToMm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToCm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToPt(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertInToPc(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertPt(const std::wstring& sValue) const;
        std::wstring ConvertPtToPx(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToMm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToCm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToIn(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPtToPc(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertPc(const std::wstring& sValue) const;
        std::wstring ConvertPcToPx(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToMm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToCm(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToIn(const float& dValue, bool bAddUM = false) const;
        std::wstring ConvertPcToPt(const float& dValue, bool bAddUM = false) const;

        std::wstring ConvertEm(const std::wstring& sValue) const;

        std::vector<unsigned int> GetWeightSelector(const std::string& sSelector) const;
        std::vector<unsigned int> GetWeightSelector(const std::wstring& sSelector) const;

        void GetOutputData(KatanaOutput* oOutput);
    public:
        CCssCalculator_Private();
        ~CCssCalculator_Private();

        std::map<std::wstring, std::map<std::wstring, std::wstring>> GetDeclarations(const std::wstring& sSelector) const;

        CCompiledStyle GetCompiledStyle(const std::vector<std::string>& arSelectors, const UnitMeasure& unitMeasure = Default);
        CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure = Default);

        // void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStyles(const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(const unsigned int& nValue);

        UnitMeasure GetUnitMeasure() const;
        std::wstring GetEncoding() const;
        unsigned int GetDpi() const;

        void Print() const;
        void Clear();
    };
}
#endif // CCSSCALCULATOR_PRIVATE_H
