
#include "xlsx_comments.h"
#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>
#include "docx_rels.h"

namespace cpdoccore {
namespace oox {

	unsigned int hex_string_to_int(std::wstring str)
	{
		unsigned int x;   
		std::wstringstream ss;
		ss << std::hex << str;
		ss >> x;
		return x;
	}	
	
class xlsx_comments::Impl
{
public:

	void serialize(std::wostream & strm) const
    {
        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"comments")
            {
                CP_XML_ATTR(L"xmlns", L"http://schemas.openxmlformats.org/spreadsheetml/2006/main");

				CP_XML_NODE(L"authors")
				{
					BOOST_FOREACH(std::wstring const & a, author_list_)
					{
						CP_XML_NODE(L"author")
						{
							CP_XML_STREAM() << a;
						}
					}
				}

				CP_XML_NODE(L"commentList")
				{
					BOOST_FOREACH(_xlsx_comment const & c, xlsx_comment_)
					{
						CP_XML_NODE(L"comment")
						{
							CP_XML_ATTR(L"ref", c.ref_);
							CP_XML_ATTR(L"authorId", c.author_);
							
							CP_XML_NODE(L"text")
							{
								CP_XML_STREAM() << c.content_;
							}
						} 					
					}
				}

            }
		}
	}
 	void serialize_vml(std::wostream & strm) const
    {
		_CP_OPT(std::wstring) strVal;
		_CP_OPT(double) dVal;

        CP_XML_WRITER(strm)
        {
		CP_XML_NODE(L"xml")
		{
			CP_XML_ATTR(L"xmlns:v", L"urn:schemas-microsoft-com:vml");
			CP_XML_ATTR(L"xmlns:o", L"urn:schemas-microsoft-com:office:office");
			CP_XML_ATTR(L"xmlns:x", L"urn:schemas-microsoft-com:office:excel");
		

 
			BOOST_FOREACH(_xlsx_comment const & c, xlsx_comment_)
			{
				std::wstring str_fill_color_ = L"00003f";
				if (odf_reader::GetProperty(c.graphicProperties_,L"fill-color",strVal))
					str_fill_color_ = strVal.get();
				
				CP_XML_NODE(L"v:shapetype")
	            {
	                CP_XML_ATTR(L"id", L"shapetype_202");
	                CP_XML_ATTR(L"coordsize", L"21600,21600");
	                CP_XML_ATTR(L"o:spt", L"202");
	                CP_XML_ATTR(L"path", L"m,l,21600l21600,21600l21600,xe");

					CP_XML_NODE(L"v:stroke")
					{
						CP_XML_ATTR(L"joinstyle", L"miter");
					}
					CP_XML_NODE(L"v:path")
					{
						CP_XML_ATTR(L"gradientshapeok", L"t");				
						CP_XML_ATTR(L"o:connecttype", L"rect");
					}
				}				
				CP_XML_NODE(L"v:shape")
				{
					if( hex_string_to_int(str_fill_color_) !=0 )
					{
						CP_XML_ATTR(L"fillcolor", std::wstring(L"#") + str_fill_color_);
					}//иначе это полная прозрачность

					CP_XML_ATTR(L"id", L"shape_0");
					
					std::wstring style = std::wstring(L"position:absolute;");
					
					style += std::wstring(L"margin-left:")	+boost::lexical_cast<std::wstring>(c.left_)		+std::wstring(L"pt;");
					style += std::wstring(L"margin-top:")	+boost::lexical_cast<std::wstring>(c.top_)		+std::wstring(L"pt;");
					style += std::wstring(L"width:")		+boost::lexical_cast<std::wstring>(c.width_)	+std::wstring(L"pt;");
					style += std::wstring(L"height:")		+boost::lexical_cast<std::wstring>(c.height_)	+std::wstring(L"pt;");

					if (c.visibly_ == false)style += std::wstring(L"visibility:hidden;");

					CP_XML_ATTR(L"style",style);
					CP_XML_ATTR(L"type", L"shapetype_202");
					//if (odf_reader::GetProperty(c.graphicProperties_,L"opacity",dVal))
					//{
					//	CP_XML_ATTR(L"opacity",boost::lexical_cast<std::wstring>((int)(100.-dVal.get())) + L"%");
					//}
					//CP_XML_NODE(L"v:shadow")
					//{
					//	CP_XML_ATTR(L"color", L"black");
					//	CP_XML_ATTR(L"obscured", L"t");
					//	CP_XML_ATTR(L"on", L"t");
					//}

					CP_XML_NODE(L"v:wrap")
					{
						CP_XML_ATTR(L"v:type", L"none");
					}
					CP_XML_NODE(L"v:fill")
					{
						CP_XML_ATTR(L"detectmouseclick", L"t");
						if( hex_string_to_int(str_fill_color_) !=0 )
						{
							CP_XML_ATTR(L"color", std::wstring(L"#") + str_fill_color_);
							CP_XML_ATTR(L"color2", std::wstring(L"#") + str_fill_color_);
							CP_XML_ATTR(L"type", L"solid");
						}//иначе это полная прозрачность
						//if (odf_reader::GetProperty(c.graphicProperties_,L"opacity",dVal))
						//{
						//	CP_XML_ATTR(L"opacity", (dVal.get())/100.);
						//	//CP_XML_ATTR(L"opacity2",(dVal.get())/100.);
						//}		
					}

					CP_XML_NODE(L"v:stroke")
					{
						if (odf_reader::GetProperty(c.graphicProperties_,L"stroke-color",strVal))
							CP_XML_ATTR(L"color", std::wstring(L"#") + strVal.get());
						else
							CP_XML_ATTR(L"color",L"#3465af");	//синенький

						if (odf_reader::GetProperty(c.graphicProperties_,L"stroke-opacity",dVal))
						{
							CP_XML_ATTR(L"opacity",(100.-dVal.get())/100.);
						}
						CP_XML_ATTR(L"endcap", L"flat");
						CP_XML_ATTR(L"joinstyle", L"round");
						CP_XML_ATTR(L"startarrow", L"block");
						CP_XML_ATTR(L"v:startarrowwidth", L"medium");
						CP_XML_ATTR(L"startarrowlength", L"medium");
					}							
					CP_XML_NODE(L"x:ClientData")
					{
						CP_XML_ATTR(L"ObjectType", L"Note");
						CP_XML_NODE(L"x:MoveWithCells"){}
						CP_XML_NODE(L"x:SizeWithCells"){}
						CP_XML_NODE(L"x:AutoFill"){CP_XML_CONTENT("False");}
						CP_XML_NODE(L"x:Row"){CP_XML_CONTENT(c.row_);}
						CP_XML_NODE(L"x:Column"){CP_XML_CONTENT(c.col_);}
					}
				}
			}
		}
		}
			
	}
	bool empty() const
    {
        return ( xlsx_comment_.empty());
    }
    void add(_xlsx_comment & c)
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
		xlsx_comment_.push_back(c);
	}
private:
	std::vector<std::wstring> author_list_;
	std::vector<_xlsx_comment> xlsx_comment_;
};

xlsx_comments::xlsx_comments() : impl_( new xlsx_comments::Impl() )
{
}

xlsx_comments::~xlsx_comments()
{
}

void xlsx_comments::add(_xlsx_comment & c)
{
	impl_->add(c);
}
bool xlsx_comments::empty() const
{
    return impl_->empty();
}
void xlsx_serialize(std::wostream & _Wostream, xlsx_comments const & val)
{
	val.impl_->serialize(_Wostream);
}

void xlsx_serialize_vml(std::wostream & _Wostream, xlsx_comments const & val)
{
    val.impl_->serialize_vml(_Wostream);
} 

xlsx_comments_ptr xlsx_comments::create()
{
    return boost::make_shared<xlsx_comments>();
}

}
}


       