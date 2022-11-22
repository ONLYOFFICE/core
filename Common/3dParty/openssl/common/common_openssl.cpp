/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "./common_openssl.h"
#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/bio.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include "../../../../DesktopEditor/common/File.h"

namespace NSOpenSSL
{
	CMemoryData::CMemoryData()
	{
		Data = NULL;
		Size = 0;
	}
	CMemoryData::~CMemoryData()
	{
	}

	std::string CMemoryData::Serialize(const int& alg)
	{
		return NSOpenSSL::Serialize(Data, (unsigned int)Size, alg);
	}
	void CMemoryData::Alloc(const size_t& size)
	{
		Data = openssl_alloc((unsigned int)size);
		Size = size;
	}
	void CMemoryData::Free()
	{
		if (Data)
		{
			openssl_free(Data);
			Size = 0;
		}
	}
}

namespace NSOpenSSL
{
	unsigned char* openssl_alloc(unsigned int len)
	{
		return (unsigned char*)malloc(len);
	}
	void openssl_free(unsigned char* data)
	{
		free(data);
	}

	// hash
	unsigned int GetHashSize(const int& alg)
	{
		switch (alg)
		{
		case OPENSSL_HASH_ALG_SHA1:
			return 20;
		case OPENSSL_HASH_ALG_SHA224:
			return 28;
		case OPENSSL_HASH_ALG_SHA256:
			return 32;
		case OPENSSL_HASH_ALG_SHA384:
			return 48;
		case OPENSSL_HASH_ALG_SHA512:
			return 64;
		default:
			break;
		}
		return 0;
	}

	unsigned char* GetHash(const unsigned char* data, const unsigned int& size, const int& alg, unsigned int& len)
	{
		len = GetHashSize(alg);
		if (0 == len)
			return NULL;
		unsigned char* res = openssl_alloc(len);
		switch (alg)
		{
		case OPENSSL_HASH_ALG_SHA1:
			SHA1(data, (size_t)size, res);
			break;
		case OPENSSL_HASH_ALG_SHA224:
			SHA224(data, (size_t)size, res);
			break;
		case OPENSSL_HASH_ALG_SHA256:
			SHA256(data, (size_t)size, res);
			break;
		case OPENSSL_HASH_ALG_SHA384:
			SHA384(data, (size_t)size, res);
			break;
		case OPENSSL_HASH_ALG_SHA512:
			SHA512(data, (size_t)size, res);
			break;
		default:
			break;
		}
		return res;
	}

	// rsa
	bool RSA_GenerateKeys(unsigned char*& publicKey, unsigned char*& privateKey)
	{
		publicKey = NULL;
		privateKey = NULL;

		RSA* rsa = RSA_new();
		BIGNUM *exponent = BN_new();

		BN_set_word(exponent, RSA_F4);
		int result = RSA_generate_multi_prime_key(rsa, 2048, 2, exponent, NULL);
		if (0 == result)
			return false;

		if (true)
		{
			BIO* bio = BIO_new(BIO_s_mem());
			if (PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL))
			{
				int key_length = BIO_pending(bio);
				privateKey = openssl_alloc(key_length + 1);
				if (key_length != BIO_read(bio, privateKey, key_length))
				{
					openssl_free(privateKey);
					privateKey = NULL;
				}
				else
				{
					privateKey[key_length] = '\0';
				}
			}
			BIO_free_all(bio);
		}
		if (true)
		{
			BIO* bio = BIO_new(BIO_s_mem());
			if (PEM_write_bio_RSA_PUBKEY(bio, rsa))
			{
				int key_length = BIO_pending(bio);
				publicKey = openssl_alloc(key_length + 1);
				if (key_length != BIO_read(bio, publicKey, key_length))
				{
					openssl_free(publicKey);
					publicKey = NULL;
				}
				else
				{
					publicKey[key_length] = '\0';
				}
			}
			BIO_free_all(bio);
		}

		BN_free(exponent);
		RSA_free(rsa);

