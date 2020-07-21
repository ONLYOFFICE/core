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
    bool bBatchMode = false;
    if(bBatchMode)
    {
        // Директория файлов
        std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/../../../examples/fb2";
        // Вложенные директории
        std::vector<std::wstring> arrDirectory;
        getDirectories(sDirectory, arrDirectory);

        // Параметры конвертации
        CFb2Params* oParams = new CFb2Params[1];
        oParams[0].bNeedDocx = true;
        oParams[0].bNeedContents = true;

        int nErrorCol = 0;
         std::vector<std::wstring> arrError;

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
                std::wstring sFileName = NSFile::GetFileName(sFile);
                std::wcout << sFileName << std::endl;
                if(!oFile.IsFb2File(sFile))
                {
                    nErrorCol++;
                    arrError.push_back(sFileName);
                    std::cout << "This isn't a fb2 file" << std::endl;
                    continue;
                }
                HRESULT nResConvert = oFile.Open(sFile, sOutputDirectory, oParams);
                if(nResConvert == S_OK)
                    std::cout << "Success" << std::endl;
                else
                {
                    nErrorCol++;
                    arrError.push_back(sFileName);
                    std::cout << "Failure" << std::endl;
                }
            }
        }
        delete[] oParams;

        std::cout << "ERRORS - "<< nErrorCol << std::endl;
        for(std::wstring sError : arrError)
            std::wcout << sError << std::endl;
    }
    else
    {
        CFb2File oFile;
        oFile.SetTmpDirectory(NSFile::GetProcessDirectory() + L"/tmp");

        // Файл, который открываем
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test1.fb2";

        // Директория, где будем создавать docx
        std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";

        bool bCheck = oFile.IsFb2File(sFile);
        if (!bCheck)
        {
            std::cout << "This isn't a fb2 file" << std::endl;
            return 1;
        }

        CFb2Params oParams;
        oParams.bNeedDocx = true;
        oParams.bNeedContents = true;

        HRESULT nResConvert = oFile.Open(sFile, sOutputDirectory, &oParams);
        if(nResConvert == S_OK)
            std::cout << "Success" << std::endl;
        else
            std::cout << "Failure" << std::endl;
    }
    std::cout << "THE END" << std::endl;
    return 0;
}
