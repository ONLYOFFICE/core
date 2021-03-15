#include "../../../../../common/Types.h"
#include "../../../../../common/File.h"
#include "../../wasm/src/base.h"

#include <string>

int main()
{
    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.jpg", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();

    CxImage* img = CxImage_Create();

    if (!CxImage_Decode(img, pData, (unsigned int)nBytesCount, 3))
        return 2;

    BYTE* pResInt = NULL;
    int nCountInt;
    if (!CxImage_Encode2RGBA(img, pResInt, nCountInt))
        return 3;

    BYTE* pRes = NULL;
    int nCount;
    if (!CxImage_Encode(img, pRes, nCount, 3))
        return 3;

    NSFile::CFileBinary oRes;
    if (oRes.CreateFileW(NSFile::GetProcessDirectory() + L"/res.jpg"))
    {
        oRes.WriteFile(pRes, nCount);
        oRes.CloseFile();
    }

    CxImage_Destroy(img);
    return 0;
}
