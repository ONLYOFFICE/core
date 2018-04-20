﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "XlsxConverter.h"
#include "PptxConverter.h"
#include "DocxConverter.h"

#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Slide.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTreeElem.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/GraphicFrame.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Pic.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/CxnSp.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/SpTree.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Table/Table.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SrgbClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/PrstClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Colors/SchemeClr.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/TextFont.h"

#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"
#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Word.h"

#include "../OdfFormat/odf_conversion_context.h"
#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"
#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"
#include "../OdfFormat/styles_list.h"

using namespace cpdoccore;

namespace Oox2Odf
{
	double Emu2Pt(int emu)
	{
		return (1.0 * emu / (635 * 20.0));
	}
//----------------------------------------------------------------------------------
void OoxConverter::convert(PPTX::Logic::SpTreeElem *oox_element)
{
	if (!oox_element) return;
	if (!oox_element->is_init()) return;

    smart_ptr<PPTX::WrapperWritingElement> elem = oox_element->GetElem();

	convert (elem.operator->());
}

void OoxConverter::convert_font(PPTX::Theme *theme, std::wstring & font)
{
	if (font == L"+mj-lt")
		font = theme ? theme->themeElements.fontScheme.majorFont.latin.typeface : L"";
	else if (font == L"+mn-lt")
		font = theme ? theme->themeElements.fontScheme.minorFont.latin.typeface : L"";
	else if (font == L"+mj-ea")
		font = theme ? theme->themeElements.fontScheme.majorFont.ea.typeface : L"";
	else if (font == L"+mn-ea")
		font = theme ? theme->themeElements.fontScheme.minorFont.ea.typeface : L"";
	else if (font == L"+mj-cs")
		font = theme ? theme->themeElements.fontScheme.majorFont.cs.typeface : L"";
	else if (font == L"+mn-cs")
		font = theme ? theme->themeElements.fontScheme.minorFont.cs.typeface : L"";
}

void OoxConverter::convert(PPTX::Logic::GraphicFrame *oox_graphic_frame)
{
	if (!oox_graphic_frame)return;

	convert(&oox_graphic_frame->nvGraphicFramePr);		
	convert(oox_graphic_frame->xfrm.GetPointer());		
	
	if ( oox_graphic_frame->chartRec.is_init())
	{
		convert(oox_graphic_frame->chartRec.GetPointer());
	}
	else if ( oox_graphic_frame->smartArt.is_init())
	{
		OoxConverter::convert(oox_graphic_frame->smartArt.GetPointer());
	}
	else if ( oox_graphic_frame->olePic.is_init())
	{
		OoxConverter::convert(oox_graphic_frame->olePic.GetPointer());
	}
	else if ( oox_graphic_frame->table.is_init())
	{
		PptxConverter *pptx_converter = dynamic_cast<PptxConverter *>(this);
		if (pptx_converter)
			pptx_converter->convert(oox_graphic_frame->table.GetPointer());
	}
	else if ( oox_graphic_frame->element.is_init())
	{
		OoxConverter::convert(oox_graphic_frame->element.GetElem().operator->());
	}
}
void OoxConverter::convert(PPTX::Logic::NvGraphicFramePr *oox_framePr)
{
	if (oox_framePr == NULL) return;

}

void OoxConverter::convert(PPTX::Logic::Xfrm *oox_xfrm)
{
	if (oox_xfrm == NULL) return;	//CTransform2D

	_CP_OPT(double) x, y, width, height;

	if (oox_xfrm->offX.IsInit())	x = Emu2Pt(*oox_xfrm->offX);
	if (oox_xfrm->offY.IsInit())	y = Emu2Pt(*oox_xfrm->offY);
	
	if (oox_xfrm->extX.IsInit())	width	= Emu2Pt(*oox_xfrm->extX);
	if (oox_xfrm->extY.IsInit())	height	= Emu2Pt(*oox_xfrm->extY);
	
	odf_context()->drawing_context()->set_position( x, y);
	odf_context()->drawing_context()->set_size(	width, height);					
	
	if (oox_xfrm->flipH.get_value_or(false))	odf_context()->drawing_context()->set_flip_H(true);
	if (oox_xfrm->flipV.get_value_or(false))	odf_context()->drawing_context()->set_flip_V(true);
	
	if (oox_xfrm->rot.get_value_or(0) > 0)
		odf_context()->drawing_context()->set_rotate(360. - oox_xfrm->rot.get_value_or(0)/60000.);
}
void OoxConverter::convert(PPTX::Logic::Xfrm *oox_txbx, PPTX::Logic::Xfrm *oox_xfrm)
{
	if (oox_txbx == NULL) return;
	if (oox_xfrm == NULL) return;

	if (oox_txbx->rot.IsInit() && oox_xfrm->rot.IsInit())
	{
		int angle1 = *oox_txbx->rot / 60000;
		int angle2 = *oox_xfrm->rot / 60000;

		oox_txbx->rot = (angle1 + angle2) * 60000;

	}

	convert(oox_txbx);
}
void OoxConverter::convert(PPTX::Logic::Pic *oox_picture)
{
	if (!oox_picture)return;

	if (oox_picture->spPr.Geometry.is_init())
	{
		int type = SimpleTypes::shapetypeRect;
		if ( oox_picture->spPr.Geometry.is<PPTX::Logic::CustGeom>() )
		{
			type = 1000;
		}
		else if ( oox_picture->spPr.Geometry.is<PPTX::Logic::PrstGeom>() )
		{
			const PPTX::Logic::PrstGeom& prstGeom = oox_picture->spPr.Geometry.as<PPTX::Logic::PrstGeom>();
			
			SimpleTypes::CShapeType<> preset;
			preset.FromString(prstGeom.prst.get());
			type = preset.GetValue();
		}

		if (type != SimpleTypes::shapetypeRect)
		{
			odf_context()->drawing_context()->start_shape(type);			
				
				convert(&oox_picture->spPr, oox_picture->style.GetPointer());
				convert(&oox_picture->nvPicPr);	
				
				odf_context()->drawing_context()->start_area_properties();
					convert(&oox_picture->blipFill);
				odf_context()->drawing_context()->end_area_properties();				

			odf_context()->drawing_context()->end_shape();
			return;
		}
	}
//--------------------------------------------------------------------------------------
	bool bEmbedded = true;
	std::wstring pathImage;
	if (oox_picture->blipFill.blip.IsInit())
	{
		if (oox_picture->blipFill.blip->embed.IsInit())
		{
			std::wstring sID = oox_picture->blipFill.blip->embed->get();
			pathImage = find_link_by_id(sID, 1);
			
		}
		else if (oox_picture->blipFill.blip->link.IsInit())
		{
			pathImage = oox_picture->blipFill.blip->link->get();	
			bEmbedded = false;
		}
	}
//--------------------------------------------------------------------------------------
	std::wstring odf_ref_image;

	if (oox_picture->nvPicPr.nvPr.media.is_init())
	{
		if (oox_picture->nvPicPr.nvPr.media.is<PPTX::Logic::MediaFile>())
		{
			PPTX::Logic::MediaFile & media = oox_picture->nvPicPr.nvPr.media.as<PPTX::Logic::MediaFile>();

			std::wstring sID		= media.link.get();
			std::wstring pathMedia	= find_link_by_id(sID, 3);

			double start = -1, end = -1;

			for (size_t i = 0; i < oox_picture->nvPicPr.nvPr.extLst.size(); i++)
			{
				PPTX::Logic::Ext & ext = oox_picture->nvPicPr.nvPr.extLst[i];
				if (pathMedia.empty() && ext.link.IsInit())
				{
					pathMedia = find_link_by_id(ext.link->get(), 3);
					//например файлики mp3
				}
				if (ext.st.IsInit())	start	= *ext.st;
				if (ext.end.IsInit())	end		= *ext.end;
			}
			
			std::wstring odf_ref_media = odf_context()->add_media(pathMedia);

			if (!odf_ref_media.empty())
			{
				odf_context()->drawing_context()->start_media(odf_ref_media);
				//... params
				
				OoxConverter::convert(&oox_picture->nvPicPr.cNvPr);		
				OoxConverter::convert(&oox_picture->spPr, oox_picture->style.GetPointer());

				odf_ref_image = bEmbedded ? odf_context()->add_image(pathImage) : pathImage;
				odf_context()->drawing_context()->set_image_replacement(odf_ref_image);
				
				odf_context()->drawing_context()->end_media();

				return;
			}			
		}
	}
	if (oox_picture->oleObject.IsInit())
	{
		if (pathImage.empty() && oox_picture->blipFill.blip.IsInit())
		{
			pathImage = oox_picture->blipFill.blip->oleFilepathImage;
		}
		std::wstring pathOle;
		
		if (oox_picture->oleObject->m_oId.IsInit())
		{
			pathOle = find_link_by_id(oox_picture->oleObject->m_oId->get(), 4);
		}
		std::wstring odf_ref_ole = odf_context()->add_oleobject(pathOle);

		if (!odf_ref_ole.empty())
		{
			odf_context()->drawing_context()->start_object_ole(odf_ref_ole);

			if (oox_picture->oleObject->m_sProgId.IsInit())
			{
				odf_context()->drawing_context()->set_program(*oox_picture->oleObject->m_sProgId);
			}
			
			if (pathImage.empty() && oox_picture->oleObject->m_sShapeId.IsInit())
			{
				PPTX::Slide			*pSlide			= dynamic_cast<PPTX::Slide*>(current_document());
				PPTX::SlideMaster	*pSlideMaster	= dynamic_cast<PPTX::SlideMaster*>(current_document());
				
				OOX::CVmlDrawing *pVml = pSlide ? pSlide->Vml.operator->() : (pSlideMaster ? pSlideMaster->Vml.operator->() : NULL);
				
				if (pVml)
				{	
					std::wstring sShapeId = oox_picture->oleObject->m_sShapeId.get();
                    boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = pVml->m_mapShapes.find(sShapeId);

                    if (pVml->m_mapShapes.end() != pFind)
					{
						OOX::Vml::CVmlCommonElements* pShape = dynamic_cast<OOX::Vml::CVmlCommonElements*>(pFind->second.pElement);

						if (pShape)
						{						
                            for(size_t i = 0; i < pShape->m_arrItems.size(); ++i)
							{
                                OOX::WritingElement* pChildElemShape = pShape->m_arrItems[i];

								if(OOX::et_v_imagedata == pChildElemShape->getType())
								{
									OOX::Vml::CImageData* pImageData = static_cast<OOX::Vml::CImageData*>(pChildElemShape);									
														
									std::wstring sIdImageFileCache;

									if (pImageData->m_oRelId.IsInit())		sIdImageFileCache = pImageData->m_oRelId->GetValue();
									else if (pImageData->m_rId.IsInit())	sIdImageFileCache = pImageData->m_rId->GetValue();
																		
									if (!sIdImageFileCache.empty())
									{
										//ищем физический файл ( rId относительно vml_drawing)									
										smart_ptr<OOX::File> pFile = pVml->Find(sIdImageFileCache);
										
										if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
										{
											OOX::Image*	pImageFileCache = static_cast<OOX::Image*>(pFile.operator->());
											
											pathImage = pImageFileCache->filename().GetPath();
										}
									}
								}
							}
						}
					}
				}

			}

			odf_ref_image = bEmbedded ? odf_context()->add_imageobject(pathImage) : pathImage; 			
			odf_context()->drawing_context()->set_image_replacement(odf_ref_image);

			odf_context()->drawing_context()->end_object_ole();
			return;
		}
	}
//--------------------------------------------------------------------------------------
	odf_ref_image = bEmbedded ? odf_context()->add_image(pathImage) : pathImage;
	
	odf_context()->drawing_context()->start_image(odf_ref_image);
	{
		double Width = 0, Height = 0;
        _graphics_utils_::GetResolution(pathImage.c_str(), Width, Height);
		
		if (oox_picture->blipFill.tile.IsInit()) 
		{
			odf_context()->drawing_context()->set_image_style_repeat(2);
		}
		if (oox_picture->blipFill.stretch.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(1);
		}
		if (oox_picture->blipFill.srcRect.IsInit() && Width > 0 && Height >0 )
		{
			odf_context()->drawing_context()->set_image_client_rect_inch(
				XmlUtils::GetInteger(oox_picture->blipFill.srcRect->l.get_value_or(L"0")) * Width	/100.	/currentSystemDPI,
				XmlUtils::GetInteger(oox_picture->blipFill.srcRect->t.get_value_or(L"0")) * Height	/100.	/currentSystemDPI,
				XmlUtils::GetInteger(oox_picture->blipFill.srcRect->r.get_value_or(L"0")) * Width	/100.	/currentSystemDPI, 
				XmlUtils::GetInteger(oox_picture->blipFill.srcRect->b.get_value_or(L"0")) * Height	/100.	/currentSystemDPI);
		}		

		OoxConverter::convert(&oox_picture->nvPicPr.cNvPr);		
		OoxConverter::convert(&oox_picture->spPr, oox_picture->style.GetPointer());

	}
	odf_context()->drawing_context()->end_image();
}

void OoxConverter::convert(PPTX::Logic::SmartArt *oox_smart_art)
{
	if (oox_smart_art == NULL) return;
	if (oox_smart_art->id_data.IsInit() == false) return;

	oox_smart_art->LoadDrawing(current_document());

	if (oox_smart_art->m_diag.IsInit())
	{
        _CP_OPT(double) x, y, width, height, cx = 1., cy= 1.;

		odf_context()->drawing_context()->get_size (width, height);
		odf_context()->drawing_context()->get_position (x, y);

		oox_current_child_document = oox_smart_art->m_pFileContainer.operator->();

		odf_context()->drawing_context()->start_group();

		odf_context()->drawing_context()->set_group_size (width, height, width, height);
		odf_context()->drawing_context()->set_group_position (x, y, cx, cy);

		for (size_t i = 0; i < oox_smart_art->m_diag->SpTreeElems.size(); i++)
		{
			odf_context()->drawing_context()->start_drawing();
				convert(&oox_smart_art->m_diag->SpTreeElems[i]);
			odf_context()->drawing_context()->end_drawing();
		}

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
		OOX::Spreadsheet::CChartSpace* pChart = dynamic_cast<OOX::Spreadsheet::CChartSpace*>(oFile.operator->());
		
		if (pChart)
		{
			oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(pChart);	
			odf_context()->drawing_context()->start_object(odf_context()->get_next_name_object());
			{
				odf_context()->start_chart();
					odf_context()->chart_context()->set_chart_size(width, height);		
		
					OoxConverter::convert(pChart->m_oChartSpace.m_oSpPr.GetPointer());			
			
					OoxConverter::convert(&pChart->m_oChartSpace);
				odf_context()->end_chart();
			}
			odf_context()->drawing_context()->end_object();	
			oox_current_child_document = NULL;
		}
	}
}
void OoxConverter::convert(PPTX::Logic::CNvGrpSpPr *oox_cnvGrpSpPr)
{
	if (!oox_cnvGrpSpPr) return;
}
void OoxConverter::convert(PPTX::Logic::NvGrpSpPr *oox_nvGrpSpPr)
{
	if (!oox_nvGrpSpPr) return;

	odf_context()->drawing_context()->set_group_name(oox_nvGrpSpPr->cNvPr.name);
	odf_context()->drawing_context()->set_group_z_order(oox_nvGrpSpPr->cNvPr.id);

	if (oox_nvGrpSpPr->cNvPr.descr.IsInit())
		odf_context()->drawing_context()->set_description(oox_nvGrpSpPr->cNvPr.descr.get());

	convert(&oox_nvGrpSpPr->cNvGrpSpPr);
	convert(&oox_nvGrpSpPr->nvPr);
}
void OoxConverter::convert(PPTX::Logic::GrpSpPr *oox_grpSpPr)
{
	if (!oox_grpSpPr) return;

	if (oox_grpSpPr->xfrm.IsInit())
	{
		if (oox_grpSpPr->xfrm->flipH.IsInit())
			odf_context()->drawing_context()->set_group_flip_H(oox_grpSpPr->xfrm->flipH.get());
		
		if (oox_grpSpPr->xfrm->flipV.IsInit())
			odf_context()->drawing_context()->set_group_flip_V(oox_grpSpPr->xfrm->flipV.get());

		_CP_OPT(double) cx, cy, ch_cx, ch_cy;
		
		if (oox_grpSpPr->xfrm->extX.IsInit())
			cx = oox_grpSpPr->xfrm->extX.get() / 12700.;
		if (oox_grpSpPr->xfrm->extY.IsInit())
			cy = oox_grpSpPr->xfrm->extY.get() / 12700.;
		
		if (oox_grpSpPr->xfrm->chExtX.IsInit())
			ch_cx = oox_grpSpPr->xfrm->chExtX.get() / 12700.;
		if (oox_grpSpPr->xfrm->chExtY.IsInit())
			ch_cy = oox_grpSpPr->xfrm->chExtY.get() / 12700.;

		odf_context()->drawing_context()->set_group_size( cx, cy, ch_cx, ch_cy );

		_CP_OPT(double) x, y, ch_x, ch_y, ext_x, ext_y;
			
		if (oox_grpSpPr->xfrm->offX.IsInit())
			x =	oox_grpSpPr->xfrm->offX.get() / 12700.;
		if (oox_grpSpPr->xfrm->offY.IsInit())
			y = oox_grpSpPr->xfrm->offY.get() / 12700.;

		if (oox_grpSpPr->xfrm->chOffX.IsInit())
			ch_x = oox_grpSpPr->xfrm->chOffX.get() / 12700.;
		if (oox_grpSpPr->xfrm->chOffY.IsInit())
			ch_y = oox_grpSpPr->xfrm->chOffY.get() / 12700.;

		int group_level = odf_context()->drawing_context()->get_group_level();

		odf_context()->drawing_context()->get_position(ext_x, ext_y);

		if (ext_x && ext_y && group_level < 2)
		{
			x = ext_x;
			y = ext_y;
		}

		odf_context()->drawing_context()->set_group_position( x, y, ch_x, ch_y );

		if (oox_grpSpPr->xfrm->rot.IsInit())
			odf_context()->drawing_context()->set_group_rotate(oox_grpSpPr->xfrm->rot.get() / 60000.);
	}
	//UniFill					Fill;
	//EffectProperties			EffectList;
	//nullable<Scene3d>			scene3d;
}

void OoxConverter::convert(PPTX::Logic::SpTree *oox_shape_tree)
{
	if (oox_shape_tree == NULL) return;
	
	odf_context()->drawing_context()->start_group();

	convert(&oox_shape_tree->nvGrpSpPr);
	convert(&oox_shape_tree->grpSpPr);

	//odf_context()->drawing_context()->set_group_size (width, height, width, height);
	//odf_context()->drawing_context()->set_group_position (x, y, cx, cy);

	for (size_t i = 0; i < oox_shape_tree->SpTreeElems.size(); i++)
	{
		odf_context()->drawing_context()->start_drawing();
			convert(oox_shape_tree->SpTreeElems[i].GetElem().operator->());
		odf_context()->drawing_context()->end_drawing();
	}

	odf_context()->drawing_context()->end_group();	

}
void OoxConverter::convert(PPTX::Logic::CxnSp *oox_connect)
{
	if (oox_connect == NULL) return;

	int type = SimpleTypes::shapetypeLine;

	if ( oox_connect->spPr.Geometry.is<PPTX::Logic::PrstGeom>() )
	{
		const PPTX::Logic::PrstGeom& prstGeom = oox_connect->spPr.Geometry.as<PPTX::Logic::PrstGeom>();
		
		SimpleTypes::CShapeType<> preset;
		preset.FromString(prstGeom.prst.get());
		type = preset.GetValue();
	}
	odf_context()->drawing_context()->start_shape(type);
	
		convert(&oox_connect->spPr, oox_connect->style.GetPointer());
		convert(&oox_connect->nvCxnSpPr);

	odf_context()->drawing_context()->end_shape();
}
void OoxConverter::convert(PPTX::Logic::Shape *oox_shape)
{
	if (oox_shape == NULL) return;

	if (oox_shape->txXfrm.IsInit())
	{
		odf_context()->drawing_context()->start_group();
		odf_context()->drawing_context()->start_drawing();
	}
	
	int type = 1000; //custom

	if (oox_shape->spPr.Geometry.is_init())
	{
		if ( oox_shape->spPr.Geometry.is<PPTX::Logic::PrstGeom>() )
		{
			const PPTX::Logic::PrstGeom& prstGeom = oox_shape->spPr.Geometry.as<PPTX::Logic::PrstGeom>();
			
			SimpleTypes::CShapeType<> preset;
			preset.FromString(prstGeom.prst.get());
			type = preset.GetValue();
		}

		if (type == SimpleTypes::shapetypeRect && (oox_shape->txBody.IsInit() || oox_shape->oTextBoxShape.IsInit())) 
			type = 2000;

		if (type == 2000)
		{
			PPTX::Logic::BodyPr *bodyPr = NULL;
			if (oox_shape->txBody.IsInit())		bodyPr = oox_shape->txBody->bodyPr.GetPointer();
			else								bodyPr = oox_shape->oTextBoxBodyPr.GetPointer();
			
			if (bodyPr && bodyPr->fromWordArt.get_value_or(false))
			{
				int wordart_type = convert(bodyPr->prstTxWarp.GetPointer());
				if (wordart_type > 0) type = wordart_type;
			}
		}
	}
	else if (oox_shape->nvSpPr.nvPr.ph.is_init())
	{
		type = 2000;
	}

	if (type < 0)return;
//-----------------------------------------------------------------------------
	odf_context()->drawing_context()->start_shape(type);
	
	convert(&oox_shape->spPr, oox_shape->style.GetPointer());

	convert(&oox_shape->nvSpPr);	

	if (oox_shape->txXfrm.IsInit() == false)
	{
		if (oox_shape->oTextBoxShape.IsInit()) //docx sdt
		{
			DocxConverter *docx_converter = dynamic_cast<DocxConverter*>(this);
			if (docx_converter)
			{
				odf_context()->start_text_context();

					//docx_converter->convert(oox_shape->oTextBoxShape.GetPointer());
					
                    for (std::vector<OOX::WritingElement*>::iterator	it = oox_shape->oTextBoxShape->m_arrItems.begin(); it != oox_shape->oTextBoxShape->m_arrItems.end(); ++it)
					{
						docx_converter->convert(*it);
				
						convert(oox_shape->oTextBoxBodyPr.GetPointer());
						
						if (oox_shape->style.IsInit())
							convert(&oox_shape->style->fontRef);
					}				
					odf_context()->drawing_context()->set_text( odf_context()->text_context());
				
				odf_context()->end_text_context();	
			}
		}
		else
			convert(oox_shape->txBody.GetPointer(), oox_shape->style.GetPointer());
	}

	odf_context()->drawing_context()->end_shape();
	
	if (oox_shape->txXfrm.IsInit())
	{
		odf_context()->drawing_context()->end_drawing();
		
		odf_context()->drawing_context()->start_drawing();
		odf_context()->drawing_context()->start_text_box();	

			convert(oox_shape->txXfrm.GetPointer(), oox_shape->spPr.xfrm.GetPointer());
			convert(oox_shape->txBody.GetPointer(), oox_shape->style.GetPointer());
			
			odf_context()->drawing_context()->start_line_properties();
				odf_context()->drawing_context()->set_no_fill();
			odf_context()->drawing_context()->end_line_properties();
		
		odf_context()->drawing_context()->end_text_box();	
		odf_context()->drawing_context()->end_drawing();
		
		odf_context()->drawing_context()->end_group();	
	}
}

void OoxConverter::convert(PPTX::Logic::SpPr *oox_spPr, PPTX::Logic::ShapeStyle* oox_sp_style)
{
	if (oox_spPr == NULL) return;

	convert(oox_spPr->xfrm.GetPointer());

	PPTX::Logic::PrstGeom* prstGeom = NULL;
	PPTX::Logic::CustGeom* custGeom = NULL;

	if (oox_spPr->Geometry.is<PPTX::Logic::PrstGeom>())
	{
		prstGeom = &oox_spPr->Geometry.as<PPTX::Logic::PrstGeom>();
	}
	if (oox_spPr->Geometry.is<PPTX::Logic::CustGeom>())
	{
		custGeom = &oox_spPr->Geometry.as<PPTX::Logic::CustGeom>();
	}

	convert(prstGeom);
	convert(custGeom);

	bool bLine = odf_context()->drawing_context()->isLineShape();

	if (custGeom && !custGeom->cxnLst.empty())
		bLine = true;

	odf_context()->drawing_context()->start_area_properties();
	{
		if (bLine)
		{	
			odf_context()->drawing_context()->set_no_fill();
		}
		else
		{
			if (oox_spPr->Fill.is_init())
				convert(&oox_spPr->Fill);
			else if (oox_sp_style)
				convert(&oox_sp_style->fillRef, 1);
		}
	}
	odf_context()->drawing_context()->end_area_properties();

	odf_context()->drawing_context()->start_line_properties();
	{
		convert(oox_spPr->ln.GetPointer(), 0, oox_sp_style);
	}
	odf_context()->drawing_context()->end_line_properties();
//-----------------------------------------------------------------------------------------------------------------------------
	PPTX::Logic::EffectLst*	effectLst = NULL;
	
	if (oox_spPr->EffectList.is<PPTX::Logic::EffectLst>())
	{
		effectLst = &oox_spPr->EffectList.as<PPTX::Logic::EffectLst>();
	}

	if		(effectLst)		convert(effectLst);
	else if (oox_sp_style)	convert(&oox_sp_style->effectRef, 3);

	//convert(oox_spPr->ExtLst.GetPointer());

	//nullable<OOX::Drawing::CEffectContainer>          EffectDag;

	//nullable<OOX::Drawing::COfficeArtExtensionList>   ExtLst;
	//nullable<OOX::Drawing::CScene3D>                  Scene3D;
	//nullable<OOX::Drawing::CShape3D>                  Sp3D;	
//-----------------------------------------------------------------------------------------------------------------------------
}

void OoxConverter::convert(OOX::Drawing::COfficeArtExtensionList *ext_list)
{
	if (ext_list == NULL)return;

	for (size_t i = 0; i < ext_list->m_arrExt.size(); i++)
	{
		convert(ext_list->m_arrExt[i]);
	}
}

void OoxConverter::convert(OOX::Drawing::COfficeArtExtension *art_ext)
{
	if (art_ext == NULL)return;

	if (art_ext->m_oSparklineGroups.IsInit() || art_ext->m_oAltTextTable.IsInit() || !art_ext->m_arrConditionalFormatting.empty())
	{
		XlsxConverter *xlsx_converter = dynamic_cast<XlsxConverter*>(this);
		if (xlsx_converter)
		{		
			xlsx_converter->convert(art_ext->m_oSparklineGroups.GetPointer());
			xlsx_converter->convert(art_ext->m_oAltTextTable.GetPointer());
		
			for (size_t i = 0; i < art_ext->m_arrConditionalFormatting.size(); i++)
			{
				xlsx_converter->convert(art_ext->m_arrConditionalFormatting[i]);
			}	
		}
	}


	//convert(art_ext->m_oCompatExt.GetPointer());
	//convert(art_ext->m_oDataModelExt.GetPointer());
}

void OoxConverter::convert(PPTX::Logic::UniFill *oox_fill, DWORD nARGB)
{
    if (oox_fill == NULL) return;
	
	if (oox_fill->is<PPTX::Logic::NoFill>())		odf_context()->drawing_context()->set_no_fill();

    if (oox_fill->is<PPTX::Logic::BlipFill>())		convert(&oox_fill->as<PPTX::Logic::BlipFill>());
    if (oox_fill->is<PPTX::Logic::GradFill>())		convert(&oox_fill->as<PPTX::Logic::GradFill>(), nARGB);
    if (oox_fill->is<PPTX::Logic::SolidFill>())		convert(&oox_fill->as<PPTX::Logic::SolidFill>(),nARGB);
    if (oox_fill->is<PPTX::Logic::PattFill>())		convert(&oox_fill->as<PPTX::Logic::PattFill>(), nARGB);
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
		odf_context()->drawing_context()->add_modifier(oox_geom->avLst[i].fmla.get_value_or(L"0"));
	}
}
void OoxConverter::convert(PPTX::Logic::CustGeom *oox_cust_geom)
{
	if (!oox_cust_geom) return;

	for (size_t i = 0; i < oox_cust_geom->gdLst.size(); i++)
	{
		odf_context()->drawing_context()->add_formula(oox_cust_geom->gdLst[i].name.get_value_or(L""), oox_cust_geom->gdLst[i].fmla.get_value_or(L""));
	}
	for (size_t i = 0; i < oox_cust_geom->pathLst.size(); i++)
	{
		convert(&oox_cust_geom->pathLst[i]);
	}
	for (size_t i = 0; i < oox_cust_geom->avLst.size(); i++)
	{
		odf_context()->drawing_context()->add_modifier(oox_cust_geom->avLst[i].fmla.get_value_or(L"0"));
	}
	for (size_t i = 0; i < oox_cust_geom->ahLst.size(); i++)
	{
		convert(oox_cust_geom->ahLst[i].ah.operator->());
	}
	if (oox_cust_geom->rect.IsInit())
	{
		odf_context()->drawing_context()->set_textarea (oox_cust_geom->rect->l.get_value_or(L"0"),
														oox_cust_geom->rect->t.get_value_or(L"0"),
														oox_cust_geom->rect->r.get_value_or(L"0"),
														oox_cust_geom->rect->b.get_value_or(L"0"));
	}
}
void OoxConverter::convert(PPTX::Logic::AhXY *oox_handle)
{
	if (!oox_handle) return;

	odf_context()->drawing_context()->add_handle(oox_handle->x,	oox_handle->y, 
												oox_handle->gdRefX.get_value_or(L""), oox_handle->gdRefY.get_value_or(L""), 
												oox_handle->minX.get_value_or(L""), oox_handle->maxX.get_value_or(L""), 
												oox_handle->minX.get_value_or(L""), oox_handle->maxY.get_value_or(L""));

}
void OoxConverter::convert(PPTX::Logic::AhPolar *oox_handle)
{
	if (!oox_handle) return;
}

