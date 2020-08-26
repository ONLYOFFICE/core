#include "CCssCalculator_Private.h"
#include <codecvt>

#include <string>
#include <wchar.h>
#include <vector>
#include <fstream>
#include <cctype>
#include <algorithm>

#include <iostream>

#include "../../katana-parser/src/selector.h"
#include "../../../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../../../DesktopEditor/common/File.h"
#include "../../../../../DesktopEditor/common/StringBuilder.h"

static std::wstring StringifyValueList(KatanaArray* oValues);
static std::wstring StringifyValue(KatanaValue* oValue);

inline std::wstring stringToWstring(const std::string& sString)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(sString);
}

inline std::string wstringToString(const std::wstring& sWstring)
{
    return std::string(sWstring.begin(), sWstring.end());
}

inline std::string GetContentAsUTF8(const std::string &sString, const std::wstring &sEncoding)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

    std::string sEnc = converter.to_bytes(sEncoding);

    NSUnicodeConverter::CUnicodeConverter oConverter;
    std::wstring sUnicodeContent = oConverter.toUnicode(sString, sEnc.c_str());
    return U_TO_UTF8(sUnicodeContent);
}

inline std::wstring GetContentAsUTF8W(const std::wstring& sFileName)
{
    std::wstring sSource;
    if (!NSFile::CFileBinary::ReadAllTextUtf8(sFileName, sSource))
        return sSource;

    std::wstring sTemp;

    if (sSource.find(L'{') != std::string::npos)
        sTemp = sSource.substr(0, sSource.find(L'{'));

    if (sTemp.empty())
        return sSource;

    std::wstring sEncoding;

    if (sTemp.find(L"@charset") != std::string::npos)
    {
         sEncoding = sTemp.substr(sTemp.find(L"@charset ") + 9);
        if (sEncoding.find(L';') != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find(L';'));

        if (sEncoding.find(L'"') != std::string::npos)
            sEncoding = sEncoding.substr(sEncoding.find(L'"') + 1);
        else if (sEncoding.find(L"'") != std::string::npos)
            sEncoding = sEncoding.substr(sEncoding.find(L"'") + 1);

        if (sEncoding.find(L'"') != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find(L'"'));
        else if (sEncoding.find(L"'") != std::string::npos)
            sEncoding = sEncoding.substr(0, sEncoding.find(L"'"));
    }
    else
        return sSource;

    if (sEncoding == L"UTF-8" || sEncoding == L"utf-8")
        return sSource;

    NSUnicodeConverter::CUnicodeConverter oConverter;
    std::string sTempSource = wstringToString(sSource);
    std::wstring sUnicodeContent = oConverter.toUnicode(sTempSource.c_str(), wstringToString(sEncoding).c_str());
    return sUnicodeContent;
}

inline std::string GetContentAsUTF8(const std::wstring& sFileName)
{
    std::wstring sUnicodeContent =GetContentAsUTF8W(sFileName);
    return U_TO_UTF8(sUnicodeContent);
}

inline std::wstring GetFirstNumber(std::wstring sString)
{
    if (sString.empty())
        return L"";

    std::wstring sValue;
    int nPos = 0;

    while(nPos < (int)sString.length() && !isdigit(sString[nPos]))
        nPos++;

    while(nPos < (int)sString.length() && isdigit(sString[nPos]))
    {
        sValue += sString[nPos];
        nPos++;
    }

    if (sString[nPos] == L'.' || sString[nPos] == L',')
    {
        sValue += L'.';
        nPos++;
        while(nPos < (int)sString.length() && isdigit(sString[nPos]))
        {
            sValue += sString[nPos];
            nPos++;
        }
    }

    return sValue;
}

inline void RemoveExcessFromStyles(std::wstring& sStyle)
{
    while (sStyle.find_first_of(L'<') != std::wstring::npos || sStyle.find_first_of(L'@') != std::wstring::npos)
    {
        if (sStyle.find_first_of(L'<') != std::wstring::npos)
            sStyle.erase(sStyle.find_first_of(L'<', 0), sStyle.find_first_of(L'>', 0) - sStyle.find_first_of(L'<', 0) + 1);
        else if (sStyle.find_first_of(L'@') != std::wstring::npos)
        {
            sStyle.erase(sStyle.find_first_of(L'@'), 1);
        }
    }
}

