#include "CGetData.h"
#include <codecvt>

#include <string>
#include <algorithm>
#include <vector>

#include "../katana-parser/src/selector.h"

static std::wstring StringifyValueList(KatanaParser* parser, KatanaArray* values);
static std::wstring StringifyValue(KatanaParser* parser, KatanaValue* value);


CGetData::CGetData()
{
}

CGetData::~CGetData()
{
    for (size_t i = 0; i < m_arData.size(); i++)
        delete m_arData[i];

    m_arData.clear();
}

std::wstring stringToWstring(const std::string& t_str)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(t_str);
}

void CGetData::GetOutputData(KatanaOutput *oOutput)
{
    if ( NULL == oOutput )
        return;

    KatanaParser parser;
    parser.options = &kKatanaDefaultOptions;

    CElement *oElement = new CElement;

    switch (oOutput->mode) {
        case KatanaParserModeStylesheet:
            GetStylesheet(&parser, oOutput->stylesheet);
            break;
        case KatanaParserModeRule:
            GetRule(&parser, oOutput->rule);
            break;
        case KatanaParserModeKeyframeRule:
            oElement = GetKeyframe(&parser, oOutput->keyframe);
            break;
        case KatanaParserModeKeyframeKeyList:
            oElement->AddSelector(GetValueList(&parser, oOutput->keyframe_keys));
            break;
        case KatanaParserModeMediaList:
            oElement->AddSelector(GetMediaList(&parser, oOutput->medias));
            break;
        case KatanaParserModeValue:
            oElement->AddSelector(GetValueList(&parser, oOutput->values));
            break;
        case KatanaParserModeSelector:
            oElement->AddSelectors(GetSelectorList(&parser, oOutput->selectors));
            break;
        case KatanaParserModeDeclarationList:
            oElement->AddDeclarations(GetDeclarationList(&parser, oOutput->declarations));
            break;
    }
    if (oElement->GetCountSelectors() > 0 ||
        oElement->GetCountDeclarations() > 0)
        m_arData.push_back(oElement);
}

void CGetData::GetStylesheet(KatanaParser *oParser, KatanaStylesheet *oStylesheet, CElement *elementRule)
{
    for (size_t i = 0; i < oStylesheet->imports.length; ++i) {
        GetRule(oParser, (KatanaRule*)oStylesheet->imports.data[i], elementRule);
    }
    for (size_t i = 0; i < oStylesheet->rules.length; ++i) {
        GetRule(oParser, (KatanaRule*)oStylesheet->rules.data[i], elementRule);
    }

}

void CGetData::GetRule(KatanaParser *oParser, KatanaRule *oRule, CElement *elementRule)
{
    if ( NULL == oRule )
        return;


    switch (oRule->type) {
        case KatanaRuleStyle:
            GetStyleRule(oParser, (KatanaStyleRule*)oRule, elementRule);
            break;
        case KatanaRuleImport:
            GetImportRule(oParser, (KatanaImportRule*)oRule, elementRule);
            break;
        case KatanaRuleFontFace:
            GetFontFaceRule(oParser, (KatanaFontFaceRule*)oRule, elementRule);
            break;
        case KatanaRuleKeyframes:
            GetKeyframesRule(oParser, (KatanaKeyframesRule*)oRule, elementRule);
            break;
        case KatanaRuleMedia:
            GetMediaRule(oParser, (KatanaMediaRule*)oRule, elementRule);
            break;
        case KatanaRuleSupports:
            break;
        case KatanaRuleUnkown:
            break;

        default:
            break;
    }
}

CElement* CGetData::GetStyleRule(KatanaParser *oParser, KatanaStyleRule *oRule, CElement *elementRule)
{
    std::vector<std::wstring> arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    arSelectors = GetSelectorList(oParser, oRule->selectors);

    if ( oRule->declarations->length )
        arDeclarations = GetDeclarationList(oParser, oRule->declarations);

    CElement *oElement = new CElement;

    oElement->AddSelectors(arSelectors);
    oElement->AddDeclarations(arDeclarations);

    if (elementRule == NULL)
    {
        m_arData.push_back(oElement);
        return NULL;
    }
    else
        return oElement;
}

std::vector<std::wstring> CGetData::GetSelectorList(KatanaParser* oParser, KatanaArray* oSelectors)
{
    std::vector<std::wstring> arSelectors;
    for (size_t i = 0; i < oSelectors->length; ++i)
        arSelectors.push_back(GetSelector(oParser, (KatanaSelector*)oSelectors->data[i]));
    return arSelectors;
}

