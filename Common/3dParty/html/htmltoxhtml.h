#ifndef HTMLTOXHTML_H
#define HTMLTOXHTML_H

#include <string>
#include <map>
#include <cctype>
#include <vector>
#include <algorithm>

#include "gumbo-parser/src/gumbo.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/common/StringBuilder.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"

static std::string nonbreaking_inline  = "|a|abbr|acronym|b|bdo|big|cite|code|dfn|em|font|i|img|kbd|nobr|s|small|span|strike|strong|sub|sup|tt|";
static std::string empty_tags          = "|area|base|basefont|bgsound|br|command|col|embed|event-source|frame|hr|image|img|input|keygen|link|menuitem|meta|param|source|spacer|track|wbr|";
static std::string preserve_whitespace = "|pre|textarea|script|style|";
static std::string special_handling    = "|html|body|";
static std::string no_entity_sub       = ""; //"|style|";
static std::string treat_like_inline   = "|p|";

static void prettyprint(GumboNode*, NSStringUtils::CStringBuilderA& oBuilder);
static std::string mhtTohtml(std::string& sFileContent);

// Заменяет в строке s все символы s1 на s2
static void replace_all(std::string& s, const std::string& s1, const std::string& s2)
{
    size_t pos = s.find(s1);
    while(pos != std::string::npos)
    {
        s.replace(pos, s1.length(), s2);
        pos = s.find(s1, pos + s2.length());
    }
}

static std::wstring htmlToXhtml(std::string& sFileContent, bool bNeedConvert)
{
    // Распознование кодировки
    if (bNeedConvert)
    {
        size_t posEncoding = sFileContent.find("charset=");
        if (posEncoding == std::string::npos)
            posEncoding = sFileContent.find("encoding=");
        if (posEncoding != std::string::npos)
        {
            posEncoding = sFileContent.find("=", posEncoding) + 1;
            char quoteSymbol = '\"';
            if(sFileContent[posEncoding] == '\"' || sFileContent[posEncoding] == '\'')
            {
                quoteSymbol = sFileContent[posEncoding];
                posEncoding += 1;
            }

            size_t posEnd = sFileContent.find(quoteSymbol, posEncoding);
            if (std::string::npos != posEnd)
            {
                std::string sEncoding = sFileContent.substr(posEncoding, posEnd - posEncoding);
                if (sEncoding != "utf-8" && sEncoding != "UTF-8")
                {
                    NSUnicodeConverter::CUnicodeConverter oConverter;
                    sFileContent = U_TO_UTF8(oConverter.toUnicode(sFileContent, sEncoding.c_str()));
                }
            }
        }
    }

    // Избавление от <a/>
    size_t posA = sFileContent.find("<a ");
    while(posA != std::string::npos)
    {
        size_t nBegin = sFileContent.find('<',  posA + 1);
        size_t nEnd   = sFileContent.find("/>", posA);
        if(nEnd < nBegin)
            sFileContent.replace(nEnd, 2, "></a>");
        posA = sFileContent.find("<a ", nBegin);
    }
    // Избавление от <title/>
    posA = sFileContent.find("<title/>");
    while (posA != std::string::npos)
    {
        sFileContent.replace(posA, 8, "<title></title>");
        posA = sFileContent.find("<title/>", posA);
    }

    // Gumbo
    GumboOptions options = kGumboDefaultOptions;
    GumboOutput* output = gumbo_parse_with_options(&options, sFileContent.data(), sFileContent.length());

    // prettyprint
    NSStringUtils::CStringBuilderA oBuilder;
    prettyprint(output->document, oBuilder);

    // Конвертирование из string utf8 в wstring
    return UTF8_TO_U(oBuilder.GetData());
}

static std::string Base64ToString(const std::string& sContent, const std::string& sCharset)
{
    std::string sRes;
    int nSrcLen = (int)sContent.length();
    int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
    BYTE* pData = new BYTE[nDecodeLen];
    if (TRUE == NSBase64::Base64Decode(sContent.c_str(), nSrcLen, pData, &nDecodeLen))
    {
        std::wstring sConvert;
        if(!sCharset.empty() && sCharset != "utf-8" && sCharset != "UTF-8")
        {
            NSUnicodeConverter::CUnicodeConverter oConverter;
            sConvert = oConverter.toUnicode(reinterpret_cast<char *>(pData), (unsigned)nDecodeLen, sCharset.data());
        }
        sRes = sConvert.empty() ? std::string(reinterpret_cast<char *>(pData), nDecodeLen) : U_TO_UTF8(sConvert);
    }
    RELEASEARRAYOBJECTS(pData);
    return sRes;
}

