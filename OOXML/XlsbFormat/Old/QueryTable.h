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
