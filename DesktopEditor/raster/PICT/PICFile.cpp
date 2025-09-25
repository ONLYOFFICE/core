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
#include "PICFile.h"
#include "pic.h"

#include "../../common/File.h"

namespace PICT {

bool ImageToFrame(ImagePICT* pImage, CBgraFrame* pFrame, bool isRGBA)
{
    int nWidth = pImage->m_nWidth;
    int nHeight = pImage->m_nHeight;
    int BufferSize = 4 * nWidth * nHeight;

    if (BufferSize < 1)
        return false;

    pFrame->put_Height(nHeight);
    pFrame->put_Width(nWidth);
    pFrame->put_Stride(4 * nWidth);

    BYTE* pData = new BYTE[BufferSize];

    if (!pData)
        return false;

    pFrame->put_Data(pData);

    unsigned char* pBufferPtr = (unsigned char*)pData;

    unsigned int indR = isRGBA ? 2 : 0;
    unsigned int indG = 1;
    unsigned int indB = isRGBA ? 0 : 2;

    for (size_t i = 0; i < pImage->m_nHeight; i++)
    {
        unsigned char* q = pImage->ppixels + pImage->number_channels * (i * pImage->m_nWidth);
        for (size_t j = 0; j < pImage->m_nWidth; j++)
        {
            pBufferPtr[indR] = * q;
            pBufferPtr[indG] = * (q + 1);
            pBufferPtr[indB] = * (q + 2);
            pBufferPtr[3] = *(q + 3);
            q += 4;
            pBufferPtr += 4;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////

bool CPictFile::Open(CBgraFrame *pFrame, const std::wstring &strFileName, bool isRGBA)
{
    NSFile::CFileBinary oFile;
    if (!oFile.OpenFile(strFileName))
        return false;

    ImagePICT* pImage = new ImagePICT;
    AquireImage(pImage);

    bool status = false;
    if (DecodePICT(oFile.GetFileNative(), pImage))
    {
        status = ImageToFrame(pImage, pFrame, isRGBA);
    }

    DestroyImage(pImage);
    oFile.CloseFile();

    return status;
}

bool CPictFile::Open(CBgraFrame *pFrame, BYTE *pBuffer, int nSize, bool isRGBA)
{
    NSFile::CFileBinary oFile;
    std::wstring sTmpFile = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"pct");

    if (sTmpFile.empty())
        return false;

    if (!oFile.CreateFile(sTmpFile))
        return false;

    ImagePICT* pImage = new ImagePICT;
    AquireImage(pImage);

    oFile.WriteFile(pBuffer, nSize);
    oFile.CloseFile();

    oFile.OpenFile(sTmpFile);

    bool status = false;
    if (DecodePICT(oFile.GetFileNative(), pImage))
    {
        status = ImageToFrame(pImage, pFrame, isRGBA);
    }

    DestroyImage(pImage);
    oFile.CloseFile();

    if (NSFile::CFileBinary::Exists(sTmpFile))
        NSFile::CFileBinary::Remove(sTmpFile);

    return status;
}
}
