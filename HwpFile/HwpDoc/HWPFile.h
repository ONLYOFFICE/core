#ifndef HWPFILE_H
#define HWPFILE_H

#include "HwpFileHeader.h"
#include "OLEdoc/CompoundFile.h"
#include "HWPSection.h"
#include <list>

namespace HWP
{
class CHWPDocInfo;

class CHWPFile
{
	STRING    m_sFileName;
	CCompoundFile  m_oOleFile;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHWPDocInfo       m_oDocInfo;
	VECTOR<CHWPSection*> m_arBodyTexts;
	VECTOR<CHWPSection*> m_arViewTexts;
public:
	CHWPFile(const STRING& sFileName);
	~CHWPFile();

	VECTOR<CHWPSection*> GetSections();
	CCompoundFile* GetOleFile();

	bool Detect();
	bool Open();
};
}

#endif // HWPFILE_H
