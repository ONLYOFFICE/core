#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"
#include "../../../../../common/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"
#include "../../../../../../DjVuFile/DjVu.h"

class CGraphicsFileDrawing
{
private:
    IOfficeDrawingFile* pReader;
    NSFonts::IApplicationFonts* pApplicationFonts;
public:
    CGraphicsFileDrawing()
    {
        pApplicationFonts = NSFonts::NSApplication::Create();
    }
    ~CGraphicsFileDrawing()
    {
        RELEASEOBJECT(pReader);
        RELEASEOBJECT(pApplicationFonts);
    }
    void CreateXPS()
    {
        pReader = new CXpsFile(pApplicationFonts);
    }
    void CreateDjVu()
    {
        pReader = new CDjVuFile(pApplicationFonts);
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
            std::swap_ranges(pixmap_cur, pixmap_cur + nRasterW * 4, pLine);
            pixmap_cur += nRasterW * 4;
        }
        return flipped;
    }
    BYTE* GetXPSGlyphs(int nPageIndex, int nRasterW, int nRasterH)
    {
        return ((CXpsFile*)pReader)->GetGlyphs(nPageIndex, nRasterW, nRasterH);
    }
    BYTE* GetDJVUGlyphs(int nPageIndex, int nRasterW, int nRasterH)
    {
        return ((CDjVuFile*)pReader)->GetPageGlyphs(nPageIndex, nRasterW, nRasterH);
    }
    BYTE* GetXPSStructure()
    {
        return ((CXpsFile*)pReader)->GetStructure();
    }
    BYTE* GetDJVUStructure()
    {
        return ((CDjVuFile*)pReader)->GetStructure();
    }
};

#endif // _WASM_GRAPHICS_
