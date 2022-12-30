#ifndef _WASM_GRAPHICS_
#define _WASM_GRAPHICS_

#include "../../../../GraphicsRenderer.h"
#include "../../../../pro/Graphics.h"
#include "../../../../pro/officedrawingfile.h"
#include "../../../../../../XpsFile/XpsFile.h"
#include "../../../../../../DjVuFile/DjVu.h"
#include "../../../../../../PdfFile/PdfFile.h"
#include "../../../../../../HtmlRenderer/include/HTMLRendererText.h"

class CGraphicsFileDrawing
{
private:
    IOfficeDrawingFile* pReader;
    NSFonts::IApplicationFonts* pApplicationFonts;
    NSFonts::IFontManager* pFontManager;
    NSHtmlRenderer::CHTMLRendererText* pTextRenderer;
    int nType;
public:
    CGraphicsFileDrawing(NSFonts::IApplicationFonts* pFonts)
    {
        pReader = NULL;
        pTextRenderer = NULL;
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
    void GetPageInfo(int nPageIndex, int& nWidth, int& nHeight, int& nPageDpiX)
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
        nWidth    = dWidth;
        nHeight   = dHeight;
        nPageDpiX = dPageDpiX;
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
};

#endif // _WASM_GRAPHICS_
