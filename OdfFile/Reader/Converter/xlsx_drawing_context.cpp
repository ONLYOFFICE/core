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
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <xml/utils.h>

#include "xlsx_drawing.h"
#include "xlsx_drawings.h"
#include "xlsx_utils.h"
#include "xlsx_drawing_context.h"
#include "xlsx_table_metrics.h"

#include "../Format/draw_common.h"
#include "../../Formulas/formulasconvert.h"

#include "drawing_object_description.h"

#include "../../../OOXML/Base/Unit.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawing_context_handle::Impl
{
public:
    Impl(mediaitems_ptr & items) : items_(items), next_drawing_id_(1), next_vml_drawing_id_(1)
    {
    }  

    mediaitems_ptr & get_mediaitems() { return items_; }

    std::pair<std::wstring, std::wstring> add_drawing_vml(std::wstring const & content, xlsx_drawings_ptr drawings)
    {
        const std::wstring id = std::to_wstring(next_vml_drawing_id_++);
        const std::wstring fileName = std::wstring(L"vmlDrawing") + id + L".vml";
        
		drawings_vml_.push_back(drawing_elm(fileName, content, drawings, typeDefault));
        const std::wstring rId = std::wstring(L"rVId") + id;
       
		return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings, _rels_type const & type_)
    {
        const std::wstring id = std::to_wstring(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
        
		drawings_.push_back(drawing_elm(fileName, content, drawings, type_));
        const std::wstring rId = std::wstring(L"rDrId") + id;
       
		return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

	const std::vector<drawing_elm> & content() const
	{
		return drawings_;
	}
	 const std::vector<drawing_elm> & content_vml() const
	 {
        return drawings_vml_;
	 }
private:
    mediaitems_ptr				items_;    
    
	std::vector<drawing_elm>	drawings_;
    std::vector<drawing_elm>	drawings_vml_;

    size_t						next_drawing_id_;
    size_t						next_vml_drawing_id_;
};

xlsx_drawing_context_handle::xlsx_drawing_context_handle(mediaitems_ptr & items) :
	next_rId_(1024), impl_(new xlsx_drawing_context_handle::Impl(items))
{
}

xlsx_drawing_context_handle::~xlsx_drawing_context_handle()
{
}

std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings, _rels_type const & type_)
{
    return impl_->add_drawing_xml(content, drawings, type_);
}
std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_vml(std::wstring const & content, xlsx_drawings_ptr drawings)
{
    return impl_->add_drawing_vml(content, drawings);
}

const std::vector<drawing_elm> & xlsx_drawing_context_handle::content() const
{
    return impl_->content();
}
const std::vector<drawing_elm> & xlsx_drawing_context_handle::content_vml() const
{
    return impl_->content_vml();
}
class xlsx_drawing_context::Impl
{
public:    
    Impl(xlsx_drawing_context_handle_ptr & handle) : xlsx_drawings_(xlsx_drawings::create(false)), handle_(handle)
    {
		current_level_			= &objects_;
		use_image_replacement_	= false;
	} 

    xlsx_drawing_context_handle_ptr				handle_;
    drawing_object_description					object_description_;    

	std::vector<drawing_object_description>		objects_;
	std::vector<drawing_object_description*>	groups_;

	std::vector<drawing_object_description>	*	current_level_;
	bool										use_image_replacement_;
//-----------------------------------------------------------------------------------

	mediaitems_ptr & get_mediaitems() { return handle_->impl_->get_mediaitems(); }

	void serialize(std::wostream & strm, const std::wstring & ns, bool local)
    {
        xlsx_drawings_->serialize(strm, ns, local);
    }
	void serialize_vml(std::wostream & strm)
    {
        xlsx_drawings_->serialize_vml(strm);
    }
    bool empty() const
    {
        return xlsx_drawings_->empty();
    }
    bool vml_empty() const
    {
        return xlsx_drawings_->vml_empty();
    }
    size_t next_rId()
    {
        return handle_->next_rId();
    }

    xlsx_drawings_ptr get_drawings()
    {
        return xlsx_drawings_;
    }
private:
    xlsx_drawings_ptr xlsx_drawings_;
};


xlsx_drawing_context::xlsx_drawing_context(xlsx_drawing_context_handle_ptr & h, bool in_text)
 : impl_(new xlsx_drawing_context::Impl(h))
{
	in_text_ = in_text;
	hlinks_size_ = 0;
	clear();
}

void xlsx_drawing_context::clear()
{
	impl_->object_description_.bInner_				= false;
	impl_->object_description_.type_				= typeUnknown;
	impl_->object_description_.in_group_			= false;
	impl_->object_description_.lined_				= false;
	impl_->object_description_.connector_			= false;
    impl_->object_description_.xlink_href_			= L"";
    impl_->object_description_.name_				= L"";
	impl_->object_description_.anchor_				= L"";
	impl_->object_description_.anchor_rel_			= anchor_none;
	impl_->object_description_.anchor_x_			= 0;
	impl_->object_description_.anchor_y_			= 0;
	impl_->object_description_.clipping_string_		= L"";
 	impl_->object_description_.svg_rect_			= boost::none;
 
	impl_->object_description_.additional_.clear();
	impl_->object_description_.hlinks_.clear();
	impl_->object_description_.action_.clear();
	impl_->object_description_.additional_.clear();

	impl_->use_image_replacement_					= false;
}

xlsx_drawing_context::~xlsx_drawing_context()
{
}

void xlsx_drawing_context::start_group(std::wstring const & name)
{
	start_drawing(name);
	
	impl_->object_description_.type_		= typeGroupShape;
	impl_->object_description_.svg_rect_	= _rect();
	impl_->object_description_.svg_rect_->x = impl_->object_description_.svg_rect_->y = 0x7fffffff;
	
	impl_->current_level_->push_back(impl_->object_description_);
	
	impl_->groups_.push_back(&impl_->current_level_->back());	
	
	impl_->current_level_ = &(impl_->groups_.back()->child_objects_);
	clear();	
}

void xlsx_drawing_context::end_group()
{
	if (impl_->groups_.size() < 1) return;

	if (impl_->groups_.back()->svg_rect_)
	{
		_rect & group_rect = impl_->groups_.back()->svg_rect_.get();

		group_rect.cy -= group_rect.y;
		group_rect.cx -= group_rect.x;

		for (size_t i = 0; i < impl_->groups_.back()->child_objects_.size(); i++)
		{
			if (!impl_->groups_.back()->child_objects_[i].svg_rect_) continue;

			_rect & r = impl_->groups_.back()->child_objects_[i].svg_rect_.get();
			
			r.y -= group_rect.y;
			r.x -= group_rect.x;
		}
	}
	
	if (impl_->groups_.back()->in_group_ && impl_->groups_.back()->svg_rect_)
	{
		_rect & objct_rect = impl_->groups_.back()->svg_rect_.get();
		
		impl_->groups_.pop_back();
		_rect & group_rect = impl_->groups_.back()->svg_rect_.get();	

		if (group_rect.x > objct_rect.x)	group_rect.x = objct_rect.x;
		if (group_rect.y > objct_rect.y)	group_rect.y = objct_rect.y;

		if (group_rect.cx < objct_rect.cx + objct_rect.x)	group_rect.cx = objct_rect.cx + objct_rect.x;
		if (group_rect.cy < objct_rect.cy + objct_rect.y)	group_rect.cy = objct_rect.cy + objct_rect.y;
	}
	else
		impl_->groups_.pop_back();
	
	if (impl_->groups_.size() > 0)
	{
		impl_->current_level_ = &(impl_->groups_.back()->child_objects_);
	}
	else
	{
		impl_->current_level_ = &impl_->objects_;
	}
}
bool xlsx_drawing_context::isDefault()
{
	return impl_->object_description_.type_ == typeUnknown;
}
void xlsx_drawing_context::start_drawing(std::wstring const & name)
{
    impl_->object_description_.name_ = name;

	if (false == impl_->groups_.empty())
		impl_->object_description_.in_group_ = true;
}

void xlsx_drawing_context::end_drawing()
{
	if (impl_->object_description_.in_group_ && 
				impl_->object_description_.svg_rect_)
	{
		_rect & objct_rect = impl_->object_description_.svg_rect_.get();
		_rect & group_rect = impl_->groups_.back()->svg_rect_.get();

		if (group_rect.x > objct_rect.x)	group_rect.x = objct_rect.x;
		if (group_rect.y > objct_rect.y)	group_rect.y = objct_rect.y;

		if (group_rect.cx < objct_rect.cx + objct_rect.x)	group_rect.cx = objct_rect.cx + objct_rect.x;
		if (group_rect.cy < objct_rect.cy + objct_rect.y)	group_rect.cy = objct_rect.cy + objct_rect.y;
	}
}

void xlsx_drawing_context::start_shape(int type)
{
	impl_->object_description_.type_		= typeShape;
	impl_->object_description_.shape_type_	= type; //2,3... 
}

void xlsx_drawing_context::end_shape()
{
	impl_->current_level_->push_back(impl_->object_description_);
	clear();
}
void xlsx_drawing_context::start_comment(int base_col, int base_row)
{
	impl_->object_description_.type_ = typeComment;
	impl_->object_description_.shape_type_ = 0x0019; // OBJ_Note object type for vml 

	set_property(odf_reader::_property(L"base_col", base_col));
	set_property(odf_reader::_property(L"base_row", base_row));
}
void xlsx_drawing_context::end_comment()
{
	impl_->current_level_->push_back(impl_->object_description_);
}
void xlsx_drawing_context::start_control(const std::wstring & ctrlPropId, int type)
{
	impl_->object_description_.type_ = typeControl;
	impl_->object_description_.shape_type_ = type; // object type for vml 
	
	impl_->object_description_.xlink_href_ = ctrlPropId;
}
void xlsx_drawing_context::end_control()
{
	impl_->current_level_->push_back(impl_->object_description_);
	clear();
}
void xlsx_drawing_context::set_use_image_replacement()
{
	impl_->use_image_replacement_ = true;
}
void xlsx_drawing_context::set_text_box()
{
	impl_->object_description_.type_		= typeShape;
	impl_->object_description_.shape_type_	= 2; //2,3... 
}

void xlsx_drawing_context::set_ole_object(const std::wstring & path, const std::wstring & progId)
{
	impl_->object_description_.type_		= typeOleObject;
	impl_->object_description_.xlink_href_	= path; 
	impl_->object_description_.descriptor_	= progId;
}
void xlsx_drawing_context::set_ms_object(const std::wstring & path, const std::wstring & progId)
{
	impl_->object_description_.type_		= typeMsObject;
	impl_->object_description_.xlink_href_	= path; 
	impl_->object_description_.descriptor_	= progId;
}
void xlsx_drawing_context::set_image(const std::wstring & path)
{
	int pos_replaicement = path.find(L"ObjectReplacements"); 
	if (pos_replaicement >= 0)
	{
		if (path.length() - (pos_replaicement + 18) < 2)
			return; //object without image
	}

	if (impl_->object_description_.type_ == typeUnknown)
	{
		impl_->object_description_.type_		= typeImage;	
		impl_->object_description_.xlink_href_	= path; 
	}
	else if (impl_->use_image_replacement_)
	{
		impl_->object_description_.fill_.type	= 2;
		impl_->object_description_.fill_.bitmap = oox::oox_bitmap_fill::create();
		impl_->object_description_.fill_.bitmap->xlink_href_ = path;
	}
}
bool xlsx_drawing_context::start_frame()
{
	if (impl_->object_description_.type_ != typeUnknown)
		impl_->object_description_.bInner_ = true;

	return impl_->object_description_.bInner_;
}
void xlsx_drawing_context::end_frame()
{
	if (impl_->object_description_.bInner_)
		impl_->object_description_.bInner_ = false;
	else
	{
		impl_->current_level_->push_back(impl_->object_description_);
	}	
	clear();
}

void xlsx_drawing_context::set_chart(const std::wstring & path)
{
	impl_->object_description_.type_		= typeChart;
	impl_->object_description_.xlink_href_	= path; 
}

void xlsx_drawing_context::set_rect(double width_pt, double height_pt, double x_pt, double y_pt)
{
	impl_->object_description_.svg_rect_ = _rect(width_pt, height_pt, x_pt, y_pt);
}

void xlsx_drawing_context::set_translate(double x_pt, double y_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect & r = impl_->object_description_.svg_rect_.get();
		
		r.x += x_pt;
		r.y += y_pt;
	}
}

