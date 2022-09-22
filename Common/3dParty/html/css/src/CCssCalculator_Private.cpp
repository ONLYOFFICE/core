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

#define MaxNumberRepetitions 6

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
    CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_nCountNodes(0), m_UnitMeasure(Default), m_mStatictics(NULL), m_sEncoding(L"UTF-8"){}

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

        if (NULL != m_mStatictics)
            delete m_mStatictics;
    }

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
        for (const std::wstring &sSelector : GetSelectorList(oRule->selectors))
        {
            std::vector<std::wstring> arWords = NS_STATIC_FUNCTIONS::GetWordsW(sSelector, L" ");

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

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode>& arSelectors, const bool& bIsSettings, const UnitMeasure& unitMeasure)
    {
        if (arSelectors.empty())
            return CCompiledStyle();

        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);

        if (!bIsSettings)
        {
            const std::map<std::vector<CNode>, CCompiledStyle*>::iterator oItem = m_mUsedStyles.find(arSelectors);

            if (oItem != m_mUsedStyles.end())
                return *oItem->second;
        }
        else if (NULL == m_mStatictics || m_mStatictics->empty())
        {
            CCompiledStyle oStyle;
            oStyle.SetDpi(m_nDpi);
            oStyle.SetUnitMeasure(m_UnitMeasure);
            oStyle.SetID(arSelectors.back().m_sName + ((!arSelectors.back().m_sClass.empty()) ? L'.' + arSelectors.back().m_sClass : L"") + ((arSelectors.back().m_sId.empty()) ? L"" : L'#' + arSelectors.back().m_sId) + L'-' + std::to_wstring(++m_nCountNodes));

            oStyle.SetSizeDeviceWindow(m_oDeviceWindow);
            oStyle.SetSizeSourceWindow(m_oSourceWindow);

            return oStyle;
        }

        CCompiledStyle *pStyle = new CCompiledStyle();

        pStyle->SetDpi(m_nDpi);
        pStyle->SetUnitMeasure(m_UnitMeasure);

        pStyle->SetSizeDeviceWindow(m_oDeviceWindow);
        pStyle->SetSizeSourceWindow(m_oSourceWindow);

        std::vector<std::wstring> arWords;
        arWords.reserve(arSelectors.size() * 2);

        std::vector<std::wstring> arNextNodes;
        arNextNodes.reserve(arSelectors.size() * 2);

        for (std::vector<CNode>::const_reverse_iterator oNode = arSelectors.rbegin(); oNode != arSelectors.rend(); ++oNode)
        {
            arWords.push_back(oNode->m_sName);

            if (oNode->m_sName == L"td")
                pStyle->m_pMargin.SetPermission(false);

            if (oNode->m_sName == L"table")
                pStyle->m_pBorder.Block();

            if (!oNode->m_sClass.empty())
            {
                if (oNode->m_sClass.find(L' ') != std::wstring::npos)
                {
                    std::vector<std::wstring> arClasses = NS_STATIC_FUNCTIONS::GetWordsW(oNode->m_sClass, L" ");

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
                    arWords.push_back(L'.' + oNode->m_sClass);
            }
            if (!oNode->m_sId.empty())
                arWords.push_back(L'#' + oNode->m_sId);
        }

        std::vector<CElement*> arElements;

        for (size_t i = 0; i < arSelectors.size(); ++i)
        {
            std::wstring sName, sId;
            std::vector<std::wstring> arClasses;

            if (arWords.back()[0] == L'#')
            {
                sId = arWords.back();
                arWords.pop_back();
                arNextNodes.push_back(sId);
            }

            if (arWords.back()[0] == L'.')
            {
                arClasses = NS_STATIC_FUNCTIONS::GetWordsW(arWords.back(), L" ");
                arNextNodes.push_back(arWords.back());
                arWords.pop_back();
            }

            sName = arWords.back();
            arWords.pop_back();
            arNextNodes.push_back(sName);
            pStyle->AddParent(sName);

            const std::map<std::wstring, CElement*>::const_iterator oFindName = m_mData.find(sName);
            std::map<std::wstring, CElement*>::const_iterator oFindId;
            std::vector<CElement*> arFindElements;

            if (!sId.empty())
            {
                oFindId = m_mData.find(sId);

                if (oFindId != m_mData.end())
                {
                    std::map<StatistickElement, unsigned int>::const_iterator oFindCountId = m_mStatictics->find(StatistickElement{StatistickElement::IsId, sId});

					if ((m_mStatictics->end() != oFindCountId) &&
					   (((bIsSettings && oFindCountId->second < MaxNumberRepetitions) ||
						 (!bIsSettings && oFindCountId->second >= MaxNumberRepetitions))))
                    {
                        if (!oFindId->second->Empty())
                            arFindElements.push_back(oFindId->second);
                    }

                    const std::vector<CElement*> arTempPrev = oFindId->second->GetPrevElements(arNextNodes.rbegin() + ((arClasses.empty()) ? 1 : 2), arNextNodes.rend());

                    if (!arTempPrev.empty())
                        arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());
                }
            }

            if (!arClasses.empty())
            {
                if (!bIsSettings)
                {
                    for (std::vector<std::wstring>::const_reverse_iterator iClass = arClasses.rbegin(); iClass != arClasses.rend(); ++iClass)
                    {
                        const std::map<std::wstring, CElement*>::const_iterator oFindClass = m_mData.find(*iClass);
                        if (oFindClass != m_mData.end())
                        {
                            if (!oFindClass->second->Empty())
                                arFindElements.push_back(oFindClass->second);

                            const std::vector<CElement*> arTempPrev = oFindClass->second->GetPrevElements(arNextNodes.rbegin() + 2, arNextNodes.rend());
                            const std::vector<CElement*> arTempKins = oFindClass->second->GetNextOfKin(sName);

                            if (!arTempPrev.empty())
                                arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());

                            if (!arTempKins.empty())
                                arFindElements.insert(arFindElements.end(), arTempKins.begin(), arTempKins.end());
                        }
                    }
                }
            }

            if (oFindName != m_mData.end())
            {
                if (!bIsSettings)
                {
                    if (!oFindName->second->Empty())
                        arFindElements.push_back(oFindName->second);

                    const std::vector<CElement*> arTempPrev = oFindName->second->GetPrevElements(arNextNodes.rbegin() + 1, arNextNodes.rend());
                    const std::vector<CElement*> arTempKins = oFindName->second->GetNextOfKin(sName, arClasses);

                    if (!arTempPrev.empty())
                        arFindElements.insert(arFindElements.end(), arTempPrev.begin(), arTempPrev.end());

                    if (!arTempKins.empty())
                        arFindElements.insert(arFindElements.end(), arTempKins.begin(), arTempKins.end());
                }
            }


            if (arFindElements.size() > 1)
            {
                std::sort(arFindElements.rbegin(), arFindElements.rend(),
                          [](CElement* oFirstElement, CElement* oSecondElement)
                          {
                              return oFirstElement->GetWeight() > oSecondElement->GetWeight();
                          });
            }

            pStyle->AddStyle(arSelectors[i].m_mAttrs, i + 1);

            for (const CElement* oElement : arFindElements)
                pStyle->AddStyle(oElement->GetStyle(), i + 1);

            std::map<StatistickElement, unsigned int>::const_iterator oFindCountStyle = m_mStatictics->find(StatistickElement{StatistickElement::IsStyle, arSelectors[i].m_sStyle});

            if (oFindCountStyle != m_mStatictics->end())
            {
                if ((bIsSettings && oFindCountStyle->second <  MaxNumberRepetitions) ||
                   (!bIsSettings && oFindCountStyle->second >= MaxNumberRepetitions))
                    pStyle->AddStyle(arSelectors[i].m_sStyle, i + 1,  true);
                else if (!bIsSettings)
                    pStyle->AddStyle(arSelectors[i].m_sStyle, i + 1, true);
            }
            else if (bIsSettings)
                pStyle->AddStyle(arSelectors[i].m_sStyle, i + 1, true);
        }

        if (!bIsSettings)
        {
            pStyle->SetID(arSelectors.back().m_sName + ((!arSelectors.back().m_sClass.empty()) ? L'.' + arSelectors.back().m_sClass : L"") + ((arSelectors.back().m_sId.empty()) ? L"" : L'#' + arSelectors.back().m_sId) + L'-' + std::to_wstring(++m_nCountNodes));
            m_mUsedStyles[arSelectors] = pStyle;
        }

        return *pStyle;
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

    void CCssCalculator_Private::SetDpi(unsigned short int nValue)
    {
        m_nDpi = nValue;
    }

    void CCssCalculator_Private::SetBodyTree(const CTree &oTree)
    {
        if (NULL == m_mStatictics)
            m_mStatictics = new std::map<StatistickElement, unsigned int>();

        CTree::CountingNumberRepetitions(oTree, *m_mStatictics);
    }

    void CCssCalculator_Private::SetSizeSourceWindow(const CSizeWindow &oSizeWindow)
    {
            m_oSourceWindow = oSizeWindow;
    }

    void CCssCalculator_Private::SetSizeDeviceWindow(const CSizeWindow &oSizeWindow)
    {
            m_oDeviceWindow = oSizeWindow;
    }

    CSizeWindow CCssCalculator_Private::GetSizeSourceWindow() const
    {
            return m_oSourceWindow;
    }

    CSizeWindow CCssCalculator_Private::GetSizeDeviceWindow() const
    {
            return m_oDeviceWindow;
    }

    void CCssCalculator_Private::SetUnitMeasure(const UnitMeasure& nType)
    {
        m_UnitMeasure = nType;
    }

    unsigned short int CCssCalculator_Private::GetDpi() const
    {
        return m_nDpi;
    }


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

        m_mData.clear();
        m_arFiles.clear();

        m_oDeviceWindow.Clear();
        m_oSourceWindow.Clear();
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


