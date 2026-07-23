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

#include <iostream>

namespace oox {

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context_handle::Impl
{
public:
    Impl() :  next_comments_id_(1) , next_comments_file_id_(1)
    {
    }  

    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, xlsx_comments_ptr comments)
    {
 		const std::wstring file_comments_id	= std::to_wstring(next_comments_file_id_++);
     
		const std::wstring fileName		= std::wstring(L"comments")		+ file_comments_id	+ L".xml";
        
		comments_.push_back(comment_elm(fileName, content, comments));
        
		const std::wstring id	= std::to_wstring(next_comments_id_++);
		const std::wstring rId	= std::wstring(L"comId") + id; 
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    const std::vector<comment_elm> & content() const
    {
        return comments_;
    }

private:
   
    std::vector<comment_elm> comments_;

    size_t next_comments_id_;
    size_t next_comments_file_id_;
	
};

xlsx_comments_context_handle::xlsx_comments_context_handle(/*size_t & next_vml_file_id*/)
											: impl_(new xlsx_comments_context_handle::Impl(/*next_vml_file_id*/))
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
        handle_(handle) , count_comments_(0)
    {} 

    xlsx_comments_context_handle &	handle_;
    _xlsx_comment					current_;    

    void add_comment(_xlsx_comment & d)
    {
        xlsx_comments_->add(d);
		count_comments_++;
    }

    void write_comments(std::wostream & strm)
    {
        xlsx_comments_->serialize(strm);
    }
    bool empty() const
    {
        return xlsx_comments_->empty();
    }

    xlsx_comments_ptr get_comments()
    {
        return xlsx_comments_;
    }
	int get_count_comments() {return count_comments_;}
private:
    xlsx_comments_ptr	xlsx_comments_;
	int					count_comments_;
};


xlsx_comments_context::xlsx_comments_context(xlsx_comments_context_handle & h)
 : impl_(new xlsx_comments_context::Impl(h))
{    
}

xlsx_comments_context::~xlsx_comments_context()
{
}

void xlsx_comments_context::start_comment ()
{
    impl_->current_.ref_		= L"";
	impl_->current_.visibly_	= false;

}

int xlsx_comments_context::get_id()
{
	return impl_->get_count_comments() + 1;
}

void xlsx_comments_context::set_content(std::wstring  content)
{
	impl_->current_.content_ = content;
}
void xlsx_comments_context::set_author(std::wstring  author)
{
	impl_->current_.author_ = author;
}
void xlsx_comments_context::set_visibly(bool Val)
{
	impl_->current_.visibly_ = Val;
}

void xlsx_comments_context::set_ref(std::wstring ref, int col, int row)
{
	impl_->current_.ref_ = ref;
	
	impl_->current_.col_ = col;
	impl_->current_.row_ = row;
}

void xlsx_comments_context::end_comment()
{
	if (impl_->current_.content_.empty() || impl_->current_.ref_.empty()) return;

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

xlsx_comments_ptr xlsx_comments_context::get_comments()
{
    return impl_->get_comments();
}

}
