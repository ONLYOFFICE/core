#include "../CEpubFile.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    CEpubFile Epub;
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../Files/Harry Potter 1.epub";
    std::wstring sTmp = NSFile::GetProcessDirectory() + L"/tmp";
    std::wstring sOutput = NSFile::GetProcessDirectory() + L"/output";

    if (Epub.IsEbubFile(sFile))
    {
        Epub.SetTempDirectory(sTmp);
        Epub.Convert(sFile, sOutput);
    }
    return 0;
}
