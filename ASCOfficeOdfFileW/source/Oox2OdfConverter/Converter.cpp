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

#include <XlsxFormat\Xlsx.h>
#include <DocxFormat\Docx.h>

#define PROGRESSEVENT_ID	0

namespace Oox2Odf
{
	static double pt2emu(double Val)
	{
		return (Val * 360000 * 2.54) / 72;
	}
	Converter::Converter(const std::wstring & path, std::wstring  type, const ProgressCallback* CallBack) 
    {
		impl_ = NULL;
		
		if (type == L"text")			impl_ = new DocxConverter(path, CallBack);
		if (type == L"spreadsheet")		impl_ = new XlsxConverter(path, CallBack);
	}

	Converter::~Converter() 
    {
		if (impl_ )delete impl_ ;
	}
    void Converter::convert()
    {
		if (!impl_)return;
		
		if (impl_->bUserStopConvert) return;
       
		impl_->convertDocument();
    }
    void Converter::write(const std::wstring & path) const
    {
		if (!impl_)return;

		if (impl_->bUserStopConvert) return;

		return impl_->write(path);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL  OoxConverter::UpdateProgress(long nComplete)
{
	if (pCallBack)
	{
		pCallBack->OnProgress (pCallBack->caller, PROGRESSEVENT_ID, nComplete);

		bUserStopConvert = 0;
		pCallBack->OnProgressEx (pCallBack->caller, PROGRESSEVENT_ID, nComplete, &bUserStopConvert);

		if (bUserStopConvert !=0 ) return TRUE;
	}

	return FALSE;
}
 
void OoxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	try
	{
		if (oox_unknown == NULL)return;

		switch(oox_unknown->getType())
		{
			case OOX::et_a_Shape:
			{
				OOX::Drawing::CShape* pShape = static_cast<OOX::Drawing::CShape*>(oox_unknown);
				convert(pShape);
			}break;
			case OOX::et_a_LockedCanvas:
			{
				OOX::Drawing::CLockedCanvas* pLockedCanvas = static_cast<OOX::Drawing::CLockedCanvas*>(oox_unknown);
				convert(pLockedCanvas);
			}break;
			case OOX::et_a_prstGeom:
			{
				OOX::Drawing::CPresetGeometry2D* pPresetGeom = static_cast<OOX::Drawing::CPresetGeometry2D*>(oox_unknown);
				convert(pPresetGeom);
			}break;
			case OOX::et_a_custGeom:
			{
				OOX::Drawing::CCustomGeometry2D* pCustomGeom = static_cast<OOX::Drawing::CCustomGeometry2D*>(oox_unknown);
				convert(pCustomGeom);
			}break;
			case OOX::et_a_lnTo:
			{
				OOX::Drawing::CPath2DLineTo* pLineTo = static_cast<OOX::Drawing::CPath2DLineTo*>(oox_unknown);
				convert(pLineTo);
			}break;
			case OOX::et_a_moveTo:
			{
				OOX::Drawing::CPath2DMoveTo* pMoveTo = static_cast<OOX::Drawing::CPath2DMoveTo*>(oox_unknown);
				convert(pMoveTo);
			}break;
			case OOX::et_a_arcTo:
			{
				OOX::Drawing::CPath2DArcTo* pArcTo = static_cast<OOX::Drawing::CPath2DArcTo*>(oox_unknown);
				convert(pArcTo);
			}break;
			case OOX::et_a_quadBezTo:
			{
				OOX::Drawing::CPath2DQuadBezierTo* pQuadBezierTo = static_cast<OOX::Drawing::CPath2DQuadBezierTo*>(oox_unknown);
				convert(pQuadBezierTo);
			}break;
			case OOX::et_a_cubicBezTo:
			{
				OOX::Drawing::CPath2DCubicBezierTo* pCubicBezierT = static_cast<OOX::Drawing::CPath2DCubicBezierTo*>(oox_unknown);
				convert(pCubicBezierT);
			}break;
			case OOX::et_a_close:
			{
				OOX::Drawing::CPath2DClose* pClose= static_cast<OOX::Drawing::CPath2DClose*>(oox_unknown);
				convert(pClose);
			}break;
			case OOX::et_a_r:
			{
				OOX::Drawing::CRun* pRun= static_cast<OOX::Drawing::CRun*>(oox_unknown);
				convert(pRun);
			}break;
			case OOX::et_a_alphaModFix:
			{
				OOX::Drawing::CAlphaModulateFixedEffect* pAlpha= static_cast<OOX::Drawing::CAlphaModulateFixedEffect*>(oox_unknown);
				if (pAlpha)
				{
					odf_context()->drawing_context()->set_opacity(pAlpha->m_oAmt.GetValue());
				}
			}break;
			case OOX::et_v_imagedata:
			{
				OOX::Vml::CImageData *vml = static_cast<OOX::Vml::CImageData*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_textbox:
			{
				OOX::Vml::CTextbox *vml = static_cast<OOX::Vml::CTextbox*>(oox_unknown);
				convert(vml);
			}break;
			//case OOX::et_v_background:
			//{
			//	OOX::Vml::CBackground *vml = static_cast<OOX::Vml::CBackground*>(oox_unknown);
			//	convert(vml);
			//}break;
			case OOX::et_v_path:
			{
				OOX::Vml::CPath *vml = static_cast<OOX::Vml::CPath*>(oox_unknown);
				convert(vml);
			}break;	
				case OOX::et_v_textpath:
			{
				OOX::Vml::CTextPath *vml = static_cast<OOX::Vml::CTextPath*>(oox_unknown);
				convert(vml);
			}break;	
			case OOX::et_v_fill:
			{
				OOX::Vml::CFill *vml = static_cast<OOX::Vml::CFill*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_stroke:
			{
				OOX::Vml::CStroke *vml = static_cast<OOX::Vml::CStroke*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_image:
			{
				OOX::Vml::CImage *vml = static_cast<OOX::Vml::CImage*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_wd_wrap:
			{
				OOX::VmlWord::CWrap *vml = static_cast<OOX::VmlWord::CWrap*>(oox_unknown);
				convert(vml);
			}break;
			// "ненужные" элементы
			case OOX::et_w_softHyphen:
			case OOX::et_w_proofErr:
			case OOX::et_w_proofState:
			{
				//бяка
			}break;

			default:
			{
				int ttt = (int) oox_unknown->getType();
				std::wstringstream ss;
				ss << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
				_CP_LOG(error) << ss.str();
			}
		}
	}catch(...)
	{
		std::wstringstream ss;
		ss << L"[error] :  no convert element(" << (oox_unknown ? oox_unknown->getType() : -1 ) << L")\n";
		_CP_LOG(error) << ss.str();
	}
}

void OoxConverter::convert(OOX::Drawing::CShape	*oox_shape)
{
	if (oox_shape == NULL)return;
	if (!oox_shape->m_oSpPr.IsInit()) return;

	odf_context()->drawing_context()->start_drawing();
	
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
		if (oox_shape->m_oNvSpPr.IsInit() && oox_shape->m_oNvSpPr->m_oCNvSpPr.IsInit() &&
													oox_shape->m_oNvSpPr->m_oCNvSpPr->m_otxBox.GetValue() == 1) type = 2000; //textBox

		if (type == SimpleTypes::shapetypeRect && oox_shape->m_oTxSp.IsInit() && oox_shape->m_oTxSp->m_oTxBody.IsInit()) type = 2000;

		if (type == 2000 && oox_shape->m_oTxSp->m_oTxBody->m_oBodyPr.IsInit() && oox_shape->m_oTxSp->m_oTxBody->m_oBodyPr->m_oFromWordArt.ToBool())
		{
			int wordart_type = convert(oox_shape->m_oTxSp->m_oTxBody->m_oBodyPr->m_oPrstTxWrap.GetPointer());

			if (wordart_type >0)type = wordart_type;
		}

		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		odf_context()->drawing_context()->start_shape(type);
		
		convert(oox_shape->m_oSpPr.GetPointer(), oox_shape->m_oShapeStyle.GetPointer());
	//имя, описалово, номер ...	
		if (oox_shape->m_oNvSpPr.IsInit())
			convert(oox_shape->m_oNvSpPr->m_oCNvPr.GetPointer());	

		if (oox_shape->m_oTxSp.IsInit() && oox_shape->m_oTxSp->m_oTxBody.IsInit())
		{
			odf_context()->start_text_context();
		
			convert(oox_shape->m_oTxSp->m_oTxBody->m_oBodyPr.GetPointer());
			for (long i=0 ; i < oox_shape->m_oTxSp->m_oTxBody->m_arrItems.size();i++)
			{
				convert(oox_shape->m_oTxSp->m_oTxBody->m_arrItems[i]);
			}
			odf_context()->drawing_context()->set_text( odf_context()->text_context());
			odf_context()->end_text_context();	
		}

	odf_context()->drawing_context()->end_shape();
	
	odf_context()->drawing_context()->end_drawing();
}

void OoxConverter::convert(OOX::Drawing::CLockedCanvas  *oox_canvas)
{
	if (oox_canvas == NULL)return;

	odf_context()->drawing_context()->start_group();
		if (oox_canvas->m_oNvGroupSpPr.IsInit() && oox_canvas->m_oNvGroupSpPr->m_oCNvPr.IsInit())
		{	
			if (oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_sName.IsInit())
				odf_context()->drawing_context()->set_group_name(string2std_string(*oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_sName));
			if (oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_oId.IsInit())
				odf_context()->drawing_context()->set_group_z_order(oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_oId->GetValue());
		}
		convert(oox_canvas->m_oGroupSpPr.GetPointer());
		convert(oox_canvas->m_oSpPr.GetPointer());

		for (long i=0; i < oox_canvas->m_arrItems.size(); i++)
		{
			convert(oox_canvas->m_arrItems[i]);
		}
	odf_context()->drawing_context()->end_group();
}
void OoxConverter::convert(OOX::Drawing::CGroupShapeProperties *   oox_group_spPr)
{
	if (!oox_group_spPr) return;
	
	if (oox_group_spPr->m_oXfrm.IsInit())	//CTransform2D
	{
		if (oox_group_spPr->m_oXfrm->m_oFlipH.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context()->set_group_flip_H(true);
		if (oox_group_spPr->m_oXfrm->m_oFlipV.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context()->set_group_flip_V(true);

		if (oox_group_spPr->m_oXfrm->m_oChExt.IsInit() && oox_group_spPr->m_oXfrm->m_oExt.IsInit())
		{
			odf_context()->drawing_context()->set_group_size(	oox_group_spPr->m_oXfrm->m_oExt->m_oCx.ToPoints(),
																oox_group_spPr->m_oXfrm->m_oExt->m_oCy.ToPoints(),
																oox_group_spPr->m_oXfrm->m_oChExt->m_oCx.ToPoints(),
																oox_group_spPr->m_oXfrm->m_oChExt->m_oCy.ToPoints());
		}
		if (oox_group_spPr->m_oXfrm->m_oOff.IsInit() && oox_group_spPr->m_oXfrm->m_oChOff.IsInit())
		{
			odf_context()->drawing_context()->set_group_position(	oox_group_spPr->m_oXfrm->m_oOff->m_oX.ToPoints(), 
																	oox_group_spPr->m_oXfrm->m_oOff->m_oY.ToPoints(),
																	oox_group_spPr->m_oXfrm->m_oChOff->m_oX.ToPoints(), 
																	oox_group_spPr->m_oXfrm->m_oChOff->m_oY.ToPoints());
		}

		if (oox_group_spPr->m_oXfrm->m_oRot.GetValue() > 0)
			odf_context()->drawing_context()->set_group_rotate(oox_group_spPr->m_oXfrm->m_oRot.GetValue());
	}

}
void OoxConverter::convert(OOX::Drawing::CFontReference *style_font_ref)
{
	if (!style_font_ref) return;

	OOX::CTheme *theme = oox_theme();
	if (!theme) return;

	CString color;
	
	if (style_font_ref->m_eType == OOX::Drawing::colorSheme)
	{
		color = style_font_ref->m_oShemeClr.m_oVal.ToString();
	}
	
	if (style_font_ref->m_oIdx.GetValue() == SimpleTypes::fontcollectionindexMajor)
	{
		convert(&theme->m_oThemeElements.m_oFontScheme.m_oMajorFont, &color);
	}
	if (style_font_ref->m_oIdx.GetValue() == SimpleTypes::fontcollectionindexMinor)
	{
		convert(&theme->m_oThemeElements.m_oFontScheme.m_oMinorFont, &color);
	}
}
void OoxConverter::convert(OOX::Drawing::CFontCollection *style_font, CString *sheme_color)
{
	if ((sheme_color) && (sheme_color->GetLength() >0))
	{
		std::wstring hexColor;
		_CP_OPT(double) opacity;

		OOX::Drawing::CSchemeColor sheme;
		sheme.m_oVal.FromString(*sheme_color);
		convert(&sheme, hexColor, opacity);

		odf_context()->drawing_context()->set_textarea_fontcolor(hexColor);
	}
	if (style_font == NULL) return;

	_CP_OPT(std::wstring) font_latin, font_cs, font_ea; 
	if (style_font->m_oCs.m_oTypeFace.IsInit())		font_cs		= string2std_string(style_font->m_oCs.m_oTypeFace->GetValue());
	if (style_font->m_oEa.m_oTypeFace.IsInit())		font_ea		= string2std_string(style_font->m_oEa.m_oTypeFace->GetValue());
	if (style_font->m_oLatin.m_oTypeFace.IsInit())	font_latin	= string2std_string(style_font->m_oLatin.m_oTypeFace->GetValue());

	odf_context()->drawing_context()->set_textarea_font(font_latin,font_ea,font_cs);

	//nullable<OOX::Drawing::COfficeArtExtensionList> m_oExtLst;
	//CSimpleArray<OOX::Drawing::CSupplementalFont>   m_arrFont;
}
void OoxConverter::convert(OOX::Drawing::CStyleMatrixReference *style_matrix_ref)
{
	if (!style_matrix_ref) return;

	int fmt_index = style_matrix_ref->m_oIdx.GetValue()-1;

	OOX::CTheme *theme = oox_theme();
	if (!theme || fmt_index <0) return;

	CString color = style_matrix_ref->m_oShemeClr.m_oVal.ToString();
	
	if (style_matrix_ref->getType() == OOX::et_a_fillRef)
	{
		if (fmt_index < 1000 && fmt_index < theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems.GetSize()) 
		{
			switch(theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems[fmt_index]->getType())
			{
				case OOX::et_a_blipFill:	
					convert((OOX::Drawing::CBlipFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_gradFill:	
					convert((OOX::Drawing::CGradientFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_pattFill:	
					convert((OOX::Drawing::CPatternFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_solidFill:	
					convert((OOX::Drawing::CSolidColorFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::Drawing::filltypeNo:
					odf_context()->drawing_context()->set_no_fill();break;
			}
		}
		else if (fmt_index > 1000 && ((fmt_index-1000) < theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems.GetSize()))
		{
			fmt_index -= 1000; 
			switch(theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems[fmt_index]->getType())
			{
				case OOX::et_a_blipFill:	
					convert((OOX::Drawing::CBlipFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_gradFill:	
					convert((OOX::Drawing::CGradientFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_pattFill:	
					convert((OOX::Drawing::CPatternFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::et_a_solidFill:	
					convert((OOX::Drawing::CSolidColorFillProperties *)theme->m_oThemeElements.m_oFmtScheme.m_oBgFillStyleLst.m_arrItems[fmt_index], &color);break;
				case OOX::Drawing::filltypeNo:
					odf_context()->drawing_context()->set_no_fill();break;
			}
		}
	}

	if (style_matrix_ref->getType() == OOX::et_a_lnRef && fmt_index < theme->m_oThemeElements.m_oFmtScheme.m_oLineStyleLst.m_arrLn.GetSize())
	{
		convert(&theme->m_oThemeElements.m_oFmtScheme.m_oLineStyleLst.m_arrLn[fmt_index], &color);
	}

	if (style_matrix_ref->getType() == OOX::et_a_effectRef && fmt_index < theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle.GetSize())
	{
		convert(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oEffectList.GetPointer(), &color);

		//todooo
		//convert(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oEffectDag.GetPointer(), &color);
		//convert(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oScene3D.GetPointer(), &color);
		//convert(theme->m_oThemeElements.m_oFmtScheme.m_oEffectStyleLst.m_arrEffectStyle[fmt_index].m_oSp3D.GetPointer(), &color);
	}
}

void OoxConverter::convert(OOX::Drawing::CShapeProperties *   oox_spPr, OOX::Drawing::CShapeStyle* oox_sp_style)
{
	if (!oox_spPr) return;

	bool use_fill_from_style = false;
	bool use_line_from_style = false;

	if (oox_spPr->m_oXfrm.IsInit())	//CTransform2D
	{
		if (oox_spPr->m_oXfrm->m_oOff.IsInit())
		{
			odf_context()->drawing_context()->set_position(oox_spPr->m_oXfrm->m_oOff->m_oX.ToPoints(),
															oox_spPr->m_oXfrm->m_oOff->m_oY.ToPoints());
		}
		if (oox_spPr->m_oXfrm->m_oExt.IsInit())
		{
			odf_context()->drawing_context()->set_size(	oox_spPr->m_oXfrm->m_oExt->m_oCx.ToPoints(),
														oox_spPr->m_oXfrm->m_oExt->m_oCy.ToPoints());					
		}
		if (oox_spPr->m_oXfrm->m_oFlipH.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context()->set_flip_H(true);
		if (oox_spPr->m_oXfrm->m_oFlipV.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context()->set_flip_V(true);
		if (oox_spPr->m_oXfrm->m_oRot.GetValue() > 0)
			odf_context()->drawing_context()->set_rotate(180. - oox_spPr->m_oXfrm->m_oRot.GetValue()/60000.);
	}
	switch(oox_spPr->m_eGeomType)
	{
	case OOX::Drawing::geomtypeCustom :
			if(oox_spPr->m_oCustGeom.IsInit())convert(oox_spPr->m_oCustGeom.GetPointer());break;
	case OOX::Drawing::geomtypePreset:
			if(oox_spPr->m_oPrstGeom.IsInit())convert(oox_spPr->m_oPrstGeom.GetPointer());break;
	case OOX::Drawing::geomtypeUnknown:
	default:
		break;
	}
	odf_context()->drawing_context()->start_area_properties();
	{
		switch (oox_spPr->m_eFillType)
		{
			case OOX::Drawing::filltypeBlip:		convert(oox_spPr->m_oBlipFill.GetPointer());break;
			case OOX::Drawing::filltypeGradient:	convert(oox_spPr->m_oGradFill.GetPointer());break;
			case OOX::Drawing::filltypePattern:		convert(oox_spPr->m_oPattFill.GetPointer());break;
			case OOX::Drawing::filltypeSolid:		convert(oox_spPr->m_oSolidFill.GetPointer());break;
			case OOX::Drawing::filltypeGroup:
			case OOX::Drawing::filltypeNo:			odf_context()->drawing_context()->set_no_fill();break;
			default:
				use_fill_from_style = true;
				break;
		}
		if ((use_fill_from_style && oox_sp_style) && (oox_sp_style->m_oFillRef.getType() == OOX::et_a_fillRef))
		{
			convert(&oox_sp_style->m_oFillRef);
		}
	}
	odf_context()->drawing_context()->end_area_properties();

	odf_context()->drawing_context()->start_line_properties();
	{
		if (oox_spPr->m_oLn.IsInit())
			convert(oox_spPr->m_oLn.GetPointer());	//CLineProperties
		else if ((oox_sp_style) && (oox_sp_style->m_oLnRef.getType() == OOX::et_a_lnRef))
		{
			convert(&oox_sp_style->m_oLnRef);
		}
	}
	odf_context()->drawing_context()->end_line_properties();
//-----------------------------------------------------------------------------------------------------------------------------
	if (oox_spPr->m_oEffectList.IsInit())
		convert(oox_spPr->m_oEffectList.GetPointer());
	else if ((oox_sp_style) && (oox_sp_style->m_oEffectRef.getType() == OOX::et_a_effectRef))
	{
		convert(&oox_sp_style->m_oEffectRef);
	}

	//nullable<OOX::Drawing::CEffectContainer>          m_oEffectDag;

	//nullable<OOX::Drawing::COfficeArtExtensionList>   m_oExtLst;
	//nullable<OOX::Drawing::CScene3D>                  m_oScene3D;
	//nullable<OOX::Drawing::CShape3D>                  m_oSp3D;	
//-----------------------------------------------------------------------------------------------------------------------------
}
void OoxConverter::convert(OOX::Drawing::CNonVisualDrawingProps * oox_cnvPr)
{
	if (!oox_cnvPr) return;

	if (oox_cnvPr->m_sName.IsInit())
	{
		std::wstring name = string2std_string(oox_cnvPr->m_sName.get());
		odf_context()->drawing_context()->set_name(name);
	}
	if (oox_cnvPr->m_oId.IsInit())
	{
		int id =oox_cnvPr->m_oId->GetValue();
		odf_context()->drawing_context()->set_z_order(id);
	}
	//nullable<CString>                               m_sDescr;
	//nullable<SimpleTypes::COnOff<>>                 m_oHidden;
	//nullable<CString>                               m_sTitle;
}

void OoxConverter::convert(OOX::Drawing::CCustomGeometry2D *oox_cust_geom)
{
	if (!oox_cust_geom)return;

	for (long i=0; i< oox_cust_geom->m_oPthLst.m_arrPath.size();i++)
	{
		convert(&oox_cust_geom->m_oPthLst.m_arrPath[i]);
	}
	for (long i=0; i< oox_cust_geom->m_oPthLst.m_arrPath.size();i++)
	{
		convert(&oox_cust_geom->m_oPthLst.m_arrPath[i]);
	}
}
void OoxConverter::convert(OOX::Drawing::CLineProperties *oox_line_prop, CString *change_sheme_color )
{
	if (!oox_line_prop)return;
	
	switch (oox_line_prop->m_eFillType)
	{
		case OOX::Drawing::filltypeGradient:	convert(oox_line_prop->m_oGradFill.GetPointer(), change_sheme_color);break;
		case OOX::Drawing::filltypePattern:		convert(oox_line_prop->m_oPattFill.GetPointer(), change_sheme_color);break;
		case OOX::Drawing::filltypeSolid:		convert(oox_line_prop->m_oSolidFill.GetPointer(), change_sheme_color);break;
		case OOX::Drawing::filltypeGroup:
		case OOX::Drawing::filltypeNo:			odf_context()->drawing_context()->set_no_fill();
			break;
	}
	if (oox_line_prop->m_oW.IsInit())
	{
		odf_context()->drawing_context()->set_line_width(oox_line_prop->m_oW->ToPoints());
	}
	if (oox_line_prop->m_oHeadEnd.IsInit())
	{
		int type =0, w=1, len =1;//medium arrow
		if (oox_line_prop->m_oHeadEnd->m_oLen.IsInit())		len = oox_line_prop->m_oHeadEnd->m_oLen->GetValue();
		if (oox_line_prop->m_oHeadEnd->m_oType.IsInit())	type = oox_line_prop->m_oHeadEnd->m_oType->GetValue();
		if (oox_line_prop->m_oHeadEnd->m_oW.IsInit())		w=oox_line_prop->m_oHeadEnd->m_oW->GetValue();
		odf_context()->drawing_context()->set_line_head(type, len, w);
	}
	if (oox_line_prop->m_oTailEnd.IsInit())
	{
		int type =0, w=1, len =1;//medium arrow
		if (oox_line_prop->m_oTailEnd->m_oLen.IsInit())		len = oox_line_prop->m_oTailEnd->m_oLen->GetValue();
		if (oox_line_prop->m_oTailEnd->m_oType.IsInit())	type = oox_line_prop->m_oTailEnd->m_oType->GetValue();
		if (oox_line_prop->m_oTailEnd->m_oW.IsInit())		w = oox_line_prop->m_oTailEnd->m_oW->GetValue();
			
		odf_context()->drawing_context()->set_line_tail(type, len, w);

	}

	if (oox_line_prop->m_oCustDash.IsInit())
	{
		//через задание стиля и описание геометрии
	}
	if (oox_line_prop->m_oPrstDash.IsInit() && oox_line_prop->m_oPrstDash->m_oVal.IsInit())
	{
		odf_context()->drawing_context()->set_line_dash_preset(oox_line_prop->m_oPrstDash->m_oVal->GetValue());
	}
	//nullable<SimpleTypes::CPenAlignment<>>            m_oAlgn;
	//nullable<SimpleTypes::CLineCap<>>                 m_oCap;
	//nullable<SimpleTypes::CCompoundLine<>>            m_oCmpd;


	//ELineJoinType                                     m_eJoinType;   // Тип соединения линий
	//nullable<OOX::Drawing::CLineJoinBevel>            m_oBevel;
	//nullable<OOX::Drawing::CLineJoinMiterProperties>  m_oMiter;
	//nullable<OOX::Drawing::CLineJoinRound>            m_oRound;

}

void OoxConverter::convert(OOX::Drawing::CPresetGeometry2D *oox_prst_geom)
{
	if (!oox_prst_geom)return;

	if (oox_prst_geom->m_oAvLst.IsInit())
	{
		for (long i=0; i<oox_prst_geom->m_oAvLst->m_arrGd.size(); i++)
		{
			odf_context()->drawing_context()->add_modifier(string2std_string(oox_prst_geom->m_oAvLst->m_arrGd[i].m_oFmla.GetValue()));
		}
	}
}
void OoxConverter::convert(OOX::Drawing::CPath2D *oox_geom_path)
{
	if (!oox_geom_path) return;

	odf_context()->drawing_context()->set_viewBox(oox_geom_path->m_oW.GetValue(), oox_geom_path->m_oH.GetValue());

	for (long i =0 ; i< oox_geom_path->m_arrItems.size(); i++)
	{
		convert(oox_geom_path->m_arrItems[i]);
	}
}
//-----------------------------------------------------------------------------------------------------------------
void OoxConverter::convert(OOX::Drawing::CEffectList *oox_effect_list, CString *change_sheme_color)
{
	if (oox_effect_list == NULL) return;

	for (long i=0;i< oox_effect_list->m_arrEffects.size(); i++)
	{
		switch(oox_effect_list->m_arrEffects[i]->getType())
		{
		case OOX::et_a_outerShdw:
			{
				OOX::Drawing::COuterShadowEffect * shadow = static_cast<OOX::Drawing::COuterShadowEffect *>(oox_effect_list->m_arrEffects[i]);
				convert(shadow, change_sheme_color);
			}break;
		case OOX::et_a_innerShdw:
			{
				OOX::Drawing::CInnerShadowEffect * shadow = static_cast<OOX::Drawing::CInnerShadowEffect *>(oox_effect_list->m_arrEffects[i]);
				convert(shadow, change_sheme_color);
			}break;
		case OOX::et_a_prstShdw:
			{
				OOX::Drawing::CPresetShadowEffect * shadow = static_cast<OOX::Drawing::CPresetShadowEffect *>(oox_effect_list->m_arrEffects[i]);
				//convert(shadow, change_sheme_color);
			}break;
		case OOX::et_a_reflection:
			{
				OOX::Drawing::CReflectionEffect * refelection = static_cast<OOX::Drawing::CReflectionEffect *>(oox_effect_list->m_arrEffects[i]);
			}
			//CGlowEffect, CBlurEffect, CFillOverlayEffect, CSoftEdgesEffect
		}
	}
}
void OoxConverter::convert(OOX::Drawing::COuterShadowEffect *oox_shadow, CString *change_sheme_color)
{
	if (oox_shadow == NULL) return; 

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	CString keep_sheme_color;
	if (oox_shadow->m_eType == OOX::Drawing::colorSheme && change_sheme_color)
	{
		keep_sheme_color = oox_shadow->m_oShemeClr.m_oVal.ToString();
		oox_shadow->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
	}
	convert(static_cast<OOX::Drawing::CColor*>(oox_shadow), hexColor, opacity);
	if (keep_sheme_color.GetLength() > 0)	oox_shadow->m_oShemeClr.m_oVal.FromString(keep_sheme_color);

	odf_context()->drawing_context()->set_shadow(1, hexColor, opacity, oox_shadow->m_oDist.ToPoints());

			//SimpleTypes::CPositiveFixedAngle<0>                      m_oDir;
			//SimpleTypes::CPositiveCoordinate<0>                      m_oBlurRad;
//-------------
	//SimpleTypes::CRectAlignment<SimpleTypes::rectalignmentB> m_oAlgn;
	//SimpleTypes::CFixedAngle<>                               m_oKx;
	//SimpleTypes::CFixedAngle<>                               m_oKy;
	//SimpleTypes::COnOff<SimpleTypes::onoffTrue>              m_oRotWithShape;
	//SimpleTypes::CPercentage                                 m_oSx;
	//SimpleTypes::CPercentage                                 m_oSy;

}
void OoxConverter::convert(OOX::Drawing::CInnerShadowEffect *oox_shadow, CString *change_sheme_color)
{
	if (oox_shadow == NULL) return;

	std::wstring hexColor;
	_CP_OPT(double) opacity;

	CString keep_sheme_color;
	if (oox_shadow->m_eType == OOX::Drawing::colorSheme && change_sheme_color)
	{
		keep_sheme_color = oox_shadow->m_oShemeClr.m_oVal.ToString();
		oox_shadow->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
	}
	convert(static_cast<OOX::Drawing::CColor*>(oox_shadow), hexColor, opacity);
	if (keep_sheme_color.GetLength() > 0)	oox_shadow->m_oShemeClr.m_oVal.FromString(keep_sheme_color);

	odf_context()->drawing_context()->set_shadow(2, hexColor, opacity, oox_shadow->m_oDist.ToPoints());

}
//-----------------------------------------------------------------------------------------------------------------
void OoxConverter::convert(OOX::Drawing::CBlipFillProperties *oox_bitmap_fill,	CString *change_sheme_color)
{
	if (oox_bitmap_fill == NULL)return;

	odf_context()->drawing_context()->start_bitmap_style();
	{
		double Width=0, Height = 0;
		if (oox_bitmap_fill->m_oBlip.IsInit())
		{
			CString sID = oox_bitmap_fill->m_oBlip->m_oEmbed.GetValue();
			CString pathImage = find_link_by_id(sID,1);

			if (pathImage.GetLength() > 0)
			{
				odf_context()->drawing_context()->set_bitmap_link(string2std_string(pathImage));
				_gdi_graphics_::GetResolution(pathImage, Width, Height);
			}
			else
			{
				sID = oox_bitmap_fill->m_oBlip->m_oLink.GetValue();
				//...
			}
			for (long i=0 ; i < oox_bitmap_fill->m_oBlip->m_arrEffects.size(); i++)
				convert(oox_bitmap_fill->m_oBlip->m_arrEffects[i]);
		}
		if (oox_bitmap_fill->m_oSrcRect.IsInit() && Width >0  && Height >0)//часть изображения
		{
		}
		if (oox_bitmap_fill->m_oTile.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(2);
			
			if (oox_bitmap_fill->m_oTile->m_oAlgn.IsInit())
				odf_context()->drawing_context()->set_bitmap_tile_align(oox_bitmap_fill->m_oTile->m_oAlgn->GetValue());

			if (oox_bitmap_fill->m_oTile->m_oFlip.IsInit())	{}

			if (oox_bitmap_fill->m_oTile->m_oSx.IsInit() && Width >0)	
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_x(oox_bitmap_fill->m_oTile->m_oSx->GetValue() / 100. * Width);
			}
			if (oox_bitmap_fill->m_oTile->m_oSy.IsInit()&& Height >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_scale_y(oox_bitmap_fill->m_oTile->m_oSy->GetValue() / 100. * Height);
			}		
			if (oox_bitmap_fill->m_oTile->m_oTx.IsInit() && Width >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_x(oox_bitmap_fill->m_oTile->m_oTx->GetValue()*100. / Width );
			}
			if (oox_bitmap_fill->m_oTile->m_oTy.IsInit() && Height >0)
			{
				odf_context()->drawing_context()->set_bitmap_tile_translate_y(oox_bitmap_fill->m_oTile->m_oTy->GetValue()*100. / Height );
			}
		}
		if (oox_bitmap_fill->m_oStretch.IsInit())
		{
			odf_context()->drawing_context()->set_image_style_repeat(1);
			if (oox_bitmap_fill->m_oStretch->m_oFillRect.IsInit()){} //заполнение неполного объема
		}
	}
	odf_context()->drawing_context()->end_bitmap_style();
}

void OoxConverter::convert(OOX::Drawing::CGradientFillProperties *oox_grad_fill,	CString *change_sheme_color)
{
	if (!oox_grad_fill)return;

	odf_context()->drawing_context()->start_gradient_style();
	{
		odf::gradient_style grad_style = odf::gradient_style::linear;
	
		if (oox_grad_fill->m_eGradType == OOX::Drawing::gradfilltypeLinear && oox_grad_fill->m_oLin->m_oAng.IsInit())
		{
			odf_context()->drawing_context()->set_gradient_angle(oox_grad_fill->m_oLin->m_oAng->GetAngle());
		}
		if (oox_grad_fill->m_eGradType == OOX::Drawing::gradfilltypePath && oox_grad_fill->m_oPath->m_oPath.IsInit())
		{
			switch(oox_grad_fill->m_oPath->m_oPath->GetValue())
			{
				case SimpleTypes::pathshadetypeCircle:	grad_style = odf::gradient_style::radial;		break;
				case SimpleTypes::pathshadetypeRect:	grad_style = odf::gradient_style::rectangular;	break;
				case SimpleTypes::pathshadetypeShape:	grad_style = odf::gradient_style::square;		break;
			}	
			if (oox_grad_fill->m_oPath->m_oFillToRect.IsInit())
			{
				odf_context()->drawing_context()->set_gradient_rect( oox_grad_fill->m_oPath->m_oFillToRect->m_oL.GetValue(),
																	oox_grad_fill->m_oPath->m_oFillToRect->m_oT.GetValue(),
																	oox_grad_fill->m_oPath->m_oFillToRect->m_oR.GetValue(),
																	oox_grad_fill->m_oPath->m_oFillToRect->m_oB.GetValue());			
			}
		}	
		odf_context()->drawing_context()->set_gradient_type(grad_style);

		if (oox_grad_fill->m_oGsLst.IsInit() && oox_grad_fill->m_oGsLst->m_arrGs.size()>1)
		{
			std::wstring hexColorStart, hexColorEnd;
			_CP_OPT(double) opacityStart, opacityEnd;
			
			if (change_sheme_color && oox_grad_fill->m_oGsLst->m_arrGs[0].m_eType == OOX::Drawing::colorSheme)
			{
				oox_grad_fill->m_oGsLst->m_arrGs[0].m_oShemeClr.m_oVal.FromString(*change_sheme_color);
				oox_grad_fill->m_oGsLst->m_arrGs[oox_grad_fill->m_oGsLst->m_arrGs.size()-1].m_oShemeClr.m_oVal.FromString(*change_sheme_color);
			}
			convert((OOX::Drawing::CColor*)(&oox_grad_fill->m_oGsLst->m_arrGs[oox_grad_fill->m_oGsLst->m_arrGs.size()-1]),hexColorStart, opacityStart);
			convert((OOX::Drawing::CColor*)(&oox_grad_fill->m_oGsLst->m_arrGs[0]),hexColorEnd, opacityEnd);
			
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
						
						if (oox_grad_fill->m_eGradType == OOX::Drawing::gradfilltypeLinear && oox_grad_fill->m_oLin->m_oAng.IsInit())
						{
							odf_context()->drawing_context()->set_opacity_angle(oox_grad_fill->m_oLin->m_oAng->GetAngle());
						}
						if (oox_grad_fill->m_eGradType == OOX::Drawing::gradfilltypePath && oox_grad_fill->m_oPath->m_oFillToRect.IsInit())
						{
							odf_context()->drawing_context()->set_opacity_rect (oox_grad_fill->m_oPath->m_oFillToRect->m_oL.GetValue(),
																			oox_grad_fill->m_oPath->m_oFillToRect->m_oT.GetValue(),
																			oox_grad_fill->m_oPath->m_oFillToRect->m_oR.GetValue(),
																			oox_grad_fill->m_oPath->m_oFillToRect->m_oB.GetValue());
						}
					odf_context()->drawing_context()->end_opacity_style();
				}
			}
		}
	}
	odf_context()->drawing_context()->end_gradient_style();
}
void OoxConverter::convert(OOX::Drawing::CPatternFillProperties *oox_pattern_fill,	CString *change_sheme_color)
{
	if (!oox_pattern_fill)return;
	
	odf_context()->drawing_context()->start_hatch_style();
	{
		if (oox_pattern_fill->m_oPrst.IsInit())
		{
			odf_context()->drawing_context()->set_hatch_type(oox_pattern_fill->m_oPrst->GetValue());
		}			
		if (oox_pattern_fill->m_oFgClr.IsInit())
		{
			std::wstring hexColor;
			_CP_OPT(double) opacity;
			convert((OOX::Drawing::CColor*)oox_pattern_fill->m_oFgClr.GetPointer(),hexColor, opacity);

			odf_context()->drawing_context()->set_hatch_line_color(hexColor);
		}
		if (oox_pattern_fill->m_oBgClr.IsInit())
		{
			if (change_sheme_color && oox_pattern_fill->m_oBgClr.IsInit() && oox_pattern_fill->m_oBgClr->m_eType == OOX::Drawing::colorSheme)
				oox_pattern_fill->m_oBgClr->m_oShemeClr.m_oVal.FromString(*change_sheme_color);

			std::wstring hexColor;
			_CP_OPT(double) opacity;
			convert((OOX::Drawing::CColor*)oox_pattern_fill->m_oBgClr.GetPointer(),hexColor, opacity);

			odf_context()->drawing_context()->set_hatch_area_color(hexColor);
		}
	}
	odf_context()->drawing_context()->end_hatch_style();

}
void OoxConverter::convert(OOX::Drawing::CSolidColorFillProperties *oox_solid_fill,std::wstring & hexColor , _CP_OPT(double) &opacity)
{
	switch( oox_solid_fill->m_eType )
	{
		case OOX::Drawing::colorSheme:	convert(&oox_solid_fill->m_oShemeClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorHsl:	convert(&oox_solid_fill->m_oHslClr,		hexColor, opacity);		break;
		case OOX::Drawing::colorPrst:	convert(&oox_solid_fill->m_oPrstClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorScRgb:	convert(&oox_solid_fill->m_oScrgbClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorSRgb:	convert(&oox_solid_fill->m_oSrgbClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorSys:	convert(&oox_solid_fill->m_oSysClr,		hexColor, opacity);		break;		
	}	
}
void OoxConverter::convert(OOX::Drawing::CColor *oox_color,std::wstring & hexColor , _CP_OPT(double) &opacity)
{
	switch( oox_color->m_eType )
	{
		case OOX::Drawing::colorSheme:	convert(&oox_color->m_oShemeClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorHsl:	convert(&oox_color->m_oHslClr,		hexColor, opacity);		break;
		case OOX::Drawing::colorPrst:	convert(&oox_color->m_oPrstClr,		hexColor, opacity);		break;
		case OOX::Drawing::colorScRgb:	convert(&oox_color->m_oScrgbClr,	hexColor, opacity);		break;
		case OOX::Drawing::colorSRgb:	convert(&oox_color->m_oSrgbClr,		hexColor, opacity);		break;
		case OOX::Drawing::colorSys:	convert(&oox_color->m_oSysClr,		hexColor, opacity);		break;		
	}	
}
void OoxConverter::convert(OOX::Drawing::CSolidColorFillProperties *oox_solid_fill,	CString *change_sheme_color)
{
	if (!oox_solid_fill)return;

	std::wstring hexColor;
	_CP_OPT(double) opacity;
	
	if (change_sheme_color && oox_solid_fill->m_eType == OOX::Drawing::colorSheme)
		oox_solid_fill->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
	
	convert(oox_solid_fill, hexColor, opacity);

	odf_context()->drawing_context()->set_solid_fill(hexColor);

	if (opacity)
	{
		odf_context()->drawing_context()->set_opacity(*opacity);
	}
}

void OoxConverter::convert(OOX::Drawing::Colors::CColorTransform     *oox_Clr,	std::wstring & hexString, _CP_OPT(double) &opacity)
{
	if (!oox_Clr)return;
	BYTE ucA=0, ucG=0, ucB=0, ucR =0;
	oox_Clr->GetRGBA(ucR,ucG,ucB,ucA);

	SimpleTypes::CHexColor<SimpleTypes::hexcolorRGB> *oRgbColor = new SimpleTypes::CHexColor<SimpleTypes::hexcolorRGB>(ucR,ucG,ucB);
	if (oRgbColor)
	{		
		hexString = string2std_string(oRgbColor->ToString().Right(6));
		delete oRgbColor;
	}
	if (ucA !=255)opacity = (ucA/255.)* 100.;
}
void OoxConverter::convert(OOX::Drawing::CSchemeColor     *oox_ShemeClr,	std::wstring & hexString, _CP_OPT(double) &opacity)
{
	OOX::CTheme * theme= oox_theme();
	if (!oox_ShemeClr || !theme)return;

	int theme_ind = oox_ShemeClr->m_oVal.GetValue();
	
	BYTE ucA=0, ucG=0, ucB=0, ucR =0;
	bool result = false;

	switch(theme_ind)
	{
		case SimpleTypes::shemecolorvalLt1:
		case SimpleTypes::shemecolorvalBg1:
			result = theme->m_oThemeElements.m_oClrScheme.m_oLt1.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalLt2:
		case SimpleTypes::shemecolorvalBg2:
			result = theme->m_oThemeElements.m_oClrScheme.m_oLt2.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalDk1:
		case SimpleTypes::shemecolorvalTx1:
			result = theme->m_oThemeElements.m_oClrScheme.m_oDk1.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalDk2:
		case SimpleTypes::shemecolorvalTx2:
			result = theme->m_oThemeElements.m_oClrScheme.m_oDk2.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent1:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent1.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent2:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent2.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent3:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent3.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent4:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent4.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent5:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent5.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalAccent6:
			result = theme->m_oThemeElements.m_oClrScheme.m_oAccent6.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalFolHlink:
			result = theme->m_oThemeElements.m_oClrScheme.m_oFolHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
		case SimpleTypes::shemecolorvalHlink:
			result = theme->m_oThemeElements.m_oClrScheme.m_oHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
	}
	if (result == true)
	{
		oox_ShemeClr->SetRGBA(ucR,ucG,ucB,ucA);
		oox_ShemeClr->GetRGBA(ucR,ucG,ucB,ucA);

		SimpleTypes::CHexColor<SimpleTypes::hexcolorRGB> *oRgbColor = new SimpleTypes::CHexColor<SimpleTypes::hexcolorRGB>(ucR,ucG,ucB);
		if (oRgbColor)
		{		
			hexString = string2std_string(oRgbColor->ToString().Right(6));
			delete oRgbColor;
		}
		if (ucA !=255)opacity = (ucA/255.)* 100.;
	}
}
void OoxConverter::convert(OOX::Drawing::CPath2DLineTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm =				boost::lexical_cast<std::wstring> ( (int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
					std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));
	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"L"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DMoveTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm =						boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
							std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"M"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DArcTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	//std::wstring path_elm =						boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
	//						std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));
	//
	//odf_context()->drawing_context()->add_path_element(std::wstring(L"A"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DQuadBezierTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm =							boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl.m_oX.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl.m_oY.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oX.GetValue())) +
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oY.GetValue())) ;	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"S"), path_elm);

}
void OoxConverter::convert(OOX::Drawing::CPath2DCubicBezierTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm =							boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl1.m_oX.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl1.m_oY.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl2.m_oX.GetValue())) +
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl2.m_oY.GetValue())) +
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oX.GetValue())) +
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oY.GetValue())) ;	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"C"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DClose *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm ;	
	odf_context()->drawing_context()->add_path_element(std::wstring(L"N"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CTextBodyProperties	*oox_bodyPr)
{
	if (!oox_bodyPr) return;

	odf_context()->drawing_context()->set_textarea_writing_mode		(oox_bodyPr->m_oVert.GetValue());
	odf_context()->drawing_context()->set_textarea_vertical_align	(oox_bodyPr->m_oAnchor.GetValue());

	odf_context()->drawing_context()->set_textarea_padding  (oox_bodyPr->m_oLIns.ToCm(),
															oox_bodyPr->m_oTIns.ToCm(),
															oox_bodyPr->m_oRIns.ToCm(),
															oox_bodyPr->m_oBIns.ToCm());
	odf_context()->drawing_context()->set_textarea_wrap(oox_bodyPr->m_oWrap.GetValue());

	if (oox_bodyPr->m_oNumCol.GetValue() > 1)
	{
		//+ style section
		//+element text:section в котором параграфы
	}
	if (oox_bodyPr->m_oFromWordArt.ToBool() && oox_bodyPr->m_oPrstTxWrap.IsInit())
	{
		for (long i=0; i< oox_bodyPr->m_oPrstTxWrap->m_oAvLst->m_arrGd.size(); i++)
		{
			odf_context()->drawing_context()->add_modifier(string2std_string(oox_bodyPr->m_oPrstTxWrap->m_oAvLst->m_arrGd[i].m_oFmla.GetValue()));
		}
	}
}

int OoxConverter::convert(OOX::Drawing::CPresetTextShape *oox_text_preset)
{
	if (oox_text_preset == NULL) return -1;
	if (oox_text_preset->m_oPrst.GetValue() ==  SimpleTypes::textshapetypeTextNoShape) return -1;

	return 2001 + oox_text_preset->m_oPrst.GetValue();
}
void OoxConverter::convert(OOX::Drawing::CRunProperty * oox_run_pr, odf::style_text_properties * text_properties)
{
	if (!oox_run_pr)		return;
	if (!text_properties)	return;

	if (oox_run_pr->m_oBold.IsInit())
	{
		if (oox_run_pr->m_oBold->ToBool() == true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}

	if (oox_run_pr->m_oSolidFill.IsInit())
	{
		std::wstring hexColor;
		_CP_OPT(double) opacity;
		convert(oox_run_pr->m_oSolidFill.GetPointer(),hexColor,opacity);
		
		text_properties->content().fo_color_ = odf::color(std::wstring(L"#") + hexColor);
	}

	if (oox_run_pr->m_oItalic.IsInit())
	{
		if (oox_run_pr->m_oItalic->ToBool() ==true)
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Normal);
	}
	if (oox_run_pr->m_oSz.IsInit())
	{
		convert(oox_run_pr->m_oSz->GetValue()/100., text_properties->content().fo_font_size_);
		convert(oox_run_pr->m_oSz->GetValue()/100., text_properties->content().style_font_size_asian_);
		convert(oox_run_pr->m_oSz->GetValue()/100., text_properties->content().style_font_size_complex_);
	}
	if (oox_run_pr->m_oLatinFont.IsInit())
	{
		if (oox_run_pr->m_oLatinFont->m_oTypeFace.IsInit())	
			text_properties->content().fo_font_family_ = string2std_string(oox_run_pr->m_oLatinFont->m_oTypeFace->GetValue());
		else
		{
		}
	}
	if (oox_run_pr->m_oAsianFont.IsInit())
	{
		if (oox_run_pr->m_oAsianFont->m_oTypeFace.IsInit())	
			text_properties->content().style_font_family_asian_ = string2std_string(oox_run_pr->m_oAsianFont->m_oTypeFace->GetValue());
		else
		{
		}
	}
	if (oox_run_pr->m_oComplexFont.IsInit())
	{
		if (oox_run_pr->m_oComplexFont->m_oTypeFace.IsInit())
			text_properties->content().style_font_family_complex_ = string2std_string(oox_run_pr->m_oComplexFont->m_oTypeFace->GetValue());
		else
		{
		}
	}
	if (oox_run_pr->m_oLanguage.IsInit())
	{
		CString oox_language =  oox_run_pr->m_oLanguage->GetValue(), oox_country;
		int res = oox_language.Find(L"-");
		if (res  >=0)
		{
			oox_country = oox_language.Right(oox_language.GetLength() - res -1);
			oox_language = oox_language.Left(res);
		}
		
		text_properties->content().fo_language_ = string2std_string(oox_language);
		if (oox_country.GetLength() > 0)
			text_properties->content().fo_country_ = string2std_string(oox_country);

	}
	if (oox_run_pr->m_oSpace.IsInit())
	{
		text_properties->content().fo_letter_spacing_ = odf::letter_spacing(odf::length(oox_run_pr->m_oSpace->GetValue()/100., odf::length::pt));
	}
	if (oox_run_pr->m_oUnderline.IsInit())
	{
		text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Solid);
		text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Single);
		
		SimpleTypes::EUnderline 	type = oox_run_pr->m_oUnderline->GetValue();
		switch(type)
		{
		case SimpleTypes::underlineNone	:
				text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::None);break;
		case SimpleTypes::underlineDash :
		case SimpleTypes::underlineDashedHeavy:
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Dash);break;
		case SimpleTypes::underlineDotDash :
		case SimpleTypes::underlineDashDotHeavy:
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::DotDash);break;
		case SimpleTypes::underlineDashLong:
		case SimpleTypes::underlineDashLongHeavy:
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::LongDash);break;
		case SimpleTypes::underlineDotDotDash:
		case SimpleTypes::underlineDashDotDotHeavy :
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::DotDotDash);break;
		case SimpleTypes::underlineDotted:
		case SimpleTypes::underlineDottedHeavy:
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Dotted);break;
		case SimpleTypes::underlineWave :
		case SimpleTypes::underlineWavyHeavy :
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Wave);break;
		case SimpleTypes::underlineDouble :
		case SimpleTypes::underlineThick :
				text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Double);break;
		case SimpleTypes::underlineWavyDouble :
				text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Double);
				text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Wave);break;
		}
	}
}
void OoxConverter::convert(OOX::Drawing::CRun		*oox_run)
{
	if (!oox_run)return;

	bool styled = false;

	if (oox_run->m_oRunProperty.IsInit())
	{
		odf::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		
		if (!text_properties)
		{
			odf_context()->styles_context()->create_style(L"",odf::style_family::Text, true, false, -1);	
			text_properties = odf_context()->styles_context()->last_state()->get_text_properties();
			styled = true;
		}
		convert(oox_run->m_oRunProperty.GetPointer(), text_properties);
	}
	
	odf_context()->text_context()->start_span(styled);	
	if (oox_run->m_oText.IsInit())
	{
		odf_context()->text_context()->add_text_content( string2std_string(oox_run->m_oText->m_sText));
	}
	odf_context()->text_context()->end_span();
}

void OoxConverter::convert(OOX::Drawing::CParagraphProperty * oox_paragraph_pr, cpdoccore::odf::style_paragraph_properties * paragraph_properties)
{
	if (!oox_paragraph_pr)		return;
	if (!paragraph_properties)	return;

	if (oox_paragraph_pr->m_oLineSpacing.IsInit())
	{
		odf::length_or_percent length;
		convert(oox_paragraph_pr->m_oLineSpacing.GetPointer(), length);
		paragraph_properties->content().fo_line_height_ = odf::line_width(length);
	}
	if (oox_paragraph_pr->m_oAfterSpacing.IsInit())
	{
 		odf::length_or_percent length;
		convert(oox_paragraph_pr->m_oAfterSpacing.GetPointer(), length);
		paragraph_properties->content().fo_margin_bottom_ = length;
	}
	if (oox_paragraph_pr->m_oBeforeSpacing.IsInit())
	{
 		odf::length_or_percent length;
		convert(oox_paragraph_pr->m_oBeforeSpacing.GetPointer(), length);
		paragraph_properties->content().fo_margin_top_ = length;
	}
	if (oox_paragraph_pr->m_oAlgn.IsInit())
	{
		switch(oox_paragraph_pr->m_oAlgn->GetValue())
		{
			case SimpleTypes::textalignmenttypeL    : paragraph_properties->content().fo_text_align_ = odf::text_align(odf::text_align::Left);		break;
			case SimpleTypes::textalignmenttypeCtr  : paragraph_properties->content().fo_text_align_ = odf::text_align(odf::text_align::Center);	break;
			case SimpleTypes::textalignmenttypeDist : paragraph_properties->content().fo_text_align_ = odf::text_align(odf::text_align::Justify);	break;
			case SimpleTypes::textalignmenttypeJust : paragraph_properties->content().fo_text_align_ = odf::text_align(odf::text_align::Justify);	break;
			case SimpleTypes::textalignmenttypeR    : paragraph_properties->content().fo_text_align_ = odf::text_align(odf::text_align::Right);		break;
		}
	}
	if (oox_paragraph_pr->m_oMarL.IsInit())
	{
		paragraph_properties->content().fo_margin_left_ = odf::length_or_percent(odf::length(oox_paragraph_pr->m_oMarL->ToCm(),odf::length::cm));
	}
	if (oox_paragraph_pr->m_oMarR.IsInit())
	{
		paragraph_properties->content().fo_margin_right_ = odf::length_or_percent(odf::length(oox_paragraph_pr->m_oMarR->ToCm(),odf::length::cm));
	}
	if (oox_paragraph_pr->m_oIndent.IsInit())
	{
		paragraph_properties->content().fo_text_indent_ = odf::length_or_percent(odf::length(oox_paragraph_pr->m_oIndent->ToCm(),odf::length::cm));
	}
	if (oox_paragraph_pr->m_oRtl.IsInit())
	{
	}	
	//nullable<SimpleTypes::CDecimalNumber<> >				m_oLvl;
	//nullable<SimpleTypes::CCoordinate32<> >				m_oDefTabSz;
	//nullable<SimpleTypes::CTextFontAlignType<>>			m_oFontAlgn;



	if (oox_paragraph_pr->m_oDefRunProperty.IsInit())//может быть пустым !!!
	{
		odf::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		if (text_properties) 
			convert(oox_paragraph_pr->m_oDefRunProperty.GetPointer(), text_properties);

		//надо подумать как брать последний по family !!! 
		//convert(oox_paragraph_pr->m_oDefRunProperty.GetPointer());
		//odf_context()->text_context()->set_parent_span_style(odf_context()->styles_context()->last_state().get_name());
	}

}
void OoxConverter::convert(OOX::Drawing::CLineSpacing* oox_spacing,  odf::length_or_percent & length_or_percent)
{
	if (!oox_spacing)return;

	if (oox_spacing->m_oLineSpacingPercent.IsInit() && oox_spacing->m_oLineSpacingPercent->m_oVal.IsInit())
	{
		odf::percent percent = odf::percent(oox_spacing->m_oLineSpacingPercent->m_oVal->GetValue());
		length_or_percent = odf::length_or_percent(percent);
	}
	if (oox_spacing->m_oLineSpacingPoints.IsInit() && oox_spacing->m_oLineSpacingPoints->m_oVal.IsInit())
	{
		odf::length length = odf::length(oox_spacing->m_oLineSpacingPoints->m_oVal->ToCm()/100.,odf::length::cm);
		length_or_percent = odf::length_or_percent(length);
	}
}

void OoxConverter::convert(OOX::Drawing::CParagraph		*oox_paragraph)
{
	if (!oox_paragraph)return;

	bool styled = false;
	if (oox_paragraph->m_oParagraphProperty.IsInit())
	{
		odf::style_paragraph_properties	* paragraph_properties = odf_context()->text_context()->get_paragraph_properties();
													//свойства могут быть приписаны не только к параграфу, но и к самому объекту		
		if (!paragraph_properties)
		{
			odf_context()->styles_context()->create_style(L"",odf::style_family::Paragraph, true, false, -1);	
			paragraph_properties = odf_context()->styles_context()->last_state()->get_paragraph_properties();
			styled = true;
		}
		convert(oox_paragraph->m_oParagraphProperty.GetPointer(), paragraph_properties);
	}
	
	odf_context()->text_context()->start_paragraph(styled);

	for (long i=0; i< oox_paragraph->m_arrItems.size();i++)
	{
		convert(oox_paragraph->m_arrItems[i]);
	}
	odf_context()->text_context()->end_paragraph();

}

void OoxConverter::convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size)
{
	 _CP_OPT(odf::length) odf_length;

	 odf_length = odf::length(oox_font_size, odf::length::pt);
	 
	 if (odf_length)
		 odf_font_size = odf::font_size(odf_length.get());
}

}