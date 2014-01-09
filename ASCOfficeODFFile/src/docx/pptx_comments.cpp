#include "precompiled_cpodf.h"
#include "pptx_comments.h"
#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

class pptx_comments::Impl
{
public:

	void serialize(std::wostream & strm) const
    {
        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"p:cmLst")
            {
                CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
                CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");

				BOOST_FOREACH(_pptx_comment const & c, pptx_comment_)
				{
					CP_XML_NODE(L"p:cm")
					{
						CP_XML_ATTR(L"idx",c.idx_);
						
						CP_XML_ATTR(L"authorId", c.author_id_);
						CP_XML_ATTR(L"dt",	c.date_);
						
						CP_XML_NODE(L"p:pos")
						{
							CP_XML_ATTR(L"x", c.x_);
							CP_XML_ATTR(L"y", c.y_);
						}
						CP_XML_NODE(L"p:text")
						{
							CP_XML_STREAM() << c.content_;
						}
					} 					
				}
			}
		}
	}
	bool empty() const
    {
        return ( pptx_comment_.empty());
    }
    void add(_pptx_comment & c)
    {
		pptx_comment_.push_back(c);
	}
private:
	std::vector<_pptx_comment> pptx_comment_;
};

pptx_comments::pptx_comments() : impl_( new pptx_comments::Impl() )
{
}

pptx_comments::~pptx_comments()
{
}

void pptx_comments::add(_pptx_comment & c)
{
	impl_->add(c);
}
bool pptx_comments::empty() const
{
    return impl_->empty();
}
void pptx_serialize(std::wostream & _Wostream, pptx_comments const & val)
{
	val.impl_->serialize(_Wostream);
}

pptx_comments_ptr pptx_comments::create()
{
    return boost::make_shared<pptx_comments>();
}

}
}


       