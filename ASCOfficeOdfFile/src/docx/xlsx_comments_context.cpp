
#include "xlsx_comments_context.h"
#include "xlsx_table_metrics.h"
#include <boost/foreach.hpp>
#include <iostream>
#include "../odf/datatypes/length.h"
#include "xlsx_utils.h"

//#include <cpdoccore/formulasconvert.h>

namespace cpdoccore { namespace oox {

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context_handle::Impl
{
public:
    Impl()
        :  next_comments_id_(1) ,next_file_id_(1)
    {
    }  

    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, std::wstring const & vml_content,xlsx_comments_ptr comments)
    {
 		const std::wstring file_id = boost::lexical_cast<std::wstring>(next_file_id_++);
      
		const std::wstring fileName = std::wstring(L"comments") + file_id + L".xml";
        const std::wstring vml_fileName = std::wstring(L"vmlDrawing") + file_id + L".vml";
        
		comments_.push_back(comment_elm(fileName,vml_fileName, content, vml_content, comments));
        
		const std::wstring id = boost::lexical_cast<std::wstring>(next_comments_id_++);
		const std::wstring rId = std::wstring(L"comId") + id; 
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    std::pair<std::wstring, std::wstring> get_vml_drawing_xml()
    {
        const std::wstring id = boost::lexical_cast<std::wstring>(next_comments_id_++);		
 		const std::wstring rId = std::wstring(L"comId") + id; 
		return std::pair<std::wstring, std::wstring>(comments_.back().vml_filename, rId);
	}

    const std::vector<comment_elm> & content() const
    {
        return comments_;
    }

private:
   
    std::vector<comment_elm> comments_;

    size_t next_comments_id_;
    size_t next_file_id_;
};

xlsx_comments_context_handle::xlsx_comments_context_handle()
: impl_(new xlsx_comments_context_handle::Impl())
{
}

xlsx_comments_context_handle::~xlsx_comments_context_handle()
{
}

std::pair<std::wstring, std::wstring> xlsx_comments_context_handle::add_comments_xml(std::wstring const & content, std::wstring const & vml_content,xlsx_comments_ptr comments)
{
    return impl_->add_comments_xml(content,vml_content, comments);
}
std::pair<std::wstring, std::wstring> xlsx_comments_context_handle::get_vml_drawing_xml()
{
    return impl_->get_vml_drawing_xml();
}

const std::vector<comment_elm> & xlsx_comments_context_handle::content() const
{
    return impl_->content();
}

class xlsx_comments_context::Impl
{
public:    
    Impl(xlsx_comments_context_handle & handle) : xlsx_comments_(xlsx_comments::create()),
        handle_(handle) 
    {} 

    xlsx_comments_context_handle & handle_;
    _xlsx_comment current_;    

    void add_comment(_xlsx_comment & d)
    {
        xlsx_comments_->add(d);
    }

    void write_comments(std::wostream & strm)
    {
        xlsx_serialize(strm, *xlsx_comments_);
    }
    void write_comments_vml(std::wostream & strm)
    {
        xlsx_serialize_vml(strm, *xlsx_comments_);
    }
    bool empty() const
    {
        return xlsx_comments_->empty();
    }

    xlsx_comments_ptr get_comments()
    {
        return xlsx_comments_;
    }
private:
    xlsx_comments_ptr xlsx_comments_;
};


xlsx_comments_context::xlsx_comments_context(xlsx_comments_context_handle & h)
 : impl_(new xlsx_comments_context::Impl(h))
{    
}

xlsx_comments_context::~xlsx_comments_context()
{
}

void xlsx_comments_context::start_comment (double width_pt, double height_pt, double x_pt, double y_pt)
{
    impl_->current_.ref_ = L"";
	impl_->current_.width_ = width_pt;
    impl_->current_.height_ = height_pt;
    impl_->current_.left_ = x_pt;
    impl_->current_.top_ = y_pt; 

	impl_->current_.visibly_ = false;
}
void xlsx_comments_context::add_content(std::wstring  content)
{
	impl_->current_.content_ = content;
}
void xlsx_comments_context::add_author(std::wstring  author)
{
	impl_->current_.author_ = author;
}
void xlsx_comments_context::set_visibly(bool Val)
{
	impl_->current_.visibly_ = Val;
}

std::vector<odf::_property> & xlsx_comments_context::get_draw_properties()
{
	return impl_->current_.graphicProperties_;

}
void xlsx_comments_context::end_comment(std::wstring ref,int col, int row)
{
	impl_->current_.ref_ = ref;
	impl_->current_.col_ = col;
	impl_->current_.row_ = row;
   
	impl_->add_comment(impl_->current_);
}

bool xlsx_comments_context::empty() const
{
    return impl_->empty();
}

void xlsx_comments_context::write_comments(std::wostream & strm)
{
    impl_->write_comments(strm);    
}
void xlsx_comments_context::write_comments_vml(std::wostream & strm)
{
	impl_->write_comments_vml(strm);    
}

xlsx_comments_ptr xlsx_comments_context::get_comments()
{
    return impl_->get_comments();
}

}
}
