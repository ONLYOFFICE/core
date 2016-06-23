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

#include "TABLESTYLES.h"
#include <Logic/Biff_records/TableStyles.h>
#include <Logic/Biff_records/TableStyle.h>
#include <Logic/Biff_records/TableStyleElement.h>

namespace XLS
{


TABLESTYLES::TABLESTYLES()
{
}


TABLESTYLES::~TABLESTYLES()
{
}



class Parenthesis_TABLESTYLES_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_TABLESTYLES_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_TABLESTYLES_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<TableStyle>())
		{
			return false;
		}
		int count = proc.repeated<TableStyleElement>(0, 28);
		return true;
	};
};


BaseObjectPtr TABLESTYLES::clone()
{
	return BaseObjectPtr(new TABLESTYLES(*this));
}


// TABLESTYLES = TableStyles *(TableStyle *28TableStyleElement)

const bool TABLESTYLES::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<TableStyles>())
	{
		return false;
	}
	
	m_TableStyles = elements_.back();
	elements_.pop_back();
		
	int count = proc.repeated<Parenthesis_TABLESTYLES_1>(0, 0);
	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeTableStyle)
		{
			_table_style new_style;
			new_style.style_ = elements_.front();
			m_arTableStyles.push_back(new_style);
		}
		else
		{
			if (m_arTableStyles.size() > 0)
				m_arTableStyles.back().elements_.push_back(elements_.front());
		}
		elements_.pop_back();
	}	
	return true;
}
int TABLESTYLES::serialize(std::wostream & stream)
{
	TableStyles * styles = dynamic_cast<TableStyles*>(m_TableStyles.get());
	if (!styles) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"tableStyles")
		{
			CP_XML_ATTR(L"count", m_arTableStyles.size());
			
			if (!styles->rgchDefPivotStyle.value().empty())
			{
				CP_XML_ATTR(L"defaultPivotStyle", styles->rgchDefPivotStyle.value());
			}
			if (!styles->rgchDefTableStyle.value().empty())
			{
				CP_XML_ATTR(L"defaultTableStyle", styles->rgchDefTableStyle.value());
			}

		}
	}
	return 0;
}
} // namespace XLS

