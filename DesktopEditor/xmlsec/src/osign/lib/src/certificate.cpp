#include "../include/osign.h"
#include "./utils.h"

#include "../../../../../../Common/3dParty/openssl/common/common_openssl.h"
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <ctime>

namespace OSign
{
	class CCertificate_private
	{
	public:
		X509*           m_cert = nullptr;
		EVP_PKEY*       m_key  = nullptr;

	public:
		CCertificate_private()
		{
		}

		~CCertificate_private()
		{
			Destroy();
		}

		void Destroy()
		{
			if (nullptr != m_cert)
				X509_free(m_cert);
			m_cert = nullptr;

			if (NULL != m_key)
				EVP_PKEY_free(m_key);
			m_key = nullptr;
		}

		bool Create()
		{
			EVP_PKEY_CTX* pctx = nullptr;

			const std::string& alg = "ed25519";
			if (alg == "ed25519")
				pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
			else if (alg == "x25519")
				pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, NULL);
			else
				return false;

			EVP_PKEY_keygen_init(pctx);
			EVP_PKEY_keygen(pctx, &m_key);
			EVP_PKEY_CTX_free(pctx);

			m_cert = X509_new();
			ASN1_INTEGER_set(X509_get_serialNumber(m_cert), 420);
			X509_time_adj_ex(X509_getm_notBefore(m_cert), 0, 0, 0);

			time_t exp;
			time(&exp);
			exp += 315360000;
			X509_time_adj_ex(X509_getm_notAfter(m_cert), 0, 0, &exp);

			if (X509_set_pubkey(m_cert, m_key) == 0)
			{
				EVP_PKEY_CTX_free(pctx);
				Destroy();
				return false;
			}

			if (false)
			{
				if (X509_sign(m_cert, m_key, EVP_sha512()) == 0)
				{
					EVP_PKEY_CTX_free(pctx);
					Destroy();
					return false;
				}
			}

			EVP_PKEY_CTX_free(pctx);
			return true;
		}
	};

	CCertificate::CCertificate()
	{
		m_internal = new CCertificate_private();
	}
	CCertificate::CCertificate(const std::map<std::wstring, std::wstring>& props)
	{
		m_internal = new CCertificate_private();
	}
	CCertificate::~CCertificate()
	{
		delete m_internal;
	}

	void CCertificate::Generate()
	{
		m_internal->Create();
	}

	std::map<std::wstring, std::wstring> CCertificate::GetProperties()
	{
		std::map<std::wstring, std::wstring> props;
		return props;
	}

	bool CCertificate::Load(CStorageBuffer* buffer)
	{
		int nLenCert = buffer->ReadInt();
		unsigned char* pDataCert = buffer->ReadData((size_t)nLenCert);
		int nLenKey = buffer->ReadInt();
		unsigned char* pDataKey = buffer->ReadData((size_t)nLenKey);

		BIO* bio = BIO_new_mem_buf((void*)pDataCert, (int)nLenCert);
		if (!PEM_read_bio_X509(bio, &m_internal->m_cert, NULL, NULL))
		{
			BIO_free(bio);
			return false;
		}
		BIO_free(bio);

		bio = BIO_new_mem_buf((void*)pDataKey, (int)nLenKey);
		if (!PEM_read_bio_PrivateKey(bio, &m_internal->m_key, NULL, NULL))
		{
			BIO_free(bio);
			return false;
		}
		BIO_free(bio);

		return true;
	}
	bool CCertificate::Save(CStorageBuffer* buffer)
	{
		if (!m_internal->m_cert || !m_internal->m_key)
			return false;

		if (true)
		{
			BIO* bio = BIO_new(BIO_s_mem());
			PEM_write_bio_X509(bio, m_internal->m_cert);
			BIO_flush(bio);

			unsigned char* data = NULL;
			int size = (int)BIO_get_mem_data(bio, &data);

			buffer->AddInt(size);
			buffer->Add(data, size);

			BIO_free(bio);
		}

		if (true)
		{
			BIO* bio = BIO_new(BIO_s_mem());
			PEM_write_bio_PrivateKey(bio, m_internal->m_key, NULL, NULL, 0, 0, NULL);
			BIO_flush(bio);

			unsigned char* data = NULL;
			int size = (int)BIO_get_mem_data(bio, &data);

			buffer->AddInt(size);
			buffer->Add(data, size);

			BIO_free(bio);
		}

		return true;
	}

	CStorageBuffer CCertificate::Sign(const CStorageBuffer& data)
	{
		CStorageBuffer buffer;
		if (!m_internal->m_cert || !m_internal->m_key)
			return buffer;

		//https://www.openssl.org/docs/manmaster/man7/Ed25519.html
		EVP_MD_CTX* pCtx = EVP_MD_CTX_new();
		EVP_DigestSignInit(pCtx, NULL, NULL, NULL, m_internal->m_key);

		size_t nSize = data.GetLength();
		/* Calculate the requires size for the signature by passing a NULL buffer */
		size_t nSignatureLen = 0;
		unsigned char* pData = data.GetData();
		EVP_DigestSign(pCtx, NULL, &nSignatureLen, pData, (size_t)nSize);
		buffer.CheckAlloc(nSignatureLen);
		EVP_DigestSign(pCtx, buffer.GetData(), &nSignatureLen,  pData, (size_t)nSize);
		EVP_MD_CTX_free(pCtx);

		buffer.AddSkip(nSignatureLen);
		return buffer;
	}
	bool CCertificate::Verify(const CStorageBuffer& data, const CStorageBuffer& signature)
	{
		EVP_PKEY* pubkey = X509_get_pubkey(m_internal->m_cert);
		EVP_MD_CTX* pCtx = EVP_MD_CTX_new();
		EVP_DigestVerifyInit(pCtx, NULL, NULL, NULL, pubkey);

		/* Calculate the requires size for the signature by passing a NULL buffer */
		int nResult = EVP_DigestVerify(pCtx, signature.GetData(), signature.GetLength(), data.GetData(), data.GetLength());

		EVP_MD_CTX_free(pCtx);
		EVP_PKEY_free(pubkey);

		return (1 == nResult) ? true : false;
	}
}

