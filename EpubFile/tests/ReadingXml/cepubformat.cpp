#include "cepubformat.h"

CEpubFormat::CEpubFormat()
{

}

CEpubFormat::~CEpubFormat()
{

}

bool CEpubFormat::IsEbubFile(const std::wstring& fileName)
{
    COfficeUtils OfficeUtils;

    if (OfficeUtils.IsArchive(fileName) == S_OK &&
        OfficeUtils.IsFileExistInArchive(fileName, L"META-INF/container.xml") == S_OK)
    {
        this->_fileName = fileName;
        return true;
    }
    return false;
}

void CEpubFormat::SetTempDirectory(const std::wstring& tempDir)
{
    this->_tempDir = tempDir;

    COfficeUtils OfficeUtils;
    wchar_t *password;

    OfficeUtils.ExtractToDirectory(_fileName, _tempDir, password, 1);
}

void CEpubFormat::Convert()
{
    XmlUtils::CXmlLiteReader XmlLiteReader;

    XmlLiteReader.FromFile(this->_tempDir + L"/content.opf");

    while (XmlLiteReader.ReadNextNode())
    {

        std::wcout <<XmlLiteReader.GetDepth() << " " << XmlLiteReader.GetName() << " - " << XmlLiteReader.GetText() << std::endl;
    }


//    for (auto item = dataAboutFile.begin(); item != dataAboutFile.end(); item++)
//        std::wcout << item->first << " - " << item->second << std::endl;
}
