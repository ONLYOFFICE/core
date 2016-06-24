/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include <logging.h>

#include <float.h>

#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

namespace cpdoccore {
namespace utils {

namespace 
{
    class Error : public std::exception {};    
    class LoadFontError : public Error {};
    class MeasureError : public Error {};
}

std::pair<float, float> GetMaxDigitSizePixelsImpl(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, CFontManager *pFontManager)
{
    if (pFontManager == NULL) return std::pair<float, float>(7,8);

   int hr = FALSE;

    if (FALSE == (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
        if (FALSE == (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
            return std::pair<float, float>(7, 8);
		}
	}

    float maxWidth = 0;
    float maxHeight = 0;

	float minWidth = 0xffff;
	float minHeight = 0xffff;

   // for (int i = 0; i <= 9; ++i)
    {
		//if (FALSE == (hr = pFontManager->LoadString2( boost::lexical_cast<std::wstring>(i), 0, 0)))
		//	return std::pair<float, float>(7,8);

		if (FALSE == (hr = pFontManager->LoadString2( L"xxxxx" , 0, 0)))
			return std::pair<float, float>(7,8);

		TBBox box;
		try
		{
           box = pFontManager->MeasureString();
		}catch(...)
		{
            return std::pair<float, float>(7,8);
		}

		if (box.fMaxX < -0xffff+1 || box.fMaxY < -0xffff+1 ||
			box.fMinX > 0xffff-1 || box.fMinY > 0xffff-1)		
				return std::pair<float, float>(7,8);
          
        if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;
       
		if (box.fMaxX - box.fMinX < minWidth)   minWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY < minHeight)  minHeight = box.fMaxY - box.fMinY;
    }

	double width = (minWidth + 2 * maxWidth) /3. /5.;
	return std::pair<float, float>(width, maxHeight);
}


std::pair<float, float> GetMaxDigitSizePixels(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, CApplicationFonts *appFonts)
{
    try 
    {
        _CP_LOG << "[info] : GetMaxDigitSizePixels...";

        if (appFonts)
        {
            CFontManager *pFontManager = appFonts->GenerateFontManager();

            std::pair<float, float> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, dpi, fontStyle, pFontManager);

            if (pFontManager)
            {
                pFontManager->m_pApplication = NULL;
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
	return std::pair<float, float>(7, 8);
}

}
}

