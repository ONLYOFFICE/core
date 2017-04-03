/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include "xlsx_drawings.h"
#include "xlsx_drawing.h"

#include <vector>
#include <cpdoccore/xml/simple_xml_writer.h>

#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawings::Impl
{
public:
    void add(_xlsx_drawing const & d, bool isInternal, std::wstring const & rid, std::wstring const & ref, RelsType type, bool sheet_rel )//объект
    {
        xlsx_drawings_.push_back(d);
		
		add (isInternal, rid, ref, type, sheet_rel);
        
		for (int i = 0 ; i < d.hlinks.size(); i++)
        {
			xlsx_drawing_rels_.push_back(_rel(false, d.hlinks[i].hId, d.hlinks[i].hRef, typeHyperlink));
		}
    }
    void add( bool isInternal, std::wstring const & rid, std::wstring const & ref, RelsType type, bool sheet_rel) //не объект
    {
		bool present = false;
		
		if (type == typeHyperlink)	isInternal = false;

		if (sheet_rel)
		{
			for (int i = 0 ; i < xlsx_sheet_rels_.size(); i++)
			{		
				if (xlsx_sheet_rels_[i].rid == rid && xlsx_sheet_rels_[i].ref == ref)
					present = true;
			}
			if (!present)
				xlsx_sheet_rels_.push_back	(_rel(isInternal, rid, ref, type));
		}
		else
		{
			for (int i = 0 ; i < xlsx_drawing_rels_.size(); i++)
			{		
				if (xlsx_drawing_rels_[i].rid == rid && xlsx_drawing_rels_[i].ref == ref)
					present = true;
			}
			if (!present)
				xlsx_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
    }
	
	void serialize(std::wostream & strm) 
    {
		if (inGroup)
		{
			for (int i = 0 ; i < xlsx_drawings_.size(); i++)
			{
				xlsx_drawings_[i].serialize(strm);
			}
		}
		else
		{
			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(L"xdr:wsDr")
				{
					CP_XML_ATTR(L"xmlns:xdr", L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
					CP_XML_ATTR(L"xmlns:a"	, L"http://schemas.openxmlformats.org/drawingml/2006/main");
					CP_XML_ATTR(L"xmlns:r"	, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");

					for (int i = 0 ; i < xlsx_drawings_.size(); i++)
					{
						xlsx_drawings_[i].serialize(CP_XML_STREAM());
					}
				}
			}
		}
    }
	void serialize_objects(std::wostream & strm) 
    {
		for (int i = 0 ; i < xlsx_drawings_.size(); i++)
		{
			if (xlsx_drawings_[i].type != typeOleObject && xlsx_drawings_[i].type != typeMsObject) continue;

			xlsx_drawings_[i].serialize_object(strm);
		}
    }
    
    bool empty() const
    {
        return (xlsx_drawings_.empty());
    }

    void dump_rels_drawing(rels & Rels)
    {
        for (int i = 0 ; i < xlsx_drawing_rels_.size(); i++)
        {
			if (xlsx_drawing_rels_[i].type == typeImage		|| 
				xlsx_drawing_rels_[i].type == typeMedia		||
				xlsx_drawing_rels_[i].type == typeChart		||
				xlsx_drawing_rels_[i].type == typeHyperlink  )
			{

				Rels.add(relationship( xlsx_drawing_rels_[i].rid,
							mediaitems::get_rel_type(xlsx_drawing_rels_[i].type),
							(xlsx_drawing_rels_[i].is_internal ? std::wstring(L"../") + xlsx_drawing_rels_[i].ref : xlsx_drawing_rels_[i].ref),
							(xlsx_drawing_rels_[i].is_internal ? L"" : L"External")) );
			}
		}
    }
    void dump_rels_sheet(rels & Rels)
    {
        for (int i = 0 ; i < xlsx_sheet_rels_.size(); i++)
        {
			Rels.add(relationship( xlsx_sheet_rels_[i].rid,
						mediaitems::get_rel_type(xlsx_sheet_rels_[i].type),
						(xlsx_sheet_rels_[i].is_internal ? std::wstring(L"../") + xlsx_sheet_rels_[i].ref : xlsx_sheet_rels_[i].ref),
						(xlsx_sheet_rels_[i].is_internal ? L"" : L"External")) );
		}
    }
	bool inGroup;

private:

	std::vector<_xlsx_drawing>	xlsx_drawings_;	
	
	std::vector<_rel>			xlsx_drawing_rels_;
	std::vector<_rel>			xlsx_sheet_rels_;
};

xlsx_drawings::xlsx_drawings(bool inGroup_) : impl_( new xlsx_drawings::Impl() )
{
	impl_->inGroup = inGroup_;
}

xlsx_drawings::~xlsx_drawings()
{
}

void xlsx_drawings::add(_xlsx_drawing const & d, bool isInternal, std::wstring const & rid,
															std::wstring const & ref, RelsType type, bool sheet_rel)
{
    impl_->add(d, isInternal, rid, ref, type, sheet_rel);
}

void xlsx_drawings::add( bool isInternal, std::wstring const & rid, std::wstring const & ref, RelsType type, bool sheet_rel)
{
    impl_->add(isInternal, rid, ref, type, sheet_rel);
}

void xlsx_drawings::serialize(std::wostream & strm)
{
    impl_->serialize(strm);
}

void xlsx_drawings::serialize_objects(std::wostream & strm)
{
    impl_->serialize_objects(strm);
}

bool xlsx_drawings::empty() const
{
    return impl_->empty();
}

void xlsx_drawings::dump_rels_drawing(rels & Rels)
{
    return impl_->dump_rels_drawing(Rels);
}

void xlsx_drawings::dump_rels_sheet(rels & Rels)
{
	return impl_->dump_rels_sheet(Rels);
}

xlsx_drawings_ptr xlsx_drawings::create(bool inGroup)
{
    return boost::make_shared<xlsx_drawings>(inGroup);
}

}
}

