#ifndef HWPXFILE_PRIVATE_H
#define HWPXFILE_PRIVATE_H

#include "HWPSection.h"
#include "HwpFileHeader.h"

#include "../../OfficeUtils/src/ZipFolder.h"

namespace HWP
{
class CHWPXFile_Private
{
	HWP_STRING m_sFileName;
	CZipFolderMemory *m_pZipFolder;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHWPDocInfo m_oDocInfo;
	VECTOR<CHWPSection*> m_arSections;
public:
	CHWPXFile_Private(const HWP_STRING& sFileName);
	~CHWPXFile_Private();

	VECTOR<const CHWPSection*> GetSections() const;
	const CHWPDocInfo* GetDocInfo() const;

	bool Detect();
	bool Open();
	void Close();
private:
	VECTOR<HWP_STRING> GetPathsToSections() const;
	bool GetFileHeader();
	bool GetDocInfo(int nVersion);
	bool ReadSection(const HWP_STRING& sName, int nVersion);
	CXMLNode GetDocument(const HWP_STRING& sEntryName);
};
}

#endif // HWPXFILE_PRIVATE_H
