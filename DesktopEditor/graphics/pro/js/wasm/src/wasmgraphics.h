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
    void GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX)
    {
        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
        nWidth = dWidth;
        nHeight = dHeight;
        nPageDpiX = dPageDpiX;
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
        BYTE* flipped = pReader->ConvertToPixels(nPageIndex, nRasterW, nRasterH);
        BYTE* pixmap_cur = flipped;
        for (int j = nRasterH - 1; j >= nRasterH / 2; --j)
        {
            BYTE* pLine = flipped + (j * nRasterW * 4);
            for (int i = 0; i < nRasterW; ++i, pixmap_cur += 4, pLine += 4)
            {
                BYTE r = pixmap_cur[0];
                BYTE g = pixmap_cur[1];
                BYTE b = pixmap_cur[2];
                BYTE a = pixmap_cur[3];
                pixmap_cur[0] = pLine[0];
                pixmap_cur[1] = pLine[1];
                pixmap_cur[2] = pLine[2];
                pixmap_cur[3] = pLine[3];
                pLine[0] = r;
                pLine[1] = g;
                pLine[2] = b;
                pLine[3] = a;
            }
        }
        return flipped;
    }
};

#endif // _WASM_GRAPHICS_
