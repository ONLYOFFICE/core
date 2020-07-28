#include <iostream>
#include <fstream>
#include "../../Common/3dParty/html/htmltoxhtml.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

void readFile( XmlUtils::CXmlLiteReader& oLightReader)
{
    int nDepth = oLightReader.GetDepth();
    while(oLightReader.ReadNextSiblingNode(nDepth))
        readFile(oLightReader);
}

int main()
{
    // Файл, который открываем
    std::wstring sFile = NSFile::GetProcessDirectory() + L"/../../../examples/test2.html";

    // Директория, где будем создавать xhtml
    std::wstring sOutputDirectory = NSFile::GetProcessDirectory() + L"/res";
    NSDirectory::DeleteDirectory(sOutputDirectory);
    NSDirectory::CreateDirectory(sOutputDirectory);

    NSFile::CFileBinary oXhtmlWriter;
    if (oXhtmlWriter.CreateFileW(sOutputDirectory + L"/res.xhtml"))
    {
        // htmlToXhtml возвращает текст файла в кодировке UTF-8
        oXhtmlWriter.WriteStringUTF8(htmlToXhtml(sFile));
        oXhtmlWriter.CloseFile();
    }

    // Проверка на чтение
    XmlUtils::CXmlLiteReader oLightReader;
    bool bRes = oLightReader.FromFile(sOutputDirectory + L"/res.xhtml");
    if(bRes)
    {
        readFile(oLightReader);
        std::cout << "Success" << std::endl;
    }
    else
        std::cout << "Failure" << std::endl;

    return 0;
}
