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

