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

#include "CONDFMT.h"
#include <Logic/Biff_records/CondFmt.h>
#include <Logic/Biff_records/CF.h>

namespace XLS
{


CONDFMT::CONDFMT()
{
}


CONDFMT::~CONDFMT()
{
}


BaseObjectPtr CONDFMT::clone()
{
	return BaseObjectPtr(new CONDFMT(*this));
}


// CONDFMT = CondFmt 1*3CF
const bool CONDFMT::loadContent(BinProcessor& proc)
{
	CondFmt cond_fmt;
	if(!proc.mandatory(cond_fmt))
	{
		return false;
	}
	m_CondFmt = elements_.back();
	elements_.pop_back();
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt>())
	{
		return false;
	}
	*/
	{
		CF cf1(cond_fmt.getLocation());
		if (proc.optional(cf1))
		{
			m_arCF.push_back( elements_.back());
			elements_.pop_back();
		}

		CF cf2(cond_fmt.getLocation());
		if(proc.optional(cf2))
		{
			m_arCF.push_back( elements_.back());
			elements_.pop_back();

			CF cf3(cond_fmt.getLocation());
			if (proc.optional(cf3))
			{
				m_arCF.push_back( elements_.back());
				elements_.pop_back();
			}
		}
	}
	/*  The above is equal to:
		proc.repeated<CF>(1, 3);
	*/
	return true;
}
int CONDFMT::serialize(std::wostream & stream)
{
	if (!m_CondFmt)			return 0;
	if (m_arCF.size() < 1)	return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"conditionalFormatting")
		{
			CondFmt * condFmt	= dynamic_cast<CondFmt*>(m_CondFmt.get());

			CP_XML_ATTR(L"sqref", condFmt->sqref.sqref);
			//condition_id
			//condition_pos
			for (int i = 0; i < m_arCF.size(); i++)
			{
				if (m_arCF[i] == NULL) continue;

				m_arCF[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}
} // namespace XLS

