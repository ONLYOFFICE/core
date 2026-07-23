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
#include "ExOleObjAtom.h"

using namespace PPT;

void CRecordExOleObjAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nDrawAspect	= StreamUtils::ReadDWORD(pStream);
    m_nType			= StreamUtils::ReadDWORD(pStream);
    m_nExObjID		= StreamUtils::ReadDWORD(pStream);
    m_nSubType		= StreamUtils::ReadDWORD(pStream);
    m_nPersistID	= StreamUtils::ReadDWORD(pStream);

    StreamUtils::StreamSkip ( 4, pStream ); //unused
}

CRecordExOleObjStg::CRecordExOleObjStg(std::wstring strTemp) : m_strTmpDirectory(strTemp)
{
}

CRecordExOleObjStg::~CRecordExOleObjStg()
{

}

void CRecordExOleObjStg::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    ULONG decompressedSize = m_oHeader.RecLen, compressedSize = m_oHeader.RecLen;

    if (m_oHeader.RecInstance == 0x01)
    {
        compressedSize = m_oHeader.RecLen - 4;
        decompressedSize = StreamUtils::ReadDWORD(pStream);
    }

    BYTE* pData = (compressedSize > 0 && compressedSize < 0xffffff ) ? new BYTE[compressedSize] : NULL;

    if (!pData) return;

    compressedSize = pStream->read(pData, compressedSize);

    if (m_oHeader.RecInstance == 0x01)
    {
        BYTE* pDataUncompress = (compressedSize > 0 && compressedSize < 0xffffff) ?  new BYTE[decompressedSize + 64] : NULL;
        if ((pDataUncompress) && (NSZip::Decompress(pData, compressedSize, pDataUncompress, decompressedSize)))
        {
            delete []pData;
            pData = pDataUncompress;
        }
        else
        {
            delete []pData;
            pData = NULL;
        }
    }
    //if (pDecryptor)
    //{
    //	pDecryptor->Decrypt((char*)pData, oHeader.RecLen - lOffset, 0);
    //}
    if (pData)
    {
        m_sFileName = m_strTmpDirectory + FILE_SEPARATOR_STR +  L"oleObject_xxx.bin";

        NSFile::CFileBinary file;
        if (file.CreateFileW(m_sFileName))
        {
            file.WriteFile(pData, decompressedSize);
            file.CloseFile();
        }
        delete[] pData;
        pData = NULL;
    }
}