void xlsx_drawing_context::set_rotate(double angle, bool translate)
{
	set_property(odf_reader::_property(L"svg:rotate", angle));
	
	if (impl_->object_description_.svg_rect_ && translate)
	{
		_rect &r = impl_->object_description_.svg_rect_.get();
		double new_x = (r.cx / 2 * cos(-angle) - r.cy / 2 * sin(-angle) ) - r.cx / 2;
		double new_y = (r.cx / 2 * sin(-angle) + r.cy / 2 * cos(-angle) ) - r.cy / 2;
		
		r.x += new_x;
		r.y	+= new_y;
	}
}
void xlsx_drawing_context::set_scale(double cx_pt, double cy_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect & r = impl_->object_description_.svg_rect_.get();
		
		r.x *= cx_pt;
		r.y *= cy_pt;

	}
}
void xlsx_drawing_context::set_rel_anchor(_INT32 owner_cx, _INT32 owner_cy)
{
	impl_->object_description_.owner_cx_ = owner_cx;
	impl_->object_description_.owner_cy_ = owner_cy;
}
void xlsx_drawing_context::set_anchor_end(std::wstring anchor, double x_pt, double y_pt, bool group)
{
	if (group)
	{
		impl_->groups_.back()->anchor_rel_ = anchor_end;
		impl_->groups_.back()->anchor_ = anchor;
		impl_->groups_.back()->anchor_x_ = x_pt;
		impl_->groups_.back()->anchor_y_ = y_pt;
	}
	else
	{
		impl_->object_description_.anchor_rel_ = anchor_end;
		impl_->object_description_.anchor_ = anchor;
		impl_->object_description_.anchor_x_ = x_pt;
		impl_->object_description_.anchor_y_ = y_pt;
	}

}

