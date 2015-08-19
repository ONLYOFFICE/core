
#include "xlsx_tablecontext.h"
#include "xlsx_textcontext.h"
#include "xlsx_conversion_context.h"
//#include "logging.h"

#include <boost/foreach.hpp>
#include <sstream>

#include "simple_xml_writer.h"


namespace oox {

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
table_state::table_state(xlsx_conversion_context & Context) : drawing_context_(Context.get_drawing_context_handle())
{
}

table_state_ptr & xlsx_table_context::state()
{
    return tables_state_.back();
}
//
//const xlsx_table_state & xlsx_table_context::state() const
//{
//    return table_state_stack_.back();
//}

xlsx_table_context::xlsx_table_context(xlsx_conversion_context & Context/*, xlsx_text_context & textContext*/) : 
										context_(Context)//, xlsx_text_context_(textContext)
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
		get_drawing_context().get_drawings()->serialize(strm);
		
		const std::pair<std::wstring, std::wstring> drawingName	= context_.get_drawing_context_handle().add_drawing_xml(strm.str(), get_drawing_context().get_drawings());

		context_.current_sheet().set_drawing_link(drawingName.first, drawingName.second);

        CP_XML_WRITER(context_.current_sheet().drawing())
        {
            CP_XML_NODE(L"drawing")
            {
                CP_XML_ATTR(L"r:id", drawingName.second);
            }
        }
    }
}


xlsx_drawing_context & xlsx_table_context::get_drawing_context()
{
    return state()->drawing_context_;
}
//
//xlsx_comments_context & xlsx_table_context::get_comments_context()
//{
//    return state().get_comments_context();
//}
//void xlsx_table_context::table_column_last_width(double w)
//{
//    return state().table_column_last_width(w);
//}
//
//double xlsx_table_context::table_column_last_width() const
//{
//    return state().table_column_last_width();
//}
//
//void xlsx_table_context::start_hyperlink()
//{
//    return state().start_hyperlink();
//}
//
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
