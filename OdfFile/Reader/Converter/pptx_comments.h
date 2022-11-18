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
