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
