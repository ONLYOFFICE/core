#pragma once 

#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/xml/attributes.h>
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

	struct _xlsx_comment
	{
		size_t left_, top_;
		size_t width_, height_;

		std::wstring  ref_;

		int col_;
		int row_;

		bool visibly_;

		std::vector<odf::_property> graphicProperties_;

		std::wstring  author_;
		std::wstring  content_;
	};
//class rels;

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

struct comment_elm
{
    comment_elm(std::wstring const & _filename, std::wstring const & _vml_filename, std::wstring const & _content, std::wstring const & _vml_content, xlsx_comments_ptr _comments)
        : filename(_filename), content(_content), comments(_comments),vml_filename(_vml_filename),vml_content(_vml_content)
    {}
	xlsx_comments_ptr comments;
    
    std::wstring filename;
    std::wstring content;
   
    std::wstring vml_filename;
	std::wstring vml_content;
};

class xlsx_comments
{
public:
    xlsx_comments();
    ~xlsx_comments();
    static xlsx_comments_ptr create();

public:
    void add(_xlsx_comment & d);
    bool empty() const;

    friend void docx_serialize(std::wostream & _Wostream, xlsx_comments const & val);    

    friend void xlsx_serialize(std::wostream & _Wostream, xlsx_comments const & val);    
    friend void xlsx_serialize_vml(std::wostream & _Wostream, xlsx_comments const & val);    

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}