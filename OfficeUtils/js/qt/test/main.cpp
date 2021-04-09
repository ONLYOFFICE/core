#include "../../../../DesktopEditor/common/Types.h"
#include "../../../../DesktopEditor/common/File.h"
#include "../../wasm/src/base.h"

#include <string>
#include <vector>

int main()
{
    DWORD nBytesCount;
    BYTE* pData;
    NSFile::CFileBinary oFile;
    if (oFile.ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.zip", &pData, nBytesCount))
        oFile.CloseFile();

    std::vector<std::wstring> paths = Zlib_GetPaths(pData, nBytesCount);
    for (std::wstring& path : paths)
    {
        myFile* res = Zlib_GetFile(pData, nBytesCount, path.c_str());

        if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L'/' + NSFile::GetFileName(path)))
        {
            if (res->data)
                oFile.WriteFile(res->data, res->size);
            oFile.CloseFile();
        }

        delete[] res->data;
        delete res;
    }

    delete[] pData;
    return 0;
}
