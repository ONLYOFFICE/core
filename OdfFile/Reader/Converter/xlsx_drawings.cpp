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

#include "xlsx_drawings.h"
#include "xlsx_drawing.h"

#include <vector>
#include <xml/simple_xml_writer.h>

#include "oox_rels.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawings::Impl
{
public:
    void add(_xlsx_drawing & d, bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel )//объект
    {
		if ( type == typeControlProps || type == typeComment)
		{
			vml_drawings_.push_back(d);
			add (isInternal, rid, ref, type, sheet_rel, true);
		}
		else if (type == typeControl)
		{
			vml_drawings_.push_back(d);
			add (isInternal, rid, ref, type, sheet_rel, true);

			d.hidden = true;
			drawings_.push_back(d);
		}
		else
		{
			drawings_.push_back(d);
			add (isInternal, rid, ref, type, sheet_rel, false);
			
			for (size_t i = 0; i < d.hlinks.size(); i++)
			{
				drawing_rels_.push_back(_rel(false, d.hlinks[i].hId, d.hlinks[i].hRef, typeHyperlink));
			}
			if (!d.action.hId.empty())
 				drawing_rels_.push_back(_rel(false, d.action.hId, d.action.hRef, d.action.typeRels));	
		}
    }
    void add( bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel, bool vml_rel) //не объект
    {
		bool present = false;
		
		if (type == typeHyperlink)	isInternal = false;

		if (sheet_rel)
		{
			for (size_t i = 0 ; i < xlsx_sheet_rels_.size(); i++)
			{		
				if (xlsx_sheet_rels_[i].rid == rid && xlsx_sheet_rels_[i].ref == ref)
					present = true;
			}
			if (!present)
				xlsx_sheet_rels_.push_back	(_rel(isInternal, rid, ref, type));
		}
		else if (vml_rel)
		{
			for (size_t i = 0 ; i < vml_drawing_rels_.size(); i++)
			{		
				if (vml_drawing_rels_[i].rid == rid && vml_drawing_rels_[i].ref == ref)
					present = true;
			}
			if (!present)
				vml_drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
		else
		{
			for (size_t i = 0 ; i < drawing_rels_.size(); i++)
			{		
				if (drawing_rels_[i].rid == rid && drawing_rels_[i].ref == ref)
					present = true;
			}
			if (!present)
				drawing_rels_.push_back(_rel(isInternal, rid, ref, type));
		}
    }
	
	void serialize(std::wostream & strm, const std::wstring & ns) 
    {
		if (drawings_.empty()) return;

		if (inGroup)
		{
			for (size_t i = 0 ; i < drawings_.size(); i++)
			{
				drawings_[i].serialize(strm, ns);
			}
		}
		else
		{
			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(ns + L":wsDr")
				{
					if (ns == L"xdr")
					{
						CP_XML_ATTR(L"xmlns:xdr", L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing");
					}
					if (ns == L"cdr")
					{
						CP_XML_ATTR(L"xmlns:cdr", L"http://schemas.openxmlformats.org/drawingml/2006/chartDrawing");
					}
					CP_XML_ATTR(L"xmlns:a"	, L"http://schemas.openxmlformats.org/drawingml/2006/main");
					CP_XML_ATTR(L"xmlns:r"	, L"http://schemas.openxmlformats.org/officeDocument/2006/relationships");
					CP_XML_ATTR(L"xmlns:a14", L"http://schemas.microsoft.com/office/drawing/2010/main");

					for (size_t i = 0 ; i < drawings_.size(); i++)
					{
						drawings_[i].serialize(CP_XML_STREAM(), ns);
					}
				}
			}
		}
    }
	void serialize_vml(std::wostream & strm) 
    {
		if (vml_drawings_.empty()) return;

		if (inGroup)
		{
			for (size_t i = 0 ; i < vml_drawings_.size(); i++)
			{
				vml_drawings_[i].serialize_vml(strm);
			}
		}
		else
		{
			CP_XML_WRITER(strm)
			{
				CP_XML_NODE(L"xml")
				{
					CP_XML_ATTR(L"xmlns:v", L"urn:schemas-microsoft-com:vml");
					CP_XML_ATTR(L"xmlns:o", L"urn:schemas-microsoft-com:office:office");
					CP_XML_ATTR(L"xmlns:x", L"urn:schemas-microsoft-com:office:excel");
					
					CP_XML_NODE(L"v:shapetype")
					{
						CP_XML_ATTR(L"id", L"_x0000_t201");   
						CP_XML_ATTR(L"o:spt", L"201");		// sptHostControl = 201

						CP_XML_ATTR(L"coordsize", L"21600,21600");
						CP_XML_ATTR(L"path", L"m,l,21600r21600,l21600,xe");

						CP_XML_NODE(L"v:stroke")
						{
							CP_XML_ATTR(L"joinstyle", L"miter");
						}
						CP_XML_NODE(L"v:path")
						{
							CP_XML_ATTR(L"shadowok", L"f");
							CP_XML_ATTR(L"o:extrusionok", L"f");
							CP_XML_ATTR(L"strokeok", L"f");
							CP_XML_ATTR(L"fillok", L"f");
							CP_XML_ATTR(L"o:connecttype", L"rect");
						}
						CP_XML_NODE(L"o:lock")
						{
							CP_XML_ATTR(L"v:ext", L"edit");
							CP_XML_ATTR(L"shapetype", L"t");
						}
					}
					CP_XML_NODE(L"v:shapetype")
					{
						CP_XML_ATTR(L"id", L"_x0000_t202");
						CP_XML_ATTR(L"o:spt", L"202");		// sptTextBox = 202

						CP_XML_ATTR(L"coordsize", L"21600,21600");
						CP_XML_ATTR(L"path", L"m,l,21600l21600,21600l21600,xe");

						CP_XML_NODE(L"v:stroke")
						{
							CP_XML_ATTR(L"joinstyle", L"miter");
						}
						CP_XML_NODE(L"v:path")
						{
							CP_XML_ATTR(L"gradientshapeok", L"t");				
							CP_XML_ATTR(L"o:connecttype", L"rect");
						}
					}
					for (size_t i = 0 ; i < vml_drawings_.size(); i++)
					{
						vml_drawings_[i].serialize_vml(CP_XML_STREAM());
					}
				}
			}
		}
    }
	void serialize_objects(std::wostream & strm) 
    {
		for (size_t i = 0 ; i < drawings_.size(); i++)
		{
			if (drawings_[i].type != typeOleObject && drawings_[i].type != typeMsObject) continue;

			drawings_[i].serialize_object(strm);
		}
    }
	void serialize_controls(std::wostream & strm) 
    {
		for (size_t i = 0 ; i < vml_drawings_.size(); i++)
		{
			if (vml_drawings_[i].type != typeControl) continue;

			vml_drawings_[i].serialize_control(strm);
		}
    }    
    bool empty() const
    {
        return (drawings_.empty());
    }
    bool vml_empty() const
    {
        return (vml_drawings_.empty());
    }

    void dump_rels_drawing(rels & Rels)
    {
        for (size_t i = 0 ; i < drawing_rels_.size(); i++)
        {
			if (drawing_rels_[i].type == typeImage		|| 
				drawing_rels_[i].type == typeMedia		||
				drawing_rels_[i].type == typeChart		||
				drawing_rels_[i].type == typeHyperlink  )
			{

				Rels.add(relationship( drawing_rels_[i].rid,
							mediaitems::get_rel_type(drawing_rels_[i].type),
							(drawing_rels_[i].is_internal ? std::wstring(L"../") + drawing_rels_[i].ref : drawing_rels_[i].ref),
							(drawing_rels_[i].is_internal ? L"" : L"External")) );
			}
		}
    }
    void dump_rels_vml_drawing(rels & Rels)
    {
        for (size_t i = 0 ; i < vml_drawing_rels_.size(); i++)
        {
			if (vml_drawing_rels_[i].type == typeImage		|| 
				vml_drawing_rels_[i].type == typeMedia		||
				vml_drawing_rels_[i].type == typeChart		||
				vml_drawing_rels_[i].type == typeHyperlink  )
			{

				Rels.add(relationship( vml_drawing_rels_[i].rid,
							mediaitems::get_rel_type(vml_drawing_rels_[i].type),
							(vml_drawing_rels_[i].is_internal ? std::wstring(L"../") + vml_drawing_rels_[i].ref : vml_drawing_rels_[i].ref),
							(vml_drawing_rels_[i].is_internal ? L"" : L"External")) );
			}
		}
    }
    void dump_rels_sheet(rels & Rels)
    {
        for (size_t i = 0 ; i < xlsx_sheet_rels_.size(); i++)
        {
			Rels.add(relationship( xlsx_sheet_rels_[i].rid,
						mediaitems::get_rel_type(xlsx_sheet_rels_[i].type),
						(xlsx_sheet_rels_[i].is_internal ? std::wstring(L"../") + xlsx_sheet_rels_[i].ref : xlsx_sheet_rels_[i].ref),
						(xlsx_sheet_rels_[i].is_internal ? L"" : L"External")) );
		}
    }
	bool inGroup;

private:

	std::vector<_xlsx_drawing>	drawings_;	
	std::vector<_xlsx_drawing>	vml_drawings_;	
	
	std::vector<_rel>			drawing_rels_;
	std::vector<_rel>			vml_drawing_rels_;
	std::vector<_rel>			xlsx_sheet_rels_;
};

xlsx_drawings::xlsx_drawings(bool inGroup_) : impl_( new xlsx_drawings::Impl() )
{
	impl_->inGroup = inGroup_;
}

xlsx_drawings::~xlsx_drawings()
{
}

void xlsx_drawings::add(_xlsx_drawing & d, bool isInternal, std::wstring const & rid,
															std::wstring const & ref, _rels_type type, bool sheet_rel)
{
    impl_->add(d, isInternal, rid, ref, type, sheet_rel);
}

void xlsx_drawings::add( bool isInternal, std::wstring const & rid, std::wstring const & ref, _rels_type type, bool sheet_rel, bool vml_rel)
{
    impl_->add(isInternal, rid, ref, type, sheet_rel, vml_rel);
}

void xlsx_drawings::serialize(std::wostream & strm, const std::wstring & ns)
{
    impl_->serialize(strm, ns);
}
void xlsx_drawings::serialize_vml(std::wostream & strm)
{
    impl_->serialize_vml(strm);
}

void xlsx_drawings::serialize_objects(std::wostream & strm)
{
    impl_->serialize_objects(strm);
}
void xlsx_drawings::serialize_controls(std::wostream & strm)
{
    impl_->serialize_controls(strm);
}

bool xlsx_drawings::empty() const
{
    return impl_->empty();
}
bool xlsx_drawings::vml_empty() const
{
    return impl_->vml_empty();
}
void xlsx_drawings::dump_rels_drawing(rels & Rels)
{
    return impl_->dump_rels_drawing(Rels);
}
void xlsx_drawings::dump_rels_vml_drawing(rels & Rels)
{
    return impl_->dump_rels_vml_drawing(Rels);
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

