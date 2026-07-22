#ifndef HWPFILE_H
#define HWPFILE_H

#include "HwpFileHeader.h"
#include "OLEdoc/CompoundFile.h"
#include "HWPElements/HWPRecordBinData.h"
#include "HWPSection.h"
#include "HWPXCrypt.h"

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

	HWP_STRING       m_sPassword;
	EHwpxCryptResult m_eCryptResult;
	bool             m_bPasswordDecrypt;
	bool             m_bAesEncrypt;
	HWP_BYTE         m_aDesKey[8];
	HWP_BYTE         m_aAesKey[16];
public:
	CHWPFile(const HWP_STRING& sFileName);
	~CHWPFile();

	VECTOR<const CHWPSection*> GetSections();
	const CCompoundFile* GetOleFile() const;

	void SetPassword(const HWP_STRING& sPassword);
	bool Encrypted() const;
	EHwpxCryptResult GetCryptResult() const;

	bool Detect();
	bool Open();
	void Close();

	bool GetFileHeader();
	const CHWPDocInfo* GetDocInfo() const;
	bool GetDocInfo(int nVersion);
	bool GetComponent(const HWP_STRING& sEntryName, CHWPStream& oBuffer);
	bool GetChildStream(const HWP_STRING& sEntryName, ECompressed eCompressed, CHWPStream& oBuffer);
private:
	CDirectoryEntry* FindChildEntry(const HWP_STRING& sBasePath, const CDirectoryEntry& oBaseEntry, const HWP_STRING& sEntryName) const;
	HWP_STRING SaveChildEntry(const HWP_STRING& sRootPath, const HWP_STRING& sEntryName, ECompressed eCompressed);

	bool Unzip(CHWPStream& oInput, CHWPStream& oBuffer);
	bool Decrypt(CHWPStream& oInput, CHWPStream& oBuffer);

	static bool DerivePasswordKey(const HWP_STRING& sPassword, HWP_BYTE* pKey);
	static bool DeriveAesKey(const HWP_STRING& sPassword, HWP_BYTE* pKey);
	bool DecryptPassword(CHWPStream& oInput, CHWPStream& oBuffer);
	bool DecryptPasswordDes(CHWPStream& oInput, CHWPStream& oBuffer);
	bool DecryptPasswordAes(CHWPStream& oInput, CHWPStream& oBuffer);
	bool VerifyPassword();
	bool DecodeStream(CHWPStream& oRaw, CHWPStream& oOut);

	bool GetBodyText(int nVersion);
	bool GetViewText(int nVersion);
};
}

#endif // HWPFILE_H
