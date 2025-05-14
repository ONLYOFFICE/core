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
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../UnicodeConverter/UnicodeConverter.h"
#include "../../../HtmlFile2/src/StringFinder.h"

static std::string nonbreaking_inline  = "|a|abbr|acronym|b|bdo|big|cite|code|dfn|em|font|i|img|kbd|nobr|s|small|span|strike|strong|sub|sup|tt|";
static std::string empty_tags          = "|area|base|basefont|bgsound|br|command|col|embed|event-source|frame|hr|image|img|input|keygen|link|menuitem|meta|param|source|spacer|track|wbr|";
static std::string preserve_whitespace = "|pre|textarea|script|style|";
static std::string special_handling    = "|html|body|";
static std::string no_entity_sub       = ""; //"|style|";
static std::string treat_like_inline   = "|p|";

static std::vector<std::string> html_tags = {"div","span","a","img","p","h1","h2","h3","h4","h5","h6",
                                             "ul", "ol", "li","td","tr","table","thead","tbody","tfoot","th",
                                             "br","form","input","button","section","nav","header","footer",
                                             "main","figure","figcaption","strong","em","i", "b", "u","pre",
                                             "code","blockquote","hr","script","link","meta","style","title",
                                             "head","body","html","legend","optgroup","option","select","dl",
                                             "dt","dd","time","data","abbr","address","area","base","bdi",
                                             "bdo","cite","col","iframe","video","source","track","textarea",
                                             "label","fieldset","colgroup","del","ins","details","summary",
                                             "dialog","embed","kbd","map","mark","menu","meter","object",
                                             "output","param","progress","q","samp","small","sub","sup","var",
                                             "wbr","acronym","applet","article","aside","audio","basefont",
                                             "bgsound","big","blink","canvas","caption","center","command",
                                             "comment","datalist","dfn","dir","font","frame","frameset",
                                             "hgroup","isindex","keygen","marquee","nobr","noembed","noframes",
                                             "noscript","plaintext","rp","rt","ruby","s","strike","tt","xmp"};

static std::vector<std::string> unchecked_nodes_new = {"svg"};

static void prettyprint(GumboNode*, NSStringUtils::CStringBuilderA& oBuilder, bool bCheckValidNode = true);
static std::string mhtTohtml(const std::string &sFileContent);

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

static bool NodeIsUnprocessed(const std::string& wsTagName)
{
	return "xml" == wsTagName;
}

static bool IsUnckeckedNodes(const std::string& sValue)
{
	return unchecked_nodes_new.end() != std::find(unchecked_nodes_new.begin(), unchecked_nodes_new.end(), sValue);
}

