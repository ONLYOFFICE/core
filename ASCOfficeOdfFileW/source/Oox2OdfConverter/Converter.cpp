#pragma once
#include "stdAfx.h"

#include "Converter.h"

#include "XlsxConverter.h"
#include "DocxConverter.h"

#include "odf_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

namespace Oox2Odf
{
	static double pt2emu(double Val)
	{
		return (Val * 360000 * 2.54) / 72;
	}
	Converter::Converter(const std::wstring & path) 
    {
		std::wstring  type = L"spreadsheet";

		if (type == L"text")			impl_ = new DocxConverter(path);
		if (type == L"spreadsheet")		impl_ = new XlsxConverter(path);
	}

	Converter::~Converter() 
    {
		if (impl_ )delete impl_ ;
	}
    void Converter::convert()
    {
		if (!impl_)return;
        impl_->convertDocument();
    }
    void Converter::write(const std::wstring & path) const
    {
        return impl_->write(path);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
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
		default:
		{
			std::wstringstream ss;
			ss << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
			_CP_LOG(error) << ss.str();
		}
	}
}

void OoxConverter::convert_SpPr(OOX::Drawing::CShapeProperties *   oox_spPr)
{
	if (!oox_spPr) return;

	if (oox_spPr->m_oXfrm.IsInit())//CTransform2D
	{
		//размеры продублированы

		if (oox_spPr->m_oXfrm->m_oFlipH.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context().set_flip_H(true);
		if (oox_spPr->m_oXfrm->m_oFlipV.GetValue() == SimpleTypes::onoffTrue)
			odf_context()->drawing_context().set_flip_V(true);
		if (oox_spPr->m_oXfrm->m_oRot.GetValue() > 0)
			odf_context()->drawing_context().set_rotate(oox_spPr->m_oXfrm->m_oRot.GetValue());
	}

	switch (oox_spPr->m_eFillType)
	{
		case OOX::Drawing::filltypeBlip:
			if(oox_spPr->m_oBlipFill.IsInit())convert(oox_spPr->m_oBlipFill.GetPointer());break;
		case OOX::Drawing::filltypeGradient:
			if(oox_spPr->m_oGradFill.IsInit())convert(oox_spPr->m_oGradFill.GetPointer());break;
		case OOX::Drawing::filltypePattern:
			if(oox_spPr->m_oPattFill.IsInit())convert(oox_spPr->m_oPattFill.GetPointer());break;
		case OOX::Drawing::filltypeSolid:
			if(oox_spPr->m_oSolidFill.IsInit())convert(oox_spPr->m_oSolidFill.GetPointer());break;
		case OOX::Drawing::filltypeGroup:
		case OOX::Drawing::filltypeNo:
			odf_context()->drawing_context().set_no_fill();
			break;
		case OOX::Drawing::filltypeUnknown:
		default: break;
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
	//EEffectType                                       m_eEffectType; // Тип контейнера эффектов
	//nullable<OOX::Drawing::CEffectContainer>          m_oEffectDag;
	//nullable<OOX::Drawing::CEffectList>               m_oEffectList;

	//nullable<OOX::Drawing::CLineProperties>           m_oLn;
	//nullable<OOX::Drawing::COfficeArtExtensionList>   m_oExtLst;
	//nullable<OOX::Drawing::CScene3D>                  m_oScene3D;
	//nullable<OOX::Drawing::CShape3D>                  m_oSp3D;

}
void OoxConverter::convert_CNvPr(OOX::Drawing::CNonVisualDrawingProps * oox_cnvPr)
{
	if (!oox_cnvPr) return;

	if (oox_cnvPr->m_sName.IsInit())
	{
		std::wstring name = string2std_string(oox_cnvPr->m_sName.get());
		odf_context()->drawing_context().set_name(name);
	}
	if (oox_cnvPr->m_oId.IsInit())
	{
		int id =oox_cnvPr->m_oId->GetValue();
		odf_context()->drawing_context().set_z_order(id);
	}
	//nullable<CString>                               m_sDescr;
	//nullable<SimpleTypes::COnOff<>>                 m_oHidden;
	//nullable<CString>                               m_sTitle;
}

void OoxConverter::convert/*_CustGeom*/(OOX::Drawing::CCustomGeometry2D *oox_cust_geom)
{
	if (!oox_cust_geom)return;

	for (long i=0; i< oox_cust_geom->m_oPthLst.m_arrPath.GetSize();i++)
	{
		convert(&oox_cust_geom->m_oPthLst.m_arrPath[i]);
	}
}

void OoxConverter::convert/*_PrstGeom*/(OOX::Drawing::CPresetGeometry2D *oox_prst_geom)
{
	if (!oox_prst_geom)return;

	//

}
void OoxConverter::convert(OOX::Drawing::CPath2D *oox_geom_path)
{
	if (!oox_geom_path) return;

	odf_context()->drawing_context().set_viewBox(oox_geom_path->m_oW.GetValue(), oox_geom_path->m_oH.GetValue());

	for (long i =0 ; i< oox_geom_path->m_arrItems.GetSize(); i++)
	{
		convert(oox_geom_path->m_arrItems[i]);
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
void OoxConverter::convert(OOX::Drawing::CBlipFillProperties *oox_bitmap_fill)
{
	if (!oox_bitmap_fill)return;
}

void OoxConverter::convert(OOX::Drawing::CGradientFillProperties *oox_grad_fill)
{
	if (!oox_grad_fill)return;
}
void OoxConverter::convert(OOX::Drawing::CPatternFillProperties *oox_pattern_fill)
{
	if (!oox_pattern_fill)return;
}
void OoxConverter::convert(OOX::Drawing::CSolidColorFillProperties *oox_solid_fill)
{
	if (!oox_solid_fill)return;
}
void OoxConverter::convert(OOX::Drawing::CPath2DLineTo *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm =				boost::lexical_cast<std::wstring> ( (int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
					std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));
	
	odf_context()->drawing_context().add_path_element(std::wstring(L"L"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DMoveTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm =						boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
							std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));
	
	odf_context()->drawing_context().add_path_element(std::wstring(L"M"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DArcTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	//std::wstring path_elm =						boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oX.GetValue())) + 
	//						std::wstring(L" ")+ boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oPt.m_oY.GetValue()));
	//
	//odf_context()->drawing_context().add_path_element(std::wstring(L"A"), path_elm);
}
void OoxConverter::convert(OOX::Drawing::CPath2DQuadBezierTo *oox_geom_path)
{
	if (!oox_geom_path) return;
	
	std::wstring path_elm =							boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl.m_oX.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oCtrl.m_oY.GetValue())) + 
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oX.GetValue())) +
							std::wstring(L" ") +	boost::lexical_cast<std::wstring> ((int)pt2emu(oox_geom_path->m_oEnd.m_oY.GetValue())) ;
	
	odf_context()->drawing_context().add_path_element(std::wstring(L"S"), path_elm);

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
	
	odf_context()->drawing_context().add_path_element(std::wstring(L"C"), path_elm);


}
void OoxConverter::convert(OOX::Drawing::CPath2DClose *oox_geom_path)
{
	if (!oox_geom_path) return;

	std::wstring path_elm ;
	
	odf_context()->drawing_context().add_path_element(std::wstring(L"N"), path_elm);

}
} // namespace Docx2Odt