namespace OSign
{
	namespace Crypt
	{
		CStorageBuffer GeneratePassword(const size_t& size)
		{
			CStorageBuffer buffer;
			buffer.Alloc(size);
			RAND_priv_bytes(buffer.GetData(), (int)size);
			buffer.Skip(size);
			return buffer;
		}

		// version;salt;encoded_buffer
		CStorageBuffer Encrypt(CStorageBuffer& buffer, CStorageBuffer& password)
		{
			const int salt_len = 32;
			unsigned char salt[salt_len];
			RAND_bytes(salt, salt_len);

			unsigned char* key = NSOpenSSL::PBKDF2((const char*)password.GetData(), (int)password.GetLength(), salt, salt_len, OPENSSL_HASH_ALG_SHA256, 32);

			NSOpenSSL::CMemoryData oBufferData;
			AES_Encrypt_desktop_GCM(key, buffer.GetData(), (int)buffer.GetLength(), oBufferData);

			CStorageBuffer result;
			result.CheckAlloc(4 + 4 + salt_len + 4 + oBufferData.Size);
			result.AddInt(OSIGN_SHARE_VERSION);
			result.AddInt(salt_len);
			result.Add(salt, salt_len);
			result.AddInt(oBufferData.Size);
			result.Add(oBufferData.Data, oBufferData.Size);
			result.Seek(0);

			NSOpenSSL::openssl_free(key);

			return result;
		}
		CStorageBuffer Decrypt(CStorageBuffer& buffer, CStorageBuffer& password)
		{
			int nVersion = buffer.ReadInt();
			OSIGN_UNUSED(nVersion);

			int salt_len = buffer.ReadInt();
			unsigned char* salt = buffer.ReadData(salt_len);
			int data_len = buffer.ReadInt();

			unsigned char* key = NSOpenSSL::PBKDF2((const char*)password.GetData(), (int)password.GetLength(), salt, salt_len, OPENSSL_HASH_ALG_SHA256, 32);

			NSOpenSSL::CMemoryData oBufferData;
			NSOpenSSL::AES_Decrypt_desktop_GCM(key, buffer.ReadData(data_len), data_len, oBufferData);

			CStorageBuffer result;
			result.Alloc(oBufferData.Size);
			result.Add(oBufferData.Data, oBufferData.Size);

			NSOpenSSL::openssl_free(key);

			return result;
		}
	}
}
