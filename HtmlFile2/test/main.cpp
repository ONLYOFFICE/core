#include <iostream>
#include <fstream>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

int main()
{
    CHtmlFile2 oFile;

    // Файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test2.html";

    // Директория, где будем создавать xhtml
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    bool bCheck = true; //oFile.IsHtmlFile(sFile);
    //if (!bCheck)
    if (bCheck)
    {
        std::cout << "This isn't a html file" << std::endl;
        return 1;
    }

    HRESULT nResConvert = S_OK; //oFile.Open(sFile, sOutputDirectory);
    if(nResConvert == S_OK)
        std::cout << "Success" << std::endl;
    else
        std::cout << "Failure" << std::endl;

    std::cout << "THE END" << std::endl;
    return 0;
}
