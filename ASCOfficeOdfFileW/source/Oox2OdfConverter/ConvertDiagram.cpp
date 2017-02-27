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
#include "Converter.h"
#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SrgbClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/PrstClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Word.h"

#include "../OdfFormat/odf_conversion_context.h"
#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"
#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"

using namespace cpdoccore;

namespace Oox2Odf
{
	double Emu2Pt(int emu)
	{
		return (1.0 * emu / (635 * 20.0));
	}

void OoxConverter::convert(PPTX::Logic::GraphicFrame *oox_graphic_frame)
{
	if (!oox_graphic_frame)return;

////////////////////////////////////////////////////////////////////////////////
	odf_context()->drawing_context()->start_drawing();
	
	convert(&oox_graphic_frame->nvGraphicFramePr);		
	
	if ( oox_graphic_frame->chartRec.IsInit())
	{
		convert(oox_graphic_frame->chartRec.GetPointer());
	}
	else if ( oox_graphic_frame->smartArt.IsInit())
	{
		OoxConverter::convert(oox_graphic_frame->smartArt.GetPointer());
	}
	else if ( oox_graphic_frame->pic.IsInit())
	{
		OoxConverter::convert(oox_graphic_frame->pic.GetPointer());
	}
	else if ( oox_graphic_frame->table.IsInit())
	{
		OoxConverter::convert(oox_graphic_frame->table.GetPointer());
	}
	else if ( oox_graphic_frame->element.IsInit())
	{
		OoxConverter::convert(oox_graphic_frame->element->GetElem().operator->());
	}
	odf_context()->drawing_context()->end_drawing();
}
void OoxConverter::convert(PPTX::Logic::NvGraphicFramePr *oox_framePr)
{
	if (oox_framePr == NULL) return;

}
void OoxConverter::convert(PPTX::Logic::Table *oox_table)
{
	if (oox_table == NULL) return;

}
void OoxConverter::convert(PPTX::Logic::SmartArt *oox_smart_art)
{
	if (oox_smart_art == NULL) return;
	if (oox_smart_art->id_data.IsInit() == false) return;

	oox_smart_art->LoadDrawing();

	if (oox_smart_art->m_diag.IsInit())
	{
		_CP_OPT(double) x, y, width, height, cx, cy;

		odf_context()->drawing_context()->get_size (width, height);
		odf_context()->drawing_context()->get_position (x, y);

		oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(oox_smart_art->m_oCommonRels.operator->());

		odf_context()->drawing_context()->start_group();

		odf_context()->drawing_context()->set_group_size (width, height, width, height);
		odf_context()->drawing_context()->set_group_position (x, y, cx, cy);

		convert(oox_smart_art->m_diag.GetPointer());

		odf_context()->drawing_context()->end_group();
		oox_current_child_document = NULL;
	}
}
void OoxConverter::convert(PPTX::Logic::ChartRec *oox_chart)
{
	if (!oox_chart) return;
	if( !oox_chart->id_data.IsInit()) return;

	_CP_OPT(double) width, height;
	odf_context()->drawing_context()->get_size (width, height);
				
	smart_ptr<OOX::File> oFile = find_file_by_id (oox_chart->id_data->get());
	if (oFile.IsInit())
	{
		OOX::Spreadsheet::CChartSpace* pChart = (OOX::Spreadsheet::CChartSpace*)oFile.operator->();
		
		if (pChart)
		{
			oox_current_child_document_spreadsheet = dynamic_cast<OOX::Spreadsheet::IFileContainer*>(pChart);	
			odf_context()->drawing_context()->start_object(odf_context()->get_next_name_object());
			{
				odf_context()->start_chart();
					odf_context()->chart_context()->set_chart_size(width, height);		
		
					OoxConverter::convert(pChart->m_oChartSpace.m_oSpPr.GetPointer());			
			
					OoxConverter::convert(&pChart->m_oChartSpace);
				odf_context()->end_chart();
			}
			odf_context()->drawing_context()->end_object();	
			oox_current_child_document_spreadsheet = NULL;
		}
	}
}
void OoxConverter::convert(PPTX::Logic::SpTree *oox_shape_tree)
{
	if (oox_shape_tree == NULL) return;
	
	for (size_t i = 0; i < oox_shape_tree->SpTreeElems.size(); i++)
	{
		convert(oox_shape_tree->SpTreeElems[i].GetElem().operator->());
	}
}

void OoxConverter::convert(PPTX::Logic::Shape *oox_shape)
{
	if (oox_shape == NULL) return;

	odf_context()->drawing_context()->start_drawing();
	
		int type = 1000;

		PPTX::Logic::PrstGeom* prstGeom = &oox_shape->spPr.Geometry.as<PPTX::Logic::PrstGeom>();

		if ( prstGeom )
		{
			SimpleTypes::CShapeType<> preset;
			preset.FromString(prstGeom->prst.get());
			type = preset.GetValue();
		}

		if (type == SimpleTypes::shapetypeRect && oox_shape->txBody.IsInit()) type = 2000;

		if (type == 2000 && oox_shape->txBody->bodyPr.IsInit() 
			&& oox_shape->txBody->bodyPr->fromWordArt.get_value_or(false))
		{
			int wordart_type = convert(oox_shape->txBody->bodyPr->prstTxWarp.GetPointer());

			if (wordart_type > 0) type = wordart_type;
		}

		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		odf_context()->drawing_context()->start_shape(type);
		
		convert(&oox_shape->spPr, oox_shape->style.GetPointer());
	//имя, описалово, номер ...	
		convert(&oox_shape->nvSpPr.cNvSpPr);	

		if ( oox_shape->txBody.IsInit() && oox_shape->txBody->Paragrs.size() >0)
		{
			odf_context()->start_text_context();	
				
			for (size_t i = 0; i < oox_shape->txBody->Paragrs.size();i++)
			{
				convert(&oox_shape->txBody->Paragrs[i]);
			}
			odf_context()->drawing_context()->set_text( odf_context()->text_context());
			
		//наложим внешние настройки для текста
			convert(oox_shape->txBody->bodyPr.GetPointer());			
			
			if (oox_shape->style.IsInit())
			{
				convert(&oox_shape->style->fontRef);
			}	


			odf_context()->end_text_context();	
		}

	odf_context()->drawing_context()->end_shape();
	
	odf_context()->drawing_context()->end_drawing();
}
void OoxConverter::convert(PPTX::Logic::SpPr *oox_spPr, PPTX::Logic::ShapeStyle* oox_sp_style)
{
	if (oox_spPr == NULL) return;

	bool use_line_from_style = false;

	if (oox_spPr->xfrm.IsInit())	//CTransform2D
	{
		_CP_OPT(double) x, y, width, height;

		if (oox_spPr->xfrm->offX.IsInit())	x = Emu2Pt(*oox_spPr->xfrm->offX);
		if (oox_spPr->xfrm->offY.IsInit())	y = Emu2Pt(*oox_spPr->xfrm->offY);
		
		odf_context()->drawing_context()->set_position( x, y);
		
		if (oox_spPr->xfrm->extX.IsInit())	width	= Emu2Pt(*oox_spPr->xfrm->extX);
		if (oox_spPr->xfrm->extY.IsInit())	height	= Emu2Pt(*oox_spPr->xfrm->extY);
		
		odf_context()->drawing_context()->set_size(	width, height);					
		
		if (oox_spPr->xfrm->flipH.get_value_or(false))
			odf_context()->drawing_context()->set_flip_H(true);
		if (oox_spPr->xfrm->flipV.get_value_or(false))
			odf_context()->drawing_context()->set_flip_V(true);
		
		if (oox_spPr->xfrm->rot.get_value_or(0) > 0)
			odf_context()->drawing_context()->set_rotate(360. - oox_spPr->xfrm->rot.get_value_or(0)/60000.);
	}
	PPTX::Logic::PrstGeom* prstGeom = &oox_spPr->Geometry.as<PPTX::Logic::PrstGeom>();
	PPTX::Logic::CustGeom* custGeom = &oox_spPr->Geometry.as<PPTX::Logic::CustGeom>();

	convert(prstGeom);
	convert(custGeom);

	odf_context()->drawing_context()->start_area_properties();
	{
		convert(&oox_spPr->Fill, oox_sp_style);
	}
	odf_context()->drawing_context()->end_area_properties();

	odf_context()->drawing_context()->start_line_properties();
	{
		if (oox_spPr->ln.IsInit())
		{
			convert(oox_spPr->ln.GetPointer());	//CLineProperties
		}
		else if (oox_sp_style)
		{
			convert(&oox_sp_style->lnRef);
		}
	}
	odf_context()->drawing_context()->end_line_properties();
//-----------------------------------------------------------------------------------------------------------------------------
	PPTX::Logic::EffectLst*	effectLst = &oox_spPr->EffectList.as<PPTX::Logic::EffectLst>();
	
	if		(effectLst)		convert(effectLst);
	else if (oox_sp_style)	convert(&oox_sp_style->effectRef);

	//nullable<OOX::Drawing::CEffectContainer>          EffectDag;

	//nullable<OOX::Drawing::COfficeArtExtensionList>   ExtLst;
	//nullable<OOX::Drawing::CScene3D>                  Scene3D;
	//nullable<OOX::Drawing::CShape3D>                  Sp3D;	
//-----------------------------------------------------------------------------------------------------------------------------
}

void OoxConverter::convert(PPTX::Logic::UniFill *oox_fill, PPTX::Logic::ShapeStyle* oox_sp_style)
{
	if (oox_fill == NULL) return;

	bool use_fill_from_style = false;

	PPTX::Logic::NoFill*	noFill		= &oox_fill->as<PPTX::Logic::NoFill>();
	PPTX::Logic::BlipFill*	blipFill	= &oox_fill->as<PPTX::Logic::BlipFill>();
	PPTX::Logic::GradFill*	gradFill	= &oox_fill->as<PPTX::Logic::GradFill>();
	PPTX::Logic::SolidFill*	solidFill	= &oox_fill->as<PPTX::Logic::SolidFill>();
	PPTX::Logic::PattFill*	pattFill	= &oox_fill->as<PPTX::Logic::PattFill>();
	
	if		(solidFill)	convert(solidFill);
	else if (blipFill)	convert(blipFill);
	else if (gradFill)	convert(gradFill);
	else if (pattFill)	convert(pattFill);			
	else if (noFill)	odf_context()->drawing_context()->set_no_fill();
	else				use_fill_from_style = true;

	if ( use_fill_from_style && oox_sp_style )
	{
		convert(&oox_sp_style->fillRef);
	}
}

int OoxConverter::convert(PPTX::Logic::PrstTxWarp *oox_text_preset)
{
	if (oox_text_preset == NULL) return -1;
	if (oox_text_preset->prst.GetBYTECode() ==  SimpleTypes::textshapetypeTextNoShape) return 2000;

	return 2001 + oox_text_preset->prst.GetBYTECode();
}
void OoxConverter::convert(PPTX::Logic::PrstGeom *oox_geom)
{
	if (!oox_geom) return;

	for (size_t i = 0; i < oox_geom->avLst.size(); i++)
	{
		if (oox_geom->avLst[i].fmla.IsInit())
			odf_context()->drawing_context()->add_modifier(oox_geom->avLst[i].fmla.get());
	}
}
void OoxConverter::convert(PPTX::Logic::CustGeom *oox_cust_geom)
{
	if (!oox_cust_geom) return;

	for (size_t i = 0; i < oox_cust_geom->pathLst.size(); i++)
	{
		convert(&oox_cust_geom->pathLst[i]);
	}
}
void OoxConverter::convert(PPTX::Logic::Path2D *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	odf_context()->drawing_context()->set_viewBox(oox_geom_path->w.get_value_or(0), oox_geom_path->h.get_value_or(0));

	for (size_t i = 0 ; i < oox_geom_path->Paths.size(); i++)
	{
		PPTX::Logic::PathBase* pathBase = &oox_geom_path->Paths[i].Path2D.as<PPTX::Logic::PathBase>();
		convert(pathBase);
	}

	odf_context()->drawing_context()->add_path_element(std::wstring(L"N"), L"");
}

void OoxConverter::convert(PPTX::Logic::PathBase *oox_path)
{
	if (!oox_path) return;

	PPTX::Logic::MoveTo*		moveTo		= static_cast<PPTX::Logic::MoveTo*>		(oox_path);
	PPTX::Logic::LineTo*		lineTo		= static_cast<PPTX::Logic::LineTo*>		(oox_path);
	PPTX::Logic::CubicBezTo*	cubicBezTo	= static_cast<PPTX::Logic::CubicBezTo*>	(oox_path);
	PPTX::Logic::Close*			close		= static_cast<PPTX::Logic::Close*>		(oox_path);
	PPTX::Logic::ArcTo*			arcTo		= static_cast<PPTX::Logic::ArcTo*>		(oox_path);
	PPTX::Logic::QuadBezTo*		quadBezTo	= static_cast<PPTX::Logic::QuadBezTo*>	(oox_path);

	if (moveTo)
	{
		std::wstring path_elm =	moveTo->x + L" " + moveTo->y;
		odf_context()->drawing_context()->add_path_element(std::wstring(L"M"), path_elm);
	}
	if (lineTo)
	{
		std::wstring path_elm =	lineTo->x + L" " + lineTo->y;
		odf_context()->drawing_context()->add_path_element(std::wstring(L"L"), path_elm);
	}
	if (cubicBezTo)
	{
		std::wstring path_elm =	cubicBezTo->x[0] + L" " + cubicBezTo->y[0] + L" " +
								cubicBezTo->x[1] + L" " + cubicBezTo->y[1] + L" " +
								cubicBezTo->x[2] + L" " + cubicBezTo->y[2];	
		odf_context()->drawing_context()->add_path_element(std::wstring(L"C"), path_elm);
	}
	if (quadBezTo)
	{
		std::wstring path_elm =	quadBezTo->x[0] + L" " + quadBezTo->y[0] + L" " +
								quadBezTo->x[1] + L" " + quadBezTo->y[1];	
		odf_context()->drawing_context()->add_path_element(std::wstring(L"S"), path_elm);
	}
	if (arcTo)
	{
	}
	if (close)
	{
		std::wstring path_elm ;	
		odf_context()->drawing_context()->add_path_element(std::wstring(L"Z"), path_elm);
	}

}

void OoxConverter::convert(PPTX::Logic::BlipFill *oox_bitmap_fill)
{
	if (oox_bitmap_fill == NULL)return;

	odf_context()->drawing_context()->start_bitmap_style();
	{
		double Width=0, Height = 0;
		if (oox_bitmap_fill->blip.IsInit())
		{
            std::wstring sID, pathImage;
            if (oox_bitmap_fill->blip->embed.IsInit())
            {
                sID         = oox_bitmap_fill->blip->embed->get();
                pathImage   = find_link_by_id(sID,1);

                if (!pathImage.empty())
                {
                    odf_context()->drawing_context()->set_bitmap_link(pathImage);
                    _graphics_utils_::GetResolution(pathImage.c_str(), Width, Height);
                }
            }
            else if (oox_bitmap_fill->blip->link.IsInit())
			{
                sID  = pathImage = oox_bitmap_fill->blip->link->get();

                odf_context()->drawing_context()->set_bitmap_link(pathImage);
				//...
			}
			//for (size_t i = 0 ; i < oox_bitmap_fill->blip->m_arrEffects.size(); i++)
			//{
			//	convert(oox_bitmap_fill->blip->m_arrEffects[i]);
			//}
		}
		if (oox_bitmap_fill->srcRect.IsInit() && Width >0  && Height >0)//часть изображения
		{
			odf_context()->drawing_context()->set_image_client_rect_inch(
				(oox_bitmap_fill->srcRect->l.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->l.get()) : 0 )  /100. * Width / currentSystemDPI,
                (oox_bitmap_fill->srcRect->t.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->t.get()) : 0 )    /100. * Height/ currentSystemDPI,
                (oox_bitmap_fill->srcRect->r.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->r.get()) : 0 ) /100. * Width / currentSystemDPI,
                (oox_bitmap_fill->srcRect->b.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->b.get()) : 0 ) /100. * Height/ currentSystemDPI);
		}
		if (oox_bitmap_fill->tile.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(2);
			
			if (oox_bitmap_fill->tile->algn.IsInit())
				odf_context()->drawing_context()->set_bitmap_tile_align(oox_bitmap_fill->tile->algn->GetBYTECode());

			if (oox_bitmap_fill->tile->flip.IsInit())	{}

			if (oox_bitmap_fill->tile->sx.IsInit() && Width >0)	
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_x(*oox_bitmap_fill->tile->sx / 100. * Width);
			}
			if (oox_bitmap_fill->tile->sy.IsInit()&& Height >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_y(*oox_bitmap_fill->tile->sy / 100. * Height);
			}		
			if (oox_bitmap_fill->tile->tx.IsInit() && Width >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_x(*oox_bitmap_fill->tile->tx * 100. / Width );
			}
			if (oox_bitmap_fill->tile->ty.IsInit() && Height >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_y(*oox_bitmap_fill->tile->ty * 100. / Height );
			}
		}
		if (oox_bitmap_fill->stretch.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(1);
			if (oox_bitmap_fill->stretch->fillRect.IsInit()){} //заполнение неполного объема
		}
	}
	odf_context()->drawing_context()->end_bitmap_style();
}
void OoxConverter::convert(PPTX::Logic::GradFill *oox_grad_fill)
{
	if (!oox_grad_fill) return;
	
	odf_context()->drawing_context()->start_gradient_style();
	{
		odf_types::gradient_style::type grad_style = odf_types::gradient_style::linear;
	
		if ( (oox_grad_fill->lin.IsInit()) && (oox_grad_fill->lin->ang.IsInit()))
		{
			odf_context()->drawing_context()->set_gradient_angle(oox_grad_fill->lin->ang.get()/60000.);
		}
		if ( (oox_grad_fill->path.IsInit()) && (oox_grad_fill->path->path.IsInit()))
		{
			grad_style = odf_types::gradient_style::linear;
			switch(oox_grad_fill->path->path->GetBYTECode())
			{
				case SimpleTypes::pathshadetypeCircle:	grad_style = odf_types::gradient_style::radial;			break;
				case SimpleTypes::pathshadetypeRect:	grad_style = odf_types::gradient_style::rectangular;	break;
				case SimpleTypes::pathshadetypeShape:	grad_style = odf_types::gradient_style::square;			break;
			}	
			if (oox_grad_fill->path->rect.IsInit())
			{
				odf_context()->drawing_context()->set_gradient_rect( XmlUtils::GetInteger(oox_grad_fill->path->rect->l.get_value_or(L"")),
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->t.get_value_or(L"")),
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->r.get_value_or(L"")),
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->b.get_value_or(L"")));			
			}
		}	
		odf_context()->drawing_context()->set_gradient_type(grad_style);

		if (oox_grad_fill->GsLst.size() > 1)
		{
			std::wstring hexColorStart, hexColorEnd;
			_CP_OPT(double) opacityStart, opacityEnd;
			
			convert(&oox_grad_fill->GsLst[0].color,hexColorEnd, opacityEnd);
			convert(&oox_grad_fill->GsLst[oox_grad_fill->GsLst.size()-1].color,hexColorStart, opacityStart);	
			
			odf_context()->drawing_context()->set_gradient_start(hexColorStart, opacityStart);
			odf_context()->drawing_context()->set_gradient_end	(hexColorEnd,	opacityEnd);

			if (opacityStart || opacityEnd)
			{
				if (!opacityStart)	opacityStart = 100;
				if (!opacityEnd)	opacityEnd = 100;
				
				if (*opacityStart == *opacityEnd)
				{
					odf_context()->drawing_context()->set_opacity(*opacityStart);
				}
				else
				{
					odf_context()->drawing_context()->start_opacity_style();
						odf_context()->drawing_context()->set_opacity_type	(grad_style);
						odf_context()->drawing_context()->set_opacity_start	(*opacityStart);
						odf_context()->drawing_context()->set_opacity_end	(*opacityEnd);
						
						if (grad_style == odf_types::gradient_style::linear)
						{
							odf_context()->drawing_context()->set_opacity_angle(oox_grad_fill->lin->ang.get()/60000.);
						}
						else
						{
							odf_context()->drawing_context()->set_opacity_rect ( XmlUtils::GetInteger(oox_grad_fill->path->rect->l.get_value_or(L"")),
																				 XmlUtils::GetInteger(oox_grad_fill->path->rect->t.get_value_or(L"")),
																				 XmlUtils::GetInteger(oox_grad_fill->path->rect->r.get_value_or(L"")),
																				 XmlUtils::GetInteger(oox_grad_fill->path->rect->b.get_value_or(L"")));
						}
					odf_context()->drawing_context()->end_opacity_style();
				}
			}
		}
	}
	odf_context()->drawing_context()->end_gradient_style();

}

