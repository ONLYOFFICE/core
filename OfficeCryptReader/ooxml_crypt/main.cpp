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
#include <iostream>

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    if (argc <= 0)
        return 0;

    std::wstring file_path;
    std::string add_record;
    std::string remove_record;
    std::wstring password;
    bool is_print_info = false;

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
            if (!add_record.empty())
                add_record += "\n";
            add_record += "{";
            add_record += U_TO_UTF8(value);
            add_record += "}";
        }
        else if (key == L"remove")
        {
            remove_record = U_TO_UTF8(value);
            remove_record += ",";
        }
        else if (key == L"password")
        {
            password = value;
        }
        else if (key == L"info")
        {
            is_print_info = true;
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

    if (!remove_record.empty() && !docinfo.empty())
    {
        const char* doc_info_str = docinfo.c_str();
        const char* doc_info_str_end = doc_info_str + docinfo.length();

        const char* remove_record_str = remove_record.c_str();
        const char* remove_record_str_end = remove_record.c_str() + remove_record.length();

        while (doc_info_str < doc_info_str_end)
        {
            const char* rec_start = doc_info_str;

            // check record...
            ++doc_info_str; // skip {
            bool is_found = true;
            while (doc_info_str < doc_info_str_end && remove_record_str < remove_record_str_end)
            {
                if (*doc_info_str++ != *remove_record_str++)
                {
                    is_found = false;
                    break;
                }
            }
            if (!is_found)
                remove_record_str = remove_record.c_str();

            // move to next record
            while (doc_info_str < doc_info_str_end)
            {
                if (*doc_info_str == '\n')
                {
                    ++doc_info_str;
                    break;
                }
                ++doc_info_str;
            }

            if (is_found)
            {
                std::string::size_type first = rec_start - docinfo.c_str();
                std::string new_doc_info;
                if (first > 0)
                {
                    if (doc_info_str >= doc_info_str_end)
                        first -= 1;
                    new_doc_info = docinfo.substr(0, first);
                }
                if (doc_info_str < doc_info_str_end)
                {
                    new_doc_info += docinfo.substr(doc_info_str - docinfo.c_str());
                }

                docinfo = new_doc_info;
                break;
            }
        }
    }

    if (!add_record.empty())
    {
        if (!docinfo.empty())
            docinfo += "\n";
        docinfo += add_record;
    }

    bool result = file.WriteAdditional(file_path, L"DocumentID", docinfo);
    if (!result)
    {
        std::cout << "error: docinfo not writed" << std::endl;
        return 3;
    }

    return 0;
}
