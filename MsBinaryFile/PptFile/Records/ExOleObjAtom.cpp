/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
