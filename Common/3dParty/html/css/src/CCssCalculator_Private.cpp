#include "CCssCalculator_Private.h"

#include <string>
#include <wchar.h>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>
#include <math.h>

#include <iostream>

#include "../../katana-parser/src/selector.h"
#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../../DesktopEditor/common/File.h"


inline static std::wstring      StringifyValueList(KatanaArray* oValues);
inline static std::wstring      StringifyValue(KatanaValue* oValue);
inline static std::wstring      stringToWstring(const std::string& sString);
inline static std::string       wstringToString(const std::wstring& sWstring);
inline static std::string       GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding);
inline static std::string       GetContentAsUTF8(const std::wstring& sFileName);
inline static bool              ThereIsNumber(const std::wstring& sString);
inline static std::wstring      ConvertAbsoluteValue(const std::wstring& sAbsoluteValue);
inline std::vector<std::string> GetWords(const std::wstring& sLine);
inline std::vector<std::wstring> GetWordsW(const std::wstring& sLine);
inline std::vector<std::string> GetSelectorsList(const std::wstring& sSelectors);
inline std::wstring             DeleteSpace(const std::wstring& sValue);
inline static void              RemoveExcessFromStyles(std::wstring& sStyle);
inline static void              TranslateToEn(std::wstring& sStyle);


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
    CCssCalculator_Private::CCssCalculator_Private() : m_nDpi(96), m_sEncoding(L"UTF-8"), m_nCountNodes(1), m_UnitMeasure(Default) {}

    CCssCalculator_Private::~CCssCalculator_Private()
    {
        for (size_t i = 0; i < m_arData.size(); ++i)
            delete[] m_arData[i];

        for (std::map<std::vector<CNode>, CCompiledStyle*>::iterator iter  = m_mUsedStyles.begin(); iter != m_mUsedStyles.end(); ++iter)
            delete iter->second;
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

        (oElement->GetCountSelectors() > 0 || oElement->GetCountDeclarations() > 0) ? m_arData.push_back(oElement) : delete oElement;
    }

    inline void CCssCalculator_Private::GetStylesheet(KatanaStylesheet *oStylesheet, CElement *elementRule)
    {
        for (size_t i = 0; i < oStylesheet->imports.length; ++i)
        {
            GetRule((KatanaRule*)oStylesheet->imports.data[i], elementRule);
        }
        for (size_t i = 0; i < oStylesheet->rules.length; ++i)
        {
            GetRule((KatanaRule*)oStylesheet->rules.data[i], elementRule);
        }
    }

    inline void CCssCalculator_Private::GetRule(KatanaRule *oRule, CElement *oElementRule)
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

    inline CElement* CCssCalculator_Private::GetStyleRule(KatanaStyleRule *oRule, CElement *oElementRule)
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

    inline std::vector<std::wstring> CCssCalculator_Private::GetSelectorList(KatanaArray* oSelectors) const
    {
        std::vector<std::wstring> arSelectors;

        for (size_t i = 0; i < oSelectors->length; ++i)
            arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));

        return arSelectors;
    }

    inline std::wstring CCssCalculator_Private::GetSelector(KatanaSelector *oSelector) const
    {
        KatanaParser oParser;
        oParser.options = &kKatanaDefaultOptions;

        std::wstring sText;
        const KatanaParserString* string = katana_selector_to_string(&oParser, oSelector, NULL);
        const char* text = katana_string_to_characters(&oParser, string);

        katana_parser_deallocate(&oParser, (void*) string->data);
        katana_parser_deallocate(&oParser, (void*) string);

        sText = stringToWstring(text);

        katana_parser_deallocate(&oParser, (void*) text);

        if (sText.find(L' ') != std::wstring::npos)
        {
            std::wstring sTempText;
            for (const wchar_t& wc : sText)
                if (!iswspace(wc))
                    sTempText += wc;

            return sTempText;
        }

        return sText;
    }

    inline std::map<std::wstring, std::wstring> CCssCalculator_Private::GetDeclarationList(KatanaArray* oDeclarations) const
    {
        std::map<std::wstring, std::wstring> arDeclarations;
        for (size_t i = 0; i < oDeclarations->length; ++i)
            arDeclarations.insert(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));
        return arDeclarations;
    }

    inline std::pair<std::wstring, std::wstring> CCssCalculator_Private::GetDeclaration(KatanaDeclaration* oDecl) const
    {

        std::wstring sValueList = StringifyValueList(oDecl->values);

        if (oDecl->important)
            sValueList += L" !important";

        const std::pair<std::wstring, std::wstring>& pDeclaration = std::make_pair(stringToWstring(oDecl->property), sValueList);

        return pDeclaration;
    }

    inline void CCssCalculator_Private::GetImportRule(KatanaImportRule *oRule)
    {
        std::wstring sSelector = L"@" + stringToWstring(oRule->base.name) + L" ";
        sSelector += L"url(" + stringToWstring(oRule->href) + L")";

        CElement *oElement = new CElement;

        oElement->AddSelectors({sSelector});
        oElement->AddDeclarations({});

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetFontFaceRule(KatanaFontFaceRule *oRule)
    {
        const std::wstring sSelector = L"@" + stringToWstring(oRule->base.name);

        CElement *oElement = new CElement;

        oElement->AddSelectors({sSelector});
        oElement->AddDeclarations(GetDeclarationList(oRule->declarations));

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetKeyframesRule(KatanaKeyframesRule *oRule)
    {
        CElement *oElement = new CElement;

        const std::wstring sSelector = L"@" + stringToWstring(oRule->base.name);
        oElement->AddSelector(sSelector);

        for (size_t i = 0; i < oRule->keyframes->length; ++i)
            oElement->AddChildren(GetKeyframe((KatanaKeyframe*)oRule->keyframes->data[i]));

        m_arData.push_back(oElement);
    }

    inline CElement* CCssCalculator_Private::GetKeyframe(KatanaKeyframe *oKeyframe)
    {
        if (oKeyframe == NULL)
            return NULL;

        CElement *oElement = new CElement;

        for (size_t i = 0; i < oKeyframe->selectors->length; ++i)
        {
            const KatanaValue* oValue = (KatanaValue*)oKeyframe->selectors->data[i];
            if (oValue->unit == KATANA_VALUE_NUMBER)
            {
//                sSelector += stringToWstring(oValue->raw);
                oElement->AddSelector(stringToWstring(oValue->raw));
            }
        }

        oElement->AddDeclarations(GetDeclarationList(oKeyframe->declarations));

        return oElement;
    }

    inline void CCssCalculator_Private::GetMediaRule(KatanaMediaRule *oRule)
    {
        std::wstring sSelector = L"@" + stringToWstring(oRule->base.name) + L" ";

        CElement *oElement = new CElement;

        if (oRule->medias->length)
            sSelector += GetMediaList(oRule->medias);

        oElement->AddSelector(sSelector);

        if (oRule->medias->length)
        {
            for (size_t i = 0; i < oRule->rules->length; ++i)
            {
                oElement->AddChildren(GetStyleRule((KatanaStyleRule*)oRule->rules->data[i], oElement));
            }
        }

        m_arData.push_back(oElement);
    }

    inline std::wstring CCssCalculator_Private::GetMediaList(KatanaArray *oMedias)
    {
        std::wstring sText;

        for (size_t i = 0; i < oMedias->length; ++i)
        {
            sText += GetMediaQuery((KatanaMediaQuery*)oMedias->data[i]);
            sText += L", ";
        }

        sText.erase(sText.length() - 2, 2);

        return sText;
    }

    inline std::wstring CCssCalculator_Private::GetMediaQuery(KatanaMediaQuery *oQuery)
    {
        std::wstring sText;
        switch (oQuery->restrictor) {
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
                sText += stringToWstring(oQuery->type);
            return sText;
        }

        if ((NULL != oQuery->type && strcmp(oQuery->type, "all")) ||
             oQuery->restrictor != KatanaMediaQueryRestrictorNone)
        {
            if (NULL != oQuery->type)
                sText += stringToWstring(oQuery->type);

            sText += L" and ";
        }

        sText += GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[0]);

        for (size_t i = 1; i < oQuery->expressions->length; ++i)
        {
            sText += L" and ";
            sText += GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[i]);
        }
        return sText;
    }

    inline std::wstring CCssCalculator_Private::GetMediaQueryExp(KatanaMediaQueryExp *oExp)
    {
        std::wstring sText;

        sText += L"(";
        if (NULL != oExp->feature)
        {
            sText += stringToWstring(oExp->feature);
        }
        if (oExp->values && oExp->values->length)
        {
            sText += L": " + StringifyValueList(oExp->values);
        }
        sText += L")";

        return sText;
    }

    inline std::map<std::wstring, std::map<std::wstring, std::wstring>> CCssCalculator_Private::GetDeclarations(const std::wstring& sSelector) const
    {
        std::map<std::wstring, std::map<std::wstring, std::wstring>> arDeclarations;

        for (const CElement* oElement : m_arData )
        {
            if (oElement->FindSelector(sSelector))
            {
                const std::map<std::wstring, std::map<std::wstring, std::wstring>> _decl = oElement->GetDeclarations(sSelector, {});
                arDeclarations.insert(_decl.begin(), _decl.end());
            }
        }
        return arDeclarations;
    }

    inline std::vector<int> CCssCalculator_Private::GetWeightSelector(const std::string& sSelector) const
    {
        std::vector<int> arWeight = {0, 0, 0, 0};

        if (sSelector.empty())
            return arWeight;

        const std::vector<std::string> arPseudoClasses = {
                                                        "invalid",
                                                        "read-only",
                                                        "-moz-placeholder",
                                                        "-webkit-input-placeholder",
                                                        "active",
                                                        "checked",
                                                        "default",
                                                        "disabled",
                                                        "empty",
                                                        "enabled",
                                                        "first-child",
                                                        "first-of-type",
                                                        "focus",
                                                        "hover",
                                                        "indeterminate",
                                                        "lang",
                                                        "last-child",
                                                        "last-of-type",
                                                        "link",
                                                        "not",
                                                        "nth-child",
                                                        "nth-last-child",
                                                        "nth-last-of-type",
                                                        "nth-of-type",
                                                        "only-child",
                                                        "only-of-type",
                                                        "optional",
                                                        "read-write",
                                                        "required",
                                                        "root",
                                                        "target",
                                                        "valid",
                                                        "visited"};


        std::vector<std::string> arSel;
        std::string sTempStr;

        bool fl1 = false;

        for (size_t i = sSelector.size() - 1; i > 0; --i)
        {
            const char& sc = sSelector[i];

            if (sc == '*')
            {
                arWeight[3]++;
            }
            else if (sc == ']')
            {
                fl1 = true;
            }
            else if (sc == '[')
            {
                fl1 = false;
                const std::string& sSel = '[' + sTempStr + ']';
                arSel.push_back(sSel);
                sTempStr.clear();
            }
            else if ((sc == '.' || sc == '#' ||
                      sc == ' ' || sc == ':') && !fl1)
            {
                const char& wcBefore = sSelector[i -1];

                if (i > 0 && wcBefore == ':')
                {
                    sTempStr = wcBefore + sTempStr;
                    arSel.push_back(sc + sTempStr);
                    --i;
                }
                else if (sTempStr.length() > 1)
                {
                    arSel.push_back(sc + sTempStr);
                }
                sTempStr.clear();
            }
            else if (sc != '+' && sc != '>')
                sTempStr = sc + sTempStr;
        }

        if (sTempStr.length() != 0)
            arSel.push_back(sTempStr);

        for (const std::string& sSel : arSel)
        {
            if (sSel.find('#') != std::string::npos)
                ++arWeight[0];
            else if (sSel.find(':') != std::string::npos)
            {
                std::string sTemp;

                for (const char& sc : sSel)
                    if (iswalpha(sc))
                        sTemp += sc;

                std::find(arPseudoClasses.begin(), arPseudoClasses.end(), sTemp) != arPseudoClasses.end() ? ++arWeight[1] : ++arWeight[2];
            }
            else if (sSel.find('.') != std::string::npos ||
                     sSel.find('[') != std::string::npos ||
                     sSel.find(']') != std::string::npos)
            {
                ++arWeight[1];
            }
            else
                ++arWeight[2];
        }

        return arWeight;
    }

    inline std::vector<int> CCssCalculator_Private::GetWeightSelector(const std::wstring& sSelector) const
    {
        return GetWeightSelector(wstringToString(sSelector));
    }


    void CCssCalculator_Private::Print() const
    {
        std::wcout << m_arData.size() << std::endl;

        for (const CElement* oElement : m_arData)
            std::wcout << oElement->GetText() << std::endl;
    }


    inline std::wstring CCssCalculator_Private::GetValueList(KatanaArray *oValues)
    {
        return StringifyValueList(oValues);
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<std::string>& arSelectors, const UnitMeasure& unitMeasure)
    {
        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);

        std::map<std::wstring, std::wstring> mStyle;

        std::map<std::wstring, std::map<std::wstring, std::wstring>> arStyle;
    //                        selector      declarations

        std::map<std::wstring, std::wstring> arPropSel; //мапа (свойство, что уже было использовано, селектор этого свойства)

        std::map<std::wstring, std::map<std::wstring, std::wstring>> arTempDecls;

        arTempDecls = GetDeclarations(L"*");
        arStyle = arTempDecls;
        arTempDecls.clear();

        for (const std::string& sSel : arSelectors)
        {
            const std::wstring& sSelector = stringToWstring(sSel);

            arTempDecls = GetDeclarations(sSelector);

            arStyle.insert(arTempDecls.begin(), arTempDecls.end());
        }

        for (const auto& pValue : arStyle)
        {
            for (const auto& pDeclaration : pValue.second)
            {
                arPropSel[pDeclaration.first] = pValue.first;

                if (mStyle.find(pDeclaration.first) == mStyle.cend())
                {
                    std::wstring sValue = pDeclaration.second;

                    if (sValue[sValue.length() - 1] == L';' || sValue[sValue.length() - 1] == L':')
                        sValue.erase(sValue.length() - 1, 1);

                    const auto& posLattice = sValue.find(L"#");

                    if (posLattice != std::wstring::npos)
                    {
                        const auto& posSpace = sValue.find(L' ', posLattice);

                        if (posSpace != std::wstring::npos)
                            sValue = sValue.substr(posLattice, (posSpace - 1));
                    }

                    mStyle[pDeclaration.first] = sValue;
                }
                else
                {
                    const std::vector<int>& arWeightFirst = GetWeightSelector(arPropSel[pDeclaration.first]);
                    const std::vector<int>& arWeightSecond = GetWeightSelector(pValue.first);

                    if ((arWeightFirst < arWeightSecond &&
                         mStyle[pDeclaration.first].find(L"!important") == std::wstring::npos) ||
                        pDeclaration.second.find(L"!important") != std::wstring::npos)
                    {
                        std::wstring sValue = pDeclaration.second;

                        if (sValue[sValue.length() - 1] == L';' || sValue[sValue.length() - 1] == L':')
                            sValue.erase(sValue.length() - 1, 1);

                        const auto& posLattice = sValue.find(L"#");

                        if (posLattice != std::wstring::npos)
                        {
                            const auto& posSpace = sValue.find(L' ', posLattice);

                            if (sValue.find(posSpace, posLattice) != std::wstring::npos)
                                sValue = sValue.substr(posLattice, (posSpace - 1));
                        }

                        mStyle[pDeclaration.first] = sValue;
                    }
                }
            }
        }

        for (const auto& oIter : mStyle)
        {
            auto posExclamatory = oIter.second.find(L"!");

            posExclamatory != std::wstring::npos ? mStyle[oIter.first] = ConvertUnitMeasure(oIter.second.substr(0, posExclamatory)) : mStyle[oIter.first] = ConvertUnitMeasure(oIter.second);
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
            oElement->AddSelector(stringToWstring(sSelectorUTF8));

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
            const std::wstring& sValue = ConvertUnitMeasure(stringToWstring(arValue[i]));
            arDecl.push_back(std::make_pair(stringToWstring(arProperty[i]), sValue));
        }

        oElement->AddDeclarations(arDecl);
        m_arData.push_back(oElement);
    }
    */

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const std::vector<CNode> &arSelectors, const UnitMeasure& unitMeasure)
    {
        const size_t& parentSize = arSelectors.size() - 1;

        if (parentSize > 0)
        {
            const auto& oItem = m_mUsedStyles.find(arSelectors);
            if (oItem != m_mUsedStyles.cend())
            {
                return *oItem->second;
            }
        }

        CCompiledStyle *oStyle = new CCompiledStyle;

        std::wstring sClassName = arSelectors.back().m_sClass;
//        TranslateToEn(sClassName);

        if (!sClassName.empty() && sClassName[0] != L'.')
            sClassName = L'.' + sClassName;

        std::wstring sIdName = arSelectors.back().m_sId;
//        TranslateToEn(sIdName);

        if (!sIdName.empty() && sIdName[0] != L'#')
            sIdName = L'#' + sIdName;

        for (std::vector<CNode>::const_iterator oParent = arSelectors.begin(); oParent != arSelectors.end() - 1; ++oParent)
        {
            if (oParent->m_sName != L"body")
            {
                *oStyle += GetCompiledStyle({*oParent}, unitMeasure);
                oStyle->AddParent(oParent->m_sName);
            }
        }

        *oStyle += GetCompiledStyle(GetSelectorsList(arSelectors.back().m_sName + sClassName + sIdName), unitMeasure);

        if (!arSelectors.back().m_sStyle.empty())
        {
            CCompiledStyle oTempStyle;
            oTempStyle.AddStyle(ConvertUnitMeasure(arSelectors.back().m_sStyle));
            *oStyle += oTempStyle;
        }


        oStyle->SetID(arSelectors.back().m_sName + sClassName + sIdName + L'-' + std::to_wstring(m_nCountNodes));
        m_nCountNodes++;

        if (parentSize > 0)
        {
            m_mUsedStyles.emplace(arSelectors, oStyle);
        }

        return *oStyle;
    }

    void CCssCalculator_Private::AddStyles(const std::string &sStyle)
    {
        if (sStyle.empty())
            return;

        // тут ВСЕГДА utf-8
        KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.length(), KatanaParserModeStylesheet);
        this->GetOutputData(output);
        katana_destroy_output(output);
    }

    void CCssCalculator_Private::AddStyles(const std::wstring &sStyle)
    {
        if (sStyle.empty())
            return;

        AddStyles(wstringToString(sStyle));
    }

    void CCssCalculator_Private::AddStylesFromFile(const std::wstring& sFileName)
    {
        if (std::find(m_arFiles.begin(), m_arFiles.end(), sFileName) != m_arFiles.cend())
            return;

        m_arFiles.push_back(sFileName);

        AddStyles(GetContentAsUTF8(sFileName));
    }

    std::wstring CCssCalculator_Private::ConvertUnitMeasure(const std::wstring &sValue) const
    {
        if (sValue.empty())
            return sValue;

        const std::vector<std::wstring>& arValues = GetWordsW(sValue);

        std::wstring sValueString;

        for (const std::wstring& sValueTemp : arValues)
        {
            const auto& nPosGrid = sValueTemp.find(L'#');

            if (nPosGrid != std::wstring::npos || !ThereIsNumber(sValueTemp))
            {
                sValueString += sValueTemp;

                if (arValues.size() > 1 && sValueTemp.find_first_of(L":;") == std::wstring::npos)
                    sValueString += L' ';

                continue;
            }

            const auto& posPercent = sValueTemp.find(L'%');

            if (posPercent != std::wstring::npos)
            {
                const double& dValue = wcstod(sValueTemp.substr(0, posPercent).c_str(), NULL) / 100 * 24;

                sValueString += std::to_wstring((int)floor(dValue + 0.5));

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"px") != std::wstring::npos)
            {
                sValueString += ConvertPx(sValueTemp);
                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"cm") != std::wstring::npos)
            {
                sValueString += ConvertCm(sValueTemp);
                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"mm") != std::wstring::npos)
            {
                sValueString += ConvertMm(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"in") != std::wstring::npos)
            {
                sValueString += ConvertIn(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"pt") != std::wstring::npos)
            {
                sValueString += ConvertPt(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"pc") != std::wstring::npos)
            {
                sValueString += ConvertPc(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else if (sValueTemp.find(L"em") != std::wstring::npos)
            {
                sValueString += ConvertEm(sValueTemp);

                if (sValueTemp.find(L';') != std::wstring::npos)
                    sValueString += L';';
                else if (arValues.size() > 2 && sValueTemp.find(L':') == std::wstring::npos)
                    sValueString += L' ';
            }
            else
            {
                const double& dValue = wcstod(sValueTemp.c_str(), NULL) * 24;

                sValueString += std::to_wstring((int)floor(dValue + 0.5));

                if (sValueTemp.find(L";") != std::wstring::npos)
                    sValueString += L';';

                break;
            }

        }

        return sValueString;
    }


    inline std::wstring CCssCalculator_Private::ConvertPx(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"px") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Cantimeter:
            {
                return ConvertPxToCm(dValue);
            }
            case Inch:
            {
                return ConvertPxToIn(dValue);
            }
            case Millimeter:
            {
                return ConvertPxToMm(dValue);
            }
            case Default:
            case Point:
            {
                return ConvertPxToPt(dValue);
            }
            case Peak:
            {
                return ConvertPxToPc(dValue);
            }
            case Pixel:
            default:
                break;
        }
        return  sConvertValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToCm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / (double)m_nDpi * 2.54;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"cm ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToIn(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 1 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"in ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToMm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / (double)m_nDpi * 25.4;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"mm ";

        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPc(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 1.0/6.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pc ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPt(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 72.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pt ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCm(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"cm") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Pixel:
            {
                return ConvertCmToPx(dValue);
            }
            case Inch:
            {
                return ConvertCmToIn(dValue);
            }
            case Millimeter:
            {
                return ConvertCmToMm(dValue);
            }
            case Default:
            case Point:
            {
                return ConvertCmToPt(dValue);
            }
            case Peak:
            {
                return ConvertCmToPc(dValue);
            }
            case Cantimeter:
            default:
                break;
        }
        return  sConvertValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToIn(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 2.54;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToMm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 10;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"mm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPc(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (6 / 2.54) * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPt(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (72.0 / 2.54) * dValue;

        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pt";

        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPx(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 2.54 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"px";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMm(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"mm") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Cantimeter:
                return ConvertMmToCm(dValue);
            case Pixel:
            {
                return ConvertMmToPx(dValue);
            }
            case Inch:
            {
                return ConvertMmToIn(dValue);
            }
            case Default:
            case Point:
            {
                return ConvertMmToPt(dValue);
            }
            case Peak:
            {
                return ConvertMmToPc(dValue);
            }
            case Millimeter:
            default:
                break;
        }
        return  sConvertValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToIn(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 25.4;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToCm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 10;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"cm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPc(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 72.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPt(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 6.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pt";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPx(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertIn(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"in") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Pixel:
            {
                return  ConvertInToPx(dValue);
            }
            case Cantimeter:
            {
                return ConvertInToCm(dValue);
            }
            case Inch:
                break;
            case Millimeter:
            {
                return ConvertInToMm(dValue);
            }
            case Default:
            case Point:
            {
                return ConvertInToPt(dValue);
            }
            case Peak:
            {
                return ConvertInToPc(dValue);
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertInToMm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 25.4;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToCm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 2.54;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPc(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPt(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPx(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * (double)m_nDpi;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPt(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pt") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Cantimeter:
                return ConvertPtToCm(dValue);
            case Pixel:
            {
                return ConvertPtToPx(dValue);
            }
            case Inch:
            {
                return ConvertPtToIn(dValue);
            }
            case Millimeter:
            {
                return ConvertPtToMm(dValue);
            }
            case Peak:
            {
                return ConvertPtToPc(dValue);
            }
            case Default:
            case Point:
            default:
                break;
        }
        return  std::to_wstring((int)dValue);
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToIn(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToCm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0 * 2.54;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPc(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 12.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToMm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0 * 25.4;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPx(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 72.0 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPc(const std::wstring& sValue) const
    {
        if (sValue.empty())
            return L"";

        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"pc") - 1);
        const double& dValue = wcstod(sConvertValue.c_str(), NULL) * 2;

        switch (m_UnitMeasure)
        {
            case Cantimeter:
                return ConvertPcToCm(dValue);
            case Pixel:
            {
                return ConvertPcToPx(dValue);
            }
            case Inch:
            {
                return ConvertPcToIn(dValue);
            }
            case Millimeter:
            {
                return ConvertPcToMm(dValue);
            }
            case Default:
            case Point:
            {
                return ConvertPcToPt(dValue);
            }
            case Peak:
            default:
                break;
        }
        return  sConvertValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToIn(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToCm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0 * 2.54;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPt(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 12.0;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToMm(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0 * 25.4;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPx(const double& dValue, bool bAddUM) const
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 6.0 * dValue;
        std::wstring sValue = std::to_wstring((int)floor(_dValue + 0.5));
        if (bAddUM)
            sValue += L"px";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertEm(const std::wstring &sValue) const
    {
        if (sValue.empty())
            return L"";
        const std::wstring& sConvertValue = sValue.substr(0, sValue.find_last_of(L"em") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);

        dValue *= 24;

        return std::to_wstring((int)floor(dValue + 0.5));
    }

    void CCssCalculator_Private::SetDpi(const int& nValue)
    {
        if (nValue > 0)
            m_nDpi = nValue;
    }

    void CCssCalculator_Private::SetUnitMeasure(const UnitMeasure& nType)
    {
        m_UnitMeasure = nType;
    }

    int CCssCalculator_Private::GetDpi() const
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
inline static std::wstring StringifyValueList(KatanaArray* oValues)
{
    if (NULL == oValues)
        return L"";

    std::wstring buffer;

    for (size_t i = 0; i < oValues->length; ++i)
    {
        KatanaValue* value = (KatanaValue*)oValues->data[i];
        buffer += StringifyValue(value);

        if ( i < oValues->length - 1 )
        {
            if ( value->unit != KATANA_VALUE_PARSER_OPERATOR )
            {
                if ( i < oValues->length - 2 )
                {
                    value = (KatanaValue*)oValues->data[i+1];
                    if ( value->unit != KATANA_VALUE_PARSER_OPERATOR )
                    {
                        buffer += L" ";
                    }
                }
                else
                {
                    buffer += L" ";
                }
            }
        }
    }

    return buffer;
}

inline static std::wstring StringifyValue(KatanaValue* oValue)
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
            str = stringToWstring(oValue->raw);
            break;
        case KATANA_VALUE_IDENT:
            str = stringToWstring(oValue->string);
            break;
        case KATANA_VALUE_STRING:
        {
            str = L"\"";
            str += stringToWstring(oValue->string);
            str += L"\"";
            break;
        }
        case KATANA_VALUE_PARSER_FUNCTION:
        {
            const std::wstring& args_str = StringifyValueList(oValue->function->args);
            if (args_str.empty())
                break;

            str = stringToWstring(oValue->function->name) + args_str + L")";
            break;
        }
        case KATANA_VALUE_PARSER_OPERATOR:
            str = L" ";
            if (oValue->iValue != '=')
            {
                str.push_back((wchar_t)static_cast<char>(oValue->iValue));
                str += L" ";
            }
            else
                str.push_back((wchar_t)static_cast<char>(oValue->iValue));
            break;
        case KATANA_VALUE_PARSER_LIST:
            return StringifyValueList(oValue->list);
            break;
        case KATANA_VALUE_PARSER_HEXCOLOR:
            str = (L"#" + stringToWstring(oValue->string));
            break;
        case KATANA_VALUE_URI:
            str = (L"url(" + stringToWstring(oValue->string) + L")");
            break;
        default:
            break;
    }

    return str;
}

inline static std::wstring stringToWstring(const std::string& sString)
{
    return UTF8_TO_U(sString);
}

inline static std::string wstringToString(const std::wstring& sWstring)
{
    return U_TO_UTF8(sWstring);
}

inline static std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding)
{
    const std::string& sEnc = U_TO_UTF8(sEncoding);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    const std::wstring& sUnicodeContent = oConverter.toUnicode(sString, sEnc.c_str());
    return U_TO_UTF8(sUnicodeContent);
}

inline static std::string GetContentAsUTF8(const std::wstring& sFileName)
{
    std::string sFileContent;

    // читаем файл как есть. utf-8 тут просто название.
    if(!NSFile::CFileBinary::ReadAllTextUtf8A(sFileName, sFileContent))
        return sFileContent;

    std::string sEncoding;
    if (true)
    {
        // определяем кодировку
        std::string::size_type posCharset = sFileContent.find("@charset");
        if (std::string::npos != posCharset)
        {
            std::string::size_type pos1 = sFileContent.find_first_of("\"';", posCharset);
            if (std::string::npos != pos1)
            {
                pos1 += 1;
                std::string::size_type pos2 = sFileContent.find_first_of("\"';", pos1);
                if (std::string::npos != pos2)
                {
                    sEncoding = sFileContent.substr(pos1, pos2 - pos1);
                }

                // check valid
                if (std::string::npos != sEncoding.find_first_of(" \n\r\t\f\v"))
                    sEncoding = "";
            }
        }
    }

    if (sEncoding.empty())
        sEncoding = "utf-8";


    if (!sEncoding.empty() && sEncoding != "utf-8" && sEncoding != "UTF-8")
    {
        NSUnicodeConverter::CUnicodeConverter oConverter;
        sFileContent = U_TO_UTF8(oConverter.toUnicode(sFileContent, sEncoding.c_str()));
    }

    return sFileContent;
}

inline static bool ThereIsNumber(const std::wstring& sString)
{
    if (sString.empty())
        return false;

    const auto& posDigit = sString.find_first_of(L"0123456789");
    const auto& posNoDigit = sString.find_first_not_of(L" \n\r\t\f\v123456789.");

    if (posDigit == std::wstring::npos)
        return false;

    if (posDigit != std::wstring::npos && posNoDigit == std::wstring::npos)
        return true;

    if ((posDigit != std::wstring::npos && posNoDigit != std::wstring::npos && posDigit < posNoDigit) || (posDigit == 0  && posNoDigit == 0))
        return true;

    return false;
}

inline static void RemoveExcessFromStyles(std::wstring& sStyle)
{
    while (true)
    {
        const auto& posLeftAngleBracket = sStyle.find(L'<');
        const auto& posRightAngleBracket = sStyle.find(L'>');

        const auto& posLeftComment = sStyle.find(L"<!--");
        const auto& posRightComment = sStyle.find(L"-->");

        const auto& posDog = sStyle.find(L'@');

        if (posLeftAngleBracket != std::wstring::npos || posDog != std::wstring::npos ||
            posLeftComment != std::wstring::npos || posRightComment != std::wstring::npos)
        {

            if (posLeftAngleBracket != std::wstring::npos &&
                posRightAngleBracket != std::wstring::npos)
            {
                sStyle.erase(posLeftAngleBracket, posRightAngleBracket - posLeftAngleBracket + 1);
            }
            else if (posLeftAngleBracket != std::wstring::npos)
                sStyle.erase(posLeftAngleBracket, 1);
            else if (posRightAngleBracket != std::wstring::npos)
                sStyle.erase(posRightAngleBracket, 1);
            else if (posDog != std::wstring::npos)
                sStyle.erase(posDog, 1);
            else if (posLeftComment != std::wstring::npos)
                sStyle.erase(posLeftComment, 4);
            else if (posRightComment != std::wstring::npos)
                sStyle.erase(posRightComment, 3);
        }
        else
            break;
    }
}

inline static void TranslateToEn(std::wstring& sStyle)
{
    bool bCorrect = false;

    for (std::wstring::size_type i = 0, len = sStyle.length(); i < len; ++i)
    {
        const wchar_t& wc = sStyle[i];
        if (wc == '\t' || wc == '\n')
        {
            // may be change to space???
            sStyle.erase(i, 1);
            --i;
            --len;
        }
        else if (wc > 255)
        {
            bCorrect = true;
            break;
        }
    }

    if (!bCorrect)
        return;

    std::wstring sNewStyle;
    for (std::wstring::size_type i = 0, len = sStyle.length(); i < len; ++i)
    {
        const wchar_t& wc = sStyle[i];
        if (wc == '\t' || wc == '\n')
        {
            continue;
        }
        else if (wc <= 255)
        {
            sNewStyle += wc;
        }
        else
        {
            sNewStyle += std::to_wstring((int)wc);
        }
    }
    sStyle = sNewStyle;
}

inline static std::wstring ConvertAbsoluteValue(const std::wstring& sAbsoluteValue)
{
    if (sAbsoluteValue.empty())
        return sAbsoluteValue;

    const std::map<std::wstring, std::wstring> arAbsoluteValues = {{L"xx-small", L"9px"},  {L"x-small", L"10px"},
                                                                   {L"small",    L"13px"}, {L"medium",  L"16px"},
                                                                   {L"large",    L"18px"}, {L"x-large", L"24px"},
                                                                   {L"xx-large", L"32px"}};

    std::wstring sNewValue = sAbsoluteValue;

    for (const auto& sAbsValue : arAbsoluteValues)
    {
        while (sNewValue.find(sAbsValue.first) != std::wstring::npos)
        {
            const auto& nPos1 = sNewValue.find(sAbsValue.first);
            const auto& nPos2 = nPos1 + sAbsValue.first.length() - 1;

            sNewValue.replace(nPos1, nPos2, sAbsValue.second);
        }
    }

    return sNewValue;
}

inline std::wstring DeleteSpace(const std::wstring& sValue)
{
    const size_t& start = sValue.find_first_not_of(L" \n\r\t\f\v");

    if (std::wstring::npos == start)
        return sValue;

    const size_t& end = sValue.find_last_not_of(L" \n\r\t\f\v"); // точно >=0

    return sValue.substr(start, end - start + 1);
}

inline std::vector<std::string> GetSelectorsList(const std::wstring& sSelectors)
{
    std::wstring sNames = sSelectors;
    std::wstring sClasses;
    std::wstring sIds;

    size_t posLattice = sNames.find(L"#");
    size_t posPoint = sNames.find(L'.');

    if (posLattice != std::wstring::npos)
    {
        sNames = sSelectors.substr(0, posLattice);
        sIds = sSelectors.substr(posLattice);
    }
    if (posPoint != std::wstring::npos)
    {
        sNames = sSelectors.substr(0, posPoint);
        sClasses = sSelectors.substr(posPoint);
        posLattice = sClasses.find(L'#');
        if (posLattice != std::wstring::npos)
            sClasses = sClasses.substr(0, posLattice);
    }

    std::vector<std::string> arNames   = GetWords(sNames);
    std::vector<std::string> arClasses = GetWords(sClasses);
    std::vector<std::string> arIds     = GetWords(sIds);
    std::vector<std::string> arSelectors(arNames);

    for (std::string sClass : arClasses)
    {
        if (sClass.find('.') == std::string::npos)
            sClass = '.' + sClass;
        arSelectors.push_back(sClass);
        for (const std::string& sName : arNames)
            arSelectors.push_back(sName + sClass);
        for (const std::string& sId : arIds)
            arSelectors.push_back(sClass + sId);
    }
    for (std::string sId : arIds)
    {
        if (sId.find('#') == std::string::npos)
            sId = '#' + sId;
        arSelectors.push_back(sId);
        for (const std::string& sName : arNames)
            arSelectors.push_back(sName + sId);
    }
    return arSelectors;
}

inline std::vector<std::string> GetWords(const std::wstring& sLine)
{
    if (sLine.empty())
        return {};

    std::vector<std::string> arWords;
    std::string sTempWord = wstringToString(sLine);

    while (true)
    {
        const auto& posFirstNotSpace = sTempWord.find_first_not_of(" \n\r\t\f\v:;");
        const auto& posLastNotSpace = sTempWord.find_first_of(" \n\r\t\f\v", posFirstNotSpace);
        const auto& posFirstSign = sTempWord.find_first_of(":;,", posFirstNotSpace);

        if (posFirstNotSpace != std::wstring::npos && posFirstSign != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1));
            sTempWord.erase(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
            sTempWord.erase(posFirstNotSpace, posLastNotSpace - posFirstNotSpace);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace == std::wstring::npos)
        {
            arWords.push_back(sTempWord);
            break;
        }
        else
            break;
    }

    return arWords;
}


inline std::vector<std::wstring> GetWordsW(const std::wstring& sLine)
{
    if (sLine.empty())
        return {};

    std::vector<std::wstring> arWords;
    std::wstring sTempWord = sLine;

    while (true)
    {
        const auto& posFirstNotSpace = sTempWord.find_first_not_of(L" \n\r\t\f\v:;");
        const auto& posLastNotSpace = sTempWord.find_first_of(L" \n\r\t\f\v", posFirstNotSpace);
        const auto& posFirstSign = sTempWord.find_first_of(L":;,", posFirstNotSpace);

        if (posFirstNotSpace != std::wstring::npos && posFirstSign != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1));
            sTempWord.erase(posFirstNotSpace, posFirstSign - posFirstNotSpace + 1);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace != std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace, posLastNotSpace - posFirstNotSpace));
            sTempWord.erase(posFirstNotSpace, posLastNotSpace - posFirstNotSpace);
        }
        else if (posFirstNotSpace != std::wstring::npos && posLastNotSpace == std::wstring::npos)
        {
            arWords.push_back(sTempWord.substr(posFirstNotSpace));
            break;
        }
        else
            break;
    }

    return arWords;
}
