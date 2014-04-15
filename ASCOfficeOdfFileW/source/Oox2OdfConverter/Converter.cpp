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
        impl_->convert();
    }
    void Converter::write(const std::wstring & path) const
    {
        return impl_->write(path);
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

void OoxConverter::convert(OOX::Drawing::CCustomGeometry2D *oox_cust_geom)
{
	if (!oox_cust_geom)return;

	//odf_context()->drawing_context().set_shape_type();



}

void OoxConverter::convert(OOX::Drawing::CPresetGeometry2D *oox_prst_geom)
{
	if (!oox_prst_geom)return;

	//odf_context()->drawing_context().set_shape_type();


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

} // namespace Docx2Odt