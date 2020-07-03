#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include <iostream>

CEpubFile::CEpubFile()
{
}

CEpubFile::~CEpubFile()
{
//    NSDirectory::DeleteDirectory(_tempDir);
}

bool CEpubFile::IsEbubFile(const std::wstring &fileName)
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

void CEpubFile::SetTempDirectory(const std::wstring &tempDir)
{
    this->_tempDir = tempDir;
}

bool CEpubFile::Convert(const std::wstring& inputFile, const std::wstring& outputFile)
{
    NSDirectory::CreateDirectory(_tempDir);
    COfficeUtils OfficeUtils;

    wchar_t* password = NULL;
    if (OfficeUtils.ExtractToDirectory(inputFile, _tempDir.c_str(), password, 1) != S_OK)
        return false;


    XmlUtils::CXmlLiteReader XmlLiteReader;

    if (XmlLiteReader.FromFile(_tempDir + L"/content.opf"))
        while (XmlLiteReader.ReadNextNode())
        {
            std::wcout << XmlLiteReader.GetDepth() << " - " << XmlLiteReader.GetName() << " - " << XmlLiteReader.GetText() << " - " << XmlLiteReader.GetNamespacePrefix() << std::endl;
        }
    else
        return false;


    return true;
}
