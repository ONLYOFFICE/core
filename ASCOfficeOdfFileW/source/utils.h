#pragma once

#include <string>
#include <vector>

#if defined(_WIN32) || defined(_WIN64)
	#include <atlbase.h>
	#include <gdiplus.h>
	#pragma comment(lib, "gdiplus.lib")

	#import "../../../Redist/ASCGraphics.dll" rename_namespace("ASCGraphics")
#endif
namespace _gdi_graphics_
{
	//todoooo переписать  !!!

	std::pair<double,double> static GetMaxDigitSizePixelsImpl(const wchar_t * fontName, float fontSize, float dpi, long fontStyle)
	{
		float width = 70, height = 80;

#if defined(_WIN32) || defined(_WIN64)
		CComPtr<ASCGraphics::IASCFontManager> fontMan;
		HRESULT hr;
		
		try
		{
			if (FAILED(hr = fontMan.CoCreateInstance(__uuidof(ASCGraphics::CASCFontManager))))
				throw;

			if (S_OK != (hr = fontMan->Initialize(L"")))
				throw;
		}
		catch(...)
		{
			return std::pair<double,double>(7,8);
		}

		if (S_OK != (hr = fontMan->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
		{
			if (S_OK != (hr = fontMan->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
			{
				return std::pair<double,double>(7,8);
			}
		}
		double maxWidth=0 ;
		float x, y;
		std::wstring TestString = L"0123456789";


		if (S_OK != (hr = fontMan->LoadString2( TestString.c_str(), 0, 0)))
			return std::pair<double,double>(7,8);

		try
		{
			hr = fontMan->MeasureString(&x, &y, &width, &height);
		}catch(...)
		{
			return std::pair<double,double>(7,8);
		}
#endif	          
		return std::pair<double,double>(width/10.,height/10.);
	}


	bool static GetResolution(const WCHAR* fileName, double & Width, double &Height) //pt
	{
		bool result =false;
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
		return result;
	}
	double	static calculate_size_symbol(std::wstring name, double size, bool italic, bool bold, std::wstring test_str = L"")
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
	double	static calculate_size_symbol_asc(std::wstring name, double size, bool italic, bool bold)
	{
		std::pair<float,float> val = GetMaxDigitSizePixelsImpl(name.c_str(), size, 96., 0);

		return val.first;

	}
};