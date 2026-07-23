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

#include "FONTLIST.h"
#include "../Biff_records/FrtFontList.h"
#include "../Biff_records/StartObject.h"
#include "../Biff_records/Font.h"
#include "../Biff_records/Fbi.h"
#include "../Biff_records/EndObject.h"

namespace XLS
{


FONTLIST::FONTLIST()
{
}


FONTLIST::~FONTLIST()
{
}


class Parenthesis_FONTLIST_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_FONTLIST_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_FONTLIST_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.mandatory<Font>())
		{
			return false;
		}
		proc.optional<Fbi>();
		return true;
	};
};



BaseObjectPtr FONTLIST::clone()
{
	return BaseObjectPtr(new FONTLIST(*this));
}


// FONTLIST = FrtFontList StartObject *(Font [Fbi]) EndObject
const bool FONTLIST::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<FrtFontList>())
	{
		return false;
	}
	proc.mandatory<StartObject>();					elements_.pop_back();
	
	proc.repeated<Parenthesis_FONTLIST_1>(0, 0);

	proc.mandatory<EndObject>();					elements_.pop_back();

	return true;
}

const bool FONTLIST::saveContent(BinProcessor& proc)
{
	if(m_fontList == nullptr)
		return  false;
	proc.mandatory(*m_fontList);
	if(m_startObject != nullptr)
		proc.mandatory(*m_startObject);
	for(auto i : m_fonts)
	{
		if(i.first != nullptr)
			proc.mandatory(*i.first);
		if(i.second != nullptr)
			proc.mandatory(*i.second);
	}
	if(m_endObject != nullptr)
		proc.mandatory(*m_endObject);
	return true;
}

} // namespace XLS