inline void TranslateToEn(std::wstring& sStyle)
{
    std::map<int, std::wstring> arAlf =        {{1072, L"a"},     {1040, L"A"},
                                                {1073, L"b"},     {1041, L"B"},
                                                {1074, L"v"},     {1042, L"V"},
                                                {1075, L"g"},     {1043, L"G"},
                                                {1076, L"d"},     {1044, L"D"},
                                                {1077, L"e"},     {1045, L"E"},
                                                {1105, L"e"},     {1025, L"E"},
                                                {1078, L"zh"},    {1046, L"ZH"},
                                                {1079, L"z"},     {1047, L"Z"},
                                                {1080, L"i"},     {1048, L"I"},
                                                {1081, L"y"},     {1049, L"Y"},
                                                {1082, L"k"},     {1050, L"K"},
                                                {1083, L"l"},     {1051, L"L"},
                                                {1084, L"m"},     {1052, L"M"},
                                                {1085, L"n"},     {1053, L"N"},
                                                {1086, L"o"},     {1054, L"O"},
                                                {1087, L"p"},     {1055, L"P"},
                                                {1088, L"r"},     {1056, L"R"},
                                                {1089, L"s"},     {1057, L"S"},
                                                {1090, L"t"},     {1058, L"T"},
                                                {1091, L"u"},     {1059, L"U"},
                                                {1092, L"f"},     {1060, L"F"},
                                                {1093, L"kh"},    {1061, L"KH"},
                                                {1094, L"ts"},    {1062, L"TS"},
                                                {1095, L"ch"},    {1063, L"CH"},
                                                {1096, L"sh"},    {1064, L"SH"},
                                                {1097, L"shch"},  {1065, L"SHCH"},
                                                {1098, L""},      {1066, L""},
                                                {1099, L"y"},     {1067, L"Y"},
                                                {1100, L""},      {1068, L""},
                                                {1101, L"e"},     {1069, L"E"},
                                                {1102, L"yu"},    {1070, L"YU"},
                                                {1103, L"ya"},    {1071, L"YA"}};

    std::wstring sNewStyle;

    for (int i = 0; i < (int)sStyle.length(); i++)
    {
        if (arAlf.find((int)sStyle[i]) != arAlf.cend())
        {
            sNewStyle += arAlf[sStyle[i]];
        }
        else
        {
            if (sStyle[i] == L'}')
                sNewStyle += L"}\n";
            else
                sNewStyle += sStyle[i];
        }
    }
    sStyle = sNewStyle;
}

namespace NSCSS
{
    CCssCalculator_Private::CCssCalculator_Private()
    {
        m_sEncoding = L"UTF-8";
        m_nDpi = 96;
        m_UnitMeasure = Default;
        m_nCountNodes = 1;
    }

    CCssCalculator_Private::~CCssCalculator_Private()
    {
        for (size_t i = 0; i < m_arData.size(); i++)
            delete m_arData[i];

//        m_arStyleUsed.clear();
//        m_arData.clear();
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

        if (sText.find(L' ') != std::wstring::npos)
        {
            std::wstring sTempText;
            for (int i = 0; i < (int)sText.length(); i++)
                if (!iswspace(sText[i]))
                    sTempText += sText[i];

            return sTempText;
        }

        return sText;
    }

