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

#ifndef BIFF12RECORDBASE_H
#define BIFF12RECORDBASE_H


#include "../../../../DesktopEditor/common/Types.h"
#include "../Base/Types_32.h"
#include "../XlsxFormat/WritingElement.h"
//#include "../../../mnt/HDD_DATA/Work/core/DesktopEditor/common/File.h"
#include <string>
#include <memory.h>
#include <iostream>
#include "RecordTypes.h"
#include "Common/BinaryBiff12StreamReader.h"
typedef BYTE *LPBYTE;

namespace XLSB
{
    class CBiff12RecordBase
    {
        //DWORD m_nReclen;
        //BYTE* m_pData;
        //CF_RECORD_TYPE m_eRecordType;

    public:
        virtual void Read(LPBYTE p, DWORD recid, DWORD reclen) {
           /* m_pData = p;
            m_nReclen = reclen;
            m_eRecordType = biff12TypeRecord.find(recid) != biff12TypeRecord.end()?
                        biff12TypeRecord.find(recid)->second : rt_UNKNOWN;*/
        }

        virtual std::string GetTag() {
            return "OVERRIDETHIS";
        }

        virtual RecordType GetRecordType() {
            return rt_UNKNOWN;
        }

        //DWORD getLengthRecord()
        //{
        //    return m_nReclen;
        //}

        virtual ~CBiff12RecordBase() {}

    };
}

#endif // BIFF12RECORDBASE_H

