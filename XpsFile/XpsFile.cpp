#include "XpsFile.h"
#include "XpsLib/Document.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/fontengine/FontManager.h"
#include "../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../DesktopEditor/graphics/GraphicsRenderer.h"
#include "../PdfWriter/PdfRenderer.h"

#include "../OfficeUtils/src/OfficeUtils.h"

using namespace XPS;

class CXpsFile_Private
{
public:
    CApplicationFonts* m_pAppFonts;
    CFontManager*      m_pFontManager;
    std::wstring       m_wsTempFolder;
    XPS::CDocument*    m_pDocument;

public:
    CXpsFile_Private(CApplicationFonts* pAppFonts)
    {
        m_pDocument = NULL;

        m_pAppFonts = pAppFonts;

        // Создаем менеджер шрифтов с собственным кэшем
        m_pFontManager = pAppFonts->GenerateFontManager();
        CFontsCache* pMeasurerCache = new CFontsCache();
        pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(16);

        m_wsTempFolder = L"";
    }
    ~CXpsFile_Private()
    {
    }
};

CXpsFile::CXpsFile(CApplicationFonts* pAppFonts)
{
    m_pInternal = new CXpsFile_Private(pAppFonts);
    SetTempDirectory(NSFile::CFileBinary::GetTempPath());
}
CXpsFile::~CXpsFile()
{
    if (L"" != m_pInternal->m_wsTempFolder)
        NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);

	Close();
    RELEASEINTERFACE((m_pInternal->m_pFontManager));
}
std::wstring CXpsFile::GetTempDirectory()
{
    return m_pInternal->m_wsTempFolder;
}
void CXpsFile::SetTempDirectory(const std::wstring& wsPath)
{
    if (L"" != m_pInternal->m_wsTempFolder)
        NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);

	int nCounter = 0;
    m_pInternal->m_wsTempFolder = wsPath + L"/XPS/";
    while (NSDirectory::Exists(m_pInternal->m_wsTempFolder))
	{
        m_pInternal->m_wsTempFolder = wsPath + L"/XPS" + std::to_wstring(nCounter) + L"/";
		nCounter++;
	}
    NSDirectory::CreateDirectory(m_pInternal->m_wsTempFolder);
}
bool CXpsFile::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions,
                            const std::wstring& owner_password, const std::wstring& user_password)
{
	Close();

	// Распаковываем Zip-архив в темповую папку
	COfficeUtils oUtils(NULL);
    if (S_OK != oUtils.ExtractToDirectory(wsSrcFileName, m_pInternal->m_wsTempFolder, NULL, 0))
		return false;

    m_pInternal->m_pDocument = new XPS::CDocument(m_pInternal->m_pFontManager);
    if (!m_pInternal->m_pDocument)
		return false;

    std::wstring wsPath = m_pInternal->m_wsTempFolder + L"/";
    m_pInternal->m_pDocument->ReadFromPath(wsPath);

	return true;
}
void CXpsFile::Close()
{
    if (m_pInternal->m_pDocument)
	{
        m_pInternal->m_pDocument->Close();
        delete m_pInternal->m_pDocument;
        m_pInternal->m_pDocument = NULL;
	}
}
int CXpsFile::GetPagesCount()
{
    if (!m_pInternal->m_pDocument)
		return 0;

    return m_pInternal->m_pDocument->GetPageCount();
}
void CXpsFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	int nW = 0, nH = 0;

    if (m_pInternal->m_pDocument)
        m_pInternal->m_pDocument->GetPageSize(nPageIndex, nW, nH);

	*pdWidth  = nW * 25.4 / 96;
	*pdHeight = nH * 25.4 / 96;
	*pdDpiX   = 25.4;
	*pdDpiY   = 25.4;
}
void CXpsFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
    if (!m_pInternal->m_pDocument)
		return;

    m_pInternal->m_pDocument->DrawPage(nPageIndex, pRenderer, pBreak);
}
void CXpsFile::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType)
{
    CFontManager *pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
	CFontsCache* pFontCache = new CFontsCache();
    pFontCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
	pFontManager->SetOwnerCache(pFontCache);

	CGraphicsRenderer oRenderer;
	oRenderer.SetFontManager(pFontManager);

	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

	int nWidth  = (int)dWidth * 96 / dPageDpiX;
	int nHeight = (int)dHeight * 96 / dPageDpiX;

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
void CXpsFile::ConvertToPdf(const std::wstring& wsPath)
{
    CPdfRenderer oPdf(m_pInternal->m_pAppFonts);
	bool bBreak = false;

	int nPagesCount = GetPagesCount();
	for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
	{
		oPdf.NewPage();
		oPdf.BeginCommand(c_nPageType);

		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

		dWidth  *= 25.4 / dPageDpiX;
		dHeight *= 25.4 / dPageDpiY;

		oPdf.put_Width(dWidth);
		oPdf.put_Height(dHeight);

		DrawPageOnRenderer(&oPdf, nPageIndex, &bBreak);

		oPdf.EndCommand(c_nPageType);

#ifdef _DEBUG
		printf("page %d / %d\n", nPageIndex + 1, nPagesCount);
#endif
	}

	oPdf.SaveToFile(wsPath);
}
