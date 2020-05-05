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

#ifndef COMMON_OPENSSL_H
#define COMMON_OPENSSL_H

#define OPENSSL_HASH_ALG_SHA1         0
#define OPENSSL_HASH_ALG_INVALID      1
#define OPENSSL_HASH_ALG_SHA256       2
#define OPENSSL_HASH_ALG_SHA224       3
#define OPENSSL_HASH_ALG_SHA384       4
#define OPENSSL_HASH_ALG_SHA512       5

#define OPENSSL_SERIALIZE_TYPE_BASE64 0
#define OPENSSL_SERIALIZE_TYPE_HEX    1
#define OPENSSL_SERIALIZE_TYPE_ASCII  2

#define OPENSSL_AES_256_CBC           0

#include <openssl/sha.h>
#include "../../../../DesktopEditor/common/File.h"

namespace NSOpenSSL
{
    // alloc
    unsigned char* openssl_alloc(unsigned int len);
    void openssl_free(unsigned char* data);

    // hash
    unsigned int GetHashSize(const int& alg);
    unsigned char* GetHash(const unsigned char* data, const unsigned int& size, const int& alg, unsigned int& len);

    // rsa
    bool RSA_GenerateKeys(unsigned char*& publicKey, unsigned char*& privateKey);
    bool RSA_EncryptPublic(const unsigned char* publicKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);
    bool RSA_DecryptPrivate(const unsigned char* privateKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_decrypt, unsigned int& data_decrypt_len);

    bool RSA_EncryptPublic_desktop(const unsigned char* publicKey, const std::string& input, std::string& out);
    bool RSA_DecryptPrivate_desktop(const unsigned char* privateKey, const std::string& input, std::string& out);

    // pbkdf2
    unsigned char* PBKDF2(const char* pass, int passlen, unsigned char* salt, int saltlen, int hash_alg, int key_len);
    unsigned char* PBKDF2_desktop(const char* pass, int passlen, unsigned char* salt, int saltlen);

    // aes
    int AES_GetKeySize(int type);
    int AES_GetIvSize(int type);
    bool AES_Encrypt(int type, unsigned char* key, unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);
    bool AES_Decrypt(int type, unsigned char* key, unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);

    bool AES_Encrypt_desktop(const std::string& pass, const std::string& input, std::string& output);
    bool AES_Decrypt_desktop(const std::string& pass, const std::string& input, std::string& output);

    // serialize
    std::string Serialize(const unsigned char* data, const unsigned int& size, const int& alg);
}

#endif // COMMON_OPENSSL_H
