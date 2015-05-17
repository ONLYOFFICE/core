#include "Converter.h"
#include "../utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "../OdfFormat/odf_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"

#include "../../../Common/DocxFormat/source/DocxFormat/Logic/Vml.h"

#include "VmlShapeTypes2Oox.h"

namespace Oox2Odf
{

void OoxConverter::convert(OOX::Vml::CShapeType *vml_shape_type)
{
	if (vml_shape_type == NULL) return;
	//custom shape 

	
	//m_oPreferRelative//типо можно менять размер 

	for (unsigned int i=0 ; i < vml_shape_type->m_arrItems.size();i++)
	{
		convert(vml_shape_type->m_arrItems[i]);
	}

	if (vml_shape_type->m_oAdj.IsInit())//настройка дл фигуры заданной формулами
	{
	}

	//o:spt
		//nullable<CString>                                  m_oAdj;
		//nullable<SimpleTypes::Vml::CVmlPath>               m_oPath;
		//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse> m_oMaster;
	//m_arrItems
	//CVmlCommonElements
}

void OoxConverter::convert(SimpleTypes::Vml::CCssStyle *vml_style, bool group)
{
	if (vml_style == NULL) return;

	_CP_OPT(double) width_pt, height_pt, x, y;

	bool anchor_set = false;

	for (unsigned int i=0; i < vml_style->m_arrProperties.size(); i++)
	{
		if (vml_style->m_arrProperties[i] == NULL) continue;

		switch(vml_style->m_arrProperties[i]->get_Type())
		{
		case SimpleTypes::Vml::cssptHeight:
			height_pt = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptWidth:
			width_pt = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptTop:
			y = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptLeft:
			x = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptMarginBottom:
			//odf_context()->drawing_context()->set_margin_bottom(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			//x = vml_style->m_arrProperties[i].get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptMarginLeft:
			//odf_context()->drawing_context()->set_margin_left (vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			x = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptMarginRight:
			//odf_context()->drawing_context()->set_margin_right(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			break;
		case SimpleTypes::Vml::cssptMarginTop:
			//odf_context()->drawing_context()->set_margin_top(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			y = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
			break;
////////////////////////////////////////////////////////////////
		case SimpleTypes::Vml::cssptMsoPositionHorizontal:
			switch(vml_style->m_arrProperties[i]->get_Value().eMsoPosHor)
			{
			case SimpleTypes::Vml::cssmsoposhorAbsolute:	odf_context()->drawing_context()->set_horizontal_pos(2); break;
			case SimpleTypes::Vml::cssmsoposhorLeft:		odf_context()->drawing_context()->set_horizontal_pos(2); break;
			case SimpleTypes::Vml::cssmsoposhorCenter:		odf_context()->drawing_context()->set_horizontal_pos(0); break;
			case SimpleTypes::Vml::cssmsoposhorRight:		odf_context()->drawing_context()->set_horizontal_pos(4); break;
			case SimpleTypes::Vml::cssmsoposhorInside:		odf_context()->drawing_context()->set_horizontal_pos(1); break;
			case SimpleTypes::Vml::cssmsoposhorOutside :	odf_context()->drawing_context()->set_horizontal_pos(3); break;
			}
			break;
		case SimpleTypes::Vml::cssptMsoPositionHorizontalRelative:
			switch(vml_style->m_arrProperties[i]->get_Value().eMsoPosHorRel)
			{
			case SimpleTypes::Vml::cssmsoposhorrelMargin:	odf_context()->drawing_context()->set_horizontal_rel(2); break; 
			case SimpleTypes::Vml::cssmsoposhorrelPage:		odf_context()->drawing_context()->set_horizontal_rel(6); break;
			case SimpleTypes::Vml::cssmsoposhorrelText:		odf_context()->drawing_context()->set_horizontal_rel(1); break;
			case SimpleTypes::Vml::cssmsoposhorrelChar:		odf_context()->drawing_context()->set_horizontal_rel(0); break;
			}
			anchor_set = true;
			break;
		case SimpleTypes::Vml::cssptMsoPositionVertical:
			switch(vml_style->m_arrProperties[i]->get_Value().eMsoPosVer)
			{
			case SimpleTypes::Vml::cssmsoposverAbsolute:	odf_context()->drawing_context()->set_vertical_pos(2); break;
			case SimpleTypes::Vml::cssmsoposverTop:			odf_context()->drawing_context()->set_vertical_pos(4); break;
			case SimpleTypes::Vml::cssmsoposverCenter:		odf_context()->drawing_context()->set_vertical_pos(1); break;
			case SimpleTypes::Vml::cssmsoposverBottom:		odf_context()->drawing_context()->set_vertical_pos(0); break;
			case SimpleTypes::Vml::cssmsoposverInside:		odf_context()->drawing_context()->set_vertical_pos(2); break;//??
			case SimpleTypes::Vml::cssmsoposverOutside:		odf_context()->drawing_context()->set_vertical_pos(3); break;//??
			}
			break;
		case SimpleTypes::Vml::cssptMsoPositionVerticalRelative:
			switch(vml_style->m_arrProperties[i]->get_Value().eMsoPosVerRel)
			{
			case SimpleTypes::Vml::cssmsoposverrelMargin:	odf_context()->drawing_context()->set_vertical_rel(3); break;//3 ???
			case SimpleTypes::Vml::cssmsoposverrelPage:		odf_context()->drawing_context()->set_vertical_rel(5); break;
			case SimpleTypes::Vml::cssmsoposverrelText:		odf_context()->drawing_context()->set_vertical_rel(6); break;
			case SimpleTypes::Vml::cssmsoposverrelLine:		odf_context()->drawing_context()->set_vertical_rel(2); break;
			}
			anchor_set= true;
			break;
		case SimpleTypes::Vml::cssptZIndex:
			{
				if (vml_style->m_arrProperties[i]->get_Value().oZIndex.eType == SimpleTypes::Vml::csszindextypeOrder)
				odf_context()->drawing_context()->set_z_order(vml_style->m_arrProperties[i]->get_Value().oZIndex.nOrder);
			}break;
		case SimpleTypes::Vml::cssptPosition:
			{
				switch (vml_style->m_arrProperties[i]->get_Value().ePosition)
				{
				case SimpleTypes::Vml::csspositionStatic:	break;
				case SimpleTypes::Vml::csspositionAbsolute:	break;
				case SimpleTypes::Vml::csspositionRelative:	break;
				}
			}break;
		case SimpleTypes::Vml::cssptRotation:
			{
				if (group)
					odf_context()->drawing_context()->set_group_rotate(180 + vml_style->m_arrProperties[i]->get_Value().dValue);
				else
					odf_context()->drawing_context()->set_rotate(360 - vml_style->m_arrProperties[i]->get_Value().dValue);
			}break;
		case SimpleTypes::Vml::cssptFlip:
			{
				switch(vml_style->m_arrProperties[i]->get_Value().eFlip)
				{
				case SimpleTypes::Vml::cssflipX:
					odf_context()->drawing_context()->set_flip_H(true); break;
				case SimpleTypes::Vml::cssflipY:
					odf_context()->drawing_context()->set_flip_V(true); break;
				case SimpleTypes::Vml::cssflipXY:
					odf_context()->drawing_context()->set_flip_H(true); 
					odf_context()->drawing_context()->set_flip_V(true); break;
				case SimpleTypes::Vml::cssflipYX:
					odf_context()->drawing_context()->set_flip_V(true); 
					odf_context()->drawing_context()->set_flip_H(true); break;
				}
			}break;
		case SimpleTypes::Vml::cssptLayoutFlow:
			{
				switch(vml_style->m_arrProperties[i]->get_Value().eLayoutFlow )
				{
				case SimpleTypes::Vml::csslayoutflowHorizontal            : break;
				case SimpleTypes::Vml::csslayoutflowVertical              :odf_context()->drawing_context()->set_textarea_writing_mode(3); break;
				case SimpleTypes::Vml::csslayoutflowVerticalIdeographic   :odf_context()->drawing_context()->set_textarea_writing_mode(0); break;
				case SimpleTypes::Vml::csslayoutflowHorizontalIdeographic :break;
				}
			}break;
		case SimpleTypes::Vml::cssptVTextAnchor:
			switch(vml_style->m_arrProperties[i]->get_Value().eVTextAnchor)
			{
				case SimpleTypes::Vml::cssvtextanchorTop                  :	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorMiddle               :	odf_context()->drawing_context()->set_textarea_vertical_align(1); break;
				case SimpleTypes::Vml::cssvtextanchorBottom               :	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopCenter            :	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorMiddleCenter         :	odf_context()->drawing_context()->set_textarea_vertical_align(1); break;
				case SimpleTypes::Vml::cssvtextanchorBottomCenter         :	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopBaseline          :	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorBottomBaseline       :	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopCenterBaseline    :	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorBottomCenterBaseline :	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
			}break;
		case SimpleTypes::Vml::cssptMsoFitShapeToText:
			if (vml_style->m_arrProperties[i]->get_Value().bValue == true)
			{
				odf_context()->drawing_context()->set_text_box_min_size(0,0);
			}break;
		}
	}
	if (group)
	{
		_CP_OPT(double) not_set;
		odf_context()->drawing_context()->set_group_size(width_pt, height_pt, not_set ,not_set);

		odf_context()->drawing_context()->set_group_position(x, y, not_set , not_set );
	}
	else
	{
		odf_context()->drawing_context()->set_size		(width_pt, height_pt);		
		odf_context()->drawing_context()->set_position	(x, y);

		if (x && y && !anchor_set) 
			odf_context()->drawing_context()->set_anchor(2);

	}

	//if (width_pt && height_pt)
	//	odf_context()->drawing_context()->set_viewBox(width_pt.get(), height_pt.get());
}
void OoxConverter::convert(OOX::Vml::CShape *vml_shape)
{
	if (vml_shape == NULL) return; 

	if (vml_shape->m_oAllowInCell.GetValue())
	{
	}
	odf_context()->drawing_context()->set_overlap(vml_shape->m_oAllowOverlap.GetValue());
	
	if (vml_shape->m_oPath.IsInit())
	{
		odf_context()->drawing_context()->set_path(string2std_string(vml_shape->m_oPath->GetValue()));
	}

	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_shape);
	convert(vml_common);

}
void OoxConverter::convert(OOX::Vml::CImage *vml_image)
{
	if (vml_image == NULL) return;
}
void OoxConverter::convert(OOX::Vml::CImageData *vml_image_data)
{
	if (vml_image_data == NULL) return;
		
	CString pathImage;
	double Width=0, Height = 0;

	if (vml_image_data->m_rId.IsInit())
	{
		CString sID = vml_image_data->m_rId->GetValue();		
		pathImage = find_link_by_id(sID,1);
	}
		
	//что именно нужно заливка объекта или картинка - разрулится внутри drawing_context
	if (pathImage.GetLength() < 1)return;
	_gdi_graphics_::GetResolution(pathImage, Width, Height);

	odf_context()->drawing_context()->start_area_properties();
		odf_context()->drawing_context()->start_bitmap_style();
		
			odf_context()->drawing_context()->set_bitmap_link(string2std_string(pathImage));
			odf_context()->drawing_context()->set_image_style_repeat(1);//stretch

			double gain = vml_image_data->m_oGain.GetValue();
			if (gain > 1)
				odf_context()->drawing_context()->set_opacity(gain/1000.);

			if (vml_image_data->m_oCropLeft.IsInit() || vml_image_data->m_oCropRight.IsInit()
				|| vml_image_data->m_oCropTop.IsInit() || vml_image_data->m_oCropBottom.IsInit())
			{
				double l_pt = 0,t_pt =0 ,r_pt = 0, b_pt = 0;
			
				if (vml_image_data->m_oCropLeft.IsInit())	l_pt = vml_image_data->m_oCropLeft->GetValue()	* Width;
				if (vml_image_data->m_oCropTop.IsInit())	t_pt = vml_image_data->m_oCropTop->GetValue()	* Height;
				if (vml_image_data->m_oCropRight.IsInit())	r_pt = vml_image_data->m_oCropRight->GetValue() * Width;
				if (vml_image_data->m_oCropBottom.IsInit()) b_pt = vml_image_data->m_oCropBottom->GetValue()* Height;

				odf_context()->drawing_context()->set_image_client_rect(l_pt ,t_pt ,r_pt, b_pt);
			}

		odf_context()->drawing_context()->end_bitmap_style();
	odf_context()->drawing_context()->end_area_properties();
}

