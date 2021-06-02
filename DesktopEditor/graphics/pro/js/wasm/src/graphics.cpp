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
WASM_EXPORT void Graphics_Destroy(void* graphics)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    delete pGraphics;
}
WASM_EXPORT void Graphics_CreateFromBgraFrame(void* graphics, CBgraFrame* pFrame)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->CreateFromBgraFrame(pFrame);
}
WASM_EXPORT void* Graphics_CreateFromEmptyFrame(double width, double height, CBgraFrame* pFrame)
{
    int nRasterW = (int)width;
    int nRasterH = (int)height;
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
    Graphics_CreateFromBgraFrame(pGraphics, pFrame);
    return pGraphics;
}
WASM_EXPORT void Graphics_SetFontManager(void* graphics, NSFonts::IFontManager* pManager = NULL)
{
    CGraphicsRenderer* pGraphics = (CGraphicsRenderer*)graphics;
    pGraphics->SetFontManager(pManager);
}
// smart methods
WASM_EXPORT void Graphics_drawHorLine(void* graphics, BYTE align, double y, double x, double r, double penW)
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
    void* testGraphics = Graphics_CreateFromEmptyFrame(50, 50, testFrame);

    Graphics_SetFontManager(testGraphics);

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
    Graphics_drawHorLine(testGraphics, 1, 50, 50, 50, 5);

    testFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.jpg", 3);
    Raster_Destroy(testFrame);
    Graphics_Destroy(testGraphics);
    return 0;
}
#endif

#endif // _WASM_GRAPHICS_
