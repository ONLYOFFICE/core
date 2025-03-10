#include "OFDFile.h"

#include "../OfficeUtils/src/OfficeUtils.h"

namespace OFD
{
COfdFile::COfdFile()
{}

COfdFile::~COfdFile()
{
	Close();
}

void COfdFile::Close()
{
}

void COfdFile::SetTempDir(const std::wstring& wsPath)
{
	m_wsTempDir = wsPath;
}

std::wstring COfdFile::GetTempDir() const
{
	return m_wsTempDir;
}

bool COfdFile::Read(IFolder* pFolder)
{
	if (nullptr == pFolder)
		return false;

	if (!m_oBase.Read(pFolder))
		return false;

	return false;
}

IFolder* COfdFile::CreateTempDir() const
{
	if (!NSDirectory::Exists(m_wsTempDir))
		NSDirectory::CreateDirectory(m_wsTempDir);

	int nCounter = 0;
	std::wstring wsTempFolder = m_wsTempDir + L"/OFD/";

	while (NSDirectory::Exists(wsTempFolder))
	{
		wsTempFolder = m_wsTempDir + L"/OFD" + std::to_wstring(nCounter) + L'/';
		nCounter++;
	}

	NSDirectory::CreateDirectory(wsTempFolder);

	return new CFolderSystem(wsTempFolder);
}

bool COfdFile::LoadFromFile(const std::wstring& wsFilePath)
{
	Close();

	IFolder* pFolder = CreateTempDir();

	if (nullptr == pFolder)
		return false;

	COfficeUtils oUtils(NULL);

	if (S_OK != oUtils.ExtractToDirectory(wsFilePath, pFolder->getFullFilePath(L""), NULL, 0))
		return false;

	bool bResult = Read(pFolder);

	if (!bResult)
	{
		pFolder->removeDirectory(L"");
		delete pFolder;
	}

	return false;
}

bool COfdFile::SaveToFile(const std::wstring& wsFilePath)
{
	return false;
}

bool COfdFile::SaveToDir(const std::wstring& wsDir)
{
	return false;
}
}
