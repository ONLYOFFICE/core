#include <malloc.h>
#include <iostream>

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"
#include "../../../../../common/Base64.h"
#include "wasmgraphics.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

CGlobalFontsMemoryStorage* CApplicationFontStreams::m_pMemoryStorage = NULL;

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT CGraphicsFileDrawing* XPS_Load(BYTE* data, LONG size)
{
    CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing();
    if (!CApplicationFontStreams::m_pMemoryStorage)
        CApplicationFontStreams::m_pMemoryStorage = new CGlobalFontsMemoryStorage();
    pGraphics->LoadFromMemory(data, size);
    return pGraphics;
}
WASM_EXPORT void  XPS_Close(CGraphicsFileDrawing* file)
{
    delete file;
    RELEASEOBJECT(CApplicationFontStreams::m_pMemoryStorage);
}
WASM_EXPORT int*  XPS_GetInfo(CGraphicsFileDrawing* pGraphics)
{
    int pages_count = pGraphics->GetPagesCount();
    int* buffer = new int[pages_count * 3 + 1];
    int buffer_index = 0;
    buffer[buffer_index++] = pages_count;
    for (int page = 0; page < pages_count; ++page)
    {
        int nW = 0;
        int nH = 0;
        int nDpi = 0;
        pGraphics->GetPageInfo(page, nW, nH, nDpi);
        buffer[buffer_index++] = nW;
        buffer[buffer_index++] = nH;
        buffer[buffer_index++] = nDpi;
    }
    return buffer;
}
WASM_EXPORT BYTE* XPS_GetPixmap(CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH)
{
    return pGraphics->GetPage(nPageIndex, nRasterW, nRasterH);
}
WASM_EXPORT void  XPS_Delete(BYTE* pData)
{
    RELEASEARRAYOBJECTS(pData);
}

