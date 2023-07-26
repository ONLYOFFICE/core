#include "../../../../../common/File.h"
#include "../../../../../../Common/3dParty/openssl/common/common_openssl.h"

#ifndef DISABLE_XMLSEC
#include "../../../include/Certificate.h"
#include "../../../include/CertificateCommon.h"
#endif

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

namespace NSInternal
{
	int CheckEncryptedVersion(const std::string& input, int& offset)
	{
		offset = 0;
		// VER%NUMBER%; version from 100 to 999
		if (input.length() > 7)
		{
			const char* input_ptr = input.c_str();
			if (input_ptr[0] == 'V' && input_ptr[1] == 'E' && input_ptr[2] == 'R')
			{
				input_ptr += 3;
				int nVersion = 0;
				for (int i = 0; i < 4; ++i)
				{
					if (*input_ptr == ';')
					{
						offset = 3 + i + 1;
						return nVersion;
					}

					nVersion *= 10;
					nVersion += (*input_ptr - '0');
					++input_ptr;
				}
				return nVersion;
			}
		}
		return 1;
	}

	std::string DecodePrivateKey(const std::string& sPrivateKeyEnc, const std::string& sPassword, const std::string& sSalt)
	{
		int nHeaderLen = 0;
		int nVersion = CheckEncryptedVersion(sPrivateKeyEnc, nHeaderLen);

		std::string sPrivateKey;
		bool bIsSuccess = false;
		if (nVersion == 2)
			bIsSuccess = NSOpenSSL::AES_Decrypt_desktop_GCM(sPassword, sPrivateKeyEnc, sPrivateKey, sSalt, nHeaderLen);
		else
			bIsSuccess = NSOpenSSL::AES_Decrypt_desktop(sPassword, sPrivateKeyEnc, sPrivateKey, sSalt);

		if (!bIsSuccess)
			return "";

		return sPrivateKey;
	}
}

