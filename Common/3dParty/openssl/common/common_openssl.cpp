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
}
