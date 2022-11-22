#include "../../../../common/File.h"

#include "../../include/Certificate.h"
#include "../../include/CertificateCommon.h"
#include "../../../../../3dParty/openssl/common/common_openssl.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT void* Crypto_Malloc(unsigned int size)
{
	return ::malloc(size);
}
WASM_EXPORT void Crypto_Free(void* p)
{
	if (p) ::free(p);
}

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

	size_t public_len = publicKey.length();
	size_t private_len = privateKeyEnc.length();
	char* result = (char*)Crypto_Malloc(public_len + private_len + 2);
	memcpy(result, publicKey.c_str(), public_len);
	result[public_len] = '\0';
	memcpy(result + public_len, privateKeyEnc.c_str(), private_len);
	result[public_len + private_len + 1] = '\0';

	return result;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
#include <iostream>
int main()
{
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

	return 0;
}
#endif
