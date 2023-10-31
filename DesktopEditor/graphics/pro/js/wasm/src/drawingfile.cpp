#include <malloc.h>

#include "../../../../../common/Base64.h"

#include "drawingfile.h"
#include "serialize.h"

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
WASM_EXPORT BYTE* GetInfo   (CGraphicsFileDrawing* pGraphics)
{
	NSWasm::CData oRes;
	oRes.SkipLen();

	oRes.AddInt(pGraphics->GetMaxRefID());

	int pages_count = pGraphics->GetPagesCount();
	oRes.AddInt(pages_count);
	for (int page = 0; page < pages_count; ++page)
	{
		int nW = 0;
		int nH = 0;
		int nDpi = 0;
		int nRotate = 0;
		pGraphics->GetPageInfo(page, nW, nH, nDpi, nRotate);
		oRes.AddInt(nW);
		oRes.AddInt(nH);
		oRes.AddInt(nDpi);
		oRes.AddInt(nRotate);
	}
	std::wstring wsInfo = pGraphics->GetInfo();
	std::string sInfo = U_TO_UTF8(wsInfo);
	oRes.WriteString((BYTE*)sInfo.c_str(), sInfo.length());

	oRes.WriteLen();
	BYTE* bRes = oRes.GetBuffer();
	oRes.ClearWithoutAttack();
	return bRes;
}
WASM_EXPORT BYTE* GetPixmap (CGraphicsFileDrawing* pGraphics, int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
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
WASM_EXPORT BYTE* GetInteractiveFormsInfo(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->GetInteractiveFormsInfo();
}
WASM_EXPORT BYTE* GetInteractiveFormsAP(CGraphicsFileDrawing* pGraphics, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget, int nView, int nButtonView)
{
	const char* sView = NULL;
	if (nView == 0)
		sView = "N";
	else if (nView == 1)
		sView = "D";
	else if (nView == 2)
		sView = "R";

	const char* sButtonView = NULL;
	if (nButtonView == 0)
		sButtonView = "Off";
	else if (nButtonView == 1)
		sButtonView = "Yes";

	return pGraphics->GetAPWidget(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, sView, sButtonView);
}
WASM_EXPORT BYTE* GetButtonIcons(CGraphicsFileDrawing* pGraphics, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int bBase64, int nButtonWidget, int nIconView)
{
	const char* sIconView = NULL;
	if (nIconView == 0)
		sIconView = "I";
	else if (nIconView == 1)
		sIconView = "RI";
	else if (nIconView == 2)
		sIconView = "IX";

	return pGraphics->GetButtonIcon(nRasterW, nRasterH, nBackgroundColor, nPageIndex, bBase64 ? true : false, nButtonWidget, sIconView);
}
WASM_EXPORT BYTE* GetAnnotationsInfo(CGraphicsFileDrawing* pGraphics, int nPageIndex)
{
	return pGraphics->GetAnnots(nPageIndex);
}
WASM_EXPORT BYTE* GetAnnotationsAP(CGraphicsFileDrawing* pGraphics, int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot, int nView)
{
	const char* sView = NULL;
	if (nView == 0)
		sView = "N";
	else if (nView == 1)
		sView = "D";
	else if (nView == 2)
		sView = "R";

	return pGraphics->GetAPAnnots(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, sView);
}
WASM_EXPORT void DestroyTextInfo(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->DestroyText();
}
WASM_EXPORT int  IsNeedCMap(CGraphicsFileDrawing* pGraphics)
{
	return pGraphics->IsNeedCMap() ? 1 : 0;
}
WASM_EXPORT void SetCMapData(CGraphicsFileDrawing* pGraphics, BYTE* data, int size)
{
	pGraphics->SetCMapData(data, size);
}

#ifdef __cplusplus
}
#endif
