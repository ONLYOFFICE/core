#include "measuredigits.h"

#include <boost/lexical_cast.hpp>
#include <logging.h>

#include <float.h>

#include "../../DesktopEditor/fontengine/FontManager.h"
#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

double getSystemDPI()
{
	//При запросе системных настроек-проблема в linux без графического интерфейса.
    //Используется в GetMaxDigitSizePixels для измерения символов, можно указывать любой dpi,
    //потому что после измерения pix переводятся обратно в метрические величины.
    //Используется для конвертации картинок с процентными размерами oox->odf. Из редактора никогда не приходят относительные размеры,
    //думаю тут несущественнен dpi.
	return 96.;
}


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
            return std::pair<float, float>(7,8);
		}
	}

    float maxWidth = 0;
    float maxHeight = 0;

	float minWidth = 0xffff;
	float minHeight = 0xffff;

    for (int i = 0; i <= 9; ++i)
    {
        if (FALSE == (hr = pFontManager->LoadString2( boost::lexical_cast<std::wstring>(i), 0, 0)))
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

    return std::pair<float, float>((minWidth + 2*maxWidth)/3.f,maxHeight);
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
	return std::pair<float, float>(7,8);
}

}
}

