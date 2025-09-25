/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
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
#pragma once
#include "PptFormula.h"
#include "../VmlPath.h"
#include "../PPTShape/PptShape.h"
#include "../PPTXShape/PptxShape.h"

#include "../../../../OOXML/Binary/Presentation/XmlWriter.h"

namespace PPTX2EditorAdvanced
{
    OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::wstring& str);
}

namespace NSGuidesVML
{
static int __wstrlen(const wchar_t* str)
{
	const wchar_t* s = str;
	for (; *s != 0; ++s);
	return (LONG)(s - str);
}

static int		g_guide_max_len	= 80;

static wchar_t*	g_guide_string1			= L"<a:gd name=\"gd";
static int		g_guide_string1_len		= __wstrlen(g_guide_string1);

static wchar_t*	g_guide_string2			= L"\"/>";
static int		g_guide_string2_len		= __wstrlen(g_guide_string2);

static wchar_t*	g_guide_string_val		= L"\" fmla=\"val ";
static int		g_guide_string_val_len	= __wstrlen(g_guide_string_val);

static wchar_t*	g_guide_string_sum		= L"\" fmla=\"+- ";
static int		g_guide_string_sum_len	= __wstrlen(g_guide_string_sum);

static wchar_t*	g_guide_string_prod		= L"\" fmla=\"*/ ";
static int		g_guide_string_prod_len	= __wstrlen(g_guide_string_prod);

static wchar_t*	g_guide_string_abs		= L"\" fmla=\"abs ";
static int		g_guide_string_abs_len	= __wstrlen(g_guide_string_abs);

static wchar_t*	g_guide_string_min		= L"\" fmla=\"min ";
static int		g_guide_string_min_len	= __wstrlen(g_guide_string_min);

static wchar_t*	g_guide_string_max		= L"\" fmla=\"max ";
static int		g_guide_string_max_len	= __wstrlen(g_guide_string_max);

static wchar_t*	g_guide_string__if		= L"\" fmla=\"?: ";
static int		g_guide_string__if_len	= __wstrlen(g_guide_string__if);

static wchar_t*	g_guide_string_sqrt		= L"\" fmla=\"sqrt ";
static int		g_guide_string_sqrt_len	= __wstrlen(g_guide_string_sqrt);

static wchar_t*	g_guide_string_at2		= L"\" fmla=\"at2 ";
static int		g_guide_string_at2_len	= __wstrlen(g_guide_string_at2);

static wchar_t*	g_guide_string_sin		= L"\" fmla=\"sin ";
static int		g_guide_string_sin_len	= __wstrlen(g_guide_string_sin);

static wchar_t*	g_guide_string_cos		= L"\" fmla=\"cos ";
static int		g_guide_string_cos_len	= __wstrlen(g_guide_string_cos);

static wchar_t*	g_guide_string_cat2		= L"\" fmla=\"cat2 ";
static int		g_guide_string_cat2_len	= __wstrlen(g_guide_string_cat2);

static wchar_t*	g_guide_string_sat2		= L"\" fmla=\"sat2 ";
static int		g_guide_string_sat2_len	= __wstrlen(g_guide_string_sat2);

static wchar_t*	g_guide_string_mod		= L"\" fmla=\"mod ";
static int		g_guide_string_mod_len	= __wstrlen(g_guide_string_mod);

static wchar_t*	g_guide_string_tag		= L"\" fmla=\"tag ";
static int		g_guide_string_tag_len	= __wstrlen(g_guide_string_tag);

#define GUIDE_PARAM_1(name)																		\
{																								\
    oWriter.m_oWriter.AddSize(g_guide_max_len);													\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string1, g_guide_string1_len);					\
    oWriter.m_oWriter.AddIntNoCheck(m_lIndexDst);												\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string_##name, g_guide_string_##name##_len);	\
    GetValue(lParam1, eType1, bExtShape1, oWriter);												\
    ++m_lIndexDst;																				\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string2, g_guide_string2_len);					\
}

#define GUIDE_PARAM_2(name)																		\
{																								\
    oWriter.m_oWriter.AddSize(g_guide_max_len);													\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string1, g_guide_string1_len);					\
    oWriter.m_oWriter.AddIntNoCheck(m_lIndexDst);												\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string_##name, g_guide_string_##name##_len);	\
    GetValue(lParam1, eType1, bExtShape1, oWriter);												\
    oWriter.m_oWriter.AddSpaceNoCheck();														\
    GetValue(lParam2, eType2, bExtShape2, oWriter);												\
    ++m_lIndexDst;																				\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string2, g_guide_string2_len);					\
}

#define GUIDE_PARAM_3(name)																		\
{																								\
    oWriter.m_oWriter.AddSize(g_guide_max_len);													\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string1, g_guide_string1_len);					\
    oWriter.m_oWriter.AddIntNoCheck(m_lIndexDst);												\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string_##name, g_guide_string_##name##_len);	\
    GetValue(lParam1, eType1, bExtShape1, oWriter);												\
    oWriter.m_oWriter.AddSpaceNoCheck();														\
    GetValue(lParam2, eType2, bExtShape2, oWriter);												\
    oWriter.m_oWriter.AddSpaceNoCheck();														\
    GetValue(lParam3, eType3, bExtShape3, oWriter);												\
    ++m_lIndexDst;																				\
	oWriter.m_oWriter.WriteStringNoSafe(g_guide_string2, g_guide_string2_len);					\
}

