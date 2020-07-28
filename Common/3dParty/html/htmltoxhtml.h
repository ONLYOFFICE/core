#ifndef HTMLTOXHTML_H
#define HTMLTOXHTML_H

#include <string>
#include "gumbo-parser/src/gumbo.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"

#ifndef HTMLTOXHTML_USE_DYNAMIC_LIBRARY
#define HTMLTOXHTML_DECL_EXPORT
#else
#include "../../../DesktopEditor/common/base_export.h"
#define HTMLTOXHTML_DECL_EXPORT Q_DECL_EXPORT
#endif

static std::string nonbreaking_inline  = "|a|abbr|acronym|b|bdo|big|cite|code|dfn|em|font|i|img|kbd|nobr|s|small|span|strike|strong|sub|sup|tt|";
static std::string empty_tags          = "|area|base|basefont|bgsound|br|command|col|embed|event-source|frame|hr|image|img|input|keygen|link|menuitem|meta|param|source|spacer|track|wbr|";
static std::string preserve_whitespace = "|pre|textarea|script|style|";
static std::string special_handling    = "|html|body|";
static std::string no_entity_sub       = "|script|style|";
static std::string treat_like_inline   = "|p|";

static void prettyprint(GumboNode*, NSStringUtils::CStringBuilderA& oBuilder);

static std::wstring htmlToXhtml(const std::wstring& sFile)
{
    std::wstring sRes;
    std::string sFileContent;
    if(!NSFile::CFileBinary::ReadAllTextUtf8A(sFile, sFileContent))
        return sRes;

    // Распознование кодировки
    size_t posEncoding = sFileContent.find("charset=");
    if (std::string::npos == posEncoding)
        return sRes;
    posEncoding += 8;
    if(sFileContent[posEncoding] == '\"')
        posEncoding += 1;

    size_t posEnd = sFileContent.find("\"", posEncoding);
    if (std::string::npos == posEnd)
        return sRes;

    std::string sEncoding = sFileContent.substr(posEncoding, posEnd - posEncoding);
    if (sEncoding != "utf-8" && sEncoding != "UTF-8")
    {
        sFileContent.replace(posEncoding, posEnd - posEncoding, "UTF-8");
        NSUnicodeConverter::CUnicodeConverter oConverter;
        sRes = oConverter.toUnicode(sFileContent, sEncoding.c_str());
        sFileContent = U_TO_UTF8(sRes);
    }

    // Gumbo
    GumboOptions options = kGumboDefaultOptions;
    GumboOutput* output = gumbo_parse_with_options(&options, sFileContent.data(), sFileContent.length());

    // prettyprint
    NSStringUtils::CStringBuilderA oBuilder;
    prettyprint(output->document, oBuilder);
    std::string sR = oBuilder.GetData();

    // Вставка кодировки в файл
    if(sR.length() > 5)
    {
        std::string sSub = sR.substr(0, 5);
        if(sSub != "<?xml")
            sR = "<?xml version=\'1.0\' encoding=\'UTF-8\'?>" + sR;
    }

    // Конвертирование из string utf8 в wstring
    return UTF8_TO_U(sR);
}

// Заменяет в строке s все символы s1 на s2
static void replace_all(std::string& s, std::string s1, std::string s2)
{
    size_t len = s1.length();
    size_t pos = s.find(s1);
    while (pos != std::string::npos)
    {
        s.replace(pos, len, s2);
        pos = s.find(s1, pos + len);
    }
}

// Заменяет сущности &,<,> в text
static void substitute_xml_entities_into_text(std::string &text)
{
    // replacing & must come first
    replace_all(text, "&", "&amp;");
    replace_all(text, "<", "&lt;");
    replace_all(text, ">", "&gt;");
}

// Заменяет сущности ",' в text
static void substitute_xml_entities_into_attributes(char quote, std::string &text)
{
   substitute_xml_entities_into_text(text);
    if (quote == '"')
        replace_all(text, "\"", "&quot;");
    else if (quote == '\'')
        replace_all(text, "'", "&apos;");
}


static std::string handle_unknown_tag(GumboStringPiece* text)
{
    if (text->data == NULL) {
        return "";
    }
    GumboStringPiece gsp = *text;
    gumbo_tag_from_original_text(&gsp);
    return std::string(gsp.data, gsp.length);
}