/*
WASM_EXPORT void* Fonts_Create()
{
    if (!CApplicationFontStreams::m_pMemoryStorage)
        CApplicationFontStreams::m_pMemoryStorage = new CGlobalFontsMemoryStorage();
    return CApplicationFontStreams::m_pMemoryStorage;
}
WASM_EXPORT void  Fonts_Destroy()
{
    RELEASEOBJECT(CApplicationFontStreams::m_pMemoryStorage);
}
WASM_EXPORT void  Fonts_Add(void* fonts, const char* id, BYTE* data, LONG size)
{
    CGlobalFontsMemoryStorage* pFonts = (CGlobalFontsMemoryStorage*)fonts;
    pFonts->Add(UTF8_TO_U(std::string(id)), data, size);
}
WASM_EXPORT void  Fonts_Remove(void* fonts, const char* id)
{
    CGlobalFontsMemoryStorage* pFonts = (CGlobalFontsMemoryStorage*)fonts;
    pFonts->Remove(UTF8_TO_U(std::string(id)));
}

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
    return new CGraphicsFileDrawing();
}
WASM_EXPORT void  Graphics_Destroy(void* graphics)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    if (pGraphics) delete pGraphics;
}
WASM_EXPORT bool  Graphics_Load(void* graphics, BYTE* data, LONG size)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    return pGraphics->LoadFromMemory(data, size);
}
WASM_EXPORT int   Graphics_GetPagesCount(void* graphics)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    return pGraphics->GetPagesCount();
}
WASM_EXPORT int   Graphics_GetPageHeight(void* graphics, int nPageIndex)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    return pGraphics->GetPageHeight(nPageIndex);
}
WASM_EXPORT int   Graphics_GetPageWidth (void* graphics, int nPageIndex)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    return pGraphics->GetPageWidth(nPageIndex);
}
WASM_EXPORT BYTE* Graphics_GetPage(void* graphics, int nPageIndex, int nRasterW, int nRasterH)
{
    CGraphicsFileDrawing* pGraphics = (CGraphicsFileDrawing*)graphics;
    return pGraphics->GetPage(nPageIndex, nRasterW, nRasterH);
}
WASM_EXPORT bool  Graphics_TEST(void* graphics)
{
    CGraphicsFileDrawing* ptGraphics = (CGraphicsFileDrawing*)graphics;

    // красная линия
    CGraphicsRenderer* pGraphics = ptGraphics->GetGraphicsForTest();
    pGraphics->SetCoordTransformOffset(-160.294, -109.826);
    pGraphics->SetTransform(1, 0, 0, 1, 0, 0);
    pGraphics->put_IntegerGrid(false);
    pGraphics->SetTransform(1, 0, 0, -1, 42.625, 68.6708);
    pGraphics->put_PenSize(352.75 / 1000.0);
    pGraphics->PathCommandEnd();
    pGraphics->PathCommandMoveTo(0, 0);
    pGraphics->PathCommandLineTo(108.744, 39.4229);
    pGraphics->put_PenColor(255 | (0 << 8) | (0 << 16));
    pGraphics->put_PenAlpha(255);
    pGraphics->DrawPath(1);
    pGraphics->PathCommandEnd();
    pGraphics->put_PenDashStyle(Aggplus::DashStyleSolid);
    pGraphics->put_IntegerGrid(true);
    pGraphics->ResetTransform();

    // круг с градиентной заливкой
    CGraphicsRenderer* pGraphics = ptGraphics->GetGraphicsForTest();
    pGraphics->SetCoordTransformOffset(-161.293, -94.833);
    pGraphics->SetTransform(1, 0, 0, 1, 0, 0);
    pGraphics->put_IntegerGrid(false);
    pGraphics->SetTransform(1, 0, 0, 1, 42.8896, 25.2792);
    pGraphics->put_PenSize(352.75 / 1000.0);
    pGraphics->BrushBounds(-2.13163e-14, -9.33201e-15, 55.0333, 55.0333);
    pGraphics->PathCommandEnd();
    pGraphics->PathCommandMoveTo(0, 27.5167);
    pGraphics->PathCommandCurveTo(-1.84863e-15, 12.4215, 12.4215, 2.9041e-14, 27.5167, -3.55271e-15);
    pGraphics->PathCommandCurveTo(42.6119, -9.33201e-15, 55.0333, 12.4215, 55.0333, 27.5167);
    pGraphics->PathCommandCurveTo(55.0333, 27.5167, 55.0333, 27.5167, 55.0333, 27.5167);
    pGraphics->PathCommandCurveTo(55.0333, 27.5167, 55.0333, 27.5167, 55.0333, 27.5167);
    pGraphics->PathCommandCurveTo(55.0333, 42.6119, 42.6119, 55.0333, 27.5167, 55.0333);
    pGraphics->PathCommandCurveTo(27.5167, 55.0333, 27.5167, 55.0333, 27.5167, 55.0333);
    pGraphics->PathCommandCurveTo(12.4215, 55.0333, -1.94677e-14, 42.6119, -2.13163e-14, 27.5167);
    pGraphics->PathCommandCurveTo(-2.13163e-14, 27.5167, -2.13163e-14, 27.5167, -2.13163e-14, 27.5167);
    pGraphics->PathCommandClose();
    pGraphics->put_PenColor(67 | (115 << 8) | (158 << 16));
    pGraphics->put_PenAlpha(255);

    double dAngle = 0;
    if (fabs(0.000550333 - (-2.13163e-19)) >= FLT_EPSILON || fabs(-9.33201e-20 - (-9.33201e-20)) >= FLT_EPSILON)
        dAngle = atan2(-9.33201e-20 - (-9.33201e-20), 0.000550333 - (-2.13163e-19)) * 180 / 3.14159265358979323846;
    pGraphics->put_BrushType(c_BrushTypePathGradient1);
    LONG   testmas1[2] = {-2778277, -1};
    double testmas2[2] = {0, 1};
    pGraphics->put_BrushGradientColors(testmas1, testmas2, 2);
    pGraphics->put_BrushLinearAngle(dAngle);

    pGraphics->DrawPath(256);
    pGraphics->DrawPath(1);
    pGraphics->PathCommandEnd();
    pGraphics->put_PenDashStyle(Aggplus::DashStyleSolid);
    pGraphics->put_IntegerGrid(true);
    pGraphics->ResetTransform();

    // треугольник с текстурной заливкой
    CGraphicsRenderer* pGraphics = ptGraphics->GetGraphicsForTest();
    pGraphics->SetCoordTransformOffset(-149.304, -101.83);
    pGraphics->SetTransform(1, 0, 0, 1, 0, 0);
    pGraphics->put_IntegerGrid(false);
    pGraphics->SetTransform(1, 0, 0, 1, 39.7146, 27.1312);
    pGraphics->put_PenSize(352.75 / 1000.0);
    pGraphics->BrushBounds(0, 0, 53.1813, 48.9479);
    pGraphics->PathCommandEnd();
    pGraphics->PathCommandMoveTo(0, 48.9479);
    pGraphics->PathCommandLineTo(26.5906, 0);
    pGraphics->PathCommandLineTo(53.1813, 48.9479);
    pGraphics->PathCommandClose();
    pGraphics->put_PenColor(0 | (0 << 8) | (0 << 16));
    pGraphics->put_PenAlpha(255);

    pGraphics->put_BrushType(c_BrushTypeTexture);
    pGraphics->put_BrushTexturePath(L"data:onlyoffice_hatch,AAAA/wAAAP8AAAD/AAAA//////////////////////8AAAD/AAAA/wAAAP8AAAD//////////////////////wAAAP8AAAD/AAAA/wAAAP//////////////////////AAAA/wAAAP8AAAD/AAAA////////////////////////////////////////////AAAA/wAAAP8AAAD/AAAA//////////////////////8AAAD/AAAA/wAAAP8AAAD//////////////////////wAAAP8AAAD/AAAA/wAAAP//////////////////////AAAA/wAAAP8AAAD/AAAA/w==");
    pGraphics->put_BrushTextureMode(1);
    pGraphics->put_BrushTextureAlpha(255);

    pGraphics->DrawPath(256);
    pGraphics->DrawPath(1);
    pGraphics->PathCommandEnd();
    pGraphics->put_PenDashStyle(Aggplus::DashStyleSolid);
    pGraphics->put_IntegerGrid(true);
    pGraphics->ResetTransform();

    // изображение
    CGraphicsRenderer* pGraphics = ptGraphics->GetGraphicsForTest();
    pGraphics->SetCoordTransformOffset(-113.3, -75.565);
    pGraphics->SetTransform(1, 0, 0, 1, 0, 0);
    pGraphics->put_IntegerGrid(false);
    pGraphics->SetTransform(1, 0, 0, 1, 30.0038, 20.0025);
    pGraphics->BrushBounds(0, 0, 70.0358, 70.0358);
    pGraphics->PathCommandEnd();
    pGraphics->PathCommandMoveTo(0, 0);
    pGraphics->PathCommandLineTo(70.0358, 0);
    pGraphics->PathCommandLineTo(70.0358, 70.0358);
    pGraphics->PathCommandLineTo(0, 70.0358);
    pGraphics->PathCommandClose();

    int nBase64BufferLen = NSBase64::Base64EncodeGetRequiredLength(length);
    BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen + 64];
    if (true == NSBase64::Base64Encode(texture, length, pbBase64Buffer, &nBase64BufferLen))
    {
        pGraphics->put_BrushType(c_BrushTypeTexture);
        pGraphics->put_BrushTexturePath(L"data:," + NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pbBase64Buffer, nBase64BufferLen));
        pGraphics->put_BrushTextureMode(0);
        pGraphics->put_BrushTextureAlpha(255);
    }
    RELEASEARRAYOBJECTS(pbBase64Buffer);

    pGraphics->DrawPath(256);
    pGraphics->PathCommandEnd();
    pGraphics->put_PenDashStyle(Aggplus::DashStyleSolid);
    pGraphics->put_IntegerGrid(true);
    pGraphics->ResetTransform();

    // текст
    CGraphicsRenderer* pGraphics = ptGraphics->GetGraphicsForTest();
    pGraphics->SetCoordTransformOffset(-210.583, -111.159);
    pGraphics->SetTransform(1, 0, 0, 1, 0, 0);
    pGraphics->put_IntegerGrid(false);
    pGraphics->SetTransform(1, 0, 0, 1, 58.4824, 30.8707);
    pGraphics->put_BrushType(c_BrushTypeSolid);
    pGraphics->put_BrushColor1(0 | (0 << 8) | (0 << 16));
    pGraphics->put_BrushAlpha1(255);

    double DpiX, DpiY;
    pGraphics->get_DpiX(&DpiX);
    pGraphics->get_DpiY(&DpiY);

    pGraphics->GetFontManager()->LoadFontByName(L"Arial", 11, 0, DpiX, DpiY);
    pGraphics->put_FontName     (L"Arial");
    pGraphics->put_FontFaceIndex(-1);
    pGraphics->put_FontSize     (11);
    pGraphics->put_FontStyle    (0);

    pGraphics->CommandDrawTextCHAR(1090, 0, 3.63992, 0, 0);
    pGraphics->CommandDrawTextCHAR(1077, 1.07813, 3.63992, 0, 0);
    pGraphics->CommandDrawTextCHAR(1089, 3.23627, 3.63992, 0, 0);
    pGraphics->CommandDrawTextCHAR(1090, 5.17651, 3.63992, 0, 0);

    pGraphics->put_IntegerGrid(true);
    pGraphics->ResetTransform();

    return true;
}
*/
/*
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
    NSFonts::IApplicationFonts* m_pApplicationFonts = NSFonts::NSApplication::Create();
    std::vector<std::wstring> test;
    m_pApplicationFonts->InitializeFromArrayFiles(test, 0);
    NSFonts::IFontManager* pManager = m_pApplicationFonts->GenerateFontManager();

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
*/
#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
static DWORD GetLength(BYTE* x)
{
    return x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24;
}