void OoxConverter::convert(PPTX::Logic::EffectLst *oox_effect_list)
{
	if (!oox_effect_list) return;

	convert(oox_effect_list->innerShdw.GetPointer());
	convert(oox_effect_list->outerShdw.GetPointer());
	convert(oox_effect_list->prstShdw.GetPointer());
}
void OoxConverter::convert(PPTX::Logic::InnerShdw *oox_shadow)
{
	if (oox_shadow == NULL) return;

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	convert(&oox_shadow->Color, hexColor, opacity);

	odf_context()->drawing_context()->set_shadow(2, hexColor, opacity, oox_shadow->dist.IsInit() ? oox_shadow->dist.get() / 12700. : 0);
}
void OoxConverter::convert(PPTX::Logic::OuterShdw *oox_shadow)
{
	if (oox_shadow == NULL) return; 

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	convert(&oox_shadow->Color, hexColor, opacity);

	odf_context()->drawing_context()->set_shadow(1, hexColor, opacity, oox_shadow->dist.IsInit() ? oox_shadow->dist.get() / 12700. : 0);

}
void OoxConverter::convert(PPTX::Logic::PrstShdw *oox_shadow)
{
	if (oox_shadow == NULL) return; 

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	convert(&oox_shadow->Color, hexColor, opacity);

	//odf_context()->drawing_context()->set_shadow(1, hexColor, opacity, oox_shadow->dist.IsInit() ? oox_shadow->dist.get() / 12700. : 0);
}

