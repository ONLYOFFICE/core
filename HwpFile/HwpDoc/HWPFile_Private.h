#ifndef HWPFILE_PRIVATE_H
#define HWPFILE_PRIVATE_H

#include "HwpFileHeader.h"
#include "OLEdoc/CompoundFile.h"
#include "HWPElements/HWPRecordBinData.h"
#include "HWPSection.h"

namespace HWP
{
class CHWPFile_Private
{
	STRING    m_sFileName;
	CCompoundFile  m_oOleFile;
	CHwpFileHeader m_oFileHeader;
	int m_nVersion;
	CHWPDocInfo       m_oDocInfo;
	VECTOR<CHWPSection*> m_arBodyTexts;
	VECTOR<CHWPSection*> m_arViewTexts;
public:
	CHWPFile_Private(const STRING& sFileName);
	~CHWPFile_Private();

	VECTOR<CHWPSection*> GetSections();
	const CCompoundFile* GetOleFile() const;

	bool Detect();
	bool Open();
	void Close();

	void SaveHWPComponent();

	bool GetFileHeader();
	const CHWPDocInfo* GetDocInfo() const;
	bool GetDocInfo(int nVersion);
	bool GetComponent(const STRING& sEntryName, CHWPStream& oBuffer);
	VECTOR<CDirectoryEntry*> GetBinData();
	void SetBinData(const VECTOR<CDirectoryEntry*>& arBinData);
	VECTOR<CHWPPargraph*> GetParas();
	void AddParas(const VECTOR<CHWPPargraph*>& arParas);
private:
	void SaveChildEntries(const STRING& sBasePath, const STRING& sStorageName, ECompressed eCompressed);
	CDirectoryEntry* FindChildEntry(const STRING& sBasePath, const CDirectoryEntry& oBaseEntry, const STRING& sEntryName);
	STRING SaveChildEntry(const STRING& sRootPath, const STRING& sEntryName, ECompressed eCompressed);
	bool GetChildStream(const STRING& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer);

	bool Unzip(CHWPStream& oInput, CHWPStream& oBuffer);
	bool Decrypt(CHWPStream& oInput, CHWPStream& oBuffer);

	bool GetBodyText(int nVersion);
	bool GetViewText(int nVersion);
};
}

#endif // HWPFILE_PRIVATE_H
