#ifndef _XMLSIGNER_OFORM_H_
#define _XMLSIGNER_OFORM_H_

#include "./../include/Certificate.h"
#include "../../../common/File.h"

#include "openssl/evp.h"
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "../../../../Common/3dParty/openssl/common/common_openssl.h"

class CCertificate_oform : public ICertificate
{
private:
	std::string m_id;
	std::string m_pem_key;

	int m_alg;

public:
	CCertificate_oform() : ICertificate()
	{
		m_alg = OOXML_HASH_ALG_SHA3_512;
	}
	virtual ~CCertificate_oform()
	{
	}

	virtual int GetType()
	{
		return CERTIFICATE_ENGINE_TYPE_OFORM;
	}

public:
	virtual std::string GetNumber()
	{
		return "";
	}

	virtual std::wstring GetSignerName()
	{
		return L"";
	}

	virtual std::wstring GetIssuerName()
	{
		return GetSignerName();
	}

	virtual std::string GetCertificateBase64()
	{
		return "";
	}

	virtual std::string GetCertificateHash()
	{
		return "";
	}

	virtual std::string GetDate()
	{
		return "";
	}

	virtual std::string GetId()
	{
		// TODO: + public key?
		if (!m_id.empty())
			return m_id;
		return GetNumber();
	}

public:
	std::string Sign(unsigned char* pData, unsigned int nSize)
	{
		NSOpenSSL::CMemoryData data = NSOpenSSL::Sign(pData, (int)nSize, m_pem_key);
		if (!data.Data)
			return "";

		char* pBase64 = NULL;
		int nBase64Len = 0;
		NSFile::CBase64Converter::Encode(data.Data, (int)data.Size, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NONE);

		data.Free();

		std::string sReturn(pBase64, nBase64Len);
		delete[] pBase64;

		return sReturn;
	}

	virtual std::string Sign(const std::string& sXml)
	{
		return Sign((BYTE*)sXml.c_str(), (unsigned int)sXml.length());
	}

	virtual bool SignPKCS7(unsigned char* pData, unsigned int nSize,
						   unsigned char*& pDataDst, unsigned int& nSizeDst)
	{
		return false;
	}

	virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)
	{
		unsigned int nBufLen = 64;
		unsigned char* pBufData = (unsigned char*)malloc(nBufLen);
		const EVP_MD* algorithm = EVP_sha3_512();
		EVP_MD_CTX* context = EVP_MD_CTX_new();
		EVP_DigestInit_ex(context, algorithm, nullptr);
		EVP_DigestUpdate(context, pData, (size_t)nSize);
		EVP_DigestFinal_ex(context, pBufData, &nBufLen);
		EVP_MD_CTX_destroy(context);

		if (0 == nBufLen)
			return "";

		char* pBase64_hash = NULL;
		int nBase64Len_hash = 0;
		NSFile::CBase64Converter::Encode(pBufData, nBufLen, pBase64_hash, nBase64Len_hash, NSBase64::B64_BASE64_FLAG_NOCRLF);

		std::string sReturn(pBase64_hash, nBase64Len_hash);
		delete [] pBase64_hash;

		delete [] pBufData;

		return sReturn;
	}

	virtual std::string GetHash(const std::string& sXml, int nAlg)
	{
		return GetHash((BYTE*)sXml.c_str(), (DWORD)sXml.length(), nAlg);
	}

	virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg)
	{
		BYTE* pFileData = NULL;
		DWORD dwFileDataLen = 0;
		NSFile::CFileBinary::ReadAllBytes(sXmlFile, &pFileData, dwFileDataLen);

		if (0 == dwFileDataLen)
			return "";

		std::string sReturn = GetHash(pFileData, dwFileDataLen, nAlg);

		RELEASEARRAYOBJECTS(pFileData);
		return sReturn;
	}

	virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)
	{
		BYTE* pDigestValue = NULL;
		int nDigestLen = 0;
		NSFile::CBase64Converter::Decode(sXmlSignature.c_str(), (int)sXmlSignature.length(), pDigestValue, nDigestLen);

		if (!pDigestValue)
			return false;

		bool bIsGood = NSOpenSSL::Verify((const unsigned char*)sXml.c_str(), (int)sXml.length(), m_pem_key, pDigestValue, nDigestLen);

		RELEASEARRAYOBJECTS(pDigestValue);

		return bIsGood;
	}

	virtual bool LoadFromBase64Data(const std::string& data)
	{
		BYTE* pData = NULL;
		int nLen = 0;
		if (NSFile::CBase64Converter::Decode(data.c_str(), (int)data.length(), pData, nLen))
		{
			m_pem_key = std::string((char*)pData, nLen);
		}

		return false;
	}

	virtual std::vector<int> GetHashAlgs()
	{
		std::vector<int> algs;
		algs.push_back(OOXML_HASH_ALG_SHA3_512);
		m_alg = OOXML_HASH_ALG_SHA3_512;
		return algs;
	}
	virtual int GetHashAlg()
	{
		if (m_alg == OOXML_HASH_ALG_INVALID)
			GetHashAlgs();
		return m_alg;
	}

	virtual int VerifySelf()
	{
		return OPEN_SSL_WARNING_OK;
	}

public:
	virtual int ShowSelectDialog(void* parent)
	{
		return -1;
	}

	virtual int ShowCertificate(void* parent)
	{
		return -1;
	}

	virtual std::string Print()
	{
		return "";
	}

	virtual bool IsGOST()
	{
		return false;
	}
};

#endif // _XMLSIGNER_OFORM_H_