static std::string QuotedPrintableDecode(const std::string& sContent, std::string& sCharset)
{
    NSStringUtils::CStringBuilderA sRes;
    size_t ip = 0;
    size_t i  = sContent.find('=');

    if(i == 0)
    {
        size_t nIgnore = 12;
        std::string charset = sContent.substr(0, nIgnore);
        if(charset == "=00=00=FE=FF")
            sCharset = "UTF-32BE";
        else if(charset == "=FF=FE=00=00")
            sCharset = "UTF-32LE";
        else if(charset == "=2B=2F=76=38" || charset == "=2B=2F=76=39" ||
                charset == "=2B=2F=76=2B" || charset == "=2B=2F=76=2F")
            sCharset = "UTF-7";
        else if(charset == "=DD=73=66=73")
            sCharset = "UTF-EBCDIC";
        else if(charset == "=84=31=95=33")
            sCharset = "GB-18030";
        else
        {
            nIgnore -= 3;
            charset.erase(nIgnore);
            if(charset == "=EF=BB=BF")
                sCharset = "UTF-8";
            else if(charset == "=F7=64=4C")
                sCharset = "UTF-1";
            else if(charset == "=0E=FE=FF")
                sCharset = "SCSU";
            else if(charset == "=FB=EE=28")
                sCharset = "BOCU-1";
            else
            {
                nIgnore -= 3;
                charset.erase(nIgnore);
                if(charset == "=FE=FF")
                    sCharset = "UTF-16BE";
                else if(charset == "=FF=FE")
                    sCharset = "UTF-16LE";
                else
                    nIgnore -= 6;
            }
        }

        ip = nIgnore;
        i  = sContent.find('=', ip);
    }

    while(i != std::string::npos && i + 2 < sContent.length())
    {
        sRes.WriteString(sContent.c_str() + ip, i - ip);
        std::string str = sContent.substr(i + 1, 2);
        if(str.front() == '\n' || str.front() == '\r')
        {
            char ch = str[1];
            if(ch != '\n' && ch != '\r')
                sRes.WriteString(&ch, 1);
        }
        else
        {
            char* err;
            char ch = (int)strtol(str.data(), &err, 16);
            if(*err)
                sRes.WriteString('=' + str);
            else
                sRes.WriteString(&ch, 1);
        }
        ip = i + 3;
        i = sContent.find('=', ip);
    }
    if(ip != std::string::npos)
        sRes.WriteString(sContent.c_str() + ip);
    return sRes.GetData();
}

