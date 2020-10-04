#include "CCssCalculator_Private.h"

#include <string>
#include <vector>
#include <fstream>
#include <cwctype>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <numeric>

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
    CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_nCountNodes(0), m_UnitMeasure(Default), m_sEncoding(L"UTF-8") {}

    CCssCalculator_Private::~CCssCalculator_Private()
    {
        m_arFiles.clear();

        for (std::map<std::wstring, CElement*>::iterator oIter = m_mData.begin(); oIter != m_mData.end(); ++oIter)
            if (oIter->second != NULL)
                delete oIter->second;

        m_mData.clear();

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

        switch (oOutput->mode) {
            case KatanaParserModeStylesheet:
                GetStylesheet(oOutput->stylesheet);
                break;
            case KatanaParserModeRule:
                GetRule(oOutput->rule);
                break;
            case KatanaParserModeKeyframeRule:
            case KatanaParserModeKeyframeKeyList:
            case KatanaParserModeMediaList:
            case KatanaParserModeValue:
            case KatanaParserModeSelector:
            case KatanaParserModeDeclarationList:
                break;
        }

    }

    inline void CCssCalculator_Private::GetStylesheet(const KatanaStylesheet *oStylesheet)
    {
        for (size_t i = 0; i < oStylesheet->imports.length; ++i)
            GetRule((KatanaRule*)oStylesheet->imports.data[i]);

        for (size_t i = 0; i < oStylesheet->rules.length; ++i)
            GetRule((KatanaRule*)oStylesheet->rules.data[i]);
    }

    inline void CCssCalculator_Private::GetRule(const KatanaRule *oRule)
    {
        if ( NULL == oRule )
            return;

        switch (oRule->type) {
            case KatanaRuleStyle:
                GetStyleRule((KatanaStyleRule*)oRule);
                break;
            case KatanaRuleImport:
            case KatanaRuleFontFace:
            case KatanaRuleKeyframes:
            case KatanaRuleMedia:
            case KatanaRuleSupports:
            case KatanaRuleUnkown:
            default:
                break;
        }
    }

    inline void CCssCalculator_Private::GetStyleRule(const KatanaStyleRule *oRule)
    {
        if (oRule->declarations->length == 0)
            return;

        std::vector<std::vector<std::wstring>> arValues;

        for (const std::wstring sSelector : GetSelectorList(oRule->selectors))
        {
            size_t posLastSpace = 0;
            while (posLastSpace != std::wstring::npos)
            {
                const size_t posPoint = sSelector.find(L'.', posLastSpace);
                const size_t posLattice = sSelector.find(L'#', posLastSpace);
                const size_t posSpace = (posLattice != std::wstring::npos) ? sSelector.find(L' ', posLattice) : (posPoint != std::wstring::npos) ? sSelector.find(L' ', posPoint) : sSelector.find(L' ', posLastSpace + 1);

                const std::wstring sName = (posPoint != std::wstring::npos) ? sSelector.substr(posLastSpace, posPoint) : (posLattice != std::wstring::npos) ? sSelector.substr(posLastSpace, posLattice) : sSelector.substr(posLastSpace + 1, sSelector.length() - 1);
                const std::wstring sClasses = (posPoint != std::wstring::npos) ? (posLattice == std::wstring::npos) ? (posSpace == std::wstring::npos) ? sSelector.substr(posPoint, sSelector.length() - 1) : sSelector.substr(posPoint, posSpace) : sSelector.substr(posPoint, posLattice) : L"";
                const std::wstring sId = (posLattice != std::wstring::npos) ? (posSpace == std::wstring::npos) ? sSelector.substr(posLattice, sSelector.length() - 1) : sSelector.substr(posLattice, posSpace) : L"";

                arValues.push_back({sName, sClasses, sId});

                posLastSpace = posSpace;
            }
        }

        CElement* oNameElement = NULL;
        CElement* oClassElement = NULL;
        CElement* oIdElement = NULL;

        CElement* oLastElement = NULL;
        CElement* oFirstElement = NULL;

//        for (const std::vector<std::wstring>& arItems : arValues)
        for (short int nIndex = arValues.size() - 1; nIndex > -1; --nIndex)
        {
            const std::vector<std::wstring>& arItems = arValues[nIndex];
            for (unsigned short int nIndex = 0; nIndex < arItems.size(); ++nIndex)
            {
                switch (nIndex)
                {
                    case 0://Name
                    {
                        if (arItems[0].empty())
                            continue;

                        oNameElement = new CElement;
                        oNameElement->SetSelector(arItems[0]);
                        break;
                    }
                    case 1://Classes
                    {
                        if (arItems[1].empty())
                            continue;

                        oClassElement = new CElement;
                        oClassElement->SetSelector(arItems[1]);
                        break;
                    }
                    case 2://Id
                    {
                        if (arItems[2].empty())
                            continue;

                        oIdElement = new CElement;
                        oIdElement->SetSelector(arItems[2]);

                        break;
                    }
                }
            }

            if (NULL != oClassElement)
            {
                const std::map<std::wstring, CElement*>::const_iterator& oFindClass = m_mData.find(oClassElement->GetSelector());

                if (oFindClass != m_mData.end())
                {
                    delete oClassElement;
                    oClassElement = oFindClass->second;
                }
                else if (NULL == oFirstElement)
                    oFirstElement = oClassElement;

                oLastElement = oClassElement;
            }

            if (NULL != oNameElement)
            {
                const std::map<std::wstring, CElement*>::const_iterator& oFindName = m_mData.find(oNameElement->GetSelector());

                if (oFindName != m_mData.end())
                {
                    if (NULL != oClassElement)
                        oNameElement->AddProperties(oFindName->second->GetStyle());
//                    else
//                    {
//                        delete oClassElement;
//                        oClassElement = oFindName->second;
//                    }
                }

                if (NULL == oClassElement && NULL == oFirstElement)
                    oFirstElement = oNameElement;

                if (NULL != oClassElement)
                    oClassElement->AddPrevElement(oNameElement);

                oLastElement = oNameElement;

            }

            oNameElement = NULL;
            oClassElement = NULL;
            oIdElement = NULL;
        }


        if (NULL != oFirstElement)
            m_mData[oFirstElement->GetSelector()] = oFirstElement;

        if (NULL != oLastElement)
            oLastElement->AddProperties(GetDeclarationList(oRule->declarations));
    }

    inline std::vector<std::wstring> CCssCalculator_Private::GetSelectorList(const KatanaArray* oSelectors) const
    {
        if (oSelectors->length == 0)
            return std::vector<std::wstring>();

        std::vector<std::wstring> arSelectors;

        for (unsigned int i = 0; i < oSelectors->length; ++i)
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

        return sText;
    }

    inline std::map<std::wstring, std::wstring> CCssCalculator_Private::GetDeclarationList(const KatanaArray* oDeclarations) const
    {
        if(oDeclarations->length == 0)
            return std::map<std::wstring, std::wstring>();

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

//        std::vector<CNode> oNodes = NS_STATIC_FUNCTIONS::ConvertSelectorsToNode(sSelector);

//        m_mData[oNodes] = {};
    }

    inline void CCssCalculator_Private::GetFontFaceRule(const KatanaFontFaceRule *oRule)
    {
        const std::wstring sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name);

