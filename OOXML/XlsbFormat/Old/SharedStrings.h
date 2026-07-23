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
#ifndef SHAREDSTRINGS_H
#define SHAREDSTRINGS_H

#include "Biff12RecordFactory.h"
#include "Biff12Structures/RichStr.h"

namespace XLSB {

    class SIRecord : public CBiff12RecordBase
    {
        RichStr m_RichStr;
        public:
            void Read(LPBYTE p, DWORD recid, DWORD reclen)
            {
                if (reclen >= 5)
                {
                    m_RichStr.Read(p);

                }
                else
                {
                    std::cout <<("\tinfo : invalid si record length\r\n");
                }

            }

            std::string GetTag()
            {
                return "si";
            }

            RecordType GetRecordType() {
                return rt_SST_ITEM;
            }

            RichStr& GetRichStr()
            {
                return m_RichStr;
            }
    };

    class SSTRecord : public CBiff12RecordBase
    {
        _INT32	cstTotal;
        _INT32	cstUnique;

        public:

            void Read(LPBYTE p, DWORD recid, DWORD reclen)
            {
                cstTotal = CBinaryBiff12StreamReader::GetDword(p);
                cstUnique = CBinaryBiff12StreamReader::GetDword(p + 4);
            }

            std::string GetTag()
            {
                return "sst";
            }

            RecordType GetRecordType() {
                return rt_BEGIN_SST;
            }

            _INT32 GetTotal()
            {
                return cstTotal;
            }

            _INT32 GetUnique()
            {
                return cstTotal;
            }
    };

    class SSTEndRecord : public CBiff12RecordBase
    {
        void Read(LPBYTE p, DWORD recid, DWORD reclen)
        {
        }

        std::string GetTag()
        {
            return "/sst";
        }

        RecordType GetRecordType() {
            return rt_END_SST;
        }
    };


    static Registrar<SIRecord> oSIRecord(BIFF12_SI);
    static Registrar<SSTRecord> oSSTRecord(BIFF12_SST);
    static Registrar<SSTEndRecord> oSSTEndRecord(BIFF12_SST_END);
}
#endif

