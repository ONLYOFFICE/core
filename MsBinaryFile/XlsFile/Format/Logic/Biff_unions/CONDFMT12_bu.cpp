/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "CONDFMT12.h"
#include "../Biff_records/CondFmt12.h"
#include "../Biff_records/CF12.h"

namespace XLS
{


CONDFMT12::CONDFMT12()
{
}


CONDFMT12::~CONDFMT12()
{
}


BaseObjectPtr CONDFMT12::clone()
{
	return BaseObjectPtr(new CONDFMT12(*this));
}


// CONDFMT12 = CondFmt12 1*CF12
const bool CONDFMT12::loadContent(BinProcessor& proc)
{
	CondFmt12 cond_fmt12;
	if(!proc.mandatory(cond_fmt12))
	{
		return false;
	}
	m_CondFmt12 = elements_.back();
	elements_.pop_back();
	/*  The above is equal to:

	if(!proc.mandatory<CondFmt12>())
	{
		return false;
	}
	*/
	{
		CF12 cf12(cond_fmt12.getLocation());
		proc.optional(cf12);

		for(;/*infinity*/;)
		{
			CF12 cf12(cond_fmt12.getLocation());
			if(!proc.optional(cf12))
			{
				break;
			}

		}
	}
	while(!elements_.empty())
	{
		m_arCF12.push_back(elements_.front());
		elements_.pop_front();	
	}
	/*  The above is equal to:
	proc.repeated<CF12>(1, 0);
	*/
	return true;

}
int CONDFMT12::serialize(std::wostream & stream)
{
	if (!m_CondFmt12)			return 0;
	if (m_arCF12.size() < 1)	return 0;
	
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"conditionalFormatting")
		{
			CondFmt12 * condFmt	= dynamic_cast<CondFmt12*>(m_CondFmt12.get());

			CP_XML_ATTR(L"sqref", condFmt->mainCF.sqref.strValue);
			//condition_id
			//condition_pos
			for (size_t i = 0; i < m_arCF12.size(); i++)
			{
				if (m_arCF12[i] == NULL) continue;
				m_arCF12[i]->serialize(CP_XML_STREAM());
			}
		}
	}
	return 0;
}
} // namespace XLS

