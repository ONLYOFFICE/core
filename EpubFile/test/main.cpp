#include <Windows.h>
#include "../CEpubFile.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"Russian");
    SetConsoleOutputCP( CP_UTF8 );

    std::vector<std::wstring> arFiles = NSDirectory::GetFiles(NSFile::GetProcessDirectory() + L"\\..\\..\\..\\Files");
    std::wstring sTmp = NSFile::GetProcessDirectory() + L"\\tmp";
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"\\OutputFiles";
    NSDirectory::CreateDirectory(sOutputDirectory);

    for (int i = 0; i < (int)arFiles.size(); i++)
    {
        std::wstring sFileName = arFiles[i];

        CEpubFile *oEpub = new CEpubFile();

        if (oEpub->IsEbubFile(sFileName) == S_OK)
        {
            std::wstring sFile = sFileName.substr(0, sFileName.find_last_of(L'.'));
            if (sFile.find(L'\\') != std::wstring::npos)
                sFile = sFile.substr(sFile.find_last_of(L'\\') + 1);

            std::wcout << L"|----------|" << sFile << L"|----------|" << std::endl;

            oEpub->SetTempDirectory(sTmp);
            oEpub->Convert(sFileName, sOutputDirectory + L"\\" + sFile + L".docx");
            oEpub->ShowMap();
        }
        else
            std::wcout << sFileName << L" this is not an epub format!" << std::endl;
    }

    return 0;
}