void OoxConverter::convert(PPTX::Logic::UniColor * color, std::wstring & hexString, _CP_OPT(double) & opacity)
{
	if (!color) return;

	PPTX::Logic::SrgbClr	*rgb	= &color->as<PPTX::Logic::SrgbClr>();
	PPTX::Logic::SchemeClr	*scheme	= &color->as<PPTX::Logic::SchemeClr>();

	if (scheme)
	{
		int theme_ind = scheme->val.GetBYTECode();
		BYTE ucA = 0, ucG = 0, ucB = 0, ucR = 0;
		
		bool result = convert(theme_ind, ucA, ucG, ucB, ucR);

		if (result == true)
		{
			scheme->SetRGB(ucR, ucG, ucB);
			DWORD dwColor = scheme->ApplyModifiers(PPTX::Logic::ColorBase::ctARGB);
			
			hexString = XmlUtils::IntToString(dwColor & 0x00FFFFFF, L"#%06X");

			if ((dwColor >> 24) != 0xff)
			{
				opacity = ((dwColor >> 24) /255.) * 100.;
			}
		}
	}
	if (rgb)
	{
	}
}

void OoxConverter::convert(PPTX::Logic::SolidFill *oox_fill)
{
	if (!oox_fill) return;
		
	std::wstring	hexString;
	_CP_OPT(double) opacity;

	convert(&oox_fill->Color, hexString, opacity);

	odf_context()->drawing_context()->set_solid_fill(hexString);

	if (opacity)
	{
		odf_context()->drawing_context()->set_opacity(*opacity);
	}
}
void OoxConverter::convert(PPTX::Logic::PattFill *oox_fill)
{
	if (!oox_fill) return;
}
void OoxConverter::convert(PPTX::Logic::EffectLst *oox_effect_lst)
{
	if (!oox_effect_lst) return;
}
void OoxConverter::convert(PPTX::Logic::Ln *oox_line_prop)
{
	if (!oox_line_prop) return;

	convert (&oox_line_prop->Fill);

	if (oox_line_prop->w.IsInit())
	{
		odf_context()->drawing_context()->set_line_width(oox_line_prop->w.get() / 12700.); //pt
	}
	if (oox_line_prop->headEnd.IsInit())
	{
		if (oox_line_prop->headEnd->len.IsInit() || oox_line_prop->headEnd->type.IsInit() || oox_line_prop->headEnd->w.IsInit())
		{
			int type = 0, w=1, len =1;//medium arrow
			if (oox_line_prop->headEnd->len.IsInit())	len		= oox_line_prop->headEnd->len->GetBYTECode();
			if (oox_line_prop->headEnd->type.IsInit())	type	= oox_line_prop->headEnd->type->GetBYTECode();
			if (oox_line_prop->headEnd->w.IsInit())		w		= oox_line_prop->headEnd->w->GetBYTECode();
			
			odf_context()->drawing_context()->set_line_head(type, len, w);
		}
	}
	if (oox_line_prop->tailEnd.IsInit())
	{
		if (oox_line_prop->tailEnd->len.IsInit() || oox_line_prop->tailEnd->type.IsInit() || oox_line_prop->tailEnd->w.IsInit())
		{
			int type =0, w=1, len =1;//medium arrow
			if (oox_line_prop->tailEnd->len.IsInit())	len		= oox_line_prop->tailEnd->len->GetBYTECode();
			if (oox_line_prop->tailEnd->type.IsInit())	type	= oox_line_prop->tailEnd->type->GetBYTECode();
			if (oox_line_prop->tailEnd->w.IsInit())		w		= oox_line_prop->tailEnd->w->GetBYTECode();
			
			odf_context()->drawing_context()->set_line_tail(type, len, w);
		}
	}

	//if (oox_line_prop->custDash.IsInit())
	//{
	//	//через задание стиля и описание геометрии
	//}
	if (oox_line_prop->prstDash.IsInit() && oox_line_prop->prstDash->val.IsInit())
	{
		odf_context()->drawing_context()->set_line_dash_preset(oox_line_prop->prstDash->val->GetBYTECode());
	}
	//nullable<SimpleTypes::CPenAlignment<>>            m_oAlgn;
	//nullable<SimpleTypes::CLineCap<>>                 m_oCap;
	//nullable<SimpleTypes::CCompoundLine<>>            m_oCmpd;


	//ELineJoinType                                     m_eJoinType;   // Тип соединения линий
	//nullable<OOX::Drawing::CLineJoinBevel>            m_oBevel;
	//nullable<OOX::Drawing::CLineJoinMiterProperties>  m_oMiter;
	//nullable<OOX::Drawing::CLineJoinRound>            m_oRound;
}

