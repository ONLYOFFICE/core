#include "CCssCalculator_Private.h"
#include <codecvt>

#include <string>
//#include <algorithm>
#include <vector>
#include <fstream>

#include "iostream"

#include "../katana-parser/src/selector.h"
#include "../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../DesktopEditor/common/File.h"

#define MAX_LINE_LENGTH 80

static std::wstring StringifyValueList(KatanaArray* oValues);
static std::wstring StringifyValue(KatanaValue* oValue);

inline std::wstring stringToWstring(const std::string& sString)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(sString);
}

inline std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    std::string sEnc = converter.to_bytes(sEncoding);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    std::wstring sUnicodeContent = oConverter.toUnicode(sString, sEnc.c_str());
    return U_TO_UTF8(sUnicodeContent);
}

inline std::string GetContentAsUTF8(const std::wstring& sFileName)
{
    std::string sSource;
    if (!NSFile::CFileBinary::ReadAllTextUtf8A(sFileName, sSource))
        return sSource;

    std::string sFirstLine;
    if (sSource.find('\n') != std::string::npos)
        sFirstLine = sSource.substr(0, sSource.find('\n'));
    else
        sFirstLine = sSource;

    std::string sEncoding;
    if (sFirstLine.find("@charset") != std::string::npos)
    {
         sEncoding = sFirstLine.substr(sFirstLine.find("@charset ") + 9);
        if (sEncoding.find(';') != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find(';'));

        if (sEncoding.find('"') != std::string::npos)
            sEncoding = sEncoding.substr(sEncoding.find('"') + 1);
        else if (sEncoding.find("'") != std::string::npos)
            sEncoding = sEncoding.substr(sEncoding.find("'") + 1);

        if (sEncoding.find('"') != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find('"'));
        else if (sEncoding.find("'") != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find("'"));
    }
    else
        return sSource;

    if (sEncoding == "UTF-8" || sEncoding == "utf-8")
        return sSource;

    NSUnicodeConverter::CUnicodeConverter oConverter;
    std::wstring sUnicodeContent = oConverter.toUnicode(sSource, sEncoding.c_str());
    return U_TO_UTF8(sUnicodeContent);
}

namespace NSCSS
{
    CCssCalculator_Private::CCssCalculator_Private()
    {
    }

    CCssCalculator_Private::~CCssCalculator_Private()
    {
        for (size_t i = 0; i < m_arData.size(); i++)
            delete m_arData[i];

        m_arData.clear();
    }

    inline CElement* CCssCalculator_Private::GetElement(const int& nIndex)
    {
        if (nIndex < 0 || nIndex >= (int)m_arData.size())
            return NULL;

        return m_arData[nIndex];
    }

