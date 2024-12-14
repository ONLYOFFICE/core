#include "HWPFile.h"
#include "HWPDocInfo.h"

namespace HWP
{
CHWPFile::CHWPFile(const STRING& sFileName)
	: m_sFileName(sFileName), m_oOleFile(sFileName), m_oDocInfo(this)
{}

CHWPFile::~CHWPFile()
{
	CLEAR_ARRAY(CHWPSection, m_arBodyTexts);
	CLEAR_ARRAY(CHWPSection, m_arViewTexts);
}

std::vector<CHWPSection*> CHWPFile::GetSections()
{
	if (m_oFileHeader.GetDistributable())
		return m_arViewTexts;
	else
		return m_arBodyTexts;
}

CCompoundFile* CHWPFile::GetOleFile()
{
	return &m_oOleFile;
}

bool CHWPFile::Detect()
{
	return true;
}

bool CHWPFile::Open()
{
	return true;
}
}
