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
	if (odf_context() == NULL) return;

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
				convert(dynamic_cast<PPTX::Logic::GraphicFrame*>(oox_unknown));
			}break;
			case OOX::et_pic:
			{
				convert(dynamic_cast<PPTX::Logic::Pic*>(oox_unknown));
			}break;
			case OOX::et_dgm_DiagrammParts:
			{
				convert(dynamic_cast<PPTX::Logic::SmartArt*>(oox_unknown));
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
				convert(dynamic_cast<PPTX::Logic::PrstGeom*>(oox_unknown));
			}break;
			case OOX::et_a_custGeom:
			{
				convert(dynamic_cast<PPTX::Logic::CustGeom*>(oox_unknown));
			}break;
			case OOX::et_a_ahXY:
			{
				convert(dynamic_cast<PPTX::Logic::AhXY*>(oox_unknown));
			}break;
			case OOX::et_a_ahPolar:
			{
				convert(dynamic_cast<PPTX::Logic::AhPolar*>(oox_unknown));
			}break;
			case OOX::et_a_lnTo:
			{
				convert(dynamic_cast<PPTX::Logic::LineTo*>(oox_unknown));
			}break;
			case OOX::et_a_moveTo:
			{
				convert(dynamic_cast<PPTX::Logic::MoveTo*>(oox_unknown));
			}break;
			case OOX::et_a_arcTo:
			{
				convert(dynamic_cast<PPTX::Logic::ArcTo*>(oox_unknown));
			}break;
			case OOX::et_a_quadBezTo:
			{
				convert(dynamic_cast<PPTX::Logic::QuadBezTo*>(oox_unknown));
			}break;
			case OOX::et_a_cubicBezTo:
			{
				convert(dynamic_cast<PPTX::Logic::CubicBezTo*>(oox_unknown));
			}break;
			case OOX::et_a_close:
			{
				convert(dynamic_cast<PPTX::Logic::Close*>(oox_unknown));
			}break;
			case OOX::et_p_r:
			case OOX::et_a_r:
			{
				convert(dynamic_cast<PPTX::Logic::Run*>(oox_unknown));
			}break;
			case OOX::et_p_br:
			case OOX::et_a_br:
			{
				convert(dynamic_cast<PPTX::Logic::Br*>(oox_unknown));
			}break;
			case OOX::et_a_fld:
			case OOX::et_p_fld:
			{
				convert(dynamic_cast<PPTX::Logic::Fld*>(oox_unknown));
			}break;
			case OOX::et_p_MathPara:
			{
				convert(dynamic_cast<PPTX::Logic::MathParaWrapper*>(oox_unknown));
			}break;
			case OOX::et_a_effectStyle:
			{
				convert(dynamic_cast<PPTX::Logic::EffectStyle*>(oox_unknown));
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
				convert(dynamic_cast<OOX::Vml::CImageData*>(oox_unknown));
			}break;
			case OOX::et_v_textbox:
			{
				convert(dynamic_cast<OOX::Vml::CTextbox*>(oox_unknown));
			}break;
			case OOX::et_v_background:
			{
				convert(dynamic_cast<OOX::Vml::CBackground*>(oox_unknown));
			}break;
			case OOX::et_v_path:
			{
				convert(dynamic_cast<OOX::Vml::CPath*>(oox_unknown));
			}break;	
				case OOX::et_v_textpath:
			{
				convert(dynamic_cast<OOX::Vml::CTextPath*>(oox_unknown));
			}break;	
			case OOX::et_v_fill:
			{
				convert(dynamic_cast<OOX::Vml::CFill*>(oox_unknown));
			}break;
			case OOX::et_v_stroke:
			{
				convert(dynamic_cast<OOX::Vml::CStroke*>(oox_unknown));
			}break;
			case OOX::et_v_formulas:
			{
				convert(dynamic_cast<OOX::Vml::CFormulas*>(oox_unknown));
			}break;
			case OOX::et_v_shadow:
			{
				convert(dynamic_cast<OOX::Vml::CShadow*>(oox_unknown));
			}break;
			case OOX::et_v_image:
			{
				convert(dynamic_cast<OOX::Vml::CImage*>(oox_unknown));
			}break;
			case OOX::et_wd_wrap:
			{
				convert(dynamic_cast<OOX::VmlWord::CWrap*>(oox_unknown));
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
                _CP_LOG << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
			}
		}
	}catch(...)
	{
        _CP_LOG << L"[error] :  no convert element(" << (oox_unknown ? oox_unknown->getType() : -1 ) << L")\n";
	}
}
std::wstring OoxConverter::find_link_by (smart_ptr<OOX::File> & oFile, int type)
{
	if (!oFile.IsInit()) return L"";
	
    std::wstring ref;
	if (type == 1 && OOX::FileTypes::Image == oFile->type())
	{
		OOX::Image* pImage = dynamic_cast<OOX::Image*>(oFile.operator->());

		if (pImage)
			ref = pImage->filename().GetPath();
	}
	if (type == 2 && OOX::FileTypes::HyperLink == oFile->type())
	{
		OOX::HyperLink* pHyperlink = dynamic_cast<OOX::HyperLink*>(oFile.operator->());
		
		if (pHyperlink && pHyperlink->bHyperlink)
			ref = pHyperlink->Uri().GetPath();
	}
	if (type == 3)
	{
		OOX::Media* pMedia = dynamic_cast<OOX::Media*>(oFile.operator->());

		if (pMedia)
			ref = pMedia->filename().GetPath();
	}
	return ref;
}

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
