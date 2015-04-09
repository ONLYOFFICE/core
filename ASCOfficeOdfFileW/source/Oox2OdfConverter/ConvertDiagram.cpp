#include "stdAfx.h"

#include "Converter.h"
#include "../utils.h"

#include "../../../Common/DocxFormat/source/DocxFormat/Diagram/DiagramDrawing.h"

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
