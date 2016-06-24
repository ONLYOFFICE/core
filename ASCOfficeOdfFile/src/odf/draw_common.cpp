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
#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/odf/odf_document.h>

#include "serialize_elements.h"

#include "draw_common.h"

#include "style_paragraph_properties.h"

#include "datatypes/length.h"
#include "datatypes/borderstyle.h"
#include "odfcontext.h"

/////////////////////////////////////////////////////////////////////////////////
#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../DesktopEditor/raster/Metafile/MetaFile.h"

namespace _image_file_
{
    bool GetResolution(const wchar_t* fileName, int & Width, int &Height, CApplicationFonts	* appFonts)
	{
		CBgraFrame image;
		MetaFile::CMetaFile meta_file(appFonts);

		if ( meta_file.LoadFromFile(fileName))
		{
			double dX = 0, dY = 0, dW = 0, dH = 0;
			meta_file.GetBounds(&dX, &dY, &dW, &dH);
			
			Width  = dW;
			Height = dH;
		}
		else if ( image.OpenFile(fileName, 0 ))
		{
			Width  = image.get_Width();
			Height = image.get_Height();

			return true;
		}


		return false;
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {


int get_value_emu(const _CP_OPT(length) & len)
{
    return static_cast<int>(len.get_value_or( length(0, length::pt) ).get_value_unit( length::emu ));
}

int get_value_emu(double pt)
{
    return static_cast<int>((pt* 360000 * 2.54) / 72);
} 
bool parse_clipping(std::wstring strClipping,std::wstring fileName, double_4 & clip_rect, CApplicationFonts	* appFonts)
{
    memset(clip_rect, 0, 4*sizeof(double));

	if (strClipping.length() <1 || fileName.length()<1)return false;
		
	//<top>, <right>, <bottom>, <left> - http://www.w3.org/TR/2001/REC-xsl-20011015/xslspec.html#clip

	bool bEnableCrop = false;

	std::vector<std::wstring>	Points;
	std::vector<length>			Points_pt;
	
	boost::algorithm::split(Points,strClipping, boost::algorithm::is_any_of(L" ,"), boost::algorithm::token_compress_on);
	BOOST_FOREACH(std::wstring const & p, Points)
	{
		Points_pt.push_back(length::parse(p) );
		
		if (Points_pt.back().get_value() > 0.00001) bEnableCrop = true;
	}

	if (!bEnableCrop) return false;

	int fileWidth=0,fileHeight=0;

	if (!_image_file_::GetResolution(fileName.data(), fileWidth, fileHeight, appFonts) || fileWidth<1 || fileHeight<1)	return false;



	if (Points_pt.size() > 3)//если другое количество точек .. попозже
	{
		float dpi_ = 96.;
		clip_rect[0] = dpi_ * Points_pt[3].get_value_unit(length::inch);
		clip_rect[1] = dpi_ * Points_pt[0].get_value_unit(length::inch);
		clip_rect[2] = dpi_ * Points_pt[1].get_value_unit(length::inch);
		clip_rect[3] = dpi_ * Points_pt[2].get_value_unit(length::inch);

		// в проценты
		clip_rect[0] = clip_rect[0]*100/fileWidth;
		clip_rect[2] = clip_rect[2]*100/fileWidth;
		clip_rect[1] = clip_rect[1]*100/fileHeight;
		clip_rect[3] = clip_rect[3]*100/fileHeight;

		if (clip_rect[0]<0.01  && clip_rect[1]<0.01  && clip_rect[2]<0.01   && clip_rect[3]<0.01)
			return false;
		return true;
	}
	return false;
}

_CP_OPT(border_widths) GetBorderLineWidths(const graphic_format_properties & graphicProperties, BorderSide borderSide)
{
    _CP_OPT(border_widths) widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_;
    if (widths)
        return widths;
    
    switch(borderSide)
    {
    case sideTop:       widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_top_; break;
    case sideBottom:    widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_bottom_; break;
    case sideLeft:      widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_left_; break;
    case sideRight:     widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_right_; break;
    default:    
        widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_top_;
        if (widths)
            break;
        else
            widths = graphicProperties.common_border_line_width_attlist_.style_border_line_width_bottom_;
    }
    return widths;
}
_CP_OPT(length) GetConsistentBorderValue(const graphic_format_properties & graphicProperties, const border_style & borderStyle, BorderSide borderSide)
{
    if ((borderStyle.get_style() ==  border_style::double_))
    {
        _CP_OPT(border_widths) borderWidths = GetBorderLineWidths(graphicProperties, borderSide);
        if (borderWidths)
            return length(borderWidths->get_summ_unit( length::pt), length::pt);
    }
    else
    {
        if (borderStyle.initialized())
        {
            if (borderStyle.is_none())
                return _CP_OPT(length)();
            else
                return borderStyle.get_length();
        }
    }
    return _CP_OPT(length)();
}
int GetMargin(const graphic_format_properties & graphicProperties, BorderSide borderSide)//emu
{
	int margin = 0;
    _CP_OPT(length_or_percent) marginVal;

    switch(borderSide)
    {
		case sideTop:       marginVal = graphicProperties.common_vertical_margin_attlist_.fo_margin_top_; break;
		case sideBottom:    marginVal = graphicProperties.common_vertical_margin_attlist_.fo_margin_bottom_; break;
		case sideLeft:      marginVal = graphicProperties.common_horizontal_margin_attlist_.fo_margin_left_; break;
		case sideRight:     marginVal = graphicProperties.common_horizontal_margin_attlist_.fo_margin_right_; break;
    }

    if (marginVal && marginVal->get_type() == length_or_percent::Length)
    {
        margin = get_value_emu(marginVal->get_length());
    }
    
    return margin;
}
int Compute_BorderWidth(const graphic_format_properties & graphicProperties, BorderSide borderSide)
{
    _CP_OPT(border_style)	borderValue;
    _CP_OPT(length)			lengthValue;

    switch(borderSide)
    {
    case sideTop:       borderValue = graphicProperties.common_border_attlist_.fo_border_top_; break;
    case sideBottom:    borderValue = graphicProperties.common_border_attlist_.fo_border_bottom_; break;
    case sideLeft:      borderValue = graphicProperties.common_border_attlist_.fo_border_left_; break;
    case sideRight:     borderValue = graphicProperties.common_border_attlist_.fo_border_right_; break;    
    }

    if (!borderValue)
        borderValue = graphicProperties.common_border_attlist_.fo_border_;                

    if (borderValue)
        lengthValue = GetConsistentBorderValue(graphicProperties, *borderValue, borderSide);

    return get_value_emu(lengthValue);
}

void Compute_HatchFill(draw_hatch * image_style,oox::oox_hatch_fill_ptr fill)
{
	int style =0;
	if (image_style->draw_style_)style = image_style->draw_style_->get_type();

	int angle = (int)(0.5 + *image_style->draw_rotation_/10.);
	
	if (image_style->draw_color_)fill->color_ref = image_style->draw_color_->get_hex_value();

	double size =0;
	if (image_style->draw_distance_)size = (image_style->draw_distance_->get_value_unit(length::cm));
	
	switch(style)
	{
	case hatch_style::single:
		if		(angle == 0		|| angle == 180)	fill->preset = L"dkHorz";	
		else if (angle == 90	|| angle == 270)	fill->preset = L"dkVert";
		else if ((angle>180 && angle <270)
					|| (angle>0 && angle <90)) 		fill->preset = L"dkUpDiag";
		else										fill->preset = L"dkDnDiag";
		break;
	case hatch_style::doublee:
		if (angle ==0 || angle == 90  || angle== 180 || angle == 270)
		{
			if (size > 0.2)	fill->preset = L"lgGrid";
			else			fill->preset = L"smGrid";
		}
		else fill->preset = L"openDmnd";
		break;
	case hatch_style::triple:
		fill->preset = L"pct5";
		break;
	}
}
void Compute_GradientFill(draw_gradient * image_style,oox::oox_gradient_fill_ptr fill)
{
	int style =0;
	if (image_style->draw_style_)style = image_style->draw_style_->get_type();

	if (image_style->draw_angle_)fill->angle = 90 - *image_style->draw_angle_/10.;
	if (fill->angle < 0) fill->angle +=360;

	oox::oox_gradient_fill::_color_position point={};
	switch(style)
	{
	case gradient_style::linear:
		{	
			fill->style = 0;
		
			point.pos = 0;
			if (image_style->draw_start_color_)point.color_ref = image_style->draw_start_color_->get_hex_value();
			if (image_style->draw_start_intensity_)point.opacity = image_style->draw_start_intensity_->get_value();

			fill->colors.push_back(point);

			point.pos = 100;
			if (image_style->draw_end_color_)point.color_ref = image_style->draw_end_color_->get_hex_value();
			if (image_style->draw_end_intensity_)point.opacity = image_style->draw_end_intensity_->get_value();
	
			fill->colors.push_back(point);
		}break;
	case gradient_style::axial:
		{
			fill->style = 0;
			
			point.pos = 0;
			if (image_style->draw_end_color_)point.color_ref = image_style->draw_end_color_->get_hex_value();
			if (image_style->draw_end_intensity_)point.opacity = image_style->draw_end_intensity_->get_value();
	
			fill->colors.push_back(point);

			point.pos = 50;
			if (image_style->draw_start_color_)point.color_ref = image_style->draw_start_color_->get_hex_value();
			if (image_style->draw_start_intensity_)point.opacity = image_style->draw_start_intensity_->get_value();

			fill->colors.push_back(point);

			point.pos = 100;
			if (image_style->draw_end_color_)point.color_ref = image_style->draw_end_color_->get_hex_value();
			if (image_style->draw_end_intensity_)point.opacity = image_style->draw_end_intensity_->get_value();
	
			fill->colors.push_back(point);
		}break;
	case gradient_style::radial:
	case gradient_style::ellipsoid:
	case gradient_style::square:
	case gradient_style::rectangular:
		{
			if (style == gradient_style::radial || style == gradient_style::ellipsoid)	fill->style = 2;
			if (style == gradient_style::square )		fill->style = 1;
			if (style == gradient_style::rectangular)	fill->style = 3;
			
			point.pos = 0;
			if (image_style->draw_end_color_)point.color_ref = image_style->draw_end_color_->get_hex_value();
			if (image_style->draw_end_intensity_)point.opacity = image_style->draw_end_intensity_->get_value();

			fill->colors.push_back(point);

			point.pos = 100;
			if (image_style->draw_start_color_)point.color_ref = image_style->draw_start_color_->get_hex_value();
			if (image_style->draw_start_intensity_)point.opacity = image_style->draw_start_intensity_->get_value();
	
			fill->colors.push_back(point);

			if (image_style->draw_cx_)//хохма - у мс в конвертилке из open-office перепутаны l & r !!!
			{
				fill->rect[0]=image_style->draw_cx_->get_value();
				fill->rect[2]=100-image_style->draw_cx_->get_value();
			}
			else
			{
				fill->rect[0]=0;
				fill->rect[2]=100;
			}
			if (image_style->draw_cy_)
			{
				fill->rect[1]=image_style->draw_cy_->get_value();
				fill->rect[3]=100-image_style->draw_cy_->get_value();
			}
			else
			{
				fill->rect[1]=0;
				fill->rect[3]=100;
			}
		}break;
	}
}


void Compute_GraphicFill(const common_draw_fill_attlist & props, const office_element_ptr & style_image, styles_lite_container &styles, oox::_oox_fill & fill)
{
	if (fill.type < 1)	fill.type = 0; 

	if (props.draw_opacity_) fill.opacity = props.draw_opacity_->get_value();
	if (props.draw_opacity_name_)
	{
		const std::wstring style_name = L"opacity:" + *props.draw_opacity_name_;
		if (office_element_ptr style = styles.find_by_style_name(style_name))
		{
			if (draw_opacity * image_style = dynamic_cast<draw_opacity *>(style.get()))
			{	
				//увы и ах но ms  не поддерживает градиентную прозрачность - сделаем средненькую
				if (image_style->draw_start_ && image_style->draw_end_)
				{
					fill.opacity = (image_style->draw_start_->get_value() + image_style->draw_end_->get_value())/2.;
				}
				else if (image_style->draw_start_)fill.opacity = image_style->draw_start_->get_value();
				else if (image_style->draw_end_)fill.opacity = image_style->draw_end_->get_value();
			}
		}
	}
	if (props.draw_image_opacity_) 
		fill.opacity = props.draw_image_opacity_->get_value();

////////////////////////////////////////////////////////////
	if (props.draw_fill_color_)
	{
		fill.solid = oox::oox_solid_fill::create();
		fill.solid->color = props.draw_fill_color_->get_hex_value();
		if (fill.type==0)fill.type = 1;	//в этом случае тип может и не быть задан явно
	}
	if (props.draw_fill_image_name_)
	{
		const std::wstring style_name = L"bitmap:" + *props.draw_fill_image_name_;
		if (office_element_ptr style = styles.find_by_style_name(style_name))
		{
			if (draw_fill_image * fill_image = dynamic_cast<draw_fill_image *>(style.get()))
			{			
				fill.bitmap = oox::oox_bitmap_fill::create();
				fill.bitmap->xlink_href_ = fill_image->xlink_attlist_.href_.get_value_or(L"");
				fill.bitmap->bTile = true;
			}
		}
	}

	if (style_image)
	{
		if (style_background_image * image = dynamic_cast<style_background_image *>(style_image.get()))
		{
			if ((image) && (image->common_xlink_attlist_))
			{
				fill.type	= 2;
				fill.bitmap = oox::oox_bitmap_fill::create();
				fill.bitmap->xlink_href_ = image->common_xlink_attlist_->href_.get_value_or(L"");
				if (image->style_repeat_)
				{
					switch(image->style_repeat_->get_type())
					{
						case style_repeat::Repeat	:	fill.bitmap->bTile = true;		break;
						case style_repeat::Stretch	:	fill.bitmap->bStretch = true;	break;
					}
				}
				if (image->draw_opacity_) 
				{
					fill.opacity = image->draw_opacity_->get_value();
				}
			}
		}
	}

	if (fill.bitmap)
	{
		if (props.style_repeat_)
		{
			switch(props.style_repeat_->get_type())
			{
				case style_repeat::Repeat	:	fill.bitmap->bTile = true;		break;
				case style_repeat::Stretch	:	fill.bitmap->bStretch = true;	break;
			}
		}
		else
		{
			if (props.draw_fill_image_width_ && props.draw_fill_image_height_)
			{
				if (props.draw_fill_image_width_->get_type() == odf_types::length_or_percent::Percent && 
					props.draw_fill_image_height_->get_type() == odf_types::length_or_percent::Percent)
				{
					if (props.draw_fill_image_width_->get_percent().get_value()  > 99.9  && 
						props.draw_fill_image_height_->get_percent().get_value() > 99.9  && 
						props.draw_fill_image_width_->get_percent().get_value()  < 100.1 && 
						props.draw_fill_image_height_->get_percent().get_value() < 100.1 )
					{
						fill.bitmap->bStretch	= true;
						fill.bitmap->bTile		= false;
					}
				}
			}
		}
	}
	if (props.draw_fill_gradient_name_)
	{
		const std::wstring style_name = L"gradient:" + *props.draw_fill_gradient_name_;
		if (office_element_ptr style = styles.find_by_style_name(style_name))
		{
			if (draw_gradient * image_style = dynamic_cast<draw_gradient *>(style.get()))
			{			
				fill.type	= 3;
				fill.gradient = oox::oox_gradient_fill::create();

				Compute_GradientFill(image_style, fill.gradient);
			}
		}
	}
	if (props.draw_fill_hatch_name_)
	{
		const std::wstring style_name = L"hatch:" + *props.draw_fill_hatch_name_;
		if (office_element_ptr style = styles.find_by_style_name(style_name))
		{
			if (draw_hatch * image_style = dynamic_cast<draw_hatch *>(style.get()))
			{			
				fill.type	= 4;
				fill.hatch = oox::oox_hatch_fill::create();

				Compute_HatchFill(image_style, fill.hatch);
			}
		}
		if ((fill.hatch) && (props.draw_fill_color_))
		{
			fill.hatch->color_back_ref = props.draw_fill_color_->get_hex_value();
		}	
	}
	if (props.draw_fill_)
		fill.type = props.draw_fill_->get_type();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


const wchar_t * draw_a::ns = L"draw";
const wchar_t * draw_a::name = L"a";
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// draw-a-attlist
void draw_a::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}
void draw_a::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_xlink_attlist_.add_attributes(Attributes);

    CP_APPLY_ATTR(L"office:name", office_name_, std::wstring(L""));
    CP_APPLY_ATTR(L"office:target-frame-name", office_target_frame_name_);
    CP_APPLY_ATTR(L"text:style-name", text_style_name_, style_ref(L""));
    CP_APPLY_ATTR(L"text:visited-style-name", text_visited_style_name_, style_ref(L""));

}

void draw_a::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    Context.get_drawing_context().add_hyperlink(common_xlink_attlist_.href_.get_value_or(L""),true);
				//стиль на текст не нужен ..текста то нет - ссылка с объекта
   
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->xlsx_convert(Context);
    }
}
void draw_a::pptx_convert(oox::pptx_conversion_context & Context)
{
   Context.get_slide_context().add_hyperlink(common_xlink_attlist_.href_.get_value_or(L""),true);//стиль на текст не нужен ..текста то нет - ссылка с объекта
   
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->pptx_convert(Context);
    }
}
void draw_a::docx_convert(oox::docx_conversion_context & Context) 
{
	std::wstring rId = Context.add_hyperlink(common_xlink_attlist_.href_.get_value_or(L""), true);//гиперлинк с объекта, а не с текста .. 
	
	BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);
    }

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void parse_string_to_points(std::wstring str, std::vector<length> & Points)
{
	std::vector<std::wstring> Points_s;

	boost::algorithm::split(Points_s,str, boost::algorithm::is_any_of(L" ,"), boost::algorithm::token_compress_on);

	BOOST_FOREACH(std::wstring const & p, Points_s)
	{
		Points.push_back(length::parse(p) );
	}
}

