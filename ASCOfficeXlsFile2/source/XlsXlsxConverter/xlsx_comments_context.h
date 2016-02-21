#pragma once

#include <string>
#include <boost/noncopyable.hpp>

#include <common.h>

#include "xlsx_comments.h"

namespace oox {

//class xlsx_table_metrics;

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context_handle
{
public:
    xlsx_comments_context_handle();
    ~xlsx_comments_context_handle();
    
    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, std::wstring const & vml_content,xlsx_comments_ptr comments);
	std::pair<std::wstring, std::wstring> get_vml_drawing_xml();
    const std::vector<comment_elm> & content() const;

    friend class xlsx_comments_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context
{
public:
    xlsx_comments_context(xlsx_comments_context_handle & h);
    ~xlsx_comments_context();

    void start_comment();

	void add_content	(std::wstring  content);
	void add_author		(std::wstring  author);
    void set_size		(double width_pt, double height_pt, double x_pt, double y_pt);
	void set_ref		(std::wstring ref,int col,int row);
	void set_visibly	(bool Val);
 	void set_fill_color	(std::wstring  color);
  	void set_line_color	(std::wstring  color);
  
	void end_comment();

    bool empty() const;

    void write_comments		(std::wostream & strm);
    void write_comments_vml	(std::wostream & strm);

    xlsx_comments_ptr get_comments();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
