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
}
