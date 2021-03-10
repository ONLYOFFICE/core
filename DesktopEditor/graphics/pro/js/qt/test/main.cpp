#include "../../../../../common/Types.h"
#include "../../../../../common/File.h"

#include <string>

BYTE* JPGToPixels(BYTE* pData, int nWidth, int nHeight)
{
    BYTE* pRes = new BYTE[4 * nWidth * nHeight];
    if (!pRes)
        return NULL;
}

int main()
{
    NSFile::CFileBinary oFile;
    if (!oFile.OpenFile(L"test.jpg"))
        return 1;

    CxIOFile hFile(oFile.GetFileNative());
    uint32_t pos = hFile.Tell();

    CxImageJPG *newima = new CxImageJPG;
    if (!newima)
        return 2;

    if (!newima->Decode(hFile))
        return 3;
    Transfer(*newima);
    delete newima;
    return 0;

    /*
    CxImage img;
    if (!img.Decode(nFile, m_nFileType))
        return 3;

    DWORD nBytesCount = 0;
    BYTE* pData = NULL;
    NSFile::CFileBinary oFileBinary;
    if (oFileBinary.OpenFile(L"test.jpg"))
    {
        long nFileSize = oFileBinary.GetFileSize();
        pData = new BYTE[nFileSize];
        if (oFileBinary.ReadFile(pData, nFileSize, nBytesCount))
            oFileBinary.CloseFile();
        else
            RELEASEARRAYOBJECTS(pData);
    }

    return 0;
    */
}
