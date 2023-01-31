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

#include <math.h>
#include "../Common.h"
#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../OOXML/Base/Unit.h"

namespace NSGuidesOOXML
{
    const double dNonDefResult = 27273042316900;
    const long NonDefResult = 0xFFFFFF;

    class CFormulaManager;

    class CFormula
    {
    public:
        enum FormulaType
        {
            // OOXML
            ftOOXMLSum			= 0,	// a + b - c
            ftOOXMLProduct		= 1,	// a * b / c
            ftOOXMLAddDivide	= 2,	// (a + b) / c
            ftOOXMLAbsolute		= 3,	// abs(a)
            ftOOXMLMin			= 4,	// min(a,b)
            ftOOXMLMax			= 5,	// max(a,b)
            ftOOXMLIf			= 6,	// if  a > 0 ? b : c
            ftOOXMLSqrt			= 7,	// sqrt(a)
            ftOOXMLMod			= 8,	// sqrt(a*a + b*b + c*c)
            ftOOXMLSin			= 9,	// a * sin(b)
            ftOOXMLCos			= 10,	// a * cos(b)
            ftOOXMLTan			= 11,	// a * tan(b)
            ftOOXMLAtan2		= 12,	// atan2(b,a)
            ftOOXMLSinatan2		= 13,	// a * sin(atan2(c,b))
            ftOOXMLCosatan2		= 14,	// a * cos(atan2(c,b))
            ftOOXMLPin			= 15,	// (b < a) ? a : ((b > c) ? c : b )
            ftOOXMLVal			= 16	// a
        };

        static FormulaType GetFormula(std::wstring strName, bool& bRes)
        {
            bRes = true;
            if		(_T("+-") == strName)	return ftOOXMLSum;
            else if (_T("*/") == strName)	return ftOOXMLProduct;
            else if (_T("+/") == strName)	return ftOOXMLAddDivide;
            else if (_T("abs") == strName)	return ftOOXMLAbsolute;
            else if (_T("min") == strName)	return ftOOXMLMin;
            else if (_T("max") == strName)	return ftOOXMLMax;
            else if (_T("?:") == strName)	return ftOOXMLIf;
            else if (_T("sqrt") == strName)	return ftOOXMLSqrt;
            else if (_T("mod") == strName)	return ftOOXMLMod;
            else if (_T("sin") == strName)	return ftOOXMLSin;
            else if (_T("cos") == strName)	return ftOOXMLCos;
            else if (_T("tan") == strName)	return ftOOXMLTan;
            else if (_T("at2") == strName)	return ftOOXMLAtan2;
            else if (_T("sat2") == strName)	return ftOOXMLSinatan2;
            else if (_T("cat2") == strName)	return ftOOXMLCosatan2;
            else if (_T("pin") == strName)	return ftOOXMLPin;
            else if (_T("val") == strName)	return ftOOXMLVal;
            else bRes = false;

            return ftOOXMLVal;
        }

    public:
        FormulaType m_eFormulaType;
        std::wstring m_sName;
        int m_lIndex;

        std::wstring m_lParam1;
        std::wstring m_lParam2;
        std::wstring m_lParam3;
    public:
        CFormula(long ind = 0)
        {
            m_sName = _T("");
            m_eFormulaType = ftOOXMLSum;
            m_lIndex = ind;
            m_lParam1 = _T("0");
            m_lParam2 = _T("0");
            m_lParam3 = _T("0");
        }

        CFormula& operator =(const CFormula& src)
        {
            m_sName = src.m_sName;
            m_eFormulaType = src.m_eFormulaType;
            m_lIndex = src.m_lIndex;
            m_lParam1 = src.m_lParam1;
            m_lParam2 = src.m_lParam2;
            m_lParam3 = src.m_lParam3;

            return *this;
        }

        void FromString(std::wstring strFormula)
        {
            std::vector<std::wstring> oArrayParams;

            boost::algorithm::split(oArrayParams, strFormula, boost::algorithm::is_any_of(L" "), boost::algorithm::token_compress_on);

            int nCount = (int)oArrayParams.size();
            if (0 >= nCount)
                return;

            bool bRes = true;
            m_eFormulaType = GetFormula(oArrayParams[0], bRes);

            //			ParamType ptType = ptValue;
            if (1 < nCount)
                m_lParam1 = oArrayParams[1];
            if (2 < nCount)
                m_lParam2 = oArrayParams[2];
            if (3 < nCount)
                m_lParam3 = oArrayParams[3];
        }

        double Calculate(CFormulaManager* pManager);
    };

    class CFormulaManager
{
private:		
    double m_lShapeWidth;
    double m_lShapeHeight;
public:
    std::map<std::wstring, long> mapAdjustments;
    std::map<std::wstring, long> mapGuides;
    std::vector<CFormula> strAdjustments;
    std::vector<CFormula> strGuides;
    std::vector<long>* Adjustments;
    std::vector<double>* Guides;
    
