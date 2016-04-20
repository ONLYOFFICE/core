#include "PdfReader.h"
#include "../Common/OfficeDefines.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/GraphicsRenderer.h"
#include "../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../DesktopEditor/fontengine/FontManager.h"
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/common/Directory.h"

#include "Src/StringExt.h"
#include "Src/PDFDoc.h"
#include "Src/GlobalParams.h"
#include "Src/ErrorConstants.h"
#include "Src/ExtractImageOutputDev.h"
#include "Src/RendererOutputDev.h"

#include <string>

namespace PdfReader
{
    class CPdfReader_Private
    {
    public:
        PDFDoc*            m_pPDFDocument;
        GlobalParams*      m_pGlobalParams;
        std::wstring       m_wsTempFolder;
        std::wstring       m_wsCMapFolder;
        CApplicationFonts* m_pAppFonts;
        CFontManager*      m_pFontManager;
        CFontList*         m_pFontList;
    };

	CPdfReader::CPdfReader(CApplicationFonts* pAppFonts)
	{
        m_pInternal = new CPdfReader_Private();

        m_pInternal->m_wsTempFolder = L"";
        m_pInternal->m_wsCMapFolder = L"";

        m_pInternal->m_pPDFDocument = NULL;
        m_pInternal->m_pFontManager = NULL;

        m_pInternal->m_pGlobalParams = new GlobalParams();
        m_pInternal->m_pFontList = new CFontList();

        m_pInternal->m_pAppFonts = pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем
        m_pInternal->m_pFontManager = pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
		pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pInternal->m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(1);
        m_pInternal->m_pGlobalParams->SetFontManager(m_pInternal->m_pFontManager);
	}
	CPdfReader::~CPdfReader()
	{
        if (m_pInternal->m_pFontList)
		{
            m_pInternal->m_pFontList->Clear();
            delete m_pInternal->m_pFontList;
		}

        m_pInternal->m_wsCMapFolder = L"";

        if (!m_pInternal->m_wsTempFolder.empty())
		{
            NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);
            m_pInternal->m_wsTempFolder = L"";
		}