#ifdef __cplusplus
extern "C" {
#endif

// Memory
WASM_EXPORT void* Crypto_Malloc(unsigned int size)
{
	return ::malloc(size);
}
WASM_EXPORT void Crypto_Free(void* p)
{
	if (p) ::free(p);
}

#ifndef DISABLE_XMLSEC

class CMemoryFile
{
public:
	unsigned int Length;
	BYTE* Data;

public:
	CMemoryFile(BYTE* data = NULL, unsigned int len = 0) { Length = len; Data = data; }
	~CMemoryFile() { if (Data) delete [] Data; }
};

class CCertificate
{
public:
	ICertificate* m_pCert;

private:
	unsigned char* m_key;
	unsigned int m_keyLen;
	std::string m_keyPass;

	unsigned char* m_cert;
	unsigned int m_certLen;
	std::string m_certPass;

public:
	CCertificate()
	{
		m_pCert = NULL;
	}
	~CCertificate()
	{
		delete m_pCert;
	}

public:
	int LoadCert(unsigned char* data, unsigned int len, char* pass)
	{
		m_cert = NULL; m_certLen = 0; m_certPass = "";
		int nRes = NSOpenSSL::LoadCertRaw(data, len, (NULL != pass) ? std::string(pass) : "");

		if (OPEN_SSL_WARNING_ALL_OK == nRes || OPEN_SSL_WARNING_OK == nRes)
		{
			m_cert = data;
			m_certLen = len;
			m_certPass = (NULL != pass) ? std::string(pass) : "";
		}
		if (OPEN_SSL_WARNING_ALL_OK == nRes)
		{
			m_key = m_cert;
			m_keyLen = m_certLen;
			m_keyPass = m_certPass;
		}

		return nRes;
	}
	int LoadKey(unsigned char* data, unsigned int len, char* pass)
	{
		m_key = NULL; m_keyLen = 0; m_keyPass = "";
		int nRes = NSOpenSSL::LoadKeyRaw(data, len, (NULL != pass) ? std::string(pass) : "");

		if (OPEN_SSL_WARNING_ALL_OK == nRes || OPEN_SSL_WARNING_OK == nRes)
		{
			m_key = data;
			m_keyLen = len;
			m_keyPass = (NULL != pass) ? std::string(pass) : "";
		}
		if (OPEN_SSL_WARNING_ALL_OK == nRes)
		{
			m_cert = m_key;
			m_certLen = m_keyLen;
			m_certPass = m_keyPass;
		}

		return nRes;
	}

	void Load()
	{
		m_pCert = NSCertificate::FromFilesRaw(m_key, m_keyLen, m_keyPass, m_cert, m_certLen, m_certPass);
	}

	ICertificate* GetCertificate()
	{
		return m_pCert;
	}
};

// X509/RSA
WASM_EXPORT void* Crypto_CreateCertificate()
{
	return new CCertificate();
}
WASM_EXPORT int Crypto_LoadCert(void* cert, unsigned char* data, unsigned int len, char* pass)
{
	CCertificate* pCert = (CCertificate*)cert;
	return pCert->LoadCert(data, len, pass);
}
WASM_EXPORT int Crypto_LoadKey(void* cert, unsigned char* data, unsigned int len, char* pass)
{
	CCertificate* pCert = (CCertificate*)cert;
	return pCert->LoadKey(data, len, pass);
}
WASM_EXPORT void Crypto_DestroyCertificate(void* cert)
{
	CCertificate* pCert = (CCertificate*)cert;
	delete pCert;
}

#endif

// methods for oform signatures
// создаем ключи, приватный шифруем паролем, результат - две base64 строки
WASM_EXPORT char* Crypto_CreateKeys(const char* alg, const char* password, const char* salt)
{
	std::string publicKey;
	std::string privateKey;

	if (!NSOpenSSL::GenerateKeysByAlgs(alg, publicKey, privateKey))
		return NULL;

	std::string privateKeyEnc;
	NSOpenSSL::AES_Encrypt_desktop_GCM(password, privateKey, privateKeyEnc, salt);

	char* pDataPublicBase64 = NULL;
	int nDataPublicBase64Len = 0;
	NSFile::CBase64Converter::Encode((BYTE*)publicKey.c_str(), (int)publicKey.length(), pDataPublicBase64, nDataPublicBase64Len, NSBase64::B64_BASE64_FLAG_NOCRLF);
	publicKey = std::string(pDataPublicBase64, (size_t)nDataPublicBase64Len);
	RELEASEARRAYOBJECTS(pDataPublicBase64);

	size_t public_len = publicKey.length();
	size_t private_len = privateKeyEnc.length();
	char* result = (char*)Crypto_Malloc((unsigned int)(public_len + private_len + 2));
	memcpy(result, publicKey.c_str(), public_len);
	result[public_len] = '\0';
	memcpy(result + public_len + 1, privateKeyEnc.c_str(), private_len);
	result[public_len + private_len + 1] = '\0';

	return result;
}

WASM_EXPORT char* Crypto_Sign(const char* privateKeyEnc, const char* password, const char* salt, const char* data, int dataLen)
{
	std::string sPrivateKey = NSInternal::DecodePrivateKey(std::string(privateKeyEnc), std::string(password), std::string(salt));
	if (sPrivateKey.empty())
		return NULL;

	if (0 == dataLen)
		dataLen = (int)strlen(data);

	NSOpenSSL::CMemoryData dataSign = NSOpenSSL::Sign((const unsigned char*)data, dataLen, sPrivateKey);

	int nDataSignBase64Len = NSBase64::Base64EncodeGetRequiredLength((int)dataSign.Size, NSBase64::B64_BASE64_FLAG_NOCRLF);
	char* pDataSignBase64 = (char*)Crypto_Malloc(nDataSignBase64Len + 1);
	memset(pDataSignBase64, 0, nDataSignBase64Len + 1);

	if (FALSE == NSBase64::Base64Encode(dataSign.Data, (int)dataSign.Size, (BYTE*)pDataSignBase64, &nDataSignBase64Len, NSBase64::B64_BASE64_FLAG_NOCRLF))
	{
		dataSign.Free();
		Crypto_Free((void*)pDataSignBase64);
		return NULL;
	}

	dataSign.Free();
	return pDataSignBase64;
}

WASM_EXPORT char* Crypto_ChangePassword(const char* privateKeyEnc, const char* passwordOld, const char* passwordNew, const char* salt)
{
	std::string sPrivateKey = NSInternal::DecodePrivateKey(std::string(privateKeyEnc), std::string(passwordOld), std::string(salt));
	if (sPrivateKey.empty())
		return NULL;

	std::string sPrivateKeyEnc = "";
	NSOpenSSL::AES_Encrypt_desktop_GCM(std::string(passwordNew), sPrivateKey, sPrivateKeyEnc, std::string(salt));

	size_t nEncLen = sPrivateKeyEnc.length();
	char* result = (char*)Crypto_Malloc((unsigned int)(nEncLen + 1));
	memcpy(result, sPrivateKeyEnc.c_str(), nEncLen);
	result[nEncLen] = '\0';

	return result;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
#include <iostream>
int main()
{
	// oform module --------------------------------

	if (true)
	{
		std::string sSalt = "123546789";
		std::string sPassword = "qwerty";
		std::string sPasswordNew = "qwerty2";

		char* pGeneratedKeys = Crypto_CreateKeys("ed25519", sPassword.c_str(), sSalt.c_str());
		if (NULL == pGeneratedKeys)
			return 1;

		size_t nLen = 0;
		while (pGeneratedKeys[nLen] != 0)
			nLen++;

		std::string sPublicKey = std::string(pGeneratedKeys, nLen);

		size_t nLen2 = nLen + 1;
		while (pGeneratedKeys[nLen2] != 0)
			nLen2++;

		std::string sPrivateKeyEnc = std::string(pGeneratedKeys + nLen + 1, nLen2 - nLen - 1);

		Crypto_Free(pGeneratedKeys);

		std::string sSignData = "hello world!";

		char* pSignData = Crypto_Sign(sPrivateKeyEnc.c_str(), sPassword.c_str(), sSalt.c_str(), sSignData.c_str(), (int)sSignData.length());
		std::string sSignature(pSignData);

		Crypto_Free(pSignData);

		char* pNewPrivateKey = Crypto_ChangePassword(sPrivateKeyEnc.c_str(), sPassword.c_str(), sPasswordNew.c_str(), sSalt.c_str());

		std::string sPrivateKeyEncNew(pNewPrivateKey);

		Crypto_Free(pNewPrivateKey);

		return 0;
	}

	// ---------------------------------------------


	std::string publicKey;
	std::string privateKey;

	NSOpenSSL::GenerateKeysByAlgs("ed25519", publicKey, privateKey);

	if (true)
	{
		std::string sTestData = "hello";
		std::string sTestData2 = "Hello";
		NSOpenSSL::CMemoryData data = NSOpenSSL::Sign((const unsigned char*)sTestData.c_str(), (int)sTestData.length(), privateKey);

		bool isVerify = NSOpenSSL::Verify((const unsigned char*)sTestData.c_str(), (int)sTestData.length(), publicKey, data.Data, (int)data.Size);
		bool isVerify2 = NSOpenSSL::Verify((const unsigned char*)sTestData2.c_str(), (int)sTestData2.length(), publicKey, data.Data, (int)data.Size);

		data.Free();
	}

	publicKey = "";
	privateKey = "";
	NSOpenSSL::GenerateKeysByAlgs("x25519", publicKey, privateKey);
	if (true)
	{
		std::string sTestData = "hello";
		NSOpenSSL::CMemoryData data = NSOpenSSL::Enrypt((const unsigned char*)sTestData.c_str(), (int)sTestData.length(), publicKey);

		if (data.Data)
		{
			NSOpenSSL::CMemoryData data2 = NSOpenSSL::Decrypt(data.Data, (int)data.Size, privateKey);
			if (data2.Data)
			{
				std::string sTestDataDecrypt = data2.Serialize(OPENSSL_SERIALIZE_TYPE_ASCII);
				std::cout << sTestDataDecrypt << std::endl;
			}

			data.Free();
		}

		data.Free();
	}

#ifndef DISABLE_XMLSEC
	std::wstring sTestDir = NSFile::GetProcessDirectory() + L"/../../../../test/";
	void* cert = Crypto_CreateCertificate();

	BYTE* pCertData = NULL;
	DWORD nCertLen  = 0;
	NSFile::CFileBinary::ReadAllBytes(sTestDir + L"keys/cert.crt", &pCertData, nCertLen);
	Crypto_LoadCert(cert, pCertData, nCertLen, NULL);

	BYTE* pKeyData = NULL;
	DWORD nKeyLen  = 0;
	NSFile::CFileBinary::ReadAllBytes(sTestDir + L"keys/key.key", &pKeyData, nKeyLen);
	Crypto_LoadKey(cert, pKeyData, nKeyLen, NULL);

	Crypto_DestroyCertificate(cert);

	RELEASEARRAYOBJECTS(pCertData);
	RELEASEARRAYOBJECTS(pKeyData);
#endif

	return 0;
}
#endif
