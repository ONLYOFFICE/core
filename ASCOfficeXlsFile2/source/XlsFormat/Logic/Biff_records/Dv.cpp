/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "Dv.h"

#include <utils.h>
#include <boost/algorithm/string.hpp>

namespace XLS
{
static std::wstring replace_zero (const std::wstring &str, const std::wstring &delimetr)
{
	if (str.empty()) return L"";

	std::wstring out;
	int pos = 0;
	while(true)
	{
		if (pos >= str.size()) break;

		if (str[pos] == '\0')
		{
			out += delimetr;
		}
		else
		{
			out += str[pos];
		}
		pos++;
	}
	return out;
}

BaseObjectPtr Dv::clone()
{
	return BaseObjectPtr(new Dv(*this));
}

void Dv::readFields(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;

	valType		= static_cast<_valTypeDv>(GETBITS(flags, 0, 3));
	errStyle	= static_cast<unsigned char>(GETBITS(flags, 4, 6));
	
	fStrLookup		= GETBIT(flags, 7);
	fAllowBlank		= GETBIT(flags, 8);
	fSuppressCombo	= GETBIT(flags, 9);
	mdImeMode		= static_cast<unsigned char>(GETBITS(flags, 10, 17));
	fShowInputMsg	= GETBIT(flags, 18);
	fShowErrorMsg	= GETBIT(flags, 19);
	typOperator		= static_cast<_typOperatorDv>(GETBITS(flags, 20, 23));

	record >> PromptTitle >> ErrorTitle >> Prompt >> Error;
	
	formula1.load(record);
	formula2.load(record);
	
	record >> sqref;
}

int Dv::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dataValidation")
		{
			CP_XML_ATTR(L"sqref",				sqref.strValue);
			CP_XML_ATTR(L"showErrorMessage",	fShowErrorMsg);
			CP_XML_ATTR(L"showInputMessage",	fShowInputMsg);
			CP_XML_ATTR(L"allowBlank",			fAllowBlank);

			switch(typOperator)
			{
				case operatorDvBetween:				CP_XML_ATTR(L"operator", L"between");			break;
				case operatorDvNotBetween:			CP_XML_ATTR(L"operator", L"notBetween");		break;
				case operatorDvEquals:				CP_XML_ATTR(L"operator", L"equal");				break;
				case operatorDvNotEquals:			CP_XML_ATTR(L"operator", L"notEqual");			break;
				case operatorDvGreaterThan:			CP_XML_ATTR(L"operator", L"greaterThan");		break;
				case operatorDvLessThan:			CP_XML_ATTR(L"operator", L"lessThan");			break;
				case operatorDvGreaterThanOrEqual:	CP_XML_ATTR(L"operator", L"greaterThanOrEqual");break;
				case operatorDvLessThanOrEqual:		CP_XML_ATTR(L"operator", L"lessThanOrEqual");	break;
			}
			switch(valType)
			{
				case typeDvCustom:		CP_XML_ATTR(L"type", L"custom");	break;
				case typeDvDate:		CP_XML_ATTR(L"type", L"date");		break;
				case typeDvDecimal:		CP_XML_ATTR(L"type", L"decimal");	break;
				case typeDvList:		CP_XML_ATTR(L"type", L"list");		break;
				case typeDvNone:		CP_XML_ATTR(L"type", L"none");		break;
				case typeDvTextLength:	CP_XML_ATTR(L"type", L"textLength");break;
				case typeDvTime:		CP_XML_ATTR(L"type", L"time");		break;
				case typeDvWhole:		CP_XML_ATTR(L"type", L"whole");		break;
			}
			if (!PromptTitle.value().empty())
			{
				CP_XML_ATTR(L"promtTitle", PromptTitle.value());
			}
			if (!Prompt.value().empty())
			{
				CP_XML_ATTR(L"promt", Prompt.value());
			}
			if (!ErrorTitle.value().empty())
			{
				CP_XML_ATTR(L"errorTitle", ErrorTitle.value());
			}
			if (!Error.value().empty())
			{
				CP_XML_ATTR(L"error", Error.value());
			}
			std::wstring sFormula1 = replace_zero(formula1.getAssembledFormula(), L",");
			std::wstring sFormula2 = replace_zero(formula2.getAssembledFormula(), L",");

			if (!sFormula1.empty())
			{
				//boost::algorithm::replace_all(sFormula1 , "\0", L",");
				CP_XML_NODE(L"formula1") {CP_XML_STREAM() << xml::utils::replace_text_to_xml(sFormula1);}
			}
			if (!sFormula2.empty())
			{
				//boost::algorithm::replace_all(sFormula2 , "\0", L",");
				CP_XML_NODE(L"formula2") {CP_XML_STREAM() << xml::utils::replace_text_to_xml(sFormula2);}
			}
		}
	}

	return 0;
}
} // namespace XLS