static std::string get_tag_name(GumboNode* node)
{
    std::string tagname = (node->type == GUMBO_NODE_DOCUMENT ? "document" : gumbo_normalized_tagname(node->v.element.tag));
    if (tagname.empty())
        tagname = handle_unknown_tag(&node->v.element.original_tag);
    return tagname;
}


static void build_doctype(GumboNode* node, NSStringUtils::CStringBuilderA& oBuilder)
{
    if (node->v.document.has_doctype)
    {
        oBuilder.WriteString("<!DOCTYPE ");
        oBuilder.WriteString(node->v.document.name);
        std::string pi(node->v.document.public_identifier);
        if ((node->v.document.public_identifier != NULL) && !pi.empty())
        {
            oBuilder.WriteString(" PUBLIC \"");
            oBuilder.WriteString(pi);
            oBuilder.WriteString("\" \"");
            oBuilder.WriteString(node->v.document.system_identifier);
            oBuilder.WriteString("\"");
        }
        oBuilder.WriteString(">");
    }
}


static void build_attributes(GumboAttribute* at, bool no_entities, NSStringUtils::CStringBuilderA& atts)
{
    std::string sVal(at->value);
    char quote = '"';
    atts.WriteString(" ");
    atts.WriteString(at->name);

    // determine original quote character used if it exists
    std::string qs ="\"";

    atts.WriteString("=");
    atts.WriteString(qs);
    if(!no_entities)
        substitute_xml_entities_into_attributes(quote, sVal);
    atts.WriteString(sVal);
    atts.WriteString(qs);
}


static void prettyprint_contents(GumboNode* node, NSStringUtils::CStringBuilderA& contents)
{
    std::string key             = "|" + get_tag_name(node) + "|";
    bool no_entity_substitution = no_entity_sub.find(key) != std::string::npos;
    bool keep_whitespace        = preserve_whitespace.find(key) != std::string::npos;
    bool is_inline              = nonbreaking_inline.find(key) != std::string::npos;

    GumboVector* children = &node->v.element.children;

    for (unsigned int i = 0; i < children->length; ++i)
    {
        GumboNode* child = static_cast<GumboNode*> (children->data[i]);

        if (child->type == GUMBO_NODE_TEXT)
        {
            std::string val(child->v.text.text);
            if(!no_entity_substitution)
                substitute_xml_entities_into_text(val);
            contents.WriteString(val);
        }
        else if ((child->type == GUMBO_NODE_ELEMENT) || (child->type == GUMBO_NODE_TEMPLATE))
        {
            prettyprint(child, contents);
        }
        else if (child->type == GUMBO_NODE_WHITESPACE)
        {
            if (keep_whitespace || is_inline)
                contents.WriteString(child->v.text.text);
        }
        else if (child->type != GUMBO_NODE_COMMENT)
        {
            // Сообщение об ошибке
            // Does this actually exist: (child->type == GUMBO_NODE_CDATA)
            // fprintf(stderr, "unknown element of type: %d\n", child->type);
        }
    }
}


static void prettyprint(GumboNode* node, NSStringUtils::CStringBuilderA& oBuilder)
{
    // special case the document node
    if (node->type == GUMBO_NODE_DOCUMENT)
    {
        build_doctype(node, oBuilder);
        prettyprint_contents(node, oBuilder);
        return;
    }

    std::string close              = "";
    std::string closeTag           = "";
    std::string tagname            = get_tag_name(node);
    std::string key                = "|" + tagname + "|";
    bool is_empty_tag              = empty_tags.find(key) != std::string::npos;
    bool no_entity_substitution    = no_entity_sub.find(key) != std::string::npos;

    // determine closing tag type
    if (is_empty_tag)
        close = "/";
    else
        closeTag = "</" + tagname + ">";

    // build results
    oBuilder.WriteString("<" + tagname);

    // build attr string
    const GumboVector * attribs = &node->v.element.attributes;
    for (int i = 0; i < attribs->length; ++i)
    {
        GumboAttribute* at = static_cast<GumboAttribute*>(attribs->data[i]);
        build_attributes(at, no_entity_substitution, oBuilder);
    }
    oBuilder.WriteString(close + ">");

    // prettyprint your contents
    prettyprint_contents(node, oBuilder);
    oBuilder.WriteString(closeTag);
}

#endif // HTMLTOXHTML_H
