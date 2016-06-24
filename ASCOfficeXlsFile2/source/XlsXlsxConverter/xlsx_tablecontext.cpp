/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "xlsx_tablecontext.h"
#include "xlsx_textcontext.h"
#include "xlsx_conversion_context.h"

#include <boost/foreach.hpp>
#include <sstream>

#include "simple_xml_writer.h"


namespace oox {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
table_state::table_state(xlsx_conversion_context & Context) : drawing_context_(Context), comments_context_(Context.get_comments_context_handle())
{
}

table_state_ptr & xlsx_table_context::state()
{
    return tables_state_.back();
}

xlsx_table_context::xlsx_table_context(xlsx_conversion_context & Context) : context_(Context)
{        
}

void xlsx_table_context::start_table(const std::wstring & name)
{
	tables_state_.push_back( table_state_ptr(new table_state(context_)));
}

void xlsx_table_context::end_table()
{
	if (!get_drawing_context().empty())
    {
		std::wstringstream strm;
		get_drawing_context().serialize(strm);
		
		const std::pair<std::wstring, std::wstring> drawingName	= 
			context_.get_drawing_context_handle().add_drawing_xml(strm.str(), get_drawing_context().get_drawings_rels());

		context_.current_sheet().set_drawing_link(drawingName.first, drawingName.second);

        CP_XML_WRITER(context_.current_sheet().drawing())
        {
            CP_XML_NODE(L"drawing")
            {
                CP_XML_ATTR(L"r:id", drawingName.second);
            }
        }
    }
	if (!get_comments_context().empty())
    {
        std::wstringstream strm;
        get_comments_context().write_comments(strm);
        
        std::wstringstream vml_strm;
        get_comments_context().write_comments_vml(vml_strm);
		
		const std::pair<std::wstring, std::wstring> commentsName =
            context_.get_comments_context_handle().add_comments_xml(strm.str(), vml_strm.str(), context_.get_comments_context().get_comments() );

		const std::pair<std::wstring, std::wstring> vml_drawingName =
								context_.get_comments_context_handle().get_vml_drawing_xml();

        context_.current_sheet().set_comments_link(commentsName.first, commentsName.second);
        context_.current_sheet().set_vml_drawing_link(vml_drawingName.first, vml_drawingName.second);
    }    
}


xlsx_drawing_context & xlsx_table_context::get_drawing_context()
{
    return state()->drawing_context_;
}

xlsx_comments_context & xlsx_table_context::get_comments_context()
{
    return state()->comments_context_;
}

std::wstring xlsx_table_context::add_hyperlink(std::wstring const & ref, std::wstring const & target, std::wstring const & display, bool bExternal)
{
    return state()->hyperlinks_.add( ref, target, display, bExternal);
}
 void xlsx_table_context::dump_rels_hyperlinks(rels & Rels)
{
	return state()->hyperlinks_.dump_rels(Rels);
}
void xlsx_table_context::serialize_hyperlinks(std::wostream & _Wostream)
{
    return state()->hyperlinks_.serialize(_Wostream);
}


}
