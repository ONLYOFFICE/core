
#include <boost/foreach.hpp>
#include <iostream>
#include "../formulasconvert/formulasconvert.h"

#include "mediaitems_utils.h"

#include "xlsx_drawing.h"
#include "xlsx_drawings.h"
#include "xlsx_utils.h"
#include "xlsx_drawing_context.h"
#include "xlsx_table_metrics.h"

#include "../odf/draw_common.h"

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
    mediaitems & items_;
    size_t next_rId_;
    
    std::vector<drawing_elm> drawings_;

    size_t next_drawing_id_;
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
    Impl(xlsx_drawing_context_handle & handle) : xlsx_drawings_(xlsx_drawings::create()),
        handle_(handle)
    {} 

    xlsx_drawing_context_handle & handle_;
    drawing_object_description object_description_;    

	std::vector<drawing_object_description> images_;
	std::vector<drawing_object_description> charts_;
	std::vector<drawing_object_description> shapes_;

    void add_drawing(_xlsx_drawing const & d,
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
    {
        xlsx_drawings_->add(d, isInternal, rid, ref, type);
    }
      void add_drawing(/**/
        bool isInternal,
        std::wstring const & rid,
        std::wstring const & ref,
        mediaitems::Type type)
    {
        xlsx_drawings_->add(isInternal, rid, ref, type);
    } 

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
}
void xlsx_drawing_context::default_set()
{
    impl_->object_description_.xlink_href_ = L"";

    impl_->object_description_.draw_name_ = L"";
  
	impl_->object_description_.additional_.clear();
	impl_->object_description_.anchor_ =L"";
	
	impl_->object_description_.clipping_string_= L"";
	impl_->object_description_.svg_rect_	= boost::none;

	impl_->object_description_.hlinks_.clear();
	impl_->object_description_.additional_.clear();

	impl_->object_description_.use_image_replace_ = false;
}
xlsx_drawing_context::~xlsx_drawing_context()
{
}

void xlsx_drawing_context::start_shapes()
{
}

void xlsx_drawing_context::end_shapes()
{
}

