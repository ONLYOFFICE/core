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
#ifndef TABLES_H
#define TABLES_H

#include "Biff12RecordBase.h"
namespace XLSB {

    class AutoFilterRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 16)
            {
                std::cout<<("\tinfo : invalid autoFilter record length\r\n");
                return;
            }

            DWORD r1 = CBinaryBiff12StreamReader::GetDword(p + 0);
            DWORD r2 = CBinaryBiff12StreamReader::GetDword(p + 4);
            DWORD c1 = CBinaryBiff12StreamReader::GetDword(p + 8);
            DWORD c2 = CBinaryBiff12StreamReader::GetDword(p + 12);

            CHAR s[256];
            sprintf(s, "\tinfo : r1=%d, c1=%d, r2=%d, c2=%d\r\n", r1, c1, r2, c2);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "autoFilter";
        }
    };

    class AutoFilterEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/autoFilter";
        }
    };

    class FilterColumnRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "filterColumn";
        }
    };

    class FilterColumnEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/filterColumn";
        }
    };

    class FiltersRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "filters";
        }
    };

    class FiltersEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/filters";
        }
    };

    class FilterRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "filter";
        }
    };

    class TableRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 16)
            {
                std::cout<<("\tinfo : invalid table record length\r\n");
                return;
            }

            DWORD r1 = CBinaryBiff12StreamReader::GetDword(p + 0);
            DWORD r2 = CBinaryBiff12StreamReader::GetDword(p + 4);
            DWORD c1 = CBinaryBiff12StreamReader::GetDword(p + 8);
            DWORD c2 = CBinaryBiff12StreamReader::GetDword(p + 12);

            CHAR s[256];
            sprintf(s, "\tinfo : r1=%d, c1=%d, r2=%d, c2=%d\r\n", r1, c1, r2, c2);
            std::cout<<(s);

        }

        std::string GetTag()
        {
            return "table";
        }
    };

    class TableEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/table";
        }
    };

    class TableColumnsRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 4)
            {
                std::cout<<("\tinfo : invalid tableColumns record length\r\n");
                return;
            }

            DWORD columns = CBinaryBiff12StreamReader::GetDword(p + 0);

            CHAR s[256];
            sprintf(s, "\tinfo : columns=%d\r\n", columns);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "tableColumns";
        }
    };

    class TableColumnsEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/tableColumns";
        }
    };

    class TableColumnRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 32)
            {
                std::cout<<("\tinfo : invalid tableColumn record length\r\n");
                return;
            }

            DWORD columnId = CBinaryBiff12StreamReader::GetDword(p + 0);

            DWORD lenColumnName = CBinaryBiff12StreamReader::GetDword(p + 28);
            std::wstring sColumnName = CBinaryBiff12StreamReader::GetString(p + 32, lenColumnName);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : columnId=%d columnName=%s lenName=%d\r\n",
                columnId,
                sColumnName.c_str(),
                lenColumnName);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "tableColumn";
        }
    };

    class TableColumnEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/tableColumn";
        }
    };

    class TableStyleInfoRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 32)
            {
                std::cout<<("\tinfo : invalid tableStyleInfo record length\r\n");
                return;
            }

            WORD grbits = CBinaryBiff12StreamReader::GetWord(p + 0);

            DWORD lenStyleName = CBinaryBiff12StreamReader::GetDword(p + 2);
            std::wstring sStyleName = CBinaryBiff12StreamReader::GetString(p + 6, lenStyleName);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : grbits=%04X styleName=%s lenStyleName=%d\r\n",
                grbits,
                sStyleName.c_str(),
                lenStyleName);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "tableStyleInfo";
        }
    };

    class SortStateRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sortState";
        }
    };

    class SortConditionRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sortCondition";
        }
    };

    class SortStateEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sortState";
        }
    };
}
#endif
