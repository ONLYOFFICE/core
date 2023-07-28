#include "../include/osign.h"
#include "./utils.h"

#include "../../../../../../Common/3dParty/openssl/common/common_openssl.h"
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>

#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <ctime>

#include <iostream>

namespace OSign
{
	const char* c_record_common_name           = "OSign Certificate";
	const char* c_record_organization_name     = "Ascensio System SIA";
	const char* c_record_country               = "US";

	// common
	void string_replace(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
	{
		size_t posn = 0;
		while (std::string::npos != (posn = text.find(replaceFrom, posn)))
		{
			text.replace(posn, replaceFrom.length(), replaceTo);
			posn += replaceTo.length();
		}
	}

	tm ASN1_GetTimeT(ASN1_TIME* time)
	{
		struct tm t;
		const char* str = (const char*) time->data;
		size_t i = 0;

		memset(&t, 0, sizeof(t));

		if (time->type == V_ASN1_UTCTIME)
		{
			/* two digit year */
			t.tm_year = (str[i++] - '0') * 10;
			t.tm_year += (str[i++] - '0');
			if (t.tm_year < 70)
				t.tm_year += 100;
		}
		else if (time->type == V_ASN1_GENERALIZEDTIME)
		{
			/* four digit year */
			t.tm_year = (str[i++] - '0') * 1000;
			t.tm_year+= (str[i++] - '0') * 100;
			t.tm_year+= (str[i++] - '0') * 10;
			t.tm_year+= (str[i++] - '0');
			t.tm_year -= 1900;
		}
		t.tm_mon  = (str[i++] - '0') * 10;
		t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
		t.tm_mday = (str[i++] - '0') * 10;
		t.tm_mday+= (str[i++] - '0');
		t.tm_hour = (str[i++] - '0') * 10;
		t.tm_hour+= (str[i++] - '0');
		t.tm_min  = (str[i++] - '0') * 10;
		t.tm_min += (str[i++] - '0');
		t.tm_sec  = (str[i++] - '0') * 10;
		t.tm_sec += (str[i++] - '0');

		/* Note: we did not adjust the time based on time zone information */
		return t;
	}

	std::wstring GetPropertyByName(const std::wstring& sSubject, const std::wstring& sName)
	{
		std::wstring::size_type pos = sSubject.find(sName + L"=");
		if (std::wstring::npos != pos)
		{
			std::wstring::size_type name_len = sName.length() + 1;
			std::wstring::size_type posEnd = sSubject.find(L"/", pos);
			if (std::wstring::npos != posEnd)
				return sSubject.substr(pos + name_len, posEnd - pos - name_len);
			return sSubject.substr(pos + name_len);
		}
		return L"";
	}

	class CCertificate_private
	{
	public:
		X509*           m_cert = nullptr;
		EVP_PKEY*       m_key  = nullptr;

		static bool g_is_initialize;

	public:
		CCertificate_private()
		{
			if (!g_is_initialize)
			{
				g_is_initialize = true;
				ERR_load_crypto_strings();
				OpenSSL_add_all_algorithms();
			}
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

		bool Create(const std::map<std::wstring, std::wstring>& props)
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

			m_cert = X509_new();
			X509_set_version(m_cert, 2);

			ASN1_STRING* serialNumber = X509_get_serialNumber(m_cert);
			if (serialNumber)
			{
				const int nBytesCount = 16;
				unsigned char pSerialNumber[nBytesCount];
				RAND_bytes(pSerialNumber, nBytesCount);

				if (1 != ASN1_STRING_set(serialNumber, pSerialNumber, nBytesCount) == 1)
					ASN1_INTEGER_set(serialNumber, 1);
			}

			X509_gmtime_adj(X509_get_notBefore(m_cert), 0);
			X509_gmtime_adj(X509_get_notAfter(m_cert), 31536000L);

			if (X509_set_pubkey(m_cert, m_key) == 0)
			{
				EVP_PKEY_CTX_free(pctx);
				Destroy();
				return false;
			}

			X509_NAME* name_record = X509_get_subject_name(m_cert);

			X509_NAME_add_entry_by_txt(name_record, "C", MBSTRING_ASC, (unsigned char *)c_record_country, -1, -1, 0);
			X509_NAME_add_entry_by_txt(name_record, "O",  MBSTRING_ASC, (unsigned char *)c_record_organization_name, -1, -1, 0);
			X509_NAME_add_entry_by_txt(name_record, "CN", MBSTRING_ASC, (unsigned char *)c_record_common_name, -1, -1, 0);

			X509_set_issuer_name(m_cert, name_record);

			if (!props.empty())
			{
				std::string sAdditions = "";
				for (std::map<std::wstring, std::wstring>::const_iterator iter = props.begin(); iter != props.end(); iter++)
				{
					// delete record for read
					if (iter->first == Properties::DateTo ||
						iter->first == Properties::DateFrom)
					{
						continue;
					}

					std::string sKey = U_TO_UTF8(iter->first);
					std::string sValue = U_TO_UTF8(iter->second);

					string_replace(sKey, ";", "&#59;");
					string_replace(sKey, "=", "&#61;");
					string_replace(sValue, ";", "&#59;");

					sAdditions += (sKey + "=" + sValue + ";");
				}

				if (!sAdditions.empty())
				{
					//const int nid_user = OBJ_create("1.2.3", "key", "value");
					ASN1_OCTET_STRING* oDataAdditions = ASN1_OCTET_STRING_new();
					int nError = ASN1_OCTET_STRING_set(oDataAdditions, (unsigned const char*)sAdditions.c_str(), (int)sAdditions.length());
					OSIGN_UNUSED(nError);
					X509_EXTENSION* ext = X509_EXTENSION_create_by_NID(nullptr, NID_subject_alt_name, false, oDataAdditions);
					nError = X509_add_ext(m_cert, ext, -1);
					OSIGN_UNUSED(nError);
					X509_EXTENSION_free(ext);
				}
			}

			if (X509_sign(m_cert, m_key, EVP_md_null()) == 0)
			{
				LogLastError();
				EVP_PKEY_CTX_free(pctx);
				Destroy();
				return false;
			}

			EVP_PKEY_CTX_free(pctx);
			return true;
		}
	};

	bool CCertificate_private::g_is_initialize = false;

	CCertificate::CCertificate()
	{
		m_internal = new CCertificate_private();
	}
	CCertificate::~CCertificate()
	{
		delete m_internal;
	}

	void CCertificate::Generate(const std::map<std::wstring, std::wstring>& props)
	{
		m_internal->Create(props);
	}

	std::map<std::wstring, std::wstring> CCertificate::GetProperties()
	{
		std::map<std::wstring, std::wstring> props;

		// date
		if (true)
		{
			ASN1_TIME* _time1 = X509_get_notBefore(m_internal->m_cert);
			struct tm t1 = ASN1_GetTimeT(_time1);
			std::string sTimeBefore = std::to_string(t1.tm_mday) + "/" + std::to_string(t1.tm_mon + 1) + "/" + std::to_string(t1.tm_year + 1900);

			ASN1_TIME* _time2 = X509_get_notAfter(m_internal->m_cert);
			struct tm t2 = ASN1_GetTimeT(_time2);
			std::string sTimeAfter = std::to_string(t2.tm_mday) + "/" + std::to_string(t2.tm_mon + 1) + "/" + std::to_string(t2.tm_year + 1900);

			props.insert(std::make_pair(Properties::DateFrom, UTF8_TO_U(sTimeBefore)));
			props.insert(std::make_pair(Properties::DateTo, UTF8_TO_U(sTimeAfter)));
		}

		// names
		if (true)
		{
			X509_NAME* pSubjectName = X509_get_subject_name(m_internal->m_cert);
			char buffer[10000];
			memset(buffer, 0, 10000);

			X509_NAME_oneline(pSubjectName, buffer, 10000);

			std::string sSubjectNameA(buffer);
			std::wstring sSubject = UTF8_TO_U(sSubjectNameA);

			std::wstring sCommonName = GetPropertyByName(sSubject, L"CN");
			std::wstring sOrganizationName = GetPropertyByName(sSubject, L"O");
			std::wstring sCountry = GetPropertyByName(sSubject, L"C");

			if (!sCommonName.empty())       props.insert(std::make_pair(Properties::CommonName,       sCommonName));
			if (!sOrganizationName.empty()) props.insert(std::make_pair(Properties::OrganizationName, sOrganizationName));
			if (!sCountry.empty())          props.insert(std::make_pair(Properties::Country,          sCountry));
		}

		// additions
		if (true)
		{
			int subjectAltNameLoc = X509_get_ext_by_NID(m_internal->m_cert, NID_subject_alt_name, -1);
			if(subjectAltNameLoc != -1)
			{
				X509_EXTENSION* ext = X509_get_ext(m_internal->m_cert, subjectAltNameLoc);
				ASN1_OCTET_STRING* oData = X509_EXTENSION_get_data(ext);

				std::string sDataString = std::string((char*)oData->data, oData->length);
				std::string::size_type posStart = 0;
				std::string::size_type posEnd = sDataString.find(';');

				while (posEnd != std::string::npos)
				{
					std::string::size_type posSplit = sDataString.find('=', posStart);
					if (std::string::npos == posSplit)
						break;

					std::string sKeyA = sDataString.substr(posStart, posSplit - posStart);
					std::string sValueA = sDataString.substr(posSplit + 1, posEnd - posSplit - 1);

					string_replace(sKeyA, "&#59;", ";");
					string_replace(sKeyA, "&#61;", "=");
					string_replace(sValueA, "&#59;", ";");

					std::wstring sKey = UTF8_TO_U(sKeyA);
					std::wstring sValue = UTF8_TO_U(sValueA);

					props.insert(std::make_pair(sKey, sValue));

					posStart = posEnd + 1;
					posEnd = sDataString.find(';', posStart);
				}
			}
		}

		return props;
	}

	bool CCertificate::Load(CStorageBuffer* buffer)
	{
		if (!LoadCert(buffer))
			return false;
		if (!LoadKey(buffer))
			return false;
		return true;
	}
	bool CCertificate::Save(CStorageBuffer* buffer)
	{
		if (!m_internal->m_cert || !m_internal->m_key)
			return false;

		SaveCert(buffer);
		SaveKey(buffer);
		return true;
	}

	bool CCertificate::LoadKey(CStorageBuffer* buffer)
	{
		int nLenKey = buffer->ReadInt();
		unsigned char* pDataKey = buffer->ReadData((size_t)nLenKey);
		BIO* bio = BIO_new_mem_buf((void*)pDataKey, (int)nLenKey);
		if (!PEM_read_bio_PrivateKey(bio, &m_internal->m_key, NULL, NULL))
		{
			BIO_free(bio);
			return false;
		}
		BIO_free(bio);
		return true;
	}
	bool CCertificate::SaveKey(CStorageBuffer* buffer)
	{
		if (!m_internal->m_key)
			return false;

		BIO* bio = BIO_new(BIO_s_mem());
		PEM_write_bio_PrivateKey(bio, m_internal->m_key, NULL, NULL, 0, 0, NULL);
		BIO_flush(bio);

		unsigned char* data = NULL;
		int size = (int)BIO_get_mem_data(bio, &data);

		buffer->AddInt(size);
		buffer->Add(data, size);

		BIO_free(bio);
		return true;
	}

	bool CCertificate::LoadCert(CStorageBuffer* buffer)
	{
		int nLenCert = buffer->ReadInt();
		unsigned char* pDataCert = buffer->ReadData((size_t)nLenCert);

		BIO* bio = BIO_new_mem_buf((void*)pDataCert, (int)nLenCert);
		if (!PEM_read_bio_X509(bio, &m_internal->m_cert, NULL, NULL))
		{
			LogLastError();
			BIO_free(bio);
			return false;
		}
		BIO_free(bio);
		return true;
	}
	bool CCertificate::SaveCert(CStorageBuffer* buffer)
	{
		if (!m_internal->m_cert)
			return false;

		BIO* bio = BIO_new(BIO_s_mem());
		PEM_write_bio_X509(bio, m_internal->m_cert);
		BIO_flush(bio);

		unsigned char* data = NULL;
		int size = (int)BIO_get_mem_data(bio, &data);

		buffer->AddInt(size);
		buffer->Add(data, size);

		BIO_free(bio);
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
		if (!m_internal->m_cert)
			return false;

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
			buffer.AddSkip(size);
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

namespace OSign
{
	std::string GetLastError()
	{
		BIO* bio = BIO_new(BIO_s_mem());
		ERR_print_errors(bio);
		char *buf = NULL;
		size_t len = BIO_get_mem_data(bio, &buf);
		std::string sRet((char*)buf, len);
		BIO_free(bio);
		return sRet;
	}

	void LogLastError()
	{
		std::cout << OSign::GetLastError() << std::endl;
	}
}
