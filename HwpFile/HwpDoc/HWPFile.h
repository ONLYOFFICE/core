#ifndef HWPFILE_H
#define HWPFILE_H

#include "HwpFileHeader.h"
#include "OLEdoc/CompoundFile.h"
#include "HWPElements/HWPRecordBinData.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPFile
{
	HWP_STRING    m_sFileName;
	CCompoundFile  m_oOleFile;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHWPDocInfo       m_oDocInfo;
	VECTOR<CHWPSection*> m_arBodyTexts;
	VECTOR<CHWPSection*> m_arViewTexts;
public:
	CHWPFile(const HWP_STRING& sFileName);
	~CHWPFile();

	VECTOR<const CHWPSection*> GetSections();
	const CCompoundFile* GetOleFile() const;

	bool Detect();
	bool Open();
	void Close();

	bool GetFileHeader();
	const CHWPDocInfo* GetDocInfo() const;
	bool GetDocInfo(int nVersion);
	bool GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer);
	VECTOR<CDirectoryEntry*> GetBinData();
	void SetBinData(const VECTOR<CDirectoryEntry*>& arBinData);
	VECTOR<CHWPPargraph*> GetParas();
	void AddParas(const VECTOR<CHWPPargraph*>& arParas);
	bool GetChildStream(const HWP_STRING& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer);
private:
	void SaveChildEntries(const HWP_STRING& sBasePath, const HWP_STRING& sStorageName, ECompressed eCompressed);
	CDirectoryEntry* FindChildEntry(const HWP_STRING& sBasePath, const CDirectoryEntry& oBaseEntry, const HWP_STRING& sEntryName) const;
	HWP_STRING SaveChildEntry(const HWP_STRING& sRootPath, const HWP_STRING& sEntryName, ECompressed eCompressed);

	bool Unzip(CHWPStream& oInput, CHWPStream& oBuffer);
	bool Decrypt(CHWPStream& oInput, CHWPStream& oBuffer);

	bool GetBodyText(int nVersion);
	bool GetViewText(int nVersion);
};
}

#endif // HWPFILE_H