std::wstring CGetData::GetSelector(KatanaParser *oParser, KatanaSelector *oSelector)
{
    std::wstring sText;
    KatanaParserString * string = katana_selector_to_string(oParser, oSelector, NULL);
    const char* text = katana_string_to_characters(oParser, string);
    katana_parser_deallocate(oParser, (void*) string->data);
    katana_parser_deallocate(oParser, (void*) string);
    sText = stringToWstring(text);
    katana_parser_deallocate(oParser, (void*) text);
    return sText;
}

std::vector<std::pair<std::wstring, std::wstring>> CGetData::GetDeclarationList(KatanaParser* oParser, KatanaArray* oDeclarations)
{
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    for (size_t i = 0; i < oDeclarations->length; ++i)
        arDeclarations.push_back(GetDeclaration(oParser, (KatanaDeclaration*)oDeclarations->data[i]));

    return arDeclarations;
}
std::pair<std::wstring, std::wstring> CGetData::GetDeclaration(KatanaParser* oParser, KatanaDeclaration* oDecl)
{
    std::pair<std::wstring, std::wstring> pDeclaration;

    std::wstring ValueList = StringifyValueList(oParser, oDecl->values);
    if (oDecl->important)
        ValueList += L" !important";
    pDeclaration = std::make_pair(stringToWstring(oDecl->property), ValueList);

    return pDeclaration;
}

void CGetData::GetImportRule(KatanaParser *parser, KatanaImportRule *rule, CElement *elementRule)
{
    std::vector<std::wstring> arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    std::wstring sSelector = L"@" + stringToWstring(rule->base.name) + L" ";
    sSelector += L"url(" + stringToWstring(rule->href) + L")";
    arSelectors.push_back(sSelector);

    CElement *oElement = new CElement;

    oElement->AddSelectors(arSelectors);
    oElement->AddDeclarations(arDeclarations);

    m_arData.push_back(oElement);
}

void CGetData::GetFontFaceRule(KatanaParser *parser, KatanaFontFaceRule *rule, CElement *elementRule)
{
    std::vector<std::wstring> arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = GetDeclarationList(parser, rule->declarations);

    std::wstring sSelector = L"@" + stringToWstring(rule->base.name);
    arSelectors.push_back(sSelector);

    CElement *oElement = new CElement;

    oElement->AddSelectors(arSelectors);
    oElement->AddDeclarations(arDeclarations);

    m_arData.push_back(oElement);
}

void CGetData::GetKeyframesRule(KatanaParser *parser, KatanaKeyframesRule *rule, CElement *elementRule)
{
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    CElement *oElement = new CElement;

    std::wstring sSelector = L"@" + stringToWstring(rule->base.name);
    oElement->AddSelector(sSelector);

    for (size_t i = 0; i < rule->keyframes->length; i++)
        oElement->AddChildren(GetKeyframe(parser, (KatanaKeyframe*)rule->keyframes->data[i]));

    m_arData.push_back(oElement);
}

CElement* CGetData::GetKeyframe(KatanaParser *parser, KatanaKeyframe *keyframe)
{
    if (keyframe == NULL)
        return NULL;

    CElement *oElement = new CElement;

    std::wstring sSelector;
    for (size_t i = 0; i < keyframe->selectors->length; i++)
    {
        KatanaValue* oValue = (KatanaValue*)keyframe->selectors->data[i];
        if (oValue->unit == KATANA_VALUE_NUMBER)
        {
            oElement->AddSelector(stringToWstring(oValue->raw));
        }
        if (i != keyframe->selectors->length - 1)
        {
            sSelector += L", ";
        }
    }

    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = GetDeclarationList(parser, keyframe->declarations);

    oElement->AddDeclarations(arDeclarations);

    return oElement;
}

void CGetData::GetMediaRule(KatanaParser *parser, KatanaMediaRule *rule, CElement *elementRule)
{

    std::wstring sSelector = L"@" + stringToWstring(rule->base.name) + L" ";

    CElement *oElement = new CElement;

    if (rule->medias->length)
        sSelector += GetMediaList(parser, rule->medias);

    oElement->AddSelector(sSelector);

    if (rule->medias->length)
    {
        for (size_t i = 0; i < rule->rules->length; ++i)
        {
            oElement->AddChildren(GetStyleRule(parser, (KatanaStyleRule*)rule->rules->data[i], oElement));
        }
    }

    m_arData.push_back(oElement);
}