static void ReadMht(std::string& sFileContent, size_t& nFound, size_t& nNextFound, const std::string& sBoundary,
                    std::map<std::string, std::string>& sRes, NSStringUtils::CStringBuilderA& oRes)
{
    // Content
    size_t nContentTag = sFileContent.find("\n\n", nFound);
    if(nContentTag == std::string::npos || nContentTag > nNextFound)
    {
        nContentTag = sFileContent.find("\r\r", nFound);
        if(nContentTag == std::string::npos || nContentTag > nNextFound)
        {
            nContentTag = sFileContent.find("\r\n\r\n", nFound);
            if(nContentTag == std::string::npos || nContentTag > nNextFound)
            {
                nFound = nNextFound;
                return;
            }
            else
                nContentTag += 4;
        }
        else
            nContentTag += 2;
    }
    else
        nContentTag += 2;

    // Content-Type
    size_t nTag = sFileContent.find("Content-Type: ", nFound);
    if(nTag == std::string::npos || nTag > nContentTag)
    {
        nFound = nNextFound;
        return;
    }
    size_t nTagEnd = sFileContent.find_first_of(";\n\r", nTag);
    nTag += 14;
    if(nTagEnd == std::string::npos || nTagEnd > nContentTag)
    {
        nFound = nNextFound;
        return;
    }
    std::string sContentType = sFileContent.substr(nTag, nTagEnd - nTag);
    if(sContentType == "multipart/alternative")
        nContentTag = nFound;

    // name
    std::string sName;
    nTag = sFileContent.find(" name=", nFound);
    if(nTag != std::string::npos && nTag < nContentTag)
    {
        nTagEnd = sFileContent.find_first_of(";\n\r", nTag);
        nTag += 6;
        if(nTagEnd != std::string::npos && nTagEnd < nContentTag)
            sName = sFileContent.substr(nTag, nTagEnd - nTag);
    }

    // charset
    std::string sCharset;
    nTag = sFileContent.find("charset=", nFound);
    if(nTag != std::string::npos && nTag < nContentTag)
    {
        nTagEnd = sFileContent.find_first_of(";\n\r", nTag);
        nTag += 8;
        if(nTagEnd != std::string::npos && nTagEnd < nContentTag)
        {
            if(sFileContent[nTag] == '\"')
            {
                nTag++;
                nTagEnd--;
            }
            sCharset = sFileContent.substr(nTag, nTagEnd - nTag);
        }
    }

    // Content-Location
    std::string sContentLocation;
    nTag = sFileContent.find("Content-Location: ", nFound);
    if(nTag != std::string::npos && nTag < nContentTag)
    {
        nTagEnd = sFileContent.find_first_of(";\n\r", nTag);
        nTag += 18;
        if(nTagEnd != std::string::npos && nTagEnd < nContentTag)
            sContentLocation = sFileContent.substr(nTag, nTagEnd - nTag);
    }

    // Content-Transfer-Encoding
    std::string sContentEncoding;
    nTag = sFileContent.find("Content-Transfer-Encoding: ", nFound);
    if(nTag != std::string::npos && nTag < nContentTag)
    {
        nTagEnd = sFileContent.find_first_of(";\n\r", nTag);
        nTag += 27;
        if(nTagEnd != std::string::npos && nTagEnd < nContentTag)
            sContentEncoding = sFileContent.substr(nTag, nTagEnd - nTag);
    }

    // Content
    nTagEnd = nNextFound - 2;
    if(nTagEnd == std::string::npos || nTagEnd < nContentTag)
    {
        nFound = nNextFound;
        return;
    }
    std::string sContent = sFileContent.substr(nContentTag, nTagEnd - nContentTag);

    // Удаляем лишнее
    sFileContent.erase(0, nNextFound);
    nFound = sFileContent.find(sBoundary);

    std::wstring sExtention = NSFile::GetFileExtention(UTF8_TO_U(sName));
    std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
    // Основной документ
    if(sContentType == "multipart/alternative")
        oRes.WriteString(mhtTohtml(sContent));
    else if((sContentType.find("text") != std::string::npos && (sExtention.empty() || sExtention == L"htm" || sExtention == L"html" || sExtention
            == L"xhtml" || sExtention == L"css")) || (sContentType == "application/octet-stream" && (sContentLocation.find("css") !=
            std::string::npos)))
    {
        // Стили заключаются в тэг <style>
        if(sContentType == "text/css" || sExtention == L"css" || sContentLocation.find("css") != std::string::npos)
            oRes.WriteString("<style>");
        if(sContentEncoding == "Base64" || sContentEncoding == "base64")
            oRes.WriteString(Base64ToString(sContent, sCharset));
        else if(sContentEncoding == "8bit" || sContentEncoding == "7bit" || sContentEncoding.empty())
        {
            if (sCharset != "utf-8" && sCharset != "UTF-8" && !sCharset.empty())
            {
                NSUnicodeConverter::CUnicodeConverter oConverter;
                sContent = U_TO_UTF8(oConverter.toUnicode(sContent, sCharset.data()));
            }
            oRes.WriteString(sContent);
        }
        else if(sContentEncoding == "quoted-printable" || sContentEncoding == "Quoted-Printable")
        {
            sContent = QuotedPrintableDecode(sContent, sCharset);
            if (sCharset != "utf-8" && sCharset != "UTF-8" && !sCharset.empty())
            {
                NSUnicodeConverter::CUnicodeConverter oConverter;
                sContent = U_TO_UTF8(oConverter.toUnicode(sContent, sCharset.data()));
            }
            oRes.WriteString(sContent);
        }
        if(sContentType == "text/css" || sExtention == L"css" || sContentLocation.find("css") != std::string::npos)
            oRes.WriteString("</style>");
    }
    // Картинки
    else if((sContentType.find("image") != std::string::npos || sExtention == L"gif" || sContentType == "application/octet-stream") &&
            (sContentEncoding == "Base64" || sContentEncoding == "base64"))
    {
        if(sExtention == L"ico" || sContentType.find("ico") != std::string::npos)
            sContentType = "image/jpg";
        else if(sExtention == L"gif")
            sContentType = "image/gif";
        int nSrcLen = (int)sContent.length();
        int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
        BYTE* pData = new BYTE[nDecodeLen];
        if (TRUE == NSBase64::Base64Decode(sContent.c_str(), nSrcLen, pData, &nDecodeLen))
            sRes.insert(std::make_pair(sContentLocation, "data:" + sContentType + ";base64," + sContent));
        RELEASEARRAYOBJECTS(pData);
    }
}