		return (NULL != publicKey && NULL != privateKey) ? true : false;
	}

	//#define USE_DEPRECATED
	bool RSA_EncryptPublic(const unsigned char* publicKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len)
	{
		BIO* bio = BIO_new_mem_buf(publicKey, (int)strlen((char*)publicKey));

#ifdef USE_DEPRECATED
		RSA* rsa = RSA_new();
		RSA* resrsa = PEM_read_bio_RSA_PUBKEY(bio, &rsa, 0, NULL);

		unsigned int key_size = (unsigned int)RSA_size(rsa);
		data_crypt = openssl_alloc(key_size);

		int res = RSA_public_encrypt((int)size, data, data_crypt, rsa, RSA_NO_PADDING);
		data_crypt_len = key_size;

		BIO_free(bio);
		RSA_free(rsa);
		return (-1 != res) ? true : false;
#else
		EVP_PKEY* publicKeyEngine = NULL;
		PEM_read_bio_PUBKEY(bio, &publicKeyEngine, 0, NULL);

		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(publicKeyEngine, NULL);
		EVP_PKEY_encrypt_init(ctx);
		EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);

		size_t out_len = 0;
		EVP_PKEY_encrypt(ctx, NULL, &out_len, data, (size_t)size);

		data_crypt = openssl_alloc((unsigned int)out_len);

		EVP_PKEY_encrypt(ctx, data_crypt, &out_len, data, (size_t)size);
		data_crypt_len = (unsigned int)out_len;

		EVP_PKEY_CTX_free(ctx);
		EVP_PKEY_free(publicKeyEngine);
		BIO_free(bio);

		return (out_len > 0) ? true : false;
