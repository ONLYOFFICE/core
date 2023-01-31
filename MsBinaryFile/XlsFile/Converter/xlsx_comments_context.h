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