void xlsx_drawing_context::set_anchor_start(std::wstring anchor, double x_pt, double y_pt, bool group)
{
	if (group)
	{
		impl_->groups_.back()->anchor_rel_ = anchor_start;
		impl_->groups_.back()->anchor_ = anchor;
		impl_->groups_.back()->anchor_x_ = x_pt;
		impl_->groups_.back()->anchor_y_ = y_pt;
	}
	else
	{
		impl_->object_description_.anchor_rel_ = anchor_start;
		impl_->object_description_.anchor_ = anchor;
		impl_->object_description_.anchor_x_ = x_pt;
		impl_->object_description_.anchor_y_ = y_pt;
	}
}
void xlsx_drawing_context::set_property(odf_reader::_property p)
{
	impl_->object_description_.additional_.push_back(p);
}
std::vector<odf_reader::_property> & xlsx_drawing_context::get_properties()
{
	return impl_->object_description_.additional_;
}
void xlsx_drawing_context::set_clipping(const std::wstring & str)
{
	impl_->object_description_.clipping_string_= str;
}
void xlsx_drawing_context::set_fill(_oox_fill & fill)
{
	impl_->object_description_.fill_= fill;
}

std::wstring xlsx_drawing_context::add_hyperlink(std::wstring const & href)
{
	++hlinks_size_;
	std::wstring hId = L"hId" + std::to_wstring(hlinks_size_);
	
	std::wstring href_correct = XmlUtils::EncodeXmlString(href);
    XmlUtils::replace_all( href_correct, L" .", L".");//1 (130).odt

	//корректность написания ссылки важна для ms office и не важна для open office ->
//todooo 
	_hlink_desc desc = {hId, href_correct};
	impl_->object_description_.hlinks_.push_back(desc);

	return hId;
}

