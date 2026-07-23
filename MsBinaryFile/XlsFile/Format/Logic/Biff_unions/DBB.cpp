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

#include "DBB.h"

#include "../Biff_records/SXDBB.h"
#include "SXOPER.h"

namespace XLS
{

DBB::DBB()
{
	bString		= false;
	bDate		= false;
	bNumber		= false;
	bEmpty		= false;
	bBool		= false;
}

DBB::~DBB()
{
}

BaseObjectPtr DBB::clone()
{
	return BaseObjectPtr(new DBB(*this));
}

// DBB = [SXDBB] *SXOPER
const bool DBB::loadContent(BinProcessor& proc)
{
	if(proc.optional<SXDBB>())
	{
		m_SXDBB = elements_.back();
		elements_.pop_back();
	}
	int count = proc.repeated<SXOPER>(0, 0);
	while(count--)
	{
		m_arSXOPER.push_back(elements_.front());	elements_.pop_front();
		SXOPER* operatr = dynamic_cast<SXOPER*>(m_arSXOPER.back().get());

		bString	|= operatr->bString;
		bDate	|= operatr->bDate;
		bNumber	|= operatr->bNumber;
		bEmpty	|= operatr->bEmpty;
		bBool	|= operatr->bBool;
	}

	if (!m_SXDBB && m_arSXOPER.empty())
		return false;

	return true;
}

const bool DBB::saveContent(BinProcessor& proc)
{
	if(m_SXDBB != nullptr)
		proc.mandatory(*m_SXDBB);
	for(auto i : m_arSXOPER)
		if(i != nullptr)
			proc.mandatory(*i);
	return true;
}

int DBB::serialize(std::wostream & strm)
{
	SXDBB* dbb = dynamic_cast<SXDBB*>(m_SXDBB.get());

	if (!dbb && m_arSXOPER.empty()) return 0;

	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"r")
		{ 		
			size_t indexOPER = 0;
			size_t posBlob = 0;

			for (size_t i = 0; i < arPivotCacheFields.size(); i++)
			{
				if(arPivotCacheFields[i] == false && indexOPER < m_arSXOPER.size())
				{
					m_arSXOPER[indexOPER++]->serialize(CP_XML_STREAM());
				}
				else if (posBlob < dbb->size)
				{
					if (arPivotCacheFieldShortSize[i])//fShortIitms
					{
						unsigned short * values = (unsigned short *)(dbb->blob.get() + posBlob);
						CP_XML_NODE(L"x")
						{		
							CP_XML_ATTR(L"v", *values);
						}
						posBlob+=2;
					}
					else
					{
						unsigned char * values = (unsigned char *)(dbb->blob.get() + posBlob);
						CP_XML_NODE(L"x")
						{		
							CP_XML_ATTR(L"v", *values);
						}
						posBlob++;
					}
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