void oox_convert_transforms(std::wstring transformStr,std::vector<odf_reader::_property> & additional)
{
	std::vector<std::wstring> transforms;
	
	boost::algorithm::split(transforms,transformStr, boost::algorithm::is_any_of(L")"), boost::algorithm::token_compress_on);
	
	BOOST_FOREACH(std::wstring const & t, transforms)
	{			
        //_CP_LOG << "[info] : transform = " << t << L"\n";
		std::vector<std::wstring> transform;
		
		boost::algorithm::split(transform,t, boost::algorithm::is_any_of(L"("), boost::algorithm::token_compress_on);
		
		if (transform.size()>1)//тока с аргументами
		{
			int res=0;
			if ((res = transform[0].find(L"translate"))>=0)//перемещение
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);

				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = 0;
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					//Context.get_drawing_context().set_translate(x_pt,y_pt);
					additional.push_back(_property(L"svg:translate_x",x_pt));
					additional.push_back(_property(L"svg:translate_y",y_pt));
				}
			}
			else if ((res = transform[0].find(L"scale"))>=0)//масштабирование
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);
				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = x_pt; 
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					//Context.get_drawing_context().set_scale(x_pt,y_pt);
					additional.push_back(_property(L"svg:scale_x",x_pt));
					additional.push_back(_property(L"svg:scale_y",y_pt));
				}
			}
			else if ((res = transform[0].find(L"rotate"))>=0)//вращение
			{
				double angle =  boost::lexical_cast<double>(transform[1]);
				additional.push_back(_property(L"svg:rotate",angle));
			}
		}
	}
}


