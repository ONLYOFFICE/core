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

#include "pptx_comments_context.h"
#include <iostream>
#include "../../DataTypes/length.h"
#include "xlsx_utils.h"

//#include <formulasconvert.h>

namespace cpdoccore { 
namespace oox {

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;

class pptx_comments_context_handle::Impl
{
public:
    Impl()
        :  next_comments_id_(1) ,next_file_id_(1)
    {
    }  

    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, pptx_comments_ptr comments)
    {
 		const std::wstring file_id = std::to_wstring(next_file_id_++);
      
		const std::wstring fileName = std::wstring(L"comment") + file_id + L".xml";
        
		comments_.push_back(pptx_comment_elm(fileName, content, comments));
        
		const std::wstring id = std::to_wstring(next_comments_id_++);
		const std::wstring rId = std::wstring(L"comId") + id; 
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    const std::vector<pptx_comment_elm> & content() const
    {
        return comments_;
    }

private:
   
    std::vector<pptx_comment_elm> comments_;

    size_t next_comments_id_;
    size_t next_file_id_;
};

pptx_comments_context_handle::pptx_comments_context_handle()
: impl_(new pptx_comments_context_handle::Impl())
{
}

pptx_comments_context_handle::~pptx_comments_context_handle()
{
}

std::pair<std::wstring, std::wstring> pptx_comments_context_handle::add_comments_xml(std::wstring const & content, pptx_comments_ptr comments)
{
    return impl_->add_comments_xml(content,comments);
}

const std::vector<pptx_comment_elm> & pptx_comments_context_handle::content() const
{
    return impl_->content();
}

class pptx_comments_context::Impl
{
public:    
    Impl(pptx_comments_context_handle & handle) : pptx_comments_(pptx_comments::create()),
        handle_(handle) 
    {} 

    pptx_comments_context_handle & handle_;
    _pptx_comment current_;    

    void add_comment(_pptx_comment & d)
    {
        pptx_comments_->add(d);
    }

    void serialize(std::wostream & strm)
    {
        pptx_serialize(strm, *pptx_comments_);
    }
    bool empty() const
    {
        return pptx_comments_->empty();
    }

    pptx_comments_ptr get_comments()
    {
        return pptx_comments_;
    }

	void clear()
	{
		pptx_comments_ = pptx_comments::create();
	}
private:
    pptx_comments_ptr pptx_comments_;
};


pptx_comments_context::pptx_comments_context(pptx_comments_context_handle & h)
 : impl_(new pptx_comments_context::Impl(h))
{    
}

pptx_comments_context::~pptx_comments_context()
{
}

void pptx_comments_context::start_comment (double x_emu, double y_emu,int id_author, int idx_comment)
{
    impl_->current_.x_ = (int)(x_emu + 0.5);
    impl_->current_.y_ = (int)(y_emu + 0.5); 
	
	impl_->current_.author_id_ = id_author;
	impl_->current_.idx_ = idx_comment;
}
void pptx_comments_context::add_content(std::wstring  content)
{
	impl_->current_.content_ = content;
}

void pptx_comments_context::add_date(std::wstring  Val)
{
	impl_->current_.date_ = Val;
}

std::vector<odf_reader::_property> & pptx_comments_context::get_draw_properties()
{
	return impl_->current_.graphicProperties_;

}
void pptx_comments_context::end_comment()
{
	impl_->add_comment(impl_->current_);
}

bool pptx_comments_context::empty() const
{
    return impl_->empty();
}

void pptx_comments_context::serialize(std::wostream & strm)
{
    impl_->serialize(strm);    
	impl_->clear();
}
pptx_comments_ptr pptx_comments_context::get_comments()
{
    return impl_->get_comments();
}

}
}
