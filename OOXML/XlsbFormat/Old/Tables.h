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
