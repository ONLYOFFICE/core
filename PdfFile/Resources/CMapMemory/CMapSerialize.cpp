#include "../../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"

#include <vector>

int main()
{
    std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"/../../CMap/CMap");

    NSWasm::CData oRes;
    for (const std::wstring& sFile : arrFiles)
    {
        BYTE* pData = NULL;
        DWORD nSize;
        NSFile::CFileBinary oFile;
        if (oFile.ReadAllBytes(sFile, &pData, nSize))
        {
            std::wstring sFileName = NSFile::GetFileName(sFile);
            BYTE* pStr = NULL;
            LONG nStrSize;
            NSFile::CUtf8Converter::GetUtf8StringFromUnicode(sFileName.c_str(), (LONG)sFileName.length(), pStr, nStrSize);
            oRes.WriteString(pStr, nStrSize);
            RELEASEARRAYOBJECTS(pStr);

            oRes.AddInt(nSize);
            oRes.Write(pData, nSize);

            oFile.CloseFile();
        }

        RELEASEARRAYOBJECTS(pData);
    }

    NSFile::CFileBinary oFile;
	if (oFile.CreateFileW(NSFile::GetProcessDirectory() + L"/../cmap.bin"))
    {
        oFile.WriteFile(oRes.GetBuffer(), oRes.GetSize());
        oFile.CloseFile();
    }

    oRes.Clear();
    return 0;
}
