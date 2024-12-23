#include "Converter2OOXML.h"

namespace HWP
{
CConverter2OOXML::CConverter2OOXML()
	: m_pHWPFile(nullptr)
{}

void CConverter2OOXML::SetHWPFile(CHWPFile_Private* pHWPFile)
{
	m_pHWPFile = pHWPFile;
}

void CConverter2OOXML::SetTempDirectory(const STRING& sTempDirectory)
{
	m_sTempDirectory = sTempDirectory;
}

bool CConverter2OOXML::ConvertTo(const STRING& sFilePath)
{
	if (nullptr == m_pHWPFile || sFilePath.empty())
		return false;

	std::vector<CHWPSection*> arSections = m_pHWPFile->GetSections();

	return false;
}
}
