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

#include <string>
#include <vector>

#include <CPOptional.h>

#include "office_elements_create.h"

namespace cpdoccore {

	using namespace odf_types;

namespace odf_writer
{

class odf_conversion_context;
class odf_style_context;

class odf_notes_context
{
public:
	odf_notes_context(odf_conversion_context *odf_context);
    ~odf_notes_context();

	void set_styles_context(odf_style_context*  styles_context);

	int find_by_id(int oox_id, int type);
	
	bool is_started();// for content

	void start_note	(office_element_ptr & elm, int oox_id, int type);
	void end_note	();

	void				start_note_content	();
	office_element_ptr	get_note_content	();
	void				end_note_content	();

private:
	std::wstring find_name_by_id(int oox_id, int type);

    class Impl;
    _CP_PTR(Impl) impl_;

};

}
}