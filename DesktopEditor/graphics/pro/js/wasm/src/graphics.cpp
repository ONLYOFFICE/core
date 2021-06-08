#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include <malloc.h>
#include <iostream>

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
    if (pGraphics) delete pGraphics;
}
WASM_EXPORT void  Graphics_DestroyFrame(CBgraFrame* p)
{
    if (p) delete p;
}
WASM_EXPORT void  Graphics_CreateFromBgraFrame(void* graphics, CBgraFrame* pFrame)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CreateFromBgraFrame(pFrame);
}
WASM_EXPORT void* Graphics_Init(CBgraFrame* pFrame, double width_mm, double height_mm)
{
    /*
    NSFonts::IApplicationFonts* m_pApplicationFonts = NSFonts::NSApplication::Create();
    std::vector<std::wstring> test;
    m_pApplicationFonts->InitializeFromArrayFiles(test, 0);
    NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();
    */

    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)NSGraphics::Create();
    //pGraphics->SetFontManager(pManager);
    pGraphics->CreateFromBgraFrame(pFrame);
    pGraphics->SetSwapRGB(true);
    pGraphics->put_Width(width_mm);
    pGraphics->put_Height(height_mm);
    return pGraphics;
}
WASM_EXPORT CBgraFrame* Graphics_InitFrame(double width_px, double height_px)
{
    int nRasterW = (int)width_px;
    int nRasterH = (int)height_px;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame* oRes = new CBgraFrame();
    oRes->put_IsRGBA(true);
    oRes->put_Data(pData);
    oRes->put_Width(nRasterW);
    oRes->put_Height(nRasterH);
    return oRes;
}
WASM_EXPORT void  Graphics_put_GlobalAlpha(void* graphics, bool enable, double alpha)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_GlobalAlphaEnabled(enable, alpha);
}
WASM_EXPORT void  Graphics_b_color1(void* graphics, int r, int g, int b, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_BrushColor2(r | (g << 8) | (b << 16));
    pGraphics->put_BrushAlpha2(a);
}
WASM_EXPORT void  Graphics_b_color2(void* graphics, int r, int g, int b, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_BrushType(c_BrushTypeSolid);
    pGraphics->put_BrushColor1(r | (g << 8) | (b << 16));
    pGraphics->put_BrushAlpha1(a);
}
WASM_EXPORT void  Graphics_End_GlobalAlpha(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    bool bIsInteger = pGraphics->get_IntegerGrid();
    pGraphics->put_IntegerGrid(true);

    pGraphics->PathCommandEnd();
    Graphics_b_color1(graphics, 255, 255, 255, 140);

    pGraphics->AddRect(0.0, 0.0, pGraphics->GetPixW(), pGraphics->GetPixH());
    pGraphics->Fill();
    pGraphics->PathCommandEnd();

    pGraphics->put_IntegerGrid(bIsInteger);
}
WASM_EXPORT void  Graphics_p_color(void* graphics, int r, int g, int b, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenColor(r | (g << 8) | (b << 16));
    pGraphics->put_PenAlpha(a);
}
WASM_EXPORT void  Graphics_p_width(void* graphics, double w)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenSize(w / 1000.0);
}
WASM_EXPORT void  Graphics_p_dash(void* graphics, int length, double* dash)
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
WASM_EXPORT void  Graphics_transform(void* graphics, double sx, double shy, double shx, double sy, double tx, double ty)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetTransform(sx, shy, shx, sy, tx, ty);
}
WASM_EXPORT void  Graphics_CalculateFullTransform(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CalculateFullTransform();
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
WASM_EXPORT void  Graphics_z(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandClose();
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
WASM_EXPORT void  Graphics_c(void* graphics, double x1, double y1, double x2, double y2, double x3, double y3)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (!pGraphics->get_IntegerGrid())
        pGraphics->PathCommandCurveTo(x1, y1, x2, y2, x3, y3);
    else
    {
        pGraphics->GetFullTransform()->TransformPoint(x1, y1);
        pGraphics->GetFullTransform()->TransformPoint(x2, y2);
        pGraphics->GetFullTransform()->TransformPoint(x3, y3);
        pGraphics->PathCommandCurveTo((int)x1 + 0.5, (int)y1 + 0.5, (int)x2 + 0.5, (int)y2 + 0.5, (int)x3 + 0.5, (int)y3 + 0.5);
    }
}
WASM_EXPORT void  Graphics_c2(void* graphics, double x1, double y1, double x2, double y2)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (!pGraphics->get_IntegerGrid())
        pGraphics->PathCommandCurveTo(x1, y1, x1, y1, x2, y2);
    else
    {
        pGraphics->GetFullTransform()->TransformPoint(x1, y1);
        pGraphics->GetFullTransform()->TransformPoint(x2, y2);
        pGraphics->PathCommandCurveTo((int)x1 + 0.5, (int)y1 + 0.5, (int)x1 + 0.5, (int)y1 + 0.5, (int)x2 + 0.5, (int)y2 + 0.5);
    }
}
WASM_EXPORT void  Graphics_ds(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->Stroke();
}
WASM_EXPORT void  Graphics_df(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->Fill();
}
WASM_EXPORT BYTE* Graphics_GetRGBA(CBgraFrame* p)
{
    BYTE* buffer = NULL;
    if (p) buffer = p->get_Data();
    return buffer;
}
WASM_EXPORT void  Graphics_restore(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->BeginCommand(c_nResetClipType);
    pGraphics->EndCommand  (c_nResetClipType);
}
WASM_EXPORT void  Graphics_clip(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->BeginCommand(c_nClipType);
    pGraphics->EndCommand  (c_nClipType);
}
WASM_EXPORT void  Graphics_reset(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->ResetTransform();
}
WASM_EXPORT void  Graphics_FreeFont(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CloseFont();
}
WASM_EXPORT void  Graphics_ClearLastFont(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->ClearInstallFont();
}
WASM_EXPORT void  Graphics_drawImage(void* graphics, BYTE* img, int size, double x, double y, double w, double h)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    CBgraFrame* pimg = new CBgraFrame();
    pimg->put_IsRGBA(true);
    pimg->Decode(img, size);
    Aggplus::CImage* pImg = new Aggplus::CImage();
    pImg->Create(pimg->get_Data(), pimg->get_Width(), pimg->get_Height(), pimg->get_Stride());
    pGraphics->DrawImage(pImg, x, y, w, h);
    RELEASEOBJECT(pImg);
    pimg->put_Data(NULL);
    RELEASEOBJECT(pimg);
}

