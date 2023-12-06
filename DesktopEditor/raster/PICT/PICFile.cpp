#include "PICFile.h"
#include "pic.h"

#include "../../common/File.h"

namespace PICT {

bool ImageToFrame(ImagePICT* pImage, CBgraFrame* pFrame, bool isRGBA) {
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

    for (int i = 0; i < pImage->m_nHeight; i++)
    {
        unsigned char* q = pImage->ppixels + pImage->number_channels * (i * pImage->m_nWidth);
        for (int j = 0; j < pImage->m_nWidth; j++)
        {
            pBufferPtr[indR] = *(q + 2);
            pBufferPtr[indG] = *(q + 1);
            pBufferPtr[indB] = *q;
            pBufferPtr[3] = *(q + 3);
            q += 4;
            pBufferPtr += 4;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////

bool CPictFile::Open(CBgraFrame *pFrame, const std::wstring &strFileName, bool isRGBA) {
    ImagePICT* pImage = new ImagePICT;

    AquireImage(pImage);

    NSFile::CFileBinary oFile;
    if (!oFile.OpenFile(strFileName))
        return false;

    if (DecodePICT(oFile.GetFileNative(), pImage))
    {
        bool status = ImageToFrame(pImage, pFrame, isRGBA);
        DestroyImage(pImage);
        oFile.CloseFile();

        return status;
    }
    else
    {
        DestroyImage(pImage);
        oFile.CloseFile();

        return false;
    }
}

bool CPictFile::Open(CBgraFrame *pFrame, BYTE *pBuffer, int nSize, bool isRGBA) {
    ImagePICT* pImage = new ImagePICT;

    AquireImage(pImage);

    NSFile::CFileBinary oFile;
    if (!oFile.CreateTempFile())
        return false;

    oFile.WriteFile(pBuffer, nSize);
    oFile.SetPosition(0);

    if (DecodePICT(oFile.GetFileNative(), pImage))
    {
        bool status = ImageToFrame(pImage, pFrame, isRGBA);
        DestroyImage(pImage);
        oFile.CloseFile();

        return status;
    }
    else
    {
        DestroyImage(pImage);
        oFile.CloseFile();

        return false;
    }
}
}