void xlsx_drawing_context::start_drawing(std::wstring const & name)
{
    impl_->object_description_.draw_name_ = name;
}
void xlsx_drawing_context::set_rect(double width_pt, double height_pt, double x_pt, double y_pt)
{
	_rect r = {width_pt,height_pt,x_pt,y_pt};
	impl_->object_description_.svg_rect_= r;
}
void xlsx_drawing_context::set_translate(double x_pt, double y_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect r = impl_->object_description_.svg_rect_.get();
		r.x_+=x_pt;
		r.y_+=y_pt;

		impl_->object_description_.svg_rect_= r;
	}
}
void xlsx_drawing_context::set_rotate(double angle)
{
	set_property(odf_reader::_property(L"svg:rotate",angle));
	if (impl_->object_description_.svg_rect_)
	{
		_rect r = impl_->object_description_.svg_rect_.get();
		//r.x_-=r.width_/2;
		//r.y_-=r.height_/2;

		impl_->object_description_.svg_rect_= r;
	}
}
void xlsx_drawing_context::set_scale(double cx_pt, double cy_pt)
{
	if (impl_->object_description_.svg_rect_)
	{
		_rect r = impl_->object_description_.svg_rect_.get();
		r.x_*=cx_pt;
		r.y_*=cy_pt;

		impl_->object_description_.svg_rect_= r;
	}
}
void xlsx_drawing_context::set_anchor(std::wstring anchor, double x_pt, double y_pt)
{
	impl_->object_description_.anchor_		= anchor;
	impl_->object_description_.anchor_x_	= x_pt;
    impl_->object_description_.anchor_y_	= y_pt;
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
std::wstring xlsx_drawing_context::add_hyperlink(std::wstring const & ref,bool object)
{
	++hlinks_size_;
	std::wstring hId=std::wstring(L"hId") + boost::lexical_cast<std::wstring>(hlinks_size_);
	
	_hlink_desc desc={hId, ref, object};
	impl_->object_description_.hlinks_.push_back(desc);

	return hId;
}

void xlsx_drawing_context::end_drawing()
{
}

void xlsx_drawing_context::start_shape(int type)
{
	impl_->object_description_.type_ = type; //2,3... 
}

void xlsx_drawing_context::start_object_ole()
{
	impl_->object_description_.use_image_replace_ = true;
}

void xlsx_drawing_context::end_object_ole()
{
}
void xlsx_drawing_context::end_shape()
{
	impl_->shapes_.push_back(impl_->object_description_);
	default_set();
}
void xlsx_drawing_context::start_image(std::wstring const & path)
{
	impl_->object_description_.type_ = 0; //frame 
	impl_->object_description_.xlink_href_ = path; 
}
void xlsx_drawing_context::start_chart(std::wstring const & path)
{
	impl_->object_description_.type_ = 0; //frame 
	impl_->object_description_.xlink_href_ = path; 
}

void xlsx_drawing_context::end_image()
{
    impl_->images_.push_back(impl_->object_description_);
	default_set();
}
void xlsx_drawing_context::end_chart()
{
    impl_->charts_.push_back(impl_->object_description_);
	default_set();
}
bool xlsx_drawing_context::empty() const
{
    return impl_->empty();
}

void xlsx_drawing_context::process_images(xlsx_table_metrics & table_metrics)
{
	int pos_replaicement=0;
	
    BOOST_FOREACH(drawing_object_description & pic, impl_->images_)
    {
		pos_replaicement= pic.xlink_href_.find(L"ObjectReplacements"); 
		if (pos_replaicement <0 || pic.use_image_replace_)//оригинал, а не заменяемый объект
		{
			_xlsx_drawing drawing=_xlsx_drawing();			
			drawing.fill = pic.fill_;
			
			process_common_properties(pic,drawing,table_metrics);

			drawing.fill.bitmap = oox_bitmap_fill::create();
			drawing.fill.type = 2;
			
			_CP_OPT(std::wstring) sTextContent;
			GetProperty(pic.additional_,L"text-content",sTextContent);

			if (sTextContent)//в ms office на картинке нельзя сделать надпись - меняем тип на рект с заливкой картинкой
			{
				drawing.type = mediaitems::typeShape;
				drawing.sub_type = 2;//rect
			}
			else
				drawing.type = mediaitems::typeImage;

			drawing.id = impl_->next_rId();
			
			drawing.name = pic.draw_name_;

			std::wstring ref;/// это ссылка на выходной внешний объект
			bool isMediaInternal = false;
			

			std::wstring fileName = odf_packet_path_ + FILE_SEPARATOR_STR +  pic.xlink_href_;			
			drawing.fill.bitmap->bCrop  = odf_reader::parse_clipping(pic.clipping_string_,fileName,drawing.fill.bitmap->cropRect);
			drawing.fill.bitmap->bStretch = true;

			drawing.fill.bitmap->rId = impl_->get_mediaitems().add_or_find(pic.xlink_href_, mediaitems::typeImage, isMediaInternal, ref);		

			if (drawing.type == mediaitems::typeShape)
			{
				impl_->add_drawing(isMediaInternal, drawing.fill.bitmap->rId, ref, mediaitems::typeImage);//собственно это не объект, а доп рел и ref объекта
			
				isMediaInternal=true;
				std::wstring rId = impl_->get_mediaitems().add_or_find(L"", mediaitems::typeShape, isMediaInternal, ref);
				impl_->add_drawing(drawing, isMediaInternal, rId, ref, mediaitems::typeShape);//объект

			}else
			{
				impl_->add_drawing(drawing, isMediaInternal, drawing.fill.bitmap->rId , ref, mediaitems::typeImage);//объект
			}
		}
    }
}

void xlsx_drawing_context::process_charts(xlsx_table_metrics & table_metrics)
{
    BOOST_FOREACH(drawing_object_description & pic, impl_->charts_)
    {	
		_xlsx_drawing drawing=_xlsx_drawing();
		
		drawing.type = mediaitems::typeChart;
        drawing.name = pic.draw_name_;
		drawing.id = impl_->next_rId();

        std::wstring ref;
        bool isMediaInternal = true;
        drawing.chartId = impl_->get_mediaitems().add_or_find(pic.xlink_href_, mediaitems::typeChart, isMediaInternal, ref);
        
		process_common_properties(pic,drawing,table_metrics);

        impl_->add_drawing(drawing, isMediaInternal, drawing.chartId, ref, mediaitems::typeChart);
    }
}

void xlsx_drawing_context::process_common_properties(drawing_object_description & pic,_xlsx_drawing & drawing,xlsx_table_metrics & table_metrics)
{
	xlsx_table_position from, to;
	
	process_position_properties(pic,table_metrics,from,to);

    drawing.from.type = xlsx_drawing_position::from;
    drawing.from.position.col = from.col;
    drawing.from.position.colOff = static_cast<size_t>(odf_types::length(from.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
    drawing.from.position.row = from.row;
    drawing.from.position.rowOff = static_cast<size_t>(odf_types::length(from.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));

    drawing.to.type = xlsx_drawing_position::to;
    drawing.to.position.col = to.col;
    drawing.to.position.colOff = static_cast<size_t>(odf_types::length(to.colOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
    drawing.to.position.row = to.row;
    drawing.to.position.rowOff = static_cast<size_t>(odf_types::length(to.rowOff, odf_types::length::pt).get_value_unit(odf_types::length::emu));
    
	if (pic.svg_rect_)
	{
		//todooo непонятно что делать с отрицательными значениями
		int val = 0.5 + odf_types::length(pic.svg_rect_.get().x_, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.x = val;
		
		val = 0.5 + odf_types::length(pic.svg_rect_.get().y_, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.y = val;

		val = 0.5 + odf_types::length(pic.svg_rect_.get().width_, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.cx = val;

		val = .5 + odf_types::length(pic.svg_rect_.get().height_, odf_types::length::pt).get_value_unit(odf_types::length::emu);
		if (val >=0)	drawing.cy = val;
	}
	
	drawing.additional=pic.additional_;

	drawing.hlinks=pic.hlinks_;
}
void xlsx_drawing_context::process_position_properties(drawing_object_description & pic,xlsx_table_metrics & table_metrics,xlsx_table_position & from,xlsx_table_position & to)
{
	size_t last_col=0, last_row=0;

	std::vector<std::wstring> distance;
	boost::algorithm::split(distance,pic.anchor_, boost::algorithm::is_any_of(L" ."), boost::algorithm::token_compress_on);
	
	double x=0,y=0,cx=0,cy=0;

	if (pic.svg_rect_)
	{
		x  = pic.svg_rect_.get().x_;
		y  = pic.svg_rect_.get().y_;
		cx = pic.svg_rect_.get().width_;
		cy = pic.svg_rect_.get().height_;
	}
	to = table_metrics.calc(x + cx, y + cy);

	if (distance.size()>1)
	{
		//атрибуты размера игнорируются
		int ind_cell=distance.size()-1;
		getCellAddressInv(distance[ind_cell],last_col,last_row);

		xlsx_table_position to_anchor = table_metrics.calc(last_col,last_row,pic.anchor_x_,pic.anchor_y_); //-можно и не считать :) , но проверим ...

		table_metrics.update_pt(last_col,last_row, pic.anchor_x_, pic.anchor_y_);

		x = pic.anchor_x_ - cx;
		y = pic.anchor_y_ - cy;

		if (pic.svg_rect_)
		{
			_rect r_old = pic.svg_rect_.get();
			_rect r ={cx,cy,x,y};
			//pic.svg_rect_.get().x_ =	pic.anchor_x_ - cx;
			pic.svg_rect_/*.get().y_*/ = r;//	pic.anchor_y_ - cy;
	
		}

		to = to_anchor;
	}
	from = table_metrics.calc(x, y);
}

void xlsx_drawing_context::process_shapes(xlsx_table_metrics & table_metrics)
{
    BOOST_FOREACH(drawing_object_description & pic, impl_->shapes_)
    {
		_xlsx_drawing drawing=_xlsx_drawing();
		drawing.fill = pic.fill_;

         std::wstring ref;
        bool isMediaInternal = true;

		drawing.type = mediaitems::typeShape;
		drawing.name = pic.draw_name_;

 		const size_t id = impl_->next_rId();
		
		if (drawing.fill.bitmap)
		{
			drawing.fill.bitmap->rId = impl_->get_mediaitems().add_or_find(drawing.fill.bitmap->xlink_href_, mediaitems::typeImage, isMediaInternal, ref);
			impl_->add_drawing(isMediaInternal, drawing.fill.bitmap->rId, ref, mediaitems::typeImage);//собственно это не объект, а доп рел и ref объекта
		}
		std::wstring rId = impl_->get_mediaitems().add_or_find(L"", mediaitems::typeShape, isMediaInternal, ref);
		drawing.id = id;
       
		process_common_properties(pic,drawing,table_metrics);

		drawing.sub_type = pic.type_;

		impl_->add_drawing(drawing, isMediaInternal, rId, ref, mediaitems::typeShape);
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
