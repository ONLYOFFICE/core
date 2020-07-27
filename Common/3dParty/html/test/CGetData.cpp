#include "CGetData.h"
#include <codecvt>

#include <string>
//#include <algorithm>
#include <vector>
#include <fstream>


#include "../katana-parser/src/selector.h"

#define MAX_LINE_LENGTH 80

static std::wstring StringifyValueList(KatanaArray* oValues);
static std::wstring StringifyValue(KatanaValue* oValue);


CCssCalculator::CCssCalculator()
{
}

CCssCalculator::~CCssCalculator()
{
    for (size_t i = 0; i < m_arData.size(); i++)
        delete m_arData[i];

    m_arData.clear();
}

std::wstring stringToWstring(const std::string& sString)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    return converter.from_bytes(sString);
}

CElement* CCssCalculator::GetElement(const int& nIndex)
{
    if (nIndex < 0 || nIndex >= (int)m_arData.size())
        return NULL;

    return m_arData[nIndex];
}

void CCssCalculator::AddElement(CElement *oElement)
{
    if (oElement != NULL)
        m_arData.push_back(oElement);
}

void CCssCalculator::GetOutputData(KatanaOutput *oOutput)
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

void CCssCalculator::GetStylesheet(KatanaStylesheet *oStylesheet, CElement *elementRule)
{
    for (size_t i = 0; i < oStylesheet->imports.length; ++i) {
        GetRule((KatanaRule*)oStylesheet->imports.data[i], elementRule);
    }
    for (size_t i = 0; i < oStylesheet->rules.length; ++i) {
        GetRule((KatanaRule*)oStylesheet->rules.data[i], elementRule);
    }
}

void CCssCalculator::GetRule(KatanaRule *oRule, CElement *oElementRule)
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

CElement* CCssCalculator::GetStyleRule(KatanaStyleRule *oRule, CElement *oElementRule)
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

std::vector<std::wstring> CCssCalculator::GetSelectorList(KatanaArray* oSelectors)
{
    std::vector<std::wstring> arSelectors;
    for (size_t i = 0; i < oSelectors->length; ++i)
        arSelectors.push_back(GetSelector((KatanaSelector*)oSelectors->data[i]));
    return arSelectors;
}

std::wstring CCssCalculator::GetSelector(KatanaSelector *oSelector)
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

std::vector<std::pair<std::wstring, std::wstring>> CCssCalculator::GetDeclarationList(KatanaArray* oDeclarations)
{
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    for (size_t i = 0; i < oDeclarations->length; ++i)
        arDeclarations.push_back(GetDeclaration((KatanaDeclaration*)oDeclarations->data[i]));

    return arDeclarations;
}
std::pair<std::wstring, std::wstring> CCssCalculator::GetDeclaration(KatanaDeclaration* oDecl)
{
    std::pair<std::wstring, std::wstring> pDeclaration;

    std::wstring sValueList = StringifyValueList(oDecl->values);
    if (oDecl->important)
        sValueList += L" !important";
    pDeclaration = std::make_pair(stringToWstring(oDecl->property), sValueList);

    return pDeclaration;
}

void CCssCalculator::GetImportRule(KatanaImportRule *oRule)
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

void CCssCalculator::GetFontFaceRule(KatanaFontFaceRule *oRule)
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

void CCssCalculator::GetKeyframesRule(KatanaKeyframesRule *oRule)
{
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    CElement *oElement = new CElement;

    std::wstring sSelector = L"@" + stringToWstring(oRule->base.name);
    oElement->AddSelector(sSelector);

    for (size_t i = 0; i < oRule->keyframes->length; i++)
        oElement->AddChildren(GetKeyframe((KatanaKeyframe*)oRule->keyframes->data[i]));

    m_arData.push_back(oElement);
}

CElement* CCssCalculator::GetKeyframe(KatanaKeyframe *oKeyframe)
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

void CCssCalculator::GetMediaRule(KatanaMediaRule *oRule)
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

std::wstring CCssCalculator::GetMediaList(KatanaArray *oMedias)
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

std::wstring CCssCalculator::GetMediaQuery(KatanaMediaQuery *oQuery)
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

std::wstring CCssCalculator::GetMediaQueryExp(KatanaMediaQueryExp *oExp)
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

std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> CCssCalculator::GetDeclarations(const std::wstring& sSelector)
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

std::vector<int> CCssCalculator::GetWeightSelector(const std::string& sSelector)
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
        if (sSelector[i] == '*')
        {
            arWeight[2]++;
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

    return arWeight;
}