void OoxConverter::convert(OOX::Vml::CArc *vml_arc)
{
	if (vml_arc == NULL) return;

	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_arc);
	convert(vml_common);
}
void OoxConverter::convert(OOX::Vml::CBackground *vml_background)
{
	if (vml_background == NULL) return;
}

void OoxConverter::convert(OOX::Vml::CFill	*vml_fill)
{
	if (vml_fill == NULL) return;

	odf_context()->drawing_context()->start_area_properties();

	if (vml_fill->m_rId.IsInit())
	{
		//bitmap fill
		odf_context()->drawing_context()->start_bitmap_style();
		{
			double Width=0, Height = 0;
			CString sID = vml_fill->m_rId->GetValue();
			CString pathImage = find_link_by_id(sID,1);

			if (pathImage.GetLength() > 0)
			{
				odf_context()->drawing_context()->set_bitmap_link(string2std_string(pathImage));
				_gdi_graphics_::GetResolution(pathImage, Width, Height);
			}
			odf_context()->drawing_context()->set_image_style_repeat(1);
		}
		odf_context()->drawing_context()->end_bitmap_style();
	}
	else
	{
		switch (vml_fill->m_oType.GetValue())
		{
		case SimpleTypes::filltypeGradient       :
		case SimpleTypes::filltypeGradientCenter :
		case SimpleTypes::filltypeGradientRadial :
		case SimpleTypes::filltypeGradientUnscaled:
		{
			odf_context()->drawing_context()->start_gradient_style();
				if (SimpleTypes::filltypeGradient		== vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf::gradient_style::linear);
				if (SimpleTypes::filltypeGradientRadial == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf::gradient_style::radial);
				if (SimpleTypes::filltypeGradientCenter == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf::gradient_style::axial);
				if (SimpleTypes::filltypeGradientUnscaled == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf::gradient_style::square);

				_CP_OPT(double) no_set;
				if (vml_fill->m_oColor.IsInit())
					odf_context()->drawing_context()->set_gradient_start(string2std_string(vml_fill->m_oColor->ToString()),no_set);
				if (vml_fill->m_oColor2.IsInit())
					odf_context()->drawing_context()->set_gradient_end(string2std_string(vml_fill->m_oColor2->ToString()),no_set);

				odf_context()->drawing_context()->set_gradient_center(vml_fill->m_oFocusPosition.GetX(), vml_fill->m_oFocusPosition.GetY());
			
			odf_context()->drawing_context()->end_gradient_style();
		}break;
		case SimpleTypes::filltypePattern:
		{
			odf_context()->drawing_context()->start_hatch_style();
				if (vml_fill->m_oColor2.IsInit())
					odf_context()->drawing_context()->set_hatch_line_color(string2std_string(vml_fill->m_oColor2->ToString()));
				if (vml_fill->m_oColor.IsInit())
					odf_context()->drawing_context()->set_hatch_area_color(string2std_string(vml_fill->m_oColor->ToString()));
				else
					odf_context()->drawing_context()->set_hatch_area_color(L"#ffffff");

			odf_context()->drawing_context()->end_hatch_style();

		}break;
		case SimpleTypes::filltypeSolid:
		default:
			if (vml_fill->m_oColor.IsInit())
				odf_context()->drawing_context()->set_solid_fill(string2std_string(vml_fill->m_oColor->ToString()));
			break;
		}
	}
//--------------------------------------------------------------------------------------------------------------------
	if (vml_fill->m_oOpacity.IsInit() && vml_fill->m_oOpacity2.IsInit() )
	{
		odf_context()->drawing_context()->start_opacity_style();
			odf_context()->drawing_context()->set_opacity_start(100 - vml_fill->m_oOpacity->GetValue() * 100);
			odf_context()->drawing_context()->set_opacity_end  (100 - vml_fill->m_oOpacity2->GetValue() * 100);
			//.....
		odf_context()->drawing_context()->end_opacity_style();
	}
	else if (vml_fill->m_oOpacity.IsInit()) 
	{
		odf_context()->drawing_context()->set_opacity(100 - vml_fill->m_oOpacity->GetValue() * 100);
	}
	else if (vml_fill->m_oOpacity2.IsInit()) 
	{
		odf_context()->drawing_context()->set_opacity(100 - vml_fill->m_oOpacity2->GetValue() * 100);
	}

	odf_context()->drawing_context()->end_area_properties();
}
void OoxConverter::convert(OOX::Vml::CLine	*vml_line)
{
	if (vml_line == NULL) return;
	
	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_line);
	convert(vml_common);

	_CP_OPT(double) x = vml_line->m_oFrom.GetX();
	_CP_OPT(double) y = vml_line->m_oFrom.GetY();

	_CP_OPT(double) x1	= vml_line->m_oTo.GetX();
	_CP_OPT(double) y1	= vml_line->m_oTo.GetY();

	odf_context()->drawing_context()->set_position_line(x, y, x1, y1);

	odf_context()->drawing_context()->set_anchor(2);
	
}
void OoxConverter::convert(OOX::Vml::COval	*vml_oval)
{
	if (vml_oval == NULL) return;
	
	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_oval);
	convert(vml_common);
}
void OoxConverter::convert(OOX::Vml::CPath	*vml_path)
{
	if (vml_path == NULL) return;
	
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oArrowOk;
	//nullable<CString>                                         m_oConnectAngles;
	//nullable<CString>                                         m_oConnectLocs;
	//SimpleTypes::CConnectType<SimpleTypes::connecttypeNone>   m_oConnectType;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oExtrusionOk;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oFillOk;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oGradientShapeOk;
	//nullable<CString>                                         m_oId;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oInsetPenOk;
	//SimpleTypes::Vml::CVml_Vector2D_Units                     m_oLimo;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oShadowOk;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oStrokeOk;
	//nullable<SimpleTypes::Vml::CVml_Polygon2D>                m_oTextBoxRect;
	//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oTextPathOk;
}
void OoxConverter::convert(OOX::Vml::CPolyLine	*vml_polyline)
{
	if (vml_polyline == NULL) return;
	
	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_polyline);
	convert(vml_common);
}
void OoxConverter::convert(OOX::Vml::CRect	*vml_rect)
{
	if (vml_rect == NULL) return;
	
	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_rect);
	convert(vml_common);
}

