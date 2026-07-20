#ifndef HWPXFILE_H
#define HWPXFILE_H

#include "HWPSection.h"
#include "HwpFileHeader.h"
#include "HWPXCrypt.h"

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
	CHwpxCrypt m_oCrypt;
	EHwpxCryptResult m_eCryptResult;
public:
	CHWPXFile(const HWP_STRING& sFileName);
	~CHWPXFile();

	VECTOR<const CHWPSection*> GetSections() const;
	const CHWPDocInfo* GetDocInfo() const;
	bool GetChildStream(const HWP_STRING& sFileName, CHWPStream& oBuffer);

	void SetPassword(const HWP_STRING& sPassword);
	bool Encrypted() const;
	EHwpxCryptResult GetCryptResult() const;

	bool Detect();
	bool Open();
	void Close();
private:
	VECTOR<HWP_STRING> GetPathsToSections() const;
	bool GetFileHeader();
	void ReadManifest();
	bool ReadDocInfo();
	bool ReadSection(const HWP_STRING& sName);
	bool GetDocument(const HWP_STRING& sEntryName, CXMLReader& oReader);
};
}

#endif // HWPXFILE_H
