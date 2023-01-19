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
#include "measuredigits.h"

#include <boost/lexical_cast.hpp>

#include "../../Common/logging.h"
#include "../../../DesktopEditor/graphics/pro/Fonts.h"

namespace cpdoccore {
namespace utils {

std::pair<double, double> GetMaxDigitSizePixelsImpl(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, NSFonts::IFontManager *pFontManager)
{
    if (pFontManager == NULL) return std::pair<double, double>(7., 8.);

	int hr = FALSE;

    if (FALSE == (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
        if (FALSE == (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
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

		if (FALSE == (hr = pFontManager->LoadString2( L"0123456789abcdefghijklmnopqrstuvwxyz" , 0, 0)))//
			return std::pair<double, double>(7., 8.);

		TBBox box;
		try
		{
           box = pFontManager->MeasureString();
		}catch(...)
		{
            return std::pair<double, double>(7.,8.);
		}

		if (box.fMaxX < -0xffff+1 || box.fMaxY < -0xffff+1 ||
			box.fMinX > 0xffff-1 || box.fMinY > 0xffff-1)		
				return std::pair<double, double>(7., 8. );
          
        if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;
       
		if (box.fMaxX - box.fMinX < minWidth)   minWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY < minHeight)  minHeight = box.fMaxY - box.fMinY;
    }

    double width = (minWidth + 2 * maxWidth) /36. /3.;
	//double width = (minWidth + 2 * maxWidth) / 5./*/36.*/ /3.;

	if (width > 0.01 && maxHeight > 0.01)
		return std::pair<double, double>(width, maxHeight);
	else
		return std::pair<double, double>(7., 8.);
}


std::pair<double, double> GetMaxDigitSizePixels(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, NSFonts::IApplicationFonts *appFonts)
{
    try 
    {
        _CP_LOG << "[info] : GetMaxDigitSizePixels...";

        if (appFonts)
        {
            NSFonts::IFontManager *pFontManager = appFonts->GenerateFontManager();

            std::pair<double, double> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, dpi, fontStyle, pFontManager);

            if (pFontManager)
            {                
                delete pFontManager;
            }

            _CP_LOG << "ok" << std::endl;
            return val;
        }
    }
    catch(...)
    {
        // TODO: default value!
    }    
	return std::pair<double, double>(7., 8.);
}

}
}

