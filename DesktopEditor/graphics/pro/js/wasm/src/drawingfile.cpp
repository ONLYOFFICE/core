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
WASM_EXPORT CGraphicsFileDrawing* Open(BYTE* data, LONG size, const char* password)
{
	if (!g_applicationFonts)
		g_applicationFonts = NSFonts::NSApplication::Create();
		
	// всегда пересоздаем сторадж
	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());
	
    CGraphicsFileDrawing* pGraphics = new CGraphicsFileDrawing(g_applicationFonts);
    pGraphics->Open(data, size, GetType(data, size), password);
    return pGraphics;
}
WASM_EXPORT int GetErrorCode(CGraphicsFileDrawing* pGraphics)
{
    if (!pGraphics)
        return -1;
    return pGraphics->GetErrorCode();
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
WASM_EXPORT BYTE* GetPixmap(CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
{
    return pGraphics->GetPage(nPageIndex, nRasterW, nRasterH, nBackgroundColor);
}
WASM_EXPORT BYTE* GetGlyphs (CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
    return pGraphics->GetGlyphs(nPageIndex);
}
WASM_EXPORT BYTE* GetLinks  (CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
    return pGraphics->GetLinks(nPageIndex);
}
WASM_EXPORT BYTE* GetStructure(CGraphicsFileDrawing* pGraphics)
{
    return pGraphics->GetStructure();
}
WASM_EXPORT void DestroyTextInfo(CGraphicsFileDrawing* pGraphics)
{
    return pGraphics->DestroyText();
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
#define XPS_TEST  1
#define DJVU_TEST 0
#define PDF_TEST  0
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

    CGraphicsFileDrawing* test = Open(pPdfData, nPdfBytesCount, "");
    int nError = GetErrorCode(test);
    if (nError != 0)
    {
        Close(test);
        if (nError == 4)
        {
            std::string sPassword = "Test123";
            test = Open(pPdfData, nPdfBytesCount, sPassword.c_str());
        }
        else
        {
            RELEASEARRAYOBJECTS(pPdfData);
            return 1;
        }
    }
    int* info = GetInfo(test);
    int pages_count = *info;
    int test_page = 0;
    int width  = info[test_page * 3 + 1];
    int height = info[test_page * 3 + 2];
    std::cout << "Page " << test_page << " width " << width << " height " << height << " dpi " << info[test_page * 3 + 3] << std::endl;

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, test_page, width, height, 0xFFFFFF);

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    std::cout << std::endl;
    BYTE* pLinks = GetLinks(test, test_page);
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
        std::cout << " Ydest " << (double)nPathLength / 100.0;
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

    std::cout << std::endl;
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

    std::cout << std::endl;
    BYTE* pGlyphs = GetGlyphs(test, test_page);

    Close(test);
    RELEASEARRAYOBJECTS(pPdfData);
    RELEASEARRAYOBJECTS(pLinks);
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

    CGraphicsFileDrawing* test = Open(pXpsData, nXpsBytesCount, "");
    int nError = GetErrorCode(test);
    if (nError != 0)
    {
        Close(test);
        RELEASEARRAYOBJECTS(pXpsData);
        return 1;
    }
    int* info = GetInfo(test);
    int pages_count = *info;
    int test_page = 0;
    int width  = info[test_page * 3 + 1];
    int height = info[test_page * 3 + 2];
    std::cout << "Page " << test_page << " width " << width << " height " << height << std::endl;

    std::cout << std::endl;
    BYTE* pStructure = GetStructure(test);
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
        std::cout << "Y " << (double)nPathLength / 100.0 << ", ";
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Description " << std::string((char*)(pStructure + i), nPathLength) << std::endl;
        i += nPathLength;
    }

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, test_page, width, height, 0xFFFFFF);
    if (!res)
    {
        RELEASEARRAYOBJECTS(pXpsData);
        RELEASEARRAYOBJECTS(info);
        return 1;
    }

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    std::cout << std::endl;
    BYTE* pGlyphs = GetGlyphs(test, test_page);

    std::cout << std::endl;
    BYTE* pLinks = GetLinks(test, test_page);
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
        std::cout << " Ydest " << (double)nPathLength / 100.0;
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

    Close(test);
    RELEASEARRAYOBJECTS(pXpsData);
    RELEASEARRAYOBJECTS(info);
    RELEASEARRAYOBJECTS(res);
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

    CGraphicsFileDrawing* test = Open(pDjVuData, nDjVuBytesCount, "");
    int nError = GetErrorCode(test);
    if (nError != 0)
    {
        Close(test);
        RELEASEARRAYOBJECTS(pDjVuData);
        return 1;
    }
    int* info = GetInfo(test);
    int pages_count = *info;
    int test_page = 1;
    int width  = info[test_page * 3 + 1];
    int height = info[test_page * 3 + 2];
    std::cout << "Page " << test_page << " width " << width << " height " << height << std::endl;

    std::cout << std::endl;
    BYTE* pStructure = GetStructure(test);
    DWORD nLength = GetLength(pStructure);
    DWORD i = 4;
    nLength -= 4;
    while (i < nLength)
    {
        DWORD nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << "Page " << nPathLength;
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::cout << " Level " << nPathLength;
        i += 4; // y 0.0
        nPathLength = GetLength(pStructure + i);
        i += 4;
        std::string oDs = std::string((char*)(pStructure + i), nPathLength);
        std::wcout << L" Description "<< UTF8_TO_U(oDs) << std::endl;
        i += nPathLength;
    }

    BYTE* res = NULL;
    if (pages_count > 0)
        res = GetPixmap(test, test_page, width, height);

    CBgraFrame* resFrame = new CBgraFrame();
    resFrame->put_Data(res);
    resFrame->put_Width(width);
    resFrame->put_Height(height);
    resFrame->put_Stride(4 * width);
    resFrame->put_IsRGBA(true);
    resFrame->SaveFile(NSFile::GetProcessDirectory() + L"/res.png", _CXIMAGE_FORMAT_PNG);
    resFrame->ClearNoAttack();

    std::cout << std::endl;
    BYTE* pGlyphs = GetGlyphs(test, test_page);

    std::cout << std::endl;
    BYTE* pLinks = GetLinks(test, test_page);
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
        std::cout << " Ydest " << (double)nPathLength / 100.0;
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

    Close(test);
    RELEASEARRAYOBJECTS(pDjVuData);
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