    inline std::vector<std::pair<std::wstring, std::wstring>> CCssCalculator_Private::GetDeclarationList(KatanaArray* oDeclarations)
    {
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

        for (size_t i = 0; i < oDeclarations->length; i++)
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

        if ((NULL != oQuery->type && strcmp(oQuery->type, "all")) ||
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

    inline std::wstring ConvertAbsoluteValue(const std::wstring& sAbsoluteValue)
    {
        std::map<std::wstring, std::wstring> arAbsoluteValues = {{L"xx-small", L"9px"},  {L"x-small", L"10px"},
                                                                 {L"small",    L"13px"}, {L"medium",  L"16px"},
                                                                 {L"large",    L"18px"}, {L"x-large", L"24px"},
                                                                 {L"xx-large", L"32px"}};

        std::wstring sNewValue = sAbsoluteValue;

        for (auto sAbsValue : arAbsoluteValues)
        {
            if (sNewValue.find(sAbsValue.first) != std::wstring::npos)
            {
                int nPos1, nPos2;
                nPos1 = sNewValue.find(sAbsValue.first);
                nPos2 = sNewValue.find(sAbsValue.first) + sAbsValue.first.length();

                std::wcout << sNewValue.substr(nPos1, nPos2) << std::endl;;
            }
        }

        return sAbsoluteValue;
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
    {
        if (unitMeasure != Default)
            SetUnitMeasure(unitMeasure);


        std::map<std::wstring, std::wstring> mStyle;

        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arStyle;
    //                        selector      declarations

        std::map<std::wstring, std::wstring> arPropSel; //мапа (свойство, что уже было использовано, селектор этого свойства)

        for (std::string sSel : arSelectors)
        {
            std::wstring sSelector = stringToWstring(sSel);

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

        for (auto iter = mStyle.begin(); iter != mStyle.end(); iter++)
        {
            if (iter->second.find(L"!") != std::wstring::npos)
                mStyle[iter->first] = iter->second.substr(0, iter->second.find(L"!"));
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
            std::wstring sValue = ConvertUnitMeasure(std::wstring(arValue[i].begin(), arValue[i].end()));
            arDecl.push_back(std::make_pair(sProperty, sValue));
        }


        oElement->AddDeclarations(arDecl);
        m_arData.push_back(oElement);
    }

    std::vector<std::string> GetWords(const std::wstring& sLine)
    {
        std::vector<std::string> arWords;
        std::wstring sTempWord;
        for (size_t i = 0; i < sLine.length(); i++)
        {
            if (iswspace(sLine[i]))
            {
                if (!sLine.empty())
                {
                    std::string sTempStr = std::string(sTempWord.begin(), sTempWord.end());
                    if (std::find(arWords.begin(), arWords.end(), sTempStr) == arWords.cend())
                    {
                        arWords.push_back(sTempStr);
                    }
                    sTempWord.clear();
                }
            }
            else if (sLine[i] != L'.' && sLine[i] != L'#')
                sTempWord += sLine[i];
        }

        if (!sTempWord.empty())
        {
            std::string sTempStr = std::string(sTempWord.begin(), sTempWord.end());
            if (std::find(arWords.begin(), arWords.end(), sTempStr) == arWords.cend())
            {
                arWords.push_back(sTempStr);
            }
        }
        return arWords;
    }

    std::vector<std::string> GetSelectorsList(const std::wstring& sSelectors)
    {
        std::vector<std::string> arSelectors;

        std::wstring sNames = sSelectors;
        if (sNames.find(L'#') != std::wstring::npos)
            sNames = sNames.substr(0, sNames.find(L'#'));
        if (sNames.find(L'.') != std::wstring::npos)
            sNames = sNames.substr(0, sNames.find(L'.'));

        std::wstring sClasses;
        if (sSelectors.find(L'.') != std::wstring::npos)
            sClasses = sSelectors.substr(sSelectors.find('.'));
        if (sClasses.find(L'#') != std::wstring::npos)
            sClasses = sClasses.substr(0, sClasses.find(L'#'));

        std::wstring sIds;
        if (sSelectors.find(L'#') != std::wstring::npos)
            sIds = sSelectors.substr(sSelectors.find('#'));

        std::vector<std::string> arNames    = GetWords(sNames);
        std::vector<std::string> arClasses  = GetWords(sClasses);
        std::vector<std::string> arIds      = GetWords(sIds);

        arSelectors.insert(arSelectors.end(), arNames.begin(), arNames.end());

        for (size_t i = 0; i < arClasses.size(); i++)
        {
            if (arClasses[i].find('.') == std::string::npos)
                arClasses[i] = '.' + arClasses[i];

            arSelectors.push_back(arClasses[i]);
        }

        for (size_t i = 0; i < arIds.size(); i++)
        {
            if (arIds[i].find('#') == std::string::npos)
                arIds[i] = '#' + arIds[i];

            arSelectors.push_back(arIds[i]);
        }

        if (arClasses.size() > 0 || arIds.size() > 0)
        {
            for (std::string sName : arNames)
            {
                for (std::string sClass : arClasses)
                    arSelectors.push_back(sName + sClass);
                for (std::string sId : arIds)
                    arSelectors.push_back(sName + sId);
            }
        }

        if (arIds.size() > 0)
        {
            for (std::string sClass : arClasses)
            {
                for (std::string sId : arIds)
                    arSelectors.push_back(sClass + sId);
            }
        }

        return arSelectors;
    }

    CCompiledStyle CCssCalculator_Private::GetCompiledStyle(const CNode &oNode, const std::vector<CNode> &oParents, UnitMeasure unitMeasure)
    {
        CCompiledStyle oStyle;
        oStyle.Clear();

//        if (oParents.size() > 0)
//            std::wcout << oNode.m_sName << std::endl;

        std::wstring sClassName = oNode.m_sClass;
        TranslateToEn(sClassName);

        if (sClassName[0] != L'.' && !sClassName.empty())
            sClassName = L'.' + sClassName;

        std::wstring sIdName = oNode.m_sId;
        TranslateToEn(sIdName);

        if (sIdName[0] != L'#' && !sIdName.empty())
            sIdName = L'#' + sIdName;

        for (auto oParent : oParents)
        {
            oStyle += GetCompiledStyle(oParent, {}, unitMeasure);
            oStyle.AddParent(oParent.m_sName);
        }

        oStyle += GetCompiledStyle(GetSelectorsList(oNode.m_sName + sClassName + sIdName), unitMeasure);

        if (!oNode.m_sStyle.empty())
        {
            CCompiledStyle oTempStyle;
            oTempStyle.AddStyle(ConvertUnitMeasure(oNode.m_sStyle));
//            oTempStyle.AddStyle(oNode.m_sStyle);
            if (!oTempStyle.Empty())
            {
                oStyle += oTempStyle;
            }
        }

        oStyle.SetID(oNode.m_sName + sClassName + sIdName + L'-' + std::to_wstring(m_nCountNodes));
        m_nCountNodes++;

        m_arUsedNode.push_back(std::make_pair(oNode, std::make_pair(oParents, oStyle.GetId())));

        return oStyle;
    }

    void CCssCalculator_Private::AddStyles(const std::string &sStyle)
    {
        std::string sStyleUTF8;

        if (m_sEncoding != L"UTF-8" && m_sEncoding != L"utf-8")
            sStyleUTF8 = GetContentAsUTF8(sStyle, m_sEncoding);
        else
            sStyleUTF8 = sStyle;

        CCssCalculator_Private data;

        KatanaOutput *output = katana_parse(sStyleUTF8.c_str(), sStyleUTF8.size(), KatanaParserModeStylesheet);
        data.GetOutputData(output);

        for (size_t i = 0; i < data.GetSize(); i++)
        {
            CElement* oElement = new CElement();
            *oElement = *data.GetElement(i);
            AddElement(oElement);
        }
        katana_destroy_output(output);

    }

    void CCssCalculator_Private::AddStyles(const std::wstring &sStyle)
    {
        std::wstring sNewStyle = sStyle;
        RemoveExcessFromStyles(sNewStyle);
        TranslateToEn(sNewStyle);
        AddStyles(wstringToString(sNewStyle));
    }

    void CCssCalculator_Private::AddStylesFromFile(const std::wstring& sFileName)
    {
        if (std::find(m_arFiles.begin(), m_arFiles.end(), sFileName) != m_arFiles.end())
            return;

        m_arFiles.push_back(sFileName);

        std::wstring sSourceUTF8 = GetContentAsUTF8W(sFileName);
        RemoveExcessFromStyles(sSourceUTF8);
        TranslateToEn(sSourceUTF8);

        AddStyles(sSourceUTF8);
    }


    std::wstring CCssCalculator_Private::ConvertUnitMeasure(const std::wstring &sValue)
    {
        if (sValue.empty())
            return sValue;

        std::vector<std::wstring> arValues;

        std::wstring sTempString;
        if (sValue.find(L':') != std::wstring::npos)
        {
            for (int  i = 0; i < (int)sValue.length(); i++)
            {
                if (sValue[i] == L':' || sValue[i] == L';')
                {
                    if (!sTempString.empty())
                    {
                        sTempString += sValue[i];
                        sTempString = ConvertAbsoluteValue(sTempString);
                        if (sTempString.find(L'.') != std::wstring::npos)
                        {
                            if (sTempString.find(L'.') == 0)
                                sTempString = L'0' + sTempString;
                            else if (!iswdigit(sTempString[sTempString.find(L'.') - 1]))
                                sTempString.insert(sTempString.find(L'.') - 1, L"0");
                        }
                        arValues.push_back(sTempString);
                    }
                    sTempString.clear();
                }
                else if (!iswspace(sValue[i]))
                {
                    sTempString += sValue[i];
                }
            }
        }
        else
            arValues.push_back(sValue);

        if (!sTempString.empty())
        {
            if (sTempString.find(L'.') != std::wstring::npos)
            {
                if (sTempString.find(L'.') == 0)
                    sTempString = L'0' + sTempString;
                else if (!iswdigit(sTempString[sTempString.find(L'.') - 1]))
                    sTempString.insert(sTempString.find(L'.') - 1, L"0");
            }
            arValues.push_back(sTempString);
        }

        std::wstring sValueString;

        for (int i = 0; i < (int)arValues.size(); i++)
        {
            if (GetFirstNumber(arValues[i]).empty() || arValues[i].find(L'#') != std::wstring::npos)
            {
                sValueString += arValues[i];
                if (arValues[i][arValues[i].length() - 1] != L':' && arValues[i][arValues[i].length() - 1] != L';' && arValues.size() > 1)
                    sValueString += L';';
                continue;
            }
            std::wstring sTempValue;
            if (arValues[i].find(L'%') != std::wstring::npos)
            {
                double dValue = wcstod(arValues[i].substr(0, arValues[i].find(L'%')).c_str(), NULL);
                dValue /= 100;
                dValue = 22 * dValue;
                sTempValue = std::to_wstring((int)dValue);
            }
            else if (arValues[i].find(L"px") != std::wstring::npos)
            {
                sTempValue = ConvertPx(arValues[i]);
            }
            else if (arValues[i].find(L"cm") != std::wstring::npos)
            {
                sTempValue = ConvertCm(arValues[i]);
            }
            else if (arValues[i].find(L"mm") != std::wstring::npos)
            {
                sTempValue = ConvertMm(arValues[i]);
            }
            else if (arValues[i].find(L"in") != std::wstring::npos)
            {
                sTempValue = ConvertIn(arValues[i]);
            }
            else if (arValues[i].find(L"pt") != std::wstring::npos)
            {
                sTempValue = ConvertPt(arValues[i]);
            }
            else if (arValues[i].find(L"pc") != std::wstring::npos)
            {
                sTempValue = ConvertPc(arValues[i]);
            }
            else if (arValues[i].find(L"em") != std::wstring::npos)
            {
                sTempValue = ConvertEm(arValues[i]);
            }

            if (!sTempValue.empty())
            {
                sValueString += sTempValue;
                if (sTempValue[sTempValue.length() - 1] != L';' && arValues.size() > 1)
                    sValueString += L';';
            }
        }
        return sValueString;
    }


    inline std::wstring CCssCalculator_Private::ConvertPx(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"px") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;
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

    inline std::wstring CCssCalculator_Private::ConvertPxToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / (double)m_nDpi * 2.54;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"cm ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 1 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"in ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / (double)m_nDpi * 25.4;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"mm ";

        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 1.0/6.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pc ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertPxToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 72.0 / (double)m_nDpi * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pt ";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCm(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"cm") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;

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

    inline std::wstring CCssCalculator_Private::ConvertCmToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 2.54;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 10;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 72.0 / 2.54 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 6.0 / 2.54 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertCmToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 2.54 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMm(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"mm") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;

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

    inline std::wstring CCssCalculator_Private::ConvertMmToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 25.4;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 10;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 72.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = 6.0 / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertMmToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 25.4 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertIn(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"in") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;

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

    inline std::wstring CCssCalculator_Private::ConvertInToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 25.4;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 2.54;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertInToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * (double)m_nDpi;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPt(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"pt") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;

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

    inline std::wstring CCssCalculator_Private::ConvertPtToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0 * 2.54;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPc(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 12.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pc";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 72.0 * 25.4;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPtToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 72.0 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPc(const std::wstring& sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"pc") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);
        dValue *= 2;

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

    inline std::wstring CCssCalculator_Private::ConvertPcToIn(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"in";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToCm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0 * 2.54;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"cm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPt(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue * 12.0;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"pt";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToMm(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = dValue / 6.0 * 25.4;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"mm";
        return sValue;}

    inline std::wstring CCssCalculator_Private::ConvertPcToPx(const double& dValue, bool bAddUM)
    {
        if (dValue == 0)
            return L"0";

        double _dValue = (double)m_nDpi / 6.0 * dValue;
        std::wstring sValue = std::to_wstring((int)_dValue);
        if (bAddUM)
            sValue += L"px";
        return sValue;
    }

    inline std::wstring CCssCalculator_Private::ConvertEm(const std::wstring &sValue)
    {
        if (sValue.empty())
            return L"";

        std::wstring sConvertValue = sValue.substr(0, sValue.find_last_of(L"em") - 1);
        double dValue = wcstod(sConvertValue.c_str(), NULL);

        dValue *= 44;

        return std::to_wstring((int)dValue);
    }

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

    void CCssCalculator_Private::Clear()
    {
        m_sEncoding     = L"UTF-8";
        m_nDpi          = 96;
        m_UnitMeasure   = Default;


        for (size_t i = 0; i < m_arData.size(); i++)
            delete m_arData[i];

//        m_arStyleUsed.clear();
        m_arData.clear();
        m_arFiles.clear();
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
