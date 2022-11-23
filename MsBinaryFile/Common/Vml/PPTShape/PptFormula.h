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

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <boost/algorithm/string.hpp>

#include <math.h>
#include "../Common.h"

#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../DesktopEditor/graphics/aggplustypes.h"

#include "../../../../OOXML/Base/Unit.h"

namespace NSGuidesVML
{
    enum FormulaType
    {
        // VML
        ftSum			= 0,	// a + b - c
        ftProduct		= 1,	// a * b / c
        ftMid			= 2,	// (a + b) / 2
        ftAbsolute		= 3,	// abs(a)
        ftMin			= 4,	// min(a,b)
        ftMax			= 5,	// max(a,b)
        ftIf			= 6,	// if  a > 0 ? b : c
        ftMod			= 7,	// sqrt(a*a + b*b + c*c)
        ftAtan2			= 8,	// atan2(b,a)
        ftSin			= 9,	// a * sin(b)
        ftCos			= 10,	// a * cos(b)
        ftCosatan2		= 11,	// a * cos(atan2(c,b))
        ftSinatan2		= 12,	// a * sin(atan2(c,b))
        ftSqrt			= 13,	// sqrt(a)
        ftSumangle		= 14,	// a + b° - c°
        ftEllipse		= 15,	// c * sqrt(1-(a/b)2)
        ftTan			= 16,	// a * tan(b)
        ftVal			= 17	// a
    };

    #define VML_GUIDE_COUNT 18
    static const wchar_t* VML_GUIDE_TYPE[VML_GUIDE_COUNT] = {
        L"sum",
        L"prod",
        L"mid",
        L"abs",
        L"min",
        L"max",
        L"if",
        L"mod",
        L"atan2",
        L"sin",
        L"cos",
        L"cosatan2",
        L"sinatan2",
        L"sqrt",
        L"sumangle",
        L"ellipse",
        L"tan",
        L"val"
    };

    const BYTE VML_GUIDE_PARAM_COUNT[VML_GUIDE_COUNT] = {
        3,
        3,
        2,
        1,
        2,
        2,
        3,
        3,
        2,
        2,
        2,
        3,
        3,
        1,
        3,
        3,
        2,
        1
    };

    enum ParamType
    {
        ptFormula			= 0,
        ptAdjust			= 1,
        ptValue				= 2
    };

    static long GetValue(std::wstring strParam, ParamType& ptType, bool& bRes,
                         long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML)
    {
        ptType		= ptValue;
        bRes		= true;
        long val	= 0;

        if ('#' == strParam[0])
        {
            ptType = ptAdjust;
            val = XmlUtils::GetInteger(strParam.substr(1));
        }
        else if ('@' == strParam[0])
        {
            ptType = ptFormula;
            val = XmlUtils::GetInteger(strParam.substr(1));
        }
        else if (!NSStringUtils::IsNumber(strParam))
        {
            if (_T("width") == strParam)
            {
                val = lShapeWidth;
            }
            else if (_T("height") == strParam)
            {
                val = lShapeHeight;
            }
            else if (_T("pixelWidth") == strParam)
            {
                val = lShapeWidth;
            }
            else if (_T("pixelHeight") == strParam)
            {
                val = lShapeHeight;
            }
            else if (_T("pixelLineWidth") == strParam || _T("lineDrawn") == strParam)
            {
                val = 1;
            }
            else
            {
                bRes = false;
                return 0;
            }
        }
        else
        {
            ptType = ptValue;
            val = XmlUtils::GetInteger(strParam);
        }
        return val;
    }

    static FormulaType GetFormula(std::wstring strName, bool& bRes)
    {
        bRes = true;
        if		(_T("sum") == strName)									return ftSum;
        else if ((_T("prod") == strName) || (_T("product") == strName)) return ftProduct;
        else if (_T("mid") == strName)									return ftMid;
        else if ((_T("absolute") == strName) || (_T("abs") == strName)) return ftAbsolute;
        else if (_T("min") == strName)									return ftMin;
        else if (_T("max") == strName)									return ftMax;
        else if (_T("if") == strName)									return ftIf;
        else if (_T("sqrt") == strName)									return ftSqrt;
        else if (_T("mod") == strName)									return ftMod;
        else if (_T("sin") == strName)									return ftSin;
        else if (_T("cos") == strName)									return ftCos;
        else if (_T("tan") == strName)									return ftTan;
        else if (_T("atan2") == strName)								return ftAtan2;
        else if (_T("sinatan2") == strName)								return ftSinatan2;
        else if (_T("cosatan2") == strName)								return ftCosatan2;
        else if (_T("sumangle") == strName)								return ftSumangle;
        else if (_T("ellipse") == strName)								return ftEllipse;
        else if (_T("val") == strName)									return ftVal;
        else bRes = false;

        return ftVal;
    }
    struct SPointType
    {
        ParamType x;
        ParamType y;
    };
    struct SPointExist
    {
        bool x;
        bool y;

        SPointExist()
        {
            x = false;
            y = false;
        }
    };

    struct SHandle
    {
        Aggplus::POINT gdRef;
        SPointType gdRefType;
        SPointExist bRefExist;
        SPointExist bRefPolarExist;

        Aggplus::POINT Max;
        SPointType MaxType;
        SPointExist bMaxExist;
        SPointExist bMaxPolarExist;

        Aggplus::POINT Min;
        SPointType MinType;
        SPointExist bMinExist;
        SPointExist bMinPolarExist;

        Aggplus::POINT Pos;
        SPointType PosType;