//        const std::vector<CNode>& arNodes = NS_STATIC_FUNCTIONS::ConvertSelectorsToNode(sSelector);

//        const auto& oFind = m_mData.find(arNodes);
        std::map<std::wstring, std::wstring>* mStyle = new std::map<std::wstring, std::wstring>();
        *mStyle = GetDeclarationList(oRule->declarations);

//        if (oFind != m_mData.end())
//            for (const std::pair<std::wstring, std::wstring>& oItem : *mStyle)
//                (*oFind->second)[oItem.first] = oItem.second;
//        else
//            m_mData[arNodes] = mStyle;

//        std::map<std::wstring, std::wstring>& oStyle = m_mData[NS_STATIC_FUNCTIONS::ConvertSelectorsToNode(sSelector)];

//        for (const std::pair<std::wstring, std::wstring> oItem : GetDeclarationList(oRule->declarations))
//            oStyle[oItem.first] = oItem.second;
    }

    inline void CCssCalculator_Private::GetKeyframesRule(const KatanaKeyframesRule *oRule)
    {
        const std::wstring& sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name);

//        oElement->AddSelector(sSelector);

//        for (size_t i = 0; i < oRule->keyframes->length; ++i)
//            oElement->AddChildren(GetKeyframe((KatanaKeyframe*)oRule->keyframes->data[i]));

