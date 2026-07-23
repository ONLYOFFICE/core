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

#include "./../common/common_openssl.h"

int main(int argc, char *argv[])
{
    argc;
    argv;
    if (true)
    {
        std::string sTestHashString = "knoejnrgijwenrgiojwnergjiwnerigjnwerojgnweorigjn";
        unsigned int data_len = 0;
        unsigned char* data = NSOpenSSL::GetHash((unsigned char*)sTestHashString.c_str(), (unsigned int)sTestHashString.length(), OPENSSL_HASH_ALG_SHA256, data_len);
        std::string sResult = NSOpenSSL::Serialize(data, data_len, OPENSSL_SERIALIZE_TYPE_HEX);
        NSOpenSSL::openssl_free(data);
    }

    if (true)
    {
        unsigned char* publicKey = NULL;
        unsigned char* privateKey = NULL;
        bool bRes = NSOpenSSL::RSA_GenerateKeys(publicKey, privateKey);
        bRes;

        std::string sPublic((char*)publicKey);
        std::string sPrivate((char*)privateKey);

        NSOpenSSL::openssl_free(publicKey);
        NSOpenSSL::openssl_free(privateKey);

        std::string sMessage = "Hello world";

        unsigned char* message_crypt = NULL;
        unsigned int message_crypt_len = 0;
        bool bEncrypt = NSOpenSSL::RSA_EncryptPublic((unsigned char*)sPublic.c_str(), (unsigned char*)sMessage.c_str(), (unsigned int)sMessage.length(), message_crypt, message_crypt_len);
        bEncrypt;

        unsigned char* message_decrypt = NULL;
        unsigned int message_decrypt_len = 0;

        bool bDecrypt = NSOpenSSL::RSA_DecryptPrivate((unsigned char*)sPrivate.c_str(), message_crypt, message_crypt_len, message_decrypt, message_decrypt_len);
        bDecrypt;

        std::string sMessageOut((char*)message_decrypt, message_decrypt_len);

        NSOpenSSL::openssl_free(message_crypt);
        NSOpenSSL::openssl_free(message_decrypt);
    }

    if (true)
    {
        std::string password = "{PASSWORD}";
        std::string message = "{MESSAGE}";
        std::string message_crypted = "";
        std::string message_decrypted = "";

        NSOpenSSL::AES_Encrypt_desktop(password, message, message_crypted);
        NSOpenSSL::AES_Decrypt_desktop(password, message_crypted, message_decrypted);

        message;
    }

    return 0;
}
