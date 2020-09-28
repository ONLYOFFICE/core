#include "CCssCalculator_Private.h"

#include <string>
#include <vector>
#include <fstream>
#include <cwctype>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <array>

#include "../../katana-parser/src/selector.h"
#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "ConstValues.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "StaticFunctions.h"


inline static std::wstring      StringifyValueList(const KatanaArray* oValues);
inline static std::wstring      StringifyValue(const KatanaValue* oValue);

bool operator<(const std::vector<NSCSS::CNode> &arLeftSelectors, const std::vector<NSCSS::CNode> &arRightSelectors)
{
    const size_t& sizeLeftSelectors = arLeftSelectors.size();
    const size_t& sizeRightSelectors = arRightSelectors.size();

    if (sizeLeftSelectors < sizeRightSelectors)
        return true;
    else if (sizeLeftSelectors > sizeRightSelectors)
        return false;

    for (size_t i = 0; i < arLeftSelectors.size(); ++i)
    {
        if (arLeftSelectors[i] < arRightSelectors[i])
            return true;
    }

    return false;
}

namespace NSCSS
{
    CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_nCountNodes(1), m_UnitMeasure(Default), m_sEncoding(L"UTF-8") {}

    CCssCalculator_Private::~CCssCalculator_Private()
    {
        for (CElement*& item : m_arData)
            delete item;
        m_arData.clear();
        m_arFiles.clear();

        for (std::map<std::vector<CNode>, CCompiledStyle*>::iterator iter  = m_mUsedStyles.begin(); iter != m_mUsedStyles.end(); ++iter)
            delete iter->second;
        m_mUsedStyles.clear();
    }

    /*
    inline CElement* CCssCalculator_Private::GetElement(const int& nIndex) const
    {
        if (nIndex < 0 || nIndex >= (int)m_arData.size())
            return NULL;

        return m_arData[nIndex];
    }

    inline void CCssCalculator_Private::AddElement(CElement *oElement)
    {
        if (oElement != NULL)
        {
            const std::vector<std::pair<std::wstring, std::wstring>>& arDeclarations = oElement->GetDeclarations();

            for (std::pair<std::wstring, std::wstring> pDeclaration : arDeclarations)
                pDeclaration.second = ConvertUnitMeasure(pDeclaration.second);

            oElement->SetDeclaratins(arDeclarations);
            m_arData.push_back(oElement);
        }
    }
    */