    inline void CCssCalculator_Private::AddElement(CElement *oElement)
    {
        if (oElement != NULL)
            m_arData.push_back(oElement);
    }

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
        if (oElement->GetCountSelectors() > 0 ||
            oElement->GetCountDeclarations() > 0)
            m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetStylesheet(KatanaStylesheet *oStylesheet, CElement *elementRule)
    {
        for (size_t i = 0; i < oStylesheet->imports.length; ++i) {
            GetRule((KatanaRule*)oStylesheet->imports.data[i], elementRule);
        }
        for (size_t i = 0; i < oStylesheet->rules.length; ++i) {
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
        std::vector<std::wstring> arSelectors;
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

        arSelectors = GetSelectorList(oRule->selectors);

        if (oRule->declarations->length)
            arDeclarations = GetDeclarationList(oRule->declarations);

        CElement *oElement = new CElement;

        oElement->AddSelectors(arSelectors);
        oElement->AddDeclarations(arDeclarations);

        if (oElementRule == NULL)
        {
            m_arData.push_back(oElement);
            return NULL;
        }
        else
            return oElement;
    }

    inline std::vector<std::wstring> CCssCalculator_Private::GetSelectorList(KatanaArray* oSelectors)
    {
        std::vector<std::wstring> arSelectors;
        for (size_t i = 0; i < oSelectors->length; ++i)
            arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));
        return arSelectors;
    }

    inline std::wstring CCssCalculator_Private::GetSelector(KatanaSelector *oSelector)
    {
        KatanaParser oParser;
        oParser.options = &kKatanaDefaultOptions;

        std::wstring sText;
        KatanaParserString * string = katana_selector_to_string(&oParser, oSelector, NULL);
        const char* text = katana_string_to_characters(&oParser, string);

        katana_parser_deallocate(&oParser, (void*) string->data);
        katana_parser_deallocate(&oParser, (void*) string);
        sText = stringToWstring(text);
        katana_parser_deallocate(&oParser, (void*) text);

        return sText;
    }

    inline std::vector<std::pair<std::wstring, std::wstring>> CCssCalculator_Private::GetDeclarationList(KatanaArray* oDeclarations)
    {
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

        for (size_t i = 0; i < oDeclarations->length; ++i)
            arDeclarations.push_back(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));

        return arDeclarations;
    }

    inline std::pair<std::wstring, std::wstring> CCssCalculator_Private::GetDeclaration(KatanaDeclaration* oDecl)
    {
        std::pair<std::wstring, std::wstring> pDeclaration;

        std::wstring sValueList = StringifyValueList(oDecl->values);

        if (oDecl->important)
            sValueList += L" !important";
        pDeclaration = std::make_pair(stringToWstring(oDecl->property), sValueList);

        return pDeclaration;
    }

    inline void CCssCalculator_Private::GetImportRule(KatanaImportRule *oRule)
    {
        std::vector<std::wstring> arSelectors;
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

        std::wstring sSelector = L"@" + stringToWstring(oRule->base.name) + L" ";
        sSelector += L"url(" + stringToWstring(oRule->href) + L")";
        arSelectors.push_back(sSelector);

        CElement *oElement = new CElement;

        oElement->AddSelectors(arSelectors);
        oElement->AddDeclarations(arDeclarations);

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetFontFaceRule(KatanaFontFaceRule *oRule)
    {
        std::vector<std::wstring> arSelectors;
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = GetDeclarationList(oRule->declarations);

        std::wstring sSelector = L"@" + stringToWstring(oRule->base.name);
        arSelectors.push_back(sSelector);

        CElement *oElement = new CElement;

        oElement->AddSelectors(arSelectors);
        oElement->AddDeclarations(arDeclarations);

        m_arData.push_back(oElement);
    }

    inline void CCssCalculator_Private::GetKeyframesRule(KatanaKeyframesRule *oRule)
    {
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

        CElement *oElement = new CElement;

        std::wstring sSelector = L"@" + stringToWstring(oRule->base.name);
        oElement->AddSelector(sSelector);

        for (size_t i = 0; i < oRule->keyframes->length; i++)
            oElement->AddChildren(GetKeyframe((KatanaKeyframe*)oRule->keyframes->data[i]));

        m_arData.push_back(oElement);
    }

    inline CElement* CCssCalculator_Private::GetKeyframe(KatanaKeyframe *oKeyframe)
    {
        if (oKeyframe == NULL)
            return NULL;

        CElement *oElement = new CElement;

        std::wstring sSelector;
        for (size_t i = 0; i < oKeyframe->selectors->length; i++)
        {
            KatanaValue* oValue = (KatanaValue*)oKeyframe->selectors->data[i];
            if (oValue->unit == KATANA_VALUE_NUMBER)
            {
                oElement->AddSelector(stringToWstring(oValue->raw));
            }
            if (i != oKeyframe->selectors->length - 1)
            {
                sSelector += L", ";
            }
        }

        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = GetDeclarationList(oKeyframe->declarations);

        oElement->AddDeclarations(arDeclarations);

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
        bool first = true;
        for (size_t i = 0; i < oMedias->length; ++i) {
            if (!first)
                sText += L", ";
            else
                first = false;
            sText += GetMediaQuery((KatanaMediaQuery*)oMedias->data[i]);
        }
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

        if ((NULL != oQuery->type && strcmpi(oQuery->type, "all")) ||
             oQuery->restrictor != KatanaMediaQueryRestrictorNone) {
            if (NULL != oQuery->type)
                sText += stringToWstring(oQuery->type);

            sText += L" and ";
        }

        sText += GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[0]);

        for (size_t i = 1; i < oQuery->expressions->length; ++i) {
            sText += L" and ";
            sText += GetMediaQueryExp((KatanaMediaQueryExp*)oQuery->expressions->data[i]);
        }
        return sText;
    }

    inline std::wstring CCssCalculator_Private::GetMediaQueryExp(KatanaMediaQueryExp *oExp)
    {
        std::wstring sText;

        sText += L"(";
        if (NULL != oExp->feature) {
            sText += stringToWstring(oExp->feature);
        }
        if (oExp->values && oExp->values->length) {
            sText += L": " + StringifyValueList(oExp->values);
        }
        sText += L")";

        return sText;
    }

    inline std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CCssCalculator_Private::GetDeclarations(const std::wstring& sSelector)
    {
        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arDeclarations;

        for (size_t i = 0; i < m_arData.size(); i++)
        {
            std::vector<std::wstring> arParent;
            std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> _decl = m_arData[i]->GetDeclarations(sSelector, arParent);
            arDeclarations.insert(arDeclarations.end(), _decl.begin(), _decl.end());
        }

        return arDeclarations;
    }

    inline std::vector<int> CCssCalculator_Private::GetWeightSelector(const std::string& sSelector)
    {
        std::vector<int> arWeight = {0, 0, 0, 0};
        if (sSelector.empty())
            return arWeight;

        std::vector<std::string> arPseudoClasses = {    "invalid",
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

        for (int i = sSelector.size() - 1; i >= 0; i--)
        {
            if (sSelector[i] == '*')
            {
                arWeight[3]++;
            }
            else if (sSelector[i] == ']')
            {
                fl1 = true;
            }
            else if (sSelector[i] == '[')
            {
                fl1 = false;
                arSel.push_back('[' + sTempStr + ']');
                sTempStr.clear();
            }
            else if ((sSelector[i] == '.' || sSelector[i] == '#' ||
                      sSelector[i] == ' ' || sSelector[i] == ':') && !fl1)
            {
                if (i > 0 && sSelector[i - 1] == ':')
                {
                    sTempStr = sSelector[i - 1] + sTempStr;
                    arSel.push_back(sSelector[i] + sTempStr);
                    i--;
                }
                else if (sTempStr.length() > 1)
                {
                    arSel.push_back(sSelector[i] + sTempStr);
                }
                sTempStr.clear();
            }
            else if (sSelector[i] != '+' && sSelector[i] != '>')
                sTempStr = sSelector[i] + sTempStr;
        }

        if (sTempStr.length() != 0)
            arSel.push_back(sTempStr);

        for (size_t i = 0; i < arSel.size(); i++)
        {
            if (arSel[i].find('#') != std::string::npos)
                arWeight[0]++;
            else if (arSel[i].find(':') != std::string::npos)
            {
                std::string sTemp;
                for (size_t n = 0; n < arSel[i].length(); n++)
                    if (isalpha(arSel[i][n]))
                        sTemp += arSel[i][n];

                if (std::find(arPseudoClasses.begin(), arPseudoClasses.end(), sTemp) != arPseudoClasses.end())
                {
                    arWeight[1]++;
                }
                else
                    arWeight[2]++;
            }
            else if (arSel[i].find('.') != std::string::npos ||
                     arSel[i].find('[') != std::string::npos ||
                     arSel[i].find(']') != std::string::npos)
            {
                arWeight[1]++;
            }
            else
                arWeight[2]++;
        }

        return arWeight;
    }

    inline std::vector<int> CCssCalculator_Private::GetWeightSelector(const std::wstring& sSelector)
    {
        std::string sSel = std::string(sSelector.begin(), sSelector.end());
        return GetWeightSelector(sSel);
    }

    void CCssCalculator_Private::Print()
    {
        std::wcout << m_arData.size() << std::endl;

        for (size_t i = 0; i < m_arData.size(); i++)
            std::wcout << m_arData[i]->GetText() << std::endl;
    }

    inline std::wstring CCssCalculator_Private::GetValueList(KatanaArray *oValues)
    {
        return StringifyValueList(oValues);
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
    {
        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);

        std::map<std::wstring, std::wstring> mStyle;

        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arStyle;
    //                        selector      declarations

        std::map<std::wstring, std::wstring> arPropSel; //мапа (свойство, что уже было использовано, селектор этого свойства)


        for (size_t i = 0; i < arSelectors.size(); i++)
        {

            std::wstring sSelector = stringToWstring(arSelectors[i]);
            std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arTempDecls = GetDeclarations(sSelector);
            arStyle.insert(arStyle.end(), arTempDecls.begin(), arTempDecls.end());
            arTempDecls.clear();
            arTempDecls = GetDeclarations(L"*");
            arStyle.insert(arStyle.end(), arTempDecls.begin(), arTempDecls.end());

        }
        for (size_t i = 0; i < arStyle.size(); i++)
        {
            std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = arStyle[i].second;
            for (size_t j = 0; j < arDeclarations.size(); j++)
            {
                arPropSel.emplace(arDeclarations[j].first, arStyle[i].first);

                if (mStyle.find(arDeclarations[j].first) == mStyle.cend())
                {
                    std::wstring sValue = ConvertUnitMeasure(arDeclarations[j].second);
                    mStyle.emplace(arDeclarations[j].first, sValue);
                }
                else
                {
                    std::vector<int> arWeightFirst = GetWeightSelector(arPropSel[arDeclarations[j].first]);
                    std::vector<int> arWeightSecond = GetWeightSelector(arStyle[i].first);

                    if ((arWeightFirst <= arWeightSecond &&
                         mStyle[arDeclarations[j].first].find(L"!important") == std::wstring::npos) ||
                        arDeclarations[j].second.find(L"!important") != std::wstring::npos)
                    {
                        std::wstring sValue = ConvertUnitMeasure(arDeclarations[j].second);
                        mStyle[arDeclarations[j].first] = sValue;
                    }
                }
            }
        }
        return  CCompiledStyle(mStyle);
    }

    void CCssCalculator_Private::AddStyle(std::vector<std::string> sSelectors, const std::string& sProperties)
    {
        std::string sPropertiesUTF8;
        if (m_sEncoding != L"UTF-8" && m_sEncoding != L"utf-8")
            sPropertiesUTF8 = GetContentAsUTF8(sProperties, m_sEncoding);
        else sPropertiesUTF8 = sProperties;

        std::vector<std::string> sSelectorsUTF8;

        if (m_sEncoding != L"UTF-8" && m_sEncoding != L"utf-8")
            for (size_t i = 0; i < sSelectors.size(); i++)
                sSelectorsUTF8.push_back(GetContentAsUTF8(sSelectors[i], m_sEncoding));
        else
            for (size_t i = 0; i < sSelectors.size(); i++)
                sSelectorsUTF8.push_back(sSelectors[i]);

        CElement *oElement = new CElement;
        for (size_t i = 0; i < sSelectorsUTF8.size(); i++)
            oElement->AddSelector(std::wstring(sSelectorsUTF8[i].begin(), sSelectorsUTF8[i].end()));

        std::vector<std::string> arProperty;
        std::vector<std::string> arValue;

        std::string sTemp;

        for (size_t i = 0; i < sPropertiesUTF8.length(); i++)
        {
            if (sPropertiesUTF8[i] != ' ')
            {
                if (sPropertiesUTF8[i] == ':')
                {
                    arProperty.push_back(sTemp);
                    sTemp.clear();
                }
                else if (sPropertiesUTF8[i] == ';')
                {
                    arValue.push_back(sTemp);
                    sTemp.clear();
                }
                else
                    sTemp += sPropertiesUTF8[i];
            }
        }

        if (!sTemp.empty())
            arValue.push_back(sTemp);

        std::vector<std::pair<std::wstring, std::wstring>> arDecl;

        size_t size;
        size_t max_size;

        if (arProperty.size() >= arValue.size())
        {
            size = arValue.size();
            max_size = arProperty.size();
        }
        else
        {
            size = arProperty.size();
            max_size = arValue.size();
        }
        for (size_t i = 0; i < size; i++)
        {
            std::wstring sProperty = std::wstring(arProperty[i].begin(), arProperty[i].end());
            std::wstring sValue = std::wstring(arValue[i].begin(), arValue[i].end());
            arDecl.push_back(std::make_pair(sProperty, sValue));
        }


        oElement->AddDeclarations(arDecl);
        m_arData.push_back(oElement);
    }

    void CCssCalculator_Private::AddStyles(const std::string& sStyle)
    {
        std::string sStyleUTF8;

        if (m_sEncoding != L"UTF-8" && m_sEncoding != L"utf-8")
            sStyleUTF8 = GetContentAsUTF8(sStyle, m_sEncoding);
        else
            sStyleUTF8 = sStyle;

        KatanaOutput *output = katana_parse(sStyleUTF8.c_str(), sStyleUTF8.size(), KatanaParserModeStylesheet);
        CCssCalculator_Private data;
        data.GetOutputData(output);

        for (size_t i = 0; i < data.GetSize(); i++)
        {
            CElement* oElement = new CElement();
            *oElement = *data.GetElement(i);
            AddElement(oElement);
        }
        katana_destroy_output(output);
    }

    void CCssCalculator_Private::AddStylesFromFile(const std::wstring& sFileName)
    {
        if (std::find(m_arFiles.begin(), m_arFiles.end(), sFileName) != m_arFiles.end())
            return;

        m_arFiles.push_back(sFileName);

        std::string sSourceUTF8 = GetContentAsUTF8(sFileName);

        AddStyles(sSourceUTF8);
    }

    inline std::wstring CCssCalculator_Private::ConvertUnitMeasure(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue;

        if (sConvertValue.find(L"px") != std::wstring::npos)
        {
            return ConvertPx(sConvertValue);
        }
        else if (sConvertValue.find(L"cm") != std::wstring::npos)
        {
            return ConvertCm(sConvertValue);
        }
        else if (sConvertValue.find(L"mm") != std::wstring::npos)
        {
            return ConvertMm(sConvertValue);
        }
        else if (sConvertValue.find(L"in") != std::wstring::npos)
        {
            return ConvertIn(sConvertValue);
        }
        else if (sConvertValue.find(L"pt") != std::wstring::npos)
        {
            return ConvertPt(sConvertValue);
        }
        else if (sConvertValue.find(L"pc") != std::wstring::npos)
        {
            return ConvertPc(sConvertValue);
        }

        return sConvertValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPx(const std::wstring& sValue)
    {
        if (sValue.find(L"px") == std::wstring::npos)
            return sValue;

        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"px"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"px") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"px") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
                break;
            case Cantimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPxToCm(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToCm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Inch:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPxToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToIn(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Millimeter:
            {
                std::wstring sValues;
                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPxToMm(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToMm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Point:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPxToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToPt(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Peak:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPxToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToPc(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 2.54 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"cm ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 1 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"in ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 25.4 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"mm ";

        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 1.0/6.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pc ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 1.0/72.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pt ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCm(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"cm"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"cm") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"cm") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertCmToPx(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToPx(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Cantimeter:
                break;
            case Inch:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertCmToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToIn(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Millimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertCmToMm(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToMm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Point:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertCmToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToPt(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Peak:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertCmToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToPc(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 2.54;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue * 10;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 72.0 / 2.54 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 6.0 / 2.54 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = (double)m_nDpi / 2.54 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMm(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"mm"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"mm") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"mm") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertMmToPx(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToPx(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Cantimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertMmToCm(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToCm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Inch:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertMmToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToIn(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Millimeter:
                break;
            case Point:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertMmToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToPt(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Peak:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertMmToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToPc(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 25.4;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 10;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 72.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = 6.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = (double)m_nDpi / 25.4 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertIn(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"in"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"in") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"in") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertInToPx(arDoubleValues[i]) + L" ";
                sValues += ConvertInToPx(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Cantimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertInToCm(arDoubleValues[i]) + L" ";
                sValues += ConvertInToCm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Inch:
                break;
            case Millimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertInToMm(arDoubleValues[i]) + L" ";
                sValues += ConvertInToMm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Point:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertInToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertInToPt(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Peak:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertInToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertInToPc(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertInToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue * 25.4;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue * 2.54;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue * (double)m_nDpi;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPt(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"pt"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"pt") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"pt") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPtToPx(arDoubleValues[i]) + L" ";
                sValues += ConvertPtToPx(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Cantimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPtToCm(arDoubleValues[i]) + L" ";
                sValues += ConvertPtToCm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Inch:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPtToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertPtToIn(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Millimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPtToMm(arDoubleValues[i]) + L" ";
                sValues += ConvertPtToMm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Point:
                break;
            case Peak:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPtToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertPtToPc(arDoubleValues[arDoubleValues.size() - 1], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPtToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 72.0 * 2.54;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 12.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 72.0 * 25.4;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = (double)m_nDpi / 72.0 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPc(const std::wstring& sValue)
    {
        std::wstring sConvertValue = sValue.substr(0, sValue.find(L"pc"));
        std::wstring sBeforeValue;
        std::wstring sAfterValue = sValue.substr(sValue.find(L"pc") + 2);

        sAfterValue = ConvertPx(sAfterValue);

        std::vector<std::wstring> arValues;

        std::wstring _doub;
        for (int i = sValue.find(L"pc") - 1; i >= 0; i--)
        {
            if (isdigit(sValue[i]) || sValue[i] == '.')
                _doub = sValue[i] +_doub;
            else if (sValue[i] == ' ' && !_doub.empty())
            {
                arValues.push_back(_doub);
                _doub.clear();
            }
            else
            {
                if (!_doub.empty())
                {
                    arValues.push_back(_doub);
                    _doub.clear();
                }
                sBeforeValue += sValue.substr(0, i + 1);
                break;
            }
        }

        if (!_doub.empty())
            arValues.push_back(_doub);

        if (arValues.empty())
            return sValue;

        std::vector<double> arDoubleValues;
        for (size_t i = 0; i < arValues.size(); i++)
            arDoubleValues.push_back(std::stod(arValues[i]));

        switch (m_UnitMeasure)
        {
            case Default:
            case Pixel:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPcToPx(arDoubleValues[i]) + L" ";
                sValues += ConvertPcToPx(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Cantimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPcToCm(arDoubleValues[i]) + L" ";
                sValues += ConvertPcToCm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Inch:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPcToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertPcToIn(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Millimeter:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPcToMm(arDoubleValues[i]) + L" ";
                sValues += ConvertPcToMm(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Point:
            {
                std::wstring sValues;

                if (arDoubleValues.size() > 1)
                    for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                        sValues += ConvertPcToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertPcToPt(arDoubleValues[0], true);

                if (!sBeforeValue.empty() && sBeforeValue[0] != L' ')
                    sBeforeValue += L" ";

                return sBeforeValue + sValues + sAfterValue;
            }
            case Peak:
                break;
            default:
                break;
        }
        return  sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPcToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 6.0 * 2.54;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue * 12.0;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = dValue / 6.0 * 25.4;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return std::to_wstring(0);

        double _dValue = (double)m_nDpi / 6.0 * dValue;
        std::wstring sValue = std::to_wstring(_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    void CCssCalculator_Private::SetDpi(const int nValue)
    {
        if (nValue > 0)
            m_nDpi = nValue;
    }

    void CCssCalculator_Private::SetUnitMeasure(const UnitMeasure nType)
    {
        m_UnitMeasure = nType;
    }

    int CCssCalculator_Private::GetDpi()
    {
        return m_nDpi;
    }

    inline size_t CCssCalculator_Private::GetSize()
    {
        return m_arData.size();
    }

    UnitMeasure CCssCalculator_Private::GetUnitMeasure()
    {
        return m_UnitMeasure;
    }

    std::wstring CCssCalculator_Private::GetEncoding()
    {
        return m_sEncoding;
    }
}
inline static std::wstring StringifyValueList(KatanaArray* oValues)
{
    if (NULL == oValues)
        return NULL;

    std::wstring buffer;

    for (size_t i = 0; i < oValues->length; ++i) {
        KatanaValue* value = (KatanaValue*)oValues->data[i];
        std::wstring str = StringifyValue(value);
        buffer += str;

        str.clear();

        if ( i < oValues->length - 1 ) {
            if ( value->unit != KATANA_VALUE_PARSER_OPERATOR ) {
                if ( i < oValues->length - 2 ) {
                    value = (KatanaValue*)oValues->data[i+1];
                    if ( value->unit != KATANA_VALUE_PARSER_OPERATOR ) {
                        buffer += L" ";
                    }
                } else {
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
            std::wstring args_str = StringifyValueList(oValue->function->args);
            str = stringToWstring(oValue->function->name) + args_str + L")";
            break;
        }
        case KATANA_VALUE_PARSER_OPERATOR:
            str = L" ";
            if (oValue->iValue != '=') {
                str.push_back((wchar_t)static_cast<char>(oValue->iValue));
                str += L" ";
            } else {
                str.push_back((wchar_t)static_cast<char>(oValue->iValue));
            }
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
