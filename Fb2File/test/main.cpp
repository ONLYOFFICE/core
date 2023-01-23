#include <iostream>
#include <vector>

#include "../Fb2File.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

void getDirectories(const std::wstring& sDirectory, std::vector<std::wstring>& arrDirectory)
{
    arrDirectory.push_back(sDirectory);
    for (const std::wstring& sT : NSDirectory::GetDirectories(sDirectory))
        getDirectories(sT, arrDirectory);
}

int main()
{
    bool bBatchMode = false;
    bool bFromHtml  = true;
    if (bBatchMode)
    {
        // Директория файлов
        std::wstring sDirectory = NSFile::GetProcessDirectory() + L"/../../../examples/fb2";
        // Вложенные директории
        std::vector<std::wstring> arrDirectory;
        getDirectories(sDirectory, arrDirectory);

        // Параметры конвертации
        CFb2Params oParams;
        oParams.bNeedDocx = false;
        oParams.bNeedContents = true;
        COfficeUtils oZip;
        // Выставляем временную директорию
        std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
        NSDirectory::DeleteDirectory(sTmp);
        NSDirectory::CreateDirectory(sTmp);

        int nErrorCol = 0;
        std::vector<std::wstring> arrError;

        for (std::wstring sD : arrDirectory)
        {
            if (bFromHtml)
            {
            }
            else
            {
                std::vector<std::wstring> arrFiles = NSDirectory::GetFiles(sD);

                // Директория, где будем создавать docx
                size_t nPos = sD.find(L"/fb2");
                sD.insert(nPos + 4, L"-res");
                NSDirectory::DeleteDirectory(sD);
                NSDirectory::CreateDirectory(sD);

                for(const std::wstring& sFile : arrFiles)
                {
                    CFb2File oFile;
                    std::wstring sFileName = NSFile::GetFileName(sFile);
                    std::wcout << sFileName << std::endl;
                    if(!oFile.IsFb2File(sFile))
                    {
                        nErrorCol++;
                        arrError.push_back(sFileName);
                        std::cout << "This isn't a fb2 file" << std::endl;
                        continue;
                    }

                    if(oFile.Open(sFile, sTmp, &oParams) == S_OK)
                    {
                        std::cout << "Success" << std::endl;
                        oZip.CompressFileOrDirectory(sTmp, sD + L"/" + sFileName + L".docx");
                        NSDirectory::DeleteDirectory(sTmp + L"/word/media");
                    }
                    else
                    {
                        nErrorCol++;
                        arrError.push_back(sFileName);
                        std::cout << "Failure" << std::endl;
                    }
                }
            }
        }

        std::cout << "ERRORS - "<< nErrorCol << std::endl;
        for(const std::wstring& sError : arrError)
            std::wcout << sError << std::endl;
    }
    else
    {
        CFb2File oFile;

        // Файл, который открываем
        std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/res.fb2";

        // Директория, где будем создавать docx
        std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
        NSDirectory::DeleteDirectory(sOutputDirectory);
        NSDirectory::CreateDirectory(sOutputDirectory);

        if (bFromHtml)
        {
            sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test.html";
            oFile.FromHtml(sFile, sOutputDirectory + L"/res.fb2", L"Test Title");
            return 0;
        }

        if (!oFile.IsFb2File(sFile))
        {
            std::cout << "This isn't a fb2 file" << std::endl;
            return 1;
        }

        CFb2Params oParams;
        oParams.bNeedDocx = true;
        oParams.bNeedContents = true;

        std::cout << (oFile.Open(sFile, sOutputDirectory, &oParams) == S_OK ? "Success" : "Failure") << std::endl;
    }
    std::cout << "THE END" << std::endl;
    return 0;
}
