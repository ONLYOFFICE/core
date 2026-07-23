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

#include "pptx_comments.h"
#include <vector>
#include <xml/simple_xml_writer.h>
#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class pptx_comments::Impl
{
public:

	void serialize(std::wostream & strm) const
    {
        CP_XML_WRITER(strm)
        {
            CP_XML_NODE(L"p:cmLst")
            {
                CP_XML_ATTR(L"xmlns:a", L"http://schemas.openxmlformats.org/drawingml/2006/main");
                CP_XML_ATTR(L"xmlns:r", L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
                CP_XML_ATTR(L"xmlns:p", L"http://schemas.openxmlformats.org/presentationml/2006/main");

				for (size_t i = 0; i < pptx_comment_.size(); i++)
				{
					CP_XML_NODE(L"p:cm")
					{
						CP_XML_ATTR(L"idx", pptx_comment_[i].idx_);
						
						CP_XML_ATTR(L"authorId", pptx_comment_[i].author_id_);
						CP_XML_ATTR(L"dt",	pptx_comment_[i].date_);
						
						CP_XML_NODE(L"p:pos")
						{
							CP_XML_ATTR(L"x", pptx_comment_[i].x_);
							CP_XML_ATTR(L"y", pptx_comment_[i].y_);
						}
						CP_XML_NODE(L"p:text")
						{
							CP_XML_STREAM() << pptx_comment_[i].content_;
						}
					} 					
				}
			}
		}
	}
	bool empty() const
    {
        return ( pptx_comment_.empty());
    }
    void add(_pptx_comment & c)
    {
		pptx_comment_.push_back(c);
	}
private:
	std::vector<_pptx_comment> pptx_comment_;
};

pptx_comments::pptx_comments() : impl_( new pptx_comments::Impl() )
{
}

pptx_comments::~pptx_comments()
{
}

void pptx_comments::add(_pptx_comment & c)
{
	impl_->add(c);
}
bool pptx_comments::empty() const
{
    return impl_->empty();
}
void pptx_serialize(std::wostream & _Wostream, pptx_comments & val)
{
	val.impl_->serialize(_Wostream);
}

pptx_comments_ptr pptx_comments::create()
{
    return boost::make_shared<pptx_comments>();
}

}
}


       