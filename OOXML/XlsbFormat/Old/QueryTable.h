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
#ifndef QUERYTABLE_H
#define QUERYTABLE_H

#include "Biff12RecordBase.h"
namespace XLSB {

    class QueryTableRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 14)
            {
                DWORD connectionId = CBinaryBiff12StreamReader::GetDword(p + 6);

                DWORD lenName = CBinaryBiff12StreamReader::GetDword(p + 10);

                std::wstring s = CBinaryBiff12StreamReader::GetString(p + 10 + 4, lenName);

                WCHAR tmp[256];
                swprintf(tmp, sizeof(tmp), L"\tinfo : connectionId=%d, name=%s lenName=%d\r\n",
                    connectionId,
                    s.c_str(),
                    lenName);
                std::cout<<(tmp);
            }
            else
            {
                std::cout<<("\tinfo : invalid queryTable record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "queryTable";
        }
    };

    class QueryTableEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/queryTable";
        }
    };

    class QueryTableRefreshRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "queryTableRefresh";
        }
    };

    class QueryTableRefreshEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/queryTableRefresh";
        }
    };

    class QueryTableFieldsRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "queryTableFields";
        }
    };

    class QueryTableFieldsEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/queryTableFields";
        }
    };

    class QueryTableFieldRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 16)
            {
                DWORD fieldId = CBinaryBiff12StreamReader::GetDword(p + 4);
                DWORD tableColumnId = CBinaryBiff12StreamReader::GetDword(p + 8);

                DWORD lenfieldName = CBinaryBiff12StreamReader::GetDword(p + 12);

                std::wstring s = CBinaryBiff12StreamReader::GetString(p + 12 + 4, lenfieldName);

                WCHAR tmp[256];
                swprintf(tmp, sizeof(tmp), L"\tinfo : fieldId=%d, fieldName=%s lenfieldName=%d, tableColumnId=%d\r\n",
                    fieldId,
                    s.c_str(),
                    lenfieldName,
                    tableColumnId);
                std::cout<<(tmp);
            }
            else
            {
                std::cout<<("\tinfo : invalid queryTableField record length\r\n");
            }
        }

        std::string GetTag()
        {
            return "queryTableField";
        }
    };

    class QueryTableFieldEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/queryTableField";
        }
    };
}
#endif