        RELEASEOBJECT((m_pInternal->m_pPDFDocument));
        RELEASEOBJECT((m_pInternal->m_pGlobalParams));
        RELEASEINTERFACE((m_pInternal->m_pFontManager));
	}
    bool CPdfReader::LoadFromFile(const std::wstring& wsSrcPath, const std::wstring& wsOptions,
                                    const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword)
	{
		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем 
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------
        RELEASEINTERFACE((m_pInternal->m_pFontManager));
        m_pInternal->m_pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
        pMeasurerCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
        m_pInternal->m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(1);
        m_pInternal->m_pGlobalParams->SetFontManager(m_pInternal->m_pFontManager);
		//------------------------------------------------------

        if (m_pInternal->m_pPDFDocument)
            delete m_pInternal->m_pPDFDocument;

		StringExt *seOwner = NULL, *seUser = NULL;
        if (!wsOwnerPassword.empty())
            seOwner = new StringExt(wsOwnerPassword.c_str());
        if (!wsUserPassword.empty())
            seUser = new StringExt(wsUserPassword.c_str());

        m_pInternal->m_pPDFDocument = new PDFDoc(m_pInternal->m_pGlobalParams, wsSrcPath.c_str(), seOwner, seUser);

		if (seUser)
			delete seUser;

		if (seOwner)
			delete seOwner;

        if (!m_pInternal->m_pPDFDocument || !m_pInternal->m_pPDFDocument->CheckValidation())
		{
            RELEASEOBJECT(m_pInternal->m_pPDFDocument);
			return false;
		}

        m_pInternal->m_pFontList->Clear();

        return (errorNone == m_pInternal->m_pPDFDocument->GetErrorCode());
	}
    void CPdfReader::Close()
	{
        RELEASEOBJECT((m_pInternal->m_pPDFDocument));
	}
    EError CPdfReader::GetError()
	{
        if (!m_pInternal->m_pPDFDocument)
			return errorOpenFile;

        return m_pInternal->m_pPDFDocument->GetErrorCode();
	}
    int CPdfReader::GetPagesCount()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

        return m_pInternal->m_pPDFDocument->GetPagesCount();
	}
    double CPdfReader::GetVersion()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

        return m_pInternal->m_pPDFDocument->GetPDFVersion();
	}
    int CPdfReader::GetPermissions()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

		int nPermissions = 0;

        if (m_pInternal->m_pPDFDocument->CheckPrint())
			nPermissions += PERMISSION_PRINT;
        if (m_pInternal->m_pPDFDocument->CheckCopy())
			nPermissions += PERMISSION_COPY;
        if (m_pInternal->m_pPDFDocument->CheckChange())
			nPermissions += PERMISSION_CHANGE;

		return nPermissions;
	}
    bool CPdfReader::ExtractAllImages(const wchar_t* wsDstPath, const wchar_t* wsPrefix)
	{
		StringExt seString(wsDstPath);
        ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev(m_pInternal->m_pGlobalParams, seString.GetBuffer(), true);
		if (!pOutputDev)
			return false;

        int nPagesCount = GetPagesCount();
        for (int nIndex = 1; nIndex <= nPagesCount; nIndex++)
		{
            m_pInternal->m_pPDFDocument->DisplayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
		}

		delete pOutputDev;

		return true;
	}
    void CPdfReader::GetPageInfo(int _nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
	{
		int nPageIndex = _nPageIndex + 1;

        if (!m_pInternal->m_pPDFDocument)
			return;

		const double c_dInch = 25.399; // Миллиметров в дюйме
		const double c_dXResolution = 154.0;
		const double c_dYResolution = 154.0;

		double dKoefX = c_dInch / c_dXResolution;
		double dKoefY = c_dInch / c_dYResolution;

        int nRotate = m_pInternal->m_pPDFDocument->GetPageRotate(nPageIndex);

		while (nRotate >= 360)
			nRotate -= 360;

		while (nRotate < 0)
			nRotate += 360;

		if (0 != nRotate && 180 != nRotate)
		{
            *pdHeight = m_pInternal->m_pPDFDocument->GetPageCropWidth(nPageIndex);
            *pdWidth  = m_pInternal->m_pPDFDocument->GetPageCropHeight(nPageIndex);
		}
		else
		{
            *pdWidth  = m_pInternal->m_pPDFDocument->GetPageCropWidth(nPageIndex);
            *pdHeight = m_pInternal->m_pPDFDocument->GetPageCropHeight(nPageIndex);
		}

		*pdDpiX   = 72;
		*pdDpiY   = 72;
	}
    void CPdfReader::DrawPageOnRenderer(IRenderer* pRenderer, int _nPageIndex, bool* pbBreak)
	{
		int nPageIndex = _nPageIndex + 1;

        if (m_pInternal->m_pPDFDocument && pRenderer)
		{
            RendererOutputDev oRendererOut(m_pInternal->m_pGlobalParams, pRenderer, m_pInternal->m_pFontManager, m_pInternal->m_pFontList);
            oRendererOut.NewPDF(m_pInternal->m_pPDFDocument->GetXRef());
			oRendererOut.SetBreak(pbBreak);
            m_pInternal->m_pPDFDocument->DisplayPage(&oRendererOut, nPageIndex, 72.0, 72.0, 0, false, true, false);
		}
	}
    void CPdfReader::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType)
	{
        CFontManager *pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
		CFontsCache* pFontCache = new CFontsCache();
        pFontCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		CGraphicsRenderer oRenderer;
		oRenderer.SetFontManager(pFontManager);

		double dWidth, dHeight;
        double dDpiX, dDpiY;
        GetPageInfo(nPageIndex, &dWidth, &dHeight, &dDpiX, &dDpiY);

		int nWidth  = (int)dWidth  * 72 / 25.4;
		int nHeight = (int)dHeight * 72 / 25.4;

		BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
		if (!pBgraData)
			return;

		memset(pBgraData, 0xff, nWidth * nHeight * 4);
		CBgraFrame oFrame;
		oFrame.put_Data(pBgraData);
		oFrame.put_Width(nWidth);
		oFrame.put_Height(nHeight);
		oFrame.put_Stride(-4 * nWidth);

		oRenderer.CreateFromBgraFrame(&oFrame);
		oRenderer.SetSwapRGB(false);
		oRenderer.put_Width(dWidth);
		oRenderer.put_Height(dHeight);

		bool bBreak = false;
		DrawPageOnRenderer(&oRenderer, nPageIndex, &bBreak);

		oFrame.SaveFile(wsDstPath, nImageType);
		RELEASEINTERFACE(pFontManager);
	}
    int CPdfReader::GetImagesCount()
	{
        ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev(m_pInternal->m_pGlobalParams, NULL, true, true);
		if (!pOutputDev)
			return 0;

        for (int nIndex = 1; nIndex <= m_pInternal->m_pPDFDocument->GetPagesCount(); nIndex++)
		{
            m_pInternal->m_pPDFDocument->DisplayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
		}

		return pOutputDev->GetImagesCount();
	}	
    void CPdfReader::SetTempDirectory(const std::wstring& wsTempFolder)
	{		
        if (!m_pInternal->m_wsTempFolder.empty())
		{
            NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);
            m_pInternal->m_wsTempFolder = wsTempFolder;
		}

        if (!wsTempFolder.empty())
		{
			std::wstring wsFolderName = std::wstring(wsTempFolder) + L"//pdftemp";
			std::wstring wsFolder = wsFolderName;
			int nCounter = 0;
			while (NSDirectory::Exists(wsFolder))
			{
				nCounter++;
				wsFolder = wsFolderName + L"_" + std::to_wstring(nCounter);
			}
			NSDirectory::CreateDirectory(wsFolder);
            m_pInternal->m_wsTempFolder = wsFolder;
		}
		else
            m_pInternal->m_wsTempFolder = L"";

        if (m_pInternal->m_pGlobalParams)
            m_pInternal->m_pGlobalParams->SetTempFolder(m_pInternal->m_wsTempFolder.c_str());
	}
    std::wstring CPdfReader::GetTempDirectory()
    {
        return m_pInternal->m_wsTempFolder;
    }

    void CPdfReader::SetCMapFolder(const wchar_t* wsCMapFolder)
	{
        m_pInternal->m_wsCMapFolder = std::wstring(wsCMapFolder);

        if (m_pInternal->m_pGlobalParams)
            m_pInternal->m_pGlobalParams->SetCMapFolder(m_pInternal->m_wsCMapFolder.c_str());
	}
    CFontManager* CPdfReader::GetFontManager()
	{
        return m_pInternal->m_pFontManager;
	}
}
