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

#include "./../source/ECMACryptFile.h"
#include "./../../DesktopEditor/common/File.h"
#include "./../../Common/3dParty/openssl/common/common_openssl.h"
#include <iostream>

// examples:
// ooxml_crypt --file=D:/cryptor/1.docx --password=111
// ooxml_crypt --file=D:/cryptor/1.docx --add={user-id-1}\ndata1
// ooxml_crypt --file=D:/cryptor/1.docx --remove={user-id-1}
// ooxml_crypt --file=D:/cryptor/1.docx --remove={user-id-1}\ndata1
// ooxml_crypt --file=D:/cryptor/1.docx --add={user-id-1}\ndata11
// ooxml_crypt --file=D:/cryptor/1.docx --info=

void string_replace(std::wstring& text, const std::wstring& replaceFrom, const std::wstring& replaceTo)
{
    size_t posn = 0;
    while (std::wstring::npos != (posn = text.find(replaceFrom, posn)))
    {
        text.replace(posn, replaceFrom.length(), replaceTo);
        posn += replaceTo.length();
    }
}
void string_replaceA(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
{
    size_t posn = 0;
    while (std::string::npos != (posn = text.find(replaceFrom, posn)))
    {
        text.replace(posn, replaceFrom.length(), replaceTo);
        posn += replaceTo.length();
    }
}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    if (argc <= 0)
        return 0;

    std::wstring file_path;
    std::wstring password;
    bool is_print_info = false;
    bool is_decrypt = false;
    std::string user;
    std::wstring user_key_file;

    std::vector<std::string> add_records;
    std::vector<std::string> remove_records;

    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring param(argv[i]);
#else
        std::string paramA(argv[i]);
        std::wstring param = UTF8_TO_U(paramA);
#endif

        std::wstring::size_type len = param.length();
        if (2 > len)
            continue;

        const wchar_t* param_str = param.c_str();
        if (param_str[0] != '-' || param_str[1] != '-')
            continue;

        std::wstring::size_type pos = param.find('=');
        if (std::wstring::npos == pos)
            continue;

        std::wstring key = param.substr(2, pos - 2);
        std::wstring value = param.substr(pos + 1);

        if (key == L"file")
        {
            file_path = value;
        }
        else if (key == L"add")
        {
            if (!value.empty())
            {
                if (value.c_str()[value.length() - 1] == '\n')
                    value = value.substr(0, value.length() - 1);

                string_replace(value, L";;;", L"\n");
                add_records.push_back(U_TO_UTF8(value));
            }
        }
        else if (key == L"remove")
        {
            if (!value.empty())
            {
                if (value.c_str()[value.length() - 1] == '\n')
                    value = value.substr(0, value.length() - 1);
                string_replace(value, L";;;", L"\n");
                remove_records.push_back(U_TO_UTF8(value));
            }
        }
        else if (key == L"password")
        {
            password = value;
        }
        else if (key == L"info")
        {
            is_print_info = true;
        }
        else if (key == L"decrypt")
        {
            is_decrypt = true;
        }
        else if (key == L"user")
        {
            user = U_TO_UTF8(value);
        }
        else if (key == L"key")
        {
            user_key_file = value;
        }
    }

    if (file_path.empty() || !NSFile::CFileBinary::Exists(file_path))
    {
        std::cout << "error: file not exist" << std::endl;
        return 1;
    }

    if (!password.empty())
    {
        // encrypt file
        ECMACryptFile file;
        bool result = file.EncryptOfficeFile(file_path, file_path, password);
        if (!result)
        {
            std::cout << "error: file is not encrypted" << std::endl;
            return 0;
        }
        return 2;
    }

    ECMACryptFile file;
    std::string docinfo = file.ReadAdditional(file_path, L"DocumentID");

    if (is_print_info)
    {
        std::cout << docinfo << std::endl;
        return 0;
    }

    const char* doc_info_str = docinfo.c_str();
    const char* doc_info_str_end = doc_info_str + docinfo.length();

    if (is_decrypt)
    {
        std::string encrypted_password = "";

        // находим нужную запись
        while (doc_info_str < doc_info_str_end)
        {
            const char* rec_start = doc_info_str;

            // 1) ищем старт записи
            while (doc_info_str < doc_info_str_end && *doc_info_str != '\n')
                ++doc_info_str;

            if (user == std::string(rec_start, doc_info_str - rec_start))
            {
                 rec_start = doc_info_str;

                 while (doc_info_str < doc_info_str_end && *doc_info_str != '\n')
                     ++doc_info_str;

                encrypted_password = std::string(rec_start, doc_info_str - rec_start);
            }

            // идем в конец записи
            while (doc_info_str < doc_info_str_end)
            {
                if (*doc_info_str++ == '\n')
                {
                    if (*doc_info_str == '\n')
                    {
                        ++doc_info_str;
                        break;
                    }
                }
            }
        }

        std::string private_key_content;
        NSFile::CFileBinary::ReadAllTextUtf8A(user_key_file, private_key_content);

        std::string passwordA;
        NSOpenSSL::RSA_DecryptPrivate_desktop((unsigned char*)private_key_content.c_str(), encrypted_password, passwordA);
        std::wstring password = UTF8_TO_U(passwordA);

        // encrypt file
        ECMACryptFile file;
        bool bDataIntegrity;
        bool result = file.DecryptOfficeFile(file_path, file_path, password, bDataIntegrity);
        if (!result)
        {
            std::cout << "error: file is not decrypted" << std::endl;
            return 0;
        }
        return 2;
    }

    std::string sResult = "";
    sResult.reserve(1000);

    while (doc_info_str < doc_info_str_end)
    {
        const char* rec_start = doc_info_str;

        // 1) ищем старт записи
        while (doc_info_str < doc_info_str_end && *doc_info_str != '\n')
            ++doc_info_str;

        std::string::size_type len_first = doc_info_str - rec_start;

        // 2) ищем конец записи
        while (doc_info_str < doc_info_str_end)
        {
            if (*doc_info_str++ == '\n')
            {
                if (*doc_info_str == '\n')
                {
                    ++doc_info_str;
                    break;
                }
            }
        }

        bool isAdd = true;
        std::string sRec = std::string(rec_start, (doc_info_str - rec_start - 2));

        // 3) проверяем запись на удаление
        for (std::vector<std::string>::iterator iter = remove_records.begin(); iter != remove_records.end(); iter++)
        {
            if (*iter == std::string(rec_start, len_first))
            {
                isAdd = false;
            }
            else if (*iter == sRec)
            {
                isAdd = false;
            }
        }

        // 4) проверяем запись на удаление
        for (std::vector<std::string>::iterator iter = add_records.begin(); iter != add_records.end(); iter++)
        {
            if (*iter == sRec)
            {
                isAdd = false;
            }
        }

        if (isAdd)
        {
            sResult += sRec;
            sResult += "\n\n";
        }
    }

    for (std::vector<std::string>::iterator iter = add_records.begin(); iter != add_records.end(); iter++)
    {
        sResult += *iter;
        sResult += "\n\n";
    }

    bool result = file.WriteAdditional(file_path, L"DocumentID", sResult);
    if (!result)
    {
        std::cout << "error: docinfo not writed" << std::endl;
        return 3;
    }

    return 0;
}
