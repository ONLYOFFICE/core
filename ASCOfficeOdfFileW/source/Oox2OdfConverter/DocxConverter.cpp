#pragma once

#include "stdAfx.h"

#include "DocxConverter.h"
#include "../utils.h"

#include <DocxFormat\Docx.h>
#include <XlsxFormat\Chart\Chart.h>

#include <boost/foreach.hpp>

#include "odf_conversion_context.h"
#include "odt_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"


using namespace cpdoccore;

namespace Oox2Odf
{
DocxConverter::DocxConverter(const std::wstring & path)
{
	output_document = new  odf::package::odf_document(L"text");

	const OOX::CPath oox_path(CString(path.c_str()));	
	docx_document = new OOX::CDocx(oox_path);	
}
void DocxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);

}
odf::odf_conversion_context* DocxConverter::odf_context()
{
	return odt_context;
}
OOX::CTheme* DocxConverter::oox_theme()
{
	if (docx_document)
		return docx_document->GetTheme();
	else
		return NULL;
}
CString	DocxConverter::find_link_by_id (CString sId, int type)
{
	CString ref;
	//if (type==1)
	//{
	//	if (docx_current_drawing)
	//	{
	//		smart_ptr<OOX::File> oFile = docx_current_drawing->Find(sId);
	//		if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
	//		{
	//			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

	//			ref = pImage->filename().GetPath();
	//		}
	//	}
	//	if (ref.GetLength() < 1 && oox_current_chart)
	//	{
	//		smart_ptr<OOX::File> oFile = oox_current_chart->Find(sId);
	//		if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
	//		{
	//			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

	//			ref = pImage->filename().GetPath();
	//		}
	//	}
	//}
	return ref;
}

void DocxConverter::convertDocument()
{
	if (!docx_document)return;

	odt_context = new odf::odt_conversion_context(output_document);

	if (!odt_context)return;
		
	odt_context->start_document();

	//convert_styles();
	convert_document();

	//удалим уже ненужный документ docx 
	delete docx_document; docx_document = NULL;

	odt_context->end_document();
 
}