        Aggplus::POINT PolarCentre;
        SPointType PolarCentreType;

    };
    class CFormulasManager;
    class CFormula
    {
    public:
        FormulaType m_eFormulaType;
        int m_lIndex;

        LONG m_lParam1;
        ParamType m_eType1;

        LONG m_lParam2;
        ParamType m_eType2;

        LONG m_lParam3;
        ParamType m_eType3;

    private:
        long m_lCountRecurs;

    public:
        CFormula()
        {
            m_eFormulaType = ftSum;
            m_lIndex = 0;
            m_lParam1 = 0;	m_eType1 = ptValue;
            m_lParam2 = 0;	m_eType2 = ptValue;
            m_lParam3 = 0;	m_eType3 = ptValue;

            m_lCountRecurs = 0;
        }

        CFormula(int nIndex)
        {
            m_eFormulaType = ftSum;
            m_lIndex = nIndex;
            m_lParam1 = 0;	m_eType1 = ptValue;
            m_lParam2 = 0;	m_eType2 = ptValue;
            m_lParam3 = 0;	m_eType3 = ptValue;

            m_lCountRecurs = 0;
        }

        CFormula& operator =(const CFormula& oSrc)
        {
            m_eFormulaType	= oSrc.m_eFormulaType;
            m_lIndex		= oSrc.m_lIndex;

            m_lParam1		= oSrc.m_lParam1;
            m_eType1		= oSrc.m_eType1;

            m_lParam2		= oSrc.m_lParam2;
            m_eType2		= oSrc.m_eType2;

            m_lParam3		= oSrc.m_lParam3;
            m_eType3		= oSrc.m_eType3;

            m_lCountRecurs = 0;
            return (*this);
        }

        void FromString(std::wstring strFormula, long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML)
        {
            std::vector<std::wstring> oArrayParams;
            boost::algorithm::split(oArrayParams, strFormula, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

            int nCount = (int)oArrayParams.size();
            if (0 >= nCount)
                return;

            bool bRes = true;
            m_eFormulaType = GetFormula(oArrayParams[0], bRes);

            ParamType ptType = ptValue;
            if (1 < nCount)
            {
                m_lParam1 = GetValue(oArrayParams[1], ptType, bRes, lShapeWidth, lShapeHeight);
                m_eType1 = ptType;
            }
            if (2 < nCount)
            {
                m_lParam2 = GetValue(oArrayParams[2], ptType, bRes, lShapeWidth, lShapeHeight);
                m_eType2 = ptType;
            }
            if (3 < nCount)
            {
                m_lParam3 = GetValue(oArrayParams[3], ptType, bRes, lShapeWidth, lShapeHeight);
                m_eType3 = ptType;
            }
        }
        LONG Calculate(CFormulasManager* pManager);
    };

    class CFormulasManager
    {
    public:
        std::vector<LONG>* m_pAdjustments;
        std::vector<LONG> m_arResults;

        std::vector<CFormula> m_arFormulas;

        long m_lShapeWidth;
        long m_lShapeHeight;

		bool m_bCalc; //status

        CFormulasManager() : m_bCalc(false)
        {
            m_pAdjustments	= NULL;
            m_lShapeWidth	= ShapeSizeVML;
            m_lShapeHeight	= ShapeSizeVML;
        }
        CFormulasManager& operator =(const CFormulasManager& oSrc)
        {
			m_bCalc			= oSrc.m_bCalc;
            m_pAdjustments	= oSrc.m_pAdjustments;
            m_lShapeWidth	= oSrc.m_lShapeWidth;
            m_lShapeHeight	= oSrc.m_lShapeHeight;

            m_arResults.clear();
            for (size_t nIndex = 0; nIndex < oSrc.m_arResults.size(); ++nIndex)
            {
                m_arResults.push_back(oSrc.m_arResults[nIndex]);
            }
            m_arFormulas.clear();
            for (size_t nIndex = 0; nIndex < oSrc.m_arFormulas.size(); ++nIndex)
            {
                m_arFormulas.push_back(oSrc.m_arFormulas[nIndex]);
            }

            return (*this);
        }

        void Clear()
        {
			m_bCalc			= false;
            m_pAdjustments	= NULL;
            m_lShapeWidth	= ShapeSizeVML;
            m_lShapeHeight	= ShapeSizeVML;

            m_arFormulas.clear();
            m_arResults.clear();
        }

        void Clear(std::vector<LONG>* pAdjusts)
        {
            m_pAdjustments = pAdjusts;

            //m_arFormulas.clear();
            //m_arResults.clear();
            for (size_t nIndex = 0; nIndex < m_arResults.size(); ++nIndex)
            {
                m_arResults[nIndex] = 0xFFFFFFFF;
            }
        }
        void AddFormula(std::wstring strFormula)
        {
            CFormula oFormula((int)m_arFormulas.size());

            oFormula.FromString(strFormula, m_lShapeWidth, m_lShapeHeight);
            m_arFormulas.push_back(oFormula);
            m_arResults.push_back(0xFFFFFFFF);
        }
        void AddFormula(CFormula oFormula)
        {
            oFormula.m_lIndex = (int)m_arFormulas.size();
            
			m_arFormulas.push_back(oFormula);
            m_arResults.push_back(0xFFFFFFFF);
        }
        void CalculateResults()
        {
            for (size_t index = 0; index < m_arFormulas.size(); ++index)
            {
                LONG lResult = m_arFormulas[index].Calculate(this);
            }

            //m_pAdjustments = NULL;
            //m_arFormulas.clear();
        }
    };
}