WASM_EXPORT void  Graphics_SetFont(void* graphics, BYTE* pData, LONG lSize, const char* name, int face, double size, int style)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    std::wstring sName = UTF8_TO_U(std::string(name));
    double DpiX, DpiY;
    pGraphics->get_DpiX(&DpiX);
    pGraphics->get_DpiY(&DpiY);
    pGraphics->GetFontManager()->LoadFontByName(sName, pData, lSize, size, style, DpiX, DpiY);
    //pGraphics->GetFontManager()->LoadFontByName(sName, size, style, DpiX, DpiY);

    pGraphics->put_FontName     (sName);
    pGraphics->put_FontFaceIndex(face);
    pGraphics->put_FontSize     (size);
    pGraphics->put_FontStyle    (style);
}
WASM_EXPORT void  Graphics_FillText(void* graphics, double x, double y, int text)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CommandDrawTextCHAR(text, x, y, 0, 0);
}

WASM_EXPORT void  Graphics_tg(void* graphics, int text, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_FontStringGID(TRUE);
    pGraphics->CommandDrawTextCHAR(text, x, y, 0, 0);
    pGraphics->put_FontStringGID(FALSE);
}
WASM_EXPORT void  Graphics_SetIntegerGrid(void* graphics, bool param)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_IntegerGrid(param);
}
WASM_EXPORT bool  Graphics_GetIntegerGrid(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    return pGraphics->get_IntegerGrid();
}
WASM_EXPORT void  Graphics_rect(void* graphics, double x, double y, double w, double h)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandEnd();
    if (pGraphics->get_IntegerGrid())
    {
        double r = x + w;
        double b = y + h;

        pGraphics->GetFullTransform()->TransformPoint(x, y);
        pGraphics->GetFullTransform()->TransformPoint(r, b);

        x = (int)(x + 0.5);
        y = (int)(y + 0.5);
        r = (int)(r + 0.5);
        b = (int)(b + 0.5);

        pGraphics->AddRect(x, y, r - x, b - y);
    }
    else
        pGraphics->AddRect(x, y, w, h);
}
WASM_EXPORT void  Graphics_StartClipPath(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->BeginCommand(c_nClipType);
}
WASM_EXPORT void  Graphics_EndClipPath(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->EndCommand(c_nClipType);
}

