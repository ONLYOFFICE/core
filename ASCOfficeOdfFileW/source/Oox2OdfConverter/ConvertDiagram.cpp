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
#include "Converter.h"
#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"

#include "../OdfFormat/odf_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

namespace Oox2Odf
{

void OoxConverter::convert(OOX::Diagram::CShapeTree *oox_shape_tree)
{
	if (oox_shape_tree == NULL) return;
	
	odf_context()->drawing_context()->start_group();

	cpdoccore::_CP_OPT(double) x, y , ch_x , ch_y ;
	odf_context()->drawing_context()->set_group_position(x, y, ch_x, ch_y);

	for (long i=0 ;i < oox_shape_tree->m_arrItems.size(); i++)
	{
		convert(oox_shape_tree->m_arrItems[i]);
	}
	odf_context()->drawing_context()->end_group();
}

void OoxConverter::convert(OOX::Diagram::CShape *oox_shape)
{
	if (oox_shape == NULL) return;

	if (!oox_shape->m_oSpPr.IsInit()) return;

	odf_context()->drawing_context()->start_drawing();
	
		int type = -1;

		if (oox_shape->m_oSpPr->m_oPrstGeom.IsInit())
		{
			OOX::Drawing::CPresetGeometry2D * geometry = oox_shape->m_oSpPr->m_oPrstGeom.GetPointer();
			type =(geometry->m_oPrst.GetValue());
		}
	
		if (type < 0 && oox_shape->m_oSpPr->m_oCustGeom.IsInit())
		{
			type = 1000;//6??? - poligon, path нужно пересчитывать
		}
		//if (oox_shape->m_oNvSpPr.IsInit() && oox_shape->m_oNvSpPr->m_oCNvSpPr.IsInit() && ?????
		//											oox_shape->m_oNvSpPr->m_oCNvSpPr->m_otxBox.GetValue() == 1) type = 2000; //textBox

		if (type == SimpleTypes::shapetypeRect && oox_shape->m_oTxBody.IsInit()) type = 2000;

		if (type == 2000 && oox_shape->m_oTxBody->m_oBodyPr.IsInit() && oox_shape->m_oTxBody->m_oBodyPr->m_oFromWordArt.ToBool())
		{
			int wordart_type = convert(oox_shape->m_oTxBody->m_oBodyPr->m_oPrstTxWrap.GetPointer());

			if (wordart_type >0)type = wordart_type;
		}

		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		odf_context()->drawing_context()->start_shape(type);
		
		convert(oox_shape->m_oSpPr.GetPointer(), oox_shape->m_oShapeStyle.GetPointer());
	//имя, описалово, номер ...	
		//if (oox_shape->m_oNvSpPr.IsInit())
		//	convert(oox_shape->m_oNvSpPr->m_oCNvPr.GetPointer());	

		if ( oox_shape->m_oTxBody.IsInit() && oox_shape->m_oTxBody->m_arrItems.size() >0)
		{
			odf_context()->start_text_context();	
				
			for (long i=0 ; i < oox_shape->m_oTxBody->m_arrItems.size();i++)
			{
				convert(oox_shape->m_oTxBody->m_arrItems[i]);
			}
			odf_context()->drawing_context()->set_text( odf_context()->text_context());
			
			//наложим внешние настройки для текста
			convert(oox_shape->m_oTxBody->m_oBodyPr.GetPointer());			
			
			if (oox_shape->m_oShapeStyle.IsInit() && oox_shape->m_oShapeStyle->m_oFontRef.getType() == OOX::et_a_fontRef)
			{
				convert(&oox_shape->m_oShapeStyle->m_oFontRef);
			}	


			odf_context()->end_text_context();	
		}

	odf_context()->drawing_context()->end_shape();
	
	odf_context()->drawing_context()->end_drawing();
}
}