    inline void CCssCalculator_Private::GetOutputData(KatanaOutput *oOutput)
    {
        if ( NULL == oOutput )
            return;

        CElement *oElement = new CElement;

        switch (oOutput->mode) {
            case KatanaParserModeStylesheet:
                GetStylesheet(oOutput->stylesheet);
                break;
            case KatanaParserModeRule:
                GetRule(oOutput->rule);
                break;
            case KatanaParserModeKeyframeRule:
                oElement = GetKeyframe(oOutput->keyframe);

                break;
            case KatanaParserModeKeyframeKeyList:
                oElement->AddSelector(GetValueList(oOutput->keyframe_keys));
                break;
            case KatanaParserModeMediaList:
                oElement->AddSelector(GetMediaList(oOutput->medias));
                break;
            case KatanaParserModeValue:
                oElement->AddSelector(GetValueList(oOutput->values));
                break;
            case KatanaParserModeSelector:
                oElement->AddSelectors(GetSelectorList(oOutput->selectors));
                break;
            case KatanaParserModeDeclarationList:
                oElement->AddDeclarations(GetDeclarationList(oOutput->declarations));
                break;
        }

        (oElement->Empty()) ? delete oElement : m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetStylesheet(const KatanaStylesheet *oStylesheet, CElement *elementRule)
    {
        for (size_t i = 0; i < oStylesheet->imports.length; ++i)
            GetRule((KatanaRule*)oStylesheet->imports.data[i], elementRule);

        for (size_t i = 0; i < oStylesheet->rules.length; ++i)
            GetRule((KatanaRule*)oStylesheet->rules.data[i], elementRule);
    }

    inline void CCssCalculator_Private::GetRule(const KatanaRule *oRule, CElement *oElementRule)
    {
        if ( NULL == oRule )
            return;

        switch (oRule->type) {
            case KatanaRuleStyle:
                GetStyleRule((KatanaStyleRule*)oRule, oElementRule);
                break;
            case KatanaRuleImport:
                GetImportRule((KatanaImportRule*)oRule);
                break;
            case KatanaRuleFontFace:
                GetFontFaceRule((KatanaFontFaceRule*)oRule);
                break;
            case KatanaRuleKeyframes:
                GetKeyframesRule((KatanaKeyframesRule*)oRule);
                break;
            case KatanaRuleMedia:
                GetMediaRule((KatanaMediaRule*)oRule);
                break;
            case KatanaRuleSupports:
                break;
            case KatanaRuleUnkown:
                break;

            default:
                break;
        }
    }

    inline CElement* CCssCalculator_Private::GetStyleRule(const KatanaStyleRule *oRule, CElement *oElementRule)
    {
        std::map<std::wstring, std::wstring> arDeclarations;

        if (oRule->declarations->length)
            arDeclarations = GetDeclarationList(oRule->declarations);

        CElement *oElement = new CElement;

        oElement->AddSelectors(GetSelectorList(oRule->selectors));
        oElement->AddDeclarations(arDeclarations);

        if (oElementRule == NULL)
        {
            m_arData.push_back(oElement);
            return NULL;
        }
        else
            return oElement;
    }

    inline std::vector<std::wstring> CCssCalculator_Private::GetSelectorList(const KatanaArray* oSelectors) const
    {
        std::vector<std::wstring> arSelectors;

        for (size_t i = 0; i < oSelectors->length; ++i)
            arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));

