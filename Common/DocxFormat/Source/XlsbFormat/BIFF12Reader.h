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

#ifndef BIFF12READER_H
#define BIFF12READER_H



#include "Workbook.h"
#include "Worksheet.h"
#include "SharedStrings.h"
#include "Styles.h"
#include "Comments.h"
#include "Tables.h"
#include "QueryTable.h"
#include "Connections.h"
#include <unordered_map>
#include <vector>
#include "../../../mnt/HDD_DATA/Work/core/DesktopEditor/common/Types.h"
#include "../Utility/SafeQueue.h"

namespace XLSB {

class CBIFF12Reader
{
   public:

    std::queue<std::shared_ptr<BaseRecord>> records;
    //SafeQueue<std::shared_ptr<BaseRecord>> records;

    typedef class _Buffer {
    public:
        BYTE* _ptr;
        int _len;

        _Buffer() {
            _ptr = NULL;
            _len = 0;
        }

        BYTE* NewOp(int len) {
            _ptr = (BYTE*) malloc(1 + len);
            _len = len;
            return _ptr;
        }

        ~_Buffer() {
            if (_ptr)
                free(_ptr);
            _ptr = NULL;
        }

    } Buffer;

    void readRecord(Buffer& buf) {

        // A generic BIFF12 part is a sequence of BIFF12 records

        // A BIFF12 record is a record identifier, followed by a record length, followed by the length

        // The record identifier is stored in 2 bytes

        // The record length is stored in a single byte (which limits record lengths to 255 bytes)

        // The record content is arbitrary content whose underlying classure is associated to the
        // the record identifier, and is defined once for all by the implementers of the file format

        BYTE* p = buf._ptr;
        int len = buf._len;

        if (!p || len == 0)
        {
           //records.stopwait();
           return;
        }

        BYTE* p_last = p + len;

        while (p < p_last)
        {
            DWORD recid = 0;
            DWORD reclen = 0;

            if (!BaseRecord::GetRecordId(/*in-out*/p, p_last, /*out*/recid) ||
                !BaseRecord::GetRecordLength(/*in-out*/p, p_last, /*out*/reclen))
            {
                std::cout << ("***Damaged buffer***\r\n");
                break;
            }

            if (recid == 0 && reclen == 0)
                break;

            std::shared_ptr<BaseRecord> record = nullptr;
            record = CBiff12RecordFactory::Instance()->Create((static_cast<int>(recid)));

            if (record != nullptr)
                record->Read(p, recid, reclen);

            records.push(record);
            //records.enqueue(record);

            if (!p) {
                std::cout <<("***Damaged buffer***\r\n");
                break;
            }

            p += reclen;

            std::cout << ("\r\n");
        }

        std::cout << ("\r\n");
        //records.stopwait();

    }

};

}

#endif
