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
#include <map>

#include <CPOptional.h>

#include "office_elements_create.h"
#include "../../DataTypes/common_attlists.h"

namespace cpdoccore {
namespace odf_writer
{

class odf_conversion_context;

class odf_controls_context
{
public:
	odf_controls_context(odf_conversion_context *odf_context);
    ~odf_controls_context();

	std::wstring start_control(int type, bool items_set = false);
	std::wstring start_control_sdt(int type);

	void end_control();

	office_element_ptr & get_root_element();

	bool is_exist_content();

	void finalize(office_element_ptr & root_elm);
	
	void set_name			(const std::wstring & name);
	void set_label			(const std::wstring & val);
	void set_linkedCell		(const std::wstring & val);
	void set_listFillRange	(const std::wstring & val);
	void set_macro			(const std::wstring & val);
	void set_disabled		(bool val);
	void set_printable		(bool val);
	void set_textHAlign		(int val);
	void set_textVAlign		(int val);
	void set_min_value		(int val);
	void set_max_value		(int val);
	void set_page_step		(int val);
	void set_step			(int val);
	void set_value			(const std::wstring & val);
	void set_horiz			(bool val);
	void set_check_state	(int val);
	void set_drop_down		(bool val);
	void set_drop_size		(int val);
	void add_item			(const std::wstring & val);
	void add_property		(const std::wstring & name, odf_types::office_value_type::type type, const std::wstring & value);

	void set_size( _CP_OPT(double) & width_pt, _CP_OPT(double) & height_pt);

private:	
    class Impl;
    _CP_PTR(Impl) impl_;
};
typedef shared_ptr<odf_controls_context>::Type	odf_controls_context_ptr;

}
}