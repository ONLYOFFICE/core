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

#include "SERIESDATA.h"
#include <Logic/Biff_records/Dimensions.h>
#include <Logic/Biff_records/SIIndex.h>
#include <Logic/Biff_records/Number.h>
#include <Logic/Biff_records/BoolErr.h>
#include <Logic/Biff_records/Blank.h>
#include <Logic/Biff_records/Label.h>

namespace XLS
{


SERIESDATA::SERIESDATA()
{
}


SERIESDATA::~SERIESDATA()
{
}


//  (Number / BoolErr / Blank / Label)
class Parenthesis_SERIESDATA_2: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESDATA_2)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESDATA_2(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		return	proc.optional<Number>() ||
				proc.optional<BoolErr>() ||
				proc.optional<Blank>() ||
				proc.optional<Label>();
	};
};


//  (SIIndex *(Number / BoolErr / Blank / Label))
class Parenthesis_SERIESDATA_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SERIESDATA_1)
public:
	std::vector<BaseObjectPtr>	m_arSERIESDATA;
	BaseObjectPtr				m_SIIndex;

	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SERIESDATA_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<SIIndex>())
		{
			return false;
		}

		int count = proc.repeated<Parenthesis_SERIESDATA_2>(0, 0);
		return true;
	};
};



BaseObjectPtr SERIESDATA::clone()
{
	return BaseObjectPtr(new SERIESDATA(*this));
}


// SERIESDATA = Dimensions 3(SIIndex *(Number / BoolErr / Blank / Label))
const bool SERIESDATA::loadContent(BinProcessor& proc)
{
	if(!proc.optional<Dimensions>())
	{
		return false;
	}

	if (elements_.size() > 0)
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();

	}

	int count = proc.repeated<Parenthesis_SERIESDATA_1>(3, 3);

	int count1= elements_.size();
	while(count1 > 0)
	{
		if ("SIIndex" == elements_.front()->getClassName())
		{
			m_arSIIndex.push_back(elements_.front()); count--;
		}
		else
		{
			SIIndex * si_in = dynamic_cast<SIIndex *>(m_arSIIndex.back().get());
			if (si_in)
				si_in->m_arData.push_back(elements_.front());
		}
		elements_.pop_front(); count1--;
	}

	return true;
}

} // namespace XLS

