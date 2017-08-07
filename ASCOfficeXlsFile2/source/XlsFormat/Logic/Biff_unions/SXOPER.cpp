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

#include "SXOPER.h"
#include "../Biff_records/SxNil.h"
#include "../Biff_records/SXNum.h"
#include "../Biff_records/SxBool.h"
#include "../Biff_records/SxErr.h"
#include "../Biff_records/SXString.h"
#include "../Biff_records/SXDtr.h"

#include <boost/lexical_cast.hpp>

namespace XLS
{

SXOPER::SXOPER()
{
	bFormula= false;

	bString	= false;
	bDate	= false;
	bNumber	= false;
	bEmpty	= false;
	bInteger= false;
	bBool	= false;
}

SXOPER::~SXOPER()
{
}

BaseObjectPtr SXOPER::clone()
{
	return BaseObjectPtr(new SXOPER(*this));
}

// SXOPER = SxNil / SXNum / SxBool / SxErr / SXString / SXDtr
const bool SXOPER::loadContent(BinProcessor& proc)
{
	if(proc.optional<SxNil>())
	{
		bEmpty = true;
		node = L"m";
	}
	else if(proc.optional<SXNum>())
	{
		SXNum *num = dynamic_cast<SXNum*>(elements_.back().get());
		if (num)
		{
			bInteger = !(num->num.data.value - floor(num->num.data.value) > 0);

			bNumber = !bInteger;
			node	= L"n";
			if (bInteger)
				value	= std::to_wstring((int)num->num.data.value);
			else
				value	= boost::lexical_cast<std::wstring>(num->num.data.value);
		}
	}
	else if(proc.optional<SxBool>())
	{
		SxBool* b = dynamic_cast<SxBool*>(elements_.back().get());	
		bBool	= true;
		node	= L"b";
		value	= std::to_wstring(b->val);
	}
	else if(proc.optional<SxErr>())
	{
		SxErr* err	= dynamic_cast<SxErr*>(elements_.back().get());
		bNumber = true;
		node	= L"e";
		value	= std::to_wstring(err->wbe);
	}
	else if(proc.optional<SXString>())
	{
		SXString* str = dynamic_cast<SXString*>(elements_.back().get());
		bString = true;
		node	= L"s";
		value	= str->value();
	}
	else if(proc.optional<SXDtr>())
	{
		SXDtr* dtr = dynamic_cast<SXDtr*>(elements_.back().get());
		bDate	= true;
		node	= L"d";
		value	= dtr->value();
	}
	else 
		return false;

	m_element = elements_.back();
	elements_.pop_back();


	return true;
}
int SXOPER::serialize(std::wostream & strm)
{
	if (!m_element) return 0;
	if (node.empty()) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(node)
		{ 
			if (!value.empty() || bString)
			{
				CP_XML_ATTR(L"v", value);
			}
			if (bFormula)
			{
				CP_XML_ATTR(L"f", 1);
			}
		}	
	}
	return 0;
}

} // namespace XLS

