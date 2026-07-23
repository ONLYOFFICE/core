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

#include <xml/simple_xml_writer.h>
#include <boost/algorithm/string.hpp>

#include "oox_rels.h"

#include "pptx_drawings.h"

namespace cpdoccore {
namespace oox {

class pptx_drawings::Impl
{


public:
    void add(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		_rels_type type)
    {
        pptx_drawings_.push_back(d);
		
		bool present = false;
        for (size_t i = 0; i < pptx_drawing_rels_.size(); i++)
        {		
			if (pptx_drawing_rels_[i].rid == rid && pptx_drawing_rels_[i].ref == ref)
				present = true;
		}
		if (!present)
		{
			pptx_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
        for (size_t i = 0; i < d.hlinks.size(); i++)
        {
			bool found = false;
			for (size_t j = 0; j < pptx_drawing_rels_.size(); j++)
			{
				if (d.hlinks[i].hId == pptx_drawing_rels_[j].rid)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				oox::_rels_type type = boost::algorithm::starts_with(d.hlinks[i].hRef, L"slide") &&
					boost::algorithm::ends_with(d.hlinks[i].hRef, L".xml") ? typeSlide : typeHyperlink;
				pptx_drawing_rels_.push_back(_rel(false, d.hlinks[i].hId, d.hlinks[i].hRef, type));
			}
				
		}
        if (!d.action.hId.empty())
        {
			bool found = false;
			for (size_t i = 0; i < pptx_drawing_rels_.size(); i++)
			{
				if (pptx_drawing_rels_[i].rid == d.action.hId)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				bool bInternal = (d.action.typeRels != typeHyperlink);
				pptx_drawing_rels_.push_back(_rel(bInternal, d.action.hId, d.action.hRef, d.action.typeRels));
			}
		}    
	}

    void add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		_rels_type type)
    {
	
		bool present = false;
        for (size_t i = 0; i < pptx_drawing_rels_.size(); i++)
        {		
			if (pptx_drawing_rels_[i].rid == rid && pptx_drawing_rels_[i].ref == ref)
				present = true;
		}
		if (!present)
		{
			if (type == typeHyperlink)	isInternal = false;
			pptx_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
    }
	void serialize(std::wostream & strm)
	{
		for (size_t i = 0; i < pptx_drawings_.size(); i++)
		{
			pptx_drawings_[i].serialize(strm);
		}
	}

    bool empty() const
    {
        return (pptx_drawings_.empty());
    }

    void dump_rels(rels & Rels)
    {
        for (size_t i = 0; i < pptx_drawing_rels_.size(); i++)
        {
			if (pptx_drawing_rels_[i].type == typeUnknown		|| 
				pptx_drawing_rels_[i].type == typeTable			||
				pptx_drawing_rels_[i].type == typeShape			||
				pptx_drawing_rels_[i].type == typeGroupShape || 
				pptx_drawing_rels_[i].type == typeControl)	continue;

			if (pptx_drawing_rels_[i].type == typeComment)
			{
				Rels.add(relationship( pptx_drawing_rels_[i].rid,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments",
							pptx_drawing_rels_[i].ref) );
			}
			else
			{
				std::wstring ref = pptx_drawing_rels_[i].ref;
				if (pptx_drawing_rels_[i].is_internal && ref != L"NULL")
				{
					ref =  L"../" + ref;
				}
				Rels.add(relationship( pptx_drawing_rels_[i].rid, 
							mediaitems::get_rel_type(pptx_drawing_rels_[i].type), ref, (pptx_drawing_rels_[i].is_internal ? L"" : L"External")) );
			}
		}
	}

	std::vector<_pptx_drawing>& get_drawings()
	{
		return pptx_drawings_;
	}

private:

	std::vector<_pptx_drawing>	pptx_drawings_;	
	std::vector<_rel>			pptx_drawing_rels_;
};

pptx_drawings::pptx_drawings() : impl_( new pptx_drawings::Impl() )
{
}

pptx_drawings::~pptx_drawings()
{
}

void pptx_drawings::add(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        _rels_type type)
{
    impl_->add(d, isInternal, rid, ref, type);
}
void pptx_drawings::add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        _rels_type type)
{
    impl_->add(isInternal, rid, ref, type);
}
void pptx_drawings::serialize(std::wostream & _Wostream)
{
	impl_->serialize(_Wostream);
}

bool pptx_drawings::empty() const
{
    return impl_->empty();
}

void pptx_drawings::dump_rels(rels & Rels)
{
    return impl_->dump_rels(Rels);
}

std::vector<_pptx_drawing>& pptx_drawings::get_drawings()
{
	return impl_->get_drawings();
}

pptx_drawings_ptr pptx_drawings::create()
{
    return boost::make_shared<pptx_drawings>();
}

}
 }

