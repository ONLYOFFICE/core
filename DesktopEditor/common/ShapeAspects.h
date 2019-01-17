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

#ifndef SHAPE_ASPECTS_H
#define SHAPE_ASPECTS_H

#include <map>
#include <string>

class CShapeAspects
{
public:
    CShapeAspects()
    {
        m_aspects[L"can"]                           = 3616635.0 / 4810125.0;
        m_aspects[L"moon"]                          = 457200.0  / 914400.0;
        m_aspects[L"leftBracket"]                   = 73152.0   / 914400.0;
        m_aspects[L"rightBracket"]                  = 73152.0   / 914400.0;
        m_aspects[L"leftBrace"]                     = 155448.0  / 914400.0;
        m_aspects[L"rightBrace"]                    = 155448.0  / 914400.0;
        m_aspects[L"triangle"]                      = 1060704.0 / 914400.0;
        m_aspects[L"parallelogram"]                 = 1216152.0 / 914400.0;
        m_aspects[L"trapezoid"]                     = 914400.0  / 1216152.0;
        m_aspects[L"pentagon"]                      = 960120.0  / 914400.0;
        m_aspects[L"hexagon"]                       = 1060704.0 / 914400.0;
        m_aspects[L"bracePair"]                     = 1069848.0 / 914400.0;
        m_aspects[L"rightArrow"]                    = 978408.0  / 484632.0;
        m_aspects[L"leftArrow"]                     = 978408.0  / 484632.0;
        m_aspects[L"upArrow"]                       = 484632.0  / 978408.0;
        m_aspects[L"downArrow"]                     = 484632.0  / 978408.0;
        m_aspects[L"leftRightArrow"]                = 1216152.0 / 484632.0;
        m_aspects[L"upDownArrow"]                   = 484632.0  / 1216152.0;
        m_aspects[L"bentArrow"]                     = 813816.0  / 868680.0;
        m_aspects[L"uturnArrow"]                    = 886968.0  / 877824.0;
        m_aspects[L"bentUpArrow"]                   = 850392.0  / 731520.0;
        m_aspects[L"curvedRightArrow"]              = 731520.0  / 1216152.0;
        m_aspects[L"curvedLeftArrow"]               = 731520.0  / 1216152.0;
        m_aspects[L"curvedUpArrow"]                 = 1216152.0 / 731520.0;
        m_aspects[L"curvedDownArrow"]               = 1216152.0 / 731520.0;
        m_aspects[L"stripedRightArrow"]             = 978408.0  / 484632.0;
        m_aspects[L"notchedRightArrow"]             = 978408.0  / 484632.0;
        m_aspects[L"homePlate"]                     = 978408.0  / 484632.0;
        m_aspects[L"leftRightArrowCallout"]         = 1216152.0 / 576072.0;
        m_aspects[L"flowChartProcess"]              = 914400.0  / 612648.0;
        m_aspects[L"flowChartAlternateProcess"]     = 914400.0  / 612648.0;
        m_aspects[L"flowChartDecision"]             = 914400.0  / 612648.0;
        m_aspects[L"flowChartInputOutput"]          = 914400.0  / 612648.0;
        m_aspects[L"flowChartPredefinedProcess"]    = 914400.0  / 612648.0;
        m_aspects[L"flowChartDocument"]             = 914400.0  / 612648.0;
        m_aspects[L"flowChartMultidocument"]        = 1060704.0 / 758952.0;
        m_aspects[L"flowChartTerminator"]           = 914400.0  / 301752.0;
        m_aspects[L"flowChartPreparation"]          = 1060704.0 / 612648.0;
        m_aspects[L"flowChartManualInput"]          = 914400.0  / 457200.0;
        m_aspects[L"flowChartManualOperation"]      = 914400.0  / 612648.0;
        m_aspects[L"flowChartPunchedCard"]          = 914400.0  / 804672.0;
        m_aspects[L"flowChartPunchedTape"]          = 914400.0  / 804672.0;
        m_aspects[L"flowChartPunchedTape"]          = 457200.0  / 914400.0;
        m_aspects[L"flowChartSort"]                 = 457200.0  / 914400.0;
        m_aspects[L"flowChartOnlineStorage"]        = 914400.0  / 612648.0;
        m_aspects[L"flowChartMagneticDisk"]         = 914400.0  / 612648.0;
        m_aspects[L"flowChartMagneticDrum"]         = 914400.0  / 685800.0;
        m_aspects[L"flowChartDisplay"]              = 914400.0  / 612648.0;
        m_aspects[L"ribbon2"]                       = 1216152.0 / 612648.0;
        m_aspects[L"ribbon"]                        = 1216152.0 / 612648.0;
        m_aspects[L"ellipseRibbon2"]                = 1216152.0 / 758952.0;
        m_aspects[L"ellipseRibbon"]                 = 1216152.0 / 758952.0;
        m_aspects[L"verticalScroll"]                = 1033272.0 / 1143000.0;
        m_aspects[L"horizontalScroll"]              = 1143000.0 / 1033272.0;
        m_aspects[L"wedgeRectCallout"]              = 914400.0  / 612648.0;
        m_aspects[L"wedgeRoundRectCallout"]         = 914400.0  / 612648.0;
        m_aspects[L"wedgeEllipseCallout"]           = 914400.0  / 612648.0;
        m_aspects[L"cloudCallout"]                  = 914400.0  / 612648.0;
        m_aspects[L"borderCallout1"]                = 914400.0  / 612648.0;
        m_aspects[L"borderCallout2"]                = 914400.0  / 612648.0;
        m_aspects[L"borderCallout3"]                = 914400.0  / 612648.0;
        m_aspects[L"accentCallout1"]                = 914400.0  / 612648.0;
        m_aspects[L"accentCallout2"]                = 914400.0  / 612648.0;
        m_aspects[L"accentCallout3"]                = 914400.0  / 612648.0;
        m_aspects[L"callout1"]                      = 914400.0  / 612648.0;
        m_aspects[L"callout2"]                      = 914400.0  / 612648.0;
        m_aspects[L"callout3"]                      = 914400.0  / 612648.0;
        m_aspects[L"accentBorderCallout1"]          = 914400.0  / 612648.0;
        m_aspects[L"accentBorderCallout2"]          = 914400.0  / 612648.0;
        m_aspects[L"accentBorderCallout3"]          = 914400.0  / 612648.0;
    }
    
    inline double GetAspect(std::wstring sName)
    {
        std::map<std::wstring, double>::iterator oIter = m_aspects.find(sName);
        if (oIter != m_aspects.end())
            return oIter->second;
        
        return 1.0;
    }
    
private:
    std::map<std::wstring, double> m_aspects;
};

#endif