void DocxConverter::convert_document()
{
	const OOX::CDocument* document = docx_document->GetDocument();
	if (!document)return;

	for ( int nIndex = 0; nIndex < document->m_arrItems.GetSize(); nIndex++ )
	{
		convert(document->m_arrItems[nIndex]);
	}
}
void DocxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_w_r:
		{
			OOX::Logic::CRun* pRun= static_cast<OOX::Logic::CRun*>(oox_unknown);
			convert(pRun);
		}break;
		case OOX::et_w_p:
		{
			OOX::Logic::CParagraph* pP= static_cast<OOX::Logic::CParagraph*>(oox_unknown);
			convert(pP);
		}break;
		case OOX::et_w_pPr:
		{
			OOX::Logic::CParagraphProperty* pPProp= static_cast<OOX::Logic::CParagraphProperty*>(oox_unknown);
			convert(pPProp);
		}break;
		case OOX::et_w_rPr:
		{
			OOX::Logic::CRunProperty* pRProp= static_cast<OOX::Logic::CRunProperty*>(oox_unknown);
			convert(pRProp);
		}break;
		case OOX::et_w_t:
		{
			OOX::Logic::CText* pText= static_cast<OOX::Logic::CText*>(oox_unknown);
			convert(pText);
		}break;
		case OOX::et_mc_alternateContent:
		{
			OOX::Logic::CAlternateContent* pAltCont= static_cast<OOX::Logic::CAlternateContent*>(oox_unknown);
			convert(pAltCont);
		}break;
		case OOX::et_w_drawing:
		{
			OOX::Logic::CDrawing* pDrawing= static_cast<OOX::Logic::CDrawing*>(oox_unknown);
			convert(pDrawing);
		}break;
		case OOX::et_w_Shape:
		{
			OOX::Logic::CShape* pShape = static_cast<OOX::Logic::CShape*>(oox_unknown);
			convert(pShape);
		}break;
		case OOX::et_w_GroupShape:
		{
			OOX::Logic::CGroupShape* pGroupShape= static_cast<OOX::Logic::CGroupShape*>(oox_unknown);
			convert(pGroupShape);
		}break;
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}
void DocxConverter::convert(OOX::Logic::CParagraph	*oox_paragraph)
{
	if (oox_paragraph == NULL) return;

	odt_context->start_paragraph();
	for ( int nIndex = 0; nIndex < oox_paragraph->m_arrItems.GetSize(); nIndex++ )
	{
		convert(oox_paragraph->m_arrItems[nIndex]);
	}
	odt_context->end_paragraph();
}
void DocxConverter::convert(OOX::Logic::CRun	*oox_run)
{
	if (oox_run == NULL) return;
	
	odt_context->start_run();	
	for(int i = 0; i < oox_run->m_arrItems.GetSize(); ++i)
	{
		convert(oox_run->m_arrItems[i]);
	}
	odt_context->end_run();

}
void DocxConverter::convert(OOX::Logic::CParagraphProperty	*oox_paragraph_prop)
{
	if (oox_paragraph_prop == NULL) return;

}
void DocxConverter::convert(OOX::Logic::CRunProperty *oox_run_prop)
{
	if (oox_run_prop == NULL) return;

}
void DocxConverter::convert(OOX::Logic::CText *oox_text)
{
	if (oox_text == NULL) return;

	odt_context->add_text_content(string2std_string(oox_text->m_sText));
}
void DocxConverter::convert(OOX::Logic::CAlternateContent *oox_alt_content)
{
	if (oox_alt_content == NULL)return;

	for(int i = 0; i < oox_alt_content->m_arrChoiceItems.GetSize(); ++i)
	{
		convert(oox_alt_content->m_arrChoiceItems[i]);
	}
	for(int i = 0; i < oox_alt_content->m_arrFallbackItems.GetSize(); ++i)
	{
		convert(oox_alt_content->m_arrFallbackItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CDrawing *oox_drawing)
{
	if (oox_drawing == NULL) return;

	odt_context->start_drawings();
		convert(oox_drawing->m_oAnchor.GetPointer());
		convert(oox_drawing->m_oInline.GetPointer());
	odt_context->end_drawings();
}
void DocxConverter::convert(OOX::Drawing::CAnchor *oox_anchor)
{
	if (oox_anchor == NULL)return;

	double width =0, height=0;
	if (oox_anchor->m_oExtent.IsInit()) //size
	{
		width = oox_anchor->m_oExtent->m_oCx.ToPoints();
		height = oox_anchor->m_oExtent->m_oCx.ToPoints();
	}

	odt_context->drawing_context()->set_drawings_rect(-1, -1, width, height);

	convert(oox_anchor->m_oGraphic.GetPointer());

}
void DocxConverter::convert(OOX::Drawing::CInline *oox_inline)
{
	if (oox_inline == NULL)return;

	double width =0, height=0;
	if (oox_inline->m_oExtent.IsInit()) //size
	{
		width = oox_inline->m_oExtent->m_oCx.ToPoints();
		height = oox_inline->m_oExtent->m_oCy.ToPoints();
	}

	odt_context->drawing_context()->set_drawings_rect(-1, -1, width, height);

	convert(oox_inline->m_oGraphic.GetPointer());

}
void DocxConverter::convert(OOX::Drawing::CGraphic *oox_graphic)
{
	if (oox_graphic == NULL)return;

	convert(oox_graphic->m_oPicture.GetPointer());
	convert(oox_graphic->m_oChart.GetPointer());
	convert(oox_graphic->m_oShape.GetPointer());
	convert(oox_graphic->m_oGroupShape.GetPointer());
}

void DocxConverter::convert(OOX::Drawing::CPicture * oox_picture)
{
	if (!oox_picture)return;

	odt_context->drawing_context()->start_drawing();

	CString pathImage;
	double Width=0, Height = 0;

	if (oox_picture->m_oBlipFill.m_oBlip.IsInit())
	{
		CString sID = oox_picture->m_oBlipFill.m_oBlip->m_oEmbed.GetValue();		
		pathImage = find_link_by_id(sID,1);
		
		if (pathImage.GetLength() < 1)
		{
			sID = oox_picture->m_oBlipFill.m_oBlip->m_oLink.GetValue();	
			//???
		}
		_gdi_graphics_::GetResolution(pathImage, Width, Height);
	}
	odt_context->start_image(string2std_string(pathImage));
	{
		if (oox_picture->m_oBlipFill.m_oTile.IsInit()) 
		{
			odt_context->drawing_context()->set_image_style_repeat(2);
		}
		if (oox_picture->m_oBlipFill.m_oStretch.IsInit())
		{
			odt_context->drawing_context()->set_image_style_repeat(1);
		}
		if (oox_picture->m_oBlipFill.m_oSrcRect.IsInit() && Width >0 && Height >0)
		{
			odt_context->drawing_context()->set_image_client_rect(oox_picture->m_oBlipFill.m_oSrcRect->m_oL.GetValue() * Width/100. ,
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oT.GetValue() * Height/100.,
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oR.GetValue() * Width/100. , 
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oB.GetValue() * Height/100.);
		}		

		OoxConverter::convert(&oox_picture->m_oNvPicPr.m_oCNvPr);		

		//oox_picture->m_oNvPicPr.m_oCNvPicPr
		//oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks
		{
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoChangeAspect)
			//{
			//}
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoCrop))
			//{
			//}
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoResize)
			//{
			//}
		}	
		//m_oExtLst


		OoxConverter::convert(&oox_picture->m_oSpPr, NULL);

	}
	odt_context->drawing_context()->end_image();
	odt_context->drawing_context()->end_drawing();
}

