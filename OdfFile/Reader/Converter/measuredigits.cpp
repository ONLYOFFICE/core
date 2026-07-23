/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "measuredigits.h"

#include <boost/lexical_cast.hpp>

#include "../../Common/logging.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace cpdoccore {
namespace utils {

std::pair<double, double> GetMaxDigitSizePixelsImpl(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, NSFonts::IFontManager *pFontManager)
{
    if (pFontManager == NULL)
    {
        _CP_LOG_CONTINUE << " error FontManager - default set" << std::endl;
        return std::pair<double, double>(7., 8.);
    }

	int hr = FALSE;

    if (FALSE == (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
        _CP_LOG_CONTINUE << " error load font ... try Arial";
        if (FALSE == (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
            _CP_LOG_CONTINUE << "... error - default set" << std::endl;
            return std::pair<double, double>(7, 8);
		}
	}

    double maxWidth = 0;
    double maxHeight = 0;

	double minWidth = 0xffffffff;
	double minHeight = 0xfffffff;

   // for (int i = 0; i <= 9; ++i)
    {
		//if (FALSE == (hr = pFontManager->LoadString2( std::to_wstring(i), 0, 0)))
		//	return std::pair<float, float>(7,8);

        if (FALSE == (hr = pFontManager->LoadString2(L"0123456789", 0, 0)))//
        {
            _CP_LOG_CONTINUE << " error load string - default set" << std::endl;
            return std::pair<double, double>(7., 8.);
        }

		TBBox box;
		try
		{
           box = pFontManager->MeasureString();
		}catch(...)
		{
            _CP_LOG_CONTINUE << " error measure string - default set" << std::endl;
            return std::pair<double, double>(7.,8.);
		}

        if (box.fMaxX < -0xffff + 1 || box.fMaxY < -0xffff + 1 ||
            box.fMinX > 0xffff - 1 || box.fMinY > 0xffff - 1)
        {
            _CP_LOG_CONTINUE << " wrong get box size from FontManager - default set" << std::endl;
            return std::pair<double, double>(7., 8.);
        }
        _CP_LOG_CONTINUE << " box_x(" << std::to_wstring(box.fMinX) << L", " << std::to_wstring(box.fMaxX) << L")";

        TBBox box2;
        box2 = pFontManager->MeasureString2();
        _CP_LOG_CONTINUE << " box2_x(" << std::to_wstring(box2.fMinX) << L", " << std::to_wstring(box2.fMaxX) << L")";
        
        if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;
       
		if (box.fMaxX - box.fMinX < minWidth)   minWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY < minHeight)  minHeight = box.fMaxY - box.fMinY;
    }

    _CP_LOG_CONTINUE << " width(" << std::to_wstring(minWidth) << L", " << std::to_wstring(maxWidth) << L")";
    double width = (minWidth + 2 * maxWidth) /10. /3.;

    if (width > 0.01 && maxHeight > 0.01)
    {
        _CP_LOG_CONTINUE << std::to_wstring(width) << ", " << std::to_wstring(maxHeight) << std::endl;
        return std::pair<double, double>(width, maxHeight);
    }
    else
    {
        _CP_LOG_CONTINUE << " wrong size - default set" << std::endl;
        return std::pair<double, double>(7., 8.);
    }
}


std::pair<double, double> GetMaxDigitSizePixels(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, NSFonts::IApplicationFonts *appFonts)
{
    _CP_LOG << "[info] : GetMaxDigitSizePixels (" << fontName << L" " << std::to_wstring(fontSize) << L"pt style " << std::to_wstring(fontStyle) << L") ...";

    _CP_LOG_CONTINUE;
    try
    {
        if (appFonts)
        {
            NSFonts::IFontManager *pFontManager = appFonts->GenerateFontManager();

            std::pair<double, double> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, dpi, fontStyle, pFontManager);

            if (pFontManager)
            {                
                delete pFontManager;
            }

            return val;
        }
    }
    catch(...)
    {
    }    
    _CP_LOG_CONTINUE << "error .. default set" << std::endl;
    return std::pair<double, double>(7., 8.);
}

}
}

