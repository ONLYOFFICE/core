#include "OFDFile_Private.h"

#include "../../OfficeUtils/src/OfficeUtils.h"

COFDFile_Private::COFDFile_Private(NSFonts::IApplicationFonts* pFonts)
	: m_pAppFonts(pFonts), m_pTempFolder(nullptr)
{
	if (nullptr == pFonts)
		return;

	// Создаем менеджер шрифтов с собственным кэшем
	m_pFontManager = m_pAppFonts->GenerateFontManager();
	NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
	pMeasurerCache->SetStreams(m_pAppFonts->GetStreams());
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
{
}

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

	if (!m_oBase.Read(pFolder))
		return false;

	return false;
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

NSFonts::IApplicationFonts* COFDFile_Private::GetFonts()
{
	return m_pAppFonts;
}
