#include <iostream>

#include "../Fb2File.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

int main()
{
    CFb2File oFile;
    oFile.SetTmpDirectory(NSFile::GetProcessDirectory() + L"/tmp");

    // файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/8-sezon-groz.fb2";

    // директория, где будем создавать docx
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    if (!NSDirectory::Exists(sOutputDirectory))
        NSDirectory::CreateDirectory(sOutputDirectory);

    bool bCheck = oFile.IsFb2File(sFile);
    if (!bCheck)
    {
        std::cout << "This isn't a fb2 file" << std::endl;
        return 1;
    }

    int nResConvert = oFile.Convert(sFile, sOutputDirectory, true);
    if(nResConvert)
        std::cout << "Success" << std::endl;
    else
        std::cout << "Failure" << std::endl;
    return 0;
}
