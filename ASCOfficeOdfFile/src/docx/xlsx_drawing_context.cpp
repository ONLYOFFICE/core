
#include <boost/foreach.hpp>
#include <iostream>
#include <cpdoccore/xml/utils.h>

#include "mediaitems_utils.h"

#include "xlsx_drawing.h"
#include "xlsx_drawings.h"
#include "xlsx_utils.h"
#include "xlsx_drawing_context.h"
#include "xlsx_table_metrics.h"

#include "../odf/draw_common.h"
#include "../formulasconvert/formulasconvert.h"

#include "drawing_object_description.h"

namespace cpdoccore {
namespace oox {

class xlsx_drawing_context_handle::Impl
{
public:
    Impl(mediaitems & items)
        : items_(items), next_rId_(1), next_drawing_id_(1) 
    {
    }  

    mediaitems & get_mediaitems() { return items_; }

    size_t next_rId()
    {
        return next_rId_++;
    }

    std::pair<std::wstring, std::wstring> add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings)
    {
        const std::wstring id = boost::lexical_cast<std::wstring>(next_drawing_id_++);
        const std::wstring fileName = std::wstring(L"drawing") + id + L".xml";
        drawings_.push_back(drawing_elm(fileName, content, drawings));
        const std::wstring rId = std::wstring(L"rId") + id;//rDrId
        return std::pair<std::wstring, std::wstring>(fileName, rId);
    }

    const std::vector<drawing_elm> & content() const
    {
        return drawings_;
    }

private:
    mediaitems &				items_;    
    std::vector<drawing_elm>	drawings_;

    size_t						next_rId_;
    size_t						next_drawing_id_;
};

xlsx_drawing_context_handle::xlsx_drawing_context_handle(mediaitems & items)
: impl_(new xlsx_drawing_context_handle::Impl(items))
{
}

xlsx_drawing_context_handle::~xlsx_drawing_context_handle()
{
}

std::pair<std::wstring, std::wstring> xlsx_drawing_context_handle::add_drawing_xml(std::wstring const & content, xlsx_drawings_ptr drawings)
{
    return impl_->add_drawing_xml(content, drawings);
}

const std::vector<drawing_elm> & xlsx_drawing_context_handle::content() const
{
    return impl_->content();
}

class xlsx_drawing_context::Impl
{
public:    
    Impl(xlsx_drawing_context_handle & handle) : xlsx_drawings_(xlsx_drawings::create(false)), handle_(handle)
    {
		current_level_ = &objects_;
	} 

    xlsx_drawing_context_handle&				handle_;
    drawing_object_description					object_description_;    

	std::vector<drawing_object_description>		objects_;
	std::vector<drawing_object_description*>	groups_;

	std::vector<drawing_object_description>	*	current_level_;
//-----------------------------------------------------------------------------------

	mediaitems & get_mediaitems() { return handle_.impl_->get_mediaitems(); }

    void write_drawing(std::wostream & strm)
    {
        xlsx_serialize(strm, *xlsx_drawings_);
    }

    bool empty() const
    {
        return xlsx_drawings_->empty();
    }

    size_t next_rId()
    {
        return handle_.impl_->next_rId();
    }

