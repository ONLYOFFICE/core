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

#include "xlsx_comments_context.h"
#include "xlsx_table_metrics.h"
#include <iostream>
#include "../../DataTypes/length.h"
#include "xlsx_utils.h"


namespace cpdoccore { namespace oox {

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context_handle::Impl
{
public:
    Impl() :  next_comments_id_(1), next_file_id_(1)
    {
    }  

    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, xlsx_comments_ptr comments)
    {
 		const std::wstring file_id = std::to_wstring(next_file_id_++);      
		const std::wstring fileName = std::wstring(L"comments") + file_id + L".xml";
        
		comments_.push_back(comment_elm(fileName, content, comments));
        
		const std::wstring id = std::to_wstring(next_comments_id_++);
		const std::wstring rId = std::wstring(L"comId") + id; 
        return std::pair<std::wstring, std::wstring>(fileName, rId);
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
std::pair<std::wstring, std::wstring> xlsx_comments_context_handle::add_comments_xml(std::wstring const & content, xlsx_comments_ptr comments)
{
    return impl_->add_comments_xml(content, comments);
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

    void serialize(std::wostream & strm)
    {
        xlsx_serialize(strm, *xlsx_comments_);
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

void xlsx_comments_context::start_comment(const std::wstring & ref)
{
	impl_->current_.ref_ = ref;
}
void xlsx_comments_context::add_content(const std::wstring & content)
{
	impl_->current_.content_ = content;
}
void xlsx_comments_context::add_author(const std::wstring & author)
{
	impl_->current_.author_ = author;
}
void xlsx_comments_context::end_comment()
{
	impl_->add_comment(impl_->current_);
}

bool xlsx_comments_context::empty() const
{
    return impl_->empty();
}

void xlsx_comments_context::serialize(std::wostream & strm)
{
    impl_->serialize(strm);    
}
xlsx_comments_ptr xlsx_comments_context::get_comments()
{
    return impl_->get_comments();
}

}
}
