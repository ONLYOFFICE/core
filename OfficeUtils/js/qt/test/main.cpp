#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../../src/OfficeUtils.h"
#include "../../wasm/src/base.h"

#include <string>
#include <iostream>
#include <fstream>

int main()
{
    COfficeUtils cOU;
    cOU.CompressFileOrDirectory(NSFile::GetProcessDirectory() + L"/test", NSFile::GetProcessDirectory() + L"/test.zip");

    std::string sData;
    NSFile::CFileBinary oFile;
    if (oFile.ReadAllTextUtf8A(NSFile::GetProcessDirectory() + L"/test.zip", sData))
        oFile.CloseFile();

    int nBytesCount = sData.length();
    char* pData = new char[nBytesCount];
    for (int i = 0; i < nBytesCount; i++)
        pData[i] = sData[i];

    myFile* res = Zlib_GetFile(pData, nBytesCount, L"a/b/c/1.txt");

    if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/res.txt"))
    {
        if (res->data)
            oFile.WriteFile(res->data, res->size);
        oFile.CloseFile();
    }

    delete[] pData;
    return 0;
}
