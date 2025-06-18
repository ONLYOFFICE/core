#include "OFDFile_Private.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "Utils/Utils.h"

COFDFile_Private::COFDFile_Private(NSFonts::IApplicationFonts* pFonts)
	: m_pAppFonts(pFonts), m_pFontManager(nullptr), m_pTempFolder(nullptr)
{
	if (nullptr == pFonts)
		return;

	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = pFonts->GenerateFontManager();
	NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
	pMeasurerCache->SetStreams(pFonts->GetStreams());
	m_pFontManager->SetOwnerCache(pMeasurerCache);
	pMeasurerCache->SetCacheSize(16);
}

COFDFile_Private::~COFDFile_Private()
{
	Close();

	if (nullptr != m_pTempFolder)
		delete m_pTempFolder;

	RELEASEINTERFACE(m_pFontManager);
}

void COFDFile_Private::Close()
{}

void COFDFile_Private::SetTempDir(const std::wstring& wsPath)
{
	if (nullptr != m_pTempFolder)
		delete m_pTempFolder;

	if (!NSDirectory::Exists(wsPath))
		NSDirectory::CreateDirectory(wsPath);

	int nCounter = 0;
	std::wstring wsTempFolder = wsPath + L"/OFD/";

	while (NSDirectory::Exists(wsTempFolder))
	{
		wsTempFolder = wsPath + L"/OFD" + std::to_wstring(nCounter) + L'/';
		nCounter++;
	}

	NSDirectory::CreateDirectory(wsTempFolder);

	m_pTempFolder = new CFolderSystem(wsTempFolder);
}

std::wstring COFDFile_Private::GetTempDir() const
{
	return (nullptr != m_pTempFolder) ? m_pTempFolder->getFullFilePath(L"") : std::wstring();
}

bool COFDFile_Private::Read(IFolder* pFolder)
{
	if (nullptr == pFolder)
		return false;

	return m_oBase.Read(pFolder);
}

bool COFDFile_Private::LoadFromFile(const std::wstring& wsFilePath)
{
	if (wsFilePath.empty() || nullptr == m_pTempFolder)
		return false;

	Close();

	COfficeUtils oUtils(NULL);

	if (S_OK != oUtils.ExtractToDirectory(wsFilePath, m_pTempFolder->getFullFilePath(L""), NULL, 0))
		return false;

	return Read(m_pTempFolder);
}

bool COFDFile_Private::LoadFromMemory(BYTE* pData, DWORD ulLength)
{
	Close();

	if (nullptr != m_pTempFolder)
		delete m_pTempFolder;

	m_pTempFolder = new CZipFolderMemory(pData, ulLength);

	return Read(m_pTempFolder);
}

unsigned int COFDFile_Private::GetPageCount() const
{
	return m_oBase.GetPageCount();
}

void COFDFile_Private::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	m_oBase.GetPageSize(nPageIndex, dWidth, dHeight);
}

void COFDFile_Private::DrawPage(IRenderer* pRenderer, int nPageIndex)
{
	m_oBase.DrawPage(pRenderer, nPageIndex);
}

void COFDFile_Private::DrawPage(IRenderer* pRenderer, int nPageIndex, const double& dX, const double& dY, const double& dWidth, const double& dHeight)
{
	if (nullptr == pRenderer)
		return;

	double dPageWidth = 0., dPageHeight = 0.;

	GetPageSize(nPageIndex, dPageWidth, dPageHeight);

	if (OFD::IsZeroValue(dPageWidth) || OFD::IsZeroValue(dPageHeight))
		return;

	double dM11, dM12, dM21, dM22, dDx, dDy;
	pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

	Aggplus::CMatrix oTransform(dM11, dM12, dM21, dM22, dDx, dDy);

	oTransform.Scale(dWidth / dPageWidth, dHeight / dPageHeight);
	oTransform.Translate(dX, dY);

	pRenderer->SetTransform(oTransform.sx(), oTransform.shy(), oTransform.shx(), oTransform.sy(), oTransform.tx(), oTransform.ty());

	m_oBase.DrawPage(pRenderer, nPageIndex);

	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
}

NSFonts::IApplicationFonts* COFDFile_Private::GetFonts()
{
	return m_pAppFonts;
}
