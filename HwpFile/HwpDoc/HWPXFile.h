#ifndef HWPXFILE_H
#define HWPXFILE_H

#include "HWPSection.h"
#include "HwpFileHeader.h"

#include "../../OfficeUtils/src/ZipFolder.h"

namespace HWP
{
class CHWPXFile
{
	HWP_STRING m_sFileName;
	CZipFolderMemory *m_pZipFolder;
	CHwpFileHeader m_oFileHeader;
	CHWPDocInfo m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
public:
	CHWPXFile(const HWP_STRING& sFileName);
	~CHWPXFile();

	VECTOR<const CHWPSection*> GetSections() const;
	const CHWPDocInfo* GetDocInfo() const;
	bool GetChildStream(const HWP_STRING& sFileName, CHWPStream& oBuffer);

	bool Detect();
	bool Open();
	void Close();
private:
	VECTOR<HWP_STRING> GetPathsToSections() const;
	bool GetFileHeader();
	bool GetDocInfo(int nVersion);
	bool ReadSection(const HWP_STRING& sName);
	bool GetDocument(const HWP_STRING& sEntryName, CXMLReader& oReader);
};
}

#endif // HWPXFILE_H
