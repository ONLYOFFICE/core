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

#ifndef BIFF12STREAMREADER_H
#define BIFF12STREAMREADER_H


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
#include "../../../DesktopEditor/common/Types.h"
#include "../../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"

namespace XLSB
{
    class CBiff12StreamReader
    {
        BYTE*	m_pData;
        LONG	m_lSize;
        LONG	m_lPos;
        BYTE*	m_pDataCur;
        BYTE*	m_pDataLast;

        public:

        CBiff12StreamReader(const std::wstring& sPath)
        {
            NSFile::CFileBinary oFile;
            if (oFile.OpenFile(sPath) == false)
                return;

            auto m_lStreamLen = (LONG)oFile.GetFileSize();
            auto m_pStream = new BYTE[m_lStreamLen];
            DWORD dwRead = 0;
            oFile.ReadFile(m_pStream, (DWORD)m_lStreamLen, dwRead);
            oFile.CloseFile();

            m_pData = m_pStream;
            m_lSize = m_lStreamLen;
            m_lPos = 0;
            m_pDataCur = m_pData;
            m_pDataLast = m_pData + m_lSize;
        }

        CF_RECORD_TYPE getNextRecordType()
        {
            DWORD recid = 0;
            auto dataCur = m_pDataCur;
            if(!CBinaryBiff12StreamReader::GetRecordId(/*in-out*/dataCur, m_pDataLast, /*out*/recid))
                return rt_UNKNOWN;
            else
                return biff12TypeRecord.find(recid)->second;
        }

        std::shared_ptr<CBiff12RecordBase> getNextRecord()
        {
            DWORD recid = 0;
            DWORD reclen = 0;
            if(m_lPos >= m_lSize)
                return nullptr;
            std::shared_ptr<CBiff12RecordBase> record = nullptr;
            if(!CBinaryBiff12StreamReader::GetRecordId(/*in-out*/m_pDataCur, m_pDataLast, /*out*/recid) ||
               !CBinaryBiff12StreamReader::GetRecordLength(/*in-out*/m_pDataCur, m_pDataLast, /*out*/reclen))
                return record;
            else
            {
                if (recid == 0 && reclen == 0)
                    return nullptr;

                record = CBiff12RecordFactory::Instance()->Create((static_cast<int>(recid)));
                if (record != nullptr)
                    record->Read(m_pDataCur, recid, reclen);

                m_lPos += reclen;
                m_pDataCur += reclen;
                return record;
            }
        }
    };
}

#endif
