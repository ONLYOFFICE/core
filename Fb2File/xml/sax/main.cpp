#include <iostream>
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/File.h"

void read(XmlUtils::CXmlLiteReader& oLightReader)
{
    std::wcout << "tag: "         << oLightReader.GetName()
               << "\tdepth: "     << oLightReader.GetDepth();
    while(oLightReader.MoveToNextAttribute())
    {
        std::wcout << "\tattribute: " << oLightReader.GetName()
                   << "\ttext: "     << oLightReader.GetText();
    }
    oLightReader.MoveToElement();
    int nDepth = oLightReader.GetDepth();
    while(oLightReader.ReadNextSiblingNode2(nDepth))
    {
        std::wstring sName = oLightReader.GetName();
        if(sName == L"#text")
        {
            std::wcout << "\ttext: " << oLightReader.GetText();
        }
        else
        {
            std::wcout << std::endl;
            read(oLightReader);
        }
    }
    std::wcout << std::endl;
}

int main()
{
    setlocale(LC_ALL, "Russian");
    XmlUtils::CXmlLiteReader oLightReader;
    if (oLightReader.FromFile(NSFile::GetProcessDirectory() + L"/../../../examples/test.xml"))
    {
        while(oLightReader.ReadNextNode())
        {
            read(oLightReader);
        }
    }
    else
    {
        std::cout << "File don't open" << std::endl;
    }

    return 0;
}
