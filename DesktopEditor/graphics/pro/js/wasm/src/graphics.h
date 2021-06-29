#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"
#include "../../../../../common/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"

class CGraphicsFileDrawing
{
private:
    IOfficeDrawingFile* pReader;
    NSFonts::IApplicationFonts* pApplicationFonts;
public:
    CGraphicsFileDrawing()
    {
        pApplicationFonts = NSFonts::NSApplication::Create();
        //pApplicationFonts->Initialize();
        pReader = new CXpsFile(pApplicationFonts);
    }
    ~CGraphicsFileDrawing()
    {
        RELEASEOBJECT(pReader);
        RELEASEOBJECT(pApplicationFonts);
    }

    bool LoadFromMemory(BYTE* data, DWORD length)
    {
        return pReader->LoadFromMemory(data, length);
    }
    int GetPagesCount()
    {
        return pReader->GetPagesCount();
    }
    int GetPageHeight(int nPageIndex)
    {
        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
        return dHeight * 96 / dPageDpiX;
    }
    int GetPageWidth (int nPageIndex)
    {
        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
        return dWidth * 96 / dPageDpiX;
    }
    BYTE* GetPage    (int nPageIndex, int nRasterW, int nRasterH)
    {
        return pReader->ConvertToPixels(nPageIndex, nRasterW, nRasterH);
    }
};

#endif // _WASM_GRAPHICS_
