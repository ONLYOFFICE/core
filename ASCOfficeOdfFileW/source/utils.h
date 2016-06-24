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
#pragma once

#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
	#include <atlbase.h>
	#include <gdiplus.h>
	#pragma comment(lib, "gdiplus.lib")
#endif

#include "../../../DesktopEditor/raster/BgraFrame.h"
#include "../../../ASCOfficeOdfFile/src/docx/measuredigits.h"

class CFontManager;

namespace _graphics_utils_
{
        bool static GetResolution(const wchar_t* fileName, double & Width, double &Height) //pt
	{
                bool result =false;

                CBgraFrame image;
                if (result = image.OpenFile(fileName, 0 ))
                {
                        Width  = image.get_Width();
                        Height = image.get_Height();

                        result = true;
                }
                else
                {
#if defined(_WIN32) || defined(_WIN64)
                    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
                    ULONG_PTR gdiplusToken=0;
                    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

                    Gdiplus::Bitmap *file = new Gdiplus::Bitmap(fileName,false);
                    if ((file) && (file->GetLastStatus()==Gdiplus::Ok))
                    {
                            Height = file->GetHeight();
                            Width  = file->GetWidth();

                            double dpi_x = file->GetHorizontalResolution();
                            double dpi_y = file->GetVerticalResolution();

                            if (dpi_x <1 )dpi_x = 96;
                            if (dpi_y <1 )dpi_y = 96;

                            Height = Height *72. / dpi_y;
                            Width = Width * 72. /dpi_x;

                            result = true;
                            delete file;
                    }
                    Gdiplus::GdiplusShutdown(gdiplusToken);
#endif
                }
		return result;
	}
        double	static calculate_size_symbol_win(std::wstring name, double size, bool italic, bool bold, std::wstring test_str = L"")
	{
		double result =0;
#if defined(_WIN32) || defined(_WIN64)
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken=0;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		////
		bool to_one_char = false;
		if (test_str.length() <1 )
		{
			test_str = L"0123456789";
			to_one_char = true;
		}

		int style = Gdiplus::FontStyleRegular;
		if (bold && italic)	style = Gdiplus::FontStyleBoldItalic;
		else if (bold)		style = Gdiplus::FontStyleBold;
		else if (italic)	style = Gdiplus::FontStyleItalic;

		Gdiplus::Graphics * gr = new Gdiplus::Graphics(GetWindowDC(NULL));
		if (gr)
		{
			Gdiplus::Font *font = new Gdiplus::Font(name.c_str(),size,style);
			if (font)
			{

				Gdiplus::SizeF layout;
					
				Gdiplus::RectF bound;
				Gdiplus::Status res = gr->MeasureString(test_str.c_str(),test_str.length(),font,layout,&bound);

				if (res==0)result = (bound.Width - 2);
				if (to_one_char) result /= test_str.length();

				//normalize to dpi = 96;
				double dpi = gr->GetDpiX();

				result = result * 96./dpi;

				delete font;
			}
			delete gr;
		}
		Gdiplus::GdiplusShutdown(gdiplusToken);
#endif
		return result;
	}
        double	static calculate_size_symbol_asc(std::wstring name, double size, bool italic, bool bold , CApplicationFonts *appFonts)
	{
            std::pair<float,float> val = cpdoccore::utils::GetMaxDigitSizePixels(name, size, 96., 0 , appFonts);

            return val.first;
	}
};
