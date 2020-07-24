#include <iostream>
#include <fstream>
#include "../../Common/3dParty/html/htmltoxhtml.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"

void readFile( XmlUtils::CXmlLiteReader& oLightReader)
{
    int nDeath = oLightReader.GetDepth();
    while(oLightReader.ReadNextSiblingNode(nDeath))
        readFile(oLightReader);
}

int main()
{
    // Файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test1.html";

    // Директория, где будем создавать xhtml
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    std::string sRes = htmlToXhtml(sFile);

    /*
    NSFile::CFileBinary oXhtmlWriter;
    if (oXhtmlWriter.CreateFileW(sOutputDirectory + L"/res.xhtml"))
    {
        oXhtmlWriter.WriteStringUTF8(sRes);
        oXhtmlWriter.CloseFile();
    }
    */

    // Запись std::string в файл
    std::ofstream out;
    out.open(sOutputDirectory + L"/res.xhtml");
    if(out.is_open())
        out << sRes << std::endl;
    out.close();

    XmlUtils::CXmlLiteReader oLightReader;
    std::string sFileContentUtf8 = XmlUtils::GetXmlContentAsUTF8(sOutputDirectory + L"/res.xhtml");
    bool bRes = oLightReader.FromStringA(sFileContentUtf8);
    if(bRes)
    {
        readFile(oLightReader);
        std::cout << "Success" << std::endl;
    }
    else
        std::cout << "Failure" << std::endl;

    return 0;
}
