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
	//// Attributes
	//nullable<SimpleTypes::CBlackWhiteMode<>>          m_oBwMode;

	//// Childs
	//EFillType                                         m_eFillType;   // Тип заливки
	//nullable<OOX::Drawing::CBlipFillProperties>       m_oBlipFill;
	//nullable<OOX::Drawing::CGradientFillProperties>   m_oGradFill;
	//nullable<OOX::Drawing::CGroupFillProperties>      m_oGrpFill;
	//nullable<OOX::Drawing::CNoFillProperties>         m_oNoFill;
	//nullable<OOX::Drawing::CPatternFillProperties>    m_oPattFill;
	//nullable<OOX::Drawing::CSolidColorFillProperties> m_oSolidFill;

	//EGeomType                                         m_eGeomType;   // Тип графики
	//nullable<OOX::Drawing::CCustomGeometry2D>         m_oCustGeom;
	//nullable<OOX::Drawing::CPresetGeometry2D>         m_oPrstGeom;

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


} // namespace Docx2Odt