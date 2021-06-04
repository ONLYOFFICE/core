#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include <malloc.h>
#include "raster.h"

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT void* Graphics_Malloc(unsigned int size)
{
    return ::malloc(size);
}
WASM_EXPORT void  Graphics_Free(void* p)
{
    if (p) ::free(p);
}
WASM_EXPORT void* Graphics_Create()
{
    return NSGraphics::Create();
}
WASM_EXPORT void  Graphics_Destroy(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    delete pGraphics;
}
WASM_EXPORT void  Graphics_CreateFromBgraFrame(void* graphics, CBgraFrame* pFrame)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CreateFromBgraFrame(pFrame);
}
WASM_EXPORT void* Graphics_Init(CBgraFrame* pFrame, double width_px, double height_px, double width_mm, double height_mm)
{
    int nRasterW = (int)width_px;
    int nRasterH = (int)height_px;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    while (pData32 < pData32End)
        *pData32++ = back;

    pFrame->put_Data(pData);
    pFrame->put_Width(nRasterW);
    pFrame->put_Height(nRasterH);

    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)NSGraphics::Create();
    pGraphics->CreateFromBgraFrame(pFrame);
    pGraphics->SetSwapRGB(false);
    pGraphics->put_Width(width_mm);
    pGraphics->put_Height(height_mm);
    return pGraphics;
}
WASM_EXPORT void  Graphics_SetFontManager(void* graphics, NSFonts::IFontManager* pManager = NULL)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetFontManager(pManager);
}
WASM_EXPORT void  Graphics_CoordTransformOffset(void* graphics, double dOffsetX, double dOffsetY)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetCoordTransformOffset(dOffsetX, dOffsetY);
}
WASM_EXPORT void  Graphics_transform(void* graphics, double sx, double shy, double shx, double sy, double tx, double ty)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetTransform(sx, shy, shx, sy, tx, ty);
}
WASM_EXPORT void  Graphics_SetIntegerGrid(void* graphics, bool param)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_IntegerGrid(param);
}
WASM_EXPORT void  Graphics_put_PenSize(void* graphics, double w)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenSize(w);
}
WASM_EXPORT void  Graphics_p_width(void* graphics, double w)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenSize(w / 1000.0);
}
WASM_EXPORT void  Graphics_s(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandEnd();
}
WASM_EXPORT void  Graphics_e(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandEnd();
}
WASM_EXPORT void  Graphics_PathCommandMoveTo(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandMoveTo(x, y);
}
WASM_EXPORT void  Graphics_m(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (!pGraphics->get_IntegerGrid())
        pGraphics->PathCommandMoveTo(x, y);
    else
    {
        pGraphics->GetFullTransform()->TransformPoint(x, y);
        pGraphics->PathCommandMoveTo((int)x + 0.5, (int)y + 0.5);
    }
}
WASM_EXPORT void  Graphics_PathCommandLineTo(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandLineTo(x, y);
}
WASM_EXPORT void  Graphics_l(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (!pGraphics->get_IntegerGrid())
        pGraphics->PathCommandLineTo(x, y);
    else
    {
        pGraphics->GetFullTransform()->TransformPoint(x, y);
        pGraphics->PathCommandLineTo((int)x + 0.5, (int)y + 0.5);
    }
}
WASM_EXPORT void  Graphics_put_PenColor(void* graphics, int r, int g, int b)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenColor(r | (g << 8) | (b << 16));
}
WASM_EXPORT void  Graphics_put_PenAlpha(void* graphics, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenAlpha(a);
}
WASM_EXPORT void  Graphics_p_color(void* graphics, int r, int g, int b, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenColor(r | (g << 8) | (b << 16));
    pGraphics->put_PenAlpha(a);
}
WASM_EXPORT void  Graphics_DrawPath(void* graphics, int path)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->DrawPath(path);
}
WASM_EXPORT void  Graphics_p_dash(void* graphics, size_t length, double* dash)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (length > 0)
    {
        double dDpiX = 0;
        pGraphics->get_DpiX(&dDpiX);
        for (size_t i = 0; i < length; i++)
            dash[i] *= (dDpiX / 25.4);

        pGraphics->put_PenDashStyle(Aggplus::DashStyleCustom);
        pGraphics->PenDashPattern(dash, length);
    }
    else
        pGraphics->put_PenDashStyle(Aggplus::DashStyleSolid);
}
WASM_EXPORT void  Graphics_reset(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->ResetTransform();
}
WASM_EXPORT void  Graphics_drawHorLine(void* graphics, BYTE align, double y, double x, double r, double penW)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->drawHorLine(align, y, x, r, penW);
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
int main()
{
    CBgraFrame* testFrame = Raster_Create();
    void* testGraphics = Graphics_Init(testFrame, 412, 151, 109.008, 39.9521);
    Graphics_CoordTransformOffset(testGraphics, -160.294, -109.826);
    Graphics_transform(testGraphics, 1, 0, 0, 1, 0, 0);
    Graphics_SetIntegerGrid(testGraphics, false);
    Graphics_transform(testGraphics, 1, 0, 0, -1, 42.625, 68.6708);
    Graphics_p_width(testGraphics, 352.75);
    Graphics_s(testGraphics);
    Graphics_m(testGraphics, 0, 0);
    Graphics_l(testGraphics, 108.744, 39.4229);
    Graphics_p_color(testGraphics, 255, 0, 0, 255);
    Graphics_DrawPath(testGraphics, 1);
    Graphics_e(testGraphics);
    Graphics_p_dash(testGraphics, 0, NULL);
    Graphics_SetIntegerGrid(testGraphics, true);
    Graphics_reset(testGraphics);
    /*
    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.png", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();
    CBgraFrame* testFrame = Raster_Load(pData, nBytesCount);
    Graphics_CreateFromBgraFrame(testGraphics, testFrame);
    */

    testFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    Raster_Destroy(testFrame);
    Graphics_Destroy(testGraphics);
    return 0;
}
#endif

#endif // _WASM_GRAPHICS_