    void SetWidthHeight(double w, double h)
    {
        if((w >= 0) && (h >= 0))
        {
            Clear();
            if(w > h)
            {
                h = (h * ShapeSize)/w;
                if(h < 1.0) h = 1.0;
                w = ShapeSize;
            }
            else if(w < h)
            {
                w = (w * ShapeSize)/h;
                if(w < 1.0) w = 1.0;
                h = ShapeSize;
            }
            else
            {
                w = ShapeSize;
                h = ShapeSize;
            }
            m_lShapeWidth = w;
            m_lShapeHeight = h;
        }
    }
    inline double GetWidth()
    {
        return m_lShapeWidth;
    }
    inline double GetHeight()
    {
        return m_lShapeHeight;
    }
    
public:
    CFormulaManager(std::vector<long>& a, std::vector<double>& g)
    {
        Adjustments = &a;
        Guides = &g;
        
        m_lShapeWidth = ShapeSize;
        m_lShapeHeight = ShapeSize;
        
        AddGuide(_T("3cd4"), _T("val 16200000"));	//The units here are in 60,000ths of a degree. This is equivalent to 270 degrees.
        AddGuide(_T("3cd8"), _T("val 8100000"));	//The units here are in 60,000ths of a degree. This is equivalent to 135 degrees.
        AddGuide(_T("5cd8"), _T("val 13500000"));	//The units here are in 60,000ths of a degree. This is equivalent to 225 degrees.
        AddGuide(_T("7cd8"), _T("val 18900000"));	//The units here are in 60,000ths of a degree. This is equivalent to 315 degrees.
        AddGuide(_T("b"), _T("val h"));				//This is the bottom edge of the shape and since the top edge of the shape is considered the 0 point, the
        //bottom edge is thus the shape height.
        AddGuide(_T("cd2"), _T("val 10800000"));	//The units here are in 60,000ths of a degree. This is equivalent to 180 degrees.
        AddGuide(_T("cd4"), _T("val 5400000"));		//The units here are in 60,000ths of a degree. This is equivalent to 90 degrees.
        AddGuide(_T("cd8"), _T("val 2700000"));		//The units here are in 60,000ths of a degree. This is equivalent to 45 degrees.
        AddGuide(_T("hc"), _T("*/ w 1 2"));			//This is the horizontal center of the shape which is just the width divided by 2.
        AddGuide(_T("hd2"), _T("*/ h 1 2"));		//This is 1/2 the shape height.
        AddGuide(_T("hd3"), _T("*/ h 1 3"));		//This is 1/3 the shape height.
        AddGuide(_T("hd4"), _T("*/ h 1 4"));		//This is 1/4 the shape height.
        AddGuide(_T("hd5"), _T("*/ h 1 5"));		//This is 1/5 the shape height.
        AddGuide(_T("hd6"), _T("*/ h 1 6"));		//This is 1/6 the shape height.
        AddGuide(_T("hd8"), _T("*/ h 1 8"));		//This is 1/8 the shape height.
        AddGuide(_T("l"), _T("val 0"));				//This is the left edge of the shape and the left edge of the shape is considered the horizontal 0 point.
        AddGuide(_T("ls"), _T("max w h"));			//This is the longest side of the shape. This value is either the width or the height depending on which is greater.
        AddGuide(_T("r"), _T("val w"));				//This is the right edge of the shape and since the left edge of the shape is considered the 0 point, the right edge
        //is thus the shape width.
        AddGuide(_T("ss"), _T("min w h"));			//This is the shortest side of the shape. This value is either the width or the height depending on which is
        //smaller.
        AddGuide(_T("ssd2"), _T("*/ ss 1 2"));		//This is 1/2 the shortest side of the shape.
        AddGuide(_T("ssd4"), _T("*/ ss 1 4"));		//This is 1/4 the shortest side of the shape.
        AddGuide(_T("ssd6"), _T("*/ ss 1 6"));		//This is 1/6 the shortest side of the shape.
        AddGuide(_T("ssd8"), _T("*/ ss 1 8"));		//This is 1/8 the shortest side of the shape.
        AddGuide(_T("t"), _T("val 0"));				//This is the top edge of the shape and the top edge of the shape is considered the vertical 0 point.
        AddGuide(_T("vc"), _T("*/ h 1 2"));			//This is the vertical center of the shape which is just the height divided by 2.
        AddGuide(_T("wd2"), _T("*/ w 1 2"));		//This is 1/2 the shape width.
        AddGuide(_T("wd4"), _T("*/ w 1 4"));		//This is 1/4 the shape width.
        AddGuide(_T("wd5"), _T("*/ w 1 5"));		//This is 1/5 the shape width.
        AddGuide(_T("wd6"), _T("*/ w 1 6"));		//This is 1/6 the shape width.
        AddGuide(_T("wd8"), _T("*/ w 1 8"));		//This is 1/8 the shape width.
        AddGuide(_T("wd10"), _T("*/ w 1 10"));		//This is 1/10 the shape width.
        AddGuide(_T("wd32"), _T("*/ w 1 32"));		//This is 1/32 the shape width.
    }
    
