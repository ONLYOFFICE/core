#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/Debug/";

    CHtmlFile oFile;

    int nResult = oFile.Convert(L"xml", sPath);

    return 0;
}
