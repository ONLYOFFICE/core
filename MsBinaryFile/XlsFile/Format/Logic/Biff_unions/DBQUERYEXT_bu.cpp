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

#include "DBQUERYEXT.h"

#include "../Biff_records/DBQueryExt.h"
#include "../Biff_records/ExtString.h"
#include "../Biff_records/OleDbConn.h"
#include "../Biff_records/TxtQry.h"

namespace XLS
{

DBQUERYEXT::DBQUERYEXT()
{
}

DBQUERYEXT::~DBQUERYEXT()
{
}

class Parenthesis_DBQUERYEXT_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DBQUERYEXT_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_DBQUERYEXT_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<OleDbConn>())
		{
			return false;
		}
		proc.repeated<ExtString>(0, 0);
		return true;
	};
};


BaseObjectPtr DBQUERYEXT::clone()
{
	return BaseObjectPtr(new DBQUERYEXT(*this));
}


// DBQUERYEXT = DBQueryExt [ExtString] *4[OleDbConn *ExtString] [TxtQry *ExtString]
const bool DBQUERYEXT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<DBQueryExt>())
	{
		return false;
	}
	m_DBQueryExt = elements_.back();
	elements_.pop_back();

	if (proc.optional<ExtString>())
	{
		m_ExtString = elements_.back();
		elements_.pop_back();
	}
	int count = proc.repeated<Parenthesis_DBQUERYEXT_1>(0, 4);

	while(!elements_.empty())
	{
		if (elements_.front()->get_type() == typeOleDbConn)
		{
			_oleDbConn conn; 
			m_arOleDbConn.push_back(conn);

			m_arOleDbConn.back().oleDbConn = elements_.front();
		}
		else
		{
			m_arOleDbConn.back().arExtString.push_back(elements_.front());
		}
		elements_.pop_front();
	}	
	if(proc.optional<TxtQry>())
	{
		m_TxtQry = elements_.back();
		elements_.pop_back();

		int count = proc.repeated<ExtString>(0, 0);

		while(count--)
		{
			m_arExtString.insert(m_arExtString.begin(), elements_.back());
			elements_.pop_back();
		}
	}

	return true;
}

const bool DBQUERYEXT::saveContent(BinProcessor& proc)
{
	if(m_DBQueryExt == nullptr)
		return false;
	proc.mandatory(*m_DBQueryExt);
	if(m_ExtString != nullptr)
		proc.mandatory(*m_ExtString);
	for(auto i : m_arOleDbConn)
	{
		if(i.oleDbConn != nullptr)
			proc.mandatory(*i.oleDbConn);
		for(auto j : i.arExtString)
			if(j != nullptr)
				proc.mandatory(*j);
	}
	if(m_TxtQry != nullptr)
		proc.mandatory(*m_TxtQry);
	for(auto i : m_arExtString)
		if(i != nullptr)
			proc.mandatory(*i);
	return true;
}

} // namespace XLS

