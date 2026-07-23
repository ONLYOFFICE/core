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