static std::string mhtTohtml(std::string& sFileContent)
{
    std::map<std::string, std::string> sRes;
    NSStringUtils::CStringBuilderA oRes;

    // Поиск boundary
    size_t nFound = sFileContent.find("boundary=");
    if(nFound == std::string::npos)
    {
        size_t nFoundEnd = sFileContent.length();
        nFound = 0;
        ReadMht(sFileContent, nFound, nFoundEnd, "no", sRes, oRes);
        return oRes.GetData();
    }
    size_t nFoundEnd = sFileContent.find_first_of(";\n\r", nFound);
    if(nFoundEnd == std::string::npos)
        return "";
    nFound += 9;
    if(sFileContent[nFound] == '\"')
    {
        nFound++;
        nFoundEnd--;
    }
    if(nFound > nFoundEnd)
        return "";
    std::string sBoundary = sFileContent.substr(nFound, nFoundEnd - nFound);
    size_t nBoundaryLength = sBoundary.length();

    // Удаляем лишнее
    nFound = sFileContent.find(sBoundary, nFoundEnd);
    sFileContent.erase(0, nFound);

    // Цикл по boundary
    nFound = 0;
    while(nFound != std::string::npos)
    {
        // Выход по --boundary--
        if(sFileContent[nFound + nBoundaryLength + 1] == '-')
            break;
        nFoundEnd = sFileContent.find(sBoundary, nFound + nBoundaryLength);
        if(nFoundEnd == std::string::npos)
            break;
        ReadMht(sFileContent, nFound, nFoundEnd, sBoundary, sRes, oRes);
    }
    std::string sFile = oRes.GetData();
    for(const std::pair<std::string, std::string>& item : sRes)
    {
        std::string sName = item.first;
        size_t found = sFile.find(sName);
        size_t sfound = sName.rfind('/');
        if(found == std::string::npos && sfound != std::string::npos)
            found = sFile.find(sName.erase(0, sfound + 1));
        while(found != std::string::npos)
        {
            size_t fq = sFile.find_last_of("\"\'>=", found);
            char ch = sFile[fq];
            if(ch != '\"' && ch != '\'')
                fq++;
            size_t tq = sFile.find_first_of("\"\'<> ", found) + 1;
            if(sFile[tq] != '\"' && sFile[tq] != '\'')
                tq--;
            if(ch != '>')
            {
                std::string is = '\"' + item.second + '\"';
                sFile.replace(fq, tq - fq, is);
                found = sFile.find(sName, fq + is.length());
            }
            else
                found = sFile.find(sName, tq);
        }
    }
    return sFile;
}

static std::wstring mhtToXhtml(std::string& sFileContent)
{
    sFileContent = mhtTohtml(sFileContent);

    // Gumbo
    GumboOptions options = kGumboDefaultOptions;
    GumboOutput* output = gumbo_parse_with_options(&options, sFileContent.data(), sFileContent.length());

    // prettyprint
    NSStringUtils::CStringBuilderA oBuilder;
    prettyprint(output->document, oBuilder);

    // Конвертирование из string utf8 в wstring
    return UTF8_TO_U(oBuilder.GetData());
}

// Заменяет сущности &,<,> в text
static void substitute_xml_entities_into_text(std::string& text)
{
    // replacing & must come first
    replace_all(text, "&", "&amp;");
    replace_all(text, "<", "&lt;");
    replace_all(text, ">", "&gt;");
}