WASM_EXPORT void  Graphics_put_brushTextureMode(void* graphics, int mode)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_BrushTextureMode(mode);
}
WASM_EXPORT void  Graphics_put_BrushTextureAlpha(void* graphics, int a)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_BrushTextureAlpha(a == 0 ? 255 : a);
}
WASM_EXPORT void  Graphics_put_BrushGradient(void* graphics, LONG* pColors, double* pPositions, size_t nCount, double x0, double y0, double x1, double y1, double r0, double r1)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (r0 > 0)
    {
        // линейный
        double dAngle = 0;
        if (fabs(x1 - x0) >= FLT_EPSILON || fabs(y1 - y0) >= FLT_EPSILON)
            dAngle = atan2(y1 - y0, x1 - x0) * 180 / 3.14159265358979323846;
        pGraphics->put_BrushType(c_BrushTypePathGradient1);
        pGraphics->put_BrushGradientColors(pColors, pPositions, nCount);
        pGraphics->put_BrushLinearAngle(dAngle);
    }
    else
    {
        // радиальный
        pGraphics->put_BrushType(c_BrushTypePathGradient2);
        pGraphics->put_BrushGradientColors(pColors, pPositions, nCount);
    }
}
WASM_EXPORT void  Graphics_put_TextureBounds(void* graphics, double x, double y, double w, double h)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    if (pGraphics->get_IntegerGrid())
    {
        double r = x + w;
        double b = y + h;
        pGraphics->GetFullTransform()->TransformPoint(x, y);
        pGraphics->GetFullTransform()->TransformPoint(r, b);
        pGraphics->BrushBounds(x, y, r - x, b - y);
    }
    else
        pGraphics->BrushBounds(x, y, w, h);
}
WASM_EXPORT void  Graphics_DrawPath(void* graphics, int path)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->DrawPath(path);
}
WASM_EXPORT void  Graphics_CoordTransformOffset(void* graphics, double dOffsetX, double dOffsetY)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetCoordTransformOffset(dOffsetX, dOffsetY);
}
WASM_EXPORT int   Graphics_GetFrameHeight(CBgraFrame* p)
{
    if (p) return p->get_Height();
    return -1;
}
WASM_EXPORT int   Graphics_GetFrameWidth (CBgraFrame* p)
{
    if (p) return p->get_Width();
    return -1;
}

WASM_EXPORT void  Graphics_SetFontManager(void* graphics, NSFonts::IFontManager* pManager = NULL)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetFontManager(pManager);
}
WASM_EXPORT void  Graphics_put_PenSize(void* graphics, double w)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->put_PenSize(w);
}
WASM_EXPORT void  Graphics_PathCommandMoveTo(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandMoveTo(x, y);
}
WASM_EXPORT void  Graphics_PathCommandLineTo(void* graphics, double x, double y)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->PathCommandLineTo(x, y);
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
    CBgraFrame* testFrame = Graphics_InitFrame(211, 119);
    void* testGraphics = Graphics_Init(testFrame, 55.8251, 31.2208);
    Graphics_CoordTransformOffset(testGraphics, -210.583, -111.159);
    Graphics_transform(testGraphics, 1, 0, 0, 1, 0, 0);
    Graphics_SetIntegerGrid(testGraphics, false);
    Graphics_transform(testGraphics, 1, 0, 0, 1, 58.4824, 30.8707);
    Graphics_b_color1(testGraphics, 0, 0, 0, 255);

    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(L"C:/Windows/Fonts/arial.ttf", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();

    Graphics_SetFont(testGraphics, pData, nBytesCount, "Arial", -1, 11, 0);
    Graphics_FillText(testGraphics, 0, 3.63992, 't');
    Graphics_SetFont(testGraphics, pData, nBytesCount, "Arial", -1, 11, 0);
    Graphics_FillText(testGraphics, 1.07813, 3.63992, 'e');
    Graphics_SetFont(testGraphics, pData, nBytesCount, "Arial", -1, 11, 0);
    Graphics_FillText(testGraphics, 3.23627, 3.63992, 's');
    Graphics_SetFont(testGraphics, pData, nBytesCount, "Arial", -1, 11, 0);
    Graphics_FillText(testGraphics, 5.17651, 3.63992, 't');

    RELEASEARRAYOBJECTS(pData);
    Graphics_SetIntegerGrid(testGraphics, true);
    Graphics_reset(testGraphics);
    /*
    CBgraFrame* testFrame = Raster_Init(1024, 1024);
    void* testGraphics = Graphics_Init(testFrame, 256, 256);
    Graphics_transform(testGraphics, 1, 0, 0, 1, 0, 0);

    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.png", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();
    Graphics_drawImage(testGraphics, pData, nBytesCount, 0, 0, 256, 256);

    Graphics_DrawPath(testGraphics, 256);
    Graphics_reset(testGraphics);
    */
    /*
    CBgraFrame* testFrame = Graphics_InitFrame(412, 151);
    void* testGraphics = Graphics_Init(testFrame, 109.008, 39.9521);
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
    */
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

    BYTE* res = Graphics_GetRGBA(testFrame);
    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";

    testFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    Graphics_DestroyFrame(testFrame);
    Graphics_Destroy(testGraphics);
    return 0;
}
#endif

#endif // _WASM_GRAPHICS_
