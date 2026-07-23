/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

#include "../../../../DesktopEditor/xmlsec/src/include/openssl_config.h"

namespace NSOpenSSL
{
	class OPENSSL_DECL CMemoryData
	{
	public:
		unsigned char* Data;
		size_t Size;

	public:
		CMemoryData();
		~CMemoryData();

	public:
		std::string Serialize(const int& alg);
		void Alloc(const size_t& size);
		void Free();
	};

	// alloc
	OPENSSL_DECL unsigned char* openssl_alloc(unsigned int len);
	OPENSSL_DECL void openssl_free(unsigned char* data);

	// hash
	OPENSSL_DECL unsigned int GetHashSize(const int& alg);
	OPENSSL_DECL unsigned char* GetHash(const unsigned char* data, const unsigned int& size, const int& alg, unsigned int& len);

	// rsa
	OPENSSL_DECL bool RSA_GenerateKeys(unsigned char*& publicKey, unsigned char*& privateKey);
	OPENSSL_DECL bool RSA_EncryptPublic(const unsigned char* publicKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);
	OPENSSL_DECL bool RSA_DecryptPrivate(const unsigned char* privateKey, const unsigned char* data, const unsigned int& size, unsigned char*& data_decrypt, unsigned int& data_decrypt_len);

	OPENSSL_DECL bool RSA_EncryptPublic_desktop(const unsigned char* publicKey, const std::string& input, std::string& out);
	OPENSSL_DECL bool RSA_DecryptPrivate_desktop(const unsigned char* privateKey, const std::string& input, std::string& out);

	// pbkdf2
	OPENSSL_DECL unsigned char* PBKDF2(const char* pass, int passlen, const unsigned char* salt, int saltlen, int hash_alg, int key_len);
	OPENSSL_DECL unsigned char* PBKDF2_desktop(const std::string& pass, const std::string& salt = "");

	// ed25519/x25519 key pairs in pem format
	OPENSSL_DECL bool GenerateKeysByAlgs(const std::string& alg, std::string& publicKey, std::string& privateKey);
	OPENSSL_DECL CMemoryData Sign(const unsigned char* data, const int& len, const std::string& privateKey);
	OPENSSL_DECL bool Verify(const unsigned char* data, const int& data_len, const std::string& publicKey,
							 const unsigned char* signature, const int& signature_len);
	OPENSSL_DECL CMemoryData Enrypt(const unsigned char* data, const int& data_len, const std::string& publicKey);
	OPENSSL_DECL CMemoryData Decrypt(const unsigned char* data, const int& data_len, const std::string& privateKey);

	// aes
	OPENSSL_DECL int AES_GetKeySize(int type);
	OPENSSL_DECL int AES_GetIvSize(int type);
	OPENSSL_DECL bool AES_Encrypt(int type, const unsigned char* key, const unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);
	OPENSSL_DECL bool AES_Decrypt(int type, const unsigned char* key, const unsigned char* iv, const unsigned char* data, const unsigned int& size, unsigned char*& data_crypt, unsigned int& data_crypt_len);

	OPENSSL_DECL bool AES_Encrypt_desktop(const std::string& pass, const std::string& input, std::string& output, const std::string& salt = "");
	OPENSSL_DECL bool AES_Decrypt_desktop(const std::string& pass, const std::string& input, std::string& output, const std::string& salt = "");
	OPENSSL_DECL bool AES_Encrypt_desktop(const unsigned char* key_iv, const std::string& input, std::string& output);
	OPENSSL_DECL bool AES_Decrypt_desktop(const unsigned char* key_iv, const std::string& input, std::string& output);

	// serialize
	OPENSSL_DECL std::string Serialize(const unsigned char* data, const unsigned int& size, const int& alg);

	// GCM
	OPENSSL_DECL unsigned char* PBKDF2_desktop_GCM(const std::string& pass, const std::string& salt = "");
	OPENSSL_DECL bool AES_Decrypt_desktop_GCM(const unsigned char* key, const unsigned char* input, const int& input_len, CMemoryData& buffer);
	OPENSSL_DECL bool AES_Decrypt_desktop_GCM(const unsigned char* key, const std::string& input, std::string& output, const int header_offset = 0);
	OPENSSL_DECL bool AES_Decrypt_desktop_GCM(const std::string& pass, const std::string& input, std::string& output, const std::string& salt = "", const int header_offset = 0);

	OPENSSL_DECL bool AES_Encrypt_desktop_GCM(const unsigned char* key, const unsigned char* input, const int& input_len, CMemoryData& buffer);
	OPENSSL_DECL bool AES_Encrypt_desktop_GCM(const unsigned char* key, const std::string& input, std::string& output);
	OPENSSL_DECL bool AES_Encrypt_desktop_GCM(const std::string& pass, const std::string& input, std::string& output, const std::string& salt = "");
}

#endif // COMMON_OPENSSL_H