void xlsx_convert_transforms(std::wstring transformStr, oox::xlsx_conversion_context & Context)
{
	std::vector<std::wstring> transforms;
	
	boost::algorithm::split(transforms,transformStr, boost::algorithm::is_any_of(L")"), boost::algorithm::token_compress_on);
	
	BOOST_FOREACH(std::wstring const & t, transforms)
	{			
        //_CP_LOG << "[info] : transform = " << t << L"\n";
		std::vector<std::wstring> transform;
		boost::algorithm::split(transform,t, boost::algorithm::is_any_of(L"("), boost::algorithm::token_compress_on);

		if (transform.size()>1)//тока с аргументами
		{
			int res=0;
			if ((res = transform[0].find(L"translate"))>=0)//перемещение
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);

				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = 0;
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					Context.get_drawing_context().set_translate(x_pt,y_pt);
				}
			}
			else if ((res = transform[0].find(L"scale"))>=0)//масштабирование
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);
				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = x_pt; 
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					Context.get_drawing_context().set_scale(x_pt,y_pt);
				}
			}
			else if ((res = transform[0].find(L"rotate"))>=0)//вращение
			{
				Context.get_drawing_context().set_rotate(boost::lexical_cast<double>(transform[1]));
			}
			else if ((res = transform[0].find(L"skewX"))>=0)//сдвиг
			{
				double angle =  boost::lexical_cast<double>(transform[1]);
				Context.get_drawing_context().set_property(_property(L"svg:skewX",angle));
			}
			else if ((res = transform[0].find(L"skewY"))>=0)//сдвиг
			{
				double angle =  boost::lexical_cast<double>(transform[1]);
				Context.get_drawing_context().set_property(_property(L"svg:skewY",angle));
			}
		}
	}
}