std::wstring CGetData::GetMediaList(KatanaParser *parser, KatanaArray *medias)
{
    std::wstring sText;
    bool first = true;
    for (size_t i = 0; i < medias->length; ++i) {
        if (!first)
            sText += L", ";
        else
            first = false;
        sText += GetMediaQuery(parser, (KatanaMediaQuery*)medias->data[i]);
    }
    return sText;
}

std::wstring CGetData::GetMediaQuery(KatanaParser *parser, KatanaMediaQuery *query)
{
    std::wstring sText;
    switch ( query->restrictor ) {
        case KatanaMediaQueryRestrictorOnly:
            sText += L"only ";
            break;
        case KatanaMediaQueryRestrictorNot:
            sText += L"not ";
            break;
        case KatanaMediaQueryRestrictorNone:
            break;
    }

    if ( NULL == query->expressions || 0 == query->expressions->length )
    {
        if ( NULL != query->type )
            sText += stringToWstring(query->type);
        return sText;
    }

    if ( (NULL != query->type && strcmpi(query->type, "all")) ||
         query->restrictor != KatanaMediaQueryRestrictorNone) {
        if ( NULL != query->type )
            sText += stringToWstring(query->type);

        sText += L" and ";
    }

    sText += GetMediaQueryExp(parser, (KatanaMediaQueryExp*)query->expressions->data[0]);

    for (size_t i = 1; i < query->expressions->length; ++i) {
        sText += L" and ";
        sText += GetMediaQueryExp(parser, (KatanaMediaQueryExp*)query->expressions->data[i]);
    }
    return sText;
}

std::wstring CGetData::GetMediaQueryExp(KatanaParser *parser, KatanaMediaQueryExp *exp)
{
    std::wstring sText;

    sText += L"(";
    if (NULL != exp->feature) {
        sText += stringToWstring(exp->feature);
    }
    if ( exp->values && exp->values->length ) {
        sText += L": " + StringifyValueList(parser, exp->values);
    }
    sText += L")";

    return sText;
}

std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CGetData::GetDeclarations(std::wstring sSelector)
{
    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arDeclarations;

    for (size_t i = 0; i < m_arData.size(); i++)
    {
        std::vector<std::wstring> arParent;
        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> _decl = m_arData[i]->GetDeclarations(sSelector, arParent);

        for (size_t i = 0; i < _decl.size(); i++)
            arDeclarations.push_back(_decl[i]);
    }

    return arDeclarations;
}