    ~CFormulaManager()
    {
        mapAdjustments.clear();
        mapGuides.clear();
        strAdjustments.clear();
        strGuides.clear();
        Adjustments->clear();
        Guides->clear();
    }
    
    CFormulaManager& operator =(const CFormulaManager& manager)
    {
        m_lShapeWidth = manager.m_lShapeWidth;
        m_lShapeHeight = manager.m_lShapeWidth;
        
        mapAdjustments.clear();
        for (std::map<std::wstring, long>::const_iterator pPair = manager.mapAdjustments.begin(); pPair != manager.mapAdjustments.end(); ++pPair)
        {
            mapAdjustments.insert(std::pair<std::wstring, long>(pPair->first, pPair->second));
        }

        mapGuides.clear();
        for (std::map<std::wstring, long>::const_iterator pPair = manager.mapGuides.begin(); pPair != manager.mapGuides.end(); ++pPair)
        {
            mapGuides.insert(std::pair<std::wstring, long>(pPair->first, pPair->second));
        }
        
        strAdjustments.clear();
        for(size_t i = 0; i < manager.strAdjustments.size(); i++)
        {
            strAdjustments.push_back(manager.strAdjustments[i]);
        }

        strGuides.clear();
        for(size_t i = 0; i < manager.strGuides.size(); i++)
        {
            strGuides.push_back(manager.strGuides[i]);
        }
        
        Adjustments->clear();
        for(size_t i = 0; i < manager.Adjustments->size(); i++)
        {
            Adjustments->push_back((*manager.Adjustments)[i]);
        }
        
        Guides->clear();
        for(size_t i = 0; i < manager.Guides->size(); i++)
        {
            Guides->push_back((*manager.Guides)[i]);
        }
        
        return *this;
    }
    
    void AddAdjustment(const std::wstring& name, const std::wstring& fmla)
    {
        std::map<std::wstring, long>::const_iterator pPair = mapAdjustments.find(name);

        if(pPair != mapAdjustments.end())
        {
            strAdjustments[pPair->second].FromString(fmla);
            (*Adjustments)[pPair->second] = NonDefResult;

            return;
        }
        CFormula formula( (int)strAdjustments.size() + 1);
        formula.m_sName = name;
        formula.FromString(fmla);

        strAdjustments.push_back(formula);
        Adjustments->push_back(NonDefResult);

        mapAdjustments.insert(std::pair<std::wstring, long>(name, (long)strAdjustments.size() - 1));
    }
    
    void AddGuide(const std::wstring& name, const std::wstring& fmla)
    {
        /*
                        long num = mapGuides.FindKey(name);
                        if(num >= 0)
                        {
                                strGuides[mapGuides.GetValueAt(num)].FromString(fmla);
                                (*Guides)[mapGuides.GetValueAt(num)] = dNonDefResult;
                                return;
                        }
                        */
        // формулы могут повторяться!!!
        // тогда по мере расчитывания они перетирают друг друга
        CFormula formula( -1 -(int)strGuides.size());
        formula.m_sName = name;
        formula.FromString(fmla);

        strGuides.push_back(formula);
        Guides->push_back(dNonDefResult);

        mapGuides.insert(std::pair<std::wstring, long>(name, (long)strGuides.size() - 1));
    }
    
    double GetValue(std::wstring str)
    {
        if(str == _T("w")) return m_lShapeWidth;
        if(str == _T("h")) return m_lShapeHeight;
        
        std::map<std::wstring, long>::iterator numGuide	= mapGuides.find(str);
        std::map<std::wstring, long>::iterator numAdj	= mapAdjustments.find(str);

        if(numGuide != mapGuides.end())
        {
            double res = (*Guides)[numGuide->second];
            if(res < dNonDefResult)
                return res;
            return strGuides[numGuide->second].Calculate(this);
        }
        if(numAdj != mapAdjustments.end())
        {
            long res = (*Adjustments)[numAdj->second];
            if(res != NonDefResult)
                return res;
            return strAdjustments[numAdj->second].Calculate(this);
        }
        return XmlUtils::GetInteger(str);
    }
    
    void Clear()
    {
        //for(long i = 0; i < Adjustments.size(); i++)
        //	Adjustments[i] = NonDefResult;
        for(size_t i = 0; i < Guides->size(); i++)
            (*Guides)[i] = dNonDefResult;
    }
    
    void ReCalculateGuides()
    {
        Clear();
        for(size_t i = 0; i <  strGuides.size(); i++)
            (*Guides)[i] = strGuides[i].Calculate(this);
    }
};
}
