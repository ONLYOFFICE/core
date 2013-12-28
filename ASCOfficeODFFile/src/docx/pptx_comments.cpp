#include "precompiled_cpodf.h"
#include "pptx_comments.h"
#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

	//unsigned int hex_string_to_int(std::wstring str)
	//{
	//	unsigned int x;   
	//	std::wstringstream ss;
	//	ss << std::hex << str;
	//	ss >> x;
	//	return x;
	//}	
	//
class pptx_comments::Impl
{
public:
	void serialize_authors(std::wostream & strm) const
    {
        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"p:cmAuthorLst")
            {
                CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
                CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");

  ////<p:cmAuthor id="0" name="" initials="" lastIdx="4" clrIdx="0"/>
  ////<p:cmAuthor id="1" name="Elen Subbotina" initials="ES" lastIdx="2" clrIdx="1"/>
		//		int i=0;
		//		int size = author_list_.size();
		//		while(true)
		//		{
		//			if (i>=size)break;

		//			CP_XML_NODE(L"p:cmAuthor")
		//			{
		//				CP_XML_ATTR(L"id", i);
		//				CP_XML_STREAM() << author_list_[i];//этот список относится к омментам на всех листах
		//			}
		//			i++;
		//		}			
			}
		}
	}
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
						CP_XML_ATTR(L"authorId", c.author_);
						
						CP_XML_NODE(L"p:pos")
						{
							CP_XML_ATTR(L"y", c.top_);
							CP_XML_ATTR(L"x", c.left_);
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
		bool find = false;
		for (long i=0;i<author_list_.size();i++)
		{		
			if (c.author_ == author_list_[i])
			{
				find=true;
				c.author_ = boost::lexical_cast<std::wstring>(i);
				break;
			}
		}
		if (!find)
		{
			author_list_.push_back(c.author_);
			c.author_ = boost::lexical_cast<std::wstring>(author_list_.size()-1);
		}
		pptx_comment_.push_back(c);
	}
private:
	std::vector<std::wstring> author_list_;
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
void pptx_serialize_authors(std::wostream & _Wostream, pptx_comments const & val)
{
	val.impl_->serialize_authors(_Wostream);
}

pptx_comments_ptr pptx_comments::create()
{
    return boost::make_shared<pptx_comments>();
}

}
}


       