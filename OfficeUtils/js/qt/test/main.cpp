#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../wasm/src/base.h"

#include <string>
#include <iostream>
#include <fstream>

int main()
{
    std::string sData;
    std::ifstream in(NSFile::GetProcessDirectory() + L"/test.zip", std::ios::in | std::ios::binary);
    if (in.is_open())
    {
        char buf[512];
        while (in.read(buf, sizeof (buf)).gcount() > 0)
            sData.append(buf, in.gcount());
    }
    in.close();

    int nBytesCount = sData.length();
    char* pData = new char[nBytesCount];
    strcpy(pData, sData.c_str());

    unsigned char* res = Zlib_GetFile(pData, nBytesCount, L"a/b/c/1.txt");

    NSFile::CFileBinary oFile;
    if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/res.txt"))
    {
        oFile.WriteStringUTF8(std::wstring((wchar_t*)res));
        oFile.CloseFile();
    }

    delete[] pData;
    return 0;
}
