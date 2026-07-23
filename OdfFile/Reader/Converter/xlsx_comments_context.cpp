/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
