#include "HWPXCrypt.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/common/File.h"

// For decrypt
#include "../../Common/3dParty/cryptopp/aes.h"
#include "../../Common/3dParty/cryptopp/modes.h"
#include "../../Common/3dParty/cryptopp/filters.h"
#include "../../Common/3dParty/cryptopp/secblock.h"
#include "../../Common/3dParty/cryptopp/pwdbased.h"
#include "../../Common/3dParty/cryptopp/sha.h"
// ----------

#define HWPX_CHECKSUM_LENGTH 1024

namespace HWP
{
THwpxEntryCrypt::THwpxEntryCrypt()
	: m_unIterations(0), m_unKeySize(0), m_unOriginalSize(0),
	  m_bStartKeySha256(false), m_bChecksumSha256(false)
{}

CHwpxCrypt::CHwpxCrypt()
{}

void CHwpxCrypt::SetPassword(const HWP_STRING& sPassword)
{
	m_sPassword = sPassword;
}

bool CHwpxCrypt::Encrypted() const
{
	return !m_mapEntries.empty();
}

bool CHwpxCrypt::Encrypted(const HWP_STRING& sEntryName) const
{
	return m_mapEntries.cend() != m_mapEntries.find(NormalizeName(sEntryName));
}

HWP_STRING CHwpxCrypt::NormalizeName(const HWP_STRING& sEntryName)
{
	HWP_STRING sResult{sEntryName};

	std::replace(sResult.begin(), sResult.end(), L'\\', L'/');

	if (!sResult.empty() && L'/' == sResult.front())
		sResult.erase(0, 1);

	return sResult;
}

std::string CHwpxCrypt::GetLocalName(const std::string& sNodeName)
{
	const size_t unPos = sNodeName.find(':');
	return (std::string::npos == unPos) ? sNodeName : sNodeName.substr(unPos + 1);
}

bool CHwpxCrypt::DecodeBase64(const std::string& sValue, VECTOR<HWP_BYTE>& arResult)
{
	arResult.clear();

	if (sValue.empty())
		return false;

	BYTE *pData = nullptr;
	int nLength = 0;

	if (!NSFile::CBase64Converter::Decode(sValue.c_str(), (int)sValue.length(), pData, nLength) || 0 >= nLength)
	{
		RELEASEARRAYOBJECTS(pData);
		return false;
	}

	arResult.assign((HWP_BYTE*)pData, (HWP_BYTE*)pData + nLength);
	RELEASEARRAYOBJECTS(pData);

	return true;
}

bool CHwpxCrypt::ParseManifest(CXMLReader& oReader)
{
	if ("manifest" != GetLocalName(oReader.GetName()))
		return false;

	WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Entry)
	{
		if ("file-entry" != GetLocalName(sNodeEntryName))
			continue;

		const HWP_STRING sFullPath{NormalizeName(oReader.GetAttribute("full-path"))};

		THwpxEntryCrypt oEntry;
		oEntry.m_unOriginalSize = (unsigned int)oReader.GetAttributeInt("size");

		bool bEncrypted = false;

		WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Data)
		{
			if ("encryption-data" != GetLocalName(sNodeDataName))
				continue;

			bEncrypted = ParseEncryptionData(oReader, oEntry);
		}
		END_WHILE

		if (bEncrypted && !sFullPath.empty())
			m_mapEntries.insert(std::make_pair(sFullPath, oEntry));
	}
	END_WHILE

	return true;
}

bool CHwpxCrypt::ParseEncryptionData(CXMLReader& oReader, THwpxEntryCrypt& oEntry)
{
	const std::string sChecksumType{oReader.GetAttributeA("checksum-type")};
	oEntry.m_bChecksumSha256 = std::string::npos != sChecksumType.find("sha256");

	if (!DecodeBase64(oReader.GetAttributeA("checksum"), oEntry.m_arChecksum))
		return false;

	bool bAlgorithm = false, bKeyDerivation = false;

	WHILE_READ_NEXT_NODE_WITH_DEPTH_AND_NAME(oReader, Param)
	{
		const std::string sLocalName{GetLocalName(sNodeParamName)};

		if ("algorithm" == sLocalName)
		{
			const std::string sAlgorithm{oReader.GetAttributeA("algorithm-name")};

			if (std::string::npos == sAlgorithm.find("aes") || std::string::npos == sAlgorithm.find("cbc"))
				return false;

			bAlgorithm = DecodeBase64(oReader.GetAttributeA("initialisation-vector"), oEntry.m_arIV);
		}
		else if ("key-derivation" == sLocalName)
		{
			if (std::string::npos == oReader.GetAttributeA("key-derivation-name").find("pbkdf2"))
				return false;

			oEntry.m_unIterations = (unsigned int)oReader.GetAttributeInt("iteration-count");
			oEntry.m_unKeySize    = (unsigned int)oReader.GetAttributeInt("key-size", 32);

			bKeyDerivation = DecodeBase64(oReader.GetAttributeA("salt"), oEntry.m_arSalt);
		}
		else if ("start-key-generation" == sLocalName)
		{
			oEntry.m_bStartKeySha256 = std::string::npos != oReader.GetAttributeA("start-key-generation-name").find("sha256");
		}
	}
	END_WHILE

	return bAlgorithm && bKeyDerivation;
}

