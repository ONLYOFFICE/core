#include "measuredigits.h"

#include <boost/lexical_cast.hpp>
#include <logging.h>

#include <float.h>

#include "../../DesktopEditor/fontengine/FontManager.h"

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
    if (pFontManager == NULL) return std::pair<float, float>(7.f,8.f);

    HRESULT hr = S_OK;

    if (S_OK != (hr = pFontManager->LoadFontByName(fontName, fontSize, fontStyle, dpi, dpi )))
	{
        if (S_OK != (hr = pFontManager->LoadFontByName(L"Arial", fontSize, fontStyle, dpi, dpi )))
		{
            return std::pair<float, float>(7,8);
		}
	}

    float maxWidth = 0;
    float maxHeight = 0;
    for (int i = 0; i <= 9; ++i)
    {
        if (S_OK != (hr = pFontManager->LoadString2( boost::lexical_cast<std::wstring>(i).c_str(), 0, 0)))
            return std::pair<float, float>(7,8);

       TBBox box;
		try
		{
           box = pFontManager->MeasureString();
		}catch(...)
		{
            return std::pair<float, float>(7,8);
		}
          
        if (box.fMaxX - box.fMinX > maxWidth)   maxWidth = box.fMaxX - box.fMinX;
        if (box.fMaxY - box.fMinY > maxHeight)  maxHeight = box.fMaxY - box.fMinY;
    }

    return std::pair<float, float>(maxWidth,maxHeight);
}


std::pair<float, float> GetMaxDigitSizePixels(const std::wstring & fontName, double fontSize, double dpi, long fontStyle, CFontManager *pFontManager)
{
    try 
    {
        _CP_LOG(info) << "[info] : GetMaxDigitSizePixels...";
        std::pair<float, float> val = GetMaxDigitSizePixelsImpl(fontName, fontSize, dpi, fontStyle, pFontManager);
        _CP_LOG(info) << "ok" << std::endl;
        return val;
    }
    catch(...)
    {
        // TODO: default value!
        return std::pair<float, float>(7,8);
    }    
}

}
}