void DocxConverter::convert(OOX::Drawing::CChart * oox_chart)
{
	if (oox_chart == NULL)return;

	if (oox_chart->m_oRId.IsInit())
	{
		smart_ptr<OOX::File> oFile = docx_document->GetDocument()->Find(oox_chart->m_oRId->GetValue());
		if (oFile.IsInit() && OOX::FileTypes::Chart == oFile->type())
		{
			OOX::Spreadsheet::CChartSpace* pChart = (OOX::Spreadsheet::CChartSpace*)oFile.operator->();
			
			if (pChart)
			{
				odt_context->drawing_context()->start_drawing();				
				odt_context->drawing_context()->start_object(odf_context()->get_next_name_object());

					double width =0, height =0;
					odt_context->drawing_context()->get_size(width, height);

					OoxConverter::convert(pChart->m_oChartSpace.m_oSpPr.GetPointer());
					
					oox_current_chart = pChart;
					odf_context()->start_chart();
						odf_context()->chart_context()->set_chart_size(width, height);
						OoxConverter::convert(&pChart->m_oChartSpace);
					odf_context()->end_chart();
					oox_current_chart = NULL; // object???

				odt_context->drawing_context()->end_object();	
				odt_context->drawing_context()->end_drawing();
			}
		}
	}

}

void DocxConverter::convert(OOX::Logic::CGroupShape	 *oox_group_shape)
{
	if (oox_group_shape == NULL)return;
	if (oox_group_shape->m_arrItems.GetSize() < 1) return;

	std::wstring name;
	int id = -1;
	
	//if (oox_group_shape->m_oCNvPr.IsInit())
	{
		//if (oox_group_shape->m_oCNvPr->m_sName.IsInit())
		//		name = string2std_string(*oox_group_shape->m_oCNvPr->m_sName);
		//if (oox_group_shape->m_oCNvPr->m_oId.IsInit())
		//		id = oox_group_shape->m_oCNvPr->m_oId->GetValue();
	}

	odt_context->drawing_context()->start_group(name,id);

	OoxConverter::convert(oox_group_shape->m_oGroupSpPr.GetPointer());

	for (long i=0; i < oox_group_shape->m_arrItems.GetSize(); i++)
	{
		convert(oox_group_shape->m_arrItems[i]);
	}

	odt_context->drawing_context()->end_group();
}
void DocxConverter::convert(OOX::Logic::CShape	 *oox_shape)
{
	if (oox_shape == NULL)return;
	if (!oox_shape->m_oSpPr.IsInit()) return;

	odt_context->drawing_context()->start_drawing();
	
		int type = -1;
		if (oox_shape->m_oSpPr->m_oCustGeom.IsInit())
		{
			type = 1000;//6???
		}
		if (oox_shape->m_oSpPr->m_oPrstGeom.IsInit())
		{
			OOX::Drawing::CPresetGeometry2D * geometry = oox_shape->m_oSpPr->m_oPrstGeom.GetPointer();
			type =(geometry->m_oPrst.GetValue());
		}
		if (oox_shape->m_oCNvSpPr.IsInit())
		{
			if (oox_shape->m_oCNvSpPr->m_otxBox.GetValue() == 1)
				type = 2000; //textBox
		}
		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		if (type == 2000)	odt_context->drawing_context()->start_text_box(); 
		else				odt_context->drawing_context()->start_shape(type);
		
		OoxConverter::convert(oox_shape->m_oSpPr.GetPointer(), oox_shape->m_oShapeStyle.GetPointer());
	//имя, описалово, номер ...		
		OoxConverter::convert(oox_shape->m_oCNvPr.GetPointer());	
	//заблокированности 
		OoxConverter::convert(oox_shape->m_oCNvSpPr.GetPointer());			
		OoxConverter::convert(oox_shape->m_oCNvConnSpPr.GetPointer());
		
		if (oox_shape->m_oTxBody.IsInit() && oox_shape->m_oTxBody->m_oTxtbxContent.IsInit())
		{
			odt_context->start_text_context();
			odt_context->text_context()->set_single_object(true, NULL, NULL);

			for (long i=0 ; i < oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems.GetSize();i++)
			{
				convert(oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems[i]);
			}
			odt_context->drawing_context()->set_text( odt_context->text_context());
			odt_context->end_text_context();	
		}

	if (type == 2000)	odt_context->drawing_context()->end_text_box();
	else				odt_context->drawing_context()->end_shape();
	
	odt_context->drawing_context()->end_drawing();
}
} 