bool CHwpxCrypt::Inflate(const HWP_BYTE* pData, unsigned int unSize, unsigned int unOriginalSize, CHWPStream& oResult)
{
	oResult.Expand(unOriginalSize);

	if (!oResult.IsValid())
		return false;

	CInflate oInflater;

	oInflater.Init2();
	oInflater.SetIn((BYTE*)pData, unSize);
	oInflater.SetOut((BYTE*)oResult.GetCurPtr(), unOriginalSize);

	const int nResult = oInflater.Process(DEFLATE_FINISH);
	const unsigned int unWritten = unOriginalSize - oInflater.GetAvailOut();

	oInflater.End();

	return DEFLATE_STREAM_END == nResult && unOriginalSize == unWritten;
}

bool CHwpxCrypt::CheckSum(const THwpxEntryCrypt& oEntry, const CHWPStream& oData)
{
	using namespace CryptoPP;

	const size_t unLength = (std::min)((size_t)HWPX_CHECKSUM_LENGTH, (size_t)oData.GetSize());

	if (oEntry.m_bChecksumSha256)
	{
		if (SHA256::DIGESTSIZE != (int)oEntry.m_arChecksum.size())
			return false;

		SHA256 oHash;
		return oHash.VerifyDigest((const byte*)oEntry.m_arChecksum.data(), (const byte*)oData.GetCurPtr(), unLength);
	}

	if (SHA1::DIGESTSIZE != (int)oEntry.m_arChecksum.size())
		return false;

	SHA1 oHash;
	return oHash.VerifyDigest((const byte*)oEntry.m_arChecksum.data(), (const byte*)oData.GetCurPtr(), unLength);
}

EHwpxCryptResult CHwpxCrypt::Decrypt(const HWP_STRING& sEntryName, const HWP_BYTE* pCipher,
                                     unsigned int unCipherSize, CHWPStream& oResult) const
{
	std::map<HWP_STRING, THwpxEntryCrypt>::const_iterator itEntry = m_mapEntries.find(NormalizeName(sEntryName));

	if (m_mapEntries.cend() == itEntry)
		return EHwpxCryptResult::NOT_ENCRYPTED;

	const THwpxEntryCrypt& oEntry = itEntry->second;

	if (CryptoPP::AES::BLOCKSIZE != oEntry.m_arIV.size() || oEntry.m_arSalt.empty() || 0 == oEntry.m_unIterations ||
	    (16 != oEntry.m_unKeySize && 24 != oEntry.m_unKeySize && 32 != oEntry.m_unKeySize))
		return EHwpxCryptResult::UNSUPPORTED;

	if (nullptr == pCipher || 0 == unCipherSize || 0 != unCipherSize % CryptoPP::AES::BLOCKSIZE || 0 == oEntry.m_unOriginalSize)
		return EHwpxCryptResult::DAMAGED;

	const std::string sPassword{U_TO_UTF8(m_sPassword)};

	CryptoPP::SecByteBlock oStartKey(oEntry.m_bStartKeySha256 ? (unsigned int)CryptoPP::SHA256::DIGESTSIZE : (unsigned int)CryptoPP::SHA1::DIGESTSIZE);

	if (oEntry.m_bStartKeySha256)
	{
		CryptoPP::SHA256 oHash;
		oHash.CalculateDigest(oStartKey, (const CryptoPP::byte*)sPassword.c_str(), sPassword.length());
	}
	else
	{
		CryptoPP::SHA1 oHash;
		oHash.CalculateDigest(oStartKey, (const CryptoPP::byte*)sPassword.c_str(), sPassword.length());
	}

	CryptoPP::SecByteBlock oKey(oEntry.m_unKeySize);
	CryptoPP::PKCS5_PBKDF2_HMAC<CryptoPP::SHA1> oDeriver;

	oDeriver.DeriveKey(oKey, oKey.size(), 0, oStartKey, oStartKey.size(),
	                   (const CryptoPP::byte*)oEntry.m_arSalt.data(), oEntry.m_arSalt.size(), oEntry.m_unIterations);

	CryptoPP::SecByteBlock oDeflated(unCipherSize);
	CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption oDecryptor;

	try
	{
		oDecryptor.SetKeyWithIV(oKey, oKey.size(), (const CryptoPP::byte*)oEntry.m_arIV.data(), oEntry.m_arIV.size());

		CryptoPP::ArraySource((const CryptoPP::byte*)pCipher, unCipherSize, true,
		                       new CryptoPP::StreamTransformationFilter(oDecryptor, new CryptoPP::ArraySink(oDeflated, oDeflated.size()),
		                                                      CryptoPP::StreamTransformationFilter::NO_PADDING));
	}
	catch (const CryptoPP::Exception&)
	{
		return EHwpxCryptResult::DAMAGED;
	}

	if (!Inflate((const HWP_BYTE*)oDeflated.data(), unCipherSize, oEntry.m_unOriginalSize, oResult))
	{
		oResult.Clear();
		return EHwpxCryptResult::WRONG_PASSWORD;
	}

	if (!CheckSum(oEntry, oResult))
	{
		oResult.Clear();
		return EHwpxCryptResult::WRONG_PASSWORD;
	}

	return EHwpxCryptResult::OK;
}
}
