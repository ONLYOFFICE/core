/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "oox_rels.h"

#include <vector>
#include <xml/simple_xml_writer.h>

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
					for (size_t i = 0 ; i < author_list_.size(); i++)
					{
						const std::wstring & a = author_list_[i];
						CP_XML_NODE(L"author")
						{
							CP_XML_STREAM() << a;
						}
					}
				}

				CP_XML_NODE(L"commentList")
				{
					for (size_t i = 0 ; i < xlsx_comment_.size(); i++)
					{
						const _xlsx_comment & c = xlsx_comment_[i];

						CP_XML_NODE(L"comment")
						{
							CP_XML_ATTR(L"ref", c.ref_);
							CP_XML_ATTR(L"authorId", c.author_);
							CP_XML_ATTR(L"shapeId", 0);
							
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
	bool empty() const
    {
        return ( xlsx_comment_.empty());
    }
    void add(_xlsx_comment & c)
    {
		bool find = false;
		for (size_t i = 0; i < author_list_.size(); i++)
		{		
			if (c.author_ == author_list_[i])
			{
				find=true;
				c.author_ =  std::to_wstring(i);
				break;
			}
		}
		if (!find)
		{
			author_list_.push_back(c.author_);
			c.author_ = std::to_wstring(author_list_.size()-1);
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

xlsx_comments_ptr xlsx_comments::create()
{
    return boost::make_shared<xlsx_comments>();
}

}
}


       