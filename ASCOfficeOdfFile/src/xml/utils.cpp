#include "../../include/cpdoccore/xml/utils.h"

#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

namespace cpdoccore {
namespace xml {

namespace utils {

namespace {

	#define XML_REPLACE_REGEX_EXPR     "(&)|(')|(<)|(>)|(\")"
	#define XML_REPLACE_FORMAT_EXPR    "(?1&amp;)(?2&apos;)(?3&lt;)(?4&gt;)(?5&quot;)"

	#define XML_REPLACE_REGEX_EXPRW   L"(&)|(')|(<)|(>)|(\")"
	#define XML_REPLACE_FORMAT_EXPRW  L"(?1&amp;)(?2&apos;)(?3&lt;)(?4&gt;)(?5&quot;)"

	#define XML_INV_REPLACE_REGEX_EXPR    "(&amp;)|(&apos;)|(&lt;)|(&gt;)|(&quot;)"
	#define XML_INV_REPLACE_FORMAT_EXPR   "(?1&)(?2')(?3<)(?4>)(?5\")"

	#define XML_INV_REPLACE_REGEX_EXPRW  L"(&amp;)|(&apos;)|(&lt;)|(&gt;)|(&quot;)"
	#define XML_INV_REPLACE_FORMAT_EXPRW L"(?1&)(?2')(?3<)(?4>)(?5\")"

	const boost::wregex replace_text_to_xml_expr_w(XML_REPLACE_REGEX_EXPRW);
	const std::wstring replace_text_to_xml_fmt_w(XML_REPLACE_FORMAT_EXPRW);

	const boost::regex replace_text_to_xml_expr(XML_REPLACE_REGEX_EXPR);
	const std::string replace_text_to_xml_fmt(XML_REPLACE_FORMAT_EXPR);

	const boost::wregex replace_xml_to_text_expr_w(XML_INV_REPLACE_REGEX_EXPRW);
	const std::wstring replace_xml_to_text_fmt_w(XML_INV_REPLACE_FORMAT_EXPRW);

	const boost::regex replace_xml_to_text_expr(XML_INV_REPLACE_REGEX_EXPR);
	const std::string replace_xml_to_text_fmt(XML_INV_REPLACE_FORMAT_EXPR);


	#undef XML_REPLACE_REGEX_EXPR
	#undef XML_REPLACE_FORMAT_EXPR

	#undef XML_REPLACE_REGEX_EXPRW
	#undef XML_REPLACE_FORMAT_EXPRW

}

namespace details {
std::wstring replace_text_to_xml_ImplRegEx(const std::wstring & Text)
{
    return boost::regex_replace(Text, replace_text_to_xml_expr_w, replace_text_to_xml_fmt_w, boost::match_default | boost::format_all);
}

std::string replace_text_to_xml_ImplRegEx(const std::string & Text)
{
    return boost::regex_replace(Text, replace_text_to_xml_expr, replace_text_to_xml_fmt, boost::match_default | boost::format_all);
}

std::wstring replace_xml_to_text_ImplRegEx(const std::wstring & Text)
{
    return boost::regex_replace(Text, replace_xml_to_text_expr_w, replace_xml_to_text_fmt_w, boost::match_default | boost::format_all);

}

std::string replace_xml_to_text_ImplRegEx(const std::string & Text)
{
    return boost::regex_replace(Text, replace_xml_to_text_expr, replace_xml_to_text_fmt, boost::match_default | boost::format_all);
}

std::wstring replace_text_to_xml_ImplReplace(const std::wstring & Text)
{
    std::wstring newStr;
    newStr.resize(Text.size());
    newStr = ::boost::algorithm::replace_all_copy(Text, L"&", L"&amp;");
    ::boost::algorithm::replace_all(newStr, L"'", L"&apos;");
    ::boost::algorithm::replace_all(newStr, L"<", L"&lt;");
    ::boost::algorithm::replace_all(newStr, L">", L"&gt;");
    ::boost::algorithm::replace_all(newStr, L"\"", L"&quot;");
    return newStr;
}

std::string replace_text_to_xml_ImplReplace(const std::string & Text)
{
    std::string newStr;
    newStr.resize(Text.size());
    newStr = ::boost::algorithm::replace_all_copy(Text, "&", "&amp;");
    ::boost::algorithm::replace_all(newStr, "'", "&apos;");
    ::boost::algorithm::replace_all(newStr, "<", "&lt;");
    ::boost::algorithm::replace_all(newStr, ">", "&gt;");
    ::boost::algorithm::replace_all(newStr, "\"", "&quot;");
    return newStr;
}
std::string replace_amp_text_to_xml_ImplReplace(const std::string & Text)
{
    std::string newStr;
    newStr.resize(Text.size());
    newStr = ::boost::algorithm::replace_all_copy(Text, "&", "&amp;");
    ::boost::algorithm::replace_all(newStr, L"&lt;", L"<");
    ::boost::algorithm::replace_all(newStr, L"&gt;", L">");
	
	return newStr;
}
std::wstring replace_amp_text_to_xml_ImplReplace(const std::wstring & Text)
{
    std::wstring newStr;
    newStr.resize(Text.size());
    newStr = ::boost::algorithm::replace_all_copy(Text, "&", "&amp;");
    ::boost::algorithm::replace_all(newStr, L"&lt;", L"<");
    ::boost::algorithm::replace_all(newStr, L"&gt;", L">");
	
	return newStr;
}
std::wstring replace_xml_to_text_ImplReplace(const std::wstring & Text)
{
    std::wstring newStr;
    newStr = ::boost::algorithm::replace_all_copy(Text, L"&amp;", L"&");
    ::boost::algorithm::replace_all(newStr, L"&apos;", L"'");
    ::boost::algorithm::replace_all(newStr, L"&lt;", L"<");
    ::boost::algorithm::replace_all(newStr, L"&gt;", L">");
    ::boost::algorithm::replace_all(newStr, L"&quot;", L"\"");
    return newStr;
}

std::string replace_xml_to_text_ImplReplace(const std::string & Text)
{
    std::string newStr;
    newStr = ::boost::algorithm::replace_all_copy(Text, "&amp;", "&");
    ::boost::algorithm::replace_all(newStr, "&apos;", "'");
    ::boost::algorithm::replace_all(newStr, "&lt;", "<");
    ::boost::algorithm::replace_all(newStr, "&gt;", ">");
    ::boost::algorithm::replace_all(newStr, "&quot;", "\"");
    return newStr;
}

std::wstring replace_lt_gt_ImplReplace(const std::wstring & Text)
{
    std::wstring newStr;
    newStr = ::boost::algorithm::replace_all_copy(Text, L"<", L"&lt;");
    ::boost::algorithm::replace_all(newStr, L">", L"&gt;");
    return newStr;
}


}

std::wstring replace_lt_gt(const std::wstring & Text)
{
    return details::replace_lt_gt_ImplReplace(Text);                
}
std::wstring replace_amp_text_to_xml(const std::wstring & Text)
{
    return details::replace_amp_text_to_xml_ImplReplace(Text);                
}
std::wstring replace_text_to_xml(const std::wstring & Text)
{
    return details::replace_text_to_xml_ImplReplace(Text);                
}

std::string replace_text_to_xml(const std::string & Text)
{
    return details::replace_text_to_xml_ImplReplace(Text);            
}

std::wstring replace_xml_to_text(const std::wstring & Text)
{
    return details::replace_xml_to_text_ImplReplace(Text);    
}

std::string replace_xml_to_text(const std::string & Text)
{
    return details::replace_xml_to_text_ImplReplace(Text);    
}

}
}
}
