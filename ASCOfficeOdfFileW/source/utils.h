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
