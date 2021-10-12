#include <malloc.h>
#include <iostream>

#include "../../../../pro/Graphics.h"
#include "../../../../../common/Base64.h"
#include "../../../../../common/File.h"
#include "drawingfile.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

NSFonts::IApplicationFonts* g_applicationFonts = NULL;

WASM_EXPORT void InitializeFontsBin(BYTE* data, int size)
{
    if (!g_applicationFonts)
	{
		g_applicationFonts = NSFonts::NSApplication::Create();
		g_applicationFonts->InitializeFromBin(data, (unsigned int)size);
	}
}

WASM_EXPORT void InitializeFontsBase64(BYTE* pDataSrc, int nLenSrc)
{
    if (!g_applicationFonts)
	{
		g_applicationFonts = NSFonts::NSApplication::Create();
		
		int nLenDst = NSBase64::Base64DecodeGetRequiredLength(nLenSrc);
        BYTE* pDataDst = new BYTE[nLenDst];

        if (FALSE == NSBase64::Base64Decode((const char*)pDataSrc, nLenSrc, pDataDst, &nLenDst))
        {
            RELEASEARRAYOBJECTS(pDataDst);
            return;
        }
		
		g_applicationFonts->InitializeFromBin(pDataDst, (unsigned int)nLenDst);
		RELEASEARRAYOBJECTS(pDataDst);
	}
}

WASM_EXPORT void SetFontBinary(char* path, BYTE* data, int size)
{
	NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	if (pStorage)
	{
		std::string sPathA(path);
		pStorage->Add(UTF8_TO_U(sPathA), data, size, true);
	}
}

WASM_EXPORT int IsFontBinaryExist(char* path)
{
	NSFonts::IFontsMemoryStorage* pStorage = NSFonts::NSApplicationFontStream::GetGlobalMemoryStorage();
	if (pStorage)
	{
		std::string sPathA(path);
		NSFonts::IFontStream* pStream = pStorage->Get(UTF8_TO_U(sPathA));
		if (pStream)
			return 1;
	}
	return 0;
}

WASM_EXPORT int GetType(BYTE* data, LONG size)
{
    // 0 - PDF
    // 1 - DJVU
    // 2 - XPS
    char* pFirst = strstr((char*)data, "%PDF-" );
    if (pFirst)
        return 0;
    if ( (8 <= size) && (0x41 == data[0] && 0x54 == data[1] && 0x26 == data[2] && 0x54 == data[3] &&
                         0x46 == data[4] && 0x4f == data[5] && 0x52 == data[6] && 0x4d == data[7]))
        return 1;
    return 2;
}
WASM_EXPORT CGraphicsFileDrawing* Open(BYTE* data, LONG size)
{
	if (!g_applicationFonts)
		g_applicationFonts = NSFonts::NSApplication::Create();
		
	// всегда пересоздаем сторадж
	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());
	
    CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing(g_applicationFonts);
    if (pGraphics->Open(data, size, GetType(data, size)))
        return pGraphics;
    
    delete pGraphics;
    return NULL;
}
WASM_EXPORT void  Close     (CGraphicsFileDrawing* pGraphics)
{
    delete pGraphics;
    NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NULL);
}
WASM_EXPORT int*  GetInfo   (CGraphicsFileDrawing* pGraphics)
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
WASM_EXPORT BYTE* GetPixmap (CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH)
{
    return pGraphics->GetPage(nPageIndex, nRasterW, nRasterH);
}
WASM_EXPORT BYTE* GetGlyphs (CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH)
{
    return pGraphics->GetGlyphs(nPageIndex, nRasterW, nRasterH);
}
WASM_EXPORT BYTE* GetLinks  (CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH)
{
    return pGraphics->GetLinks(nPageIndex, nRasterW, nRasterH);
}
WASM_EXPORT BYTE* GetStructure(CGraphicsFileDrawing* pGraphics)
{
    return pGraphics->GetStructure();
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
static DWORD GetLength(BYTE* x)
{
    return x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
}

int main()
{
#define XPS_TEST  0
#define DJVU_TEST 0
#define PDF_TEST  1
#if PDF_TEST
    BYTE* pPdfData = NULL;
    DWORD nPdfBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.pdf", &pPdfData, nPdfBytesCount))
    {
        RELEASEARRAYOBJECTS(pPdfData);
        return 1;
    }
    oFile.CloseFile();

    CGraphicsFileDrawing* test = Open(pPdfData, nPdfBytesCount);
    int* info = GetInfo(test);
    int pages_count = *info;
    int width  = info[1] * 96 / info[3];
    int height = info[2] * 96 / info[3];

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, 2, width, height);

    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";
    std::cout << std::endl;

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(-4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    BYTE* pLinks = GetLinks(test, 2, width, height);
    DWORD nLength = GetLength(pLinks);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout <<  "Link "<< std::string((char*)(pLinks + i), nPathLength);
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " X " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " Y " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " W " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " H " << (double)nPathLength / 100.0 << std::endl;
    }

    BYTE* pStructure = GetStructure(test);
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
        std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Description " << std::string((char*)(pStructure + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    BYTE* pGlyphs = GetGlyphs(test, 1, width, height);
    nLength = GetLength(pGlyphs);
    i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::string oWord = std::string((char*)(pGlyphs + i), nPathLength);
        std::wcout << L"Word " << UTF8_TO_U(oWord) << L" ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "X " << (double)nPathLength / 100.0 << " ";
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "Y " << (double)nPathLength / 100.0 << " ";
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "W " << (double)nPathLength / 100.0 << " ";
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "H " << (double)nPathLength / 100.0 << std::endl;
    }

    Close(test);
    RELEASEARRAYOBJECTS(pPdfData);
    RELEASEARRAYOBJECTS(pLinks);
    RELEASEARRAYOBJECTS(pGlyphs);
    RELEASEARRAYOBJECTS(pStructure);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
    RELEASEOBJECT(resFrame);
    return 0;
