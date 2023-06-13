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
		CFormula();
		CFormula(int nIndex);

		CFormula& operator =(const CFormula& oSrc);

		void FromString(std::wstring strFormula, long lShapeWidth = ShapeSizeVML, long lShapeHeight = ShapeSizeVML);
        LONG Calculate(CFormulasManager* pManager);
    };

    class CFormulasManager
    {
    public:
        std::vector<long>* m_pAdjustments;
        std::vector<long> m_arResults;

        std::vector<CFormula> m_arFormulas;

        long m_lShapeWidth;
        long m_lShapeHeight;

		bool m_bCalc; //status

		CFormulasManager();

		CFormulasManager& operator =(const CFormulasManager& oSrc);

		void Clear();
		void Clear(std::vector<long>* pAdjusts);

		void AddFormula(std::wstring strFormula);
		void AddFormula(CFormula oFormula);

		void CalculateResults();
    };
}