void OoxConverter::convert(PPTX::Logic::EffectStyle *oox_effects)
{
	if (!oox_effects) return;

	if (oox_effects->EffectList.is_init())
	{
		convert(oox_effects->EffectList.List.GetPointer());
	}
	if (oox_effects->scene3d.IsInit())
	{
	}
	if (oox_effects->sp3d.IsInit())
	{
	}
}
void OoxConverter::convert(PPTX::Logic::Path2D *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	odf_context()->drawing_context()->set_viewBox(oox_geom_path->w.get_value_or(0), oox_geom_path->h.get_value_or(0));

	for (size_t i = 0 ; i < oox_geom_path->Paths.size(); i++)
	{
		if (oox_geom_path->Paths[i].Path2D.is<PPTX::Logic::PathBase>())
		{
			convert(&oox_geom_path->Paths[i].Path2D.as<PPTX::Logic::PathBase>());
		}
	}

	if (oox_geom_path->stroke.IsInit() && *oox_geom_path->stroke == false)
		odf_context()->drawing_context()->add_path_element(std::wstring(L"S"), L"");
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"N"), L"");
}

void OoxConverter::convert(PPTX::Logic::PathBase *oox_path)
{
	if (!oox_path) return;

	PPTX::Logic::MoveTo*		moveTo		= dynamic_cast<PPTX::Logic::MoveTo*>	(oox_path);
	PPTX::Logic::LineTo*		lineTo		= dynamic_cast<PPTX::Logic::LineTo*>	(oox_path);
	PPTX::Logic::CubicBezTo*	cubicBezTo	= dynamic_cast<PPTX::Logic::CubicBezTo*>(oox_path);
	PPTX::Logic::Close*			close		= dynamic_cast<PPTX::Logic::Close*>		(oox_path);
	PPTX::Logic::ArcTo*			arcTo		= dynamic_cast<PPTX::Logic::ArcTo*>		(oox_path);
	PPTX::Logic::QuadBezTo*		quadBezTo	= dynamic_cast<PPTX::Logic::QuadBezTo*>	(oox_path);

	if (moveTo)		convert(moveTo);
	if (lineTo)		convert(lineTo);
	if (cubicBezTo)	convert(cubicBezTo);
	if (quadBezTo)	convert(quadBezTo);
	if (arcTo)		convert(arcTo);
	if (close)		convert(close);


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
		if (oox_bitmap_fill->srcRect.IsInit() && Width > 0  && Height > 0)//часть изображения
		{
			odf_context()->drawing_context()->set_image_client_rect_inch(
				(oox_bitmap_fill->srcRect->l.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->l.get()) : 0 ) /100. * Width / currentSystemDPI,
                (oox_bitmap_fill->srcRect->t.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->t.get()) : 0 ) /100. * Height/ currentSystemDPI,
                (oox_bitmap_fill->srcRect->r.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->r.get()) : 0 ) /100. * Width / currentSystemDPI,
                (oox_bitmap_fill->srcRect->b.IsInit() ? XmlUtils::GetInteger(oox_bitmap_fill->srcRect->b.get()) : 0 ) /100. * Height/ currentSystemDPI);
		}
		if (oox_bitmap_fill->tile.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(2);
			
			if (oox_bitmap_fill->tile->algn.IsInit())
				odf_context()->drawing_context()->set_bitmap_tile_align(oox_bitmap_fill->tile->algn->GetBYTECode());

			if (oox_bitmap_fill->tile->flip.IsInit())	{}

			if (oox_bitmap_fill->tile->sx.IsInit() && Width > 0)	
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_x(*oox_bitmap_fill->tile->sx / 100000. * Width);
			}
			if (oox_bitmap_fill->tile->sy.IsInit()&& Height > 0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_y(*oox_bitmap_fill->tile->sy / 100000. * Height);
			}		
			if (oox_bitmap_fill->tile->tx.IsInit() && Width > 0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_x(*oox_bitmap_fill->tile->tx * 100000. / Width );
			}
			if (oox_bitmap_fill->tile->ty.IsInit() && Height > 0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_y(*oox_bitmap_fill->tile->ty * 100000. / Height );
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
void OoxConverter::convert(PPTX::Logic::GradFill *oox_grad_fill, DWORD nARGB)
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
				odf_context()->drawing_context()->set_gradient_rect( XmlUtils::GetInteger(oox_grad_fill->path->rect->l.get_value_or(L"")) / 1000.,
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->t.get_value_or(L"")) / 1000.,
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->r.get_value_or(L"")) / 1000.,
																	 XmlUtils::GetInteger(oox_grad_fill->path->rect->b.get_value_or(L"")) / 1000.);			
			}
		}	
		odf_context()->drawing_context()->set_gradient_type(grad_style);

		if (oox_grad_fill->GsLst.size() > 1)
		{
			std::wstring hexColorStart, hexColorEnd;
			_CP_OPT(double) opacityStart, opacityEnd;
			
            convert(&oox_grad_fill->GsLst[0].color,hexColorEnd, opacityEnd, nARGB);
            convert(&oox_grad_fill->GsLst[oox_grad_fill->GsLst.size()-1].color,hexColorStart, opacityStart, nARGB);
			
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
						
						if (oox_grad_fill->lin.is_init())
						{
							odf_context()->drawing_context()->set_opacity_angle(oox_grad_fill->lin->ang.get()/60000.);
						}
						else if (oox_grad_fill->path.is_init() && oox_grad_fill->path->rect.is_init())
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

void OoxConverter::convert(PPTX::Logic::UniColor * color, DWORD & nARGB)
{
	if (!color) return;

	smart_ptr<PPTX::Logic::ClrMap>	clrMap(oox_clrMap()); clrMap.AddRef();
	smart_ptr<PPTX::Theme>			theme(oox_theme()); theme.AddRef();
	
    nARGB = color->GetRGBColor(theme, clrMap, nARGB);
}

void OoxConverter::convert(PPTX::Logic::UniColor * color, std::wstring & hexString, _CP_OPT(double) & opacity, DWORD nARGB)
{
	if (!color) return;

    convert(color, nARGB);

	if (nARGB != 0)
	{	
		hexString = XmlUtils::IntToString(nARGB & 0x00FFFFFF, L"#%06X");

		if ((nARGB >> 24) != 0xff)
		{
			opacity = ((nARGB >> 24) /255.) * 100.;
		}
	}
	else 
	{
		//not found in theme
	}
}

void OoxConverter::convert(PPTX::Logic::SolidFill *oox_fill, DWORD nARGB)
{
	if (!oox_fill) return;
		
	std::wstring	hexString;
	_CP_OPT(double) opacity;

    convert(&oox_fill->Color, hexString, opacity, nARGB);

	odf_context()->drawing_context()->set_solid_fill(hexString);

	if (opacity)
	{
		odf_context()->drawing_context()->set_opacity(*opacity);
	}
}
void OoxConverter::convert(PPTX::Logic::PattFill *oox_pattern_fill, DWORD nARGB)
{
	if (!oox_pattern_fill) return;

	odf_context()->drawing_context()->start_hatch_style();
	{
		if (oox_pattern_fill->prst.IsInit())
		{
			odf_context()->drawing_context()->set_hatch_type(oox_pattern_fill->prst->GetBYTECode());
		}			
		if (oox_pattern_fill->fgClr.is_init())
		{
			std::wstring hexColor;
			_CP_OPT(double) opacity;

			convert(&oox_pattern_fill->fgClr, hexColor, opacity, nARGB);

			odf_context()->drawing_context()->set_hatch_line_color(hexColor);
		}
		if (oox_pattern_fill->bgClr.is_init())
		{
			std::wstring hexColor;
			_CP_OPT(double) opacity;

			convert(&oox_pattern_fill->bgClr, hexColor, opacity, nARGB);

			odf_context()->drawing_context()->set_hatch_area_color(hexColor);
		}
	}
	odf_context()->drawing_context()->end_hatch_style();
}

void OoxConverter::convert(PPTX::Logic::Ln *oox_line_prop, DWORD ARGB, PPTX::Logic::ShapeStyle* oox_sp_style)
{
	if (oox_sp_style)
	{
		convert(&oox_sp_style->lnRef, 2);
	}
	
	if (!oox_line_prop) return;

	if (oox_line_prop->Fill.is_init())
	{
		convert (&oox_line_prop->Fill, ARGB);
	}
	if (oox_line_prop->w.IsInit())
	{
		odf_context()->drawing_context()->set_line_width(oox_line_prop->w.get() / 12700.); //pt
	}
	if (oox_line_prop->headEnd.IsInit())
	{
		if (oox_line_prop->headEnd->len.IsInit() || oox_line_prop->headEnd->type.IsInit() || oox_line_prop->headEnd->w.IsInit())
		{
			int type = 1, w = 1, len = 1;//medium arrow
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
			int type = 1, w = 1, len = 1;//medium arrow
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

	if (oox_bodyPr->lIns.IsInit()) lIns = oox_bodyPr->lIns.get() / 12700.; //pt
	if (oox_bodyPr->tIns.IsInit()) tIns = oox_bodyPr->tIns.get() / 12700.;
	if (oox_bodyPr->rIns.IsInit()) rIns = oox_bodyPr->rIns.get() / 12700.;
	if (oox_bodyPr->bIns.IsInit()) bIns = oox_bodyPr->bIns.get() / 12700.;	
		
	odf_context()->drawing_context()->set_textarea_padding (lIns, tIns, rIns, bIns);

	if (oox_bodyPr->wrap.IsInit())
		odf_context()->drawing_context()->set_textarea_wrap(oox_bodyPr->wrap->GetBYTECode());

	if ((oox_bodyPr->numCol.IsInit()) && (oox_bodyPr->numCol.get() > 1))
	{
		//+ style section
		//+element text:section в котором параграфы
	}
	if (oox_bodyPr->rot.IsInit())
	{
		odf_context()->drawing_context()->set_textarea_rotation(oox_bodyPr->rot.get() / 60000);
	}

	switch(oox_bodyPr->Fit.type)
	{
		case  PPTX::Logic::TextFit::FitSpAuto:
		{//изменяемы размеры			
			odf_context()->drawing_context()->set_text_box_min_size(true);//уже выставленые в min
		}break;
		case  PPTX::Logic::TextFit::FitNo:
		{
		}break;
		case  PPTX::Logic::TextFit::FitNormAuto:
		{
		}break;
		default:
		{
		}
	}

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
		odf_context()->drawing_context()->start_action(oox_cnvPr->hlinkClick->action.get_value_or(L""));

			if (oox_cnvPr->hlinkClick->snd.IsInit())
			{
				std::wstring sound = find_link_by_id(oox_cnvPr->hlinkClick->snd->embed.get(), 3);

				std::wstring href = odf_context()->add_media(sound);				
				odf_context()->drawing_context()->add_sound(href);	
			}
			if (oox_cnvPr->hlinkClick->id.IsInit())
			{
				std::wstring hlink = find_link_by_id(oox_cnvPr->hlinkClick->id.get(), 2);
				
				odf_context()->drawing_context()->add_link(hlink);	
			}
		odf_context()->drawing_context()->end_action();
	}
	//nullable_string		title;
	//nullable<Hyperlink>	hlinkHover;
}
void OoxConverter::convert(PPTX::Logic::CNvSpPr *oox_cnvSpPr)
{
	if (!oox_cnvSpPr) return;

}
void OoxConverter::convert(PPTX::Logic::CNvCxnSpPr *oox_cnvSpPr)
{
	if (!oox_cnvSpPr) return;

}
void OoxConverter::convert(PPTX::Logic::NvCxnSpPr *oox_nvSpPr)
{
	if (!oox_nvSpPr) return;

	convert(&oox_nvSpPr->cNvPr);
	convert(&oox_nvSpPr->cNvCxnSpPr);
	convert(&oox_nvSpPr->nvPr);
}

void OoxConverter::convert(PPTX::Logic::NvPr *oox_nvPr)
{
	if (!oox_nvPr) return;
	


//ph уровнем выше

}

void OoxConverter::convert_list_level(PPTX::Logic::TextParagraphPr	*oox_para_props, int level)
{//одномерные списки тока
	//if (!oox_list)return;

	if (!oox_para_props) return;

	PPTX::Theme *theme = oox_theme();

	PPTX::Logic::Bullet & bullet = oox_para_props->ParagraphBullet;

	if (bullet.is<PPTX::Logic::BuNone>())return;

	int odf_list_type = 13; // 1, 2, 3 ...

	if (bullet.is<PPTX::Logic::BuAutoNum>())
	{
		const PPTX::Logic::BuAutoNum & buAutoNum = bullet.as<PPTX::Logic::BuAutoNum>();
		int pptx_type = buAutoNum.type.GetBYTECode();

		if (				  pptx_type < 3) odf_list_type = 46;
		if (pptx_type > 2  && pptx_type < 6) odf_list_type = 60;
		if (pptx_type > 28 && pptx_type < 32) odf_list_type = 47;
		if (pptx_type > 31 && pptx_type < 35) odf_list_type = 61;

		odf_context()->styles_context()->lists_styles().start_style_level(level, odf_list_type); 

		if (buAutoNum.startAt.IsInit())
		{
			int start_value = *buAutoNum.startAt;
			if (start_value > 1)
				odf_context()->styles_context()->lists_styles().set_start_number(start_value);
		}
	}
	if (bullet.is<PPTX::Logic::BuChar>())
	{
		odf_list_type = 5;

		const PPTX::Logic::BuChar & buChar = bullet.as<PPTX::Logic::BuChar>();
		odf_context()->styles_context()->lists_styles().start_style_level(level, odf_list_type);
	
		odf_context()->styles_context()->lists_styles().set_bullet_char(buChar.Char);
	}
	if (bullet.is<PPTX::Logic::BuBlip>())
	{
		const PPTX::Logic::BuBlip & buBlip = bullet.as<PPTX::Logic::BuBlip>();
		
		std::wstring odf_ref;
		bool bEmbedded = true;
			
		if (buBlip.blip.embed.IsInit())
		{
			std::wstring sID = buBlip.blip.embed->get();
			std::wstring pathImage = find_link_by_id(sID, 1);

			if (pathImage.empty())
				pathImage = buBlip.blip.GetFullPicName(); // only for presentation merge shapes !!
			
			odf_ref = odf_context()->add_image(pathImage);
		}
		else if (buBlip.blip.link.IsInit())
		{
			odf_ref = buBlip.blip.link->get();	
			bEmbedded = false;
		}
		
		if (!odf_ref.empty())
		{
			odf_list_type = 1000;
			odf_context()->styles_context()->lists_styles().start_style_level(level, odf_list_type );
			odf_context()->styles_context()->lists_styles().set_bullet_image(odf_ref);
		}
		else
		{
			odf_list_type = 5;
			odf_context()->styles_context()->lists_styles().start_style_level(level, odf_list_type );
			odf_context()->styles_context()->lists_styles().set_bullet_char(L"\x2022");
		}
	}

	//odf_writer::style_list_level_label_alignment	* aligment_props	= odf_context()->styles_context()->lists_styles().get_list_level_alignment_properties();
	odf_writer::style_list_level_properties			* level_props		= odf_context()->styles_context()->lists_styles().get_list_level_properties();
	odf_writer::style_text_properties				* text_properties	= odf_context()->styles_context()->lists_styles().get_text_properties();
	
	convert(oox_para_props->defRPr.GetPointer(), text_properties);

	if (oox_para_props->indent.IsInit() && level_props)
	{
		level_props->text_min_label_width_	= odf_types::length(- oox_para_props->indent.get() / 12700., odf_types::length::pt);
		level_props->text_space_before_		= odf_types::length(1, odf_types::length::pt);
			
	}else
	{
		//aligment_props->fo_text_indent_ = odf_types::length(0, odf_types::length::cm);
		//aligment_props->fo_margin_left_ = odf_types::length(0, odf_types::length::cm);
	}

	if (oox_para_props->buColor.is<PPTX::Logic::BuClrTx>())
	{
		const PPTX::Logic::BuClrTx & buClrTx = oox_para_props->buColor.as<PPTX::Logic::BuClrTx>();
	}
	if (oox_para_props->buColor.is<PPTX::Logic::BuClr>())
	{
		PPTX::Logic::BuClr & buClr = oox_para_props->buColor.as<PPTX::Logic::BuClr>();

		std::wstring	hexColor;
		_CP_OPT(double) opacity;
		
		convert(&buClr.Color, hexColor, opacity);
		if (!hexColor.empty())
		{		
			int res = 0;
			if ((res = hexColor.find(L"#")) < 0) hexColor = std::wstring(L"#") + hexColor;
			if (text_properties) text_properties->content_.fo_color_ = odf_types::color(hexColor);
		}
	}
//-----------------------------------
	if (oox_para_props->buSize.is<PPTX::Logic::BuSzPts>())
	{
		const PPTX::Logic::BuSzPts & buSzPts = oox_para_props->buSize.as<PPTX::Logic::BuSzPts>();
		if (buSzPts.val.IsInit())
		{
			if (text_properties)
				text_properties->content_.fo_font_size_ = odf_types::length(*buSzPts.val, odf_types::length::pt);
			else if (odf_list_type == 1000)
				odf_context()->styles_context()->lists_styles().set_bullet_image_size(*buSzPts.val);
		}
	}
	else
	{
		double size_pt = oox_para_props->defRPr.IsInit() ? (oox_para_props->defRPr->sz.IsInit() ? *oox_para_props->defRPr->sz /100. : 0) : 0;
		
		if (size_pt < 0.001 && odf_list_type == 1000)
		{
			odf_writer::style_text_properties * text_props = odf_context()->styles_context()->last_state(odf_types::style_family::Paragraph)->get_text_properties();
			if (text_props && text_props->content_.fo_font_size_)
			{
				size_pt = text_props->content_.fo_font_size_->get_length().get_value_unit(odf_types::length::pt);
			}		
			if (size_pt < 0.001)
			{
				odf_writer::odf_style_state_ptr style_state;
				odf_context()->styles_context()->find_odf_default_style_state(odf_types::style_family::Text, style_state);
				if (style_state)
					text_props = style_state->get_text_properties();

				if (text_props && text_props->content_.fo_font_size_)
				{
					size_pt = text_props->content_.fo_font_size_->get_length().get_value_unit(odf_types::length::pt);
				}	
			}
		}		
		
		if (oox_para_props->buSize.is<PPTX::Logic::BuSzTx>())
		{
			//equal tet size
		}
		
		if (oox_para_props->buSize.is<PPTX::Logic::BuSzPct>())
		{
			const PPTX::Logic::BuSzPct & buSzPct = oox_para_props->buSize.as<PPTX::Logic::BuSzPct>();
			if (buSzPct.val.IsInit())
			{
				if (text_properties)
					text_properties->content_.fo_font_size_ = odf_types::percent(*buSzPct.val / 1000.);

				size_pt *= *buSzPct.val / 100000.;
			}
		}
		if (text_properties && !text_properties->content_.fo_font_size_)
			text_properties->content_.fo_font_size_ = odf_types::percent(100.);	
		else if (odf_list_type == 1000)
		{
			odf_context()->styles_context()->lists_styles().set_bullet_image_size(size_pt);
		}
	}
//----------------	
	if (oox_para_props->buTypeface.is<PPTX::Logic::BuFontTx>())
	{
		const PPTX::Logic::BuFontTx & buFontTx = oox_para_props->buTypeface.as<PPTX::Logic::BuFontTx>();
	}
	if (oox_para_props->buTypeface.is<PPTX::Logic::TextFont>())
	{
		const PPTX::Logic::TextFont & textFont = oox_para_props->buTypeface.as<PPTX::Logic::TextFont>();

		std::wstring font = textFont.typeface;
		convert_font(theme, font);
		if (!font.empty() && text_properties)
			text_properties->content_.fo_font_family_ = font;	
	}

	odf_context()->styles_context()->lists_styles().end_style_level();
}

void OoxConverter::convert(PPTX::Logic::Paragraph *oox_paragraph, PPTX::Logic::TextListStyle *oox_list_style)
{
	if (!oox_paragraph)return;

	bool		 styled			= false;

	bool		 list_local		= false;
	bool		 list_present	= false;
	std::wstring list_style_name;

	int			 list_level		= 0;//-1;
	
	NSCommon::nullable<PPTX::Logic::TextParagraphPr> paraPr;

	if (oox_paragraph->pPr.IsInit())
	{
		if (oox_paragraph->pPr->lvl.IsInit())
			list_level = *oox_paragraph->pPr->lvl;	
		if (oox_paragraph->pPr->ParagraphBullet.is_init())
			list_local = true;	
	}

	if (oox_list_style)
	{
		int i = (list_level >= 0 && list_level < 10) ? list_level : 0;

		if (oox_list_style->levels[i].IsInit())
			oox_list_style->levels[i]->Merge(paraPr);
	}
	if (oox_paragraph->pPr.IsInit())
		oox_paragraph->pPr->Merge(paraPr);

	if (paraPr.IsInit())
	{
		list_level = paraPr->lvl.IsInit() ? *paraPr->lvl : 0;

		if (list_level > 0) list_present = true;
		if (paraPr->ParagraphBullet.is_init())
		{			
			list_present = true;		
			if (paraPr->ParagraphBullet.is<PPTX::Logic::BuNone>())
				list_present = false;
		}
														//свойства могут быть приписаны не только к параграфу, но и к самому объекту		
		odf_writer::style_paragraph_properties* paragraph_properties = odf_context()->text_context()->get_paragraph_properties();
		odf_writer::style_text_properties*		text_properties = odf_context()->text_context()->get_text_properties();
		
		if (!paragraph_properties)
		{
			odf_context()->text_context()->get_styles_context()->create_style(L"", odf_types::style_family::Paragraph, true, false, -1);	
			
			paragraph_properties	= odf_context()->text_context()->get_styles_context()->last_state()->get_paragraph_properties();
			text_properties			= odf_context()->text_context()->get_styles_context()->last_state()->get_text_properties();

			styled = true;
		}
		convert(paraPr.GetPointer(), paragraph_properties, text_properties);

		if (odf_context()->drawing_context()->is_wordart())
			odf_context()->drawing_context()->set_paragraph_properties(paragraph_properties);
	}	

	if (oox_paragraph->RunElems.empty() && list_present) list_present = false; // ms не обозначает присутствие списка, libra - показывает значек
	
	while (odf_context()->text_context()->list_state_.levels.size() > list_level)
	{
		odf_context()->text_context()->end_list();
	}	
	
	if(list_present)
	{
		if (odf_context()->text_context()->list_state_.levels.empty())
		{
			odf_context()->text_context()->list_state_.started_list = false;
			odf_context()->text_context()->list_state_.style_name	= L"";
		}

		if (list_local)
		{
			_CP_OPT(int) inStyles = odf_context()->drawing_context()->get_presentation();

			odf_context()->styles_context()->lists_styles().start_style(inStyles && *inStyles > 0);
				convert_list_level(oox_paragraph->pPr.GetPointer(), list_level /*- 1*/);
			odf_context()->styles_context()->lists_styles().end_style();
	
		}		
		list_style_name = odf_context()->styles_context()->lists_styles().get_style_name(); //last added

		list_level++;

		if (odf_context()->text_context()->list_state_.levels.size() < list_level)
		{
			while (odf_context()->text_context()->list_state_.levels.size() < list_level)
			{
				odf_context()->text_context()->start_list(list_style_name);
				odf_context()->text_context()->start_list_item();
				
				if (odf_context()->text_context()->list_state_.style_name == list_style_name)
					list_style_name = L"";
			}
		}
		else
			odf_context()->text_context()->start_list_item();
	}
	else if (odf_context()->text_context()->list_state_.started_list == true)
	{
		odf_context()->text_context()->end_list();
	}
	odf_context()->text_context()->start_paragraph(styled);

	for (size_t i=0; i< oox_paragraph->RunElems.size(); i++)
	{
		convert(&oox_paragraph->RunElems[i].as<OOX::WritingElement>());
	}
	odf_context()->text_context()->end_paragraph();

	//if(list_present)
	//{
	//	odf_context()->text_context()->end_list_item();
	//}
}
void OoxConverter::convert(PPTX::Logic::TextListStyle *oox_list_style, int level, odf_writer::style_paragraph_properties * paragraph_properties
																				, odf_writer::style_text_properties * text_properties)
{
	if (!oox_list_style)		return;
	if (!paragraph_properties)	return;

	convert(oox_list_style->levels[9].GetPointer(), paragraph_properties, text_properties);
	if (level < 0 || level > 9) return;

	convert(oox_list_style->levels[level].GetPointer(), paragraph_properties, text_properties);
}
void OoxConverter::convert(PPTX::Logic::TextParagraphPr *oox_paragraph_pr	, odf_writer::style_paragraph_properties * paragraph_properties
																			, odf_writer::style_text_properties * text_properties)
{
	if (!oox_paragraph_pr) return;
	
	if (paragraph_properties)
	{
		if (oox_paragraph_pr->lnSpc.IsInit())
		{
			odf_types::length_or_percent length;
			convert(oox_paragraph_pr->lnSpc.GetPointer(), length);
			paragraph_properties->content_.fo_line_height_ = odf_types::line_width(length);
		}
		if (oox_paragraph_pr->spcAft.IsInit())
		{
 			odf_types::length_or_percent length;
			convert(oox_paragraph_pr->spcAft.GetPointer(), length);
			paragraph_properties->content_.fo_margin_bottom_ = length;
		}
		if (oox_paragraph_pr->spcBef.IsInit())
		{
 			odf_types::length_or_percent length;
			convert(oox_paragraph_pr->spcBef.GetPointer(), length);
			paragraph_properties->content_.fo_margin_top_ = length;
		}
		if (oox_paragraph_pr->algn.IsInit())
		{
			switch(oox_paragraph_pr->algn->GetBYTECode())
			{
				case 0 : paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Center);	break;
				case 1 : 
				case 2 : 
				case 3 : paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify);	break;
				case 5 : paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Right);	break;
				case 4 : 
				default:
					paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Left);			break;
			}
		}
		if (oox_paragraph_pr->marL.IsInit())
		{
			paragraph_properties->content_.fo_margin_left_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->marL.get() / 12700., odf_types::length::pt));
		}
		if (oox_paragraph_pr->marR.IsInit())
		{
			paragraph_properties->content_.fo_margin_right_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->marR.get() / 12700., odf_types::length::pt));
		}
		if (oox_paragraph_pr->indent.IsInit())
		{
			paragraph_properties->content_.fo_text_indent_ = odf_types::length_or_percent(odf_types::length(oox_paragraph_pr->indent.get() / 12700., odf_types::length::pt));
		}
		if (oox_paragraph_pr->rtl.IsInit() && *oox_paragraph_pr->rtl)
		{
			paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::RlTb);
		}	
	}
	//defTabSz;
	//fontAlgn;

	if (oox_paragraph_pr->defRPr.IsInit())
	{
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
		odf_types::length length = odf_types::length(-oox_spacing->GetVal() / 100., odf_types::length::pt);
		length_or_percent = odf_types::length_or_percent(length);
	}
}

