#include "HWPFile.h"

#include "HwpDoc/Common/WriterContext.h"
#include "../DesktopEditor/common/File.h"

#include "HwpDoc/Conversion/Converter2OOXML.h"

CHWPFile::CHWPFile()
	: m_pInternal(new HWP::CWriterContext())
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

bool CHWPFile::OpenHWP(const std::wstring& wsFilePath)
{
	if (nullptr == m_pInternal)
		return false;

	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWP);
}

bool CHWPFile::OpenHWPX(const std::wstring& wsFilePath)
{
	if (nullptr == m_pInternal)
		return false;

	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWPX);
}

bool CHWPFile::OpenHWPML(const std::wstring &wsFilePath)
{
	if (nullptr == m_pInternal)
		return false;
	
	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWPML);
}

void CHWPFile::Close()
{
	if (nullptr != m_pInternal)
		m_pInternal->Close();
}

bool CHWPFile::ConvertToOOXML(const std::wstring& wsFilePath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetContext(m_pInternal);
	oConverter.SetTempDirectory(m_wsTempDirectory);

	return oConverter.ConvertToFile(wsFilePath);
}

bool CHWPFile::ConvertToOOXML_Dir(const std::wstring& wsDirectoryPath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetContext(m_pInternal);

	return oConverter.ConvertToDir(wsDirectoryPath);
}

bool CHWPFile::IsHWPFormat(const std::wstring& wsFilePath)
{
	return HWP::EHanType::HWP == HWP::CWriterContext::DetectHancom(wsFilePath);
}

bool CHWPFile::IsHWPXFormat(const std::wstring& wsFilePath)
{
	return HWP::EHanType::HWPX == HWP::CWriterContext::DetectHancom(wsFilePath);
}
