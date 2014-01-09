#pragma once


#include <string>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>

#include "pptx_comments.h"

namespace cpdoccore { namespace oox {

class pptx_table_metrics;

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;



class pptx_comments_context_handle
{
public:
    pptx_comments_context_handle();
    ~pptx_comments_context_handle();
    
    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, pptx_comments_ptr comments);
    const std::vector<pptx_comment_elm> & content() const;

    friend class pptx_comments_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;

class pptx_comments_context
{
public:
    pptx_comments_context(pptx_comments_context_handle & h);
    ~pptx_comments_context();

    void start_comment(double x_emu, double y_emu,int id_author, int idx_comment);

	void add_content	(std::wstring  content);
	void add_date		(std::wstring date);
	
	std::vector<odf::_property> & get_draw_properties();
    
	void end_comment();

    bool empty() const;

    void write_comments(std::wostream & strm);
    
	pptx_comments_ptr get_comments();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}
