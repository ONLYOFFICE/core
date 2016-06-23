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

#include "FEAT11.h"
#include <Logic/Biff_records/FeatHdr11.h>
#include <Logic/Biff_records/Feature11.h>
#include <Logic/Biff_records/Feature12.h>
#include <Logic/Biff_records/ContinueFrt11.h>
#include <Logic/Biff_records/List12.h>
#include <Logic/Biff_records/AutoFilter12.h>
#include <Logic/Biff_records/ContinueFrt12.h>
#include <Logic/Biff_records/List12.h>
#include <Logic/Biff_unions/SORTDATA12.h>

namespace XLS
{


FEAT11::FEAT11()
{
}


FEAT11::~FEAT11()
{
}



class Parenthesis_FEAT11_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FEAT11_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FEAT11_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<Feature11>())
		{
			if(!proc.optional<Feature12>())
			{
				return false;
			}
		}
		int count = 0;
		count = proc.repeated<ContinueFrt11>(0, 0);
		count = proc.repeated<List12>(0, 0);
		
		while (proc.optional<AutoFilter12>())
		{
			count = proc.repeated<ContinueFrt12>(0, 0);
		}		
		
		count = proc.repeated<List12>(0, 0);
		proc.optional<SORTDATA12>();
		return true;
	};
};


BaseObjectPtr FEAT11::clone()
{
	return BaseObjectPtr(new FEAT11(*this));
}


/*
FEAT11 = FeatHdr11 *((Feature11 / Feature12) *ContinueFrt11 *List12 [AutoFilter12 *ContinueFrt12] *List12 [SORTDATA12])
*/
const bool FEAT11::loadContent(BinProcessor& proc)
{
	if (!proc.mandatory<FeatHdr11>())
	{
		return false;
	}
	m_FeatHdr11 = elements_.back();
	elements_.pop_back();
	
	int count = proc.repeated<Parenthesis_FEAT11_1>(0, 0);

	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeFeature11 || 
			elements_.front()->get_type() == typeFeature12 )
		{
			_data new_data;
			new_data.m_Feature = elements_.front();

			m_arFEAT.push_back(new_data);
		}

		if (elements_.front()->get_type() == typeList12)
		{
			if (m_arFEAT.back().m_AutoFilter12)
				m_arFEAT.back().m_arList12_second.push_back(elements_.front());
			else
				m_arFEAT.back().m_arList12.push_back(elements_.front());
		}
		if (elements_.front()->get_type() == typeAutoFilter12)
		{
			m_arFEAT.back().m_AutoFilter12 = elements_.front();
		}
		if (elements_.front()->get_type() == typeSORTDATA12)
		{
			m_arFEAT.back().m_SORTDATA12 = elements_.front();
		}
		elements_.pop_front();
	}
	return true;
}

} // namespace XLS

