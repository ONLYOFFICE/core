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
#ifndef WORKBOOK_H
#define WORKBOOK_H

#include "BaseRecord.h"
#include <vector>
namespace XLSB {

    class DefinedNameRecord : public BaseRecord
    {

        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen >= 17)
            {
                WORD grbits    = GetWord(p + 0);
                DWORD nametype = GetDword(p + 5);
                DWORD lenName  = GetDword(p + 9);

                std::wstring wszName = GetString(p + 13, lenName);

                DWORD lenFormula = GetDword(p + 13 + 2 * lenName);
                // formula ptg tokens follow between
                //  p + 13 + 2 * lenName + 4
                //   and
                //  p + 13 + 2 * lenName + 4 + lenFormula

                bool bHidden  = (grbits & 0x0001) != 0;
                bool bFunc    = (grbits & 0x0002) != 0;
                bool bBuiltin = (grbits & 0x0020) != 0;

                WCHAR wszNameType[64];
                switch (nametype)
                {
                    case 0 : wcscpy(wszNameType, L"Print_Area"); break; // or Print_Titles in fact
                    case 1 : wcscpy(wszNameType, L"Database"); break;
                    //TODO (Consolidate_Area,Auto_Open,Auto_Close,Extract,Criteria,Recorder
                    //      Data_Form,Auto_Activate,Auto_Deactivate,Sheet_Title,FilterDatabase)
                    default : wcscpy(wszNameType, L"Normal"); break;
                }


                WCHAR s[256];
                swprintf(s, sizeof(s), L"\tinfo : hidden=%s, addinfunc=%s, builtin=%s, type=%s, name=%s, lenName=%d, lenFormula=%d\r\n",
                    bHidden ? L"true" : L"false",
                    bFunc ? L"true" : L"false",
                    bBuiltin ? L"true" : L"false",
                    wszNameType,
                    wszName.c_str(),
                    lenName,
                    lenFormula);
                std::cout<<(s);
            }
            else
            {
                std::cout<<("\tinfo : invalid defined name record length\r\n");
            }

        }

        std::string GetTag()
        {
            return "definedName";
        }
    };

    class FileVersionRecord : public BaseRecord
    {

        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "fileVersion";
        }
    };

    class WorkbookRecord : public BaseRecord
    {
    public:
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "workbook";
        }

    };

    class WorkbookEndRecord : public BaseRecord
    {
    public:
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/workbook";
        }

    };

    class BookViewsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "bookViews";
        }

    };

    class BookViewsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/bookViews";
        }

    };

    class SheetsRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "sheets";
        }

    };

    class SheetsEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sheets";
        }

    };

    class WorkbookPRRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "workbookPr";
        }

    };

    class SheetRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            DWORD sheet_id = GetDword(p + 8);

            DWORD sheet_opc_ref_len = GetDword(p + 12);

            std::wstring sheet_opc_ref = GetString(p + 16, sheet_opc_ref_len);

            DWORD sheet_name_len = GetDword( p + 16 + 2 * sheet_opc_ref_len);

            std::wstring sheet_name = GetString(p + 16 + 2 * sheet_opc_ref_len + 4, sheet_name_len);

            WCHAR s[300];
           swprintf(s, sizeof(s), L"\tinfo : id=%d, r:id=%s, name=%s\r\n",
                sheet_id,
                sheet_opc_ref.c_str(),
                sheet_name.c_str());
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "sheet";
        }

    };

    class CalcPRRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "calcPr";
        }

    };

    class WorkbookViewRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "workbookView";
        }

    };

    class WebPublishingRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "webPublishing";
        }

    };

    class ExternalReferencesRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "externalReferences";
        }

    };

    class ExternalReferenceRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
            if (reclen < 4)
            {
                std::cout<<("\tinfo : invalid externalReference record length\r\n");
                return;
            }

            DWORD lenStr = GetDword(p + 0);
            std::wstring sRId = GetString(p + 4, lenStr);

            WCHAR s[256];
           swprintf(s, sizeof(s), L"\tinfo : r:id=%s r:id:lenStr=%d\r\n",
                sRId.c_str(),
                lenStr);
            std::cout<<(s);
        }

        std::string GetTag()
        {
            return "externalReference";
        }

    };

    class ExternalReferencesEndRecord : public BaseRecord
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/externalReferences";
        }

    };
}


#endif
