#pragma once

#include <vector>

#include "xlsx_drawings.h"
	
namespace oox {

class external_items;

class xlsx_drawing_context_handle
{
public:
    xlsx_drawing_context_handle(external_items & items);
    ~xlsx_drawing_context_handle();
    
    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings);
    const std::vector<drawing_elm> & content() const;

    friend class xlsx_drawing_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};


class xlsx_drawing_context
{
public:
    xlsx_drawing_context(xlsx_drawing_context_handle & h);
	~xlsx_drawing_context(){}

	void start_drawing(std::wstring const & name, int type);	

	void end_drawing();

	std::wostream & drawing_stream() {return stream_;}
    
	xlsx_drawings_ptr get_drawings();

	bool empty();

private:
	std::wstringstream				stream_;
	xlsx_drawing_context_handle	  & handle_;
	xlsx_drawings_ptr				xlsx_drawings_;
	int								count_object;
};

}
