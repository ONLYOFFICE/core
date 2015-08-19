#pragma once

#include <list>

#include "xlsx_drawing_context.h"
//#include "xlsx_comments_context.h"
#include "xlsx_hyperlinks.h"

namespace oox {

class xlsx_conversion_context;
class xlsx_text_context;

struct table_state
{
	table_state(xlsx_conversion_context & Context);

	xlsx_hyperlinks				hyperlinks_;
	xlsx_drawing_context		drawing_context_;
   //xlsx_comments_context	comments_context_;

};
typedef _CP_PTR(table_state) table_state_ptr;

class xlsx_table_context
{
public:
    xlsx_table_context(xlsx_conversion_context & Context/*, xlsx_text_context & textCotnext*/);

public:
	void start_table(const std::wstring & name);
    void end_table();

	xlsx_drawing_context & get_drawing_context();

	//xlsx_comments_context & get_comments_context();
	//
	table_state_ptr & state();

	std::wstring	add_hyperlink(std::wstring const & ref, std::wstring const & target, std::wstring const & display, bool bExternal);
	void			dump_rels_hyperlinks(rels & Rels);
	void			serialize_hyperlinks(std::wostream & _Wostream);

private:
    xlsx_conversion_context		& context_;
    //xlsx_text_context			& xlsx_text_context_;

	std::list<table_state_ptr>		tables_state_;
};


}