std::vector<int> CCssCalculator::GetWeightSelector(const std::wstring& sSelector)
{
    std::string sSel = std::string(sSelector.begin(), sSelector.end());
    return GetWeightSelector(sSel);
}

void CCssCalculator::Print()
{
    std::wcout << m_arData.size() << std::endl;

    for (size_t i = 0; i < m_arData.size(); i++)
        std::wcout << m_arData[i]->GetText() << std::endl;
}

std::wstring CCssCalculator::GetValueList(KatanaArray *oValues)
{
    return StringifyValueList(oValues);
}

std::map<std::string, std::string> CCssCalculator::GetCompiledStyle(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
{
    std::map<std::string, std::string> mStyle;

    std::map<std::wstring, std::wstring> mStyleW = GetCompiledStyleW(arSelectors, unitMeasure);

    for (auto iter = mStyleW.begin(); iter != mStyleW.end(); iter++)
        mStyle.emplace(std::string(iter->first.begin(), iter->first.end()), std::string(iter->second.begin(), iter->second.end()));

    return mStyle;
}

std::map<std::wstring, std::wstring> CCssCalculator::GetCompiledStyleW(std::vector<std::string> arSelectors, UnitMeasure unitMeasure)
{
    SetUnitMeasure(unitMeasure);

    std::map<std::wstring, std::wstring> mStyle;

    std::vector<std::pair<std::wstring, std::vector<std::pair<std::wstring, std::wstring>>>> arStyle;
//                        selector      declarations

    std::map<std::wstring, std::wstring> arPropSel; //мапа (свойство, что уже было использовано, селектор этого свойства)


    for (size_t i = 0; i < arSelectors.size(); i++)
    {
        std::wstring sSelector = std::wstring(arSelectors[i].begin(), arSelectors[i].end());
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

                if (arWeightFirst <= arWeightSecond)
                {
                    std::wstring sValue = ConvertUnitMeasure(arDeclarations[j].second);
                    mStyle[arDeclarations[j].first] = sValue;
                }
            }
        }
    }
    return  mStyle;
}

