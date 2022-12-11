/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#include "../Path.h"
#include "../PPTXShape/PptxShape.h"

#include "../../../../OOXML/Binary/Presentation/XmlWriter.h"

namespace PPTX2EditorAdvanced
{
    OOXMLShapes::ShapeType GetShapeTypeFromStr(const std::wstring& str);
}

namespace NSGuidesVML
{
AVSINLINE int __wstrlen(const wchar_t* str)
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

    CSlicePath(RulesType rule = rtMoveTo, LONG x = 0, LONG y = 0) : m_lX(x), m_lY(y), m_eRuler(rule)
    {
        m_nCountElementsPoint = 0;
    }

    void AddParam(LONG lParam, ParamType eParType)
    {
        long lPoint = m_nCountElementsPoint % 2;
        if (0 == lPoint)
        {
            Aggplus::POINT point;
            SPointType pointType;

            point.x = lParam;
            if (m_eRuler !=  rtRMoveTo && m_eRuler !=  rtRLineTo && m_eRuler !=  rtRCurveTo)
            {
                point.x -= m_lX;
            }
            point.y = 0;
            pointType.x = eParType;
            pointType.y = ptValue;
            m_arPoints.push_back(point);
            m_arPointsType.push_back(pointType);
        }
        else
        {
            m_arPoints.back().y = lParam;
            if (m_eRuler !=  rtRMoveTo && m_eRuler !=  rtRLineTo && m_eRuler !=  rtRCurveTo)
            {
                m_arPoints.back().y -= m_lY;
            }

            m_arPointsType.back().y = eParType;
        }
        ++m_nCountElementsPoint;
    }
};

class CFormulaConverter
{
private:
    int m_lIndexSrc;
    int m_lIndexDst;

    LONG m_lWidth;
    LONG m_lHeight;

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
    // все в одно не получится, формулы появляются и при конвертации path/adj и т.д.
    NSBinPptxRW::CXmlWriter m_oGuidsRes;
    NSBinPptxRW::CXmlWriter m_oPathRes;
    NSBinPptxRW::CXmlWriter m_oHandleRes;
    NSBinPptxRW::CXmlWriter m_oAdjRes;
    NSBinPptxRW::CXmlWriter m_oTextRect;
    NSBinPptxRW::CXmlWriter m_oCoef;

