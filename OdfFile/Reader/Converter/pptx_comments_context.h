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