#endif
	}

	bool RSA_DecryptPrivate(const unsigned char* privateKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_decrypt, unsigned int& data_decrypt_len)
	{
		BIO* bio = BIO_new_mem_buf(privateKey, (int)strlen((char*)privateKey));

#ifdef USE_DEPRECATED
		RSA* rsa = RSA_new();
		PEM_read_bio_RSAPrivateKey(bio, &rsa, 0, NULL);

		unsigned int key_size = (unsigned int)RSA_size(rsa);
		data_decrypt = openssl_alloc(key_size);

		int res = RSA_private_decrypt((int)size, data, data_decrypt, rsa, RSA_NO_PADDING);
		data_decrypt_len = key_size;

		BIO_free(bio);
		RSA_free(rsa);
		return (-1 != res) ? true : false;
#else
		EVP_PKEY* privateKeyEngine = NULL;
		PEM_read_bio_PrivateKey(bio, &privateKeyEngine, 0, NULL);

		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(privateKeyEngine, NULL);
		EVP_PKEY_decrypt_init(ctx);
		EVP_PKEY_CTX_set_rsa_padding(ctx, RSA_PKCS1_OAEP_PADDING);

		size_t out_len = 0;
		EVP_PKEY_decrypt(ctx, NULL, &out_len, data, (size_t)size);

		data_decrypt = openssl_alloc((unsigned int)out_len);

		EVP_PKEY_decrypt(ctx, data_decrypt, &out_len, data, (size_t)size);
		data_decrypt_len = (unsigned int)out_len;

		EVP_PKEY_CTX_free(ctx);
		EVP_PKEY_free(privateKeyEngine);
		BIO_free(bio);

		return (out_len > 0) ? true : false;
#endif
	}

	bool RSA_EncryptPublic_desktop(const unsigned char* publicKey, const std::string& input, std::string& out)
	{
		unsigned char* out_ptr = NULL;
		unsigned int out_ptr_len = 0;

		if (!RSA_EncryptPublic(publicKey, (unsigned char*)input.c_str(), (unsigned int)input.length(), out_ptr, out_ptr_len))
			return false;

		out = Serialize(out_ptr, out_ptr_len, OPENSSL_SERIALIZE_TYPE_BASE64);
		openssl_free(out_ptr);
		return true;
	}
	bool RSA_DecryptPrivate_desktop(const unsigned char* privateKey, const std::string& input, std::string& out)
	{
		unsigned char* input_ptr = NULL;
		int input_ptr_len = 0;
		bool bBase64 = NSFile::CBase64Converter::Decode(input.c_str(), (int)input.length(), input_ptr, input_ptr_len);
		if (!bBase64)
			return false;

		unsigned char* out_ptr = NULL;
		unsigned int out_ptr_len = 0;

		if (!RSA_DecryptPrivate(privateKey, input_ptr, (unsigned int)input_ptr_len, out_ptr, out_ptr_len))
		{
			openssl_free(input_ptr);
			return false;
		}

		//out = Serialize(out_ptr, out_ptr_len, OPENSSL_SERIALIZE_TYPE_ASCII);
		out = std::string((char*)out_ptr, out_ptr_len);
		openssl_free(input_ptr);
		openssl_free(out_ptr);
		return true;
	}

	// pbkdf2
	const EVP_MD* Get_EVP_MD(int nAlg)
	{
		switch (nAlg)
		{
		case OPENSSL_HASH_ALG_SHA1:
		{
			return EVP_sha1();
		}
		case OPENSSL_HASH_ALG_SHA224:
		{
			return EVP_sha224();
		}
		case OPENSSL_HASH_ALG_SHA256:
		{
			return EVP_sha256();
		}
		case OPENSSL_HASH_ALG_SHA384:
		{
			return EVP_sha384();
		}
		case OPENSSL_HASH_ALG_SHA512:
		{
			return EVP_sha512();
		}
		default:
			break;
		}
		return EVP_sha1();
	}
	unsigned char* PBKDF2(const char* pass, int passlen, const unsigned char* salt, int saltlen, int hash_alg, int key_len)
	{
		unsigned char* out = openssl_alloc(key_len);
		if (0 == PKCS5_PBKDF2_HMAC(pass, passlen, salt, saltlen, 1000, Get_EVP_MD(hash_alg), key_len, out))
		{
			openssl_free(out);
			out = NULL;
		}
		return out;
	}
	unsigned char* PBKDF2_desktop(const std::string& pass, const std::string& salt)
	{
		unsigned char* key_iv = NULL;
		if (salt.empty())
		{
			unsigned int pass_salt_len = 0;
			unsigned char* pass_salt = NSOpenSSL::GetHash((unsigned char*)pass.c_str(), (unsigned int)pass.length(), OPENSSL_HASH_ALG_SHA512, pass_salt_len);
			key_iv = PBKDF2(pass.c_str(), (int)pass.length(), pass_salt, pass_salt_len, OPENSSL_HASH_ALG_SHA256, 32 + 16);
			openssl_free(pass_salt);
		}
		else
		{
			key_iv = PBKDF2(pass.c_str(), (int)pass.length(), (const unsigned char*)salt.c_str(), (unsigned int)salt.length(), OPENSSL_HASH_ALG_SHA256, 32 + 16);
		}
		return key_iv;
	}

	// new algs
	bool GenerateKeysByAlgs(const std::string& alg, std::string& publicKey, std::string& privateKey)
	{
		EVP_PKEY* pkey = NULL;
		EVP_PKEY_CTX* pctx = NULL;

		if (alg == "ed25519")
			pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_ED25519, NULL);
		else if (alg == "x25519")
			pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, NULL);
		else
			return false;

		EVP_PKEY_keygen_init(pctx);
		EVP_PKEY_keygen(pctx, &pkey);
		EVP_PKEY_CTX_free(pctx);

		BYTE* dataBio = NULL;
		int dataBioSize = 0;

		BIO* bio = BIO_new(BIO_s_mem());
		PEM_write_bio_PrivateKey(bio, pkey, NULL, NULL, 0, NULL, NULL);
		dataBioSize = (int)BIO_get_mem_data(bio, &dataBio);
		privateKey = std::string((char*)dataBio, dataBioSize);
		BIO_free(bio);

		bio = BIO_new(BIO_s_mem());
		PEM_write_bio_PUBKEY(bio, pkey);
		dataBioSize = (int)BIO_get_mem_data(bio, &dataBio);
		publicKey = std::string((char*)dataBio, dataBioSize);
		BIO_free(bio);

		EVP_PKEY_free(pkey);
		return true;
	}
	CMemoryData Sign(const unsigned char* data, const int& len, const std::string& privateKey)
	{
		CMemoryData returnData;

		BIO* bio = BIO_new_mem_buf((void*)privateKey.c_str(), (int)privateKey.length());
		EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
		if (!pkey)
		{
			BIO_free(bio);
			return returnData;
		}
		BIO_free(bio);

		size_t nSignatureLen = 0;
		EVP_MD_CTX* pCtx = EVP_MD_CTX_new();
		EVP_DigestSignInit(pCtx, NULL, NULL, NULL, pkey);

		/* Calculate the requires size for the signature by passing a NULL buffer */
		EVP_DigestSign(pCtx, NULL, &nSignatureLen, data, (size_t)len);
		returnData.Alloc(nSignatureLen);
		EVP_DigestSign(pCtx, returnData.Data, &nSignatureLen,  data, (size_t)len);
		returnData.Size = nSignatureLen;

		EVP_MD_CTX_free(pCtx);
		EVP_PKEY_free(pkey);

		return returnData;
	}

	bool Verify(const unsigned char* data, const int& data_len, const std::string& publicKey,
				const unsigned char* signature, const int& signature_len)
	{
		BIO* bio = BIO_new_mem_buf((void*)publicKey.c_str(), (int)publicKey.length());
		EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
		if (!pkey)
		{
			BIO_free(bio);
			return false;
		}
		BIO_free(bio);

		EVP_MD_CTX* pCtx = EVP_MD_CTX_new();
		EVP_DigestVerifyInit(pCtx, NULL, NULL, NULL, pkey);
		int nResult = EVP_DigestVerify(pCtx, signature, (size_t)signature_len, data, (size_t)data_len);

		EVP_MD_CTX_free(pCtx);
		EVP_PKEY_free(pkey);

		return (1 == nResult) ? true : false;
	}

	CMemoryData Enrypt(const unsigned char* data, const int& data_len, const std::string& publicKey)
	{
		CMemoryData returnData;

		BIO* bio = BIO_new_mem_buf((void*)publicKey.c_str(), (int)publicKey.length());
		EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
		if (!pkey)
		{
			BIO_free(bio);
			return returnData;
		}
		BIO_free(bio);

		EVP_PKEY_CTX* ctx;
		ctx = EVP_PKEY_CTX_new(pkey, NULL);
		if (EVP_PKEY_encrypt_init(ctx) <= 0)
		{
			EVP_PKEY_free(pkey);
			EVP_PKEY_CTX_free(ctx);
			return returnData;
		}

		size_t ciphertextLen = 0;
		EVP_PKEY_encrypt(ctx, NULL, &ciphertextLen, data, (size_t)data_len);
		returnData.Alloc(ciphertextLen);
		EVP_PKEY_encrypt(ctx, returnData.Data, &returnData.Size, data, (size_t)data_len);

		EVP_PKEY_free(pkey);
		EVP_PKEY_CTX_free(ctx);

		return returnData;
	}

	CMemoryData Decrypt(const unsigned char* data, const int& data_len, const std::string& privateKey)
	{
		CMemoryData returnData;

		BIO* bio = BIO_new_mem_buf((void*)privateKey.c_str(), (int)privateKey.length());
		EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
		if (!pkey)
		{
			BIO_free(bio);
			return returnData;
		}
		BIO_free(bio);

		EVP_PKEY_CTX* ctx;
		ctx = EVP_PKEY_CTX_new(pkey, NULL);
		if (EVP_PKEY_decrypt_init(ctx) <= 0)
		{
			EVP_PKEY_free(pkey);
			EVP_PKEY_CTX_free(ctx);
			return returnData;
		}

		size_t ciphertextLen = 0;
		EVP_PKEY_decrypt(ctx, NULL, &ciphertextLen, data, (size_t)data_len);
		returnData.Alloc(ciphertextLen);
		EVP_PKEY_decrypt(ctx, returnData.Data, &returnData.Size, data, (size_t)data_len);

		EVP_PKEY_free(pkey);
		EVP_PKEY_CTX_free(ctx);

		return returnData;
	}

	// aes
	int AES_GetKeySize(int type)
	{
		switch (type)
		{
		case OPENSSL_AES_256_CBC:
			return 32;
		}
		return 32;
	}
	int AES_GetIvSize(int type)
	{
		switch (type)
		{
		case OPENSSL_AES_256_CBC:
			return 16;
		}
		return 16;
	}

	const EVP_CIPHER* _get_cipher_aes(int type)
	{
		switch (type)
		{
		case OPENSSL_AES_256_CBC:
			return EVP_aes_256_cbc();
		}
		return NULL;
	}
	bool AES_Encrypt(int type, const unsigned char* key, const unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len)
	{
		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		EVP_CIPHER_CTX_init(ctx);
		EVP_EncryptInit_ex(ctx, _get_cipher_aes(type), NULL, key, iv);
		EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL);
		int out_len1 = (int)size + AES_BLOCK_SIZE;
		int out_len2 = 0;
		data_crypt = openssl_alloc(out_len1);
		EVP_EncryptUpdate(ctx, data_crypt, &out_len1, data, (int)size);
		EVP_EncryptFinal_ex(ctx, data_crypt + out_len1, &out_len2);
		data_crypt_len = out_len1 + out_len2;
		EVP_CIPHER_CTX_free(ctx);
		EVP_cleanup();
		return true;
	}
	bool AES_Decrypt(int type, const unsigned char* key, const unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_decrypt, unsigned int& data_decrypt_len)
	{
		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		EVP_CIPHER_CTX_init(ctx);
		EVP_DecryptInit_ex(ctx, _get_cipher_aes(type), NULL, key, iv);
		EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 16, NULL);
		int out_len1 = (int)size;
		int out_len2 = 0;
		data_decrypt = openssl_alloc(out_len1);
		EVP_DecryptUpdate(ctx, data_decrypt, &out_len1, data, (int)size);
		EVP_DecryptFinal_ex(ctx, data_decrypt + out_len1, &out_len2);
		data_decrypt_len = out_len1 + out_len2;
		EVP_CIPHER_CTX_free(ctx);
		EVP_cleanup();
		return true;
	}

	bool AES_Encrypt_desktop(const std::string& pass, const std::string& input, std::string& output, const std::string& salt)
	{
		unsigned char* key_iv = PBKDF2_desktop(pass, salt);
		bool bRes = AES_Encrypt_desktop(key_iv, input, output);
		openssl_free(key_iv);
		return bRes;
	}
	bool AES_Decrypt_desktop(const std::string& pass, const std::string& input, std::string& output,  const std::string& salt)
	{
		unsigned char* key_iv = PBKDF2_desktop(pass, salt);
		bool bRes = AES_Decrypt_desktop(key_iv, input, output);
		openssl_free(key_iv);
		return bRes;
	}
	bool AES_Encrypt_desktop(const unsigned char* key_iv, const std::string& input, std::string& output)
	{
		unsigned char* data_crypt = NULL;
		unsigned int data_crypt_len = 0;
		bool bRes = AES_Encrypt(OPENSSL_AES_256_CBC, key_iv, key_iv + 32, (unsigned char*)input.c_str(), (unsigned int)input.length(), data_crypt, data_crypt_len);

		if (!bRes)
			return false;

		output = Serialize(data_crypt, data_crypt_len, OPENSSL_SERIALIZE_TYPE_BASE64);
		openssl_free(data_crypt);
		return true;
	}
	bool AES_Decrypt_desktop(const unsigned char* key_iv, const std::string& input, std::string& output)
	{
		unsigned char* input_ptr = NULL;
		int input_ptr_len = 0;
		bool bBase64 = NSFile::CBase64Converter::Decode(input.c_str(), (int)input.length(), input_ptr, input_ptr_len);
		if (!bBase64)
			return false;

		unsigned char* data_decrypt = NULL;
		unsigned int data_decrypt_len = 0;
		bool bRes = AES_Decrypt(OPENSSL_AES_256_CBC, key_iv, key_iv + 32, input_ptr, input_ptr_len, data_decrypt, data_decrypt_len);

		if (!bRes)
		{
			RELEASEARRAYOBJECTS(input_ptr);
			return false;
		}

		//output = Serialize(out_ptr, out_ptr_len, OPENSSL_SERIALIZE_TYPE_ASCII);
		output = std::string((char*)data_decrypt, data_decrypt_len);
		RELEASEARRAYOBJECTS(input_ptr);
		openssl_free(data_decrypt);
		return true;
	}

	// serialize
	std::string Serialize(const unsigned char* data, const unsigned int& size, const int& alg)
	{
		switch (alg)
		{
		case OPENSSL_SERIALIZE_TYPE_ASCII:
		{
			return std::string((char*)data, size);
		}
		case OPENSSL_SERIALIZE_TYPE_HEX:
		{
			std::string res;
			res.reserve(2 * size + 1);
			char tmp[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
			for (unsigned int i = 0; i < size; ++i)
			{
				res.append(1, tmp[data[i] >> 4]);
				res.append(1, tmp[data[i] & 0x0F]);
			}
			return res;
		}
		case OPENSSL_SERIALIZE_TYPE_BASE64:
		{
			char* pDataDst = NULL;
			int nDataDst = 0;
			NSFile::CBase64Converter::Encode((BYTE*)data, (int)size, pDataDst, nDataDst, NSBase64::B64_BASE64_FLAG_NOCRLF);
			std::string sBase64((char*)pDataDst, nDataDst);
			RELEASEARRAYOBJECTS(pDataDst);
			return sBase64;
		}
		default:
			break;
		}
		return "";
	}

	// AES GCM for private rooms
#define GCM_IV_LENGTH   12
#define GCM_TAG_LENGHT  16

	// для того, чтобы мы могли менять алгоритмы, в зависимости от версии
	// на шифровке - дописываем хедер. сделано на этом уровне, чтобы лишний раз не выделять/копировать память
	std::string g_aes_header = "VER2;";

	unsigned char* PBKDF2_desktop_GCM(const std::string& pass, const std::string& salt)
	{
		unsigned char* key = NULL;
		if (salt.empty())
		{
			unsigned int pass_salt_len = 0;
			unsigned char* pass_salt = NSOpenSSL::GetHash((unsigned char*)pass.c_str(), (unsigned int)pass.length(), OPENSSL_HASH_ALG_SHA512, pass_salt_len);
			key = PBKDF2(pass.c_str(), (int)pass.length(), pass_salt, pass_salt_len, OPENSSL_HASH_ALG_SHA256, 32);
			openssl_free(pass_salt);
		}
		else
		{
			key = PBKDF2(pass.c_str(), (int)pass.length(), (const unsigned char*)salt.c_str(), (unsigned int)salt.length(), OPENSSL_HASH_ALG_SHA256, 32);
		}
		return key;
	}

	bool AES_Decrypt_desktop_GCM(const unsigned char* key, const std::string& input, std::string& output, const int header_offset)
	{
		unsigned char* input_ptr = NULL;
		int input_ptr_len = 0;
		bool bBase64 = NSFile::CBase64Converter::Decode(input.c_str() + header_offset, (int)input.length() - header_offset, input_ptr, input_ptr_len);
		if (!bBase64)
			return false;

		unsigned char* iv_ptr = input_ptr;
		unsigned char* tag_ptr = input_ptr + GCM_IV_LENGTH;
		unsigned char* ciphertext_ptr = tag_ptr + GCM_TAG_LENGHT;
		int ciphertext_len = input_ptr_len - (GCM_IV_LENGTH + GCM_TAG_LENGHT);
		unsigned char* output_ptr = NULL;
		int output_len = 0;
		int final_len = 0;

		bool bResult = false;

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		if (!ctx)
			goto end;

		if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
			goto end;

		if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LENGTH, NULL))
			goto end;

		if (!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv_ptr))
			goto end;

		output_ptr = openssl_alloc(ciphertext_len);
		if (!EVP_DecryptUpdate(ctx, output_ptr, &output_len, ciphertext_ptr, ciphertext_len))
			goto end;

		if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, GCM_TAG_LENGHT, tag_ptr))
			goto end;

		if (EVP_DecryptFinal_ex(ctx, output_ptr + output_len, &final_len))
		{
			output_len += final_len;
			bResult = true;
		}

