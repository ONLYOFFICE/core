#include "CCssCalculator_Private.h"

#include <string>
#include <vector>
#include <fstream>
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

        const std::map<std::wstring, std::wstring> mStyle = GetDeclarationList(oRule->declarations);
        for (const std::wstring sSelector : GetSelectorList(oRule->selectors))
        {
            std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sSelector, L" ");
//            std::wcout << sSelector << std::endl;

            CElement* oLastElement = NULL;
            CElement* oFirstElement = NULL;
            bool bCreateFirst = true;

            for (std::vector<std::wstring>::reverse_iterator oWord = arWords.rbegin(); oWord != arWords.rend(); ++oWord)
            {
                    const size_t posPoint = oWord->find(L'.');
                    const size_t posLattice = oWord->find(L'#');

                    const std::wstring sName = (posPoint != std::wstring::npos) ? oWord->substr(0, posPoint) : (posLattice != std::wstring::npos) ? oWord->substr(0, posLattice) : *oWord;
                    const std::wstring sClass = (posPoint != std::wstring::npos) ? (posLattice == std::wstring::npos) ? oWord->substr(posPoint, oWord->length()) : oWord->substr(posPoint, posLattice - posPoint) : L"";
                    const std::wstring sId = (posLattice != std::wstring::npos) ? oWord->substr(posLattice, oWord->length()) : L"";

                    CElement* oNameElement = NULL;
                    CElement* oClassElement = NULL;
                    CElement* oIdElement = NULL;
                    bool bIsNewElement = true;

                    if (!sId.empty())
                    {
                        if (NULL == oFirstElement && bCreateFirst)
                        {
                            const std::map<std::wstring, CElement*>::const_iterator& oFindId = m_mData.find(sId);
                            if (oFindId != m_mData.end())
                            {
                                oIdElement = oFindId->second;
                                bCreateFirst = false;
                            }
                            else
                            {
                                oIdElement = new CElement;
                                oIdElement->SetSelector(sId);
                                if (bCreateFirst)
                                    oFirstElement = oIdElement;
                            }
                        }
                        else
                        {
                            oIdElement = new CElement;
                            oIdElement->SetSelector(sId);

                            oLastElement->AddPrevElement(oIdElement);
                        }
                        bIsNewElement = false;
                        oLastElement = oIdElement;
                    }

                    if (!sClass.empty())
                    {
                        if (NULL == oFirstElement && bCreateFirst)
                        {
                            const std::map<std::wstring, CElement*>::const_iterator& oFindClass = m_mData.find(sClass);
                            if (oFindClass != m_mData.end())
                            {
                                oClassElement = oFindClass->second;
                                bCreateFirst = false;
                            }
                            else
                            {
                                oClassElement = new CElement;
                                oClassElement->SetSelector(sClass);
                                if (bCreateFirst)
                                    oFirstElement = oClassElement;
                            }
                        }
                        else
                        {
                            oClassElement = new CElement;
                            oClassElement->SetSelector(sClass);

                            if (bIsNewElement)
                                oLastElement->AddPrevElement(oClassElement);
                            else
                                oLastElement->AddKinElement(oClassElement);
                        }

                        bIsNewElement = false;
                        oLastElement = oClassElement;
                    }

                    if (!sName.empty())
                    {
                        if (NULL == oFirstElement && bCreateFirst)
                        {
                            const std::map<std::wstring, CElement*>::const_iterator& oFindName = m_mData.find(sName);
                            if (oFindName != m_mData.end())
                            {
                                oNameElement = oFindName->second;
                                bCreateFirst = false;
                            }
                            else
                            {
                                oNameElement = new CElement;
                                oNameElement->SetSelector(sName);
                                if (bCreateFirst)
                                    oFirstElement = oNameElement;
                            }
                        }
                        else
                        {
                            oNameElement = new CElement;
                            oNameElement->SetSelector(sName);

                            if (bIsNewElement)
                                oLastElement->AddPrevElement(oNameElement);
                            else
                                oLastElement->AddKinElement(oNameElement);

                        }
                        oLastElement = oNameElement;
                    }
            }

            if (NULL != oLastElement)
                oLastElement->AddProperties(mStyle);

            if (NULL != oFirstElement)
                m_mData[oFirstElement->GetSelector()] = oFirstElement;
        }
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

    inline std::wstring CCssCalculator_Private::GetValueList(const KatanaArray *oValues)
    {
        return StringifyValueList(oValues);
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode>& arSelectors, const UnitMeasure& unitMeasure)
    {
        if (arSelectors.empty())
            return CCompiledStyle();

        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);

        const std::map<std::vector<CNode>, CCompiledStyle*>::iterator oItem = m_mUsedStyles.find(arSelectors);

        if (oItem != m_mUsedStyles.end())
            return *oItem->second;

        CCompiledStyle *oStyle = new CCompiledStyle();

        std::vector<std::wstring> arWords;
        arWords.reserve(arSelectors.size() * 2);

        for (const CNode& oNode : arSelectors)
        {
            oStyle->AddStyle(oNode.m_sStyle, true);

            arWords.push_back(oNode.m_sName);

            if (!oNode.m_sClass.empty())
            {
                if (oNode.m_sClass.find(L' ') != std::wstring::npos)
                {
                    std::vector<std::wstring> arClasses = NS_STATIC_FUNCTIONS::GetWordsW(oNode.m_sClass, L" ");

                    if (arClasses.size() > 1)
                        arClasses.resize(unique(arClasses.begin(),arClasses.end()) - arClasses.begin());
                    switch (arClasses.size())
                    {
                        case 1:
                        {
                            arWords.push_back(L'.' + arClasses[0]);
                            break;
                        }
                        case 2:
                        {
                            arWords.push_back(L'.' + arClasses[0] + L" ." + arClasses[1]);
                            break;
                        }
                        case 3:
                        {
                            arWords.push_back(L'.' + arClasses[0] + L" ." + arClasses[1] + L" ." + arClasses[2]);
                            break;
                        }
                        default:
                        {
                            arWords.push_back(std::accumulate(arClasses.begin(), arClasses.end(), std::wstring(),
                                                              [](std::wstring sRes, const std::wstring& sClass)
                                                                {return sRes += L'.' + sClass + L' ';}));
                            break;
                        }
                    }
                }
                else
                    arWords.push_back(L'.' + oNode.m_sClass);
            }
            if (!oNode.m_sId.empty())
                arWords.push_back(L'#' + oNode.m_sId);
        }

        std::vector<CElement*> arElements;

        for (std::vector<CNode>::const_reverse_iterator iNode = arSelectors.rbegin(); iNode != arSelectors.rend(); ++iNode)
        {
            std::vector<std::wstring> arKins;
            for (std::vector<std::wstring>::const_reverse_iterator sNode = arWords.rbegin(); sNode != arWords.rend(); ++sNode)
            {
                arKins.push_back(*sNode);
                if ((*sNode)[0] != L'#' && (*sNode)[0] != L'.')
                    break;
            }
            std::wstring sName, sId;
            std::vector<std::wstring> arClasses;

            if (arWords.back()[0] == L'#')
            {
                sId = arWords.back();
                arWords.pop_back();
            }

            if (arWords.back()[0] == L'.')
            {
                arClasses = NS_STATIC_FUNCTIONS::GetWordsW(arWords.back(), L" ");
                arWords.pop_back();
            }

            sName = arWords.back();
            oStyle->AddParent(sName);

            const std::map<std::wstring, CElement*>::const_iterator oFindName = m_mData.find(sName);
            std::map<std::wstring, CElement*>::const_iterator oFindId;
            std::vector<CElement*> arFindElements;


            if (!sId.empty())
            {
                oFindId = m_mData.find(sId);

                if (oFindId != m_mData.end())
                {
                    const std::vector<CElement*> arTemp1 = oFindId->second->GetNextOfKin(sName, arClasses);
                    const std::vector<CElement*> arTemp2 = oFindId->second->GetAllElements(arWords);

                    arFindElements.insert(arFindElements.end(), arTemp1.begin(), arTemp1.end());
                    arFindElements.insert(arFindElements.end(), arTemp2.begin(), arTemp2.end());
                }
            }

            if (!arClasses.empty())
            {
                for (std::vector<std::wstring>::const_reverse_iterator iClass = arClasses.rbegin(); iClass != arClasses.rend(); ++iClass)
                {
                    const std::map<std::wstring, CElement*>::const_iterator oFindClass = m_mData.find(*iClass);
                    if (oFindClass != m_mData.end())
                    {
                        arFindElements.push_back(oFindClass->second);
                        const std::vector<CElement*> arTemp1 = oFindClass->second->GetNextOfKin(sName);
                        const std::vector<CElement*> arTemp2 = oFindClass->second->GetAllElements(arWords);

                        arFindElements.insert(arFindElements.end(), arTemp1.begin(), arTemp1.end());
                        arFindElements.insert(arFindElements.end(), arTemp2.begin(), arTemp2.end());
                    }
                }
            }

            if (oFindName != m_mData.end())
            {
                const std::vector<CElement*> arTemp = oFindName->second->GetAllElements(arWords);
                arFindElements.insert(arFindElements.end(), arTemp.begin(), arTemp.end());
            }

            std::sort(arFindElements.rbegin(), arFindElements.rend(), [](CElement* oFirstElement, CElement* oSecondElement)
            {
                return oFirstElement->GetWeight() < oSecondElement->GetWeight();
            });

            for (const CElement* oElement : arFindElements)
                oStyle->AddStyle(oElement->GetStyle());

            arWords.pop_back();
        }

        oStyle->SetID(arSelectors.back().m_sName + ((!arSelectors.back().m_sClass.empty()) ? L'.' + arSelectors.back().m_sClass : L"") + ((arSelectors.back().m_sId.empty()) ? L"" : L'#' + arSelectors.back().m_sId) + L'-' + std::to_wstring(++m_nCountNodes));

        for (std::pair<const std::wstring, std::wstring>& pProperie : *(oStyle->GetStyleMap()))
            pProperie.second = ConvertUnitMeasure(pProperie.second);

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
            const size_t nPosImportant = sValueTemp.find(L'!');
            if (nPosImportant != std::wstring::npos)
                sValueTemp = sValueTemp.substr(0, nPosImportant);

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
                const float dValue = wcstof(sValueTemp.substr(0, posPercent).c_str(), NULL) * NS_CONST_VALUES::FONT_SIZE / 100;

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

        const float fValue = wcstof(sValue.c_str(), NULL) * NS_CONST_VALUES::FONT_SIZE;
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