void OoxConverter::convert(PPTX::Logic::BodyPr *oox_bodyPr)
{
	if (!oox_bodyPr) return;

	if (oox_bodyPr->vert.IsInit())
	{
		odf_context()->drawing_context()->set_textarea_writing_mode (oox_bodyPr->vert->GetBYTECode());
	}
	if (oox_bodyPr->anchor.IsInit())
	{
		odf_context()->drawing_context()->set_textarea_vertical_align (oox_bodyPr->anchor->GetBYTECode());
	}

	_CP_OPT(double) lIns, tIns, rIns, bIns;

	if (oox_bodyPr->lIns.IsInit()) lIns = oox_bodyPr->lIns.get() / 12700. / 28.34467120181406; //cm
	if (oox_bodyPr->tIns.IsInit()) tIns = oox_bodyPr->tIns.get() / 12700. / 28.34467120181406;
	if (oox_bodyPr->rIns.IsInit()) rIns = oox_bodyPr->rIns.get() / 12700. / 28.34467120181406;
	if (oox_bodyPr->bIns.IsInit()) bIns = oox_bodyPr->bIns.get() / 12700. / 28.34467120181406;	
		
	odf_context()->drawing_context()->set_textarea_padding (lIns, tIns, rIns, bIns);

	if (oox_bodyPr->wrap.IsInit())
		odf_context()->drawing_context()->set_textarea_wrap(oox_bodyPr->wrap->GetBYTECode());

	if ((oox_bodyPr->numCol.IsInit()) && (oox_bodyPr->numCol.get() > 1))
	{
		//+ style section
		//+element text:section в котором параграфы
	}
	//if (oox_bodyPr->spAutoFit.IsInit())
	//{
	//	//изменяемы размеры
	//	odf_context()->drawing_context()->set_text_box_min_size(true);//уже выставленые в min

	//}
	if (oox_bodyPr->fromWordArt.IsInit() && oox_bodyPr->prstTxWarp.IsInit())
	{
		for (size_t i = 0; i < oox_bodyPr->prstTxWarp->avLst.size(); i++)
		{
			if (oox_bodyPr->prstTxWarp->avLst[i].fmla.IsInit())
			{
				odf_context()->drawing_context()->add_modifier(oox_bodyPr->prstTxWarp->avLst[i].fmla.get());
			}
		}
	}
}
void OoxConverter::convert(PPTX::Logic::NvSpPr *oox_nvSpPr)
{
	if (!oox_nvSpPr) return;
	
	convert (&oox_nvSpPr->cNvPr);
	convert (&oox_nvSpPr->cNvSpPr);
	convert (&oox_nvSpPr->nvPr);
}
void OoxConverter::convert(PPTX::Logic::CNvPr *oox_cnvPr)
{
	if (!oox_cnvPr) return;

	odf_context()->drawing_context()->set_name(oox_cnvPr->name);

	if (oox_cnvPr->descr.IsInit())
	{
		odf_context()->drawing_context()->set_description(oox_cnvPr->descr.get());
	}
	if (oox_cnvPr->hidden.IsInit())
	{
		odf_context()->drawing_context()->set_hidden(oox_cnvPr->hidden.get());
	}
	if (oox_cnvPr->hlinkClick.IsInit())
	{
	}
	//nullable_string		title;
	//nullable<Hyperlink>	hlinkHover;
}
void OoxConverter::convert(PPTX::Logic::CNvSpPr *oox_cnvSpPr)
{
	if (!oox_cnvSpPr) return;
}
void OoxConverter::convert(PPTX::Logic::NvPr *oox_nvPr)
{
	if (!oox_nvPr) return;
}
void OoxConverter::convert(PPTX::Logic::Paragraph *oox_paragraph)
{
	if (!oox_paragraph)return;

	bool styled = false;
	if (oox_paragraph->pPr.IsInit())
	{
		odf_writer::style_paragraph_properties	* paragraph_properties = odf_context()->text_context()->get_paragraph_properties();
													//свойства могут быть приписаны не только к параграфу, но и к самому объекту		
		if (!paragraph_properties)
		{
			odf_context()->styles_context()->create_style(L"",odf_types::style_family::Paragraph, true, false, -1);	
			paragraph_properties = odf_context()->styles_context()->last_state()->get_paragraph_properties();
			styled = true;
		}
		convert(oox_paragraph->pPr.GetPointer(), paragraph_properties);
	}
	
	odf_context()->text_context()->start_paragraph(styled);

	for (size_t i=0; i< oox_paragraph->RunElems.size();i++)
	{
		convert(&oox_paragraph->RunElems[i].as<OOX::WritingElement>());
	}
	odf_context()->text_context()->end_paragraph();
}
void OoxConverter::convert(PPTX::Logic::TextParagraphPr *oox_paragraph_pr, odf_writer::style_paragraph_properties * paragraph_properties)
{
	if (!oox_paragraph_pr)		return;
	if (!paragraph_properties)	return;

	if (oox_paragraph_pr->lnSpc.IsInit())
	{
		odf_types::length_or_percent length;
		convert(oox_paragraph_pr->lnSpc.GetPointer(), length);
		paragraph_properties->content().fo_line_height_ = odf_types::line_width(length);
	}
	if (oox_paragraph_pr->spcAft.IsInit())
	{
 		odf_types::length_or_percent length;
		convert(oox_paragraph_pr->spcAft.GetPointer(), length);
		paragraph_properties->content().fo_margin_bottom_ = length;
	}
	if (oox_paragraph_pr->spcBef.IsInit())
	{
 		odf_types::length_or_percent length;
		convert(oox_paragraph_pr->spcBef.GetPointer(), length);
		paragraph_properties->content().fo_margin_top_ = length;
	}
	if (oox_paragraph_pr->algn.IsInit())
	{
		switch(oox_paragraph_pr->algn->GetBYTECode())
		{
			case 0 : paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Center);		break;
			case 1 : 
			case 2 : 
			case 3 : paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify);	break;
			case 5 : paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Right);		break;
			case 4 : 
			default:
				paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Left);			break;
		}
	}
	if (oox_paragraph_pr->marL.IsInit())
	{
		paragraph_properties->content().fo_margin_left_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->marL.get() / 12700., odf_types::length::pt));
	}
	if (oox_paragraph_pr->marR.IsInit())
	{
		paragraph_properties->content().fo_margin_right_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->marR.get() / 12700., odf_types::length::pt));
	}
	if (oox_paragraph_pr->indent.IsInit())
	{
		paragraph_properties->content().fo_text_indent_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->indent.get() / 12700., odf_types::length::pt));
	}
	if (oox_paragraph_pr->rtl.IsInit())
	{
	}	
	//nullable<SimpleTypes::CDecimalNumber<> >				m_oLvl;
	//nullable<SimpleTypes::CCoordinate32<> >				m_oDefTabSz;
	//nullable<SimpleTypes::CTextFontAlignType<>>			m_oFontAlgn;



	if (oox_paragraph_pr->defRPr.IsInit())//может быть пустым !!!
	{
		odf_writer::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		if (text_properties) 
			convert(oox_paragraph_pr->defRPr.GetPointer(), text_properties);

		//надо подумать как брать последний по family !!! 
		//convert(oox_paragraph_pr->m_oDefRunProperty.GetPointer());
		//odf_context()->text_context()->set_parent_span_style(odf_context()->styles_context()->last_state().get_name());
	}
}