end:
		RELEASEARRAYOBJECTS(input_ptr);
		if (ctx)
			EVP_CIPHER_CTX_free(ctx);

		if (bResult)
		{
			output = std::string((char*)output_ptr, output_len);
		}

		if (output_ptr)
			openssl_free(output_ptr);

		return bResult;
	}
	bool AES_Encrypt_desktop_GCM(const unsigned char* key, const std::string& input, std::string& output)
	{
		const unsigned char* input_ptr = (const unsigned char*)input.c_str();
		int input_len = (int)input.length();

		int output_buffer_all_offset = GCM_IV_LENGTH + GCM_TAG_LENGHT;
		int output_buffer_len = input_len + output_buffer_all_offset;
		unsigned char* output_ptr = NULL;
		unsigned char* iv_ptr = NULL;
		unsigned char* tag_ptr = NULL;
		unsigned char* ciphertext_ptr = NULL;
		int ciphertext_len = 0;
		int final_len = 0;

		bool bResult = false;

		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		if (!ctx)
			goto end;

		if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL))
			goto end;

		if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, GCM_IV_LENGTH, NULL))
			goto end;

		output_ptr = openssl_alloc(output_buffer_len);

		iv_ptr = output_ptr;
		tag_ptr = iv_ptr + GCM_IV_LENGTH;
		ciphertext_ptr = tag_ptr + GCM_TAG_LENGHT;

		if (1 != RAND_bytes(iv_ptr, GCM_IV_LENGTH))
			goto end;

		if (1 != EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv_ptr))
			goto end;

		if (1 != EVP_EncryptUpdate(ctx, ciphertext_ptr, &ciphertext_len, input_ptr, input_len))
			goto end;

		if (1 != EVP_EncryptFinal_ex(ctx, ciphertext_ptr + ciphertext_len, &final_len))
			goto end;
		ciphertext_len += final_len;

		if (1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, GCM_TAG_LENGHT, tag_ptr))
			goto end;

		bResult = true;

