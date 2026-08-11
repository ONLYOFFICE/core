#ifndef HWPXCRYPT_H
#define HWPXCRYPT_H

#include <map>

#include "HWPStream.h"
#include "Common/XMLReader.h"

namespace HWP
{
enum class EHwpxCryptResult
{
	OK,
	NOT_ENCRYPTED,
	UNSUPPORTED,
	WRONG_PASSWORD,
	DAMAGED
};

struct THwpxEntryCrypt
{
	VECTOR<HWP_BYTE> m_arIV;
	VECTOR<HWP_BYTE> m_arSalt;
	VECTOR<HWP_BYTE> m_arChecksum;
	unsigned int     m_unIterations;
	unsigned int     m_unKeySize;
	unsigned int     m_unOriginalSize;
	bool             m_bStartKeySha256;
	bool             m_bChecksumSha256;

	THwpxEntryCrypt();
};

class CHwpxCrypt
{
	std::map<HWP_STRING, THwpxEntryCrypt> m_mapEntries;
	HWP_STRING m_sPassword;
public:
	CHwpxCrypt();

	bool ParseManifest(CXMLReader& oReader);
	void SetPassword(const HWP_STRING& sPassword);

	bool Encrypted() const;
	bool Encrypted(const HWP_STRING& sEntryName) const;

	EHwpxCryptResult Decrypt(const HWP_STRING& sEntryName, const HWP_BYTE* pCipher,
	                         unsigned int unCipherSize, CHWPStream& oResult) const;
private:
	bool ParseEncryptionData(CXMLReader& oReader, THwpxEntryCrypt& oEntry);

	static HWP_STRING NormalizeName(const HWP_STRING& sEntryName);
	static std::string GetLocalName(const std::string& sNodeName);
	static bool DecodeBase64(const std::string& sValue, VECTOR<HWP_BYTE>& arResult);
	static bool Inflate(const HWP_BYTE* pData, unsigned int unSize, unsigned int unOriginalSize, CHWPStream& oResult);
	static bool CheckSum(const THwpxEntryCrypt& oEntry, const CHWPStream& oData);
};
}

#endif // HWPXCRYPT_H
