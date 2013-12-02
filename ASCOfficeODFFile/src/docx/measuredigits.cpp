#include "precompiled_cpodf.h"

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif						
#include <atlbase.h>
#include <boost/lexical_cast.hpp>
#include <logging.h>
#import "../../../Redist/ASCGraphics.dll" rename_namespace("ASCGraphics")

namespace cpdoccore {
namespace utils {

namespace 
{
    class Error : public std::exception {};    
    class LoadFontError : public Error {};
    class MeasureError : public Error {};
}

std::pair<float,float> GetMaxDigitSizePixelsImpl(const wchar_t * fontName, float fontSize, float dpi, long fontStyle)
{
    ATL::CComPtr<ASCGraphics::IASCFontManager> fontMan;
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
		return std::pair<float,float>(7,8);
	}

	if (S_OK != (hr = fontMan->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
		if (S_OK != (hr = fontMan->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
			return std::pair<float,float>(7,8);
		}
	}

    float maxWidth = 0;
	float maxHeight = 0;
    for (int i = 0; i <= 9; ++i)
    {
        if (S_OK != (hr = fontMan->LoadString2( boost::lexical_cast<std::wstring>(i).c_str(), 0, 0)))
            throw MeasureError();

        float x, y, width, height;
        if (S_OK != (hr = fontMan->MeasureString(&x, &y, &width, &height)))
            throw MeasureError();

        if (width> maxWidth)
            maxWidth = width;
		if (height>maxHeight)
			maxHeight = height;
    }

    return std::pair<float,float>(maxWidth,maxHeight);
}


std::pair<float,float> GetMaxDigitSizePixels(const wchar_t * fontName, float fontSize, float dpi, long fontStyle)
{
    try 
    {
        _CP_LOG(info) << "[info] : GetMaxDigitSizePixels...";
        std::pair<float,float> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, dpi, fontStyle);
        _CP_LOG(info) << "ok" << std::endl;
        return val;
    }
    catch(...)
    {
        // TODO: default value!
        return std::pair<float,float>(7,8);    
    }    
}

}
}

