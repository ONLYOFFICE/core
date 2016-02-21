
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