struct CFormParam
{
    LONG m_lParam;
    ParamType m_eType;
    LONG m_lCoef;
};

class CSlicePath
{
private:
    int		m_nCountElementsPoint;
    LONG	m_lX;
    LONG	m_lY;

public:
    RulesType m_eRuler;
    std::vector<Aggplus::POINT> m_arPoints;
    std::vector<SPointType> m_arPointsType;

	CSlicePath(RulesType rule = rtMoveTo, LONG x = 0, LONG y = 0);

	void AddParam(LONG lParam, ParamType eParType);
};

class CFormulaConverter
{
private:
    int m_lIndexSrc;
    int m_lIndexDst;

    LONG m_lX;
    LONG m_lY;

    std::vector<LONG>		m_arIndexDst;
    std::vector<CSlicePath> m_arSlicesPath;
    std::vector<CPartPath>	m_arParts;

    std::wstring		strFmlaNum;
    std::wstring		strSign;
    std::wstring		strFrmla;
    std::wstring		strResult;
    Aggplus::POINT		pCurPoint;
    SPointType			pCurPointType;
    Aggplus::POINT		pCurPoint1;
    SPointType			pCurPointType1;
    Aggplus::POINT		pTmpPoint;
    CFormParam			m_oParam;

    int m_lMaxAdjUse;

public:
    LONG m_lWidth;
    LONG m_lHeight;
    
    // все в одно не получится, формулы появляются и при конвертации path/adj и т.д.
    NSBinPptxRW::CXmlWriter m_oGuidsRes;
    NSBinPptxRW::CXmlWriter m_oPathRes;
    NSBinPptxRW::CXmlWriter m_oHandleRes;
    NSBinPptxRW::CXmlWriter m_oAdjRes;
    NSBinPptxRW::CXmlWriter m_oTextRect;
    NSBinPptxRW::CXmlWriter m_oCoef;

	CFormulaConverter();

	void ConvertCoef(NSGuidesVML::CFormParam pCoef);
	void ConvertCoef2(NSGuidesVML::CFormParam pCoef);
	void ConvertAdj(const std::vector<LONG>& arAdj);
	void ConvertFormula(const std::vector<CFormula>& arFormulas);
	void ConvertPath(std::wstring strPath, const ODRAW::CPath& oPath);
	void ConvertHandle(const std::vector<CHandle_>& arHandles, std::vector<long>& arAdj, PPTShapes::ShapeType oSType);

	void SetTextRectDefault();
	void ConvertTextRect ( std::wstring strRect);

private:
	void GetValue(const LONG& lParam, const ParamType& eParamType, const bool& bExtShape, NSBinPptxRW::CXmlWriter& oWriter);
	std::wstring GetValue2(const LONG& lParam, const ParamType& eParamType, const bool& bExtShape);

	//---------------------------------------

	void ConvertVal(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertSum(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertProd(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

	void ConvertAbs(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertMin(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertMax (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertIf(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
				   const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

	void ConvertSqrt(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertAt2 (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertSin (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertCos (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertCat2(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertSat2(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertMod(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter);

    void ConvertTag (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
					 const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter);

	void ConvertProd(const std::wstring& strParam1, const std::wstring& strParam2, const LONG& lParam3, NSBinPptxRW::CXmlWriter& oWriter);

    //-------------------------------------
	void FromXML(std::wstring strPath, bool &m_bFill, bool &m_bStroke);
	void CreateHandle ( SHandle oHnd );

	LONG GetHandlePos(const std::wstring& strParam, const std::wstring& strBase, LONG lSize);
	LONG GetHandleValue(const std::wstring& strParam, const LONG& lVal, ParamType& ptType);

	void ConvertQuadrX(Aggplus::POINT pPoint, SPointType pPointType);
	void ConvertQuadrY(Aggplus::POINT pPoint, SPointType pPointType);
    //-------------------------------------

	void ConvertSlice_MoveTo(const CSlicePath& oSlice);
	void ConvertSlice_RMoveTo(const CSlicePath& oSlice);
	void ConvertSlice_LineTo(const CSlicePath& oSlice);
	void ConvertSlice_RLineTo(const CSlicePath& oSlice);
	void ConvertSlice_ArcTo(const CSlicePath& oSlice);
	void ConvertSlice_ClockwiseTo(const CSlicePath& oSlice);
	void ConvertSlice_QuadrBesier(const CSlicePath& oSlice);
	void ConvertSlice_CurveTo(const CSlicePath& oSlice);
	void ConvertSlice_RCurveTo(const CSlicePath& oSlice);
	void ConvertSlice_AngleEllipse(const CSlicePath& oSlice);
	void ConvertSlice_EllipticalQuadrX(const CSlicePath& oSlice);
	void ConvertSlice_EllipticalQuadrY(const CSlicePath& oSlice);
};
}
