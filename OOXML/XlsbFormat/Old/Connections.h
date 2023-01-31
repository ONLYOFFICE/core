/*
 * (c) Copyright Ascensio System SIA 2010-2021
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
#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include "Biff12RecordBase.h"
namespace XLSB {

    class ConnectionsRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "connections";
        }
    };

    class ConnectionsEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/connections";
        }
    };

    class ConnectionRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 16)
            {
                DWORD lensourceFile = CBinaryBiff12StreamReader::GetDword(p + 23);
                std::wstring wszSourceFile = CBinaryBiff12StreamReader::GetString(p + 23 + 4, lensourceFile);

                DWORD lenName = CBinaryBiff12StreamReader::GetDword(p + 23 + 4 + 2 * lensourceFile);
                std::wstring wszName = CBinaryBiff12StreamReader::GetString(p + 23 + 4 + 2 * lensourceFile + 4, lenName);

                WCHAR tmp[256];
                swprintf(tmp, sizeof(tmp), L"\tinfo : sourceFile=%s lensourceFile=%d, name=%s lenName=%d\r\n",
                    wszSourceFile.c_str(),
                    lensourceFile,
                    wszName.c_str(),
                    lenName);
                std::cout<<(tmp);
            }
            else
            {
                std::cout<<("\tinfo : invalid connection record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "connection";
        }
    };

    class ConnectionEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/connection";
        }
    };

    class DBPRRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 16)
            {
                DWORD lenConnection = CBinaryBiff12StreamReader::GetDword(p + 5);
                std::wstring wszConnection = CBinaryBiff12StreamReader::GetString(p + 5 + 4, lenConnection);

                WCHAR tmp[256];
                swprintf(tmp, sizeof(tmp), L"\tinfo : connection=%s lenConnection=%d\r\n",
                    wszConnection.c_str(),
                    lenConnection);
                std::cout<<(tmp);
            }
            else
            {
                std::cout<<("\tinfo : invalid dbPr record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "dbPr";
        }
    };

    class DBPREndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/dbPr";
        }
    };
}
#endif
