#include "CGetData.h"
#include <codecvt>

#include "../Katana/katana-parser/src/parser.c"

CGetData::CGetData()
{

}

std::wstring stringToWstring(const std::string& t_str)
{
    //setup converter
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;

    //use converter (.to_bytes: wstr->str, .from_bytes: str->wstr)
    return converter.from_bytes(t_str);
}

void CGetData::GetOutputData(KatanaOutput *oOutput)
{
    if ( NULL == oOutput )
        return;

    KatanaParser parser;
    parser.options = &kKatanaDefaultOptions;

    switch (oOutput->mode) {
        case KatanaParserModeStylesheet:
            GetStylesheet(&parser, oOutput->stylesheet);
            break;
        case KatanaParserModeRule:
//            katana_print_rule(&parser, oOutput->rule);
            break;
        case KatanaParserModeKeyframeRule:
//            katana_print_keyframe(&parser, oOutput->keyframe);
            break;
        case KatanaParserModeKeyframeKeyList:
//            katana_print_value_list(&parser, oOutput->keyframe_keys);
            break;
        case KatanaParserModeMediaList:
//            katana_print_media_list(&parser, oOutput->medias);
            break;
        case KatanaParserModeValue:
//            katana_print_value_list(&parser, oOutput->values);
            break;
        case KatanaParserModeSelector:
//            katana_print_selector_list(&parser, oOutput->selectors);
            break;
        case KatanaParserModeDeclarationList:
//            katana_print_declaration_list(&parser, oOutput->declarations);
            break;
    }
}

void CGetData::GetStylesheet(KatanaParser *oParser, KatanaStylesheet *oStylesheet)
{
    for (size_t i = 0; i < oStylesheet->imports.length; ++i) {
        GetRule(oParser, (KatanaRule*)oStylesheet->imports.data[i]);
    }
    for (size_t i = 0; i < oStylesheet->rules.length; ++i) {
        GetRule(oParser, (KatanaRule*)oStylesheet->rules.data[i]);
    }
}

void CGetData::GetRule(KatanaParser *oParser, KatanaRule *oRule)
{
    if ( NULL == oRule )
        return;

    switch (oRule->type) {
        case KatanaRuleStyle:
            GetStyleRule(oParser, (KatanaStyleRule*)oRule);
            break;
        case KatanaRuleImport:
//            katana_print_import_rule(oParser, (KatanaImportRule*)oRule);
            break;
        case KatanaRuleFontFace:
//            katana_print_font_face_rule(oParser, (KatanaFontFaceRule*)oRule);
            break;
        case KatanaRuleKeyframes:
//            katana_print_keyframes_rule(oParser, (KatanaKeyframesRule*)oRule);
            break;
        case KatanaRuleMedia:
//            katana_print_media_rule(oParser, (KatanaMediaRule*)oRule);
            break;
        case KatanaRuleSupports:
            break;
        case KatanaRuleUnkown:
            break;

        default:
            break;
    }
}

void CGetData::GetStyleRule(KatanaParser *oParser, KatanaStyleRule *oRule)
{
    std::vector<std::wstring> arSelectors;
    std::vector<std::pair<std::wstring, std::wstring>> arDeclarations;

    arSelectors = GetSelectorList(oParser, oRule->selectors);

    if ( oRule->declarations->length )
        arDeclarations = GetDeclarationList(oParser, oRule->declarations);

    m_arData.push_back(std::make_pair(arSelectors, arDeclarations));
}

std::vector<std::wstring> CGetData::GetSelectorList(KatanaParser* oParser, KatanaArray* oSelectors)
{
    std::vector<std::wstring> arSelectors;
    for (size_t i = 0; i < oSelectors->length; ++i)
        arSelectors.push_back(GetSelector(oParser, (KatanaSelector*)oSelectors->data[i]));
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