        return arSelectors;
    }

    inline std::wstring CCssCalculator_Private::GetSelector(const KatanaSelector *oSelector) const
    {
        KatanaParser oParser;
        oParser.options = &kKatanaDefaultOptions;

        std::wstring sText;
        const KatanaParserString* string = katana_selector_to_string(&oParser, const_cast<KatanaSelector*>(oSelector), NULL);
        const char* text = katana_string_to_characters(&oParser, string);

        katana_parser_deallocate(&oParser, (void*) string->data);
        katana_parser_deallocate(&oParser, (void*) string);

        sText = NS_STATIC_FUNCTIONS::stringToWstring(text);

        katana_parser_deallocate(&oParser, (void*) text);

        sText.erase(std::remove_if(sText.begin(), sText.end(), [] (wchar_t ch) { return std::iswspace(ch); }));

        return sText;
    }

    inline std::map<std::wstring, std::wstring> CCssCalculator_Private::GetDeclarationList(const KatanaArray* oDeclarations) const
    {
        std::map<std::wstring, std::wstring> arDeclarations;

        for (size_t i = 0; i < oDeclarations->length; ++i)
            arDeclarations.insert(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));

        return arDeclarations;
    }

    inline std::pair<std::wstring, std::wstring> CCssCalculator_Private::GetDeclaration(const KatanaDeclaration* oDecl) const
    {
        std::wstring sValueList = StringifyValueList(oDecl->values);

        if (oDecl->important)
            sValueList += L" !important";

        return std::make_pair(NS_STATIC_FUNCTIONS::stringToWstring(oDecl->property), sValueList);
    }

    inline void CCssCalculator_Private::GetImportRule(const KatanaImportRule *oRule)
    {
        const std::wstring& sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name) + L" " +  L"url(" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->href) + L")";

        CElement *oElement = new CElement;

        oElement->AddSelectors({sSelector});
        oElement->AddDeclarations({});

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetFontFaceRule(const KatanaFontFaceRule *oRule)
    {
        const std::wstring sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name);

        CElement *oElement = new CElement;

        oElement->AddSelectors({sSelector});
        oElement->AddDeclarations(GetDeclarationList(oRule->declarations));

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetKeyframesRule(const KatanaKeyframesRule *oRule)
    {
        CElement *oElement = new CElement;

        const std::wstring& sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name);
        oElement->AddSelector(sSelector);

        for (size_t i = 0; i < oRule->keyframes->length; ++i)
            oElement->AddChildren(GetKeyframe((KatanaKeyframe*)oRule->keyframes->data[i]));

        m_arData.push_back(oElement);
    }

    inline CElement* CCssCalculator_Private::GetKeyframe(const KatanaKeyframe *oKeyframe)
    {
        if (oKeyframe == NULL)
            return NULL;

        CElement *oElement = new CElement;

        for (size_t i = 0; i < oKeyframe->selectors->length; ++i)
        {
            const KatanaValue* oValue = (KatanaValue*)oKeyframe->selectors->data[i];
            if (oValue->unit == KATANA_VALUE_NUMBER)
                oElement->AddSelector(NS_STATIC_FUNCTIONS::stringToWstring(oValue->raw));
        }

        oElement->AddDeclarations(GetDeclarationList(oKeyframe->declarations));

        return oElement;
    }

    inline void CCssCalculator_Private::GetMediaRule(const KatanaMediaRule *oRule)
    {
        std::wstring sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name) + L" ";

        CElement *oElement = new CElement;

        if (oRule->medias->length)
            sSelector += GetMediaList(oRule->medias);

        oElement->AddSelector(sSelector);

        if (oRule->medias->length)
            for (size_t i = 0; i < oRule->rules->length; ++i)
                oElement->AddChildren(GetStyleRule((KatanaStyleRule*)oRule->rules->data[i], oElement));

        m_arData.push_back(oElement);
    }

    inline std::wstring CCssCalculator_Private::GetMediaList(const KatanaArray *oMedias)
    {
        std::wstring sText;

        for (size_t i = 0; i < oMedias->length; ++i)
            sText += GetMediaQuery((KatanaMediaQuery*)oMedias->data[i]) + L", ";

        sText.erase(sText.length() - 2, 2);

        return sText;
    }

    inline std::wstring CCssCalculator_Private::GetMediaQuery(const KatanaMediaQuery *oQuery)
    {
        std::wstring sText;
        switch (oQuery->restrictor)
        {
            case KatanaMediaQueryRestrictorOnly:
                sText += L"only ";
                break;
            case KatanaMediaQueryRestrictorNot:
                sText += L"not ";
                break;
            case KatanaMediaQueryRestrictorNone:
                break;
        }

        if (NULL == oQuery->expressions || 0 == oQuery->expressions->length)
        {
            if (NULL != oQuery->type)
                sText += NS_STATIC_FUNCTIONS::stringToWstring(oQuery->type);
            return sText;
        }

        if ((NULL != oQuery->type && strcmp(oQuery->type, "all")) ||
             oQuery->restrictor != KatanaMediaQueryRestrictorNone)
        {
            if (NULL != oQuery->type)
                sText += NS_STATIC_FUNCTIONS::stringToWstring(oQuery->type);

            sText += L" and ";
        }

        sText += GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[0]);

        for (size_t i = 1; i < oQuery->expressions->length; ++i)
            sText += L" and " + GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[i]);

        return sText;
    }

    inline std::wstring CCssCalculator_Private::GetMediaQueryExp(const KatanaMediaQueryExp *oExp)
    {
        std::wstring sText;

        sText += L"(";

        if (NULL != oExp->feature)
            sText += NS_STATIC_FUNCTIONS::stringToWstring(oExp->feature);

        if (oExp->values && oExp->values->length)
            sText += L": " + StringifyValueList(oExp->values);

        sText += L")";

        return sText;
    }

    std::map<std::wstring, std::wstring> CCssCalculator_Private::GetDeclarations(const std::wstring& sSelector) const
    {
        if (sSelector.empty() || m_arData.empty())
            return std::map<std::wstring, std::wstring>();

        for (const CElement* oElement : m_arData )
            if (oElement->FindSelector(sSelector))
                return oElement->GetDeclarations(sSelector, {});

        return std::map<std::wstring, std::wstring>();
    }

    std::vector<unsigned short int> CCssCalculator_Private::GetWeightSelector(const std::string& sSelector) const
    {
        if (sSelector.empty())
            return std::vector<unsigned short int>{0, 0, 0, 0};

        std::vector<unsigned short int> arWeight = {0, 0, 0, 0};

        const std::vector<std::string> arSel = NS_STATIC_FUNCTIONS::GetWords(NS_STATIC_FUNCTIONS::stringToWstring(sSelector));

        for (const std::string& sSel : arSel)
        {
            if (sSel == "*")
                ++arWeight[3];
            else if (sSel.find('#') != std::string::npos)
                ++arWeight[0];
            else if (sSel.find(':') != std::string::npos)
            {
                std::string sTemp(sSel);
                sTemp.erase(std::remove_if(sTemp.begin(), sTemp.end(), [] (wchar_t ch) { return !std::iswalpha(ch); }));
                std::find(NS_CONST_VALUES::arPseudoClasses.begin(), NS_CONST_VALUES::arPseudoClasses.end(), sTemp) != NS_CONST_VALUES::arPseudoClasses.end() ? ++arWeight[1] : ++arWeight[2];
            }
            else if (sSel.find('.') != std::string::npos ||
                     sSel.find('[') != std::string::npos ||
                     sSel.find(']') != std::string::npos)
                ++arWeight[1];
            else
                ++arWeight[2];
        }

        return arWeight;
    }

    inline std::vector<unsigned short int> CCssCalculator_Private::GetWeightSelector(const std::wstring& sSelector) const
    {
        return GetWeightSelector(NS_STATIC_FUNCTIONS::wstringToString(sSelector));
    }


    void CCssCalculator_Private::Print() const
    {
        std::wcout << m_arData.size() << std::endl;

        for (const CElement* oElement : m_arData)
            std::wcout << oElement->GetText() << std::endl;
    }


    inline std::wstring CCssCalculator_Private::GetValueList(const KatanaArray *oValues)
    {
        return StringifyValueList(oValues);
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<std::string>& arSelectors, const UnitMeasure& unitMeasure)
    {
        if (arSelectors.empty())
            return CCompiledStyle();

        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);


        std::map<std::wstring, std::wstring> mStyle = GetDeclarations(L"*");

        for (const std::string& sSelector : arSelectors)
        {
            for (const auto& oDeclarations : GetDeclarations(NS_STATIC_FUNCTIONS::stringToWstring(sSelector)))
            {
                if (mStyle[oDeclarations.first].empty() || mStyle[oDeclarations.first].find(L"important") == std::wstring::npos)
                    mStyle[oDeclarations.first] = oDeclarations.second;
            }
        }

        for (const auto& oIter : mStyle)
        {
            const size_t posRgb = oIter.second.find(L"rgb");

            if (posRgb != std::wstring::npos)
            {
                mStyle[oIter.first] = NS_STATIC_FUNCTIONS::ConvertRgbToHex(oIter.second);
                continue;
            }
            const size_t posExclamatory = oIter.second.find(L"!");

            if (posExclamatory != std::wstring::npos)
                mStyle[oIter.first] = ConvertUnitMeasure(oIter.second.substr(0, posExclamatory));
            else
                mStyle[oIter.first] = ConvertUnitMeasure(oIter.second);
        }

        return  CCompiledStyle(mStyle);
    }

    /*
    void CCssCalculator_Private::AddStyle(const std::vector<std::string>& sSelectors, const std::string& sProperties)
    {
        std::string sPropertiesUTF8;

        bool bIsUTF8 = false;

        if (m_sEncoding != L"UTF-8" && m_sEncoding != L"utf-8")
            bIsUTF8 = true;

        sPropertiesUTF8 = bIsUTF8 ? GetContentAsUTF8(sProperties, m_sEncoding) : sProperties;

        std::vector<std::string> sSelectorsUTF8;

        if (bIsUTF8)
            for (const std::string& sSelector : sSelectors)
                sSelectorsUTF8.push_back(GetContentAsUTF8(sSelector, m_sEncoding));
        else
            sSelectorsUTF8 = sSelectors;
//            for (std::string sSelector : sSelectors)
//                sSelectorsUTF8.push_back(sSelector);


        CElement *oElement = new CElement;

        for (const std::string& sSelectorUTF8 : sSelectorsUTF8)
            oElement->AddSelector(NS_STATIC_FUNCTIONS::stringToWstring(sSelectorUTF8));

        std::vector<std::string> arProperty;
        std::vector<std::string> arValue;

        std::string sTemp;

        for (const char& sc : sPropertiesUTF8)
        {
            if (sc != ' ')
            {
                if (sc == ':')
                {
                    arProperty.push_back(sTemp);
                    sTemp.clear();
                }
                else if (sc == ';')
                {
                    arValue.push_back(sTemp);
                    sTemp.clear();
                }
                else
                    sTemp += sc;
            }
        }

        if (!sTemp.empty())
            arValue.push_back(sTemp);

        std::vector<std::pair<std::wstring, std::wstring>> arDecl;

        size_t size;

        arProperty.size() >= arValue.size() ? size = arValue.size() : size = arProperty.size();

        for (size_t i = 0; i < size; ++i)
        {
            const std::wstring& sValue = ConvertUnitMeasure(NS_STATIC_FUNCTIONS::stringToWstring(arValue[i]));
            arDecl.push_back(std::make_pair(NS_STATIC_FUNCTIONS::stringToWstring(arProperty[i]), sValue));
        }

        oElement->AddDeclarations(arDecl);
        m_arData.push_back(oElement);
    }
    */

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure)
    {
        if (arSelectors.empty())
            return CCompiledStyle();

        bool parentSize = arSelectors.size() > 1;
        if (parentSize)
        {
            const auto& oItem = m_mUsedStyles.find(arSelectors);
            if (oItem != m_mUsedStyles.end())
                return *oItem->second;
        }

        CCompiledStyle *oStyle = new CCompiledStyle();

        const std::wstring& sClassName = (!arSelectors.back().m_sClass.empty() && arSelectors.back().m_sClass.front() != L'.')
                                         ? L'.' + arSelectors.back().m_sClass
                                         : arSelectors.back().m_sClass;

        const std::wstring& sIdName = (!arSelectors.back().m_sId.empty() && arSelectors.back().m_sId.front() != L'#')
                                      ? L'#' + arSelectors.back().m_sId
                                      : arSelectors.back().m_sId;

        for (auto oParent = arSelectors.begin(); oParent != arSelectors.end() - 1; ++oParent)
        {
            if (oParent->m_sName != L"body")
            {
                *oStyle += GetCompiledStyle({*oParent}, unitMeasure);
                oStyle->AddParent(oParent->m_sName);
            }
        }

        *oStyle += GetCompiledStyle(NS_STATIC_FUNCTIONS::GetSelectorsList(arSelectors.back().m_sName + sClassName + sIdName), unitMeasure);

        // подумать, что с этим делать (замедляет прилично, но без него могут пропасть некоторые стили (в некоторых случаях))
//        if (arSelectors.size() > 3)
//            *oStyle += GetCompiledStyle(NS_STATIC_FUNCTIONS::GetSelectorsListWithParents(arSelectors), unitMeasure);


        if (!arSelectors.back().m_sStyle.empty())
        {
            CCompiledStyle oTempStyle;
            oTempStyle.AddStyle(ConvertUnitMeasure(arSelectors.back().m_sStyle));
            *oStyle += oTempStyle;
        }

        oStyle->SetID(arSelectors.back().m_sName + sClassName + sIdName + L'-' + std::to_wstring(m_nCountNodes++));

//        std::wcout << oStyle->GetId() << L" - " << oStyle->GetStyleW() << std::endl;

        if (parentSize)
            m_mUsedStyles[arSelectors] = oStyle;

        return *oStyle;
    }

    void CCssCalculator_Private::AddStyles(const std::string &sStyle)
    {
        if (sStyle.empty())
            return;

        KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.length(), KatanaParserModeStylesheet);
        this->GetOutputData(output);
        katana_destroy_output(output);
    }

    void CCssCalculator_Private::AddStyles(const std::wstring &sStyle)
    {
        if (sStyle.empty())
            return;

        AddStyles(NS_STATIC_FUNCTIONS::wstringToString(sStyle));
    }

    void CCssCalculator_Private::AddStylesFromFile(const std::wstring& sFileName)
    {
        if (std::find(m_arFiles.begin(), m_arFiles.end(), sFileName) != m_arFiles.end())
            return;

        m_arFiles.push_back(sFileName);

        AddStyles(NS_STATIC_FUNCTIONS::GetContentAsUTF8(sFileName));
    }

    std::wstring CCssCalculator_Private::ConvertUnitMeasure(const std::wstring &sValue) const
    {
        if (sValue.empty())
            return sValue;

        std::vector<std::wstring> arValues = NS_STATIC_FUNCTIONS::GetWordsWithSigns(sValue);

        std::wstring sValueString;

        for (std::wstring& sValueTemp : arValues)
        {
            size_t nPosGrid = sValueTemp.find(L'#');

            if (nPosGrid != std::wstring::npos || !NS_STATIC_FUNCTIONS::ThereIsNumber(sValueTemp))
            {
                if (!NS_STATIC_FUNCTIONS::ConvertAbsoluteValue(sValueTemp))
                {
                    sValueString += sValueTemp;
                    continue;
                }
            }

            const size_t posPercent = sValueTemp.find(L'%');

            if (posPercent != std::wstring::npos)
            {
                const float dValue = wcstof(sValueTemp.substr(0, posPercent).c_str(), NULL) * 0.22f;

                sValueString += std::to_wstring(static_cast<short int>(dValue + 0.5f));

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"px") != std::wstring::npos)
            {
                sValueString += ConvertPx(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"cm") != std::wstring::npos)
            {
                sValueString += ConvertCm(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"mm") != std::wstring::npos)
            {
                sValueString += ConvertMm(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"in") != std::wstring::npos)
            {
                sValueString += ConvertIn(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"pt") != std::wstring::npos)
            {
                sValueString += ConvertPt(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"pc") != std::wstring::npos)
            {
                sValueString += ConvertPc(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"em") != std::wstring::npos)
            {
                sValueString += ConvertEm(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 1 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else
            {
                sValueString += sValueTemp;

                if (sValueTemp.find(L";") != std::wstring::npos)
                    sValueString += L';';

                break;
            }

            if (sValueTemp.back() != L';' && sValueTemp.back() != L':' && sValueTemp.back() != L' ')
                sValueTemp += L' ';
        }

        return sValueString;
    }

    inline std::wstring CCssCalculator_Private::ConvertPx(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"px") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Pixel:
                return std::to_wstring(static_cast<short int>(dValue));;
            case Default:
            case Point:
                return ConvertPxToPt(dValue);
            case Cantimeter:
                return ConvertPxToCm(dValue);
            case Millimeter:
                return ConvertPxToMm(dValue);
            case Inch:
                return ConvertPxToIn(dValue);
            case Peak:
                return ConvertPxToPc(dValue);
        }

        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToCm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / static_cast<float>(m_nDpi) * 2.54f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToIn(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(1.0f / static_cast<float>(m_nDpi) * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToMm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / static_cast<float>(m_nDpi) * 25.4f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPc(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(0.16667f / static_cast<float>(m_nDpi) * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPt(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(72.0f /  static_cast<float>(m_nDpi) * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertCm(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"cm") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Default:
                ConvertCmToPt(dValue);
            case Pixel:
                return ConvertCmToPx(dValue);
            case Point:
                return ConvertCmToPt(dValue);
            case Cantimeter:
                return std::to_wstring(static_cast<short int>(dValue));;
            case Millimeter:
                return ConvertCmToMm(dValue);
            case Inch:
                return ConvertCmToIn(dValue);
            case Peak:
                return ConvertCmToPc(dValue);
        }

        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToIn(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 2.54f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToMm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 10.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPc(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(2.36f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPt(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(28.35f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPx(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 2.54f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertMm(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"mm") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Pixel:
                return ConvertMmToPx(dValue);
            case Default:
            case Point:
                return ConvertMmToPt(dValue);
            case Cantimeter:
                return ConvertMmToCm(dValue);
            case Millimeter:
                return std::to_wstring(static_cast<short int>(dValue));;
            case Inch:
                return ConvertMmToIn(dValue);
            case Peak:
                return ConvertMmToPc(dValue);
        }
        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToIn(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 25.4f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToCm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 10.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPc(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(2.8346f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPt(const float& dValue) const
    {
        if (dValue == 0)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(0.23262f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPx(const float& dValue) const
    {
        if (dValue == 0)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 25.4f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertIn(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"in") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Pixel:
                return  ConvertInToPx(dValue);
            case Default:
            case Point:
                return ConvertInToPt(dValue);
            case Cantimeter:
                return ConvertInToCm(dValue);
            case Millimeter:
                return ConvertInToMm(dValue);
            case Inch:
                return std::to_wstring(static_cast<short int>(dValue));;
            case Peak:
                return ConvertInToPc(dValue);
        }
        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertInToMm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 25.4f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertInToCm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 2.54f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertInToPc(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 72.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertInToPt(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 6.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertInToPx(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * static_cast<float>(m_nDpi) + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPt(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pt") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Pixel:
                return ConvertPtToPx(dValue);
            case Default:
            case Point:
                return std::to_wstring(static_cast<short int>(dValue));
            case Cantimeter:
                return ConvertPtToCm(dValue);
            case Millimeter:
                return ConvertPtToMm(dValue);
            case Inch:
                return ConvertPtToIn(dValue);
            case Peak:
                return ConvertPtToPc(dValue);
        }

        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToIn(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring((dValue / 72.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToCm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 0.03528f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToPc(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 12.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToMm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring((dValue * 0.3528f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToPx(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 72.0f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPc(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring();

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pc") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 2.0f;

        switch (m_UnitMeasure)
        {
            case Pixel:
                return ConvertPcToPx(dValue);
            case Default:
            case Point:
                return ConvertPcToPt(dValue);
            case Cantimeter:
                return ConvertPcToCm(dValue);
            case Millimeter:
                return ConvertPcToMm(dValue);
            case Inch:
                return ConvertPcToIn(dValue);
            case Peak:
                return std::to_wstring(static_cast<short int>(dValue));
        }

        return std::wstring();
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToIn(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue / 6.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToCm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 0.423f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToPt(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 12.0f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToMm(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(dValue * 4.23f + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToPx(const float& dValue) const
    {
        if (dValue == 0.0f)
            return std::wstring(L"0");

        return std::to_wstring(static_cast<short int>(static_cast<float>(m_nDpi) / 6.0f * dValue + 0.5f));
    }

    inline std::wstring CCssCalculator_Private::ConvertEm(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return std::wstring(L"");

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"em") - 1);
        const float dValue = wcstof(sConvertValue.c_str(), NULL) * 22.0f;

        return std::to_wstring(static_cast<short int>(dValue + 0.5f));
    }

    void CCssCalculator_Private::SetDpi(unsigned short int nValue)
    {
        m_nDpi = nValue;
    }

    void CCssCalculator_Private::SetUnitMeasure(const UnitMeasure& nType)
    {
        m_UnitMeasure = nType;
    }

    unsigned short int CCssCalculator_Private::GetDpi() const
    {
        return m_nDpi;
    }

    /*
    inline size_t CCssCalculator_Private::GetSize() const
    {
        return m_arData.size();
    }
    */

    UnitMeasure CCssCalculator_Private::GetUnitMeasure() const
    {
        return m_UnitMeasure;
    }

    std::wstring CCssCalculator_Private::GetEncoding() const
    {
        return m_sEncoding;
    }

    void CCssCalculator_Private::Clear()
    {
        m_sEncoding     = L"UTF-8";
        m_nDpi          = 96;
        m_UnitMeasure   = Default;

        for (CElement* oElement : m_arData)
            delete oElement;

//        m_arStyleUsed.clear();
        m_arData.clear();
        m_arFiles.clear();
    }
}
inline static std::wstring StringifyValueList(const KatanaArray* oValues)
{
    if (NULL == oValues)
        return std::wstring();

    std::wstring buffer;

    for (size_t i = 0; i < oValues->length; ++i)
    {
        KatanaValue* value = (KatanaValue*)oValues->data[i];
        buffer += StringifyValue(value);

        if ( i < oValues->length - 1 && value->unit != KATANA_VALUE_PARSER_OPERATOR )
        {
            if ( i < oValues->length - 2 )
            {
                value = (KatanaValue*)oValues->data[i + 1];
                if ( value->unit != KATANA_VALUE_PARSER_OPERATOR )
                    buffer += L" ";
            }
            buffer += L" ";
        }
    }

    return buffer;
}

inline static std::wstring StringifyValue(const KatanaValue* oValue)
{
    std::wstring str;

    switch (oValue->unit) {
        case KATANA_VALUE_NUMBER:
        case KATANA_VALUE_PERCENTAGE:
        case KATANA_VALUE_EMS:
        case KATANA_VALUE_EXS:
        case KATANA_VALUE_REMS:
        case KATANA_VALUE_CHS:
        case KATANA_VALUE_PX:
        case KATANA_VALUE_CM:
        case KATANA_VALUE_DPPX:
        case KATANA_VALUE_DPI:
        case KATANA_VALUE_DPCM:
        case KATANA_VALUE_MM:
        case KATANA_VALUE_IN:
        case KATANA_VALUE_PT:
        case KATANA_VALUE_PC:
        case KATANA_VALUE_DEG:
        case KATANA_VALUE_RAD:
        case KATANA_VALUE_GRAD:
        case KATANA_VALUE_MS:
        case KATANA_VALUE_S:
        case KATANA_VALUE_HZ:
        case KATANA_VALUE_KHZ:
        case KATANA_VALUE_TURN:
            str = NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->raw);
            break;
        case KATANA_VALUE_IDENT:
            str = NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->string);
            break;
        case KATANA_VALUE_STRING:
        {
            str = L"\"" + NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->string) + L"\"";
            break;
        }
        case KATANA_VALUE_PARSER_FUNCTION:
        {
            const std::wstring& args_str = StringifyValueList(oValue->function->args);
            if (args_str.empty())
                break;

            str = NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->function->name) + args_str + L")";
            break;
        }
        case KATANA_VALUE_PARSER_OPERATOR:
            str = L" ";
            if (oValue->iValue != '=')
            {
                str += static_cast<wchar_t>(oValue->iValue);
                str += L" ";
            }
            else
                str += static_cast<wchar_t>(oValue->iValue);
            break;
        case KATANA_VALUE_PARSER_LIST:
            return StringifyValueList(oValue->list);
            break;
        case KATANA_VALUE_PARSER_HEXCOLOR:
            str = L"#" + NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->string);
            break;
        case KATANA_VALUE_URI:
            str = L"url(" + NSCSS::NS_STATIC_FUNCTIONS::stringToWstring(oValue->string) + L")";
            break;
        default:
            break;
    }

    return str;
}


