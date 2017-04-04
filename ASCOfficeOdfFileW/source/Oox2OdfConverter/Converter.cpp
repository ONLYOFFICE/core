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

#include "Oox2OdfConverter.h"

#include "Converter.h"

#include "../utils.h"

#include <boost/lexical_cast.hpp>

#include "XlsxConverter.h"
#include "DocxConverter.h"
#include "PptxConverter.h"

#include "../OdfFormat/odf_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"

#include "../../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Presentation.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramData.h"

#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/Shape.h"
#include "../../../ASCOfficePPTXFile/PPTXFormat/Logic/CxnSp.h"

#define PROGRESSEVENT_ID	0

namespace Oox2Odf
{
	static double pt2emu(double Val)
	{
		return (Val * 360000 * 2.54) / 72;
	}
    Converter::Converter(const std::wstring & path, const std::wstring  & type, const std::wstring & fontsPath,  const ProgressCallback* CallBack)
    {
		impl_ = NULL;
		
        if (type == _T("text"))			impl_ = new DocxConverter(path, CallBack);
        if (type == _T("spreadsheet"))	impl_ = new XlsxConverter(path, CallBack);
        if (type == _T("presentation"))	impl_ = new PptxConverter(path, CallBack);

        if (impl_)
            impl_->set_fonts_directory(fontsPath);
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
double OoxConverter::getSystemDPI()
{
    //При запросе системных настроек-проблема в linux без графического интерфейса.
    //Используется в GetMaxDigitSizePixels для измерения символов, можно указывать любой dpi,
    //потому что после измерения pix переводятся обратно в метрические величины.
    //Используется для конвертации картинок с процентными размерами oox->odf. Из редактора никогда не приходят относительные размеры,
    //думаю тут несущественнен dpi.
	return 96.;
}

bool  OoxConverter::UpdateProgress(long nComplete)
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

void OoxConverter::set_fonts_directory(const std::wstring &fontsPath)
{
    odf_context()->set_fonts_directory(fontsPath);
}

void OoxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	try
	{
		if (oox_unknown == NULL)return;

		switch(oox_unknown->getType())
		{
			case OOX::et_graphicFrame:
			{
				PPTX::Logic::GraphicFrame* pGraphicFrame= dynamic_cast<PPTX::Logic::GraphicFrame*>(oox_unknown);
				convert(pGraphicFrame);
			}break;
			case OOX::et_pic:
			{
				PPTX::Logic::Pic* pPic= dynamic_cast<PPTX::Logic::Pic*>(oox_unknown);
				convert(pPic);
			}break;
			case OOX::et_dgm_DiagrammParts:
			{
				PPTX::Logic::SmartArt* pDiagrammParts = dynamic_cast<PPTX::Logic::SmartArt*>(oox_unknown);
				convert(pDiagrammParts);
			}break;
			case OOX::et_a_Shape:
			case OOX::et_p_Shape:
			case OOX::et_dsp_Shape:
			case OOX::et_w_Shape:
			{
				convert(dynamic_cast<PPTX::Logic::Shape*>(oox_unknown));
			}break;
			case OOX::et_p_ShapeTree:
			case OOX::et_a_GroupShape:
			case OOX::et_w_GroupShape:
			{
				convert(dynamic_cast<PPTX::Logic::SpTree *>(oox_unknown));				
			}break;
			case OOX::et_cxnSp:
			{
				convert(dynamic_cast<PPTX::Logic::CxnSp*>(oox_unknown));
			}break;
			case OOX::et_a_prstGeom:
			{
				PPTX::Logic::PrstGeom* pPresetGeom = dynamic_cast<PPTX::Logic::PrstGeom*>(oox_unknown);
				convert(pPresetGeom);
			}break;
			case OOX::et_a_custGeom:
			{
				PPTX::Logic::CustGeom* pCustomGeom = dynamic_cast<PPTX::Logic::CustGeom*>(oox_unknown);
				convert(pCustomGeom);
			}break;
			case OOX::et_a_lnTo:
			{
				PPTX::Logic::LineTo* pLineTo = dynamic_cast<PPTX::Logic::LineTo*>(oox_unknown);
				convert(pLineTo);
			}break;
			case OOX::et_a_moveTo:
			{
				PPTX::Logic::MoveTo* pMoveTo = dynamic_cast<PPTX::Logic::MoveTo*>(oox_unknown);
				convert(pMoveTo);
			}break;
			case OOX::et_a_arcTo:
			{
				PPTX::Logic::ArcTo* pArcTo = dynamic_cast<PPTX::Logic::ArcTo*>(oox_unknown);
				convert(pArcTo);
			}break;
			case OOX::et_a_quadBezTo:
			{
				PPTX::Logic::QuadBezTo* pQuadBezierTo = dynamic_cast<PPTX::Logic::QuadBezTo*>(oox_unknown);
				convert(pQuadBezierTo);
			}break;
			case OOX::et_a_cubicBezTo:
			{
				PPTX::Logic::CubicBezTo* pCubicBezierT = dynamic_cast<PPTX::Logic::CubicBezTo*>(oox_unknown);
				convert(pCubicBezierT);
			}break;
			case OOX::et_a_close:
			{
				PPTX::Logic::Close* pClose= dynamic_cast<PPTX::Logic::Close*>(oox_unknown);
				convert(pClose);
			}break;
			case OOX::et_a_r:
			{
				PPTX::Logic::Run* pRun= dynamic_cast<PPTX::Logic::Run*>(oox_unknown);
				convert(pRun);
			}break;
			//case OOX::et_a_alphaModFix:
			//{
			//	OOX::Drawing::CAlphaModulateFixedEffect* pAlpha= dynamic_cast<OOX::Drawing::CAlphaModulateFixedEffect*>(oox_unknown);
			//	if (pAlpha)
			//	{
			//		odf_context()->drawing_context()->set_opacity(pAlpha->m_oAmt.GetValue());
			//	}
			//}break;
			case OOX::et_v_imagedata:
			{
				OOX::Vml::CImageData *vml = dynamic_cast<OOX::Vml::CImageData*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_textbox:
			{
				OOX::Vml::CTextbox *vml = dynamic_cast<OOX::Vml::CTextbox*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_background:
			{
				OOX::Vml::CBackground *vml = dynamic_cast<OOX::Vml::CBackground*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_path:
			{
				OOX::Vml::CPath *vml = dynamic_cast<OOX::Vml::CPath*>(oox_unknown);
				convert(vml);
			}break;	
				case OOX::et_v_textpath:
			{
				OOX::Vml::CTextPath *vml = dynamic_cast<OOX::Vml::CTextPath*>(oox_unknown);
				convert(vml);
			}break;	
			case OOX::et_v_fill:
			{
				OOX::Vml::CFill *vml = dynamic_cast<OOX::Vml::CFill*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_stroke:
			{
				OOX::Vml::CStroke *vml = dynamic_cast<OOX::Vml::CStroke*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_shadow:
			{
				OOX::Vml::CShadow *vml = dynamic_cast<OOX::Vml::CShadow*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_v_image:
			{
				OOX::Vml::CImage *vml = dynamic_cast<OOX::Vml::CImage*>(oox_unknown);
				convert(vml);
			}break;
			case OOX::et_wd_wrap:
			{
				OOX::VmlWord::CWrap *vml = dynamic_cast<OOX::VmlWord::CWrap*>(oox_unknown);
				convert(vml);
			}break;
			// "ненужные" элементы
			case OOX::et_w_softHyphen:
			case OOX::et_w_proofErr:
			case OOX::et_w_proofState:
			{
				//бяка
			}break;
			case OOX::et_p_r:
			{
				PPTX::Logic::Run* pRun= dynamic_cast<PPTX::Logic::Run*>(oox_unknown);
				convert(pRun);
			}break;
			case OOX::et_p_fld:
			{
				PPTX::Logic::Fld* pFld= dynamic_cast<PPTX::Logic::Fld*>(oox_unknown);
				convert(pFld);
			}break;
			case OOX::et_p_br:
			{
				PPTX::Logic::Br* pBr= dynamic_cast<PPTX::Logic::Br*>(oox_unknown);
				convert(pBr);
			}break;
			case OOX::et_p_MathPara:
			{
				PPTX::Logic::MathParaWrapper* pMathPara= dynamic_cast<PPTX::Logic::MathParaWrapper*>(oox_unknown);
				convert(pMathPara);
			}break;
			default:
			{
                _CP_LOG << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
			}
		}
	}catch(...)
	{
        _CP_LOG << L"[error] :  no convert element(" << (oox_unknown ? oox_unknown->getType() : -1 ) << L")\n";
	}
}

//void OoxConverter::convert(OOX::Drawing::CLockedCanvas  *oox_canvas)
//{
//	if (oox_canvas == NULL)return;
//
//	odf_context()->drawing_context()->start_group();
//		if (oox_canvas->m_oNvGroupSpPr.IsInit() && oox_canvas->m_oNvGroupSpPr->m_oCNvPr.IsInit())
//		{	
//			if (oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_sName.IsInit())
//				odf_context()->drawing_context()->set_group_name(*oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_sName);
//			if (oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_oId.IsInit())
//				odf_context()->drawing_context()->set_group_z_order(oox_canvas->m_oNvGroupSpPr->m_oCNvPr->m_oId->GetValue());
//		}
//		convert(oox_canvas->m_oGroupSpPr.GetPointer());
//		convert(oox_canvas->m_oSpPr.GetPointer());
//
//		for (size_t i = 0; i < oox_canvas->m_arrItems.size(); i++)
//		{
//			convert(oox_canvas->m_arrItems[i]);
//		}
//	odf_context()->drawing_context()->end_group();
//}
////-----------------------------------------------------------------------------------------------------------------
//void OoxConverter::convert(OOX::Drawing::CEffectList *oox_effect_list, std::wstring *change_sheme_color)
//{
//	if (oox_effect_list == NULL) return;
//
//	for (size_t i = 0; i < oox_effect_list->m_arrEffects.size(); i++)
//	{
//		switch(oox_effect_list->m_arrEffects[i]->getType())
//		{
//		case OOX::et_a_outerShdw:
//			{
//				OOX::Drawing::COuterShadowEffect * shadow = dynamic_cast<OOX::Drawing::COuterShadowEffect *>(oox_effect_list->m_arrEffects[i]);
//				convert(shadow, change_sheme_color);
//			}break;
//		case OOX::et_a_innerShdw:
//			{
//				OOX::Drawing::CInnerShadowEffect * shadow = dynamic_cast<OOX::Drawing::CInnerShadowEffect *>(oox_effect_list->m_arrEffects[i]);
//				convert(shadow, change_sheme_color);
//			}break;
//		case OOX::et_a_prstShdw:
//			{
//				OOX::Drawing::CPresetShadowEffect * shadow = dynamic_cast<OOX::Drawing::CPresetShadowEffect *>(oox_effect_list->m_arrEffects[i]);
//				//convert(shadow, change_sheme_color);
//			}break;
//		case OOX::et_a_reflection:
//			{
//				OOX::Drawing::CReflectionEffect * refelection = dynamic_cast<OOX::Drawing::CReflectionEffect *>(oox_effect_list->m_arrEffects[i]);
//			}
//			//CGlowEffect, CBlurEffect, CFillOverlayEffect, CSoftEdgesEffect
//		}
//	}
//}
//void OoxConverter::convert(OOX::Drawing::COuterShadowEffect *oox_shadow, std::wstring *change_sheme_color)
//{
//	if (oox_shadow == NULL) return; 
//
//	std::wstring hexColor;
//	_CP_OPT(double) opacity;
//
//    std::wstring keep_sheme_color;
//	if (oox_shadow->m_eType == OOX::Drawing::colorSheme && change_sheme_color)
//	{
//		keep_sheme_color = oox_shadow->m_oShemeClr.m_oVal.ToString();
//		oox_shadow->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
//	}
//	convert(dynamic_cast<OOX::Drawing::CColor*>(oox_shadow), hexColor, opacity);
//    if (keep_sheme_color.length() > 0)	oox_shadow->m_oShemeClr.m_oVal.FromString(keep_sheme_color);
//
//	odf_context()->drawing_context()->set_shadow(1, hexColor, opacity, oox_shadow->m_oDist.ToPoints());
//
//			//SimpleTypes::CPositiveFixedAngle<0>                      m_oDir;
//			//SimpleTypes::CPositiveCoordinate<0>                      m_oBlurRad;
////-------------
//	//SimpleTypes::CRectAlignment<SimpleTypes::rectalignmentB> m_oAlgn;
//	//SimpleTypes::CFixedAngle<>                               m_oKx;
//	//SimpleTypes::CFixedAngle<>                               m_oKy;
//	//SimpleTypes::COnOff<SimpleTypes::onoffTrue>              m_oRotWithShape;
//	//SimpleTypes::CPercentage                                 m_oSx;
//	//SimpleTypes::CPercentage                                 m_oSy;
//
//}
//void OoxConverter::convert(OOX::Drawing::CInnerShadowEffect *oox_shadow, std::wstring *change_sheme_color)
//{
//	if (oox_shadow == NULL) return;
//
//	std::wstring hexColor;
//	_CP_OPT(double) opacity;
//
//    std::wstring keep_sheme_color;
//	if (oox_shadow->m_eType == OOX::Drawing::colorSheme && change_sheme_color)
//	{
//		keep_sheme_color = oox_shadow->m_oShemeClr.m_oVal.ToString();
//		oox_shadow->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
//	}
//	convert(dynamic_cast<OOX::Drawing::CColor*>(oox_shadow), hexColor, opacity);
//    if (keep_sheme_color.length() > 0)	oox_shadow->m_oShemeClr.m_oVal.FromString(keep_sheme_color);
//
//	odf_context()->drawing_context()->set_shadow(2, hexColor, opacity, oox_shadow->m_oDist.ToPoints());
//
//}
//void OoxConverter::convert(OOX::Drawing::CPatternFillProperties *oox_pattern_fill,	std::wstring *change_sheme_color)
//{
//	if (!oox_pattern_fill)return;
//	
//	odf_context()->drawing_context()->start_hatch_style();
//	{
//		if (oox_pattern_fill->m_oPrst.IsInit())
//		{
//			odf_context()->drawing_context()->set_hatch_type(oox_pattern_fill->m_oPrst->GetValue());
//		}			
//		if (oox_pattern_fill->m_oFgClr.IsInit())
//		{
//			std::wstring hexColor;
//			_CP_OPT(double) opacity;
//			convert((OOX::Drawing::CColor*)oox_pattern_fill->m_oFgClr.GetPointer(),hexColor, opacity);
//
//			odf_context()->drawing_context()->set_hatch_line_color(hexColor);
//		}
//		if (oox_pattern_fill->m_oBgClr.IsInit())
//		{
//			if (change_sheme_color && oox_pattern_fill->m_oBgClr.IsInit() && oox_pattern_fill->m_oBgClr->m_eType == OOX::Drawing::colorSheme)
//				oox_pattern_fill->m_oBgClr->m_oShemeClr.m_oVal.FromString(*change_sheme_color);
//
//			std::wstring hexColor;
//			_CP_OPT(double) opacity;
//			convert((OOX::Drawing::CColor*)oox_pattern_fill->m_oBgClr.GetPointer(),hexColor, opacity);
//
//			odf_context()->drawing_context()->set_hatch_area_color(hexColor);
//		}
//	}
//	odf_context()->drawing_context()->end_hatch_style();
//
//}

bool OoxConverter::convert(std::wstring sSchemeColor, DWORD & argb)
{
	PPTX::Theme * theme= oox_theme();
	if (!theme)return false;

	std::map<std::wstring, PPTX::Logic::UniColor>::iterator pFind = theme->themeElements.clrScheme.Scheme.find(sSchemeColor);

	if (pFind != theme->themeElements.clrScheme.Scheme.end())
	{
		PPTX::Logic::UniColor & color = pFind->second;
		
		argb = color.GetARGB(); 

		return true;
	}
	return false;
}

void OoxConverter::convert(double oox_font_size,  _CP_OPT(odf_types::font_size) & odf_font_size)
{
	 _CP_OPT(odf_types::length) odf_length;

	 odf_length = odf_types::length(oox_font_size, odf_types::length::pt);
	 
	 if (odf_length)
		 odf_font_size = odf_types::font_size(odf_length.get());
}

}
