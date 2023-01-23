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
#include "Converter.h"
#include "../../Common/utils.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "../Format/odf_conversion_context.h"

#include "../Format/odf_text_context.h"
#include "../Format/odf_drawing_context.h"

#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"

#include "../../../OOXML/DocxFormat/Logic/Vml.h"
#include "../../../DesktopEditor/raster/BgraFrame.h"

#include "VmlShapeTypes2Oox.h"

namespace Oox2Odf
{

	void OoxConverter::convert(OOX::Vml::CShapeType *vml_shape_type)
	{
		if (vml_shape_type == NULL) return;

		//save cash to other shape with this type
		std::wstring strId = vml_shape_type->m_sId.get_value_or(L"");
		if (strId.empty()) vml_shape_type->m_sType.get_value_or(L"");

		if (false == strId.empty())
		{
			if (strId[0] == (wchar_t)('#'))
			{
				strId = strId.substr(1);
			}
			//if (m_mapShapeTypes.find(strId) == m_mapShapeTypes.end())//?? с затиранием ???
			{
				m_mapVmlShapeTypes.insert(std::make_pair(strId, vml_shape_type));
			}
		}
	}
	void OoxConverter::convert(OOX::Vml::CFormulas *vml_formulas)
	{
		if (vml_formulas == NULL) return;

		for (std::vector<OOX::Vml::CF*>::iterator it = vml_formulas->m_arrItems.begin(); it != vml_formulas->m_arrItems.end(); ++it)
		{
			OOX::Vml::CF *cf = dynamic_cast<OOX::Vml::CF *>(*it);
			if (cf == NULL) continue;

			//odf_context()->drawing_context()->add_formula(L"", cf->m_sEqn);
		}
	}

