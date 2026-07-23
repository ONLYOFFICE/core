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

#include "SXRANGE.h"

#include "../Biff_records/SXRng.h"
#include "../Biff_records/SXNum.h"
#include "../Biff_records/SXDtr.h"
#include "../Biff_records/SXInt.h"

namespace XLS
{

//  (3SXNum / (2SXDtr SXInt)
class Parenthesis_SXRANGE: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_SXRANGE)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_SXRANGE(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		int count = proc.repeated<SXNum>(0, 3);

		if (count < 1)
		{
			count = proc.repeated<SXDtr>(0, 2);
			if (count < 1)
				return false;
			if (proc.optional<SXInt>())
				count++;
		}
		return (count == 3);
	};
};


SXRANGE::SXRANGE()
{
}

SXRANGE::~SXRANGE()
{
}

BaseObjectPtr SXRANGE::clone()
{
	return BaseObjectPtr(new SXRANGE(*this));
}

// SXRANGE = SXRng (3SXNum / (2SXDtr SXInt))
const bool SXRANGE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXRng>())
	{
		return false;
	}
	m_SXRng = elements_.back();
	elements_.pop_back();

	if(proc.optional<Parenthesis_SXRANGE>())
	{
		while(elements_.empty() == false)
		{
			SXNum* num = dynamic_cast<SXNum*>(elements_.front().get());
			if (num)
			{
				m_arSXNum.push_back(elements_.front());
			}
			else
			{
				SXDtr* dtr = dynamic_cast<SXDtr*>(elements_.front().get());
				if (dtr)
				{
					m_arSXDtr.push_back(elements_.front());
				}
			}
			elements_.pop_front();
		}
	}

	return true;
}

const bool SXRANGE::saveContent(BinProcessor& proc)
{
	if(m_SXRng == nullptr)
		return false;
	proc.mandatory(*m_SXRng);
	if(!m_arSXNum.empty())
	{
		for(auto i : m_arSXNum)
			if(i != nullptr)
				proc.mandatory(*i);
	}
	else if (!m_arSXDtr.empty())
	{
		for(auto i : m_arSXDtr)
			if(i!= nullptr)
				proc.mandatory(*i);
	}
	return true;
}

int SXRANGE::serialize(std::wostream & strm)
{
	SXRng* rng = dynamic_cast<SXRng*>(m_SXRng.get());

	if (!rng) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"rangePr")
		{ 
			switch(rng->iByType)
			{
			case 0:	CP_XML_ATTR(L"groupBy", L"range");		break;
			case 1:	CP_XML_ATTR(L"groupBy", L"seconds");	break;
			case 2:	CP_XML_ATTR(L"groupBy", L"minutes");	break;
			case 3:	CP_XML_ATTR(L"groupBy", L"hours");		break;
			case 4:	CP_XML_ATTR(L"groupBy", L"days");		break;
			case 5:	CP_XML_ATTR(L"groupBy", L"months");		break;
			case 6:	CP_XML_ATTR(L"groupBy", L"quarters");	break;
			case 7:	CP_XML_ATTR(L"groupBy", L"years");		break;
			}
			if (m_arSXDtr.size() == 2)
			{
				SXDtr* startDate = dynamic_cast<SXDtr*>(m_arSXDtr[0].get());
				SXDtr* endDate = dynamic_cast<SXDtr*>(m_arSXDtr[1].get());

				CP_XML_ATTR(L"startDate", startDate->value());	
				CP_XML_ATTR(L"endDate", endDate->value());	
			}
			if (m_arSXNum.size() == 3)
			{
			}
		}
	}

	
	return 0;
}
} // namespace XLS

