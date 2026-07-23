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

#include "../../Common/Utils/OptPtr.h"

#include "xlsx_comments.h"

namespace oox {

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context_handle
{
public:
    xlsx_comments_context_handle	();
    ~xlsx_comments_context_handle	();
    
    std::pair<std::wstring, std::wstring> add_comments_xml(std::wstring const & content, xlsx_comments_ptr comments);
    const std::vector<comment_elm> & content() const;

    friend class xlsx_comments_context;
private:
    class Impl;
    _CP_PTR(Impl) impl_;
};

class xlsx_comments;
typedef _CP_PTR(xlsx_comments) xlsx_comments_ptr;

class xlsx_comments_context
{
public:
    xlsx_comments_context(xlsx_comments_context_handle & h);
    ~xlsx_comments_context();

    void start_comment();

	int get_id();

	void set_content		(std::wstring  content);
	void set_author			(std::wstring  author);
	void set_ref			(std::wstring ref, int col, int row);
	void set_visibly		(bool Val);
      
	void end_comment();

    bool empty() const;

    void write_comments		(std::wostream & strm);

    xlsx_comments_ptr get_comments();

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
