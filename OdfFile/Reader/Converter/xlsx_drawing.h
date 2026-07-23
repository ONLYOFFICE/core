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

#include <iosfwd>
#include <iostream>

#include "oox_drawing.h"
#include "xlsx_table_position.h"
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

struct xlsx_drawing_position
{
    enum type_t {from, to}	type;
    xlsx_table_position		position;

	void serialize(std::wostream & _Wostream, const std::wstring & ns_title = L"xdr", const std::wstring & ns = L"xdr");
	std::wstring vml_serialize();
};

class _xlsx_drawing : public _oox_drawing
{
public:
	_xlsx_drawing() : _oox_drawing(), type_anchor(1) {}
	
	int type_anchor;
    
	xlsx_drawing_position	from_;
    xlsx_drawing_position	to_;

	_CP_OPT(_INT32)			owner_cx_;
	_CP_OPT(_INT32)			owner_cy_;

	std::wstring			content_group_;

	std::vector<_xlsx_drawing>	childs_;

	virtual void serialize	(std::wostream & strm) { return serialize(strm, L"xdr", false); }

    void serialize			(std::wostream & strm, const std::wstring & ns, bool local);    
	void serialize_vml		(std::wostream & strm);    
	void serialize_object	(std::wostream & strm);    
	void serialize_control	(std::wostream & strm);    
};
}
}
