/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "xlsx_comments.h"

#include <vector>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>

#include <simple_xml_writer.h>
#include "oox_rels.h"


struct sort_
{
	inline bool operator() (const oox::_xlsx_comment& c1, const oox::_xlsx_comment& c2)
    {
		if (c1.row_ < c2.row_) return true;
		if (c1.row_ > c2.row_) return false;
		
		if (c1.col_ < c2.col_) return true;

        return false;
    }
};

namespace oox {

class xlsx_comments::Impl
{
public:

	void serialize(std::wostream & strm)
    {
		//std::sort(xlsx_comment_.begin(), xlsx_comment_.end(), sort_()); - излишне
        
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
			
			CP_XML_NODE(L"v:shapetype")
            {
                CP_XML_ATTR(L"id"		, L"_x0000_t202");
                CP_XML_ATTR(L"coordsize", L"21600,21600");
                CP_XML_ATTR(L"o:spt"	, L"202");
                CP_XML_ATTR(L"path"		, L"m,l,21600r21600,l21600,xe");

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

			int count = 0;

			BOOST_FOREACH(_xlsx_comment const & c, xlsx_comment_)
			{
				CP_XML_NODE(L"v:shape")
				{
					CP_XML_ATTR(L"fillcolor", std::wstring(L"#") + c.fill_);

					CP_XML_ATTR(L"id", L"shape_" + boost::lexical_cast<std::wstring>(count++));
					
					std::wstring style = std::wstring(L"position:absolute;");
					
					//todooo сделать "покороче" значения .. достаточно 2 знаков после запятой
					style += std::wstring(L"margin-left:")	+ boost::lexical_cast<std::wstring>(c.left_)	+ std::wstring(L"pt;");
					style += std::wstring(L"margin-top:")	+ boost::lexical_cast<std::wstring>(c.top_)		+ std::wstring(L"pt;");
					style += std::wstring(L"width:")		+ boost::lexical_cast<std::wstring>(c.width_)	+ std::wstring(L"pt;");
					style += std::wstring(L"height:")		+ boost::lexical_cast<std::wstring>(c.height_)	+ std::wstring(L"pt;");
					style += std::wstring(L"z-index:")		+ boost::lexical_cast<std::wstring>(count)		+ std::wstring(L";");


					if (c.visibly_ == false) style += std::wstring(L"visibility:hidden;");

					CP_XML_ATTR(L"style",style);
					CP_XML_ATTR(L"type", L"_x0000_t202");
					//	CP_XML_ATTR(L"opacity",boost::lexical_cast<std::wstring>((int)(100.-dVal.get())) + L"%");
					CP_XML_NODE(L"v:shadow")
					{
						CP_XML_ATTR(L"color", L"black");
						CP_XML_ATTR(L"obscured", L"t");
						CP_XML_ATTR(L"on", L"t");
					}

					CP_XML_NODE(L"v:wrap")
					{
						CP_XML_ATTR(L"v:type", L"none");
					}
					CP_XML_NODE(L"v:fill")
					{
						CP_XML_ATTR(L"color"	, std::wstring(L"#") + c.fill_);
						//CP_XML_ATTR(L"color2"	, std::wstring(L"#") + c.fill_);
						CP_XML_ATTR(L"type"		, L"solid");
						//	CP_XML_ATTR(L"opacity", (dVal.get())/100.);
					}

					CP_XML_NODE(L"v:stroke")
					{
						CP_XML_ATTR(L"color", std::wstring(L"#") + c.line_);
						//	CP_XML_ATTR(L"opacity",(100.-dVal.get())/100.);
					}							
					CP_XML_NODE(L"x:ClientData")
					{
						CP_XML_ATTR(L"ObjectType", L"Note");
						CP_XML_NODE(L"x:MoveWithCells"){}
						CP_XML_NODE(L"x:SizeWithCells"){}
						if (!c.anchor_.empty())
						{
							CP_XML_NODE(L"x:Anchor"){CP_XML_CONTENT(c.anchor_);}
						}
						CP_XML_NODE(L"x:AutoFill")	{CP_XML_CONTENT("False");}
						CP_XML_NODE(L"x:Row")		{CP_XML_CONTENT(c.row_);}
						CP_XML_NODE(L"x:Column")	{CP_XML_CONTENT(c.col_);}
						if (c.visibly_) CP_XML_NODE(L"x:Visible");
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
void xlsx_comments::serialize(std::wostream & _Wostream)
{
	impl_->serialize(_Wostream);
}

void xlsx_comments::serialize_vml(std::wostream & _Wostream)
{
    impl_->serialize_vml(_Wostream);
} 

xlsx_comments_ptr xlsx_comments::create()
{
    return boost::make_shared<xlsx_comments>();
}

}



       