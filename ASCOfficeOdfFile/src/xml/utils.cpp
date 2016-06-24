/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
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
