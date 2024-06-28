#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../../common/File.h"
#include "../../../../pro/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"
#include "../../../../../../DjVuFile/DjVu.h"
#include "../../../../../../PdfFile/PdfFile.h"
#include "../../../../../../HtmlRenderer/include/HTMLRendererText.h"
#include "../../../../../../DocxRenderer/DocxRenderer.h"
#include "serialize.h"

class CGraphicsFileDrawing
{
private:
	IOfficeDrawingFile* pReader;
	NSFonts::IApplicationFonts* pApplicationFonts;
	NSFonts::IFontManager* pFontManager;
	NSHtmlRenderer::CHTMLRendererText* pTextRenderer;
	NSDocxRenderer::IImageStorage* pImageStorage;
	int nType;
public:
	CGraphicsFileDrawing(NSFonts::IApplicationFonts* pFonts)
	{
		pReader = NULL;
		pTextRenderer = NULL;
		pImageStorage = NULL;
		pApplicationFonts = pFonts;
		pApplicationFonts->AddRef();

		pFontManager = pApplicationFonts->GenerateFontManager();
		NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(pApplicationFonts->GetStreams());
		pFontCache->SetCacheSize(8);
		pFontManager->SetOwnerCache(pFontCache);
		
		nType = -1;
	}
	~CGraphicsFileDrawing()
	{
		RELEASEOBJECT(pReader);
		RELEASEOBJECT(pTextRenderer);
		RELEASEOBJECT(pFontManager);
		RELEASEINTERFACE(pApplicationFonts);
		RELEASEOBJECT(pImageStorage);
		nType = -1;
	}
	bool Open(BYTE* data, DWORD length, int _nType, const char* password)
	{
		nType = _nType;
		if (nType == 0)
			pReader = new CPdfFile(pApplicationFonts);
		else if (nType == 1)
			pReader = new CDjVuFile(pApplicationFonts);
		else if (nType == 2)
			pReader = new CXpsFile(pApplicationFonts);
		if (!pReader)
			return false;
		std::wstring sPassword = L"";
		if (password)
		{
			std::string sPass(password);
			sPassword = UTF8_TO_U(sPass);
		}
		return pReader->LoadFromMemory(data, length, L"", sPassword, sPassword);
	}
	int GetErrorCode()
	{
		if (!pReader)
			return -1;
		if (nType == 0)
			// диапозон ошибки от 0 до 10
			return ((CPdfFile*)pReader)->GetError();
		return 0; // errNone
	}
	int GetPagesCount()
	{
		return pReader->GetPagesCount();
	}
	int GetMaxRefID()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetMaxRefID();
		return 0;
	}
	void GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX, int& nRotate)
	{
		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		pReader->GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		if (nType == 2)
		{
			dWidth    = dWidth    / 25.4 * 96.0;
			dHeight   = dHeight   / 25.4 * 96.0;
			dPageDpiX = dPageDpiX / 25.4 * 96.0;
		}
		if (nType == 0)
			nRotate = ((CPdfFile*)pReader)->GetRotate(nPageIndex);
		nWidth    = dWidth;
		nHeight   = dHeight;
		nPageDpiX = dPageDpiX;
	}
	std::wstring GetFont(const std::wstring& sFontName)
	{
		std::wstring sFontFile;
		if (nType == 0)
			sFontFile = ((CPdfFile*)pReader)->GetFontPath(sFontName);
		return sFontFile;
	}
	BYTE* GetPage(int nPageIndex, int nRasterW, int nRasterH, int nBackgroundColor)
	{
		return pReader->ConvertToPixels(nPageIndex, nRasterW, nRasterH, true, pFontManager, nBackgroundColor, (nBackgroundColor == 0xFFFFFF) ? false : true);
	}
	BYTE* GetGlyphs(int nPageIndex)
	{
		if (NULL == pTextRenderer)
			pTextRenderer = new NSHtmlRenderer::CHTMLRendererText();

		pTextRenderer->Init(pReader, 8);
		pReader->DrawPageOnRenderer(pTextRenderer, nPageIndex, NULL);

		return pTextRenderer->GetBuffer();
	}
	BYTE* GetLinks(int nPageIndex)
	{
		return pReader->GetLinks(nPageIndex);
	}
	BYTE* GetStructure()
	{
		return pReader->GetStructure();
	}
	BYTE* GetInteractiveFormsInfo()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetWidgets();
		return NULL;
	}
	BYTE* GetAnnotFonts(int nTypeFonts)
	{
		if (nType == 0)
		{
			if (nTypeFonts == 1)
				return ((CPdfFile*)pReader)->GetAnnotEmbeddedFonts();
			if (nTypeFonts == 2)
				return ((CPdfFile*)pReader)->GetAnnotStandardFonts();
		}
		return NULL;
	}
	BYTE* GetAnnots(int nPageIndex = -1)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAnnots(nPageIndex);
		return NULL;
	}
	BYTE* GetAPWidget  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget  = -1, const char* sView  = NULL, const char* sBView = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAPWidget(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nWidget, sView, sBView);
		return NULL;
	}
	BYTE* GetButtonIcon(int nBackgroundColor, int nPageIndex, bool bBase64, int nBWidget = -1, const char* sIView = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetButtonIcon(nBackgroundColor, nPageIndex, bBase64, nBWidget, sIView);
		return NULL;
	}
	BYTE* GetAPAnnots  (int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nAnnot   = -1, const char* sView  = NULL)
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->GetAPAnnots(nRasterW, nRasterH, nBackgroundColor, nPageIndex, nAnnot, sView);
		return NULL;
	}
	std::wstring GetInfo()
	{
		return pReader->GetInfo();
	}
	bool IsNeedCMap()
	{
		if (nType == 0)
			return ((CPdfFile*)pReader)->IsNeedCMap();
		return false;
	}
	void SetCMapData(BYTE* pData, DWORD nSizeData)
	{
		if (nType == 0)
			((CPdfFile*)pReader)->SetCMapMemory(pData, nSizeData);
	}
	void DestroyText()
	{
		RELEASEOBJECT(pTextRenderer);
	}

	BYTE* GetPageShapes(const int& nPageIndex, int mode)
	{
		if (NULL == pImageStorage)
			pImageStorage = NSDocxRenderer::CreateWasmImageStorage();

		CDocxRenderer oRenderer(pApplicationFonts);
		oRenderer.SetExternalImageStorage(pImageStorage);
		oRenderer.SetTextAssociationType(NSDocxRenderer::TextAssociationType::tatParagraphToShape);
		
		std::vector<std::wstring> arShapes;
		if (0 == mode)
		 	arShapes = oRenderer.ScanPage(pReader, nPageIndex);
		else
		 	arShapes = oRenderer.ScanPagePptx(pReader, nPageIndex);

		int nLen = (int)arShapes.size();

		NSWasm::CData oRes;
		oRes.SkipLen();
		oRes.AddInt(nLen);

		for (int i = 0; i < nLen; ++i)
			oRes.WriteString(arShapes[i]);

		oRes.WriteLen();

		BYTE* res = oRes.GetBuffer();
		oRes.ClearWithoutAttack();
		return res;
	}

	std::string* GetImageBase64(int nRId)
	{
		if (NULL == pImageStorage)
			return NULL;
		return pImageStorage->GetBase64(nRId);
	}
};

#endif // _WASM_GRAPHICS_
