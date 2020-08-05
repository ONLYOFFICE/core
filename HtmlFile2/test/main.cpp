#include <iostream>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

int main()
{
    CHtmlFile2 oFile;

    // Файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test4.xhtml";

    // Директория, где будем создавать xhtml
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    oFile.SetTmpDirectory(sOutputDirectory);

    HRESULT nResConvert = oFile.Open(sFile, sOutputDirectory);
    if(nResConvert == S_OK)
    {
        std::cout << "Success" << std::endl;
        COfficeUtils oZip;
        oZip.CompressFileOrDirectory(sOutputDirectory, sOutputDirectory + L"/" + NSFile::GetFileName(sFile) + L".docx");
    }
    else
        std::cout << "Failure" << std::endl;

    std::cout << "THE END" << std::endl;
    return 0;
}
