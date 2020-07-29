#include <iostream>
#include "../htmlfile2.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../Common/3dParty/html/htmltoxhtml.h"
#include "../../ASCOfficeDocxFile2/BinReader/FileWriter.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../../DesktopEditor/common/SystemUtils.h"

int main()
{
    CHtmlFile2 oFile;

    // Файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test2.html";

    // Директория, где будем создавать xhtml
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    bool bCheck = oFile.IsHtmlFile(sFile);
    if (!bCheck)
    {
        std::cout << "This isn't a html file" << std::endl;
        return 1;
    }

    HRESULT nResConvert = oFile.Open(sFile, sOutputDirectory);
    if(nResConvert == S_OK)
        std::cout << "Success" << std::endl;
    else
        std::cout << "Failure" << std::endl;

    std::cout << "THE END" << std::endl;
    return 0;
}