#endif
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

    CGraphicsFileDrawing* test = Open(pXpsData, nXpsBytesCount);
    RELEASEARRAYOBJECTS(pXpsData);
    int* info = GetInfo(test);
    int pages_count = *info;
    int width  = info[1] * 96 / info[3];
    int height = info[2] * 96 / info[3];

    BYTE* pGlyphs = GetGlyphs(test, 22, width, height);
    DWORD nLength = GetLength(pGlyphs);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "Font " << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "Size " << (double)nPathLength / 100.0 << " ";
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "Amount " << nPathLength << std::endl;
        DWORD nAmount = nPathLength;
        for (DWORD j = 0; j < nAmount; j++)
        {
            nPathLength = GetLength(pGlyphs + i);
            i += 4;
            std::cout << "X " << (double)nPathLength / 100.0 << " ";
            nPathLength = GetLength(pGlyphs + i);
            i += 4;
            std::cout << "Y " << (double)nPathLength / 100.0 << " ";
            nPathLength = GetLength(pGlyphs + i);
            i += 4;
            std::cout << "Symbol " << nPathLength << std::endl;
        }
    }

    BYTE* pLinks = GetLinks(test, 22, width, height);
    nLength = GetLength(pLinks);
    i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout <<  "Link "<< std::string((char*)(pLinks + i), nPathLength);
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " X " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " Y " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " W " << (double)nPathLength / 100.0;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << " H " << (double)nPathLength / 100.0 << std::endl;
    }

    BYTE* pStructure = GetStructure(test);
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
        std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Description " << std::string((char*)(pStructure + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, 22, width, height);

    for (int i = 0; i < 100; i++)
        std::cout << (int)res[i] << " ";
    std::cout << std::endl;

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(-4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    Close(test);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
    RELEASEARRAYOBJECTS(pGlyphs);
    RELEASEARRAYOBJECTS(pLinks);
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

    CGraphicsFileDrawing* test = Open(pDjVuData, nDjVuBytesCount);
    RELEASEARRAYOBJECTS(pDjVuData);
    int* info = GetInfo(test);
    int pages_count = *info;
    int width  = info[1] * 96 / info[3];
    int height = info[2] * 96 / info[3];

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, 0, width, height);

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

    BYTE* pGlyphs = GetGlyphs(test, 0, width, height);
    DWORD nLength = GetLength(pGlyphs);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::string oWord = std::string((char*)(pGlyphs + i), nPathLength);
        std::wcout << L"Word " << UTF8_TO_U(oWord) << L" ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "X " << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "Y " << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "W " << std::string((char*)(pGlyphs + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pGlyphs + i);
        i += 4;
        std::cout << "H " << std::string((char*)(pGlyphs + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    BYTE* pLinks = GetLinks(test, 0, width, height);
    nLength = GetLength(pLinks);
    i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout <<  "Link "<< std::string((char*)(pLinks + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << "X " << std::string((char*)(pLinks + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << "Y " << std::string((char*)(pLinks + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << "W " << std::string((char*)(pLinks + i), nPathLength) << " ";
        i += nPathLength;
        nPathLength = GetLength(pLinks + i);
        i += 4;
        std::cout << "H " << std::string((char*)(pLinks + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    BYTE* pStructure = GetStructure(test);
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
        std::string oDs = std::string((char*)(pStructure + i), nPathLength);
        std::wcout << L"Description "<< UTF8_TO_U(oDs) << std::endl;
        i += nPathLength;
    }

    Close(test);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
    RELEASEARRAYOBJECTS(pGlyphs);
    RELEASEARRAYOBJECTS(pLinks);
    RELEASEARRAYOBJECTS(pStructure);
    RELEASEOBJECT(resFrame);
    return 0;
#endif
}
#endif
