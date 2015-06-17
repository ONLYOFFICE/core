#pragma once 

#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/xml/attributes.h>
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

	struct _pptx_comment
	{
		size_t x_, y_;

		int author_id_;
		int idx_;
		
		std::wstring  date_;

		std::wstring  content_;

		std::vector<odf_reader::_property> graphicProperties_;
	};
//class rels;

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;

struct pptx_comment_elm
{
    pptx_comment_elm(std::wstring const & _filename, std::wstring const & _content, pptx_comments_ptr _comments)
        : filename(_filename), content(_content), comments(_comments)
    {}
	pptx_comments_ptr comments;
    
    std::wstring filename;
    std::wstring content;
};

class pptx_comments
{
public:
    pptx_comments();
    ~pptx_comments();
    static pptx_comments_ptr create();

public:
    void add(_pptx_comment & d);
    bool empty() const;

	friend void pptx_serialize(std::wostream & _Wostream, pptx_comments & val);    

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}