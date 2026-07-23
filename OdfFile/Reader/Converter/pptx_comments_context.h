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
#pragma once


#include <string>
#include <boost/noncopyable.hpp>

#include "pptx_comments.h"

namespace cpdoccore { namespace oox {

class pptx_table_metrics;

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;



class pptx_comments_context_handle
{
public:
    pptx_comments_context_handle();
    ~pptx_comments_context_handle();
    
    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, pptx_comments_ptr comments);
    const std::vector<pptx_comment_elm> & content() const;

    friend class pptx_comments_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;

class pptx_comments_context
{
public:
    pptx_comments_context(pptx_comments_context_handle & h);
    ~pptx_comments_context();

    void start_comment(double x_emu, double y_emu,int id_author, int idx_comment);

	void add_content	(std::wstring  content);
	void add_date		(std::wstring date);
	
	std::vector<odf_reader::_property> & get_draw_properties();
    
	void end_comment();

    bool empty() const;

    void serialize(std::wostream & strm);
    
	pptx_comments_ptr get_comments();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}
