/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <boost/foreach.hpp>
#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "mediaitems_utils.h"
#include "oox_rels.h"

#include "pptx_drawings.h"
#include "pptx_drawing.h"

namespace cpdoccore {
namespace oox {

class pptx_drawings::Impl
{


public:
    void add(_pptx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		RelsType type)
    {
        pptx_drawings_.push_back(d);
		
		bool present = false;
        BOOST_FOREACH(_rel const & r, pptx_drawing_rels_)
        {		
			if (r.rid == rid && r.ref == ref)
				present = true;
		}
		if (!present)
		{
			pptx_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
        BOOST_FOREACH(_hlink_desc h, d.hlinks)
        {
			pptx_drawing_rels_.push_back(_rel(false, h.hId, h.hRef, typeHyperlink));
		}
    }

    void add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
		RelsType type)
    {
	
		bool present = false;
        BOOST_FOREACH(_rel const & r, pptx_drawing_rels_)
        {		
			if (r.rid == rid && r.ref == ref)
				present = true;
		}
		if (!present)
		{
			pptx_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
    }
	void serialize(std::wostream & strm)
	{
		BOOST_FOREACH(_pptx_drawing & d, pptx_drawings_)
		{
			pptx_serialize(strm,d);
		}
	}

    bool empty() const
    {
        return (pptx_drawings_.empty());
    }

    void dump_rels(rels & Rels)
    {
        BOOST_FOREACH(_rel const & r, pptx_drawing_rels_)
        {
			if (r.type == typeChart)//временно - нужно потом все загнать в релс
			{
				Rels.add(relationship(
							r.rid,
							utils::media::get_rel_type(r.type),
							(r.is_internal ? std::wstring(L"../") + r.ref : r.ref),
							(r.is_internal ? L"" : L"External")
							) 
					);
			}
			else if (r.type == typeImage)
			{
				Rels.add(relationship(
							r.rid,
							utils::media::get_rel_type(r.type),
							r.is_internal ? std::wstring(L"../") + r.ref : r.ref,
							(r.is_internal ? L"" : L"External")
							) 
					);
			}
 			else if (r.type == typeHyperlink)
			{
				Rels.add(relationship(
							r.rid,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink",
							r.ref,
							L"External")
				);
			}
			else if (r.type == typeComment)
			{
				Rels.add(relationship(
							r.rid,
							L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments",
							r.ref)
				);
			}
		}
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
        RelsType type)
{
    impl_->add(d, isInternal, rid, ref, type);
}
void pptx_drawings::add(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        RelsType type)
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

pptx_drawings_ptr pptx_drawings::create()
{
    return boost::make_shared<pptx_drawings>();
}

}
}