int main()
{
    //void* test = Graphics_Create(412, 151, 109.008, 39.9521);
    //void* test = Graphics_Create(210, 210, 55.5625, 55.5625);
    //void* test = Graphics_Create(203, 187, 53.7104, 49.4771);
    //void* test = Graphics_Create(265, 265, 70.1146, 70.1146);
    //void* test = Graphics_Create(211, 119, 55.8251, 31.2208);
    //void* test = Graphics_Create();
    //void* fonts = Fonts_Create();

    /*
    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    //if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.png", &pData, nBytesCount))
    if (!oFile.ReadAllBytes(L"C:/Windows/Fonts/arial.ttf", &pData, nBytesCount))
    {
        Fonts_Destroy();
        Graphics_Destroy(test);
        RELEASEARRAYOBJECTS(pData);
        return 1;
    }
    oFile.CloseFile();
    //Fonts_Add(fonts, "Arial", pData, nBytesCount);
    */

    BYTE* pXpsData = NULL;
    DWORD nXpsBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.xps", &pXpsData, nXpsBytesCount))
    {
        RELEASEARRAYOBJECTS(pXpsData);
        return 1;
    }
    oFile.CloseFile();

    CGraphicsFileDrawing* test = XPS_Load(pXpsData, nXpsBytesCount);
    XPS_Delete(pXpsData);
    int* info = XPS_GetInfo(test);
    int pages_count = *info;
    BYTE* res = NULL;
    if (pages_count > 0)
        res = XPS_GetPixmap(test, 0, info[1], info[2]);

    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(info[1]);
    resFrame->put_Height(info[2]);
    resFrame->put_Stride(-4 * info[1]);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    DWORD nLength = GetLength(res + info[1] * info[2] * 4);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(res + info[1] * info[2] * 4 + i);
        i += 4;
        std::cout << std::string((char*)(res + info[1] * info[2] * 4 + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(res + info[1] * info[2] * 4 + i);
        i += 4;
        std::cout << std::string((char*)(res + info[1] * info[2] * 4 + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(res + info[1] * info[2] * 4 + i);
        i += 4;
        std::cout << std::string((char*)(res + info[1] * info[2] * 4 + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(res + info[1] * info[2] * 4 + i);
        i += 4;
        std::cout << std::string((char*)(res + info[1] * info[2] * 4 + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(res + info[1] * info[2] * 4 + i);
        i += 4;
        std::cout << nPathLength << std::endl;
    }

    XPS_Close(test);
    RELEASEARRAYOBJECTS(res);
    RELEASEOBJECT(resFrame);
    return 0;
}
#endif