    CFormulaConverter()
    {
        m_lIndexDst = 0;
        m_lIndexSrc = -1;

        m_lWidth	= 0;
        m_lHeight	= 0;

        m_lX = 0;
        m_lY = 0;

        m_lMaxAdjUse = -1;
    }
    void ConvertCoef(NSGuidesVML::CFormParam pCoef)
    {
        if (pCoef.m_eType == ptValue)
            ConvertVal(pCoef.m_lParam, pCoef.m_eType, false, m_oCoef);
        else if (pCoef.m_eType == ptFormula)
        {
            ConvertProd(22, ptValue, 1, ptValue, 7, ptValue, false, false, false, m_oCoef);
            ConvertProd(m_lIndexDst-1, ptFormula, 1, ptValue, 180, ptValue, true, false, false, m_oCoef);
        }
        m_oParam.m_lParam	= m_lIndexDst-1;
        m_oParam.m_eType	= ptFormula;
        m_oParam.m_lCoef	= pCoef.m_lCoef;
    }
    void ConvertCoef2(NSGuidesVML::CFormParam pCoef)
    {
        if (pCoef.m_eType == ptValue)
            ConvertVal(pCoef.m_lParam, pCoef.m_eType, false, m_oGuidsRes);
        else if (pCoef.m_eType == ptFormula)
        {
            ConvertProd(22, ptValue, 1, ptValue, 7, ptValue, false, false, false, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, 1, ptValue, 180, ptValue, true, false, false, m_oGuidsRes);
        }
        m_oParam.m_lParam	= m_lIndexDst-1;
        m_oParam.m_eType	= ptFormula;
        m_oParam.m_lCoef	= pCoef.m_lCoef;
    }
    void ConvertAdj(const std::vector<LONG>& arAdj)
    {
        for (int i = 0; i < (int)arAdj.size(); ++i)
        {
            m_oAdjRes.WriteString(L"<a:gd name=\"adj");
            m_oAdjRes.WriteINT(i);
            m_oAdjRes.WriteString(L"\" fmla=\"val ");
            m_oAdjRes.WriteINT(arAdj[i]);
            m_oAdjRes.WriteString(L"\"/>");
        }
        for (int i = (int)arAdj.size(); i <= m_lMaxAdjUse; ++i)
        {
            m_oAdjRes.WriteString(L"<a:gd name=\"adj");
            m_oAdjRes.WriteINT(i);
            m_oAdjRes.WriteString(L"\" fmla=\"val ");
            m_oAdjRes.WriteINT(0);
            m_oAdjRes.WriteString(L"\"/>");
        }
    }
    void ConvertFormula(const std::vector<CFormula>& arFormulas)
    {
        for (size_t i = 0; i < arFormulas.size(); ++i)
        {
            const CFormula& pFormula = arFormulas[i];
            m_lIndexSrc++;

            switch (pFormula.m_eFormulaType)
            {
            case ftVal:
            {
                ConvertVal(pFormula.m_lParam1, pFormula.m_eType1, false, m_oGuidsRes);
                break;
            }
            case ftSum:
            {
                ConvertSum(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                break;
            }
            case ftProduct:
            {
                ConvertProd(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                break;
            }
            case ftMid:
            {
                // TODO: +/ ???
                ConvertSum(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, 0, ptValue, false, false, false, m_oGuidsRes);
                ConvertProd(m_lIndexDst-1, ptFormula, 1, ptValue, 2, ptValue, true, false, false, m_oGuidsRes);
                break;
            }
            case ftAbsolute:
            {
                ConvertAbs (pFormula.m_lParam1, pFormula.m_eType1, false, m_oGuidsRes);
                break;
            }
            case ftMin:
            {
                ConvertMin(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, false, false, m_oGuidsRes);
                break;
            }
            case ftMax:
            {
                ConvertMax(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, false, false, m_oGuidsRes);
                break;
            }
            case ftIf:
            {
                ConvertIf(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                break;
            }
            case ftMod:
            {
                ConvertMod(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                break;
            }
            case ftAtan2:
            {
                ConvertAt2(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, false, false, m_oGuidsRes);
                ConvertProd(m_lIndexDst-1, ptFormula, m_oParam.m_lParam, m_oParam.m_eType, pow3_16, ptValue, true, true, false, m_oGuidsRes);
                break;
            }
            case ftSin:
            {
                ConvertProd(pFormula.m_lParam2, pFormula.m_eType2, pow3_16, ptValue, m_oParam.m_lParam, m_oParam.m_eType, false, false, true, m_oGuidsRes);
                ConvertSin(pFormula.m_lParam1, pFormula.m_eType1, m_lIndexDst-1, ptFormula, false, true, m_oGuidsRes);
                break;
            }
            case ftCos:
            {
                ConvertProd(pFormula.m_lParam2, pFormula.m_eType2, pow3_16, ptValue, m_oParam.m_lParam, m_oParam.m_eType, false, false, true, m_oGuidsRes);
                ConvertCos(pFormula.m_lParam1, pFormula.m_eType1, m_lIndexDst-1, ptFormula, false, true, m_oGuidsRes);
                break;
            }
            case ftCosatan2:
            {
                ConvertCat2(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                ConvertProd(m_lIndexDst-1, ptFormula, m_oParam.m_lParam, m_oParam.m_eType, pow3_16, ptValue, true, true, false, m_oGuidsRes);
                break;
            }
            case ftSinatan2:
            {
                ConvertSat2(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam3, pFormula.m_eType3, false, false, false, m_oGuidsRes);
                ConvertProd(m_lIndexDst-1, ptFormula, m_oParam.m_lParam, m_oParam.m_eType, pow3_16, ptValue, true, true, false, m_oGuidsRes);
                break;
            }
            case ftSqrt:
            {
                ConvertSqrt(pFormula.m_lParam1, pFormula.m_eType1, false, m_oGuidsRes);
                break;
            }
            case ftTan:
            {
                ConvertTag(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam2, pFormula.m_eType2, false, false, m_oGuidsRes);
                break;
            }
            case ftSumangle:
            {
                ConvertProd(pFormula.m_lParam1, pFormula.m_eType1, pow3_16, ptValue, m_oParam.m_lParam, m_oParam.m_eType, false, false, true, m_oGuidsRes);
                ConvertProd(pow3_16, ptValue, pFormula.m_lParam2, pFormula.m_eType2, 1, ptValue, false, false, false, m_oGuidsRes);
                ConvertProd(pow3_16, ptValue, pFormula.m_lParam3, pFormula.m_eType3, 1, ptValue, false, false, false, m_oGuidsRes);
                ConvertSum(m_lIndexDst-3, ptFormula, m_lIndexDst-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);
                ConvertProd(m_lIndexDst-1, ptFormula, m_oParam.m_lParam, m_oParam.m_eType, pow3_16, ptValue, true, true, false, m_oGuidsRes);
                break;
            }
            case ftEllipse:
            {
                ConvertProd(pFormula.m_lParam1, pFormula.m_eType1, pFormula.m_lParam1, pFormula.m_eType1, 1, ptValue , false, false, false, m_oGuidsRes);
                ConvertProd(pFormula.m_lParam2, pFormula.m_eType2, pFormula.m_lParam2, pFormula.m_eType2, 1, ptValue , false, false, false, m_oGuidsRes);
                ConvertProd(1, ptValue, m_lIndexDst-2, ptFormula, m_lIndexDst-1, ptFormula, false, true, true, m_oGuidsRes);
                ConvertSum(0, ptValue, 1, ptValue, m_lIndexDst-1, ptFormula, false, false, true, m_oGuidsRes);
                ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
                ConvertProd(pFormula.m_lParam3, pFormula.m_eType3, m_lIndexDst-1, ptFormula, 1, ptValue, false, true, false, m_oGuidsRes);
                break;
            }
            default:
                break;
            }

            m_arIndexDst.push_back(m_lIndexDst-1);
        }
    }
    void ConvertPath(std::wstring strPath, const ODRAW::CPath& oPath)
    {
        m_arParts.clear();
        std::vector<std::wstring> oArray;
        boost::algorithm::split(oArray, strPath, boost::algorithm::is_any_of(L"e"),  boost::algorithm::token_compress_on);

        for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
        {
            if (oPath.m_arParts.size() <= nIndex)
                break;
            const CPartPath& oPart = oPath.m_arParts[nIndex];

            m_lWidth = oPart.width;
            m_lHeight = oPart.height;

            m_lX = oPart.x;
            m_lY = oPart.y;

            bool bFill = false;
            bool bStroke = false;
            std::wstring strValue;
            FromXML(oArray[nIndex], bFill, bStroke);

            m_oPathRes.StartNode(L"a:path");
            m_oPathRes.StartAttributes();
            m_oPathRes.WriteAttribute(L"w",  m_lWidth);
            m_oPathRes.WriteAttribute(L"h",  m_lHeight);

            if (!bStroke)
                m_oPathRes.WriteAttribute(L"stroke",  (std::wstring)L"false");
            if (!bFill)
                m_oPathRes.WriteAttribute(L"fill",  (std::wstring)L"none");
            m_oPathRes.EndAttributes();

            for (size_t i = 0; i < m_arSlicesPath.size(); ++i)
            {
                CSlicePath& oSlice = m_arSlicesPath[i]; 

                //m_lIndexDst-1 - номер последней записанной формулы
                //m_lIndexSrc - номер последнего блока формул
                //m_arIndexDst[n] - возвращает номер последней формулы в блоке n
                //m_arIndexDst[m_lIndexSrc] = m_lIndexDst

                switch (oSlice.m_eRuler)
                {
                case rtMoveTo:
                {
                    ConvertSlice_MoveTo(oSlice);
                    break;
                }
                case rtRMoveTo:
                {
                    ConvertSlice_RMoveTo(oSlice);
                    break;
                }
                case rtClose:
                {
                    m_oPathRes.WriteString(L"<a:close/>");
                    break;
                }
                case rtLineTo:
                {
                    ConvertSlice_LineTo(oSlice);
                    break;
                }
                case rtRLineTo:
                {
                    ConvertSlice_RLineTo(oSlice);
                    break;
                }
                case rtArcTo:
                case rtArc:
                {
                    ConvertSlice_ArcTo(oSlice);
                    break;
                }
                case rtClockwiseArcTo:
                case rtClockwiseArc:
                {
                    ConvertSlice_ClockwiseTo(oSlice);
                    break;
                }
                case rtQuadrBesier:
                {
                    ConvertSlice_QuadrBesier(oSlice);
                    break;
                }
                case rtCurveTo:
                {
                    ConvertSlice_CurveTo(oSlice);
                    break;
                }
                case rtRCurveTo:
                {
                    ConvertSlice_RCurveTo(oSlice);
                    break;
                }
                case rtAngleEllipse:
                case rtAngleEllipseTo:
                {
                    ConvertSlice_AngleEllipse(oSlice);
                    break;
                }
                case rtEllipticalQuadrX:
                {
                    ConvertSlice_EllipticalQuadrX(oSlice);
                    break;
                }
                case rtEllipticalQuadrY:
                {
                    ConvertSlice_EllipticalQuadrY(oSlice);
                    break;
                }
                default:
                    break;
                }
            }

            m_oPathRes.WriteString(L"</a:path>");
        }
    }

    void ConvertHandle(const std::vector<CHandle_>& arHandles, std::vector<long>& arAdj, PPTShapes::ShapeType oSType)
    {
        size_t nHandlesCount = arHandles.size();
        if (oSType == 19) // в пптх не реализована функция изменения размера шейпа при изменении handle
            nHandlesCount = 0;
        for (size_t i = 0; i < nHandlesCount; ++i)
        {
            const CHandle_& pHnPoint = arHandles[i];
            std::vector<std::wstring> arPos;

            std::wstring sPos0 = L"";
            std::wstring sPos1 = L"";

            SHandle oHandle;
            ParamType ptType;

            if (pHnPoint.position != L"")
            {
                boost::algorithm::split(arPos, pHnPoint.position, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);
                sPos0 = arPos[0];
                sPos1 = arPos[1];

                oHandle.gdRef.x = GetHandleValue(sPos0, m_lWidth, ptType);
                oHandle.gdRefType.x = ptType;
                if (oHandle.gdRefType.x == ptAdjust)
                    oHandle.bRefExist.x = true;

                oHandle.gdRef.y = GetHandleValue(sPos1, m_lWidth, ptType);
                oHandle.gdRefType.y = ptType;
                if (oHandle.gdRefType.y == ptAdjust)
                    oHandle.bRefExist.y = true;

                if (pHnPoint.polar != L"")
                {
                    //пришел полярный угол
                    oHandle.bRefExist.y = false;

                    oHandle.bRefPolarExist.y = true;
                    oHandle.bMinPolarExist.y = true;

                    boost::algorithm::split(arPos, pHnPoint.polar, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);

                    oHandle.PolarCentre.x = GetHandleValue(arPos[0], m_lWidth, ptType);
                    oHandle.PolarCentreType.x = ptType;

                    oHandle.PolarCentre.y = GetHandleValue(arPos[1], m_lWidth, ptType);
                    oHandle.PolarCentreType.y = ptType;

                    if (oHandle.gdRefType.y == ptAdjust)
                    {
                        //угол должен храниться в pptx формате, поэтому придётся его везде заменить
                        std::wstring strNewFmla = GetValue2(m_lIndexDst, ptFormula, true);
                        std::wstring strOldFmla = GetValue2(oHandle.gdRef.y, oHandle.gdRefType.y, false);
                        LONG nIndex = m_lIndexDst;

                        // TODO: !!! тут медленный код.
                        m_oGuidsRes.ReplaceString(strOldFmla, strNewFmla);

                        m_lIndexSrc++;

                        // TODO: !!! тут медленный код.
                        NSBinPptxRW::CXmlWriter memGuidsRes;
                        ConvertProd(oHandle.gdRef.y, oHandle.gdRefType.y, m_oParam.m_lCoef, ptValue, pow3_16, ptValue, false, true, false, memGuidsRes);
                        m_oGuidsRes.m_oWriter.WriteBefore(memGuidsRes.m_oWriter);
                        m_arIndexDst.push_back(m_lIndexDst-1);

                        if (oHandle.gdRefType.y == ptAdjust)
                        {
                            double dScale = (double)pow3_16 / m_oParam.m_lCoef;

                            if (oHandle.gdRef.y >= (int)arAdj.size())
                            {
                                // дурацкий код. надо память перевыделить, а старую скопировать
                                // пока сделаю так, чтобы наверняка
                                int nNewSize = (int)oHandle.gdRef.y + 1;
                                while ((int)arAdj.size() < nNewSize)
                                    arAdj.push_back(0);
                            }

                            LONG lVal = (LONG)(dScale * arAdj[oHandle.gdRef.y]);
                            arAdj[oHandle.gdRef.y] =  lVal;
                        }

                        //рассчитаем координаты точки handle

                        m_lIndexSrc++;
                        ConvertCos(oHandle.gdRef.x, oHandle.gdRefType.x, oHandle.gdRef.y, oHandle.gdRefType.y, false, false, m_oGuidsRes);
                        ConvertSin(oHandle.gdRef.x, oHandle.gdRefType.x, oHandle.gdRef.y, oHandle.gdRefType.y, false, false, m_oGuidsRes);
                        ConvertSum(oHandle.PolarCentre.x, oHandle.PolarCentreType.x, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, false, m_oGuidsRes);
                        ConvertSum(oHandle.PolarCentre.y, oHandle.PolarCentreType.y, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, false, m_oGuidsRes);
                        m_arIndexDst.push_back(m_lIndexDst-1);

                        std::wstring strMem = L"&" + std::to_wstring(m_lIndexDst-2);
                        oHandle.Pos.x = GetHandlePos(strMem, L"w",  m_lWidth);
                        oHandle.PosType.x = ptFormula;

                        strMem = L"&" + std::to_wstring(m_lIndexDst-2); //?????
                        oHandle.Pos.y = GetHandlePos(strMem, L"h",  m_lHeight);
                        oHandle.PosType.y = ptFormula;
                    }

                }
                else//если пришли обычные координаты
                {
                    if ((pHnPoint.xrange != L"" && oHandle.gdRefType.x != ptAdjust && oHandle.gdRefType.y == ptAdjust) ||
                            (pHnPoint.yrange != L"" && oHandle.gdRefType.x == ptAdjust && oHandle.gdRefType.y != ptAdjust))
                    {
                        oHandle.Pos.x = GetHandlePos(sPos1, L"w",  m_lWidth);
                        oHandle.PosType.x = ptFormula;

                        oHandle.Pos.y = GetHandlePos(sPos0, L"h",  m_lHeight);
                        oHandle.PosType.y = ptFormula;
                    }
                    else
                    {
                        oHandle.Pos.x = GetHandlePos(sPos0, L"w",  m_lWidth);
                        oHandle.PosType.x = ptFormula;

                        oHandle.Pos.y = GetHandlePos(sPos1, L"h",  m_lHeight);
                        oHandle.PosType.y = ptFormula;
                    }
                }
            }

            if (pHnPoint.xrange != L"")
            {
                if ( oHandle.gdRefType.x != ptAdjust && oHandle.gdRefType.y == ptAdjust)
                {
                    oHandle.gdRef.x = GetHandleValue(sPos1, m_lWidth, ptType);
                    oHandle.gdRefType.x = ptType;
                    oHandle.bRefExist.x = true;

                    oHandle.gdRef.y = GetHandleValue(sPos0, m_lHeight, ptType);
                    oHandle.gdRefType.y = ptType;
                    oHandle.bRefExist.y = false;
                }

                boost::algorithm::split(arPos, pHnPoint.xrange, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);

                oHandle.Min.x = GetHandleValue(arPos[0], m_lWidth, ptType);
                oHandle.MinType.x = ptType;
                if ( oHandle.bRefExist.x)
                    oHandle.bMinExist.x = true;

                oHandle.Max.x = GetHandleValue(arPos[1], m_lWidth, ptType);
                oHandle.MaxType.x = ptType;
                if ( oHandle.bRefExist.x)
                    oHandle.bMaxExist.x = true;
            }

            if (pHnPoint.yrange != L"")
            {
                if (oHandle.gdRefType.x == ptAdjust && oHandle.gdRefType.y != ptAdjust)
                {
                    oHandle.gdRef.x = GetHandleValue(sPos1, m_lWidth, ptType);
                    oHandle.gdRefType.x = ptType;
                    oHandle.bRefExist.x = false;

                    oHandle.gdRef.y = GetHandleValue(sPos0, m_lHeight, ptType);
                    oHandle.gdRefType.y = ptType;
                    oHandle.bRefExist.y = true;
                }
                boost::algorithm::split(arPos, pHnPoint.yrange, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);

                oHandle.Min.y = GetHandleValue(arPos[0], m_lHeight, ptType);
                oHandle.MinType.y = ptType;
                if ( oHandle.bRefExist.y )
                    oHandle.bMinExist.y = true;

                oHandle.Max.y = GetHandleValue(arPos[1], m_lHeight, ptType);
                oHandle.MaxType.y = ptType;
                if ( oHandle.bRefExist.y )
                    oHandle.bMaxExist.y = true;
            }

            if (pHnPoint.radiusrange != L"")
            {
                boost::algorithm::split(arPos, pHnPoint.radiusrange, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);

                oHandle.Min.x = GetHandleValue(arPos[0], m_lHeight, ptType);
                oHandle.MinType.x = ptType;
                oHandle.bMinPolarExist.x = true;

                oHandle.Max.x = GetHandleValue(arPos[1], m_lHeight, ptType);
                oHandle.MaxType.x = ptType;
                oHandle.bMinPolarExist.y = true;

                if (oHandle.gdRefType.x == ptAdjust)
                {
                    oHandle.bRefPolarExist.x = true;
                    oHandle.bRefExist.x = false;
                }
            }

            CreateHandle(oHandle);

        }
        ConvertAdj(arAdj);
    }
	void SetTextRectDefault()
	{
		m_lIndexSrc++;

		ConvertProd(L"w",  L"0", m_lWidth, m_oGuidsRes);
		ConvertProd(L"h",  L"0", m_lHeight, m_oGuidsRes);
		ConvertProd(L"w",  std::to_wstring(m_lWidth), m_lWidth, m_oGuidsRes);
		ConvertProd(L"h",  std::to_wstring(m_lHeight), m_lHeight, m_oGuidsRes);

		m_arIndexDst.push_back(m_lIndexDst);

		m_oTextRect.WriteString(L"<a:rect l=\"");
		GetValue(m_lIndexDst - 4, ptFormula, true, m_oTextRect);
		m_oTextRect.WriteString(L"\" t=\"");
		GetValue(m_lIndexDst - 3, ptFormula, true, m_oTextRect);
		m_oTextRect.WriteString(L"\" r=\"");
		GetValue(m_lIndexDst - 2, ptFormula, true, m_oTextRect);
		m_oTextRect.WriteString(L"\" b=\"");
		GetValue(m_lIndexDst - 1, ptFormula, true, m_oTextRect);
		m_oTextRect.WriteString(L"\" />");
	}
    void ConvertTextRect ( std::wstring strRect)
    {
        if (strRect.empty())
            return;

        std::vector<std::wstring> arBorder;
        boost::algorithm::split(arBorder, strRect, boost::algorithm::is_any_of(L","),  boost::algorithm::token_compress_on);

        m_lIndexSrc++;
		if (arBorder.size() > 0 && !arBorder[0].empty())
			ConvertProd(L"w",  arBorder[0], m_lWidth, m_oGuidsRes);
		else
			ConvertProd(L"w",  L"0", m_lWidth, m_oGuidsRes);

		if (arBorder.size() > 1 && !arBorder[1].empty())
			ConvertProd(L"h",  arBorder[1], m_lHeight, m_oGuidsRes);
		else 
			ConvertProd(L"h",  L"0", m_lHeight, m_oGuidsRes);

		if (arBorder.size() > 2 && !arBorder[2].empty())
			ConvertProd(L"w",  arBorder[2], m_lWidth, m_oGuidsRes);
		else
			ConvertProd(L"w",  std::to_wstring(m_lWidth), m_lWidth, m_oGuidsRes);
		
		if (arBorder.size() > 3 && !arBorder[3].empty())
			ConvertProd(L"h",  arBorder[3], m_lHeight, m_oGuidsRes);
		else
			ConvertProd(L"h",  std::to_wstring(m_lHeight), m_lHeight, m_oGuidsRes);
       
		m_arIndexDst.push_back(m_lIndexDst);

        //            if (true)
        //            {
        m_oTextRect.WriteString(L"<a:rect l=\"");
        GetValue(m_lIndexDst-4, ptFormula, true, m_oTextRect);
        m_oTextRect.WriteString(L"\" t=\"");
        GetValue(m_lIndexDst-3, ptFormula, true, m_oTextRect);
        m_oTextRect.WriteString(L"\" r=\"");
        GetValue(m_lIndexDst-2, ptFormula, true, m_oTextRect);
        m_oTextRect.WriteString(L"\" b=\"");
        GetValue(m_lIndexDst-1, ptFormula, true, m_oTextRect);
        m_oTextRect.WriteString(L"\" />");
        //            } else
        //            {
        //        m_oTextRect.WriteString(L"<a:rect l=\"l\" t=\"t\" r=\"r\" b=\"b\"/>");
        //            }

    }
private:

    AVSINLINE void GetValue(const LONG& lParam, const ParamType& eParamType, const bool& bExtShape, NSBinPptxRW::CXmlWriter& oWriter)
    {
        oWriter.m_oWriter.AddSize(15);
        switch (eParamType)
        {
        case ptFormula:
            if (bExtShape || lParam < (int)m_arIndexDst.size())
            {
                oWriter.m_oWriter.AddCharNoCheck((WCHAR)'g');
                oWriter.m_oWriter.AddCharNoCheck((WCHAR)'d');

                if (bExtShape)
                    oWriter.m_oWriter.AddIntNoCheck(lParam);
                else
                    oWriter.m_oWriter.AddIntNoCheck(m_arIndexDst[lParam]);
            }
            break;
        case ptAdjust:
        {
            oWriter.m_oWriter.AddCharNoCheck((WCHAR)'a');
            oWriter.m_oWriter.AddCharNoCheck((WCHAR)'d');
            oWriter.m_oWriter.AddCharNoCheck((WCHAR)'j');
            oWriter.m_oWriter.AddIntNoCheck(lParam);

            if (lParam > m_lMaxAdjUse)
                m_lMaxAdjUse = lParam;

            break;
        }
        case ptValue:
        {
            oWriter.m_oWriter.AddIntNoCheck(lParam);
            break;
        }
        default:
            break;
        }
    }
    std::wstring GetValue2(const LONG& lParam, const ParamType& eParamType, const bool& bExtShape)
    {
        std::wstring strValue;
        switch (eParamType)
        {
        case ptFormula:
        {
            if (bExtShape)
                strValue = std::to_wstring(lParam);
            else
                strValue = std::to_wstring(m_arIndexDst[lParam]);
            strValue = L"gd" + strValue;
            break;
        }
        case ptAdjust:
        {
            strValue = std::to_wstring(lParam);
            strValue = L"adj" + strValue;
            break;
        }
        case ptValue:
        {
            strValue = std::to_wstring(lParam);
            strValue = strValue;
            break;
        }
        default:
            break;
        }
        return strValue;
    }
    //---------------------------------------

    void ConvertVal(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_1(val)
    }

    void ConvertSum(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                    const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(sum)
    }

    void ConvertProd(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(prod)
    }

    void ConvertAbs(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_1(abs)
    }

    void ConvertMin(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                    const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(min)
    }

    void ConvertMax (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(max)
    }

    void ConvertIf(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                   const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(_if)
    }

    void ConvertSqrt(const LONG& lParam1, const ParamType& eType1, const bool& bExtShape1, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_1(sqrt)
    }

    void ConvertAt2 (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(at2)
    }

    void ConvertSin (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(sin)
    }

    void ConvertCos (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(cos)
    }

    void ConvertCat2(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(cat2)
    }

    void ConvertSat2(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(sat2)
    }

    void ConvertMod(const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                    const LONG& lParam3, const ParamType& eType3, const bool& bExtShape1, const bool& bExtShape2, const bool& bExtShape3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_3(mod)
    }

    void ConvertTag (const LONG& lParam1, const ParamType& eType1, const LONG& lParam2, const ParamType& eType2,
                     const bool& bExtShape1, const bool& bExtShape2, NSBinPptxRW::CXmlWriter& oWriter)
    {
        GUIDE_PARAM_2(tag)
    }

    void ConvertProd(const std::wstring& strParam1, const std::wstring& strParam2, const LONG& lParam3, NSBinPptxRW::CXmlWriter& oWriter)
    {
        oWriter.m_oWriter.AddSize(g_guide_max_len);
		oWriter.m_oWriter.WriteStringNoSafe(g_guide_string1, g_guide_string1_len);
        oWriter.m_oWriter.AddIntNoCheck(m_lIndexDst);
		oWriter.m_oWriter.WriteStringNoSafe(g_guide_string_prod, g_guide_string_prod_len);
        oWriter.WriteString(strParam1);
        oWriter.m_oWriter.AddSpaceNoCheck();

        if ('#' == strParam2[0])
        {
            LONG lRes = (LONG)XmlUtils::GetInteger(strParam2.substr(1));
            GetValue(lRes, ptAdjust, false, oWriter);
        }
        else if ('@' == strParam2[0])
        {
            LONG lRes = (LONG)XmlUtils::GetInteger(strParam2.substr(1));
            GetValue(lRes, ptFormula, false, oWriter);
        }
        else
            oWriter.WriteString(strParam2);

        oWriter.m_oWriter.AddSpaceNoCheck();
        GetValue(lParam3, ptValue, false, oWriter);

        ++m_lIndexDst;
		oWriter.m_oWriter.WriteStringNoSafe(g_guide_string2, g_guide_string2_len);
    }

    //-------------------------------------
    void FromXML(std::wstring strPath, bool &m_bFill, bool &m_bStroke)
    {
        m_arSlicesPath.clear();
        NSStringUtils::CheckPathOn_Fill_Stroke(strPath, m_bFill, m_bStroke);
        std::vector<std::wstring> oArray;

        NSStringUtils::ParsePath2(strPath, &oArray);

        ParamType eParamType = ptValue;
        RulesType eRuler = rtEnd;
        LONG lValue;
        bool bRes = true;

        for (size_t nIndex = 0; nIndex < oArray.size(); ++nIndex)
        {
            std::wstring str = oArray[nIndex];
            lValue = NSGuidesVML::GetValue(oArray[nIndex], eParamType, bRes);
            if (bRes)
            {
                if (0 != m_arSlicesPath.size())
                {
                    if ( lValue >= 0x7fffff00 || lValue <= -0x7fffff00)
                    {
                        lValue  =  0;	//process(2).ppt - todooo разобраться что за хрень это ваще приплыла
                    }

                    m_arSlicesPath[m_arSlicesPath.size() - 1].AddParam(lValue, eParamType);
                }
            }
            else
            {
                eRuler = GetRuler(oArray[nIndex], bRes);
                if (bRes)
                {
                    if (rtNoFill == eRuler)
                    {
                        m_bFill = false;
                    }
                    else if (rtNoStroke == eRuler)
                    {
                        m_bStroke = false;
                    }
                    else
                    {
                        CSlicePath oSlice(eRuler, m_lX, m_lY);

                        m_arSlicesPath.push_back(oSlice);
                    }
                }
            }
        }
    }
    void CreateHandle ( SHandle oHnd )
    {
        std::wstring strRes = L"";
        if (oHnd.bRefPolarExist.x || oHnd.bRefPolarExist.y)
            m_oHandleRes.WriteString(L"<a:ahPolar");
        else if (oHnd.bRefExist.x || oHnd.bRefExist.y)
            m_oHandleRes.WriteString(L"<a:ahXY");
        else
            return;

        if (oHnd.bRefExist.x)
        {
            m_oHandleRes.WriteString(L" gdRefX=\"");
            GetValue(oHnd.gdRef.x, oHnd.gdRefType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }
        if (oHnd.bRefExist.y)
        {
            m_oHandleRes.WriteString(L" gdRefY=\"");
            GetValue(oHnd.gdRef.y, oHnd.gdRefType.y, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }
        if (oHnd.bRefPolarExist.x)
        {
            m_oHandleRes.WriteString(L" gdRefR=\"");
            GetValue(oHnd.gdRef.x, oHnd.gdRefType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }
        if (oHnd.bRefPolarExist.y)
        {
            m_oHandleRes.WriteString(L" gdRefAng=\"");
            GetValue(oHnd.gdRef.y, oHnd.gdRefType.y, true, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }

        //min max 1го параметра
        if (oHnd.bMinExist.x)
        {
            m_oHandleRes.WriteString(L" minX=\"");
            GetValue(oHnd.Min.x, oHnd.MinType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\" maxX=\"");
            GetValue(oHnd.Max.x, oHnd.MaxType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }
        else if (oHnd.bRefExist.x)
        {
            m_oHandleRes.WriteString(L" minX=\"-21474836\" maxX=\"21474836\"");
        }

        if (oHnd.bMinPolarExist.x)
        {
            m_oHandleRes.WriteString(L" minR=\"");
            GetValue(oHnd.Min.x, oHnd.MinType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\" maxR=\"");
            GetValue(oHnd.Max.x, oHnd.MaxType.x, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }

        //min max 2го параметра
        if (oHnd.bMinExist.y)
        {
            m_oHandleRes.WriteString(L" minY=\"");
            GetValue(oHnd.Min.y, oHnd.MinType.y, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\" maxY=\"");
            GetValue(oHnd.Max.y, oHnd.MaxType.y, false, m_oHandleRes);
            m_oHandleRes.WriteString(L"\"");
        }
        else if (oHnd.bRefExist.y)
        {
            m_oHandleRes.WriteString(L" minY=\"-21474836\" maxY=\"21474836\"");
        }

        if (oHnd.bMinPolarExist.y)
        {
            m_oHandleRes.WriteString(L" minAng=\"0\" maxAng=\"21600000\"");
        }

        m_oHandleRes.WriteString(L"><a:pos x=\"");
        GetValue(oHnd.Pos.x, oHnd.PosType.x, true, m_oHandleRes);
        m_oHandleRes.WriteString(L"\" y=\"");
        GetValue(oHnd.Pos.y, oHnd.PosType.y, true, m_oHandleRes);

        if (oHnd.bRefPolarExist.x || oHnd.bRefPolarExist.y)
            m_oHandleRes.WriteString(L"\"/></a:ahPolar>");
        else if (oHnd.bRefExist.x || oHnd.bRefExist.y)
            m_oHandleRes.WriteString(L"\"/></a:ahXY>");
    }

    LONG GetHandlePos(const std::wstring& strParam, const std::wstring& strBase, LONG lSize)
    {
        LONG lRes;
        std::wstring strSize = L" " + std::to_wstring(lSize);
        std::wstring strIndex;

        if ('#' == strParam[0])
        {
            lRes = (LONG)XmlUtils::GetInteger(strParam.substr(1));
            strFrmla = GetValue2(lRes, ptAdjust, false) + strSize;
        }
        else if ('&' == strParam[0])
        {
            lRes = (LONG)XmlUtils::GetInteger(strParam.substr(1));
            strFrmla = GetValue2(lRes, ptFormula, true) + strSize;
        }
        else if ('@' == strParam[0])
        {
            lRes = (LONG)XmlUtils::GetInteger(strParam.substr(1));
            strFrmla = GetValue2(lRes, ptFormula, false) + strSize;
        }
        else if (!NSStringUtils::IsNumber(strParam))
        {
            if (L"center" == strParam)
                strFrmla = L"1 2";
            else if (L"topLeft" == strParam)
                strFrmla = L"0 1";
            else if ( L"bottomRight" == strParam)
                strFrmla = L"1 1";
        }
        else
        {
            lRes = (LONG)XmlUtils::GetInteger(strParam);
            strFrmla = GetValue2(lRes, ptValue, false) + strSize;
        }

        m_lIndexSrc++;
        m_arIndexDst.push_back(m_lIndexDst);
        strIndex = std::to_wstring(m_lIndexDst);

        m_oGuidsRes.WriteString(L"<a:gd name=\"gd");
        m_oGuidsRes.WriteString(strIndex);
        m_oGuidsRes.WriteString(L"\" fmla=\"*/ ");
        m_oGuidsRes.WriteString(strBase);
        m_oGuidsRes.WriteString(L" ");
        m_oGuidsRes.WriteString(strFrmla);
        m_oGuidsRes.WriteString(L"\"/>");

        m_lIndexDst++;

        return m_lIndexDst-1;
    }


    LONG GetHandleValue(const std::wstring& strParam, const LONG& lVal, ParamType& ptType)
    {
        ptType = ptValue;
        if ('#' == strParam[0])
        {
            ptType = ptAdjust;
            return (LONG)XmlUtils::GetInteger(strParam.substr(1));
        }
        else if ('@' == strParam[0])
        {
            ptType = ptFormula;
            return (LONG)XmlUtils::GetInteger(strParam.substr(1));
        }
        else if (!NSStringUtils::IsNumber(strParam))
        {
            if (L"center" == strParam)
                return lVal/2;
            else if (L"bottomRight" == strParam)//Demo-Hayden-Management-v2.docx
                return lVal;
            else
                return 0; //???
            //todooo прописать все варианты правильно
        }
        else
        {
            ptType = ptValue;
            return (LONG)XmlUtils::GetInteger(strParam);
        }
    }
    void ConvertQuadrX(Aggplus::POINT pPoint, SPointType pPointType)
    {
        LONG nIndex = m_arIndexDst[m_lIndexSrc];

        m_lIndexSrc++;

        ConvertSum(pPoint.x, pPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
        ConvertSum(pPoint.y, pPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
        ConvertProd(m_lIndexDst-2, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-4, ptFormula, 16200000, ptValue, 5400000, ptValue, true, true, true, m_oGuidsRes);	//stAng
        ConvertIf(m_lIndexDst-2, ptFormula, 5400000, ptValue, -5400000, ptValue, true, true, true, m_oGuidsRes);	//swAng
        ConvertProd(m_lIndexDst-7, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertProd(m_lIndexDst-7, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-9, ptFormula, m_lIndexDst-9, ptFormula, m_lIndexDst-2, ptFormula, true, true, true, m_oGuidsRes);//wR
        ConvertIf(m_lIndexDst-9, ptFormula, m_lIndexDst-9, ptFormula, m_lIndexDst-2, ptFormula, true, true, true, m_oGuidsRes);//hR

        m_arIndexDst.push_back(m_lIndexDst-1);

        nIndex = m_arIndexDst[m_lIndexSrc];

        m_oPathRes.WriteString(L"<a:arcTo wR=\"");
        GetValue(nIndex-1, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" hR=\"");
        GetValue(nIndex, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" stAng=\"");
        GetValue(nIndex-5, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" swAng=\"");
        GetValue(nIndex-4, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" />");

        m_lIndexSrc++;
        ConvertVal(pPoint.x, pPointType.x, false, m_oGuidsRes);
        ConvertVal(pPoint.y, pPointType.y, false, m_oGuidsRes);
        m_arIndexDst.push_back(m_lIndexDst-1);
        return;
    }

    void ConvertQuadrY(Aggplus::POINT pPoint, SPointType pPointType)
    {
        LONG nIndex = m_arIndexDst[m_lIndexSrc];

        m_lIndexSrc++;

        ConvertSum(pPoint.x, pPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
        ConvertSum(pPoint.y, pPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
        ConvertProd(m_lIndexDst-2, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-5, ptFormula, 10800000, ptValue, 0, ptValue, true, true, true, m_oGuidsRes);	//stAng
        ConvertIf(m_lIndexDst-2, ptFormula, -5400000, ptValue, 5400000, ptValue, true, true, true, m_oGuidsRes);	//swAng
        ConvertProd(m_lIndexDst-7, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertProd(m_lIndexDst-7, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
        ConvertIf(m_lIndexDst-9, ptFormula, m_lIndexDst-9, ptFormula, m_lIndexDst-2, ptFormula, true, true, true, m_oGuidsRes);//wR
        ConvertIf(m_lIndexDst-9, ptFormula, m_lIndexDst-9, ptFormula, m_lIndexDst-2, ptFormula, true, true, true, m_oGuidsRes);//hR
        m_arIndexDst.push_back(m_lIndexDst-1);

        nIndex = m_arIndexDst[m_lIndexSrc];

        m_oPathRes.WriteString(L"<a:arcTo wR=\"");
        GetValue(nIndex-1, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" hR=\"");
        GetValue(nIndex, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" stAng=\"");
        GetValue(nIndex-5, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" swAng=\"");
        GetValue(nIndex-4, ptFormula, true, m_oPathRes);
        m_oPathRes.WriteString(L"\" />");

        m_lIndexSrc++;
        ConvertVal(pPoint.x, pPointType.x, false, m_oGuidsRes);
        ConvertVal(pPoint.y, pPointType.y, false, m_oGuidsRes);
        m_arIndexDst.push_back(m_lIndexDst-1);
        return;
    }
    //-------------------------------------

    void ConvertSlice_MoveTo(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); ++j)
        {
            pCurPoint		= oSlice.m_arPoints[j];
            pCurPointType	= oSlice.m_arPointsType[j];

            m_lIndexSrc++;
            ConvertVal(pCurPoint.x, pCurPointType.x, false, m_oGuidsRes);
            ConvertVal(pCurPoint.y, pCurPointType.y, false, m_oGuidsRes);

            m_arIndexDst.push_back(m_lIndexDst-1);

            m_oPathRes.WriteString(L"<a:moveTo><a:pt x=\"");
            GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" y=\"");
            GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" /></a:moveTo>");
        }
    }

    void ConvertSlice_RMoveTo(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j++)
        {
            pCurPoint		= oSlice.m_arPoints[j];
            pCurPointType	= oSlice.m_arPointsType[j];

            m_lIndexSrc ++;
            ConvertSum(m_lIndexDst-2, ptFormula, pCurPoint.x, pCurPointType.x, 0, ptValue, true, false, true, m_oGuidsRes);
            ConvertSum(m_lIndexDst-2, ptFormula, pCurPoint.y, pCurPointType.y, 0, ptValue, true, false, true, m_oGuidsRes);

            m_arIndexDst.push_back(m_lIndexDst-1);

            m_oPathRes.WriteString(L"<a:moveTo><a:pt x=\"");
            GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" y=\"");
            GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" /></a:moveTo>");
        }
    }

    void ConvertSlice_LineTo(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j++)
        {
            pCurPoint = oSlice.m_arPoints[j];
            pCurPointType = oSlice.m_arPointsType[j];

            m_lIndexSrc++;
            ConvertVal(pCurPoint.x, pCurPointType.x, false, m_oGuidsRes);
            ConvertVal(pCurPoint.y, pCurPointType.y, false, m_oGuidsRes);

            m_arIndexDst.push_back(m_lIndexDst-1);

            m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
            GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" y=\"");
            GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" /></a:lnTo>");
        }
    }

    void ConvertSlice_RLineTo(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j++)
        {
            pCurPoint = oSlice.m_arPoints[j];
            pCurPointType = oSlice.m_arPointsType[j];

            m_lIndexSrc++;
            ConvertSum(m_lIndexDst-2, ptFormula, pCurPoint.x, pCurPointType.x, 0, ptValue, true, false, true, m_oGuidsRes);
            ConvertSum(m_lIndexDst-2, ptFormula, pCurPoint.y, pCurPointType.y, 0, ptValue, true, false, true, m_oGuidsRes);

            m_arIndexDst.push_back(m_lIndexDst-1);

            m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
            GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" y=\"");
            GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
            m_oPathRes.WriteString(L"\" /></a:lnTo>");
        }
    }

    void ConvertSlice_ArcTo(const CSlicePath& oSlice)
    {
        LONG nIndex = 0;
        LONG nIndex1 = 0;
        LONG nIndex2 = 0;

        for (size_t j = 0; j < oSlice.m_arPoints.size(); j+=4)
        {
            pCurPoint		= oSlice.m_arPoints[j];
            pCurPointType	= oSlice.m_arPointsType[j];
            pCurPoint1		= oSlice.m_arPoints[j+1];
            pCurPointType1	= oSlice.m_arPointsType[j+1];

            //длина сторон
            m_lIndexSrc++;
            ConvertSum(pCurPoint1.x, pCurPointType1.x, 0, ptValue, pCurPoint.x, pCurPointType.x, false, true, false, m_oGuidsRes);
            ConvertSum(pCurPoint1.y, pCurPointType1.y, 0, ptValue, pCurPoint.y, pCurPointType.y, false, true, false, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты центра
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-1, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes); //a=wR
            ConvertProd(nIndex, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes); //b=hR

            ConvertSum(pCurPoint.x, pCurPointType.x, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //рассчет для stAng---------------------------
            //расположение текущей точки по четвертям 3 4
            //										  2 1

            pCurPoint = oSlice.m_arPoints[j+2];
            pCurPointType = oSlice.m_arPointsType[j+2];
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertSum(pCurPoint.x, pCurPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол между Ох и радиус-вектором к точке
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-2, ptFormula, 1, ptValue, nIndex-3, ptFormula, true, true, true, m_oGuidsRes);
            ConvertAt2(1, ptValue, m_lIndexDst-1, ptFormula, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //радиус к точке
            nIndex = m_arIndexDst[m_lIndexSrc];
            nIndex1 = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertCos(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-2, ptFormula, nIndex1-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(b*cos(u))^2

            ConvertSin(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(a*sin(u))^2

            ConvertSum(m_lIndexDst-1, ptFormula, m_lIndexDst-5, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//r
            m_arIndexDst.push_back(m_lIndexDst-1);
            //если точка во 2 и 3 четвертях, прибавляем по 180grad
            nIndex = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertIf(nIndex-3, ptFormula, 0, ptValue, 10800000, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол до 1 точки (stAngle)
            nIndex = m_arIndexDst[m_lIndexSrc-2];//угол между ох и радиусом
            nIndex1 = m_arIndexDst[m_lIndexSrc-3];//расположение точки по четвертям
            nIndex2 = m_arIndexDst[m_lIndexSrc];//прибавка 180 или 0

            m_lIndexSrc ++;
            ConvertProd(nIndex1-1, ptFormula, nIndex1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//знак у угла
            ConvertProd(nIndex, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex, ptFormula, nIndex, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//угол всегда положительный

            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//u с нужным знаком

            ConvertSum(m_lIndexDst-1, ptFormula, nIndex2, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты стартовой точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            nIndex1 = m_arIndexDst[m_lIndexSrc-4];
            nIndex2 = m_arIndexDst[m_lIndexSrc-5];

            m_lIndexSrc++;
            ConvertProd(nIndex, ptFormula, nIndex-10, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*cos(a)
            ConvertProd(nIndex1-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2-1, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes); //x

            ConvertProd(nIndex-6, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex-6, ptFormula, nIndex-6, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//sin теперь всегда>0

            ConvertProd(nIndex, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*sin
            ConvertProd(nIndex1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//y
            m_arIndexDst.push_back(m_lIndexDst-1);
            //рассчет для swAng---------------------------
            //расположение текущей точки по четвертям 3 4
            //										  2 1

            pCurPoint = oSlice.m_arPoints[j+3];
            pCurPointType = oSlice.m_arPointsType[j+3];

            nIndex = m_arIndexDst[m_lIndexSrc-6];

            m_lIndexSrc++;
            ConvertSum(pCurPoint.x, pCurPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол между Ох и радиус-вектором к точке
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-2, ptFormula, 1, ptValue, nIndex-3, ptFormula, true, true, true, m_oGuidsRes);
            ConvertAt2(1, ptValue, m_lIndexDst-1, ptFormula, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //радиус к точке
            nIndex = m_arIndexDst[m_lIndexSrc];
            nIndex1 = m_arIndexDst[m_lIndexSrc-8];

            m_lIndexSrc++;
            ConvertCos(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-2, ptFormula, nIndex1-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);

            ConvertSin(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);

            ConvertSum(m_lIndexDst-1, ptFormula, m_lIndexDst-5, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //если точка во 2 и 3 четвертях, прибавляем по 180grad
            nIndex = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertIf(nIndex-3, ptFormula, 0, ptValue, 10800000, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол до 2 точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];//угол между ох и радиусом
            nIndex1 = m_arIndexDst[m_lIndexSrc-3];//расположение точки по четвертям
            nIndex2 = m_arIndexDst[m_lIndexSrc];//прибавка 180 или 0

            m_lIndexSrc++;
            ConvertProd(nIndex1-1, ptFormula, nIndex1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//знак у угла
            ConvertProd(nIndex, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex, ptFormula, nIndex, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//угол всегда положительный

            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//u с нужным знаком

            ConvertSum(m_lIndexDst-1, ptFormula, nIndex2, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты конечной точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            nIndex1 = m_arIndexDst[m_lIndexSrc-4];
            nIndex2 = m_arIndexDst[m_lIndexSrc-11];

            m_lIndexSrc++;
            ConvertProd(nIndex, ptFormula, nIndex-10, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*cos(a)
            ConvertProd(nIndex1-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2-1, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//x

            ConvertProd(nIndex-6, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex-6, ptFormula, nIndex-6, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//sin теперь всегда>0

            ConvertProd(nIndex, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*sin(a)
            ConvertProd(nIndex1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//y
            m_arIndexDst.push_back(m_lIndexDst-1);
            //swAngle
            nIndex = m_arIndexDst[m_lIndexSrc-1]; //2 угол
            nIndex1 = m_arIndexDst[m_lIndexSrc-7]; //1 угол

            m_lIndexSrc++;
            ConvertSum(nIndex1, ptFormula, 0, ptValue, nIndex, ptFormula, true, true, true, m_oGuidsRes); // (1)-(2), если отрицательный то берем его, иначе пересчитываем
            ConvertProd(m_lIndexDst-1, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);

            ConvertSum(nIndex1, ptFormula, 21600000, ptValue, nIndex, ptFormula, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes); // -1*((1)+360-(2))

            ConvertIf(m_lIndexDst-4, ptFormula, m_lIndexDst-3, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);

            m_arIndexDst.push_back(m_lIndexDst-1);
            //wR,hR
            nIndex = m_arIndexDst[m_lIndexSrc-14];
            m_lIndexSrc++;
            ConvertProd(nIndex-1, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);

            //---------------------------------------------------------
            nIndex = m_arIndexDst[m_lIndexSrc-8];//координаты стартовой точки
            nIndex1 = m_arIndexDst[m_lIndexSrc-9];//stAng
            nIndex2 = m_arIndexDst[m_lIndexSrc-1];//swAng

            if (oSlice.m_eRuler == rtArc && j == 0)
                strFrmla = L"moveTo";
            else
                strFrmla = L"lnTo";

            if (oSlice.m_eRuler == rtArc && j == 0)
            {
                m_oPathRes.WriteString(L"<a:moveTo><a:pt x=\"");
                GetValue(nIndex-5, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:moveTo><a:arcTo wR=\"");
                GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }
            else
            {
                m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                GetValue(nIndex-5, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:lnTo><a:arcTo wR=\"");
                GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }

            //старт
            /*
                                    LONG nIndex3 = m_arIndexDst[m_lIndexSrc-14];
                                    strPathRes += L"<a:moveTo><a:pt x=\"0\" y=\"0\" /></a:moveTo><a:lnTo><a:pt x=\"") + GetValue(nIndex3-1, ptFormula, true) + L"\" y=\"") + GetValue(nIndex3, ptFormula, true) +
                                            + L"\" /></a:lnTo>");
                                    */

            //текущая точка
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            ConvertVal(nIndex-5, ptFormula, true, m_oGuidsRes);
            ConvertVal(nIndex, ptFormula, true, m_oGuidsRes);
        }
    }

    void ConvertSlice_ClockwiseTo(const CSlicePath& oSlice)
    {
        LONG nIndex = 0;
        LONG nIndex1 = 0;
        LONG nIndex2 = 0;

        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 4)
        {
            pCurPoint		= oSlice.m_arPoints[j];
            pCurPointType	= oSlice.m_arPointsType[j];
            pCurPoint1		= oSlice.m_arPoints[j+1];
            pCurPointType1	= oSlice.m_arPointsType[j+1];

            //длина сторон
            m_lIndexSrc++;
            ConvertSum(pCurPoint1.x, pCurPointType1.x, 0, ptValue, pCurPoint.x, pCurPointType.x, false, true, false, m_oGuidsRes);
            ConvertSum(pCurPoint1.y, pCurPointType1.y, 0, ptValue, pCurPoint.y, pCurPointType.y, false, true, false, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты центра
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-1, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes); //a=wR
            ConvertProd(nIndex, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes); //b=hR

            ConvertSum(pCurPoint.x, pCurPointType.x, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, m_lIndexDst-2, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //рассчет для stAng---------------------------
            //расположение текущей точки по четвертям 3 4
            //										  2 1

            pCurPoint = oSlice.m_arPoints[j+2];
            pCurPointType = oSlice.m_arPointsType[j+2];
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertSum(pCurPoint.x, pCurPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол между Ох и радиус-вектором к точке
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-2, ptFormula, 1, ptValue, nIndex-3, ptFormula, true, true, true, m_oGuidsRes);
            ConvertAt2(1, ptValue, m_lIndexDst-1, ptFormula, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //радиус к точке
            nIndex = m_arIndexDst[m_lIndexSrc];
            nIndex1 = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertCos(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-2, ptFormula, nIndex1-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(b*cos(u))^2

            ConvertSin(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(a*sin(u))^2

            ConvertSum(m_lIndexDst-1, ptFormula, m_lIndexDst-5, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//r
            m_arIndexDst.push_back(m_lIndexDst-1);
            //если точка во 2 и 3 четвертях, прибавляем по 180grad
            nIndex = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertIf(nIndex-3, ptFormula, 0, ptValue, 10800000, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол до 1 точки (stAngle)
            nIndex = m_arIndexDst[m_lIndexSrc-2];//угол между ох и радиусом
            nIndex1 = m_arIndexDst[m_lIndexSrc-3];//расположение точки по четвертям
            nIndex2 = m_arIndexDst[m_lIndexSrc];//прибавка 180 или 0

            m_lIndexSrc++;
            ConvertProd(nIndex1-1, ptFormula, nIndex1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//знак у угла
            ConvertProd(nIndex, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex, ptFormula, nIndex, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//угол всегда положительный

            ConvertProd ( m_lIndexDst-1, ptFormula, m_lIndexDst-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//u с нужным знаком

            ConvertSum ( m_lIndexDst-1, ptFormula, nIndex2, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты стартовой точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            nIndex1 = m_arIndexDst[m_lIndexSrc-4];
            nIndex2 = m_arIndexDst[m_lIndexSrc-5];

            m_lIndexSrc++;
            ConvertProd(nIndex, ptFormula, nIndex-10, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*cos(a)
            ConvertProd(nIndex1-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2-1, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes); //x

            ConvertProd(nIndex-6, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex-6, ptFormula, nIndex-6, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//sin теперь всегда>0

            ConvertProd(nIndex, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*sin
            ConvertProd(nIndex1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//y
            m_arIndexDst.push_back(m_lIndexDst-1);
            //рассчет для swAng---------------------------
            //расположение текущей точки по четвертям 3 4
            //										  2 1

            pCurPoint = oSlice.m_arPoints[j+3];
            pCurPointType = oSlice.m_arPointsType[j+3];

            nIndex = m_arIndexDst[m_lIndexSrc-6];

            m_lIndexSrc++;
            ConvertSum(pCurPoint.x, pCurPointType.x, 0, ptValue, nIndex-1, ptFormula, false, true, true, m_oGuidsRes);
            ConvertSum(pCurPoint.y, pCurPointType.y, 0, ptValue, nIndex, ptFormula, false, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, 1, ptValue, -1, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол между Ох и радиус-вектором к точке
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertProd(nIndex-2, ptFormula, 1, ptValue, nIndex-3, ptFormula, true, true, true, m_oGuidsRes);
            ConvertAt2(1, ptValue, m_lIndexDst-1, ptFormula, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //радиус к точке
            nIndex = m_arIndexDst[m_lIndexSrc];
            nIndex1 = m_arIndexDst[m_lIndexSrc-8];

            m_lIndexSrc++;
            ConvertCos(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-2, ptFormula, nIndex1-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);

            ConvertSin(1, ptValue, nIndex, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);

            ConvertSum(m_lIndexDst-1, ptFormula, m_lIndexDst-5, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
            ConvertProd(nIndex1-3, ptFormula, nIndex1-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //если точка во 2 и 3 четвертях, прибавляем по 180grad
            nIndex = m_arIndexDst[m_lIndexSrc-2];

            m_lIndexSrc++;
            ConvertIf(nIndex-3, ptFormula, 0, ptValue, 10800000, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //угол до 2 точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];//угол между ох и радиусом
            nIndex1 = m_arIndexDst[m_lIndexSrc-3];//расположение точки по четвертям
            nIndex2 = m_arIndexDst[m_lIndexSrc];//прибавка 180 или 0

            m_lIndexSrc ++;
            ConvertProd(nIndex1-1, ptFormula, nIndex1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//знак у угла
            ConvertProd(nIndex, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex, ptFormula, nIndex, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//угол всегда положительный

            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-3, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//u с нужным знаком

            ConvertSum(m_lIndexDst-1, ptFormula, nIndex2, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
            //координаты конечной точки
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            nIndex1 = m_arIndexDst[m_lIndexSrc-4];
            nIndex2 = m_arIndexDst[m_lIndexSrc-11];

            m_lIndexSrc++;
            ConvertProd(nIndex, ptFormula, nIndex-10, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*cos(a)
            ConvertProd(nIndex1-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2-1, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//x

            ConvertProd(nIndex-6, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(nIndex-6, ptFormula, nIndex-6, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//sin теперь всегда>0

            ConvertProd(nIndex, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//r*sin(a)
            ConvertProd(nIndex1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertSum(nIndex2, ptFormula, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);//y
            m_arIndexDst.push_back(m_lIndexDst-1);
            //swAngle
            nIndex = m_arIndexDst[m_lIndexSrc-1];
            nIndex1 = m_arIndexDst[m_lIndexSrc-7];

            m_lIndexSrc++;
            ConvertSum(nIndex, ptFormula, 0, ptValue, nIndex1, ptFormula, true, true, true, m_oGuidsRes);
            ConvertSum(21600000, ptValue, m_lIndexDst-1, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertIf(m_lIndexDst-2, ptFormula, m_lIndexDst-2, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//swAng

            m_arIndexDst.push_back(m_lIndexDst-1);
            //wR,hR
            nIndex = m_arIndexDst[m_lIndexSrc-14];
            m_lIndexSrc++;
            ConvertProd(nIndex-1, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex, ptFormula, 1, ptValue, 2, ptValue, true, true, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);

            //---------------------------------------------------------
            nIndex = m_arIndexDst[m_lIndexSrc-8];//координаты стартовой точки
            nIndex1 = m_arIndexDst[m_lIndexSrc-9];//stAng
            nIndex2 = m_arIndexDst[m_lIndexSrc-1];//swAng

            if (oSlice.m_eRuler == rtClockwiseArc && j == 0)
            {
                m_oPathRes.WriteString(L"<a:moveTo><a:pt x=\"");
                GetValue(nIndex-5, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:moveTo><a:arcTo wR=\"");
                GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }
            else
            {
                m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                GetValue(nIndex-5, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:lnTo><a:arcTo wR=\"");
                GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }

            //текущая точка
            nIndex = m_arIndexDst[m_lIndexSrc-2];
            ConvertVal(nIndex-5, ptFormula, true, m_oGuidsRes);
            ConvertVal(nIndex, ptFormula, true, m_oGuidsRes);
        }
    }

    void ConvertSlice_QuadrBesier(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 2)
        {
            int l = (int)(oSlice.m_arPoints.size() - j - 3);
            if (l >= 0)
            {
                m_oPathRes.WriteString(L"<a:quadBezTo>");
                for (int k = 0; k < 2; ++k)
                {
                    pCurPoint = oSlice.m_arPoints[j+k];
                    pCurPointType = oSlice.m_arPointsType[j+k];

                    m_oPathRes.WriteString(L"<a:pt x=\"");
                    GetValue(pCurPoint.x, pCurPointType.x, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(pCurPoint.y, pCurPointType.y, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" />");
                }
                m_oPathRes.WriteString(L"</a:quadBezTo>");
            }
            else
            {
                for (size_t k = 0; k < oSlice.m_arPoints.size() - j; ++k)
                {
                    pCurPoint = oSlice.m_arPoints[j+k];
                    pCurPointType = oSlice.m_arPointsType[j+k];

                    m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                    GetValue(pCurPoint.x, pCurPointType.x, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(pCurPoint.y, pCurPointType.y, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" /></a:lnTo>");
                }
            }

            m_lIndexSrc++;
            ConvertVal(pCurPoint.x, pCurPointType.x, false, m_oGuidsRes);
            ConvertVal(pCurPoint.y, pCurPointType.y, false, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
        }
    }

    void ConvertSlice_CurveTo(const CSlicePath& oSlice)
    {
        LONG nIndex = 0;
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 3)
        {
            int l = (int)(oSlice.m_arPoints.size() - j - 3);
            if ( l >= 0 )
            {
                m_oPathRes.WriteString(L"<a:cubicBezTo>");
                for (int k = 0; k < 3; ++k)
                {
                    pCurPoint = oSlice.m_arPoints[j+k];
                    pCurPointType = oSlice.m_arPointsType[j+k];

                    m_oPathRes.WriteString(L"<a:pt x=\"");
                    GetValue(pCurPoint.x, pCurPointType.x, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(pCurPoint.y, pCurPointType.y, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" />");
                }
                m_oPathRes.WriteString(L"</a:cubicBezTo>");
            }
            else
            {
                for (size_t k = 0; k < oSlice.m_arPoints.size() - j; ++k)
                {
                    m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                    GetValue(pCurPoint.x, pCurPointType.x, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(pCurPoint.y, pCurPointType.y, false, m_oPathRes);
                    m_oPathRes.WriteString(L"\" /></a:lnTo>");
                }
            }

            m_lIndexSrc++;
            ConvertVal(pCurPoint.x, pCurPointType.x, false, m_oGuidsRes);
            ConvertVal(pCurPoint.y, pCurPointType.y, false, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
        }
    }

    void ConvertSlice_RCurveTo(const CSlicePath& oSlice)
    {
        LONG nIndex = 0;
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 3)
        {
            nIndex = m_arIndexDst[m_lIndexSrc];

            int l = (int)(oSlice.m_arPoints.size() - j - 3);
            if (l >= 0)
            {
                m_oPathRes.WriteString(L"<a:cubicBezTo>");
                for (int k = 0; k < 3; ++k)
                {
                    pCurPoint = oSlice.m_arPoints[j+k];
                    pCurPointType = oSlice.m_arPointsType[j+k];

                    m_lIndexSrc++;
                    ConvertSum(nIndex-1, ptFormula, pCurPoint.x, pCurPointType.x, 0, ptValue, true, false, true, m_oGuidsRes);
                    ConvertSum(nIndex, ptFormula, pCurPoint.y, pCurPointType.y, 0, ptValue, true, false, true, m_oGuidsRes);
                    m_arIndexDst.push_back(m_lIndexDst-1);

                    m_oPathRes.WriteString(L"<a:pt x=\"");
                    GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                    m_oPathRes.WriteString(L"\" />");
                }
                m_oPathRes.WriteString(L"</a:cubicBezTo>");
            }
            else
            {
                for (size_t k=0; k < oSlice.m_arPoints.size() - j; ++k)
                {
                    pCurPoint		= oSlice.m_arPoints[j+k];
                    pCurPointType	= oSlice.m_arPointsType[j+k];

                    m_lIndexSrc++;
                    ConvertSum(nIndex-1, ptFormula, pCurPoint.x, pCurPointType.x, 0, ptValue, true, false, true, m_oGuidsRes);
                    ConvertSum(nIndex, ptFormula, pCurPoint.y, pCurPointType.y, 0, ptValue, true, false, true, m_oGuidsRes);
                    m_arIndexDst.push_back(m_lIndexDst-1);

                    m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                    GetValue(m_lIndexDst-2, ptFormula, true, m_oPathRes);
                    m_oPathRes.WriteString(L"\" y=\"");
                    GetValue(m_lIndexDst-1, ptFormula, true, m_oPathRes);
                    m_oPathRes.WriteString(L"\" /></a:lnTo>");
                }
            }

            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertVal(nIndex-1, ptFormula, true, m_oGuidsRes);
            ConvertVal(nIndex, ptFormula, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
        }
    }

    void ConvertSlice_AngleEllipse(const CSlicePath& oSlice)
    {
        LONG nIndex = 0;
        LONG nIndex1 = 0;
        LONG nIndex2 = 0;

        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 3)
        {
            pCurPoint		= oSlice.m_arPoints[j+1];
            pCurPointType	= oSlice.m_arPointsType[j+1];
            pCurPoint1		= oSlice.m_arPoints[j+2];
            pCurPointType1	= oSlice.m_arPointsType[j+2];

            //конвертация углов в pptx формат
            m_lIndexSrc++;
            ConvertProd(pCurPoint1.x, pCurPointType1.x, pow3_16, ptValue, m_oParam.m_lParam, m_oParam.m_eType, false, true, true, m_oGuidsRes); //1 угол
            ConvertProd(pCurPoint1.y, pCurPointType1.y, pow3_16, ptValue, m_oParam.m_lParam, m_oParam.m_eType, false, true, true, m_oGuidsRes); //2 угол
            m_arIndexDst.push_back(m_lIndexDst-1);

            //wR и hR
            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertVal(pCurPoint.x, pCurPointType.x, false, m_oGuidsRes);//wr=a
            ConvertVal(pCurPoint.y, pCurPointType.y, false, m_oGuidsRes);//hr=b

            m_arIndexDst.push_back(m_lIndexDst-1);

            //stAng и swAng
            nIndex = m_arIndexDst[m_lIndexSrc-1];

            m_lIndexSrc++;
            ConvertProd(nIndex-1, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes); //stAng
            ConvertSum(nIndex-1, ptFormula, nIndex, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex, ptFormula, -1, ptValue, 1, ptValue, true, true, true, m_oGuidsRes);//swAng
            m_arIndexDst.push_back(m_lIndexDst-1);

            //радиус до стартовой точки

            nIndex = m_arIndexDst[m_lIndexSrc]; //stang
            nIndex1 = m_arIndexDst[m_lIndexSrc-1]; //wr hr

            m_lIndexSrc++;
            ConvertCos(1, ptValue, nIndex-2, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1, ptFormula, nIndex1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(b*cos(u))^2

            ConvertSin(1, ptValue, nIndex-2, ptFormula, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(nIndex1-1, ptFormula, nIndex1-1, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);
            ConvertProd(m_lIndexDst-1, ptFormula, m_lIndexDst-2, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);//(a*sin(u))^2

            ConvertSum(m_lIndexDst-1, ptFormula, m_lIndexDst-5, ptFormula, 0, ptValue, true, true, true, m_oGuidsRes);
            ConvertSqrt(m_lIndexDst-1, ptFormula, true, m_oGuidsRes);
            ConvertProd(nIndex1, ptFormula, nIndex1-1, ptFormula, m_lIndexDst-1, ptFormula, true, true, true, m_oGuidsRes);//r
            m_arIndexDst.push_back(m_lIndexDst-1);

            //координаты конечной точки (она же начальная для эллипса)

            pCurPoint1 = oSlice.m_arPoints[j];//коорд центра
            pCurPointType1 = oSlice.m_arPointsType[j];

            nIndex = m_arIndexDst[m_lIndexSrc-1]; //stang
            nIndex1 = m_arIndexDst[m_lIndexSrc]; //r

            m_lIndexSrc++;

            ConvertProd(nIndex1, ptFormula, nIndex1-10, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);  //r*cos
            ConvertSum(pCurPoint1.x, pCurPointType1.x, m_lIndexDst-1, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);//x

            ConvertProd(nIndex1, ptFormula, nIndex1-6, ptFormula, 1, ptValue, true, true, true, m_oGuidsRes);// r*sin
            ConvertSum(pCurPoint1.y, pCurPointType1.y, m_lIndexDst-1, ptFormula, 0, ptValue, false, true, true, m_oGuidsRes);//y
            m_arIndexDst.push_back(m_lIndexDst-1);
            //---------------------

            nIndex = m_arIndexDst[m_lIndexSrc];//текущая точка
            nIndex1 = m_arIndexDst[m_lIndexSrc-2];//stAng и swAng
            nIndex2 = m_arIndexDst[m_lIndexSrc-3];//wR и hR

            if (j == 0)
                strFrmla = L"moveTo";
            else
                strFrmla = L"lnTo";

            if (j == 0)
            {
                m_oPathRes.WriteString(L"<a:moveTo><a:pt x=\"");
                GetValue(nIndex-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:moveTo><a:arcTo wR=\"");
                GetValue(nIndex2-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }
            else
            {
                m_oPathRes.WriteString(L"<a:lnTo><a:pt x=\"");
                GetValue(nIndex-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" y=\"");
                GetValue(nIndex, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" /></a:lnTo><a:arcTo wR=\"");
                GetValue(nIndex2-1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" hR=\"");
                GetValue(nIndex2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" stAng=\"");
                GetValue(nIndex1-2, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" swAng=\"");
                GetValue(nIndex1, ptFormula, true, m_oPathRes);
                m_oPathRes.WriteString(L"\" />");
            }

            //текущая точка

            nIndex = m_arIndexDst[m_lIndexSrc];

            m_lIndexSrc++;
            ConvertVal(nIndex-2, ptFormula, true, m_oGuidsRes);
            ConvertVal(nIndex, ptFormula, true, m_oGuidsRes);
            m_arIndexDst.push_back(m_lIndexDst-1);
        }
    }

    void ConvertSlice_EllipticalQuadrX(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 2)
        {
            pCurPoint		= oSlice.m_arPoints[j];
            pCurPointType	= oSlice.m_arPointsType[j];

            ConvertQuadrX(pCurPoint, pCurPointType);

            if (j + 1 < oSlice.m_arPoints.size())
            {
                pCurPoint1 = oSlice.m_arPoints[j+1];
                pCurPointType1 = oSlice.m_arPointsType[j+1];
                ConvertQuadrY(pCurPoint1, pCurPointType1);
            }
        }
    }

    void ConvertSlice_EllipticalQuadrY(const CSlicePath& oSlice)
    {
        for (size_t j = 0; j < oSlice.m_arPoints.size(); j += 2)
        {
            pCurPoint = oSlice.m_arPoints[j];
            pCurPointType = oSlice.m_arPointsType[j];
            ConvertQuadrY(pCurPoint, pCurPointType);

            if (j + 1 < oSlice.m_arPoints.size())
            {
                pCurPoint1 = oSlice.m_arPoints[j+1];
                pCurPointType1 = oSlice.m_arPointsType[j+1];
                ConvertQuadrX(pCurPoint1, pCurPointType1);
            }
        }
    }
};
}
