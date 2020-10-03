#ifndef CCSSCALCULATOR_PRIVATE_H
#define CCSSCALCULATOR_PRIVATE_H

#include <vector>
#include <map>
#include "CCssCalculator.h"
#include <list>
#include <functional>
#include "CElement.h"
#include "../../katana-parser/src/katana.h"

namespace NSCSS
{
    class CCssCalculator_Private
    {
        unsigned short int m_nDpi;
        unsigned short int m_nCountNodes;
        UnitMeasure m_UnitMeasure;

        std::list<std::wstring> m_arFiles;

        std::map<std::wstring, CElement*> m_mData;

//        std::map<std::vector<CNode>, std::map<std::wstring, std::wstring>*> m_mData;

        std::map<std::vector<CNode>, CCompiledStyle*> m_mUsedStyles;

        std::wstring m_sEncoding;

        // void AddElement(CElement* oElement);
        // size_t GetSize() const;

        void GetStylesheet(const KatanaStylesheet* oStylesheet);
        void GetRule(const KatanaRule* oRule);

        void GetStyleRule(const KatanaStyleRule* oRule);
        void GetImportRule(const KatanaImportRule* oRule);
        void GetFontFaceRule(const KatanaFontFaceRule* oRule);
        void GetKeyframesRule(const KatanaKeyframesRule* oRule);
        void GetMediaRule(const KatanaMediaRule* oRule);

        // CElement* GetElement(const int& nIndex) const;

        std::wstring GetMediaList(const KatanaArray* oMedias);
        std::wstring GetMediaQuery(const KatanaMediaQuery* oQuery);
        std::wstring GetMediaQueryExp(const KatanaMediaQueryExp* oExp);

        std::wstring GetValueList(const KatanaArray* oValues);

        void GetKeyframe(const KatanaKeyframe* oKeyframe);

        std::vector<std::wstring> GetSelectorList(const KatanaArray* oSelectors) const;
        std::wstring GetSelector(const KatanaSelector* oSelector) const;

        std::map<std::wstring, std::wstring> GetDeclarationList(const KatanaArray* oDeclarations) const;
        std::pair<std::wstring, std::wstring> GetDeclaration(const KatanaDeclaration* oDecl) const;

        std::wstring ConvertUnitMeasure(const std::wstring& sValue) const;

        std::wstring ConvertPx(const std::wstring& sValue) const;
        std::wstring ConvertPxToCm(const float& dValue) const;
        std::wstring ConvertPxToIn(const float& dValue) const;
        std::wstring ConvertPxToMm(const float& dValue) const;
        std::wstring ConvertPxToPt(const float& dValue) const;
        std::wstring ConvertPxToPc(const float& dValue) const;     

        std::wstring ConvertCm(const std::wstring& sValue) const;
        std::wstring ConvertCmToPx(const float& dValue) const;
        std::wstring ConvertCmToIn(const float& dValue) const;
        std::wstring ConvertCmToMm(const float& dValue) const;
        std::wstring ConvertCmToPt(const float& dValue) const;
        std::wstring ConvertCmToPc(const float& dValue) const;

        std::wstring ConvertMm(const std::wstring& sValue) const;
        std::wstring ConvertMmToPx(const float& dValue) const;
        std::wstring ConvertMmToIn(const float& dValue) const;
        std::wstring ConvertMmToCm(const float& dValue) const;
        std::wstring ConvertMmToPt(const float& dValue) const;
        std::wstring ConvertMmToPc(const float& dValue) const;

        std::wstring ConvertIn(const std::wstring& sValue) const;
        std::wstring ConvertInToPx(const float& dValue) const;
        std::wstring ConvertInToMm(const float& dValue) const;
        std::wstring ConvertInToCm(const float& dValue) const;
        std::wstring ConvertInToPt(const float& dValue) const;
        std::wstring ConvertInToPc(const float& dValue) const;

        std::wstring ConvertPt(const std::wstring& sValue) const;
        std::wstring ConvertPtToPx(const float& dValue) const;
        std::wstring ConvertPtToMm(const float& dValue) const;
        std::wstring ConvertPtToCm(const float& dValue) const;
        std::wstring ConvertPtToIn(const float& dValue) const;
        std::wstring ConvertPtToPc(const float& dValue) const;

        std::wstring ConvertPc(const std::wstring& sValue) const;
        std::wstring ConvertPcToPx(const float& dValue) const;
        std::wstring ConvertPcToMm(const float& dValue) const;
        std::wstring ConvertPcToCm(const float& dValue) const;
        std::wstring ConvertPcToIn(const float& dValue) const;
        std::wstring ConvertPcToPt(const float& dValue) const;

        std::wstring ConvertEm(const std::wstring& sValue) const;

        std::vector<unsigned short int> GetWeightSelector(const std::string& sSelector) const;
        std::vector<unsigned short int> GetWeightSelector(const std::wstring& sSelector) const;

        void GetOutputData(KatanaOutput* oOutput);

    public:
        CCssCalculator_Private();
        ~CCssCalculator_Private();

        std::map<std::wstring, std::wstring> GetDeclarations(const std::wstring& sSelector) const;

        CCompiledStyle GetCompiledStyle(const std::vector<std::string>& arSelectors, const UnitMeasure& unitMeasure = Default);
        CCompiledStyle GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure = Default);

        // void AddStyle(const std::vector<std::string>& sSelectors, const std::string& sStyle);
        void AddStyles(const std::string& sStyle);
        void AddStyles(const std::wstring& sStyle);
        void AddStylesFromFile(const std::wstring& sFileName);

        void SetUnitMeasure(const UnitMeasure& nType);
        void SetDpi(unsigned short int nValue);

        UnitMeasure GetUnitMeasure() const;
        std::wstring GetEncoding() const;
        unsigned short int GetDpi() const;

        void Print() const;
        void Clear();        

    };
}
#endif // CCSSCALCULATOR_PRIVATE_H