static std::wstring htmlToXhtml(std::string& sFileContent, bool bNeedConvert)
{
	if (bNeedConvert)
	{ // Определение кодировки
		std::string sEncoding = NSStringFinder::FindProperty(sFileContent, "charset", {"="}, {";", "\\n", "\\r", " ", "\""}).m_sValue;

		if (sEncoding.empty())
			sEncoding = NSStringFinder::FindProperty(sFileContent, "encoding", {"="}, {";", "\\n", "\\r", " "}).m_sValue;

		if (!sEncoding.empty() && !NSStringFinder::Equals("utf-8", sEncoding))
		{
			NSUnicodeConverter::CUnicodeConverter oConverter;
			sFileContent = U_TO_UTF8(oConverter.toUnicode(sFileContent, sEncoding.c_str()));
		}
	}

	// Избавляемся от лишних символов до <...
	boost::regex oRegex("<[a-zA-Z]");
	boost::match_results<typename std::string::const_iterator> oResult;

	if (boost::regex_search(sFileContent, oResult, oRegex))
		sFileContent.erase(0, oResult.position());

	//Избавление от <a ... />
	while (NSStringFinder::RemoveEmptyTag(sFileContent, "a"));
	//Избавление от <title ... />
	while (NSStringFinder::RemoveEmptyTag(sFileContent, "title"));
	//Избавление от <script ... />
	while (NSStringFinder::RemoveEmptyTag(sFileContent, "script"));

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
		if(!sCharset.empty() && NSStringFinder::Equals<std::string>("utf-8", sCharset))
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

static void ReadMht(const std::string& sMhtContent, std::map<std::string, std::string>& sRes, NSStringUtils::CStringBuilderA& oRes)
{
	size_t unContentPosition = 0, unCharsetBegin = 0, unCharsetEnd = std::string::npos;

	NSStringFinder::TFoundedData<char> oData;
	
	// Content-Type
	oData =  NSStringFinder::FindProperty(sMhtContent, "content-type", {":"}, {";", "\\n", "\\r"});
	const std::string sContentType{oData.m_sValue};

	if (sContentType.empty())
		return;
	
	if (NSStringFinder::Equals(sContentType, "multipart/alternative"))
	{
		oRes.WriteString(mhtTohtml(sMhtContent.substr(oData.m_unEndPosition, sMhtContent.length() - oData.m_unEndPosition)));
		return;
	}

	unContentPosition = std::max(unContentPosition, oData.m_unEndPosition);
	unCharsetBegin = oData.m_unEndPosition;

	// name
//	std::string sName = NSStringFinder::FindProperty(sMhtContent, "name", {"="}, {";", "\\n", "\\r"}, 0, unLastPosition);
//	unContentPosition = std::max(unContentPosition, unLastPosition);

	// Content-Location
	oData = NSStringFinder::FindProperty(sMhtContent, "content-location", {":"}, {";", "\\n", "\\r"});
	std::string sContentLocation{oData.m_sValue};

	if (!oData.Empty())
		unContentPosition = std::max(unContentPosition, oData.m_unEndPosition);

	// Content-ID
	oData = NSStringFinder::FindProperty(sMhtContent, "content-id", {":"}, {";", "\\n", "\\r"});
	std::string sContentID{oData.m_sValue};

	if (!oData.Empty())
	{
		unContentPosition = std::max(unContentPosition, oData.m_unEndPosition);
		unCharsetEnd = std::min(unCharsetEnd, oData.m_unBeginPosition);
		NSStringFinder::CutInside<std::string>(sContentID, "<", ">");
	}

	if (sContentLocation.empty() && !sContentID.empty())
		sContentLocation = "cid:" + sContentID;

	// Content-Transfer-Encoding
	oData = NSStringFinder::FindProperty(sMhtContent, "content-transfer-encoding", {":"}, {";", "\\n", "\\r"});
	const std::string sContentEncoding{oData.m_sValue};

	if (!oData.Empty())
	{
		unContentPosition = std::max(unContentPosition, oData.m_unEndPosition);
		unCharsetEnd = std::min(unCharsetEnd, oData.m_unBeginPosition);
	}

	// charset
	std::string sCharset = "utf-8";

	if (std::string::npos != unCharsetEnd && unCharsetBegin < unCharsetEnd)
	{
		sCharset = NSStringFinder::FindProperty(sMhtContent.substr(unCharsetBegin, unCharsetEnd - unCharsetBegin), "charset", {"="}, {";", "\\n", "\\r"}).m_sValue;
		NSStringFinder::CutInside<std::string>(sCharset, "\"");
	}

	// Content
	std::string sContent = sMhtContent.substr(unContentPosition, sMhtContent.length() - unContentPosition);

//	std::wstring sExtention = NSFile::GetFileExtention(UTF8_TO_U(sName));
//	std::transform(sExtention.begin(), sExtention.end(), sExtention.begin(), tolower);
	// Основной документ
	if (NSStringFinder::Equals(sContentType, "multipart/alternative"))
		oRes.WriteString(mhtTohtml(sContent));
	else if ((NSStringFinder::Find(sContentType, "text") /*&& (sExtention.empty() || NSStringFinder::EqualOf(sExtention, {L"htm", L"html", L"xhtml", L"css"}))*/) 
	          || (NSStringFinder::Equals(sContentType, "application/octet-stream") && NSStringFinder::Find(sContentLocation, "css")))
	{
		// Стили заключаются в тэг <style>
		const bool bAddTagStyle = NSStringFinder::Equals(sContentType, "text/css") /*|| NSStringFinder::Equals(sExtention, L"css")*/ || NSStringFinder::Find(sContentLocation, "css");

		if (bAddTagStyle)
			oRes.WriteString("<style>");

		if (NSStringFinder::Equals(sContentEncoding, "base64"))
			sContent = Base64ToString(sContent, sCharset);
		else if (NSStringFinder::EqualOf(sContentEncoding, {"8bit", "7bit"}) || sContentEncoding.empty())
		{
			if (!NSStringFinder::Equals(sCharset, "utf-8") && !sCharset.empty())
			{
				NSUnicodeConverter::CUnicodeConverter oConverter;
				sContent = U_TO_UTF8(oConverter.toUnicode(sContent, sCharset.data()));
			}
		}
		else if (NSStringFinder::Equals(sContentEncoding, "quoted-printable"))
		{
			sContent = QuotedPrintableDecode(sContent, sCharset);
			if (!NSStringFinder::Equals(sCharset, "utf-8") && !sCharset.empty())
			{
				NSUnicodeConverter::CUnicodeConverter oConverter;
				sContent = U_TO_UTF8(oConverter.toUnicode(sContent, sCharset.data()));
			}
		}

		if (NSStringFinder::Equals(sContentType, "text/html"))
			sContent = U_TO_UTF8(htmlToXhtml(sContent, false));

		oRes.WriteString(sContent);

		if(bAddTagStyle)
			oRes.WriteString("</style>");
	}
	// Картинки
	else if ((NSStringFinder::Find(sContentType, "image") /*|| NSStringFinder::Equals(sExtention, L"gif")*/ || NSStringFinder::Equals(sContentType, "application/octet-stream")) && 
			  NSStringFinder::Equals(sContentEncoding, "base64"))
	{
//		if (NSStringFinder::Equals(sExtention, L"ico") || NSStringFinder::Find(sContentType, "ico"))
//			sContentType = "image/jpg";
//		else if(NSStringFinder::Equals(sExtention, L"gif"))
//			sContentType = "image/gif";
		int nSrcLen = (int)sContent.length();
		int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
		BYTE* pData = new BYTE[nDecodeLen];
		if (TRUE == NSBase64::Base64Decode(sContent.c_str(), nSrcLen, pData, &nDecodeLen))
			sRes.insert(std::make_pair(sContentLocation, "data:" + sContentType + ";base64," + sContent));
		RELEASEARRAYOBJECTS(pData);
	}
}

static std::string mhtTohtml(const std::string& sFileContent)
{
	std::map<std::string, std::string> sRes;
	NSStringUtils::CStringBuilderA oRes;

	// Поиск boundary
	NSStringFinder::TFoundedData<char> oData{NSStringFinder::FindProperty(sFileContent, "boundary", {"="}, {"\\r", "\\n", "\""})};

	size_t nFound{oData.m_unEndPosition};
	std::string sBoundary{oData.m_sValue};

	if (sBoundary.empty())
	{
		size_t nFoundEnd = sFileContent.length();
		nFound = 0;
		ReadMht(sFileContent.substr(nFound, nFoundEnd), sRes, oRes);
		return oRes.GetData();
	}

	NSStringFinder::CutInside<std::string>(sBoundary, "\"");

	size_t nFoundEnd{nFound};

	sBoundary = "--" + sBoundary;
	size_t nBoundaryLength = sBoundary.length();

	nFound = sFileContent.find(sBoundary, nFound) + nBoundaryLength;

	// Цикл по boundary
	while(nFound != std::string::npos)
	{
		nFoundEnd = sFileContent.find(sBoundary, nFound + nBoundaryLength);
		if(nFoundEnd == std::string::npos)
			break;

		ReadMht(sFileContent.substr(nFound, nFoundEnd - nFound), sRes, oRes);

		nFound = sFileContent.find(sBoundary, nFoundEnd);
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

			if (std::string::npos == fq)
				break;

			char ch = sFile[fq];
			if(ch != '\"' && ch != '\'')
				fq++;
			size_t tq = sFile.find_first_of("\"\'<> ", found) + 1;

			if (std::string::npos == tq)
				break;

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

// Удаляем символы, которые ломают работу XmlUtils::CXmlLiteReader
static void remove_control_character(std::string& sText)
{
	sText.erase(std::remove_if(sText.begin(), sText.end(), [](char chValue){ return chValue < 0x09; }));
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
		remove_control_character(pi);
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

		remove_control_character(sVal);
		remove_control_character(sName);

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

static void prettyprint_contents(GumboNode* node, NSStringUtils::CStringBuilderA& contents, bool bCheckValidNode)
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
			remove_control_character(val);

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
			prettyprint(child, contents, bCheckValidNode);
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

static void prettyprint(GumboNode* node, NSStringUtils::CStringBuilderA& oBuilder, bool bCheckValidNode)
{
	// special case the document node
	if (node->type == GUMBO_NODE_DOCUMENT)
	{
		build_doctype(node, oBuilder);
		prettyprint_contents(node, oBuilder, bCheckValidNode);
		return;
	}

	std::string tagname            = get_tag_name(node);
	remove_control_character(tagname);

	if (NodeIsUnprocessed(tagname))
		return;

	if (bCheckValidNode)
		bCheckValidNode = !IsUnckeckedNodes(tagname);

	if (bCheckValidNode && html_tags.end() == std::find(html_tags.begin(), html_tags.end(), tagname))
	{
		prettyprint_contents(node, oBuilder, bCheckValidNode);
		return;
	}

	std::string close              = "";
	std::string closeTag           = "";
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
	prettyprint_contents(node, oBuilder, bCheckValidNode);
	oBuilder.WriteString(closeTag);
}

#endif // HTMLTOXHTML_H
