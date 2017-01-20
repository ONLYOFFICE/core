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
#include "../../../Common/DocxFormat/Source/Common/SimpleTypes_Drawing.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../OdfFormat/odf_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

using namespace cpdoccore;

namespace Oox2Odf
{
	double Emu2Pt(int emu)
	{
		return (1.0 * emu / (635 * 20.0));
	}
void OoxConverter::convert(PPTX::Logic::SpTree *oox_shape_tree)
{
	if (oox_shape_tree == NULL) return;
	
	odf_context()->drawing_context()->start_group();

	cpdoccore::_CP_OPT(double) x, y , ch_x , ch_y ;
	odf_context()->drawing_context()->set_group_position(x, y, ch_x, ch_y);

	for (long i=0 ;i < oox_shape_tree->SpTreeElems.size(); i++)
	{
		convert(oox_shape_tree->SpTreeElems[i].GetElem().operator->());
	}
	odf_context()->drawing_context()->end_group();
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

		if (type == 2000 && oox_shape->txBody->bodyPr.fromWordArt.get_value_or(false))
		{
			int wordart_type = convert(oox_shape->txBody->bodyPr.prstTxWarp.GetPointer());

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
				
			for (long i=0 ; i < oox_shape->txBody->Paragrs.size();i++)
			{
				convert(&oox_shape->txBody->Paragrs[i]);
			}
			odf_context()->drawing_context()->set_text( odf_context()->text_context());
			
		//наложим внешние настройки для текста
			convert(&oox_shape->txBody->bodyPr);			
			
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

	bool use_fill_from_style = false;
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
		PPTX::Logic::NoFill*	noFill		= &oox_spPr->Fill.as<PPTX::Logic::NoFill>();
		PPTX::Logic::BlipFill*	blipFill	= &oox_spPr->Fill.as<PPTX::Logic::BlipFill>();
		PPTX::Logic::GradFill*	gradFill	= &oox_spPr->Fill.as<PPTX::Logic::GradFill>();
		PPTX::Logic::SolidFill*	solidFill	= &oox_spPr->Fill.as<PPTX::Logic::SolidFill>();
		PPTX::Logic::PattFill*	pattFill	= &oox_spPr->Fill.as<PPTX::Logic::PattFill>();
		
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
int OoxConverter::convert(PPTX::Logic::PrstTxWarp *oox_text_preset)
{
	if (oox_text_preset == NULL) return -1;
	if (oox_text_preset->prst.GetBYTECode() ==  SimpleTypes::textshapetypeTextNoShape) return 2000;

	return 2001 + oox_text_preset->prst.GetBYTECode();
}
}
