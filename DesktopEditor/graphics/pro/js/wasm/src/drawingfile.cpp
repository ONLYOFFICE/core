#include <malloc.h>

#include "../../../../../common/Base64.h"
#include "../../../../../doctrenderer/drawingfile.h"

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
WASM_EXPORT void InitializeFontsRanges(BYTE* pDataSrc)
{
	if (g_applicationFonts && pDataSrc)
		g_applicationFonts->InitializeRanges(pDataSrc);
}
WASM_EXPORT void SetFontBinary(char* path, BYTE* data, int size)
{
	NSFonts::IFontsMemoryStorage* pStorage = CDrawingFile::GetFontsStorage();
	if (pStorage)
	{
		std::string sPathA(path);
		pStorage->Add(UTF8_TO_U(sPathA), data, size, true);
	}
}
WASM_EXPORT int IsFontBinaryExist(char* path)
{
	NSFonts::IFontsMemoryStorage* pStorage = CDrawingFile::GetFontsStorage();
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
	LONG nHeaderSearchSize = 1024;
	LONG nSize = size < nHeaderSearchSize ? size : nHeaderSearchSize;
	char* pData = (char*)data;
	for (int i = 0; i < nSize - 5; ++i)
	{
		int nPDF = strncmp(&pData[i], "%PDF-", 5);
		if (!nPDF)
			return 0;
	}
	if ( (8 <= size) && (0x41 == data[0] && 0x54 == data[1] && 0x26 == data[2] && 0x54 == data[3] &&
						 0x46 == data[4] && 0x4f == data[5] && 0x52 == data[6] && 0x4d == data[7]))
		return 1;
	return 2;
}
WASM_EXPORT CDrawingFile* Open(BYTE* data, LONG size, const char* password)
{
	if (!g_applicationFonts)
		g_applicationFonts = NSFonts::NSApplication::Create();

	// всегда пересоздаем сторадж
	CDrawingFile::InitFontsGlobalStorage();

	CDrawingFile* pFile = new CDrawingFile(g_applicationFonts);
	std::wstring sPassword = L"";
	if (NULL != password)
		sPassword = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)password, strlen(password));
	pFile->OpenFile(data, size, sPassword);
	return pFile;
}
WASM_EXPORT int GetErrorCode(CDrawingFile* pFile)
{
	if (!pFile)
		return -1;
	return pFile->GetErrorCode();
}
WASM_EXPORT void Close(CDrawingFile* pFile)
{
	delete pFile;
	NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NULL);
}
WASM_EXPORT BYTE* GetInfo(CDrawingFile* pFile)
{
	return pFile->GetInfo();
}
WASM_EXPORT BYTE* GetPixmap(CDrawingFile* pFile, int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
{
	return pFile->GetPixmap(nPageIndex, nRasterW, nRasterH, nBackgroundColor);
}
WASM_EXPORT BYTE* GetGlyphs(CDrawingFile* pFile, int nPageIndex)
{
	return pFile->GetGlyphs(nPageIndex);
}
WASM_EXPORT BYTE* GetLinks  (CDrawingFile* pFile, int nPageIndex)
{
	return pFile->GetLinks(nPageIndex);
}
WASM_EXPORT BYTE* GetStructure(CDrawingFile* pFile)
{
	return pFile->GetStructure();
}
WASM_EXPORT BYTE* GetInteractiveFormsInfo(CDrawingFile* pFile)
{
	return pFile->GetInteractiveFormsInfo();
}
WASM_EXPORT BYTE* GetInteractiveFormsFonts(CDrawingFile* pFile, int nType)
{
	return pFile->GetInteractiveFormsFonts(nType);
}
WASM_EXPORT BYTE* GetInteractiveFormsAP(CDrawingFile* pFile, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget, int nView, int nButtonView)
{
	return pFile->GetInteractiveFormsAP(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, nView, nButtonView);
}
WASM_EXPORT BYTE* GetButtonIcons(CDrawingFile* pFile, int nBackgroundColor, int nPageIndex, int bBase64, int nButtonWidget, int nIconView)
{
	return pFile->GetButtonIcons(nBackgroundColor, nPageIndex, bBase64 ? true : false, nButtonWidget, nIconView);
}
WASM_EXPORT BYTE* GetAnnotationsInfo(CDrawingFile* pFile, int nPageIndex)
{
	return pFile->GetAnnotationsInfo(nPageIndex);
}
WASM_EXPORT BYTE* GetAnnotationsAP(CDrawingFile* pFile, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot, int nView)
{
	return pFile->GetAnnotationsAP(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, nView);
}
WASM_EXPORT BYTE* GetFontBinary(CDrawingFile* pFile, char* path)
{
	return pFile->GetFontBinary(std::string(path));
}
WASM_EXPORT void DestroyTextInfo(CDrawingFile* pFile)
{
	return pFile->DestroyTextInfo();
}
WASM_EXPORT int  IsNeedCMap(CDrawingFile* pFile)
{
	return pFile->IsNeedCMap() ? 1 : 0;
}
WASM_EXPORT void SetCMapData(CDrawingFile* pFile, BYTE* data, int size)
{
	pFile->SetCMapData(data, size);
}
WASM_EXPORT BYTE* ScanPage(CDrawingFile* pFile, int nPageIndex, int mode)
{
	return pFile->ScanPage(nPageIndex, mode);
}

WASM_EXPORT void* GetImageBase64(CDrawingFile* pFile, int rId)
{
	return pFile->GetImageBase64(rId);
}
WASM_EXPORT int GetImageBase64Len(std::string* p)
{
	return (int)p->length();
}
WASM_EXPORT char* GetImageBase64Ptr(std::string* p)
{
	return (char*)p->c_str();
}
WASM_EXPORT void GetImageBase64Free(std::string* p)
{
	*p = "";
}

#ifdef __cplusplus
}
#endif
