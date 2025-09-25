/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "ExObjListContainer.h"

using namespace PPT;

void CRecordExObjListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);
}

CRecordExObjStg::CRecordExObjStg(const std::wstring& name, const std::wstring& tempPath)
{
    if (name.empty())
    {
        m_sFileName = NSFile::CFileBinary::CreateTempFileWithUniqueName(tempPath, L"bin");
    }
    else
    {
        m_sFileName = tempPath + FILE_SEPARATOR_STR + name;
    }
}

CRecordExObjStg::~CRecordExObjStg()
{
}

void CRecordExObjStg::ReadFromStream(SRecordHeader& oHeader, POLE::Stream* pStream)
{
    m_oHeader = oHeader;

    ULONG decompressedSize = m_oHeader.RecLen, compressedSize = m_oHeader.RecLen;

    BYTE* pData = new BYTE[compressedSize];
    if (!pData) return;

    if (m_oHeader.RecInstance == 0x01)
    {
        decompressedSize = StreamUtils::ReadDWORD(pStream) + 64;
        compressedSize -= 4;
    }
    pStream->read(pData, compressedSize);

    //if (pDecryptor)
    //{
    //	pDecryptor->Decrypt((char*)pData, compressedSize, 0);
    //}

    if (m_oHeader.RecInstance == 0x01)
    {
        BYTE* pDataUncompress = new BYTE[decompressedSize];
        NSZip::Decompress(pData, compressedSize, pDataUncompress, decompressedSize);

        delete[]pData;
        pData = pDataUncompress;
    }

    NSFile::CFileBinary file;
    if (file.CreateFileW(m_sFileName))
    {
        file.WriteFile(pData, decompressedSize);
        file.CloseFile();
    }
    else
    {
        m_sFileName.clear();
    }
    delete[] pData;
    pData = NULL;
}
