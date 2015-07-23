#pragma once

#include <string>
#include <list>

//#include "xlsx_drawing_context.h"
//#include "xlsx_comments_context.h"
#include "xlsx_hyperlinks.h"

namespace oox {

class xlsx_conversion_context;
class xlsx_text_context;

/// \class xlsx_table_context
class xlsx_table_context
{
public:
    xlsx_table_context(xlsx_conversion_context & Context, xlsx_text_context & textCotnext);

public:
	void start_table(const std::wstring & name);
    void end_table();

    std::wstring get_current_table_name() const;
 //   
 //   void start_cell(const std::wstring & formula,
 //                   size_t columnsSpanned,
 //                   size_t rowsSpanned);

 //   void end_cell();



	//xlsx_table_metrics & get_table_metrics();
 // 
	//xlsx_drawing_context & get_drawing_context();

	//xlsx_comments_context & get_comments_context();
	//


	std::wstring	add_hyperlink(std::wstring const & ref, std::wstring const & target, std::wstring const & display);
	void			dump_rels_hyperlinks(rels & Rels);
	void			serialize_hyperlinks(std::wostream & _Wostream);

private:
    xlsx_conversion_context		& context_;
    xlsx_text_context			& xlsx_text_context_;
	xlsx_hyperlinks				xlsx_hyperlinks_;
};


}

