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

#include "xlsx_sheet_context.h"
#include "xlsx_textcontext.h"
#include "xlsx_conversion_context.h"

#include <sstream>

#include "../../Common/Utils/simple_xml_writer.h"

namespace oox {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
table_state::table_state(xlsx_conversion_context & Context) : drawing_context_(Context), comments_context_(Context.get_comments_context_handle())
{
}

table_state_ptr & xlsx_sheet_context::state()
{
	if (false == tables_state_.empty())
	{
		return tables_state_.back();
	}
	else
	{
		throw;
	}
}

xlsx_sheet_context::xlsx_sheet_context(xlsx_conversion_context & Context) : context_(Context)
{        
}

void xlsx_sheet_context::start_table()
{
	tables_state_.push_back( table_state_ptr(new table_state(context_)));
}

void xlsx_sheet_context::end_table()
{
	if (!get_drawing_context().empty())
    {
		std::wstringstream strm;
		get_drawing_context().serialize(strm);
		
		const std::pair<std::wstring, std::wstring> drawingName	= 
			context_.get_drawing_context_handle().add_drawing_xml(strm.str(), get_drawing_context().get_rels());

		context_.current_sheet().set_drawing_link(drawingName.first, drawingName.second);

        CP_XML_WRITER(context_.current_sheet().drawing())
        {
            CP_XML_NODE(L"drawing")
            {
                CP_XML_ATTR(L"r:id", drawingName.second);
            }
        }
    }
	if (!get_drawing_context().empty_vml_HF())
    {
		std::wstringstream strm;
		get_drawing_context().serialize_vml_HF(strm);
		
		const std::pair<std::wstring, std::wstring> vmlDrawingName	= 
			context_.get_drawing_context_handle().add_drawing_vml(strm.str(), get_drawing_context().get_vml_HF_rels());

		context_.current_sheet().set_vml_HF_drawing_link(vmlDrawingName.first, vmlDrawingName.second);
	}
	if (!get_drawing_context().empty_vml())
    {
		std::wstringstream strm;
		get_drawing_context().serialize_vml(strm);
		
		const std::pair<std::wstring, std::wstring> vmlDrawingName	= 
			context_.get_drawing_context_handle().add_drawing_vml(strm.str(), get_drawing_context().get_vml_rels());

		context_.current_sheet().set_vml_drawing_link(vmlDrawingName.first, vmlDrawingName.second);
	}
	if (!get_comments_context().empty())
    {
        std::wstringstream strm;
        get_comments_context().write_comments(strm);
        
		const std::pair<std::wstring, std::wstring> commentsName =
            context_.get_comments_context_handle().add_comments_xml(strm.str(), context_.get_comments_context().get_comments());

        context_.current_sheet().set_comments_link(commentsName.first, commentsName.second);
    }    
}


xlsx_drawing_context & xlsx_sheet_context::get_drawing_context()
{
	if (state())
		return state()->drawing_context_;
	else
	{
		throw;
	}
}

xlsx_comments_context & xlsx_sheet_context::get_comments_context()
{
	if (state())
		return state()->comments_context_;
	else
	{
		throw;
	}
}

std::wstring xlsx_sheet_context::add_hyperlink(std::wstring const & ref, std::wstring const & target, std::wstring const & display, bool bExternal)
{
	if (state())
		return state()->hyperlinks_.add(ref, target, display, bExternal);
	else return L"";
}
 void xlsx_sheet_context::dump_rels_hyperlinks(rels & Rels)
{
	 if (state())
		 state()->hyperlinks_.dump_rels(Rels);
}
void xlsx_sheet_context::serialize_hyperlinks(std::wostream & _Wostream)
{
	if (state())
		state()->hyperlinks_.serialize(_Wostream);
}
void xlsx_sheet_context::dump_rels_drawing(rels & Rels)
{
	if (state())
	{
		xlsx_drawings_rels_ptr drawing_rels = state()->drawing_context_.get_sheet_rels();

		drawing_rels->dump_rels(Rels);
	}
}
void xlsx_sheet_context::serialize_ole_objects(std::wostream & strm)
{
	if (state())
		state()->drawing_context_.serialize_objects(strm);
}
void xlsx_sheet_context::serialize_controls(std::wostream & strm)
{
	if (state())
		state()->drawing_context_.serialize_controls(strm);
}
}
