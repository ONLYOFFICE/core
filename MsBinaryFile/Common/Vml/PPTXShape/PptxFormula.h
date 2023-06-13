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

		static FormulaType GetFormula(std::wstring strName, bool& bRes);

    public:
        FormulaType m_eFormulaType;
        std::wstring m_sName;
        int m_lIndex;

        std::wstring m_lParam1;
        std::wstring m_lParam2;
        std::wstring m_lParam3;

    public:
		CFormula(long ind = 0);
		CFormula& operator =(const CFormula& src);

		void FromString(std::wstring strFormula);
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
    
	void SetWidthHeight(double w, double h);

    inline double GetWidth()
    {
        return m_lShapeWidth;
    }
    inline double GetHeight()
    {
        return m_lShapeHeight;
    }
    
public:
	CFormulaManager(std::vector<long>& a, std::vector<double>& g);
	~CFormulaManager();
    
	CFormulaManager& operator =(const CFormulaManager& manager);
    
	void AddAdjustment(const std::wstring& name, const std::wstring& fmla);
	void AddGuide(const std::wstring& name, const std::wstring& fmla);

	double GetValue(std::wstring str);
	void Clear();
	void ReCalculateGuides();
};
}
