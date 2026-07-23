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
#include "oox_package.h"
#include <unordered_map>

namespace oox {

class xlsx_pivot_cache_context;
typedef _CP_PTR(xlsx_pivot_cache_context) xlsx_pivot_cache_context_ptr;

class xlsx_pivots_context
{
public:
    xlsx_pivots_context();
    ~xlsx_pivots_context();

	int add_view(std::wstring table_view, int indexCache);
	int get_view_count();

	void add_cache(std::wstring definitions, std::wstring records);
	void add_cache_external(std::unordered_map<std::wstring, std::wstring> &externals);

	int get_cache_count();

	void write_cache_definitions_to	(int index, std::wostream & strm);
	void write_cache_records_to		(int index, std::wostream & strm);

	void write_table_view_to		(int index, std::wostream & strm);

	void dump_rels_cache(int index, rels & Rels);
	void dump_rels_view	(int index, rels & Rels);

	void add_connections(std::wstring connections);
private:
    class Impl;
    _CP_PTR(Impl) impl_;
              
};

}
