#include "HWPFile.h"

#include "HwpDoc/HWPFile_Private.h"
#include "../DesktopEditor/common/File.h"

CHWPFile::CHWPFile(const std::wstring& wsFileName)
	: m_pInternal(new HWP::CHWPFile_Private(wsFileName))
{}

CHWPFile::~CHWPFile()
{
	if (nullptr != m_pInternal)
		delete m_pInternal;
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