void pptx_convert_transforms(std::wstring transformStr, oox::pptx_conversion_context & Context)
{
	std::vector<std::wstring> transforms;
	
	boost::algorithm::split(transforms,transformStr, boost::algorithm::is_any_of(L")"), boost::algorithm::token_compress_on);
	
	BOOST_FOREACH(std::wstring const & t, transforms)
	{			
        //_CP_LOG << "[info] : transform = " << t << L"\n";
		std::vector<std::wstring> transform;
		boost::algorithm::split(transform,t, boost::algorithm::is_any_of(L"("), boost::algorithm::token_compress_on);

		if (transform.size()>1)//тока с аргументами
		{
			int res=0;
			if ((res = transform[0].find(L"translate"))>=0)//перемещение
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);

				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = 0;
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					Context.get_slide_context().set_translate(x_pt,y_pt);
				}
			}
			else if ((res = transform[0].find(L"scale"))>=0)//масштабирование
			{
				std::vector<length> Points ;
				parse_string_to_points(transform[1], Points);
				if (Points.size()>0)
				{
					double x_pt = Points[0].get_value_unit(length::pt);
					double y_pt = x_pt; 
					if (Points.size()>1)y_pt = Points[1].get_value_unit(length::pt);//ее может не быть

					Context.get_slide_context().set_scale(x_pt,y_pt);
				}
			}
			else if ((res = transform[0].find(L"rotate"))>=0)//вращение
			{
				Context.get_slide_context().set_rotate( boost::lexical_cast<double>(transform[1]));
			}
			else if ((res = transform[0].find(L"skewX"))>=0)//вращение
			{
				double angle =  boost::lexical_cast<double>(transform[1]);
				Context.get_slide_context().set_property(_property(L"svg:skewX",angle));
			}
			else if ((res = transform[0].find(L"skewY"))>=0)//вращение
			{
				double angle =  boost::lexical_cast<double>(transform[1]);
				Context.get_slide_context().set_property(_property(L"svg:skewY",angle));
			}
		}
	}
}

}
}