//        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetKeyframe(const KatanaKeyframe *oKeyframe)
    {
//        if (oKeyframe == NULL)
//            return NULL;

//        CElement *oElement = new CElement;

//        for (size_t i = 0; i < oKeyframe->selectors->length; ++i)
//        {
//            const KatanaValue* oValue = (KatanaValue*)oKeyframe->selectors->data[i];
//            if (oValue->unit == KATANA_VALUE_NUMBER)
//                oElement->AddSelector(NS_STATIC_FUNCTIONS::stringToWstring(oValue->raw));
//        }

//        oElement->AddDeclarations(GetDeclarationList(oKeyframe->declarations));

//        return oElement;
    }

    inline void CCssCalculator_Private::GetMediaRule(const KatanaMediaRule *oRule)
    {
        std::wstring sSelector = L"@" + NS_STATIC_FUNCTIONS::stringToWstring(oRule->base.name) + L" ";

//        CElement *oElement = new CElement;

//        if (oRule->medias->length)
//            sSelector += GetMediaList(oRule->medias);

//        oElement->AddSelector(sSelector);

//        if (oRule->medias->length)
//            for (size_t i = 0; i < oRule->rules->length; ++i)
//                oElement->AddChildren(GetStyleRule((KatanaStyleRule*)oRule->rules->data[i], oElement));

//        m_arData.push_back(oElement);
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
        if (sSelector.empty() || m_mData.empty())
            return std::map<std::wstring, std::wstring>();

//        const std::map<std::vector<CNode>, std::map<std::wstring, std::wstring>*>::const_iterator& oFind = m_mData.find(NS_STATIC_FUNCTIONS::ConvertSelectorsToNode(sSelector));

//        if (oFind != m_mData.end())
//            return *oFind->second;

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
            else if (sSel.find_first_of(".[]") != std::string::npos/*
                     sSel.find('.') != std::string::npos ||
                     sSel.find('[') != std::string::npos ||
                     sSel.find(']') != std::string::npos*/)
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
        std::wcout << m_mData.size() << std::endl;

//        for (const CElement* oElement : m_arData)
//            std::wcout << oElement->GetText() << std::endl;
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

        std::map<std::wstring, std::wstring> mStyle;

        for (const std::string& sSelector : arSelectors)
        {
            for (const std::pair<std::wstring, std::wstring>& oDeclarations : GetDeclarations(NS_STATIC_FUNCTIONS::stringToWstring(sSelector)))
            {
                if (mStyle.find(oDeclarations.first) == mStyle.end())
                {
                    mStyle[oDeclarations.first] = oDeclarations.second;
                    continue;
                }

                if (mStyle[oDeclarations.first].find(L"important") == std::wstring::npos)
                {
//                    std::wcout << oDeclarations.first << L" : " << mStyle[oDeclarations.first].c_str() << " <- " << oDeclarations.second << std::endl;
                    mStyle[oDeclarations.first] = oDeclarations.second;

//                    const size_t posEm = oDeclarations.second.find(L"em");
//                    if (posEm != std::wstring::npos)
//                    {
//                        std::wcout << L"INPUT: " << mStyle[oDeclarations.first] << L" - " << oDeclarations.second << std::endl;

//                        const float fCurrentValue = (mStyle[oDeclarations.first].empty() || mStyle[oDeclarations.first] == L"22") ? 22.0f : wcstof(ConvertUnitMeasure(mStyle[oDeclarations.first]).c_str(), NULL);
//                        const float fValue = wcstof(oDeclarations.second.c_str(), NULL);

//                        std::wcout << fCurrentValue << L" * " << fValue << L" = " << fValue * fCurrentValue << std::endl;

//                        mStyle[oDeclarations.first] = std::to_wstring(static_cast<short int>(fValue * fCurrentValue + 0.5f));
//                        std::wcout << L"-----" << mStyle[oDeclarations.first] << std::endl;
//                        std::wcout << L"Shipped: " << oDeclarations.second << L" + " << mStyle[oDeclarations.first] << std::endl;
//                        mStyle[oDeclarations.first] = ConvertEm(oDeclarations.second, mStyle[oDeclarations.first]);
//                        std::wcout << L"-----" << mStyle[oDeclarations.first] << std::endl;
//                        continue;
//                    }
                }
            }
        }

//        std::cout << std::endl;

        for (const std::pair<std::wstring, std::wstring>& oIter : mStyle)
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

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode>& arSelectors, const UnitMeasure& unitMeasure)
    {
        if (arSelectors.empty())
            return CCompiledStyle();

        CCompiledStyle *oStyle = new CCompiledStyle();

        std::wstring sSelector;

        for (short int nIndex = arSelectors.size() - 1; nIndex > -1; --nIndex)
        {
            sSelector += (arSelectors[nIndex].m_sName) + ((!arSelectors[nIndex].m_sClass.empty()) ? L"." + arSelectors[nIndex].m_sClass : L"") + L" ";
        }

        for (short int nIndex = arSelectors.size() - 1; nIndex > -1; --nIndex)
        {
            const std::map<std::wstring, CElement*>::const_iterator oFind = m_mData.find((!arSelectors[nIndex].m_sClass.empty()) ? arSelectors[nIndex].m_sClass : arSelectors[nIndex].m_sName);

            if (oFind != m_mData.end())
            {
//                std::wcout << oFind->second->GetSelector() << std::endl;
                oStyle->AddStyle(oFind->second->GetStyle());
            }
            oStyle->AddStyle(arSelectors[nIndex].m_sStyle);
        }
        sSelector.pop_back();

//        std::wcout << sSelector << std::endl;

        oStyle->SetID(arSelectors.back().m_sName + ((!arSelectors.back().m_sClass.empty()) ? L'.' + arSelectors.back().m_sClass : L"") + ((arSelectors.back().m_sId.empty()) ? L"" : L'.' + arSelectors.back().m_sId) + L'-' + std::to_wstring(++m_nCountNodes));
//        std::wcout << oStyle->GetStyleW() << std::endl;

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
            case Point:
                return ConvertCmToPt(dValue);
            case Pixel:
                return ConvertCmToPx(dValue);
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

        const float fValue = wcstof(sValue.c_str(), NULL) * 22.0f;
        return std::to_wstring(static_cast<short int>(fValue + 0.5f));
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

//        for (CElement* oElement : m_arData)
//            delete oElement;

//        m_arStyleUsed.clear();
        m_mData.clear();
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