std::vector<int> CGetData::GetWeightSelector(std::string sSelector)
{
    std::vector<int> arWeight = {0, 0, 0};
    if (sSelector.length() == 0)
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

    for (int i = sSelector.size(); i >= 0; i--)
    {
        if (sSelector[i] == ']')
        {
            fl1 = true;
        }
        else if (sSelector[i] == '[')
        {
            fl1 = false;
            arSel.push_back('[' + sTempStr + ']');
            sTempStr.clear();
        }
        else if ((sSelector[i] == '.' || sSelector[i] == '#' || sSelector[i] == ' ' || sSelector[i] == ':') && !fl1)
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

//    for (size_t i = 0; i < arSel.size(); i++)
//        std::cout << arSel[i] << " - ";
//    std::cout << std::endl;

//    for (size_t i = 0; i < arWeight.size(); i++)
//        std::cout << arWeight[i] << " - ";
//    std::cout << std::endl;

    return arWeight;
}

std::vector<int> CGetData::GetWeightSelector(std::wstring sSelector)
{
    std::string sSel = std::string(sSelector.begin(), sSelector.end());
    return GetWeightSelector(sSel);
}

void CGetData::Print()
{
    std::wcout << m_arData.size() << std::endl;

    for (size_t i = 0; i < m_arData.size(); i++)
        std::wcout << m_arData[i]->GetText() << std::endl;
}

std::wstring CGetData::GetValueList(KatanaParser *parser, KatanaArray *values)
{
    return StringifyValueList(parser, values);
}

std::map<std::string, std::string> CGetData::GetStyle(std::vector<std::string> arSelectors)
{
    std::map<std::string, std::string> mStyle;

    std::map<std::wstring, std::wstring> mStyleW = GetStyleW(arSelectors);

    for (auto iter = mStyleW.begin(); iter != mStyleW.end(); iter++)
        mStyle.emplace(std::string(iter->first.begin(), iter->first.end()), std::string(iter->second.begin(), iter->second.end()));

    return mStyle;
}

std::map<std::wstring, std::wstring> CGetData::GetStyleW(std::vector<std::string> arSelectors)
{
    std::map<std::wstring, std::wstring> mStyle;

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arDecls;

    std::map<std::wstring, std::wstring> arPropSel; //мапа (свойство, что уже было использовано, селектор этого свойства)


    for (size_t i = 0; i < arSelectors.size(); i++)
    {
        std::wstring sSelector = std::wstring(arSelectors[i].begin(), arSelectors[i].end());
        std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arTempDecls = GetDeclarations(sSelector);
        arDecls.insert(arDecls.end(), arTempDecls.begin(), arTempDecls.end());
        arTempDecls.clear();
        arTempDecls = GetDeclarations(L"*");
        arDecls.insert(arDecls.end(), arTempDecls.begin(), arTempDecls.end());
    }


    for (size_t i = 0; i < arDecls.size(); i++)
    {
        std::vector<std::pair<std::wstring, std::wstring>> arDeclarations = arDecls[i].second;
        for (size_t j = 0; j < arDeclarations.size(); j++)
        {
            arPropSel.emplace(arDeclarations[j].first, arDecls[i].first);
            if (mStyle.find(arDeclarations[j].first) == mStyle.cend())
            {
                mStyle.emplace(arDeclarations[j].first, arDeclarations[j].second);
            }
            else
            {
                std::vector<int> arWeightFirst = GetWeightSelector(arPropSel[arDeclarations[j].first]);
                std::vector<int> arWeightSecond = GetWeightSelector(arDecls[i].first);

                if (arWeightFirst <= arWeightSecond)
                    mStyle[arDeclarations[j].first] = arDeclarations[j].second;
            }
        }
    }
    return  mStyle;
}

void CGetData::AddStyle(std::vector<std::string> sSelectors, std::string sStyle)
{
    CElement *oElement = new CElement;
    for (size_t i = 0; i < sSelectors.size(); i++)
        oElement->AddSelector(std::wstring(sSelectors[i].begin(), sSelectors[i].end()));

//    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;


    std::vector<std::string> sProperty;
    std::vector<std::string> sValue;

    std::string sTemp;

    for (size_t i = 0; i < sStyle.length(); i++)
    {
        if (sStyle[i] != ' ')
        {
            if (sStyle[i] == ':')
            {
                sProperty.push_back(sTemp);
                sTemp.clear();
            }
            else if (sStyle[i] == ';')
            {
                sValue.push_back(sTemp);
                sTemp.clear();
            }
            else
                sTemp += sStyle[i];
        }
    }

    if (!sTemp.empty())
        sValue.push_back(sTemp);

    for (size_t i = 0; i < sProperty.size(); i++)
        std::cout << sProperty[i] << " --- " << sValue[i] << std::endl;
//    oElement->AddDeclarations()
}

static std::wstring StringifyValueList(KatanaParser* parser, KatanaArray* values)
{
    if (NULL == values)
        return NULL;

    std::wstring buffer;


    for (size_t i = 0; i < values->length; ++i) {
        KatanaValue* value = (KatanaValue*)values->data[i];
        std::wstring str = StringifyValue(parser, value);

        buffer += str;

        str.clear();

        if ( i < values->length - 1 ) {
            if ( value->unit != KATANA_VALUE_PARSER_OPERATOR ) {
                if ( i < values->length - 2 ) {
                    value = (KatanaValue*)values->data[i+1];
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

static std::wstring StringifyValue(KatanaParser* parser, KatanaValue* value)
{
    std::wstring str;

    switch (value->unit) {
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
            str = stringToWstring(value->raw);
            break;
        case KATANA_VALUE_IDENT:
            str = stringToWstring(value->string);
            break;
        case KATANA_VALUE_STRING:
            str = stringToWstring(value->string);
            break;
        case KATANA_VALUE_PARSER_FUNCTION:
        {
            std::wstring args_str = StringifyValueList(parser, value->function->args);
            str = stringToWstring(value->function->name) + args_str + L")";
            break;
        }
        case KATANA_VALUE_PARSER_OPERATOR:
            if (value->iValue != '=') {
                str = (L" " + std::to_wstring(value->iValue));
            } else {
                str = (L" " + std::to_wstring(value->iValue));
            }
            break;
        case KATANA_VALUE_PARSER_LIST:
            return StringifyValueList(parser, value->list);
            break;
        case KATANA_VALUE_PARSER_HEXCOLOR:
            str = (L"#" + stringToWstring(value->string));
            break;
        case KATANA_VALUE_URI:
            str = (L"url(" + stringToWstring(value->string) + L")");
            break;
        default:
            break;
    }

    return str;
}
