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

#include "hyperlinks.h"

#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore { 
namespace oox {

hyperlinks::_ref  hyperlinks::last()
{
	_ref r={};
	
	if (!hrefs_.empty())
		r = hrefs_.back();

	return r;
}

std::wstring hyperlinks::add(const std::wstring & href, _rels_type_place type_place, bool drawing)
{
	std::wstring id = std::wstring(L"rHpId") + std::to_wstring(hrefs_.size() + 1);
	
	_ref r = { XmlUtils::EncodeXmlString(href), type_place, drawing, id } ;
	
	hrefs_.push_back(r);

	return id;
}


void hyperlinks::dump_rels(rels & Rels, _rels_type_place type_place)
{
    size_t i = 0;
    for (size_t i = 0; i < hrefs_.size(); i++)
    {
 		if (hrefs_[i].used_rels)continue; // already used this rels

		if (hrefs_[i].type_place == type_place)
		{
			Rels.add( relationship(hrefs_[i].id, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink", hrefs_[i].href, L"External" ) );
			hrefs_[i].used_rels = true;
		}
    }
}

}
}
