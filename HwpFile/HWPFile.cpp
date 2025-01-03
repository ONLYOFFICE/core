#include "HWPFile.h"

#include "HwpDoc/HWPFile_Private.h"
#include "../DesktopEditor/common/File.h"

#include "HwpDoc/Conversion/Converter2OOXML.h"

CHWPFile::CHWPFile(const std::wstring& wsFileName)
	: m_pInternal(new HWP::CHWPFile_Private(wsFileName))
{}

CHWPFile::~CHWPFile()
{
	if (nullptr != m_pInternal)
		delete m_pInternal;
}

void CHWPFile::SetTempDirectory(const std::wstring& wsTempDirectory)
{
	m_wsTempDirectory = wsTempDirectory;
}

bool CHWPFile::Open()
{
	if (nullptr == m_pInternal)
		return false;

	return m_pInternal->Open();
}

void CHWPFile::Close()
{
	if (nullptr != m_pInternal)
		m_pInternal->Close();
}

bool CHWPFile::ConvertToOOXML(const std::wstring& wsFilePath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetHWPFile(m_pInternal);
	oConverter.SetTempDirectory(m_wsTempDirectory);

	return oConverter.ConvertToFile(wsFilePath);
}

bool CHWPFile::ConvertToOOXML_Dir(const std::wstring& wsDirectoryPath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetHWPFile(m_pInternal);

	return oConverter.ConvertToDir(wsDirectoryPath);
}