void CCssCalculator::AddStyle(std::vector<std::string> sSelectors, const std::string& sStyle)
{
    CElement *oElement = new CElement;
    for (size_t i = 0; i < sSelectors.size(); i++)
        oElement->AddSelector(std::wstring(sSelectors[i].begin(), sSelectors[i].end()));

    std::vector<std::string> arProperty;
    std::vector<std::string> arValue;

    std::string sTemp;

    for (size_t i = 0; i < sStyle.length(); i++)
    {
        if (sStyle[i] != ' ')
        {
            if (sStyle[i] == ':')
            {
                arProperty.push_back(sTemp);
                sTemp.clear();
            }
            else if (sStyle[i] == ';')
            {
                arValue.push_back(sTemp);
                sTemp.clear();
            }
            else
                sTemp += sStyle[i];
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

void CCssCalculator::AddStyle(const std::string& sStyle)
{
    KatanaOutput *output = katana_parse(sStyle.c_str(), sStyle.size(), KatanaParserModeStylesheet);
    CCssCalculator data;
    data.GetOutputData(output);

    for (size_t i = 0; i < data.GetSize(); i++)
    {
        CElement* oElement = new CElement();
        *oElement = *data.GetElement(i);
        AddElement(oElement);
    }
    katana_destroy_output(output);
}

void CCssCalculator::AddStyles(const std::wstring& sFileName)
{
    FILE *fFile = fopen(std::string(sFileName.begin(), sFileName.end()).c_str(), "r");

    if (fFile == NULL)
        return;

    KatanaOutput *output = katana_parse_in(fFile);

    CCssCalculator data;
    data.GetOutputData(output);
    for (size_t i = 0; i < data.GetSize(); i++)
    {
        CElement* oElement = new CElement();
        *oElement = *data.GetElement(i);
        AddElement(oElement);
    }
    data.m_arData.clear();
    katana_destroy_output(output);

    rewind(fFile);

    wchar_t line[MAX_LINE_LENGTH] = {0};
    while(std::fgetws(line, MAX_LINE_LENGTH, fFile))
    {
        std::wstring sTemp = line;
        if (sTemp.find(L"@charset") != std::string::npos)
        {
            char сQuote;
            if (sTemp.find(L"'") != std::string::npos)
                сQuote = '\'';
            else if (sTemp.find(L'"') != std::string::npos)
                сQuote = '"';
            else
                break;

            sTemp = sTemp.substr(sTemp.find(сQuote) + 1);
            sTemp = sTemp.substr(0, sTemp.find(сQuote));
            m_sEncoding = sTemp;
        }
    }

    fclose(fFile);
}

std::wstring CCssCalculator::ConvertUnitMeasure(const std::wstring& sValue)
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

std::wstring CCssCalculator::ConvertPx(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
            break;
        case Cantimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPxToCm(arDoubleValues[i]) + L" ";
            sValues += ConvertPxToCm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Inch:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPxToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToIn(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Millimeter:
        {
            std::wstring sValues;
            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPxToMm(arDoubleValues[i]) + L" ";
            sValues += ConvertPxToMm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Point:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPxToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToPt(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Peak:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPxToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertPxToPc(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertPxToCm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 2.54 / (double)m_nDpi * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"cm ";
    return sValue;
}

std::wstring CCssCalculator::ConvertPxToIn(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 1 / (double)m_nDpi * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"in ";
    return sValue;
}

std::wstring CCssCalculator::ConvertPxToMm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 25.4 / (double)m_nDpi * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"mm ";

    return sValue;
}

std::wstring CCssCalculator::ConvertPxToPc(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 1.0/6.0 / (double)m_nDpi * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pc ";
    return sValue;
}

std::wstring CCssCalculator::ConvertPxToPt(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 1.0/72.0 / (double)m_nDpi * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pt ";
    return sValue;
}

std::wstring CCssCalculator::ConvertCm(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertCmToPx(arDoubleValues[i]) + L" ";
            sValues += ConvertCmToPx(arDoubleValues[0], true);

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

            return sBeforeValue + sValues + sAfterValue;
        }
        case Millimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertCmToMm(arDoubleValues[i]) + L" ";
            sValues += ConvertCmToMm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Point:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertCmToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToPt(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Peak:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertCmToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertCmToPc(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertCmToIn(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 2.54;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"in";
    return sValue;
}

std::wstring CCssCalculator::ConvertCmToMm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue * 10;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"mm";
    return sValue;
}

std::wstring CCssCalculator::ConvertCmToPc(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 72.0 / 2.54 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pc";
    return sValue;
}

std::wstring CCssCalculator::ConvertCmToPt(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 6.0 / 2.54 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pt";
    return sValue;
}

std::wstring CCssCalculator::ConvertCmToPx(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = (double)m_nDpi / 2.54 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"px";
    return sValue;
}

std::wstring CCssCalculator::ConvertMm(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertMmToPx(arDoubleValues[i]) + L" ";
            sValues += ConvertMmToPx(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Cantimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertMmToCm(arDoubleValues[i]) + L" ";
            sValues += ConvertMmToCm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Inch:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertMmToIn(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToIn(arDoubleValues[0], true);

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

            return sBeforeValue + sValues + sAfterValue;
        }
        case Peak:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertMmToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertMmToPc(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertMmToIn(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 25.4;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"in";
    return sValue;
}

std::wstring CCssCalculator::ConvertMmToCm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 10;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"cm";
    return sValue;
}

std::wstring CCssCalculator::ConvertMmToPc(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 72.0 / 25.4 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pc";
    return sValue;
}

std::wstring CCssCalculator::ConvertMmToPt(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = 6.0 / 25.4 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pt";
    return sValue;
}

std::wstring CCssCalculator::ConvertMmToPx(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = (double)m_nDpi / 25.4 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"px";
    return sValue;}

std::wstring CCssCalculator::ConvertIn(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertInToPx(arDoubleValues[i]) + L" ";
            sValues += ConvertInToPx(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Cantimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertInToCm(arDoubleValues[i]) + L" ";
            sValues += ConvertInToCm(arDoubleValues[0], true);

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

            return sBeforeValue + sValues + sAfterValue;
        }
        case Point:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertInToPt(arDoubleValues[i]) + L" ";
                sValues += ConvertInToPt(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Peak:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertInToPc(arDoubleValues[i]) + L" ";
                sValues += ConvertInToPc(arDoubleValues[arDoubleValues.size() - 1], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertInToMm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue * 25.4;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"mm";
    return sValue;}

std::wstring CCssCalculator::ConvertInToCm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue * 2.54;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"cm";
    return sValue;}

std::wstring CCssCalculator::ConvertInToPc(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 72.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pc";
    return sValue;}

std::wstring CCssCalculator::ConvertInToPt(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 6.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pt";
    return sValue;}

std::wstring CCssCalculator::ConvertInToPx(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue * (double)m_nDpi;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"px";
    return sValue;}

std::wstring CCssCalculator::ConvertPt(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPtToPx(arDoubleValues[i]) + L" ";
            sValues += ConvertPtToPx(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Cantimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPtToCm(arDoubleValues[i]) + L" ";
            sValues += ConvertPtToCm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Inch:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPtToIn(arDoubleValues[i]) + L" ";
            sValues += ConvertPtToIn(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Millimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPtToMm(arDoubleValues[i]) + L" ";
            sValues += ConvertPtToMm(arDoubleValues[0], true);

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

            return sBeforeValue + sValues + sAfterValue;
        }
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertPtToIn(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 72.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"in";
    return sValue;}

std::wstring CCssCalculator::ConvertPtToCm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 72.0 * 2.54;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"cm";
    return sValue;}

std::wstring CCssCalculator::ConvertPtToPc(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 12.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pc";
    return sValue;}

std::wstring CCssCalculator::ConvertPtToMm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 72.0 * 25.4;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"mm";
    return sValue;}

std::wstring CCssCalculator::ConvertPtToPx(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = (double)m_nDpi / 72.0 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"px";
    return sValue;}

std::wstring CCssCalculator::ConvertPc(const std::wstring& sValue)
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
        case Defoult:
        case Pixel:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPcToPx(arDoubleValues[i]) + L" ";
            sValues += ConvertPcToPx(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Cantimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPcToCm(arDoubleValues[i]) + L" ";
            sValues += ConvertPcToCm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Inch:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPcToIn(arDoubleValues[i]) + L" ";
            sValues += ConvertPcToIn(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Millimeter:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPcToMm(arDoubleValues[i]) + L" ";
            sValues += ConvertPcToMm(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Point:
        {
            std::wstring sValues;

            if (arDoubleValues.size() > 1)
                for (size_t i = arDoubleValues.size() - 1; i > 0; i--)
                    sValues += ConvertPcToPt(arDoubleValues[i]) + L" ";
            sValues += ConvertPcToPt(arDoubleValues[0], true);

            return sBeforeValue + sValues + sAfterValue;
        }
        case Peak:
            break;
        default:
            break;
    }
    return  sValue;
}

std::wstring CCssCalculator::ConvertPcToIn(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 6.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"in";
    return sValue;}

std::wstring CCssCalculator::ConvertPcToCm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 6.0 * 2.54;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"cm";
    return sValue;}

std::wstring CCssCalculator::ConvertPcToPt(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue * 12.0;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"pt";
    return sValue;}

std::wstring CCssCalculator::ConvertPcToMm(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = dValue / 6.0 * 25.4;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"mm";
    return sValue;}

std::wstring CCssCalculator::ConvertPcToPx(const double& dValue, bool bAddUM)
{
    if (dValue == 0)
        return std::to_wstring(0);

    double _dValue = (double)m_nDpi / 6.0 * dValue;
    std::wstring sValue = std::to_wstring(_dValue);
    if (bAddUM)
        sValue += L"px";
    return sValue;}

void CCssCalculator::SetDpi(const int nValue)
{
    if (nValue > 0)
        m_nDpi = nValue;
}

void CCssCalculator::SetUnitMeasure(const UnitMeasure nType)
{
    m_UnitMeasure = nType;
}

int CCssCalculator::GetDpi()
{
    return m_nDpi;
}

size_t CCssCalculator::GetSize()
{
    return m_arData.size();
}

UnitMeasure CCssCalculator::GetUnitMeasure()
{
    return m_UnitMeasure;
}

std::wstring CCssCalculator::GetEncoding()
{
    return m_sEncoding;
}

static std::wstring StringifyValueList(KatanaArray* oValues)
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

static std::wstring StringifyValue(KatanaValue* oValue)
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
            str = stringToWstring(oValue->string);
            break;
        case KATANA_VALUE_PARSER_FUNCTION:
        {
            std::wstring args_str = StringifyValueList(oValue->function->args);
            str = stringToWstring(oValue->function->name) + args_str + L")";
            break;
        }
        case KATANA_VALUE_PARSER_OPERATOR:
            if (oValue->iValue != '=') {
                str = (L" " + std::to_wstring(oValue->iValue));
            } else {
                str = (L" " + std::to_wstring(oValue->iValue));
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
