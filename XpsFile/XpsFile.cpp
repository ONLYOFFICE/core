#include "XpsFile.h"
#include "XpsLib/Folder.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/fontengine/FontManager.h"
#include "../DesktopEditor/fontengine/ApplicationFonts.h"
#include "../DesktopEditor/graphics/GraphicsRenderer.h"

#include "../ASCOfficeUtils/ASCOfficeUtilsLib/OfficeUtils.h"

using namespace XPS;

CXpsFile::CXpsFile(CApplicationFonts* pAppFonts)
{
	std::wstring wsTemp = NSFile::CFileBinary::GetTempPath();
	wsTemp += L"/XPS/";
	m_wsTempDirectory = wsTemp;
	NSDirectory::CreateDirectory(m_wsTempDirectory);

	m_pAppFonts = pAppFonts;

	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = pAppFonts->GenerateFontManager();
	CFontsCache* pMeasurerCache = new CFontsCache();
	pMeasurerCache->SetStreams(pAppFonts->GetStreams());
	m_pFontManager->SetOwnerCache(pMeasurerCache);
}
CXpsFile::~CXpsFile()
{
	Close();
	NSDirectory::DeleteDirectory(m_wsTempDirectory);
	RELEASEINTERFACE(m_pFontManager);
}
std::wstring CXpsFile::GetTempDirectory() const
{
	return m_wsTempDirectory;
}
void         CXpsFile::SetTempDirectory(const std::wstring& wsDirectory)
{
	NSDirectory::DeleteDirectory(m_wsTempDirectory);
	m_wsTempDirectory = wsDirectory;
	m_wsTempDirectory += L"/XPS/";
	NSDirectory::CreateDirectory(m_wsTempDirectory);
}
bool         CXpsFile::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXmlOptions)
{
	Close();

	// Распаковываем Zip-архив в темповую папку
	COfficeUtils oUtils(NULL);
	if (S_OK != oUtils.ExtractToDirectory(wsSrcFileName, m_wsTempDirectory, NULL, 0))
		return false;

	m_pFolder = new XPS::Folder(m_pFontManager);
	if (!m_pFolder)
		return false;

	std::wstring wsPath = m_wsTempDirectory + L"/";
	m_pFolder->ReadFromPath(wsPath);

	return true;
}
void         CXpsFile::Close()
{
	if (m_pFolder)
	{
		m_pFolder->Close();
		delete m_pFolder;
		m_pFolder = NULL;
	}
}
int          CXpsFile::GetPagesCount()
{
	if (!m_pFolder)
		return 0;

	return m_pFolder->GetPageCount();
}
void         CXpsFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	int nW = 0, nH = 0;

	if (m_pFolder)
		m_pFolder->GetPageSize(nPageIndex, nW, nH);

	*pdWidth  = nW * 25.4 / 96;
	*pdHeight = nH * 25.4 / 96;
	*pdDpiX   = 96;
	*pdDpiY   = 96;
}
void         CXpsFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
	if (!m_pFolder)
		return;

	m_pFolder->DrawPage(nPageIndex, pRenderer, pBreak);
}
void         CXpsFile::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType)
{
	CFontManager *pFontManager = m_pAppFonts->GenerateFontManager();
	CFontsCache* pFontCache = new CFontsCache();
	pFontCache->SetStreams(m_pAppFonts->GetStreams());
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