#include "../../../../../common/Types.h"
#include "../../../../../common/File.h"
#include "../../wasm/src/base.h"

#include <string>
#include <iostream>

int main()
{
    BYTE* pData = NULL;
    DWORD nBytesCount;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.jpg", &pData, nBytesCount))
        return 1;
    oFile.CloseFile();

    CxImage* img = CxImage_Load(pData, (unsigned int)nBytesCount, 3);

    BYTE* pResInt = CxImage_GetRGBA(img);
    int nCountInt = 4 * CxImage_GetHeight(img) * CxImage_GetWidth(img);

    for (int i = 0; i < (nCountInt > 100 ? 100 : nCountInt); i++)
        std::cout << pResInt[i] << " ";

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
