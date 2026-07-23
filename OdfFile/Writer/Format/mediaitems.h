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
#pragma once

#include <vector>
#include <string>

namespace cpdoccore { 
namespace odf_writer {

class rels;

class _mediaitems
{
public:
    enum Type { typeUnknown = 0, typeImage, typeMedia, typeOleObject, typeObjectReplacement};

    struct item 
    {
		item(const std::wstring & _oox_ref, Type _type, const std::wstring & _odf_ref);

        Type type = typeUnknown;
        
		std::wstring oox_ref;
        std::wstring odf_ref;
    };
    _mediaitems()
    {
 		count_image			= 0;
 		count_media			= 0;
 		count_object		= 0;
 		count_image_object	= 0;
 		count_control		= 0;
	}
	
	size_t count_image;
	size_t count_media;
	size_t count_object;
	size_t count_image_object;
	size_t count_control;

    void add_or_find(const std::wstring & oox_ref, Type type, std::wstring & odf_ref);
    
	void dump_rels(rels & Rels, std::wstring local_path = L"");
    std::vector<item> & items() { return items_; }

private:
    std::vector<item> items_;

};

}
}