void OoxConverter::convert(PPTX::Logic::RunProperties *oox_run_pr, odf_writer::style_text_properties * text_properties)
{
	if (!oox_run_pr)		return;
	if (!text_properties)	return;
	
	PPTX::Theme *theme = oox_theme();

//------------------------------------------------------	
	odf_writer::odf_drawing_context *drawing = odf_context()->drawing_context(); 
	if (drawing)	//from styles drawing impossible(  ... todoooo ??? 
	{	
		if ((oox_run_pr->Fill.is<PPTX::Logic::GradFill>()) ||
			((oox_run_pr->ln.IsInit()) && (oox_run_pr->ln->Fill.is_init() && oox_run_pr->ln->Fill.getType() != OOX::et_a_noFill)))
		{
			drawing->change_text_box_2_wordart();
		}

		if (drawing->is_wordart())
		{
			if (oox_run_pr->Fill.is_init())
			{
				drawing->start_area_properties();
					convert(&oox_run_pr->Fill);
				drawing->end_area_properties();
			}
			if (oox_run_pr->ln.is_init())
			{
				drawing->start_line_properties();
				if (oox_run_pr->ln->Fill.is_init() && oox_run_pr->ln->Fill.getType() != OOX::et_a_noFill )
				{
					drawing->set_line_dash_preset(6);
					convert(oox_run_pr->ln.GetPointer());
				}
				else if (drawing->is_wordart())
					drawing->set_no_fill();
				drawing->end_line_properties();
			}
		}
	}
//---------------------------------------
	_CP_OPT(double) opacityText;
	std::wstring	hexColorText;
			
	PPTX::Logic::GradFill*	gradFill	= NULL;
	PPTX::Logic::SolidFill*	solidFill	= NULL;

	if (oox_run_pr->Fill.is<PPTX::Logic::GradFill>())
	{
		gradFill = &oox_run_pr->Fill.as<PPTX::Logic::GradFill>();
	}
	if (gradFill && !gradFill->GsLst.empty())
	{
		convert(&gradFill->GsLst[0].color, hexColorText, opacityText);
	}
	if (oox_run_pr->Fill.is<PPTX::Logic::SolidFill>())
	{
		solidFill = &oox_run_pr->Fill.as<PPTX::Logic::SolidFill>();
	}
	if (solidFill)
	{
		convert(&solidFill->Color, hexColorText, opacityText);
	}	
	if (!hexColorText.empty())
	{		
		int res = 0;
		if ((res = hexColorText.find(L"#")) < 0) hexColorText = std::wstring(L"#") + hexColorText;
		text_properties->content_.fo_color_ = odf_types::color(hexColorText);
	}
//---------------------------------------
	if (oox_run_pr->b.IsInit())
	{
		if (oox_run_pr->b.get() == true) 
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		else
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);
	}

	if (oox_run_pr->i.IsInit())
	{
		if (oox_run_pr->i.get() ==true)
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
		else
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	}
	if (oox_run_pr->sz.IsInit())
	{
		convert(oox_run_pr->sz.get()/100., text_properties->content_.fo_font_size_);
		convert(oox_run_pr->sz.get()/100., text_properties->content_.style_font_size_asian_);
		convert(oox_run_pr->sz.get()/100., text_properties->content_.style_font_size_complex_);
	}
	if (oox_run_pr->latin.IsInit())
	{
		if (!oox_run_pr->latin->typeface.empty())	
		{
			std::wstring font = oox_run_pr->latin->typeface;
			convert_font(theme, font);
							
			if (!font.empty())
				text_properties->content_.fo_font_family_ = font;
		}
		else
		{
			text_properties->content_.fo_font_family_ = L"Calibri";//default_font; ???? 
		}
	}
	if (oox_run_pr->ea.IsInit())
	{
		if (!oox_run_pr->ea->typeface.empty())	
		{
			std::wstring font = oox_run_pr->ea->typeface;
			convert_font(theme, font);
			
			if (!font.empty())
				text_properties->content_.style_font_family_asian_ = font;
		}
		else
		{
		}
	}
	if (oox_run_pr->cs.IsInit())
	{
		if (!oox_run_pr->cs->typeface.empty())
		{
			std::wstring font = oox_run_pr->cs->typeface;
			convert_font(theme, font);
			
			if (!font.empty())
				text_properties->content_.style_font_family_complex_ = font;
		}
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
            oox_country = oox_language.substr(res + 1);
            oox_language = oox_language.substr(0, res);
		}
		
		text_properties->content_.fo_language_ = oox_language;
        if (!oox_country.empty())
			text_properties->content_.fo_country_ = oox_country;

	}
	if (oox_run_pr->spc.IsInit())
	{
		text_properties->content_.fo_letter_spacing_ = odf_types::letter_spacing(odf_types::length(oox_run_pr->spc.get()/100., odf_types::length::pt));
	}
	if (oox_run_pr->u.IsInit())
	{
		text_properties->content_.style_text_underline_style_	= odf_types::line_style	(odf_types::line_style::Solid);
		text_properties->content_.style_text_underline_type_	= odf_types::line_type	(odf_types::line_type::Single);

		switch(oox_run_pr->u->GetBYTECode())
		{
			case 12:
				text_properties->content_.style_text_underline_style_	= boost::none;
				text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::None);break;
			case 0:
			case 1:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Dash);break;
			case 5:
			case 6:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::DotDash);break;
			case 2:
			case 3:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::LongDash);break;
			case 7:
			case 8:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::DotDotDash);break;
			case 9:
			case 10:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Dotted);break;
			case 14:
			case 16:
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Wave);break;
			case 4:
				text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::Double);break;
			case 15:
				text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::Double);
				text_properties->content_.style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Wave);break;
		}
		PPTX::Logic::UniFill *fill = NULL;
		if (oox_run_pr->uFillTx.IsInit())		fill = &oox_run_pr->uFillTx->Fill;
		else if (oox_run_pr->uFill.IsInit())	fill = &oox_run_pr->uFill->Fill;
		if (fill && fill->is<PPTX::Logic::SolidFill>() )
		{
			PPTX::Logic::SolidFill &solid = fill->as<PPTX::Logic::SolidFill>();

			_CP_OPT(double) opacityText;
			std::wstring	hexColorText;
			convert(&solid.Color, hexColorText, opacityText);
			if (!hexColorText.empty())
			{
				text_properties->content_.style_text_underline_color_ = odf_types::color(hexColorText);
			}
		}
	}
	if (oox_run_pr->highlight.IsInit())
	{
		_CP_OPT(double) opacityText;
		std::wstring	hexColorText;
		convert(&oox_run_pr->highlight->Color, hexColorText, opacityText);
		
		text_properties->content_.fo_background_color_ = odf_types::color(hexColorText);
	}

	if (oox_run_pr->strike.IsInit())
	{
		switch(oox_run_pr->strike->GetBYTECode())
		{
		case 0:
			text_properties->content_.style_text_line_through_style_	= odf_types::line_style	(odf_types::line_style::Solid);
			text_properties->content_.style_text_line_through_type_		= odf_types::line_type(odf_types::line_type::Double);	break;
		case 1:
			text_properties->content_.style_text_line_through_style_	= boost::none;
			text_properties->content_.style_text_line_through_type_		= odf_types::line_type(odf_types::line_type::None);		break;
		case 2:
			text_properties->content_.style_text_line_through_style_	= odf_types::line_style	(odf_types::line_style::Solid);
			text_properties->content_.style_text_line_through_type_		= odf_types::line_type(odf_types::line_type::Single);	break;
		}
	}

	if (oox_run_pr->baseline.IsInit())
	{
		text_properties->content_.style_text_position_ = odf_types::text_position(*oox_run_pr->baseline / 1000.); 
	}
	if (oox_run_pr->cap.IsInit())
	{
		switch(oox_run_pr->cap->GetBYTECode())
		{
		case 0:
			text_properties->content_.fo_text_transform_ = odf_types::text_transform(odf_types::text_transform::Uppercase);	break;
		case 1:
			text_properties->content_.fo_font_variant_ = odf_types::font_variant(odf_types::font_variant::SmallCaps);		break;
		case 2:
			text_properties->content_.fo_font_variant_ = odf_types::font_variant(odf_types::font_variant::Normal);			break;
		}
		
	}
	if (oox_run_pr->normalizeH.IsInit() && *oox_run_pr->normalizeH)
	{
		text_properties->content_.fo_text_transform_ = odf_types::text_transform(odf_types::text_transform::Capitalize);
	}

}
void OoxConverter::convert(PPTX::Logic::Run *oox_run)
{
	if (!oox_run) return;
	bool styled = false;

	odf_writer::odf_text_context* text_context = odf_context()->text_context();

	if (oox_run->rPr.IsInit())
	{
		odf_writer::style_text_properties * text_properties = text_context->get_text_properties();
		
		if (!text_properties)
		{
			text_context->get_styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);	
			text_properties = text_context->get_styles_context()->last_state()->get_text_properties();
			styled = true;
		}
		convert(oox_run->rPr.GetPointer(), text_properties);
	}
	
	text_context->start_span(styled);	

	if ((oox_run->rPr.IsInit()) && (oox_run->rPr->hlinkClick.IsInit()) && (oox_run->rPr->hlinkClick->id.IsInit()))
	{
		odf_writer::style_text_properties * text_properties = text_context->get_text_properties();
		
		if (!text_properties->content_.fo_color_)
		{
			PPTX::Logic::UniColor colorLink;
			colorLink.Color.reset(new PPTX::Logic::SchemeClr());

			PPTX::Logic::SchemeClr & clr = colorLink.as<PPTX::Logic::SchemeClr>();
			clr.val.set(L"hlink");

			std::wstring	strHexColor;
			_CP_OPT(double) opacity;
			convert(&colorLink, strHexColor, opacity);
			if (!strHexColor.empty())
				text_properties->content_.fo_color_ = strHexColor;
		}
		text_properties->content_.style_text_underline_type_	= odf_types::line_type::Single;
		text_properties->content_.style_text_underline_style_	= odf_types::line_style::Solid;
		
		std::wstring hlink = find_link_by_id(oox_run->rPr->hlinkClick->id.get(), 2);
		text_context->add_hyperlink(hlink, oox_run->GetText());
	}
	else
	{
		text_context->add_text_content( oox_run->GetText());
	}
	text_context->end_span();
}
void OoxConverter::convert(PPTX::Logic::Fld *oox_fld)
{
	if (!oox_fld) return;

	bool styled = false;

	if (oox_fld->rPr.IsInit())
	{
		odf_writer::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		
		if (!text_properties)
		{
			odf_context()->styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);	
			text_properties = odf_context()->styles_context()->last_state()->get_text_properties();
			styled = true;
		}
		convert(oox_fld->rPr.GetPointer(), text_properties);
	}
	
	odf_context()->text_context()->start_span(styled);	

	std::wstring fld_type = oox_fld->type.get_value_or(L"");

	if ((oox_fld->rPr.IsInit()) && (oox_fld->rPr->hlinkClick.IsInit()) && (oox_fld->rPr->hlinkClick->id.IsInit()))
	{
		std::wstring hlink = find_link_by_id(oox_fld->rPr->hlinkClick->id.get(), 2);
		odf_context()->text_context()->add_hyperlink(hlink, oox_fld->GetText());

	}
	else if (fld_type == L"slidenum")
	{
		odf_context()->text_context()->add_text_page_number( oox_fld->GetText());
	}
	else if (fld_type == L"datetime1")
	{
		odf_context()->text_context()->add_text_date( oox_fld->GetText());
	}
	else
	{
		odf_context()->text_context()->add_text_content( oox_fld->GetText());
	}
	odf_context()->text_context()->end_span();
}
void OoxConverter::convert(PPTX::Logic::Br *oox_br)
{
	if (!oox_br) return;
	odf_context()->text_context()->set_type_break(2, true);
}
void OoxConverter::convert(PPTX::Logic::MathParaWrapper *oox_math)
{
	if (!oox_math) return;
}
void OoxConverter::convert(PPTX::Logic::LineTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm =	oox_geom_path->x + L" " + oox_geom_path->y;
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"L"), path_elm);
}
void OoxConverter::convert(PPTX::Logic::MoveTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm =	oox_geom_path->x + L" " + oox_geom_path->y;

	odf_context()->drawing_context()->add_path_element(std::wstring(L"M"), path_elm);
}
void OoxConverter::convert(PPTX::Logic::TextListStyle *oox_list_style)
{
	if (!oox_list_style) return;
	if (oox_list_style->IsListStyleEmpty()) return;

	_CP_OPT(int) inStyles = odf_context()->drawing_context()->get_presentation();

	odf_context()->styles_context()->lists_styles().start_style(inStyles && *inStyles > 0); // masters 
	for (int i = 0; i < 9; i++)
	{
		OoxConverter::convert_list_level(oox_list_style->levels[i].GetPointer(), i);
	}
	odf_context()->styles_context()->lists_styles().end_style();
}
void OoxConverter::convert(PPTX::Logic::TxBody *oox_txBody, PPTX::Logic::ShapeStyle* oox_style)
{
	if (!oox_txBody) return;
	if (oox_txBody->Paragrs.empty()) return;

	odf_context()->start_text_context();	

	convert(oox_txBody->lstStyle.GetPointer());

	for (size_t i = 0; i < oox_txBody->Paragrs.size(); i++)
	{
		convert(&oox_txBody->Paragrs[i], oox_txBody->lstStyle.GetPointer());
	
	//внешние настройки для текста
		convert(oox_txBody->bodyPr.GetPointer());			
		
		if (oox_style)
		{
			convert(&oox_style->fontRef);
		}		
	}
	odf_context()->drawing_context()->set_text( odf_context()->text_context());
	
	odf_context()->end_text_context();	
}
void OoxConverter::convert(PPTX::Logic::ArcTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	int stAng = XmlUtils::GetInteger(oox_geom_path->stAng);
	int swAng = XmlUtils::GetInteger(oox_geom_path->swAng);
	
	std::wstring path_elm = oox_geom_path->wR + L" " + oox_geom_path->hR + L" " + std::to_wstring(stAng/60000) + L" " + std::to_wstring(swAng /60000);
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"G"), path_elm);
}
void OoxConverter::convert(PPTX::Logic::QuadBezTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm = oox_geom_path->x[0] + L" " + oox_geom_path->y[0] + L" " + oox_geom_path->x[1] + L" " + oox_geom_path->y[1];
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"S"), path_elm);

}
void OoxConverter::convert(PPTX::Logic::CubicBezTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm =	oox_geom_path->x[0] + L" " + oox_geom_path->y[0] + L" " + 
							oox_geom_path->x[1] + L" " + oox_geom_path->y[1] + L" " +
							oox_geom_path->x[2] + L" " + oox_geom_path->y[2];
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"C"), path_elm);
}
void OoxConverter::convert(PPTX::Logic::Close *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm ;	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"Z"), path_elm);
}
void OoxConverter::convert(PPTX::Logic::StyleRef *style_ref, int type)
{
	if (!style_ref) return;

	DWORD nARGB = 0;
	convert(&style_ref->Color, nARGB);

	if (style_ref->idx.IsInit() == false)
	{
		std::wstring hexColor =  XmlUtils::IntToString(nARGB & 0x00FFFFFF, L"#%06X");
		_CP_OPT(double) opacity;

		if ((nARGB >> 24) != 0xff)
		{
			opacity = ((nARGB >> 24) /255.) * 100.;
		}
		
		if (type != 3) //?? todooo
		{
			odf_context()->drawing_context()->set_solid_fill(hexColor);
		
			if (opacity)
				odf_context()->drawing_context()->set_opacity(*opacity);
		}
		return;
	}

	int index = *style_ref->idx;

	PPTX::Theme *theme = oox_theme();
	if (!theme) return;
		
	OOX::IFileContainer* old = oox_current_child_document;
	oox_current_child_document = theme;

	if (type == 1)
	{
		PPTX::Logic::UniFill *fill = NULL;
		if (index < 1000)
		{
			index -= 1;
			if ((index >= 0) || (index < theme->themeElements.fmtScheme.fillStyleLst.size()))
			{
				fill = &theme->themeElements.fmtScheme.fillStyleLst[index];		
			}
		}
		else if (index > 1000)
		{
			index -= 1001;
			if ((index >= 0) || (index < theme->themeElements.fmtScheme.bgFillStyleLst.size()))
			{
				fill = &theme->themeElements.fmtScheme.bgFillStyleLst[index];		
			}
		}
		
		convert(fill, nARGB);
	}
	else if (type == 2)
	{
		index -= 1;
		if ((index >= 0) || (index < theme->themeElements.fmtScheme.lnStyleLst.size()))
		{
			convert(&theme->themeElements.fmtScheme.lnStyleLst[index], nARGB);		
		}
	}
	else if (type == 3)
	{
		index -= 1;
		if ((index >= 0) || (index < theme->themeElements.fmtScheme.effectStyleLst.size()))
		{
			convert(&theme->themeElements.fmtScheme.effectStyleLst[index]);		
		}
	}
	oox_current_child_document = old;
}
void OoxConverter::convert(PPTX::Logic::FontRef *style_font_ref)
{
	if (!style_font_ref) return;

	PPTX::Theme *theme = oox_theme();
	if (!theme) return;

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	convert(&style_font_ref->Color, hexColor,  opacity);	
	odf_context()->drawing_context()->set_textarea_fontcolor(hexColor);
	
	if (style_font_ref->idx.IsInit())
	{	
		PPTX::Logic::FontCollection *style_font = NULL;
		if (style_font_ref->idx->GetBYTECode() == SimpleTypes::fontcollectionindexMajor)
		{
			style_font = &theme->themeElements.fontScheme.majorFont;
		}
		if (style_font_ref->idx->GetBYTECode() == SimpleTypes::fontcollectionindexMinor)
		{
			style_font = &theme->themeElements.fontScheme.minorFont;
		}
		if (style_font)
		{
			std::wstring latin	= style_font->latin.typeface;
			std::wstring ea		= style_font->ea.typeface;
			std::wstring cs		= style_font->cs.typeface;

			//theme->themeElements.fontScheme

			odf_context()->drawing_context()->set_textarea_font(latin, ea, cs);
		}
	}
}

}
