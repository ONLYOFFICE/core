#pragma once

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

namespace _gdi_graphics_
{
	bool static GetResolution(const WCHAR* fileName, double & Width, double &Height) //pt
	{
		bool result =false;
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
		return result;
	}
	double	static calculate_size_symbol(std::wstring name, double size, bool italic, bool bold)
	{
		double result =0;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken=0;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		////
		std::wstring test_string = L"0123456789";

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
				Gdiplus::Status res = gr->MeasureString(test_string.c_str(),test_string.length(),font,layout,&bound);

				if (res==0)result = bound.Width / test_string.length();
				//normalize to dpi = 96;
	
				double dpi = gr->GetDpiX();

				result = result * 96./dpi;

				delete font;
			}
			delete gr;
		}
		Gdiplus::GdiplusShutdown(gdiplusToken);
		return result;
	}
};