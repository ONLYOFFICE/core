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

#include "METADATA.h"
#include "MDTINFO.h"
#include "MDXSTR.h"
#include "MDBLOCK.h"
#include "MDXTUPLESET.h"

#include "../Biff_records/MDXProp.h"
#include "../Biff_records/MDXKPI.h"

namespace XLS
{


METADATA::METADATA()
{
}


METADATA::~METADATA()
{
}

class Parenthesis_METADATA_1: public ABNFParenthesis
{
	BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_METADATA_1)
public:
	BaseObjectPtr clone()
	{
		return BaseObjectPtr(new Parenthesis_METADATA_1(*this));
	}

	const bool loadContent(BinProcessor& proc)
	{
		if(!proc.optional<MDXTUPLESET>())
		{
			if(!proc.optional<MDXProp>())
			{
				if(!proc.optional<MDXKPI>())
				{
					return false;
				}
			}
		}
		return true;
	};
};

BaseObjectPtr METADATA::clone()
{
	return BaseObjectPtr(new METADATA(*this));
}


// METADATA = *MDTINFO *MDXSTR *(MDXTUPLESET / MDXProp / MDXKPI) *MDBLOCK
const bool METADATA::loadContent(BinProcessor& proc)
{
	int count1 = proc.repeated<MDTINFO>(0, 0);
	while(!elements_.empty())
	{
		m_arMDTINFO.insert(m_arMDTINFO.begin(), elements_.back());
		elements_.pop_back();
	}
	
	int count2 = proc.repeated<MDXSTR>(0, 0);
	while(!elements_.empty())
	{
		m_arMDXSTR.insert(m_arMDXSTR.begin(), elements_.back());
		elements_.pop_back();
	}	
	
	int count3 = proc.repeated<Parenthesis_METADATA_1>(0, 0);
	while(!elements_.empty())
	{
		m_arMDTSET.insert(m_arMDTSET.begin(), elements_.back());
		elements_.pop_back();
	}

	int count4 = proc.repeated<MDBLOCK>(0, 0);
	while(!elements_.empty())
	{
		m_arMDBLOCK.insert(m_arMDBLOCK.begin(), elements_.back());
		elements_.pop_back();
	}
	if (count1 > 0 || count2 > 0 || count3 > 0 || count4 > 0)
	{
		return true;
	}
	return false;
}

const bool METADATA::saveContent(BinProcessor& proc)
{
	for(auto i : m_arMDTINFO)
		if(i != nullptr)
			proc.mandatory(*i);
	for(auto i : m_arMDXSTR)
		if(i != nullptr)
			proc.mandatory(*i);
	for(auto i : m_arMDTSET)
		if(i != nullptr)
			proc.mandatory(*i);
	for(auto i : m_arMDBLOCK)
		if(i != nullptr)
			proc.mandatory(*i);
	return true;
}

} // namespace XLS