void OoxConverter::convert(PPTX::Logic::TextSpacing *oox_spacing, cpdoccore::odf_types::length_or_percent & length_or_percent)
{
	if (!oox_spacing) return;

	if ( oox_spacing->spcPct.IsInit() )
	{
		odf_types::percent percent = odf_types::percent(oox_spacing->GetVal());
		length_or_percent = odf_types::length_or_percent(percent);
	}
	if ( oox_spacing->spcPts.IsInit() )
	{
		odf_types::length length = odf_types::length(oox_spacing->GetVal() / 12700., odf_types::length::pt);
		length_or_percent = odf_types::length_or_percent(length);
	}}

void OoxConverter::convert(PPTX::Logic::RunProperties *oox_run_pr, odf_writer::style_text_properties * text_properties)
{
	if (!oox_run_pr)		return;
	if (!text_properties)	return;

	if (oox_run_pr->b.IsInit())
	{
		if (oox_run_pr->b.get() == true) 
			text_properties->content().fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);
	}

	PPTX::Logic::SolidFill*	solidFill	= &oox_run_pr->Fill.as<PPTX::Logic::SolidFill>();
	if (solidFill)
	{
		std::wstring hexColor;
		_CP_OPT(double) opacity;
		convert(&solidFill->Color, hexColor, opacity);
		
		text_properties->content().fo_color_ = odf_types::color(std::wstring(L"#") + hexColor);
	}
	if (oox_run_pr->i.IsInit())
	{
		if (oox_run_pr->i.get() ==true)
			text_properties->content().fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	}
	if (oox_run_pr->sz.IsInit())
	{
		convert(oox_run_pr->sz.get()/100., text_properties->content().fo_font_size_);
		convert(oox_run_pr->sz.get()/100., text_properties->content().style_font_size_asian_);
		convert(oox_run_pr->sz.get()/100., text_properties->content().style_font_size_complex_);
	}
	if (oox_run_pr->latin.IsInit())
	{
		if (!oox_run_pr->latin->typeface.empty())	
			text_properties->content().fo_font_family_ = oox_run_pr->latin->typeface;
		else
		{
			text_properties->content().fo_font_family_ = L"Calibri";//default_font; ???? 
		}
	}
	if (oox_run_pr->ea.IsInit())
	{
		if (!oox_run_pr->ea->typeface.empty())	
			text_properties->content().style_font_family_asian_ = oox_run_pr->ea->typeface;
		else
		{
		}
	}
	if (oox_run_pr->cs.IsInit())
	{
		if (!oox_run_pr->cs->typeface.empty())
			text_properties->content().style_font_family_complex_ = oox_run_pr->cs->typeface;
		else
		{
		}
	}
	if (oox_run_pr->lang.IsInit())
	{
        std::wstring oox_language =  oox_run_pr->lang.get(), oox_country;
        int res = oox_language.find(L"-");
        if (res >= 0)
		{
            oox_country = oox_language.substr(res);
            oox_language = oox_language.substr(0, res);
		}
		
		text_properties->content().fo_language_ = oox_language;
        if (oox_country.length() > 0)
			text_properties->content().fo_country_ = oox_country;

	}
	if (oox_run_pr->spc.IsInit())
	{
		text_properties->content().fo_letter_spacing_ = odf_types::letter_spacing(odf_types::length(oox_run_pr->spc.get()/100., odf_types::length::pt));
	}
	if (oox_run_pr->u.IsInit())
	{
		text_properties->content().style_text_underline_style_	= odf_types::line_style	(odf_types::line_style::Solid);
		text_properties->content().style_text_underline_type_	= odf_types::line_type	(odf_types::line_type::Single);
		
		switch(oox_run_pr->u->GetBYTECode())
		{
		case SimpleTypes::underlineNone	:
                text_properties->content().style_text_underline_type_= odf_types::line_type(odf_types::line_type::None);break;
		case SimpleTypes::underlineDash :
		case SimpleTypes::underlineDashedHeavy:
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Dash);break;
		case SimpleTypes::underlineDotDash :
		case SimpleTypes::underlineDashDotHeavy:
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::DotDash);break;
		case SimpleTypes::underlineDashLong:
		case SimpleTypes::underlineDashLongHeavy:
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::LongDash);break;
		case SimpleTypes::underlineDotDotDash:
		case SimpleTypes::underlineDashDotDotHeavy :
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::DotDotDash);break;
		case SimpleTypes::underlineDotted:
		case SimpleTypes::underlineDottedHeavy:
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Dotted);break;
		case SimpleTypes::underlineWave :
		case SimpleTypes::underlineWavyHeavy :
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Wave);break;
		case SimpleTypes::underlineDouble :
		case SimpleTypes::underlineThick :
				text_properties->content().style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);break;
		case SimpleTypes::underlineWavyDouble :
				text_properties->content().style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);
				text_properties->content().style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Wave);break;
		}
	}
}
void OoxConverter::convert(PPTX::Logic::Run *oox_run)
{
	if (!oox_run) return;
	bool styled = false;

	if (oox_run->rPr.IsInit())
	{
		odf_writer::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		
		if (!text_properties)
		{
			odf_context()->styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);	
			text_properties = odf_context()->styles_context()->last_state()->get_text_properties();
			styled = true;
		}
		convert(oox_run->rPr.GetPointer(), text_properties);
	}
	
	odf_context()->text_context()->start_span(styled);	
		odf_context()->text_context()->add_text_content( oox_run->GetText());
	odf_context()->text_context()->end_span();
}
void OoxConverter::convert(PPTX::Logic::Fld *oox_fld)
{
	if (!oox_fld) return;
}
void OoxConverter::convert(PPTX::Logic::Br *oox_br)
{
	if (!oox_br) return;
}
void OoxConverter::convert(PPTX::Logic::MathParaWrapper *oox_math)
{
	if (!oox_math) return;
}

}
