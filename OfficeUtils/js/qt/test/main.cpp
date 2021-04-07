#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../wasm/src/base.h"

#include <string>
#include <iostream>

int main()
{
    std::string pData;
    NSFile::CFileBinary oFile;
    if (!oFile.ReadAllTextUtf8A(NSFile::GetProcessDirectory() + L"/test.zip", pData))
        return 1;
    oFile.CloseFile();

    unsigned char* res = Zlib_GetFile(pData, L"a/b/c/1.txt");

    if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/res.txt"))
    {
        oFile.WriteStringUTF8(std::wstring((wchar_t*)res));
        oFile.CloseFile();
    }

    return 0;
}
