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

#include <list>

#include "xlsx_drawing_context.h"
#include "xlsx_comments_context.h"
#include "xlsx_hyperlinks.h"

namespace oox {

class xlsx_conversion_context;
class xlsx_text_context;

struct table_state
{
	table_state(xlsx_conversion_context & Context);

	xlsx_hyperlinks				hyperlinks_;
	xlsx_drawing_context		drawing_context_;
	xlsx_comments_context		comments_context_;

};
typedef _CP_PTR(table_state) table_state_ptr;

class xlsx_sheet_context
{
public:
    xlsx_sheet_context(xlsx_conversion_context & Context);

	void start_table();
    void end_table();

	xlsx_drawing_context	& get_drawing_context();
	xlsx_comments_context	& get_comments_context();
	//
	table_state_ptr & state();

	std::wstring	add_hyperlink(std::wstring const & ref, std::wstring const & target, std::wstring const & display, bool bExternal);
	void			serialize_hyperlinks(std::wostream & _Wostream);
	void			dump_rels_hyperlinks(rels & Rels);
    
	void			serialize_ole_objects(std::wostream & _Wostream);
	void			serialize_controls(std::wostream & _Wostream);
	
	void			dump_rels_drawing(rels & Rels);
private:
    xlsx_conversion_context		& context_;

	std::list<table_state_ptr>	tables_state_;
};


}

