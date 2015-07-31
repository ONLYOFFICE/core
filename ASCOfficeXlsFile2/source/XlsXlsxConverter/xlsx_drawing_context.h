#pragma once

#include <vector>
#include <iosfwd>

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

class _drawing_state
{
public:
	_drawing_state() {isMediaInternal = false;}

	external_items::Type			type;

	std::wstring					rId;
	std::wstring					target;
	std::wstring					anchor;
	std::wstring					shape;
	bool							isMediaInternal;
};

class xlsx_drawing_context
{
public:


    xlsx_drawing_context(xlsx_drawing_context_handle & h);
	~xlsx_drawing_context(){}

	bool start_drawing(int type);	

	void end_drawing();

	xlsx_drawings_ptr get_drawings();

	bool empty();

	void start_image();
	void start_shape(int type);

	void set_anchor(std::wstring & str);
	void set_properties(std::wstring & str);

private:

	std::vector<_drawing_state>		drawing_state;
	
	xlsx_drawing_context_handle	  & handle_;
	xlsx_drawings_ptr				xlsx_drawings_;
	int								count_object;
};

}
