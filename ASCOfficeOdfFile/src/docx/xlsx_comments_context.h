#pragma once


#include <string>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "xlsx_comments.h"

namespace cpdoccore { namespace oox {

class xlsx_table_metrics;

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

    void start_comment(double width_pt, double height_pt, double x_pt, double y_pt);

	void add_content(std::wstring  content);
	void add_author(std::wstring  author);
	
	std::vector<odf::_property> & get_draw_properties();

	void set_visibly(bool Val);
    
	void end_comment(std::wstring ref,int col,int row);

    bool empty() const;

    void write_comments(std::wostream & strm);
    void write_comments_vml(std::wostream & strm);
    xlsx_comments_ptr get_comments();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}
