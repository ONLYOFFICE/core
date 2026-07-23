/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "SERIESDATA.h"
#include "../Biff_records/Dimensions.h"
#include "../Biff_records/SIIndex.h"
#include "../Biff_records/Number.h"
#include "../Biff_records/BoolErr.h"
#include "../Biff_records/Blank.h"
#include "../Biff_records/Label.h"

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
	bool result = false;
	if(proc.optional<Dimensions>())
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();
		result = true;
	}

	int count = proc.repeated<Parenthesis_SERIESDATA_1>(3, 3);
	if (count > 0)
	{
		result = true;

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
	}

	return result;
}

const bool SERIESDATA::saveContent(BinProcessor& proc)
{
    if(m_Dimensions == nullptr)
        return false;
    proc.mandatory(*m_Dimensions);
    for(auto i : m_arSIIndex)
    {   if(i != nullptr)
        {
            proc.mandatory(*i);
            auto castedPtr = static_cast<SIIndex*>(i.get());
            for(auto j : castedPtr->m_arData)
                if(j != nullptr)
                    proc.mandatory(*j);
        }
    }

    return true;
}

} // namespace XLS