void OoxConverter::convert(OOX::Vml::CRoundRect	*vml_roundrect)
{
	if (vml_roundrect == NULL) return;
	
	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_roundrect);
	convert(vml_common);
}

void OoxConverter::convert(OOX::Vml::CCurve	*vml_curve)
{
	if (vml_curve == NULL) return;

	OOX::Vml::CVmlCommonElements *vml_common = static_cast<OOX::Vml::CVmlCommonElements *>(vml_curve);
	convert(vml_common);
}
void OoxConverter::convert(OOX::Vml::CShadow *vml_shadow)
{
	if (vml_shadow == NULL) return;
	if (vml_shadow->m_oOn.GetValue() == false) return;

	std::wstring hexColor = string2std_string(vml_shadow->m_oColor.ToString());
	_CP_OPT(double) opacity;
	
	if (vml_shadow->m_oOpacity.IsInit()) opacity = 100 - 100 * vml_shadow->m_oOpacity->GetValue();

	double offset_x = vml_shadow->m_oOffset.IsXinPoints() ? vml_shadow->m_oOffset.GetX() : -1;
	double offset_y = vml_shadow->m_oOffset.IsYinPoints() ? vml_shadow->m_oOffset.GetY() : -1;

	odf_context()->drawing_context()->set_shadow(1,hexColor,opacity,offset_x, offset_y);
	
}
void OoxConverter::convert(OOX::Vml::CStroke *vml_stroke)
{
	if (vml_stroke == NULL) return;
	if (vml_stroke->m_oOn.GetValue() == false) return;
	if (vml_stroke->m_oColor.IsInit() == false) return; // ?????

			//nullable<SimpleTypes::CColorType<SimpleTypes::colortypeBlack>>        m_oColor2;
			//SimpleTypes::Vml::CVmlDashStyle<SimpleTypes::Vml::vmldashstyleSolid>  m_oDahsStyle;
			//SimpleTypes::CStrokeEndCap<SimpleTypes::strokeendcapFlat>             m_oEndCap;
			//SimpleTypes::CFillType<SimpleTypes::filltypeSolid, 0>                 m_oFillType;
			//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>                    m_oForceDash;
			//SimpleTypes::CStrokeJoinStyle<SimpleTypes::strokejoinstyleRound>      m_oJoinStyle;

	odf_context()->drawing_context()->start_line_properties();

	//switch(vml_stroke->m_oFillType){}

	if (vml_stroke->m_oColor.IsInit())
	{
		std::wstring hexColor = string2std_string(vml_stroke->m_oColor->ToString());
		if (hexColor.length() < 1)hexColor = L"000000";
		odf_context()->drawing_context()->set_solid_fill(hexColor);
	}

	if (vml_stroke->m_oOpacity.IsInit())
	{
		odf_context()->drawing_context()->set_opacity(100 - vml_stroke->m_oOpacity->GetValue() * 100);
	}
	odf_context()->drawing_context()->set_line_width(vml_stroke->m_oWeight.GetValue());

	switch(vml_stroke->m_oStartArrow.GetValue())
	{
		case SimpleTypes::strokearrowtypeBlock:
			odf_context()->drawing_context()->set_line_head(SimpleTypes::lineendtypeStealth,vml_stroke->m_oStartArrowLength.GetValue(),vml_stroke->m_oStartArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeClassic:
			odf_context()->drawing_context()->set_line_head(SimpleTypes::lineendtypeArrow,vml_stroke->m_oStartArrowLength.GetValue(),vml_stroke->m_oStartArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeDiamond:
			odf_context()->drawing_context()->set_line_head(SimpleTypes::lineendtypeDiamond,vml_stroke->m_oStartArrowLength.GetValue(),vml_stroke->m_oStartArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeOpen:
			odf_context()->drawing_context()->set_line_head(SimpleTypes::lineendtypeTriangle,vml_stroke->m_oStartArrowLength.GetValue(),vml_stroke->m_oStartArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeOval:
			odf_context()->drawing_context()->set_line_head(SimpleTypes::lineendtypeOval,vml_stroke->m_oStartArrowLength.GetValue(),vml_stroke->m_oStartArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeNone:
		default:
				break;
	}
	switch(vml_stroke->m_oEndArrow.GetValue())
	{
		case SimpleTypes::strokearrowtypeBlock:
			odf_context()->drawing_context()->set_line_tail(SimpleTypes::lineendtypeStealth,vml_stroke->m_oEndArrowLength.GetValue(),vml_stroke->m_oEndArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeClassic:
			odf_context()->drawing_context()->set_line_tail(SimpleTypes::lineendtypeArrow,vml_stroke->m_oEndArrowLength.GetValue(),vml_stroke->m_oEndArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeDiamond:
			odf_context()->drawing_context()->set_line_tail(SimpleTypes::lineendtypeDiamond,vml_stroke->m_oEndArrowLength.GetValue(),vml_stroke->m_oEndArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeOpen:
			odf_context()->drawing_context()->set_line_tail(SimpleTypes::lineendtypeTriangle,vml_stroke->m_oEndArrowLength.GetValue(),vml_stroke->m_oEndArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeOval:
			odf_context()->drawing_context()->set_line_tail(SimpleTypes::lineendtypeOval,vml_stroke->m_oEndArrowLength.GetValue(),vml_stroke->m_oEndArrowWidth.GetValue()); break;
		case SimpleTypes::strokearrowtypeNone:
		default:
				break;
	}	
	odf_context()->drawing_context()->end_line_properties();
}
void OoxConverter::convert(OOX::Vml::CTextbox *vml_textbox)
{
	if (vml_textbox == NULL) return;
	if (vml_textbox->m_oTxtbxContent.IsInit() == false) return;
	if (vml_textbox->m_oTxtbxContent->m_arrItems.size() < 1) return;

	convert(vml_textbox->m_oStyle.GetPointer());

	if (vml_textbox->m_oInsetMode.GetValue() == SimpleTypes::insetmodeCustom)
	{
		odf_context()->drawing_context()->set_textarea_padding(odf::length(vml_textbox->m_oInset.GetLeft(0), odf::length::pt).get_value_unit(odf::length::cm),
													odf::length(vml_textbox->m_oInset.GetTop(0), odf::length::pt).get_value_unit(odf::length::cm), 
													odf::length(vml_textbox->m_oInset.GetRight(0), odf::length::pt).get_value_unit(odf::length::cm),
													odf::length(vml_textbox->m_oInset.GetBottom(0), odf::length::pt).get_value_unit(odf::length::cm));	
	}

	odf_context()->drawing_context()->set_textarea_wrap(true);
	
	DocxConverter *docx_converter = dynamic_cast<DocxConverter*>(this);

	odf_context()->start_text_context();
	{
		for (unsigned int i=0 ; i < vml_textbox->m_oTxtbxContent->m_arrItems.size();i++)
		{
			if (docx_converter)docx_converter->convert(vml_textbox->m_oTxtbxContent->m_arrItems[i]);
		}
		odf_context()->drawing_context()->set_text( odf_context()->text_context());
	}
	odf_context()->end_text_context();	

}
void OoxConverter::convert(OOX::Vml::CTextPath *vml_textpath)
{
	if (vml_textpath == NULL) return; //это типо фигурный текст
	if (vml_textpath->m_sString.IsInit()==false) return;

	//DocxConverter *docx_converter = dynamic_cast<DocxConverter*>(this);

	odf_context()->start_text_context();

	odf_context()->text_context()->start_paragraph(false);
		odf_context()->text_context()->start_span(false);
			odf_context()->text_context()->add_text_content(string2std_string(vml_textpath->m_sString.get()));
		odf_context()->text_context()->end_span();
	odf_context()->text_context()->end_paragraph();

	odf_context()->drawing_context()->set_text( odf_context()->text_context());
	odf_context()->end_text_context();	


	odf_context()->drawing_context()->set_textarea_wrap(true);
}

void OoxConverter::convert(OOX::VmlWord::CWrap	*vml_wrap)
{
	if (vml_wrap == NULL) return;

	if (vml_wrap->m_oType.IsInit())
	{
		switch(vml_wrap->m_oType->GetValue())
		{
			case SimpleTypes::wraptypeNone  :
			{
				odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::None);
			}break;
			case SimpleTypes::wraptypeSquare:
			{
				if (vml_wrap->m_oSide.IsInit())
				{
					if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideLargest)
						odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Dynamic);
					else if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideLeft)
						odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Left);
					else if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideRight)
						odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Right);
					else
						odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
				}
			}break;
			case SimpleTypes::wraptypeThrough:
			{
				odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::RunThrough);
			}break;
			case SimpleTypes::wraptypeTight:
			{
				odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
			}break;
			case SimpleTypes::wraptypeTopAndBottom:
			{
				odf_context()->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
			}break;
		}
	}
	if (vml_wrap->m_oAnchorX.IsInit())
	{
		switch(vml_wrap->m_oAnchorX->GetValue())
		{
		case SimpleTypes::horizontalanchorMargin: 
			odf_context()->drawing_context()->set_horizontal_rel(4); break;
		case SimpleTypes::horizontalanchorPage:			
			odf_context()->drawing_context()->set_horizontal_rel(6); break;
		case SimpleTypes::horizontalanchorText:
			odf_context()->drawing_context()->set_horizontal_rel(1); break;//???? paragraph ????			
		case SimpleTypes::horizontalanchorChar: 			
		default :     
			odf_context()->drawing_context()->set_horizontal_rel(0);
		}
	}
	if (vml_wrap->m_oAnchorY.IsInit())
	{
		switch(vml_wrap->m_oAnchorY->GetValue())
		{
		case SimpleTypes::verticalanchorMargin: 		
			odf_context()->drawing_context()->set_vertical_rel(0); break;
		case SimpleTypes::verticalanchorPage:  
			odf_context()->drawing_context()->set_anchor(0); //page
			odf_context()->drawing_context()->set_vertical_rel(5); break;
		case SimpleTypes::verticalanchorText:				
			odf_context()->drawing_context()->set_vertical_rel(0); break;
		case SimpleTypes::verticalanchorLine: 		
		default :
			odf_context()->drawing_context()->set_vertical_rel(2);//Line
		}
	}
}
void OoxConverter::convert(OOX::Vml::CVmlCommonElements *vml_common)
{
	if (vml_common == NULL) return;

	convert(vml_common->m_oStyle.GetPointer());
	
	odf_context()->drawing_context()->start_line_properties();
	{
		if (vml_common->m_oStroked.IsInit() && vml_common->m_oStroked->GetValue() == SimpleTypes::booleanFalse)
			odf_context()->drawing_context()->set_no_fill();
		else
		{
			if (vml_common->m_oStrokeWeight.IsInit() || (vml_common->m_oStroked.IsInit() && vml_common->m_oStroked->GetValue()))
			{
				if (vml_common->m_oStrokeWeight.IsInit()) 
					odf_context()->drawing_context()->set_line_width(vml_common->m_oStrokeWeight->ToPoints());
			}
			if (vml_common->m_oStrokeColor.IsInit())
			{
				unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
				ucR = vml_common->m_oStrokeColor->Get_R(); 
				ucB = vml_common->m_oStrokeColor->Get_B(); 
				ucG = vml_common->m_oStrokeColor->Get_G(); 
				SimpleTypes::CHexColor<> *oRgbColor = new SimpleTypes::CHexColor<>(ucR,ucG,ucB);
				if (oRgbColor)
				{		
					odf_context()->drawing_context()->set_solid_fill(string2std_string(oRgbColor->ToString().Right(6)));
					delete oRgbColor;
				}			
			}
		}
	}
	odf_context()->drawing_context()->end_line_properties();
	if (vml_common->m_oFillColor.IsInit())
	{
		unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
		ucR = vml_common->m_oFillColor->Get_R(); 
		ucB = vml_common->m_oFillColor->Get_B(); 
		ucG = vml_common->m_oFillColor->Get_G(); 
		SimpleTypes::CHexColor<> *oRgbColor = new SimpleTypes::CHexColor<>(ucR,ucG,ucB);
		if (oRgbColor)
		{		
			odf_context()->drawing_context()->start_area_properties();
				odf_context()->drawing_context()->set_solid_fill(string2std_string(oRgbColor->ToString().Right(6)));
			odf_context()->drawing_context()->end_area_properties();
			delete oRgbColor;
		}
	}
	for (unsigned int i=0 ; i < vml_common->m_arrItems.size();i++)
	{
		convert(vml_common->m_arrItems[i]);
	}

	if (vml_common->m_oFilled.IsInit() && vml_common->m_oFilled->GetValue() == SimpleTypes::booleanFalse)
	{
		odf_context()->drawing_context()->start_area_properties();
			odf_context()->drawing_context()->set_no_fill();
		odf_context()->drawing_context()->end_area_properties();
	}
}
void OoxConverter::convert(OOX::Vml::CGroup *vml_group)
{
	if (vml_group == NULL) return;

	odf_context()->drawing_context()->start_group();
		
		convert(vml_group->m_oStyle.GetPointer(), true);
			
		if (vml_group->m_oCoordSize.IsInit())
		{
			odf_context()->drawing_context()->set_group_scale(vml_group->m_oCoordSize->GetX(), vml_group->m_oCoordSize->GetY());
		}	

		if (vml_group->m_oCoordOrigin.IsInit())
		{
			odf_context()->drawing_context()->set_group_shift(vml_group->m_oCoordOrigin->GetX(), vml_group->m_oCoordOrigin->GetY());
		}		

		for (unsigned int i=0; i < vml_group->m_arrItems.size(); i++)
		{
			if (vml_group->m_arrItems[i] == NULL) continue;

			if (vml_group->m_arrItems[i]->getType() == OOX::et_v_group)
			{
				OOX::Vml::CGroup * vml = static_cast<OOX::Vml::CGroup*>(vml_group->m_arrItems[i]);
				convert(vml);
				continue;
			}
			
			OOX::Vml::CVmlCommonElements * vml_common = dynamic_cast<OOX::Vml::CVmlCommonElements*>(vml_group->m_arrItems[i]);
			if (vml_common == NULL) continue; // не элемент

			odf_context()->drawing_context()->start_drawing();						
			
			switch(vml_group->m_arrItems[i]->getType())
			{
				case OOX::et_v_shape:
				{
					OOX::Vml::CShape * vml = static_cast<OOX::Vml::CShape*>(vml_group->m_arrItems[i]);
					bool bSet = false;
					if (vml)
					{
						OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(vml->m_oSpt.GetValue());
						if (sptType != OOX::Vml::SptType::sptNotPrimitive)
						{
							odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));
							bSet = true;
						}
						else if (vml->m_oConnectorType.GetValue() != SimpleTypes::connectortypeNone)
						{
							odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeStraightConnector1);
							odf_context()->drawing_context()->set_line_width(1.);
							bSet = true;
						}
						else if (vml->m_oPath.IsInit())
						{
							odf_context()->drawing_context()->start_shape(1001);
							odf_context()->drawing_context()->set_line_width(1.);
							bSet = true;
						}
					}
					if (!bSet)
					{
						odf_context()->drawing_context()->set_name(L"Rect");
						odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);			
					}
						OoxConverter::convert(vml);			
					odf_context()->drawing_context()->end_shape(); 
				}break;	
				case OOX::et_v_shapetype:
				{
					OOX::Vml::CShapeType * vml = static_cast<OOX::Vml::CShapeType*>(vml_group->m_arrItems[i]);
					OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(vml->m_oSpt.GetValue());
					odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));					
						OoxConverter::convert(vml);			
					odf_context()->drawing_context()->end_shape(); 
				}break;
				case OOX::et_v_oval:
				{
					OOX::Vml::COval * vml = static_cast<OOX::Vml::COval*>(vml_group->m_arrItems[i]);
					odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeEllipse);		
						OoxConverter::convert(vml);					
					odf_context()->drawing_context()->end_shape();
				}break;		
				case OOX::et_v_rect:
				{
					OOX::Vml::CRect * vml = static_cast<OOX::Vml::CRect*>(vml_group->m_arrItems[i]);
					odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);		
						OoxConverter::convert(vml);					
					odf_context()->drawing_context()->end_shape();
				}break;		
				case OOX::et_v_line:
				{
					OOX::Vml::CLine * vml = static_cast<OOX::Vml::CLine*>(vml_group->m_arrItems[i]);
					odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);		
						OoxConverter::convert(vml);					
					odf_context()->drawing_context()->end_shape();
				}break;	
				case OOX::et_v_curve:
				{
					OOX::Vml::CCurve * vml = static_cast<OOX::Vml::CCurve*>(vml_group->m_arrItems[i]);
					odf_context()->drawing_context()->start_shape(1000);		
						OoxConverter::convert(vml);					
					odf_context()->drawing_context()->end_shape();
				}break;	
				default: 
					convert(vml_group->m_arrItems[i]);
			}
			odf_context()->drawing_context()->end_drawing();
		}
	odf_context()->drawing_context()->end_group();
}
}