end:
		if (ctx)
			EVP_CIPHER_CTX_free(ctx);

		if (bResult)
		{
			// header + base64
			char* pDataDst = NULL;
			int nDataDst = 0;
			NSFile::CBase64Converter::Encode(output_ptr, ciphertext_len + output_buffer_all_offset, pDataDst, nDataDst, NSBase64::B64_BASE64_FLAG_NOCRLF);
			output = "";
			output.reserve(g_aes_header.length() + (size_t)nDataDst + 1);
			output.append(g_aes_header);
			output.append((char*)pDataDst, nDataDst);
			RELEASEARRAYOBJECTS(pDataDst);
		}

		if (output_ptr)
			openssl_free(output_ptr);

		return bResult;
	}

	bool AES_Encrypt_desktop_GCM(const std::string& pass, const std::string& input, std::string& output, const std::string& salt)
	{
		unsigned char* key = PBKDF2_desktop_GCM(pass, salt);
		bool bRes = AES_Encrypt_desktop_GCM(key, input, output);
		openssl_free(key);
		return bRes;
	}
	bool AES_Decrypt_desktop_GCM(const std::string& pass, const std::string& input, std::string& output, const std::string& salt, const int header_offset)
	{
		unsigned char* key = PBKDF2_desktop_GCM(pass, salt);
		bool bRes = AES_Decrypt_desktop_GCM(key, input, output, header_offset);
		openssl_free(key);
		return bRes;
	}
}