	void OoxConverter::convert(SimpleTypes::Vml::CCssStyle *vml_style)
	{
		if (vml_style == NULL) return;

		_CP_OPT(double) width, height, x, y, x2, y2; // in pt

		_CP_OPT(int) anchor_type_x, anchor_type_y;

		bool bPosition = false;
		bool bTextRelativeX = false, bTextRelativeY = false;

		for (size_t i = 0; i < vml_style->m_arrProperties.size(); i++)
		{
			if (vml_style->m_arrProperties[i] == NULL) continue;

			switch (vml_style->m_arrProperties[i]->get_Type())
			{
			case SimpleTypes::Vml::cssptHeight:
				height = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptWidth:
				width = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptTop:
				y = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptLeft:
				x = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptMarginBottom:
				y2 = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptMarginLeft:
				x = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptMarginRight:
				x2 = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
			case SimpleTypes::Vml::cssptMarginTop:
				y = vml_style->m_arrProperties[i]->get_Value().oValue.dValue;
				break;
				////////////////////////////////////////////////////////////////
			case SimpleTypes::Vml::cssptMsoPositionHorizontal:
			{
				switch (vml_style->m_arrProperties[i]->get_Value().eMsoPosHor)
				{
				case SimpleTypes::Vml::cssmsoposhorAbsolute:	odf_context()->drawing_context()->set_horizontal_pos(2); break;
				case SimpleTypes::Vml::cssmsoposhorLeft:		odf_context()->drawing_context()->set_horizontal_pos(2); break;
				case SimpleTypes::Vml::cssmsoposhorCenter:		odf_context()->drawing_context()->set_horizontal_pos(0); break;
				case SimpleTypes::Vml::cssmsoposhorRight:		odf_context()->drawing_context()->set_horizontal_pos(4); break;
				case SimpleTypes::Vml::cssmsoposhorInside:		odf_context()->drawing_context()->set_horizontal_pos(1); break;
				case SimpleTypes::Vml::cssmsoposhorOutside:	odf_context()->drawing_context()->set_horizontal_pos(3); break;
				}
			}break;
			case SimpleTypes::Vml::cssptMsoPositionHorizontalRelative:
			{
				switch (vml_style->m_arrProperties[i]->get_Value().eMsoPosHorRel)
				{
					case SimpleTypes::Vml::cssmsoposhorrelRightMargin:
					{
						anchor_type_x = 2;
						odf_context()->drawing_context()->set_horizontal_rel(5);
					}break;
					case SimpleTypes::Vml::cssmsoposhorrelLeftMargin:
					{
						anchor_type_x = 2;
						odf_context()->drawing_context()->set_horizontal_rel(4);
					}break;
					case SimpleTypes::Vml::cssmsoposhorrelMargin:
					{
						anchor_type_x = 2;
						odf_context()->drawing_context()->set_horizontal_rel(2);
					}break;
					case SimpleTypes::Vml::cssmsoposhorrelPage:
					{
						anchor_type_x = 0;
						odf_context()->drawing_context()->set_horizontal_rel(6);
					}break;
					case SimpleTypes::Vml::cssmsoposhorrelText:
					case SimpleTypes::Vml::cssmsoposhorrelChar:
					{
						bTextRelativeX = true;
						odf_context()->drawing_context()->set_horizontal_rel(0);
					}break;
				}
			}break;
			case SimpleTypes::Vml::cssptMsoPositionVertical:
				switch (vml_style->m_arrProperties[i]->get_Value().eMsoPosVer)
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
			{
				switch (vml_style->m_arrProperties[i]->get_Value().eMsoPosVerRel)
				{
					case SimpleTypes::Vml::cssmsoposverrelBottomMargin:
					{
						anchor_type_y = 2;
						odf_context()->drawing_context()->set_vertical_pos(0);
						odf_context()->drawing_context()->set_vertical_rel(3);
					}break;
					case SimpleTypes::Vml::cssmsoposverrelTopMargin:
					case SimpleTypes::Vml::cssmsoposverrelMargin:
					{
						anchor_type_y = 2;
						odf_context()->drawing_context()->set_vertical_rel(3);
					}break;
					case SimpleTypes::Vml::cssmsoposverrelPage:
					{
						anchor_type_y = 0;
						odf_context()->drawing_context()->set_vertical_rel(5);
					}break;
					case SimpleTypes::Vml::cssmsoposverrelText:
					{
						bTextRelativeY = true;
						odf_context()->drawing_context()->set_vertical_rel(6);
					}break;
					case SimpleTypes::Vml::cssmsoposverrelLine:
					{
						bTextRelativeY = true;
						odf_context()->drawing_context()->set_vertical_rel(2);
					}break;
				}
			}break;
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
				bPosition = true;
			}break;
			case SimpleTypes::Vml::cssptRotation:
			{
				//if (group)
				//	odf_context()->drawing_context()->set_group_rotate(180 + vml_style->m_arrProperties[i]->get_Value().oValue.dValue);
				//else
					odf_context()->drawing_context()->set_rotate(360 - vml_style->m_arrProperties[i]->get_Value().oValue.dValue);
			}break;
			case SimpleTypes::Vml::cssptFlip:
			{
				switch (vml_style->m_arrProperties[i]->get_Value().eFlip)
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
				switch (vml_style->m_arrProperties[i]->get_Value().eLayoutFlow)
				{
				case SimpleTypes::Vml::csslayoutflowHorizontal: break;
				case SimpleTypes::Vml::csslayoutflowVertical:odf_context()->drawing_context()->set_textarea_writing_mode(3); break;
				case SimpleTypes::Vml::csslayoutflowVerticalIdeographic:odf_context()->drawing_context()->set_textarea_writing_mode(0); break;
				case SimpleTypes::Vml::csslayoutflowHorizontalIdeographic:break;
				}
			}break;
			case SimpleTypes::Vml::cssptVTextAnchor:
				switch (vml_style->m_arrProperties[i]->get_Value().eVTextAnchor)
				{
				case SimpleTypes::Vml::cssvtextanchorTop:	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorMiddle:	odf_context()->drawing_context()->set_textarea_vertical_align(1); break;
				case SimpleTypes::Vml::cssvtextanchorBottom:	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopCenter:	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorMiddleCenter:	odf_context()->drawing_context()->set_textarea_vertical_align(1); break;
				case SimpleTypes::Vml::cssvtextanchorBottomCenter:	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopBaseline:	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorBottomBaseline:	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				case SimpleTypes::Vml::cssvtextanchorTopCenterBaseline:	odf_context()->drawing_context()->set_textarea_vertical_align(4); break;
				case SimpleTypes::Vml::cssvtextanchorBottomCenterBaseline:	odf_context()->drawing_context()->set_textarea_vertical_align(0); break;
				}break;
			case SimpleTypes::Vml::cssptMsoFitShapeToText:
				if (vml_style->m_arrProperties[i]->get_Value().bValue == true)
				{
					odf_context()->drawing_context()->set_text_box_min_size(0, 0);
				}break;
			}
		}

		if (x2 && !x && width)	x = *x2 - *width;
		if (y2 && !y && height)	y = *y2 - *height;

		//if (group)
		//{
		//	odf_context()->drawing_context()->set_group_size(width, height, width, height);
		//	odf_context()->drawing_context()->set_group_position(x, y, x, y);
		//}
		//else
		{
			odf_context()->drawing_context()->set_size(width, height);
			odf_context()->drawing_context()->set_position(x, y);

			if (bTextRelativeX && bTextRelativeY)
				odf_context()->drawing_context()->set_anchor(3);
			else if ((anchor_type_x && anchor_type_y) && (*anchor_type_x == *anchor_type_y))
				odf_context()->drawing_context()->set_anchor(*anchor_type_x);
			else if (x && y)
				odf_context()->drawing_context()->set_anchor(2);
			else if (false == bPosition)
				odf_context()->drawing_context()->set_anchor(4);


			//if (x && y && !anchor_set) 
			//	odf_context()->drawing_context()->set_anchor(2);

		}

