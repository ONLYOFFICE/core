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