// Заменяет сущности " в text
static void substitute_xml_entities_into_attributes(std::string& text)
{
    substitute_xml_entities_into_text(text);
    replace_all(text, "\"", "&quot;");
}

static std::string handle_unknown_tag(GumboStringPiece* text)
{
    if (text->data == NULL)
        return "";
    GumboStringPiece gsp = *text;
    gumbo_tag_from_original_text(&gsp);
    std::string sAtr = std::string(gsp.data, gsp.length);
    size_t found = sAtr.find_first_of("-'+,./=?;!*#@$_%<>&;\"\'()[]{}");
    while(found != std::string::npos)
    {
        sAtr.erase(found, 1);
        found = sAtr.find_first_of("-'+,./=?;!*#@$_%<>&;\"\'()[]{}", found);
    }
    return sAtr;
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

static void build_attributes(const GumboVector* attribs, bool no_entities, NSStringUtils::CStringBuilderA& atts)
{
    std::vector<std::string> arrRepeat;
    for (size_t i = 0; i < attribs->length; ++i)
    {
        GumboAttribute* at = static_cast<GumboAttribute*>(attribs->data[i]);
        std::string sVal(at->value);
        std::string sName(at->name);
        atts.WriteString(" ");

        bool bCheck = false;
        size_t nBad = sName.find_first_of("+,.=?#%<>&;\"\'()[]{}");
        while(nBad != std::string::npos)
        {
            sName.erase(nBad, 1);
            nBad = sName.find_first_of("+,.=?#%<>&;\"\'()[]{}", nBad);
            if(sName.empty())
                break;
            bCheck = true;
        }
        if(sName.empty())
            continue;
        while(sName.front() >= '0' && sName.front() <= '9')
        {
            sName.erase(0, 1);
            if(sName.empty())
                break;
            bCheck = true;
        }
        if(bCheck)
        {
            GumboAttribute* check = gumbo_get_attribute(attribs, sName.c_str());
            if(check || std::find(arrRepeat.begin(), arrRepeat.end(), sName) != arrRepeat.end())
                continue;
            else
                arrRepeat.push_back(sName);
        }

        if(sName.empty())
            continue;
        atts.WriteString(sName);

        // determine original quote character used if it exists
        std::string qs ="\"";
        atts.WriteString("=");
        atts.WriteString(qs);
        if(!no_entities)
            substitute_xml_entities_into_attributes(sVal);
        atts.WriteString(sVal);
        atts.WriteString(qs);
    }
}

static void prettyprint_contents(GumboNode* node, NSStringUtils::CStringBuilderA& contents)
{
    std::string key             = "|" + get_tag_name(node) + "|";
    bool no_entity_substitution = no_entity_sub.find(key) != std::string::npos;
    bool keep_whitespace        = preserve_whitespace.find(key) != std::string::npos;
    bool is_inline              = nonbreaking_inline.find(key) != std::string::npos;
    bool is_like_inline         = treat_like_inline.find(key) != std::string::npos;

    GumboVector* children = &node->v.element.children;

    for (size_t i = 0; i < children->length; i++)
    {
        GumboNode* child = static_cast<GumboNode*> (children->data[i]);

        if (child->type == GUMBO_NODE_TEXT)
        {
            std::string val(child->v.text.text);
            if(!no_entity_substitution)
                substitute_xml_entities_into_text(val);

            // Избавление от FF
            size_t found = val.find_first_of("\014");
            while(found != std::string::npos)
            {
                val.erase(found, 1);
                found = val.find_first_of("\014", found);
            }

            contents.WriteString(val);
        }
        else if ((child->type == GUMBO_NODE_ELEMENT) || (child->type == GUMBO_NODE_TEMPLATE))
            prettyprint(child, contents);
        else if (child->type == GUMBO_NODE_WHITESPACE)
        {
            if (keep_whitespace || is_inline || is_like_inline)
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
    const GumboVector* attribs = &node->v.element.attributes;
    build_attributes(attribs, no_entity_substitution, oBuilder);
    oBuilder.WriteString(close + ">");

    // prettyprint your contents
    prettyprint_contents(node, oBuilder);
    oBuilder.WriteString(closeTag);
}

#endif // HTMLTOXHTML_H
