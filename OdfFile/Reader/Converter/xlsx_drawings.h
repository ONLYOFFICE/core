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

#include <CPScopedPtr.h>
#include <xml/attributes.h>

#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawings;
typedef _CP_PTR(xlsx_drawings) xlsx_drawings_ptr;

struct drawing_elm
{
    drawing_elm(std::wstring const & _filename, std::wstring const & _content, xlsx_drawings_ptr _drawings, _rels_type const & _type)
        : filename(_filename), content(_content), drawings(_drawings), type(_type)
    {}
    
    std::wstring		filename;
    std::wstring		content;
    xlsx_drawings_ptr	drawings;
	_rels_type			type;
};

class _xlsx_drawing;

class xlsx_drawings
{
public:
    xlsx_drawings	(bool inGroup_);
    ~xlsx_drawings	();
    static xlsx_drawings_ptr create(bool inGroup);

    void add ( _xlsx_drawing & d,	bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel = false );
    void add (						bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel, bool vml_rel);
    
	bool empty() const;
	bool vml_empty() const;
   
	void dump_rels_sheet		(rels & Rels);
    void dump_rels_drawing		(rels & Rels);
    void dump_rels_vml_drawing	(rels & Rels);

    void serialize			(std::wostream & _Wostream, const std::wstring & ns, bool local);  
	void serialize_vml		(std::wostream & _Wostream);
	void serialize_objects	(std::wostream & _Wostream);  
	void serialize_controls	(std::wostream & _Wostream);  

	std::vector<_xlsx_drawing> get();
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}