		//if (width_pt && height_pt)
		//	odf_context()->drawing_context()->set_viewBox(width_pt.get(), height_pt.get());
	}
	void OoxConverter::convert(OOX::Vml::CShape *vml_shape)
	{
		if (vml_shape == NULL) return;

		if (vml_shape->m_sType.IsInit())
		{
			if ((false == vml_shape->m_sType->empty()) && (vml_shape->m_sType->at(0) == (wchar_t)('#')))
			{
				vml_shape->m_sType = vml_shape->m_sType->substr(1);
			}
			std::map<std::wstring, OOX::Vml::CShapeType*>::iterator pFind = m_mapVmlShapeTypes.find(*vml_shape->m_sType);

			if (pFind != m_mapVmlShapeTypes.end())
			{
				vml_shape->mergeFrom(pFind->second);
			}
		}

		odf_context()->drawing_context()->start_drawing();

		odf_context()->drawing_context()->set_overlap(vml_shape->m_oAllowOverlap.get_value_or(true));

		if (vml_shape->m_oSpt.IsInit())
		{
			SimpleTypes::Vml::SptType sptType = static_cast<SimpleTypes::Vml::SptType>(vml_shape->m_oSpt->GetValue());
			odf_context()->drawing_context()->start_shape(OOX::VmlShapeType2PrstShape(sptType));
		}
		else if ((vml_shape->m_oConnectorType.IsInit()) && (vml_shape->m_oConnectorType->GetValue() != SimpleTypes::connectortypeNone))
		{
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeStraightConnector1);
			odf_context()->drawing_context()->set_line_width(1.);
		}
		else if (vml_shape->m_oPath.IsInit())
		{
			odf_context()->drawing_context()->start_shape(1000);
			odf_context()->drawing_context()->set_line_width(1.);

			std::wstring vml_path = vml_shape->m_oPath->GetValue();
			std::wstring odf_path;

			bool isDigitLast = true;

			for (size_t i = 0; i < vml_path.size(); ++i)
			{
				if (vml_path[i] == L',')
				{
					odf_path += L" ";
					isDigitLast = true;
				}
				else 
				{
					if (vml_path[i] >= L'0' && vml_path[i] <= L'9')
					{
						if (!isDigitLast) odf_path += L" ";
						isDigitLast = true;
					}
					else
					{
						if (isDigitLast) odf_path += L" ";
						isDigitLast = false;
					}

					odf_path += vml_path[i];
					std::transform(odf_path.begin(), odf_path.end(), odf_path.begin(), toupper);
				}
			}

			odf_context()->drawing_context()->set_path(odf_path);

			if (vml_shape->m_oCoordSize.IsInit())
			{
				odf_context()->drawing_context()->set_viewBox(vml_shape->m_oCoordSize->GetX(), vml_shape->m_oCoordSize->GetY());
			}
			else
			{
				odf_context()->drawing_context()->set_viewBox(21600, 212600);
			}
		}
		else if (vml_shape->m_bImage)
		{
			odf_context()->drawing_context()->start_shape(3000);
		}
		else
		{
			odf_context()->drawing_context()->set_name(L"Rect");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);
		}

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_shape));

		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::CImage *vml_image)
	{
		if (vml_image == NULL) return;

		odf_context()->drawing_context()->start_drawing();
		odf_context()->drawing_context()->start_shape(3000);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_image));

		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::CImageData *vml_image_data)
	{
		if (vml_image_data == NULL) return;

		std::wstring pathImage;
		double Width = 0, Height = 0;

		std::wstring sID;
		if (vml_image_data->m_rId.IsInit())	sID = vml_image_data->m_rId->GetValue();
		else if (vml_image_data->m_oRelId.IsInit())	sID = vml_image_data->m_oRelId->GetValue();
		else if (vml_image_data->m_rPict.IsInit())	sID = vml_image_data->m_rPict->GetValue();

		if (!sID.empty())
		{
			pathImage = find_link_by_id(sID, 1);
		}

		//что именно нужно заливка объекта или картинка - разрулится внутри drawing_context
		if (pathImage.empty())return;

		_graphics_utils_::GetResolution(pathImage.c_str(), Width, Height);

		odf_context()->drawing_context()->start_area_properties();
		odf_context()->drawing_context()->start_bitmap_style();

		odf_context()->drawing_context()->set_bitmap_link(pathImage);
		odf_context()->drawing_context()->set_image_style_repeat(1);//stretch

		double gain = vml_image_data->m_oGain.get_value_or(0);
		if (gain > 1)
			odf_context()->drawing_context()->set_opacity(gain / 1000.);

		if (vml_image_data->m_oCropLeft.IsInit() || vml_image_data->m_oCropRight.IsInit()
			|| vml_image_data->m_oCropTop.IsInit() || vml_image_data->m_oCropBottom.IsInit())
		{
			double l_pt = 0, t_pt = 0, r_pt = 0, b_pt = 0;

			if (vml_image_data->m_oCropLeft.IsInit())	l_pt = vml_image_data->m_oCropLeft->GetValue()	* Width;
			if (vml_image_data->m_oCropTop.IsInit())	t_pt = vml_image_data->m_oCropTop->GetValue()	* Height;
			if (vml_image_data->m_oCropRight.IsInit())	r_pt = vml_image_data->m_oCropRight->GetValue() * Width;
			if (vml_image_data->m_oCropBottom.IsInit()) b_pt = vml_image_data->m_oCropBottom->GetValue()* Height;

			odf_context()->drawing_context()->set_image_client_rect_pt(l_pt, t_pt, r_pt, b_pt);
		}

		odf_context()->drawing_context()->end_bitmap_style();
		odf_context()->drawing_context()->end_area_properties();
	}

	void OoxConverter::convert(OOX::Vml::CArc *vml_arc)
	{
		if (vml_arc == NULL) return;

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_arc));
	}
	void OoxConverter::convert(OOX::Vml::CBackground *vml_background)
	{
		if (vml_background == NULL) return;

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_background));
	}

	void OoxConverter::convert(OOX::Vml::CFill	*vml_fill)
	{
		if (vml_fill == NULL) return;

		odf_context()->drawing_context()->start_area_properties();

		std::wstring sImagePath;

		_CP_OPT(unsigned int) nRgbColor1, nRgbColor2;
		_CP_OPT(std::wstring) sRgbColor1, sRgbColor2;

		if (vml_fill->m_oColor.IsInit())
		{
			nRgbColor1 = ((unsigned int)(((BYTE)(vml_fill->m_oColor->Get_B())
				| ((unsigned int)((BYTE)(vml_fill->m_oColor->Get_G())) << 8))
				| (((unsigned int)(BYTE)(vml_fill->m_oColor->Get_R())) << 16)));

			sRgbColor1 = XmlUtils::ToString(*nRgbColor1, L"%06X");
		}

		if (vml_fill->m_oColor2.IsInit())
		{
			nRgbColor2 = ((unsigned int)(((BYTE)(vml_fill->m_oColor2->Get_B())
				| ((unsigned int)((BYTE)(vml_fill->m_oColor2->Get_G())) << 8))
				| (((unsigned int)(BYTE)(vml_fill->m_oColor2->Get_R())) << 16)));
			sRgbColor2 = XmlUtils::ToString(*nRgbColor2, L"%06X");
		}

		std::wstring sID;

		if (vml_fill->m_rId.IsInit())			sID = vml_fill->m_rId->GetValue();
		else if (vml_fill->m_oRelId.IsInit())	sID = vml_fill->m_oRelId->GetValue();
		else if (vml_fill->m_sId.IsInit())		sID = *vml_fill->m_sId;

		if (!sID.empty())
		{
			//bitmap fill
			odf_context()->drawing_context()->start_bitmap_style();
			{
				double Width = 0, Height = 0;

				sImagePath = find_link_by_id(sID, 1);

				if (!sImagePath.empty())
				{
					odf_context()->drawing_context()->set_bitmap_link(sImagePath);
					_graphics_utils_::GetResolution(sImagePath.c_str(), Width, Height);
				}
				odf_context()->drawing_context()->set_image_style_repeat(1);
			}
			odf_context()->drawing_context()->end_bitmap_style();
		}

		switch (vml_fill->m_oType.GetValue())
		{
		case SimpleTypes::filltypeGradient:
		case SimpleTypes::filltypeGradientCenter:
		case SimpleTypes::filltypeGradientRadial:
		case SimpleTypes::filltypeGradientUnscaled:
		{
			odf_context()->drawing_context()->start_gradient_style();
			if (SimpleTypes::filltypeGradient == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf_types::gradient_style::linear);
			if (SimpleTypes::filltypeGradientRadial == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf_types::gradient_style::radial);
			if (SimpleTypes::filltypeGradientCenter == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf_types::gradient_style::axial);
			if (SimpleTypes::filltypeGradientUnscaled == vml_fill->m_oType.GetValue()) odf_context()->drawing_context()->set_gradient_type(odf_types::gradient_style::square);

			_CP_OPT(double) no_set;
			if (sRgbColor1)
				odf_context()->drawing_context()->set_gradient_start(*sRgbColor1, no_set);
			if (sRgbColor2)
				odf_context()->drawing_context()->set_gradient_end(*sRgbColor2, no_set);
			else
				odf_context()->drawing_context()->set_gradient_end(L"#ffffff", no_set);

			if (vml_fill->m_oAngle.IsInit())
			{
				odf_context()->drawing_context()->set_gradient_angle(vml_fill->m_oAngle->GetValue() + 90);
			}
			if (vml_fill->m_oFocusPosition.IsInit())
			{
				odf_context()->drawing_context()->set_gradient_center(vml_fill->m_oFocusPosition->GetX(), vml_fill->m_oFocusPosition->GetY());
			}

			odf_context()->drawing_context()->end_gradient_style();
		}break;
		case SimpleTypes::filltypePattern:
		{
			if (!sImagePath.empty())
			{
				odf_context()->drawing_context()->set_image_style_repeat(2);

				if (!nRgbColor1)
					nRgbColor1 = odf_context()->drawing_context()->get_fill_color();

				CBgraFrame bgraFrame;

				bgraFrame.ReColorPatternImage(sImagePath, nRgbColor1.get_value_or(0xffffff), nRgbColor2.get_value_or(0x000000));
			}
			else
			{
				odf_context()->drawing_context()->start_hatch_style();
				if (sRgbColor2)
					odf_context()->drawing_context()->set_hatch_line_color(*sRgbColor2);
				if (sRgbColor1)
					odf_context()->drawing_context()->set_hatch_area_color(*sRgbColor1);
				else
					odf_context()->drawing_context()->set_hatch_area_color(L"#ffffff");

				odf_context()->drawing_context()->end_hatch_style();
			}
		}break;
		case SimpleTypes::filltypeSolid:
		default:
			if (sImagePath.empty() && sRgbColor1)
				odf_context()->drawing_context()->set_solid_fill(*sRgbColor1);
			break;
		}
		//--------------------------------------------------------------------------------------------------------------------
		if (vml_fill->m_oOpacity.IsInit() && vml_fill->m_oOpacity2.IsInit())
		{
			odf_context()->drawing_context()->start_opacity_style();
			odf_context()->drawing_context()->set_opacity_start(vml_fill->m_oOpacity->GetValue() * 100);
			odf_context()->drawing_context()->set_opacity_end(vml_fill->m_oOpacity2->GetValue() * 100);
			//.....
			odf_context()->drawing_context()->end_opacity_style();
		}
		else if (vml_fill->m_oOpacity.IsInit())
		{
			odf_context()->drawing_context()->set_opacity(vml_fill->m_oOpacity->GetValue() * 100);
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

		odf_context()->drawing_context()->start_drawing();

		odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_line));

		_CP_OPT(double) x = vml_line->m_oFrom.GetX();
		_CP_OPT(double) y = vml_line->m_oFrom.GetY();

		_CP_OPT(double) x1 = vml_line->m_oTo.GetX();
		_CP_OPT(double) y1 = vml_line->m_oTo.GetY();

		odf_context()->drawing_context()->set_position_line(x, y, x1, y1);

		odf_context()->drawing_context()->set_anchor(2);

		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::COval *vml_oval)
	{
		if (vml_oval == NULL) return;

		odf_context()->drawing_context()->start_drawing();

		odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeEllipse);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_oval));
		odf_context()->drawing_context()->end_shape();

		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::CPath	*vml_path)
	{
		if (vml_path == NULL) return;

		//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oArrowOk;
		//nullable<std::wstring>                                         m_oConnectAngles;
		//nullable<std::wstring>                                         m_oConnectLocs;
		//SimpleTypes::CConnectType<SimpleTypes::connecttypeNone>   m_oConnectType;
		//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oExtrusionOk;
		//SimpleTypes::CTrueFalse<SimpleTypes::booleanTrue>         m_oFillOk;
		//SimpleTypes::CTrueFalse<SimpleTypes::booleanFalse>        m_oGradientShapeOk;
		//nullable<std::wstring>                                         m_oId;
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

		odf_context()->drawing_context()->start_drawing();
		odf_context()->drawing_context()->start_shape(1001);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_polyline));

		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::CRect	*vml_rect)
	{
		if (vml_rect == NULL) return;

		odf_context()->drawing_context()->start_drawing();
		odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_rect));

		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}

	void OoxConverter::convert(OOX::Vml::CRoundRect	*vml_roundrect)
	{
		if (vml_roundrect == NULL) return;

		odf_context()->drawing_context()->start_drawing();
		odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_roundrect));

		double arc_size = vml_roundrect->m_oArcSize.GetValue();

		if (arc_size > 0)
		{
			_CP_OPT(double) width, height;

			odf_context()->drawing_context()->get_size(width, height);

			odf_types::length corner = odf_types::length(width.get_value_or(200) * arc_size, odf_types::length::pt);
			odf_context()->drawing_context()->set_corner_radius(corner);
		}
		odf_context()->drawing_context()->end_shape();
		odf_context()->drawing_context()->end_drawing();
	}

	void OoxConverter::convert(OOX::Vml::CCurve	*vml_curve)
	{
		if (vml_curve == NULL) return;

		odf_context()->drawing_context()->start_drawing();

		odf_context()->drawing_context()->start_shape(1000);
		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_curve));
		odf_context()->drawing_context()->end_shape();

		odf_context()->drawing_context()->end_drawing();
	}
	void OoxConverter::convert(OOX::Vml::CShadow *vml_shadow)
	{
		if (vml_shadow == NULL) return;
		if (vml_shadow->m_oOn.GetValue() == false) return;

		std::wstring hexColor = vml_shadow->m_oColor.ToString();
		_CP_OPT(double) opacity;

		if (vml_shadow->m_oOpacity.IsInit()) opacity = 100 - 100 * vml_shadow->m_oOpacity->GetValue();

		double offset_x = vml_shadow->m_oOffset.IsXinPoints() ? vml_shadow->m_oOffset.GetX() : -1;
		double offset_y = vml_shadow->m_oOffset.IsYinPoints() ? vml_shadow->m_oOffset.GetY() : -1;

		odf_context()->drawing_context()->set_shadow(1, hexColor, opacity, offset_x, offset_y);

	}
	void OoxConverter::convert(OOX::Vml::CStroke *vml_stroke)
	{
		if (vml_stroke == NULL) return;
		if (vml_stroke->m_oOn.IsInit() && vml_stroke->m_oOn->GetBool() == false) return;
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
			std::wstring hexColor = vml_stroke->m_oColor->ToString();
			if (hexColor.empty())	hexColor = L"000000";
			odf_context()->drawing_context()->set_solid_fill(hexColor);
		}

		if (vml_stroke->m_oOpacity.IsInit())
		{
			odf_context()->drawing_context()->set_opacity(100 - *vml_stroke->m_oOpacity * 100);
		}
		odf_context()->drawing_context()->set_line_width(vml_stroke->m_oWeight.get_value_or(0));

		if (vml_stroke->m_oStartArrow.IsInit() && vml_stroke->m_oStartArrowLength.IsInit() && vml_stroke->m_oStartArrowWidth.IsInit())
		{
			switch (vml_stroke->m_oStartArrow->GetValue())
			{
			case SimpleTypes::strokearrowtypeBlock:
				odf_context()->drawing_context()->set_line_head(4, vml_stroke->m_oStartArrowLength->GetValue(), vml_stroke->m_oStartArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeClassic:
				odf_context()->drawing_context()->set_line_head(1, vml_stroke->m_oStartArrowLength->GetValue(), vml_stroke->m_oStartArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeDiamond:
				odf_context()->drawing_context()->set_line_head(2, vml_stroke->m_oStartArrowLength->GetValue(), vml_stroke->m_oStartArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeOpen:
				odf_context()->drawing_context()->set_line_head(5, vml_stroke->m_oStartArrowLength->GetValue(), vml_stroke->m_oStartArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeOval:
				odf_context()->drawing_context()->set_line_head(3, vml_stroke->m_oStartArrowLength->GetValue(), vml_stroke->m_oStartArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeNone:
			default:
				break;
			}
		}
		if (vml_stroke->m_oEndArrow.IsInit() && vml_stroke->m_oEndArrowLength.IsInit() && vml_stroke->m_oEndArrowWidth.IsInit())
		{
			switch (vml_stroke->m_oEndArrow->GetValue())
			{
			case SimpleTypes::strokearrowtypeBlock:
				odf_context()->drawing_context()->set_line_tail(4, vml_stroke->m_oEndArrowLength->GetValue(), vml_stroke->m_oEndArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeClassic:
				odf_context()->drawing_context()->set_line_tail(1, vml_stroke->m_oEndArrowLength->GetValue(), vml_stroke->m_oEndArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeDiamond:
				odf_context()->drawing_context()->set_line_tail(2, vml_stroke->m_oEndArrowLength->GetValue(), vml_stroke->m_oEndArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeOpen:
				odf_context()->drawing_context()->set_line_tail(5, vml_stroke->m_oEndArrowLength->GetValue(), vml_stroke->m_oEndArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeOval:
				odf_context()->drawing_context()->set_line_tail(3, vml_stroke->m_oEndArrowLength->GetValue(), vml_stroke->m_oEndArrowWidth->GetValue()); break;
			case SimpleTypes::strokearrowtypeNone:
			default:
				break;
			}
		}
		odf_context()->drawing_context()->end_line_properties();
	}
	void OoxConverter::convert(OOX::Vml::CTextbox *vml_textbox)
	{
		if (vml_textbox == NULL) return;

		if (vml_textbox->m_oTxtbxContent.IsInit() == false) return;
		if (vml_textbox->m_oTxtbxContent->m_arrItems.empty()) return;

		convert(vml_textbox->m_oStyle.GetPointer());

		if (vml_textbox->m_oInsetMode.GetValue() == SimpleTypes::insetmodeCustom)
		{
			_CP_OPT(double) lIns, tIns, rIns, bIns;

			if (vml_textbox->m_oInset.IsInit())
			{
				lIns = odf_types::length(vml_textbox->m_oInset->GetLeft(0), odf_types::length::pt).get_value_unit(odf_types::length::cm);
				tIns = odf_types::length(vml_textbox->m_oInset->GetTop(0), odf_types::length::pt).get_value_unit(odf_types::length::cm);
				rIns = odf_types::length(vml_textbox->m_oInset->GetRight(0), odf_types::length::pt).get_value_unit(odf_types::length::cm);
				bIns = odf_types::length(vml_textbox->m_oInset->GetBottom(0), odf_types::length::pt).get_value_unit(odf_types::length::cm);
			}
			odf_context()->drawing_context()->set_textarea_padding(lIns, tIns, rIns, bIns);
		}

		odf_context()->drawing_context()->set_textarea_wrap(true);

		DocxConverter *docx_converter = dynamic_cast<DocxConverter*>(this);
		if (docx_converter)
		{
			odf_context()->start_text_context();
			{
				for (std::vector<OOX::WritingElement*>::iterator it = vml_textbox->m_oTxtbxContent->m_arrItems.begin(); it != vml_textbox->m_oTxtbxContent->m_arrItems.end(); ++it)
				{
					docx_converter->convert(*it);
				}
				odf_context()->drawing_context()->set_text(odf_context()->text_context());
			}
			odf_context()->end_text_context();
		}

	}
	void OoxConverter::convert(OOX::Vml::CTextPath *vml_textpath)
	{
		if (vml_textpath == NULL) return; //это типо фигурный текст

		if (vml_textpath->m_sString.IsInit() == false) return;

		odf_context()->drawing_context()->change_text_box_2_wordart();

		if (vml_textpath->m_oStyle.IsInit())
		{
			odf_context()->styles_context()->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);

			odf_writer::style_paragraph_properties	*paragraph_properties = odf_context()->styles_context()->last_state()->get_paragraph_properties();
			odf_writer::style_text_properties		*text_properties = odf_context()->styles_context()->last_state()->get_text_properties();

			for (size_t i = 0; i < vml_textpath->m_oStyle->m_arrProperties.size(); i++)
			{
				if (vml_textpath->m_oStyle->m_arrProperties[i] == NULL) continue;

				switch (vml_textpath->m_oStyle->m_arrProperties[i]->get_Type())
				{
				case SimpleTypes::Vml::cssptFont:
					//height = vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().oValue.dValue;
					break;
				case SimpleTypes::Vml::cssptFontSize:
					//todooo проверять на размерность
					text_properties->content_.fo_font_size_ = odf_types::length(vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().oValue.dValue, odf_types::length::pt);
					break;
				case SimpleTypes::Vml::cssptFontStyle:
					//width = vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().oValue.dValue;
					break;
				case SimpleTypes::Vml::cssptFontWeight:
					//width = vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().oValue.dValue;
					break;
				case SimpleTypes::Vml::cssptFontFamily:
				{
					std::wstring font_family = vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().wsValue;
					XmlUtils::replace_all(font_family, L"\"", L"");
					text_properties->content_.fo_font_family_ = font_family;
				}break;
				case SimpleTypes::Vml::cssptHTextAlign:
					switch (vml_textpath->m_oStyle->m_arrProperties[i]->get_Value().eVTextAlign)
					{
					case SimpleTypes::Vml::cssvtextalignLeft:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Left); break;
					case SimpleTypes::Vml::cssvtextalignRight:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Right); break;
					case SimpleTypes::Vml::cssvtextalignCenter:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Center); break;
					case SimpleTypes::Vml::cssvtextalignJustify:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Left); break;
					case SimpleTypes::Vml::cssvtextalignLetterJustify:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify); break;
					case SimpleTypes::Vml::cssvtextalignStretchJustify:
						paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify); break;
					}break;
				}
			}
		}
		odf_context()->start_text_context();

		odf_context()->text_context()->start_paragraph(vml_textpath->m_oStyle.IsInit());
		odf_context()->text_context()->start_span(/*vml_textpath->m_oStyle.IsInit()*/);
		odf_context()->text_context()->add_text_content(vml_textpath->m_sString.get());
		odf_context()->text_context()->end_span();
		odf_context()->text_context()->end_paragraph();

		odf_context()->drawing_context()->set_text(odf_context()->text_context());
		odf_context()->end_text_context();


		odf_context()->drawing_context()->set_textarea_wrap(true);
	}

	void OoxConverter::convert(OOX::VmlWord::CWrap	*vml_wrap)
	{
		if (vml_wrap == NULL) return;

		odf_context()->drawing_context()->set_default_wrap_style();

		if (vml_wrap->m_oType.IsInit())
		{
			switch (vml_wrap->m_oType->GetValue())
			{
			case SimpleTypes::wraptypeNone:
			{
				odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::None);
			}break;
			case SimpleTypes::wraptypeSquare:
			{
				odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);
				if (vml_wrap->m_oSide.IsInit())
				{
					/*					if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideLargest)
											odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);
										else */
					if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideLeft)
						odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Left);
					else if (vml_wrap->m_oSide->GetValue() == SimpleTypes::wrapsideRight)
						odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Right);
					else
						odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
				}
			}break;
			case SimpleTypes::wraptypeThrough:
			{
				odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::RunThrough);
			}break;
			case SimpleTypes::wraptypeTight:
			{
				odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
			}break;
			case SimpleTypes::wraptypeTopAndBottom:
			{
				odf_context()->drawing_context()->set_wrap_style(odf_types::style_wrap::None);
			}break;
			}
		}

		bool anchor_page_x = false;
		if (vml_wrap->m_oAnchorX.IsInit())
		{
			switch (vml_wrap->m_oAnchorX->GetValue())
			{
			case SimpleTypes::horizontalanchorMargin:
				odf_context()->drawing_context()->set_horizontal_rel(4); break;
			case SimpleTypes::horizontalanchorPage:
			{
				anchor_page_x = true;
				odf_context()->drawing_context()->set_horizontal_rel(6);
			}break;
			case SimpleTypes::horizontalanchorText:
				odf_context()->drawing_context()->set_horizontal_rel(1); break;//???? paragraph ????			
			case SimpleTypes::horizontalanchorChar:
			default:
				odf_context()->drawing_context()->set_horizontal_rel(0);
			}
		}
		if (vml_wrap->m_oAnchorY.IsInit())
		{
			switch (vml_wrap->m_oAnchorY->GetValue())
			{
			case SimpleTypes::verticalanchorMargin:
				odf_context()->drawing_context()->set_vertical_rel(3); break;
			case SimpleTypes::verticalanchorPage:
			{
				if (anchor_page_x)
					odf_context()->drawing_context()->set_anchor(0); //page
				odf_context()->drawing_context()->set_vertical_rel(5);
			}break;
			case SimpleTypes::verticalanchorText:
				odf_context()->drawing_context()->set_vertical_rel(0); break;
			case SimpleTypes::verticalanchorLine:
			default:
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
					unsigned char ucA = 0, ucR = 0, ucG = 0, ucB = 0;
					ucR = vml_common->m_oStrokeColor->Get_R();
					ucB = vml_common->m_oStrokeColor->Get_B();
					ucG = vml_common->m_oStrokeColor->Get_G();

					SimpleTypes::CHexColor *oRgbColor = new SimpleTypes::CHexColor(ucR, ucG, ucB);
					if (oRgbColor)
					{
						odf_context()->drawing_context()->set_solid_fill(oRgbColor->ToString().substr(2));//.Right(6));
						delete oRgbColor;
					}
				}
			}
		}
		odf_context()->drawing_context()->end_line_properties();
		if (vml_common->m_oFillColor.IsInit())
		{
			unsigned char ucA = 0, ucR = 0, ucG = 0, ucB = 0;
			ucR = vml_common->m_oFillColor->Get_R();
			ucB = vml_common->m_oFillColor->Get_B();
			ucG = vml_common->m_oFillColor->Get_G();

			SimpleTypes::CHexColor *oRgbColor = new SimpleTypes::CHexColor(ucR, ucG, ucB);

			if (oRgbColor)
			{
				unsigned int nRgbColor = ((unsigned int)(((BYTE)(oRgbColor->Get_B())
					| ((unsigned int)((BYTE)(oRgbColor->Get_G())) << 8))
					| (((unsigned int)(BYTE)(oRgbColor->Get_R())) << 16)));

				odf_context()->drawing_context()->set_fill_color(nRgbColor);

				odf_context()->drawing_context()->start_area_properties();
				odf_context()->drawing_context()->set_solid_fill(oRgbColor->ToString().substr(2));//.Right(6));
				odf_context()->drawing_context()->end_area_properties();

				delete oRgbColor;
			}
		}
		for (size_t i = 0; i < vml_common->m_arrItems.size(); ++i)
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

		if (vml_group->m_oCoordSize.IsInit())
		{
			odf_context()->drawing_context()->set_group_scale(vml_group->m_oCoordSize->GetX(), vml_group->m_oCoordSize->GetY());
		}

		if (vml_group->m_oCoordOrigin.IsInit())
		{
			odf_context()->drawing_context()->set_group_shift(vml_group->m_oCoordOrigin->GetX(), vml_group->m_oCoordOrigin->GetY());
		}

		convert(dynamic_cast<OOX::Vml::CVmlCommonElements *>(vml_group));

		for (size_t i = 0; i < vml_group->m_arrShapeTypes.size(); ++i)
		{
			convert(vml_group->m_arrShapeTypes[i]);
		}
		for (size_t i = 0; i < vml_group->m_arrElements.size(); ++i)
		{
			convert(vml_group->m_arrElements[i]);
		}
		odf_context()->drawing_context()->end_group();
	}
}
