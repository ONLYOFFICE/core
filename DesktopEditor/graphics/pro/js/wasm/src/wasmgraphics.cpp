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

WASM_EXPORT CGraphicsFileDrawing* XPS_Load (BYTE* data, LONG size)
{
    CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing();
    pGraphics->CreateXPS();
    if (!CApplicationFontStreams::m_pMemoryStorage)
        CApplicationFontStreams::m_pMemoryStorage = new CGlobalFontsMemoryStorage();
    pGraphics->LoadFromMemory(data, size);
    return pGraphics;
}
WASM_EXPORT CGraphicsFileDrawing* DJVU_Load(BYTE* data, LONG size)
{
    CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing();
    pGraphics->CreateDjVu();
    if (!CApplicationFontStreams::m_pMemoryStorage)
        CApplicationFontStreams::m_pMemoryStorage = new CGlobalFontsMemoryStorage();
    pGraphics->LoadFromMemory(data, size);
    return pGraphics;
}
WASM_EXPORT void  XPS_Close    (CGraphicsFileDrawing* pGraphics)
{
    delete pGraphics;
    RELEASEOBJECT(CApplicationFontStreams::m_pMemoryStorage);
}
WASM_EXPORT int*  XPS_GetInfo  (CGraphicsFileDrawing* pGraphics)
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
WASM_EXPORT BYTE* XPS_GetGlyphs(CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
    return pGraphics->GetXPSGlyphs(nPageIndex);
}
WASM_EXPORT BYTE* XPS_GetStructure(CGraphicsFileDrawing* pGraphics)
{
    return pGraphics->GetXPSStructure();
}
WASM_EXPORT BYTE* DJVU_GetStructure(CGraphicsFileDrawing* pGraphics)
{
    return pGraphics->GetDJVUStructure();
}
WASM_EXPORT void  XPS_Delete(BYTE* pData)
{
    RELEASEARRAYOBJECTS(pData);
}

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
#define XPS_TEST  0
#define DJVU_TEST 1
#if XPS_TEST
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
    int width  = info[1] * 96 / info[3];
    int height = info[2] * 96 / info[3];
    BYTE* res = NULL;
    if (pages_count > 0)
        res = XPS_GetPixmap(test, 0, width, height);
    RELEASEARRAYOBJECTS(res);
    if (pages_count > 0)
        res = XPS_GetPixmap(test, 0, width, height);

    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(-4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    BYTE* pGlyphs = XPS_GetGlyphs(test, 0);
    DWORD nLength = GetLength(pGlyphs);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << nPathLength << std::endl;
    }

    BYTE* pStructure = XPS_GetStructure(test);
    nLength = GetLength(pStructure);
    i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Page " << nPathLength << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Level " << nPathLength << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Y "<< std::string((char*)(pStructure + i), nPathLength) << ", ";
        i += nPathLength;
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Description "<< std::string((char*)(pStructure + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    XPS_Close(test);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
    RELEASEARRAYOBJECTS(pGlyphs);
    RELEASEARRAYOBJECTS(pStructure);
    RELEASEOBJECT(resFrame);
    return 0;
#endif
#if DJVU_TEST
    BYTE* pDjVuData = NULL;
    DWORD nDjVuBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.djvu", &pDjVuData, nDjVuBytesCount))
    {
        RELEASEARRAYOBJECTS(pDjVuData);
        return 1;
    }
    oFile.CloseFile();

    CGraphicsFileDrawing* test = DJVU_Load(pDjVuData, nDjVuBytesCount);
    XPS_Delete(pDjVuData);
    int* info = XPS_GetInfo(test);
    int pages_count = *info;
    int width  = info[1] * 96 / info[3];
    int height = info[2] * 96 / info[3];
    BYTE* res = NULL;
    if (pages_count > 0)
        res = XPS_GetPixmap(test, 0, width, height);

    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(-4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    BYTE* pStructure = DJVU_GetStructure(test);
    DWORD nLength = GetLength(pStructure);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Page " << nPathLength << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Level " << nPathLength << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::string oDs = std::string((char*)(pStructure + i), nPathLength);
        std::wcout << L"Description "<< UTF8_TO_U(oDs) << std::endl;
        i += nPathLength;
    }

    XPS_Close(test);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
    RELEASEARRAYOBJECTS(pStructure);
    RELEASEOBJECT(resFrame);
    return 0;
#endif
}
#endif
