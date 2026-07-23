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

#include <iosfwd>
#include <CPScopedPtr.h>
#include <CPSharedPtr.h>
#include <xml/attributes.h>
#include "mediaitems.h"

namespace cpdoccore {
namespace oox {

	struct _pptx_comment
	{
		int x_, y_;

		int author_id_;
		int idx_;
		
		std::wstring  date_;

		std::wstring  content_;

		std::vector<odf_reader::_property> graphicProperties_;
	};
//class rels;

class pptx_comments;
typedef _CP_PTR(pptx_comments) pptx_comments_ptr;

struct pptx_comment_elm
{
    pptx_comment_elm(std::wstring const & _filename, std::wstring const & _content, pptx_comments_ptr _comments)
        : filename(_filename), content(_content), comments(_comments)
    {}
	pptx_comments_ptr comments;
    
    std::wstring filename;
    std::wstring content;
};

class pptx_comments
{
public:
    pptx_comments();
    ~pptx_comments();
    static pptx_comments_ptr create();

public:
    void add(_pptx_comment & d);
    bool empty() const;

	friend void pptx_serialize(std::wostream & _Wostream, pptx_comments & val);    

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
    
};

}
}