bool xlsx_drawing_context::empty() const
{
    return impl_->empty();
}
bool xlsx_drawing_context::vml_empty() const
{
    return impl_->vml_empty();
}
void xlsx_drawing_context::process_common_properties(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_table_metrics & table_metrics)
{
	if (obj.anchor_rel_ == anchor_none)
	{
		if (obj.owner_cx_ && obj.owner_cy_)
		{
			drawing.type_anchor = 3; // relative
			drawing.owner_cx_ = obj.owner_cx_;
			drawing.owner_cy_ = obj.owner_cy_;
		}
		else
			drawing.type_anchor = 2; // absolute
	}
	
	if (drawing.type_anchor == 1 || obj.type_ == typeMsObject || obj.type_ == typeOleObject || obj.type_ == typeControl)
	{
		xlsx_table_position from, to;
		
		process_position_properties	(obj, table_metrics, from, to, obj.anchor_rel_ == anchor_end);

		drawing.from_.type				= xlsx_drawing_position::from;
		drawing.from_.position.col		= from.col;
		drawing.from_.position.colOff	= static_cast<size_t>(odf_types::length(from.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		drawing.from_.position.row		= from.row;
		drawing.from_.position.rowOff	= static_cast<size_t>(odf_types::length(from.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));

		drawing.to_.type				= xlsx_drawing_position::to;
		drawing.to_.position.col		= to.col;
		drawing.to_.position.colOff		= static_cast<size_t>(odf_types::length(to.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		drawing.to_.position.row		= to.row;
		drawing.to_.position.rowOff		= static_cast<size_t>(odf_types::length(to.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
	}

	if (obj.svg_rect_)
	{
		_rect & r = obj.svg_rect_.get();

		//todooo непонятно что делать с отрицательными значениями
		_INT32 val = 0;
			
		val = (_INT32) (0.5 + odf_types::length(obj.svg_rect_->x, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if (val >= 0)	drawing.x = val;
		
		val = (_INT32) (0.5 + odf_types::length(obj.svg_rect_->y, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if (val >= 0)	drawing.y = val;

		val = (_INT32) (0.5 + odf_types::length(obj.svg_rect_->cx, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if (val >= 0)	drawing.cx = val;

		val = (_INT32) (0.5 + odf_types::length(obj.svg_rect_->cy, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		if (val >= 0)	drawing.cy = val;
	}
	
	drawing.additional	= obj.additional_;
	drawing.hlinks		= obj.hlinks_;
	drawing.action		= obj.action_;
}
void xlsx_drawing_context::process_position_properties(drawing_object_description & obj, xlsx_table_metrics & table_metrics,xlsx_table_position & from,xlsx_table_position & to, bool byEnd)
{
	size_t column_anchor = 0, row_anchor = 0;

	std::vector<std::wstring> distance;
	boost::algorithm::split(distance, obj.anchor_, boost::algorithm::is_any_of(L" ."), boost::algorithm::token_compress_on);

	double x = 0, y = 0, cx = 0, cy = 0;

	if (obj.svg_rect_)
	{
		x = obj.svg_rect_->x;
		y  = obj.svg_rect_->y;
		cx = obj.svg_rect_->cx;
		cy = obj.svg_rect_->cy;
	}
	to		= table_metrics.calc(x + cx, y + cy);
	from	= table_metrics.calc(x, y);

	if (!distance.empty() && !obj.anchor_.empty())
	{
		int ind_cell = distance.size() - 1;
		
		if (false == getCellAddressInv(distance[ind_cell], column_anchor, row_anchor)) return;
			
		xlsx_table_position pos_anchor = table_metrics.calc(column_anchor, row_anchor, obj.anchor_x_, obj.anchor_y_);
		table_metrics.update_pt(column_anchor, row_anchor, obj.anchor_x_, obj.anchor_y_);

		if (byEnd)
		{
			to = pos_anchor;
			
			x = obj.anchor_x_ - cx;
			y = obj.anchor_y_ - cy;
			
			if (x < 0) x = 0;
			if (y < 0) y = 0; // calcul dun MS.ods
			
			from = table_metrics.calc(x, y);
		}
		else
		{
			from = pos_anchor;

			x = obj.anchor_x_ + cx;
			y = obj.anchor_y_ + cy;

			to = table_metrics.calc(x, y);
		}

		if (obj.svg_rect_)
		{
			_rect r_old = obj.svg_rect_.get();
			
			obj.svg_rect_= _rect (cx, cy, x, y);	
		}
	}
}


void xlsx_drawing_context::process_image(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	if (!drawing.fill.bitmap)
	{
		drawing.fill.bitmap = oox_bitmap_fill::create();
		drawing.fill.type	= 2;
	}
	_CP_OPT(std::wstring)	sTextContent, sColorMode;
	
	GetProperty(obj.additional_, L"text-content", sTextContent);
	GetProperty(obj.additional_, L"color-mode", sColorMode);
	GetProperty(obj.additional_, L"luminance", drawing.fill.bitmap->luminance);
	GetProperty(obj.additional_, L"contrast", drawing.fill.bitmap->contrast);

	if (sTextContent)//в ms office на картинке нельзя сделать надпись - меняем тип на рект с заливкой картинкой
	{
		drawing.type		= typeShape;
		drawing.sub_type	= 2;//rect
	}
	std::wstring fileName = odf_packet_path_ + FILE_SEPARATOR_STR +  obj.xlink_href_;			
	
	drawing.fill.bitmap->bCrop		= odf_reader::parse_clipping(obj.clipping_string_, fileName, drawing.fill.bitmap->cropRect, impl_->get_mediaitems()->applicationFonts());
	drawing.fill.bitmap->bStretch	= true;

	if ((sColorMode) && (*sColorMode == L"greyscale"))
		drawing.fill.bitmap->bGrayscale	= true;

	std::wstring ref;/// это ссылка на выходной внешний объект
	bool isMediaInternal = false;

	drawing.fill.bitmap->rId = impl_->get_mediaitems()->add_or_find(obj.xlink_href_, typeImage, isMediaInternal, ref, oox::document_place);		

	if (drawing.type == typeShape)
	{
		impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, typeImage, false, false);//собственно это не объект, а доп рел и ref объекта
	
		isMediaInternal=true;
		std::wstring rId = impl_->get_mediaitems()->add_or_find(L"", typeShape, isMediaInternal, ref, oox::document_place);
		
		xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, typeShape);//объект

	}else
	{
		xlsx_drawings_->add(drawing, isMediaInternal, drawing.fill.bitmap->rId , ref, typeImage);//объект
		
		if (drawing.inGroup)
			impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, obj.type_, false, false); // не объект
	}
}

void xlsx_drawing_context::process_chart(drawing_object_description & obj,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	std::wstring ref;
    bool isMediaInternal = true;
	
	drawing.objectId = impl_->get_mediaitems()->add_or_find(obj.xlink_href_, obj.type_, isMediaInternal, ref, oox::document_place);
    xlsx_drawings_->add(drawing, isMediaInternal, drawing.objectId, ref, obj.type_);
	
	if (drawing.inGroup)
		impl_->get_drawings()->add(isMediaInternal, drawing.objectId, ref, obj.type_, false, false); // не объект
}

void xlsx_drawing_context::process_object(drawing_object_description & obj, xlsx_table_metrics & table_metrics, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	std::wstring ref;
    bool isMediaInternal = true;
	
	if (drawing.type_anchor == 2) // absolute
	{
		//пересчет нужен 
		xlsx_table_position from, to;
		
		process_position_properties	(obj, table_metrics, from, to);

		drawing.from_.type				= xlsx_drawing_position::from;
		drawing.from_.position.col		= from.col;
		drawing.from_.position.colOff	= static_cast<size_t>(odf_types::length(from.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		drawing.from_.position.row		= from.row;
		drawing.from_.position.rowOff	= static_cast<size_t>(odf_types::length(from.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));

		drawing.to_.type				= xlsx_drawing_position::to;
		drawing.to_.position.col		= to.col;
		drawing.to_.position.colOff		= static_cast<size_t>(odf_types::length(to.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
		drawing.to_.position.row		= to.row;
		drawing.to_.position.rowOff		= static_cast<size_t>(odf_types::length(to.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
	}	

	if (obj.type_ == typeControl || obj.type_ == typeComment)
	{
		drawing.objectId = obj.xlink_href_;

		xlsx_drawings_->add(drawing, isMediaInternal, drawing.objectId, ref, obj.type_, false);
	}
	else
	{
		drawing.objectId		= impl_->get_mediaitems()->add_or_find(obj.xlink_href_, obj.type_, isMediaInternal, ref, oox::document_place);
		drawing.objectProgId	= obj.descriptor_;
		
		xlsx_drawings_->add(drawing, isMediaInternal, drawing.objectId, ref, obj.type_, true);
	}
	
	if (drawing.inGroup)
	{// не объекты 
		if (obj.type_ == typeControl || obj.type_ == typeComment)
			impl_->get_drawings()->add(isMediaInternal, drawing.objectId, ref, obj.type_, false, false); 
		else
			impl_->get_drawings()->add(isMediaInternal, drawing.objectId, ref, obj.type_, true, false); 
	}
}
void xlsx_drawing_context::process_shape(drawing_object_description & obj,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	std::wstring ref;
    bool isMediaInternal = true;

	std::wstring rId = impl_->get_mediaitems()->add_or_find(L"", obj.type_, isMediaInternal, ref, oox::document_place);
	xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, obj.type_);
}

void xlsx_drawing_context::process_group(drawing_object_description & obj, xlsx_table_metrics & table_metrics, _xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	xlsx_drawings_ptr xlsx_drawings_child(xlsx_drawings::create(true));
	
	process_group_objects( obj.child_objects_, table_metrics, xlsx_drawings_child);	

	drawing.childs_ = xlsx_drawings_child->get();
	
	std::wstringstream strm;

	xlsx_drawings_child->serialize(strm, L"xdr", in_text_);

	drawing.content_group_ = strm.str();

//-----------------------------------------------------------------------------------------------
	std::wstring ref;
    bool isMediaInternal = true;

	std::wstring rId	= impl_->get_mediaitems()->add_or_find(L"", obj.type_, isMediaInternal, ref, oox::document_place);
	xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, obj.type_);

}
void xlsx_drawing_context::process_objects(xlsx_table_metrics & table_metrics)
{
	process_group_objects(impl_->objects_, table_metrics, impl_->get_drawings());
}
void xlsx_drawing_context::process_group_objects(std::vector<drawing_object_description> objects, xlsx_table_metrics & table_metrics, xlsx_drawings_ptr xlsx_drawings_)
{
	for (size_t i = 0 ; i < objects.size(); i++)
	{
		drawing_object_description & obj = objects[i];

		_xlsx_drawing drawing	=_xlsx_drawing();

		drawing.type		= obj.type_;
		drawing.name		= obj.name_;
		drawing.fill		= obj.fill_;
		drawing.inGroup		= obj.in_group_;
		drawing.id			= impl_->next_rId();
		drawing.lined		= obj.lined_;
		drawing.connector	= obj.connector_;

		drawing.sub_type	= obj.shape_type_;
		
		if (drawing.fill.bitmap)
		{
			std::wstring ref;
			bool isMediaInternal = true;
			
			drawing.fill.bitmap->rId = impl_->get_mediaitems()->add_or_find(drawing.fill.bitmap->xlink_href_, typeImage, isMediaInternal, ref, oox::document_place);
			
			bool in_sheet = (obj.type_== typeOleObject || obj.type_== typeMsObject) ? true : false;
			impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, typeImage, in_sheet, false);//собственно это не объект, а доп рел и ref объекта

			//object dumps in sheet rels !!
		}
	   
		process_common_properties(obj, drawing, table_metrics);

		switch(obj.type_)
		{
			case typeChart:			process_chart	( obj, drawing, xlsx_drawings_);				break;
			case typeImage:			process_image	( obj, drawing, xlsx_drawings_);				break;
			case typeShape:			process_shape	( obj, drawing, xlsx_drawings_);				break;
			case typeGroupShape:	process_group	( obj, table_metrics, drawing, xlsx_drawings_);	break;
			case typeMsObject:	
			case typeOleObject:
			case typeControl:									
			case typeComment:		process_object	( obj, table_metrics, drawing, xlsx_drawings_);	break;
		}
	}
}

void xlsx_drawing_context::serialize(std::wostream & strm, const std::wstring& ns, bool local)
{
    impl_->serialize(strm, ns, local);
}
void xlsx_drawing_context::serialize_vml(std::wostream & strm)
{
    impl_->serialize_vml(strm);    
}

xlsx_drawings_ptr xlsx_drawing_context::get_drawings()
{
    return impl_->get_drawings();
}
void xlsx_drawing_context::start_action(std::wstring action)
{
	impl_->object_description_.action_.enabled	= true;
}

void xlsx_drawing_context::set_link(std::wstring link, _rels_type typeRels)
{//hyprelinks only
	++hlinks_size_;
	std::wstring hId = L"hId" + std::to_wstring(hlinks_size_);
	
	link = XmlUtils::EncodeXmlString(link);
	
	if (typeRels == typeHyperlink)
		XmlUtils::replace_all( link, L" .", L".");		//1 (130).odt

	impl_->object_description_.action_.hId		= hId;
	impl_->object_description_.action_.hRef		= link;
	impl_->object_description_.action_.typeRels	= typeRels;
}
void xlsx_drawing_context::end_action()
{
}
void xlsx_drawing_context::set_is_line_shape(bool val)
{
	impl_->object_description_.lined_ = val;
}
void xlsx_drawing_context::set_is_connector_shape(bool val)
{
	impl_->object_description_.connector_ = val;
}

}
}
