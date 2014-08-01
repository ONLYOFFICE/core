#include "stdAfx.h"

#include "Converter.h"
#include "../utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "odf_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

#include "style_text_properties.h"
#include "style_paragraph_properties.h"

#include <DocxFormat\Logic\Vml.h>

#include "VmlShapeTypes2Oox.h"

namespace Oox2Odf
{

void OoxConverter::convert(OOX::Vml::CVmlShapeElements *unknown)
{
	if (unknown == NULL) return;

	switch(unknown->getType())
	{
		case OOX::et_v_image:
		{
			OOX::Vml::CImage *vml = static_cast<OOX::Vml::CImage*>(unknown);
			convert(vml);
		}break;		
		//et_v_f, // <v:f>
		//et_v_formulas, // <v:formulas>
		//et_v_group, // <v:group>
		//et_v_h, // <v:h>
		//et_v_handles, // <v:handles>
		//et_v_shadow, // <v:shadow>
		default:
		{
			std::wstringstream ss;
			ss << L"[warning] :  no convert vml element(" << unknown->getType() << L")\n";
			_CP_LOG(error) << ss.str();
		}
	}
}
void OoxConverter::convert(OOX::Vml::CShapeType *vml_shape_type)
{
	if (vml_shape_type == NULL) return;
	//custom shape 

	if (vml_shape_type->m_oCoordSize.IsInit())
	{
		odf_context()->drawing_context()->set_position(vml_shape_type->m_oCoordSize->GetX(), vml_shape_type->m_oCoordSize->GetY());
	}

	for (long i=0 ; i < vml_shape_type->m_arrItems.GetSize();i++)
	{
		convert(vml_shape_type->m_arrItems[i]);
	}

	//o:spt
		//nullable<CString>                                  m_oAdj;
		//nullable<SimpleTypes::Vml::CVmlPath>               m_oPath;
		//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse> m_oMaster;
	//m_arrItems
	//CVmlAttributes
}

void OoxConverter::convert(SimpleTypes::Vml::CCssStyle *vml_style)
{
	if (vml_style == NULL) return;

	_CP_OPT(double) width_pt, height_pt, x, y;

	for (long i=0; i < vml_style->m_arrProperties.GetSize(); i++)
	{
		switch(vml_style->m_arrProperties[i].get_Type())
		{
		case SimpleTypes::Vml::cssptHeight:
			height_pt = vml_style->m_arrProperties[i].get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptWidth:
			width_pt = vml_style->m_arrProperties[i].get_Value().oValue.dValue;
			break;
		case SimpleTypes::Vml::cssptMarginBottom:
			odf_context()->drawing_context()->set_margin_bottom(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			break;
		case SimpleTypes::Vml::cssptMarginLeft:
			odf_context()->drawing_context()->set_margin_left (vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			break;
		case SimpleTypes::Vml::cssptMarginRight:
			odf_context()->drawing_context()->set_margin_right(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			break;
		case SimpleTypes::Vml::cssptMarginTop:
			odf_context()->drawing_context()->set_margin_top(vml_style->m_arrProperties[i].get_Value().oValue.dValue);
			break;
		}
	}
	odf_context()->drawing_context()->set_drawings_rect(x, y, width_pt, height_pt);
}
void OoxConverter::convert(OOX::Vml::CShape *vml_shape)
{
	if (vml_shape == NULL) return;

	for (long i=0 ; i < vml_shape->m_arrItems.GetSize();i++)
	{
		convert(vml_shape->m_arrItems[i]);
	}
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
		
	if (pathImage.GetLength() < 1)return;
	_gdi_graphics_::GetResolution(pathImage, Width, Height);

	odf_context()->drawing_context()->start_area_properies();
		odf_context()->drawing_context()->start_bitmap_style();
		
			odf_context()->drawing_context()->set_bitmap_link(string2std_string(pathImage));
			odf_context()->drawing_context()->set_image_style_repeat(1);//stretch

		odf_context()->drawing_context()->end_bitmap_style();
	odf_context()->drawing_context()->end_area_properies();
}

void OoxConverter::convert(OOX::Vml::CArc *vml_arc)
{
	if (vml_arc == NULL) return;

	convert(vml_arc->m_oStyle.GetPointer());
}
void OoxConverter::convert(OOX::Vml::CBackground *vml_background)
{
	if (vml_background == NULL) return;
}

void OoxConverter::convert(OOX::Vml::CFill	*vml_fill)
{
	if (vml_fill == NULL) return;

	odf_context()->drawing_context()->start_area_properies();

	odf_context()->drawing_context()->end_area_properies();
}
void OoxConverter::convert(OOX::Vml::CLine	*vml_line)
{
	if (vml_line == NULL) return;
	
	convert(vml_line->m_oStyle.GetPointer());
}
void OoxConverter::convert(OOX::Vml::COval	*vml_oval)
{
	if (vml_oval == NULL) return;
	
	convert(vml_oval->m_oStyle.GetPointer());
	
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
	//nullable<SimpleTypes::Vml::CVmlPath>                      m_oV;

	if (vml_path->m_oV.IsInit())
	{
	}
}
void OoxConverter::convert(OOX::Vml::CPolyLine	*vml_polyline)
{
	if (vml_polyline == NULL) return;
	
	convert(vml_polyline->m_oStyle.GetPointer());
}
void OoxConverter::convert(OOX::Vml::CRect	*vml_rect)
{
	if (vml_rect == NULL) return;
	
	
	convert(vml_rect->m_oStyle.GetPointer());	
}

void OoxConverter::convert(OOX::Vml::CRoundRect	*vml_roundrect)
{
	if (vml_roundrect == NULL) return;
	
	convert(vml_roundrect->m_oStyle.GetPointer());
}

void OoxConverter::convert(OOX::Vml::CCurve	*vml_curve)
{
	if (vml_curve == NULL) return;

	convert(vml_curve->m_oStyle.GetPointer());
}
void OoxConverter::convert(OOX::Vml::CShadow *vml_shadow)
{
	if (vml_shadow == NULL) return;
	if (vml_shadow->m_oOn.GetValue() == false) return;

	std::wstring hexColor = string2std_string(vml_shadow->m_oColor.ToString());
	_CP_OPT(double) opacity = 100 - 100 * vml_shadow->m_oOpacity.GetValue();

	double offset_x = vml_shadow->m_oOffset.IsXinPoints() ? vml_shadow->m_oOffset.GetX() : -1;
	double offset_y = vml_shadow->m_oOffset.IsYinPoints() ? vml_shadow->m_oOffset.GetY() : -1;

	odf_context()->drawing_context()->set_shadow(1,hexColor,opacity,offset_x, offset_y);
	
}
void OoxConverter::convert(OOX::Vml::CStroke *vml_stroke)
{
	if (vml_stroke == NULL) return;
	if (vml_stroke->m_oOn.GetValue() == false) return;

			//nullable<SimpleTypes::CColorType<SimpleTypes::colortypeBlack>>        m_oColor2;
			//SimpleTypes::Vml::CVmlDashStyle<SimpleTypes::Vml::vmldashstyleSolid>  m_oDahsStyle;
			//SimpleTypes::CStrokeEndCap<SimpleTypes::strokeendcapFlat>             m_oEndCap;
			//SimpleTypes::CFillType<SimpleTypes::filltypeSolid, 0>                 m_oFillType;
			//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>                    m_oForceDash;
			//SimpleTypes::CStrokeJoinStyle<SimpleTypes::strokejoinstyleRound>      m_oJoinStyle;

	odf_context()->drawing_context()->start_line_properies();

	//switch(vml_stroke->m_oFillType){}

	std::wstring hexColor = string2std_string(vml_stroke->m_oColor.ToString());
	if (hexColor.length() < 1)hexColor = L"000000";
	odf_context()->drawing_context()->set_solid_fill(hexColor);

	odf_context()->drawing_context()->set_opacity(100 - vml_stroke->m_oOpacity.GetValue() * 100);
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
	odf_context()->drawing_context()->end_line_properies();
}
void OoxConverter::convert(OOX::Vml::CTextbox *vml_textbox)
{
	if (vml_textbox == NULL) return;
	if (vml_textbox->m_oTxtbxContent.IsInit() == false) return;
	if (vml_textbox->m_oTxtbxContent->m_arrItems.GetSize() < 1) return;

	
	convert(vml_textbox->m_oStyle.GetPointer());
	
	DocxConverter *docx_converter = dynamic_cast<DocxConverter*>(this);

	odf_context()->start_text_context();
	for (long i=0 ; i < vml_textbox->m_oTxtbxContent->m_arrItems.GetSize();i++)
	{
		if (docx_converter)docx_converter->convert(vml_textbox->m_oTxtbxContent->m_arrItems[i]);
	}
	odf_context()->drawing_context()->set_text( odf_context()->text_context());
	odf_context()->end_text_context();	


	odf_context()->drawing_context()->set_textarea_wrap(true);


}
void OoxConverter::convert(OOX::Vml::CTextPath *vml_textpath)
{
	if (vml_textpath == NULL) return;
}

void OoxConverter::convert(OOX::VmlWord::CWrap	*vml_wrap)
{
	if (vml_wrap == NULL) return;

	if (vml_wrap->m_oType.IsInit() == false) return;

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
void OoxConverter::convert(OOX::Vml::CGroup * vml_group)
{
	if (vml_group == NULL) return;

	odf_context()->drawing_context()->start_group(L"",1);

	if (vml_group->m_oShapeGroup.IsInit())
	{
		convert(vml_group->m_oShapeGroup.GetPointer());
	}
	else
	{			
		odf_context()->drawing_context()->start_drawing();
	
		if (vml_group->m_oShapeOval.IsInit())
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeEllipse);
			
			convert(vml_group->m_oShapeOval.GetPointer());
			convert(vml_group->m_oShape.GetPointer());	
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapeLine.IsInit())
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);
			
			convert(vml_group->m_oShapeLine.GetPointer());
			convert(vml_group->m_oShape.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapePolyLine.IsInit())
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);
			
			convert(vml_group->m_oShapePolyLine.GetPointer());
			convert(vml_group->m_oShape.GetPointer());	
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapeArc.IsInit())
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);
			
			convert(vml_group->m_oShapeArc.GetPointer());
			convert(vml_group->m_oShape.GetPointer());	
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapeRoundRect.IsInit())
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRoundRect);
			
			convert(vml_group->m_oShapeRoundRect.GetPointer());
			convert(vml_group->m_oShape.GetPointer());	
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapeCurve.IsInit())
		{
			odf_context()->drawing_context()->start_shape(1000);
			
			convert(vml_group->m_oShapeCurve.GetPointer());
			convert(vml_group->m_oShape.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (vml_group->m_oShapeType.IsInit())
		{
			OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(vml_group->m_oShapeType->m_oSpt.GetValue());

			odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));
			
			convert(vml_group->m_oShapeType.GetPointer());
			convert(vml_group->m_oShape.GetPointer());	
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else // and oox_pic->m_oShapeRect
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);			
				OoxConverter::convert(vml_group->m_oShape.GetPointer()); 			
			odf_context()->drawing_context()->end_shape(); 
		}

		odf_context()->drawing_context()->end_drawing();
	}

	odf_context()->drawing_context()->end_group();
}
}