    xlsx_drawings_ptr get_drawings()
    {
        return xlsx_drawings_;
    }
private:
    xlsx_drawings_ptr xlsx_drawings_;
};


xlsx_drawing_context::xlsx_drawing_context(xlsx_drawing_context_handle & h)
 : impl_(new xlsx_drawing_context::Impl(h))
{    
	hlinks_size_ = 0;
	clear();
}

void xlsx_drawing_context::clear()
{
	impl_->object_description_.type_				= mediaitems::typeUnknown;
	impl_->object_description_.in_group_			= false;
    impl_->object_description_.xlink_href_			= L"";
    impl_->object_description_.name_				= L"";
	impl_->object_description_.anchor_				= L"";
	impl_->object_description_.clipping_string_		= L"";
 	impl_->object_description_.svg_rect_			= boost::none;
	impl_->object_description_.use_image_replace_	= false;
 
	impl_->object_description_.additional_.clear();
	impl_->object_description_.hlinks_.clear();
	impl_->object_description_.additional_.clear();

}

xlsx_drawing_context::~xlsx_drawing_context()
{
}

void xlsx_drawing_context::start_group(std::wstring const & name)
{
	start_drawing(name);
	
	impl_->object_description_.type_		= mediaitems::typeGroup;
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

	_rect & group_rect = impl_->groups_.back()->svg_rect_.get();

	group_rect.cy -= group_rect.y;
	group_rect.cx -= group_rect.x;

	for (int i = 0; i < impl_->groups_.back()->child_objects_.size(); i++)
	{
		_rect & r = impl_->groups_.back()->child_objects_[i].svg_rect_.get();
		
		r.y -= group_rect.y;
		r.x -= group_rect.x;
	}
	
	if (impl_->groups_.back()->in_group_)
	{
		_rect & objct_rect = impl_->groups_.back()->svg_rect_.get();
		
		impl_->groups_.pop_back();
		_rect & group_rect = impl_->groups_.back()->svg_rect_.get();	

		if (group_rect.x > objct_rect.x)	group_rect.x = objct_rect.x;
		if (group_rect.y > objct_rect.y)	group_rect.y = objct_rect.y;

		if (group_rect.cx < objct_rect.cx + objct_rect.x)	group_rect.cx = objct_rect.cx + group_rect.x;
		if (group_rect.cy < objct_rect.cy + objct_rect.y)	group_rect.cy = objct_rect.cy + group_rect.y;
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

void xlsx_drawing_context::start_drawing(std::wstring const & name)
{
    impl_->object_description_.name_ = name;

	if (impl_->groups_.size() > 0)
		impl_->object_description_.in_group_ = true;
}

void xlsx_drawing_context::end_drawing()
{
	if (impl_->object_description_.in_group_)
	{
		_rect & objct_rect = impl_->object_description_.svg_rect_.get();
		_rect & group_rect = impl_->groups_.back()->svg_rect_.get();

		if (group_rect.x > objct_rect.x)	group_rect.x = objct_rect.x;
		if (group_rect.y > objct_rect.y)	group_rect.y = objct_rect.y;

		if (group_rect.cx < objct_rect.cx + objct_rect.x)	group_rect.cx = objct_rect.cx + group_rect.x;
		if (group_rect.cy < objct_rect.cy + objct_rect.y)	group_rect.cy = objct_rect.cy + group_rect.y;
	}
}

void xlsx_drawing_context::start_shape(int type)
{
	impl_->object_description_.type_		= mediaitems::typeShape;
	impl_->object_description_.shape_type_	= type; //2,3... 
}

void xlsx_drawing_context::end_shape()
{
	impl_->current_level_->push_back(impl_->object_description_);
}

void xlsx_drawing_context::start_object_ole()
{
}

void xlsx_drawing_context::end_object_ole()
{
}

void xlsx_drawing_context::start_image(std::wstring const & path)
{
	impl_->object_description_.type_		= mediaitems::typeImage;
	impl_->object_description_.xlink_href_	= path; 
}
void xlsx_drawing_context::end_image()
{
    impl_->current_level_->push_back(impl_->object_description_);
}
void xlsx_drawing_context::start_chart(std::wstring const & path)
{
	impl_->object_description_.type_		= mediaitems::typeChart;
	impl_->object_description_.xlink_href_	= path; 
}
void xlsx_drawing_context::end_chart()
{
    impl_->current_level_->push_back(impl_->object_description_);
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
void xlsx_drawing_context::set_rotate(double angle)
{
	set_property(odf_reader::_property(L"svg:rotate", angle));
	
	if (impl_->object_description_.svg_rect_)
	{
		_rect & r = impl_->object_description_.svg_rect_.get();
		
		//r.x -= r.width_/2;
		//r.y -= r.height_/2;

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
void xlsx_drawing_context::set_anchor(std::wstring anchor, double x_pt, double y_pt, bool group)
{
	if (group)
	{
		impl_->groups_.back()->anchor_			= anchor;
		impl_->groups_.back()->anchor_x_		= x_pt;
		impl_->groups_.back()->anchor_y_		= y_pt;
	}
	else
	{
		impl_->object_description_.anchor_		= anchor;
		impl_->object_description_.anchor_x_	= x_pt;
		impl_->object_description_.anchor_y_	= y_pt;
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
std::wstring xlsx_drawing_context::add_hyperlink(std::wstring const & href,bool object)
{
	++hlinks_size_;
	std::wstring hId=std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_size_);
	
	std::wstring href_correct = xml::utils::replace_text_to_xml(href);
    boost::algorithm::replace_all(href_correct, L" .", L".");//1 (130).odt

	_hlink_desc desc = {hId, href_correct, object}; //корректность написания ссылки важна для ms office и не важна для open office ->
	//todooo 
	impl_->object_description_.hlinks_.push_back(desc);

	return hId;
}

void xlsx_drawing_context::set_use_image_replacement()
{
	impl_->object_description_.use_image_replace_ = true;
}

bool xlsx_drawing_context::empty() const
{
    return impl_->empty();
}

void xlsx_drawing_context::process_common_properties(drawing_object_description & obj, _xlsx_drawing & drawing, xlsx_table_metrics & table_metrics)
{
	if (obj.anchor_.empty())
	{
		drawing.type_anchor = 2; // absolute
	}
	else
	{
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

	if (obj.svg_rect_)
	{
		_rect & r = obj.svg_rect_.get();

		//todooo непонятно что делать с отрицательными значениями
		int val = 0.5 + odf_types::length(obj.svg_rect_->x, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.x = val;
		
		val = 0.5 + odf_types::length(obj.svg_rect_->y, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.y = val;

		val = 0.5 + odf_types::length(obj.svg_rect_->cx, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.cx = val;

		val = .5 + odf_types::length(obj.svg_rect_->cy, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.cy = val;
	}
	
	drawing.additional	= obj.additional_;
	drawing.hlinks		= obj.hlinks_;
}
void xlsx_drawing_context::process_position_properties(drawing_object_description & obj, xlsx_table_metrics & table_metrics,xlsx_table_position & from,xlsx_table_position & to)
{
	size_t last_col=0, last_row=0;

	std::vector<std::wstring> distance;
	boost::algorithm::split(distance, obj.anchor_, boost::algorithm::is_any_of(L" ."), boost::algorithm::token_compress_on);

	double x=0,y=0,cx=0,cy=0;

	if (obj.svg_rect_)
	{
		x  = obj.svg_rect_->x;
		y  = obj.svg_rect_->y;
		cx = obj.svg_rect_->cx;
		cy = obj.svg_rect_->cy;
	}
	to = table_metrics.calc(x + cx, y + cy);

	if (distance.size() > 1)
	{
		//атрибуты размера игнорируются
		int ind_cell=distance.size()-1;
		getCellAddressInv(distance[ind_cell],last_col,last_row);

		xlsx_table_position to_anchor = table_metrics.calc(last_col,last_row,obj.anchor_x_,obj.anchor_y_); //-можно и не считать :) , но проверим ...

		table_metrics.update_pt(last_col,last_row, obj.anchor_x_, obj.anchor_y_);

		x = obj.anchor_x_ - cx;
		y = obj.anchor_y_ - cy;

		if (obj.svg_rect_)
		{
			_rect r_old = obj.svg_rect_.get();
			
			obj.svg_rect_= _rect (cx, cy, x, y);	
		}

		to = to_anchor;
	}
	from = table_metrics.calc(x, y);
}


void xlsx_drawing_context::process_image(drawing_object_description & obj,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	if (!drawing.fill.bitmap)
	{
		drawing.fill.bitmap = oox_bitmap_fill::create();
		drawing.fill.type	= 2;
	}
	_CP_OPT(std::wstring) sTextContent;
	GetProperty(obj.additional_,L"text-content",sTextContent);

	if (sTextContent)//в ms office на картинке нельзя сделать надпись - меняем тип на рект с заливкой картинкой
	{
		drawing.type		= mediaitems::typeShape;
		drawing.sub_type	= 2;//rect
	}
	std::wstring fileName = odf_packet_path_ + FILE_SEPARATOR_STR +  obj.xlink_href_;			
	
	drawing.fill.bitmap->bCrop		= odf_reader::parse_clipping(obj.clipping_string_, fileName, drawing.fill.bitmap->cropRect);
	drawing.fill.bitmap->bStretch	= true;

	std::wstring ref;/// это ссылка на выходной внешний объект
	bool isMediaInternal = false;

	drawing.fill.bitmap->rId = impl_->get_mediaitems().add_or_find(obj.xlink_href_, mediaitems::typeImage, isMediaInternal, ref);		

	if (drawing.type == mediaitems::typeShape)
	{
		impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, mediaitems::typeImage);//собственно это не объект, а доп рел и ref объекта
	
		isMediaInternal=true;
		std::wstring rId = impl_->get_mediaitems().add_or_find(L"", mediaitems::typeShape, isMediaInternal, ref);
		
		xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, mediaitems::typeShape);//объект

	}else
	{
		xlsx_drawings_->add(drawing, isMediaInternal, drawing.fill.bitmap->rId , ref, mediaitems::typeImage);//объект
		
		if (drawing.inGroup)
			impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, obj.type_); // не объект
	}
}

void xlsx_drawing_context::process_chart(drawing_object_description & obj,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	std::wstring ref;
    bool isMediaInternal = true;
	
	drawing.chartId = impl_->get_mediaitems().add_or_find(obj.xlink_href_, obj.type_, isMediaInternal, ref);
    xlsx_drawings_->add(drawing, isMediaInternal, drawing.chartId, ref, obj.type_);
	
	if (drawing.inGroup)
		impl_->get_drawings()->add(isMediaInternal, drawing.chartId, ref, obj.type_); // не объект
}

void xlsx_drawing_context::process_shape(drawing_object_description & obj,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	std::wstring ref;
    bool isMediaInternal = true;

	std::wstring rId	= impl_->get_mediaitems().add_or_find(L"", obj.type_, isMediaInternal, ref);
	xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, obj.type_);
}

void xlsx_drawing_context::process_group(drawing_object_description & obj, xlsx_table_metrics & table_metrics,_xlsx_drawing & drawing, xlsx_drawings_ptr xlsx_drawings_)
{
	xlsx_drawings_ptr xlsx_drawings_child(xlsx_drawings::create(true));
	
	process_objects	( obj.child_objects_, table_metrics, xlsx_drawings_child);	
	
	std::wstringstream strm;

	xlsx_serialize(strm, *xlsx_drawings_child);

	drawing.content_group_ = strm.str();

//-----------------------------------------------------------------------------------------------
	std::wstring ref;
    bool isMediaInternal = true;

	std::wstring rId	= impl_->get_mediaitems().add_or_find(L"", obj.type_, isMediaInternal, ref);
	xlsx_drawings_->add(drawing, isMediaInternal, rId, ref, obj.type_);

}
void xlsx_drawing_context::process_objects(xlsx_table_metrics & table_metrics)
{
	process_objects(impl_->objects_, table_metrics, impl_->get_drawings());
}
void xlsx_drawing_context::process_objects(std::vector<drawing_object_description> objects, xlsx_table_metrics & table_metrics, xlsx_drawings_ptr xlsx_drawings_)
{
	for (int i = 0 ; i < objects.size(); i++)
	{
		drawing_object_description & obj = objects[i];

		int pos_replaicement = obj.xlink_href_.find(L"ObjectReplacements");	
		if (pos_replaicement >= 0 && !obj.use_image_replace_)continue;	//объект

		_xlsx_drawing drawing	=_xlsx_drawing();

		drawing.type	= obj.type_;
		drawing.name	= obj.name_;
		drawing.fill	= obj.fill_;
		drawing.inGroup	= obj.in_group_;
		drawing.id		= impl_->next_rId();

		drawing.sub_type = obj.shape_type_;
		
		if (drawing.fill.bitmap)
		{
			std::wstring ref;
			bool isMediaInternal = true;
			
			drawing.fill.bitmap->rId = impl_->get_mediaitems().add_or_find(drawing.fill.bitmap->xlink_href_, mediaitems::typeImage, isMediaInternal, ref);
			impl_->get_drawings()->add(isMediaInternal, drawing.fill.bitmap->rId, ref, mediaitems::typeImage);//собственно это не объект, а доп рел и ref объекта
		}
	   
		process_common_properties(obj, drawing, table_metrics);

		switch(obj.type_)
		{
			case mediaitems::typeChart:		process_chart	( obj, drawing, xlsx_drawings_);				break;
			case mediaitems::typeImage:		process_image	( obj, drawing, xlsx_drawings_);				break;
			case mediaitems::typeShape:		process_shape	( obj, drawing, xlsx_drawings_);				break;
			case mediaitems::typeGroup:		process_group	( obj, table_metrics, drawing, xlsx_drawings_);	break;
		}
	}
}

void xlsx_drawing_context::write_drawing(std::wostream & strm)
{
    impl_->write_drawing(strm);    
}


xlsx_drawings_ptr xlsx_drawing_context::get_drawings()
{
    return impl_->get_drawings();
}

}
}
