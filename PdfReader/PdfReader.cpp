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
	CPdfReader::CPdfReader(CApplicationFonts* pAppFonts)
	{
		m_wsTempFolder = NULL;
		m_wsCMapFolder = NULL;

		m_pPDFDocument = NULL;
		m_pFontManager = NULL;

		m_pGlobalParams = new GlobalParams();
		m_pFontList = new CFontList();

		m_pAppFonts = pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем
		m_pFontManager = pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
		pMeasurerCache->SetStreams(pAppFonts->GetStreams());
		m_pFontManager->SetOwnerCache(pMeasurerCache);
		m_pGlobalParams->SetFontManager(m_pFontManager);
	}
	CPdfReader::~CPdfReader()
	{
		if (m_pFontList)
		{
			m_pFontList->Clear();
			delete m_pFontList;
		}

		if (m_wsCMapFolder)
			FreeWString(m_wsCMapFolder);

		if (m_wsTempFolder)
		{
			NSDirectory::DeleteDirectory(m_wsTempFolder);
			FreeWString(m_wsTempFolder);
		}

		RELEASEOBJECT(m_pPDFDocument);
		RELEASEOBJECT(m_pGlobalParams);
		RELEASEINTERFACE(m_pFontManager);
	}
	bool         CPdfReader::LoadFromFile(const wchar_t* wsSrcPath, const wchar_t* wsOwnerPassword, const wchar_t* wsUserPassword, const wchar_t* wsOptions)
	{
		// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем 
		//       FontManager, потому что сейчас в нем кэш без ограничения.
		//------------------------------------------------------
		RELEASEINTERFACE(m_pFontManager);
		m_pFontManager = m_pAppFonts->GenerateFontManager();
		CFontsCache* pMeasurerCache = new CFontsCache();
		pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
		m_pFontManager->SetOwnerCache(pMeasurerCache);
		m_pGlobalParams->SetFontManager(m_pFontManager);
		//------------------------------------------------------

		if (m_pPDFDocument)
			delete m_pPDFDocument;

		StringExt *seOwner = NULL, *seUser = NULL;
		if (NULL != wsOwnerPassword)
			seOwner = new StringExt(wsOwnerPassword);
		if (NULL != wsUserPassword)
			seUser = new StringExt(wsUserPassword);

		m_pPDFDocument = new PDFDoc(m_pGlobalParams, wsSrcPath, seOwner, seUser);

		if (seUser)
			delete seUser;

		if (seOwner)
			delete seOwner;

		if (!m_pPDFDocument || !m_pPDFDocument->CheckValidation())
		{
			if (m_pPDFDocument)
				delete m_pPDFDocument;

			return false;
		}

		m_pFontList->Clear();

		return (errorNone == m_pPDFDocument->GetErrorCode());
	}
	void         CPdfReader::Close()
	{
		RELEASEOBJECT(m_pPDFDocument);
	}
	EError       CPdfReader::GetError()
	{
		return m_pPDFDocument->GetErrorCode();
	}
	int          CPdfReader::GetPagesCount()
	{
		if (!m_pPDFDocument)
			return 0;

		return m_pPDFDocument->GetPagesCount();
	}
	double       CPdfReader::GetVersion()
	{
		if (!m_pPDFDocument)
			return 0;

		return m_pPDFDocument->GetPDFVersion();
	}
	int          CPdfReader::GetPermissions()
	{
		if (!m_pPDFDocument)
			return 0;

		int nPermissions = 0;

		if (m_pPDFDocument->CheckPrint())
			nPermissions += PERMISSION_PRINT;
		if (m_pPDFDocument->CheckCopy())
			nPermissions += PERMISSION_COPY;
		if (m_pPDFDocument->CheckChange())
			nPermissions += PERMISSION_CHANGE;

		return nPermissions;
	}
	bool         CPdfReader::ExtractAllImages(const wchar_t* wsDstPath, const wchar_t* wsPrefix)
	{
		StringExt seString(wsDstPath);
		ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev(m_pGlobalParams, seString.GetBuffer(), true);
		if (!pOutputDev)
			return false;

		for (int nIndex = 1; nIndex <= m_pPDFDocument->GetPagesCount(); nIndex++)
		{
			m_pPDFDocument->DisplayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
		}

		delete pOutputDev;

		return true;
	}
	void         CPdfReader::GetPageSize(int _nPageIndex, double* pdWidth, double* pdHeight)
	{
		int nPageIndex = _nPageIndex + 1;

		if (!m_pPDFDocument)
			return;

		const double c_dInch = 25.399; // Миллиметров в дюйме
		const double c_dXResolution = 154.0;
		const double c_dYResolution = 154.0;

		double dKoefX = c_dInch / c_dXResolution;
		double dKoefY = c_dInch / c_dYResolution;

		int nRotate = m_pPDFDocument->GetPageRotate(nPageIndex);

		while (nRotate >= 360)
			nRotate -= 360;

		while (nRotate < 0)
			nRotate += 360;

		if (0 != nRotate && 180 != nRotate)
		{
			*pdHeight = PDFCoordsToMM(m_pPDFDocument->GetPageCropWidth(nPageIndex));
			*pdWidth  = PDFCoordsToMM(m_pPDFDocument->GetPageCropHeight(nPageIndex));
		}
		else
		{
			*pdWidth  = PDFCoordsToMM(m_pPDFDocument->GetPageCropWidth(nPageIndex));
			*pdHeight = PDFCoordsToMM(m_pPDFDocument->GetPageCropHeight(nPageIndex));
		}
	}
	void         CPdfReader::DrawPageOnRenderer(IRenderer* pRenderer, int _nPageIndex, bool* pbBreak)
	{
		int nPageIndex = _nPageIndex + 1;

		if (m_pPDFDocument && pRenderer)
		{
			RendererOutputDev oRendererOut(m_pGlobalParams, pRenderer, m_pFontManager, m_pFontList);
			oRendererOut.NewPDF(m_pPDFDocument->GetXRef());
			oRendererOut.SetBreak(pbBreak);
			m_pPDFDocument->DisplayPage(&oRendererOut, nPageIndex, 72.0, 72.0, 0, false, true, false);
		}
	}
	void         CPdfReader::ConvertToRaster(int nPageIndex, const wchar_t* wsDstPath, int nImageType)
	{
		CFontManager *pFontManager = m_pAppFonts->GenerateFontManager();
		CFontsCache* pFontCache = new CFontsCache();
		pFontCache->SetStreams(m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

		CGraphicsRenderer oRenderer;
		oRenderer.SetFontManager(pFontManager);

		double dWidth, dHeight;
		GetPageSize(nPageIndex, &dWidth, &dHeight);

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
	int          CPdfReader::GetImagesCount()
	{
		ExtractImageOutputDev *pOutputDev = new ExtractImageOutputDev(m_pGlobalParams, NULL, true, true);
		if (!pOutputDev)
			return 0;

		for (int nIndex = 1; nIndex <= m_pPDFDocument->GetPagesCount(); nIndex++)
		{
			m_pPDFDocument->DisplayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
		}

		return pOutputDev->GetImagesCount();
	}	
	void         CPdfReader::SetTempFolder(const wchar_t* wsTempFolder)
	{		
		if (m_wsTempFolder)
		{
			NSDirectory::DeleteDirectory(m_wsTempFolder);
			FreeWString(m_wsTempFolder);
		}

		if (NULL != wsTempFolder)
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
			m_wsTempFolder = AllocWString(wsFolder);
		}
		else
			m_wsTempFolder = NULL;

		if (m_pGlobalParams)
			m_pGlobalParams->SetTempFolder(m_wsTempFolder);
	}
	void         CPdfReader::SetCMapFolder(const wchar_t* wsCMapFolder)
	{
		if (m_wsCMapFolder)
			FreeWString(m_wsCMapFolder);

		if (NULL != wsCMapFolder)
			m_wsCMapFolder = AllocWString(wsCMapFolder);
		else
			m_wsCMapFolder = NULL;


		if (m_pGlobalParams)
			m_pGlobalParams->SetCMapFolder(m_wsCMapFolder);
	}
	CFontManager*CPdfReader::GetFontManager()
	{
		return m_pFontManager;
	}
}