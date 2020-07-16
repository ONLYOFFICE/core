#include <iostream>
#include <vector>

#include "../Fb2File.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

void getDirectories(std::wstring sDirectory, std::vector<std::wstring>& arrDirectory)
{
    arrDirectory.push_back(sDirectory);
    for(std::wstring sT : NSDirectory::GetDirectories(sDirectory))
        getDirectories(sT, arrDirectory);
}

int main()
{
    bool bBatchMode = true;
    if(bBatchMode)
    {
        // Директория файлов
        std::wstring sDirectory = L"C:/Users/Kulik/Downloads/fb2";
        // Вложенные директории
        std::vector<std::wstring> arrDirectory;// = NSDirectory::GetDirectories(sDirectory);
        getDirectories(sDirectory, arrDirectory);

        // Параметры конвертации
        CFb2Params* oParams = new CFb2Params[1];
        oParams[0].bNeedDocx = true;
        oParams[0].bNeedContents = true;

        for(std::wstring sD : arrDirectory)
        {
            std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);

            // Директория, где будем создавать docx
            size_t nPos = sD.find(L"/fb2");
            std::wstring sOutputDirectory = sD.insert(nPos + 4, L"-res");
            NSDirectory::DeleteDirectory(sOutputDirectory);
            NSDirectory::CreateDirectory(sOutputDirectory);

            for(std::wstring sFile : arrFiles)
            {
                CFb2File oFile;
                // Выставляем временную директорию
                oFile.SetTmpDirectory(NSFile::GetProcessDirectory() + L"/tmp");
                std::wcout << NSFile::GetFileName(sFile) << std::endl;
                if(!oFile.IsFb2File(sFile))
                {
                    std::cout << "This isn't a fb2 file" << std::endl;
                    continue;
                }
                int nResConvert = oFile.Convert(sFile, sOutputDirectory, oParams);
                if(nResConvert)
                    std::cout << "Success" << std::endl;
                else
                    std::cout << "Failure" << std::endl;
            }


        }

        delete[] oParams;
    }
    else
    {
        CFb2File oFile;
        oFile.SetTmpDirectory(NSFile::GetProcessDirectory() + L"/tmp");

        // Файл, который открываем
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/8-sezon-groz.fb2";

        // Директория, где будем создавать docx
        std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
        NSDirectory::DeleteDirectory(sOutputDirectory);
        NSDirectory::CreateDirectory(sOutputDirectory);

        bool bCheck = oFile.IsFb2File(sFile);
        if (!bCheck)
        {
            std::cout << "This isn't a fb2 file" << std::endl;
            return 1;
        }

        CFb2Params* oParams = new CFb2Params[1];
        oParams[0].bNeedDocx = true;
        oParams[0].bNeedContents = true;

        int nResConvert = oFile.Convert(sFile, sOutputDirectory, oParams);
        if(nResConvert)
            std::cout << "Success" << std::endl;
        else
            std::cout << "Failure" << std::endl;

        delete[] oParams;
    }
    return 0;
}
