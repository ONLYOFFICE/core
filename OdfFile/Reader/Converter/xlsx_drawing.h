/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
