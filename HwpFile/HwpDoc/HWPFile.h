#ifndef HWPFILE_H
#define HWPFILE_H

#include "HwpFileHeader.h"
#include <list>

namespace HWP
{
class CHWPFile
{
	std::string    m_sFileName;
	CCompoundFile  m_oOleFile;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHwpDocInfo    m_oDocInfo;
	std::list<CHwpSection> m_lBodyText;
	std::list<CHwpSection> m_lViewText;
public:
	CHWPFile();
};
}

#endif // HWPFILE_H
