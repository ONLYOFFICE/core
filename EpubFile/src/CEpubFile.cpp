#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

CEpubFile::CEpubFile()
{
}

CEpubFile::~CEpubFile()
{
    NSDirectory::DeleteDirectory(_tempDir);
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
    {
        /*
        // читаем ноду
        XmlLiteReader.ReadNextNode();

        if (true)
        {
            // вот как пробежаться по всей ноде:

            // читаем ее атрибуты, если нужно
            if (XmlLiteReader.GetAttributesCount() > 0 &&
                XmlLiteReader.MoveToFirstAttribute())
            {
                std::wstring sAttributeName = XmlLiteReader.GetName();

                while (!sAttributeName.empty())
                {
                    // обработка атрибута
                    std::wstring sAttributeValue = XmlLiteReader.GetText();

                    if (!XmlLiteReader.MoveToNextAttribute())
                        break;
                    sAttributeName = XmlLiteReader.GetName();
                }

                XmlLiteReader.MoveToElement();
            }

            if (!XmlLiteReader.IsEmptyNode())
            {
                int nParentDepth = XmlLiteReader.GetDepth();
                while( XmlLiteReader.ReadNextSiblingNode( nParentDepth ) )
                {
                    std::wstring sName = XmlLiteReader.GetName();

                    // и тут так же читаем всю ноду
                }
            }
        }

        чтение sax - ридером не так удобно, как dom, но он быстрее.
        за один проход надо все прочитать.
        */

        while (XmlLiteReader.ReadNextNode())
        {
            if (XmlLiteReader.GetNamespacePrefix() == L"dc")
            {
                std::wstring name = XmlLiteReader.GetName();
                std::wstring value = XmlLiteReader.GetText2();
                dataAboutFile[name.substr(3, name.length())] = value;
            }
        }
    }
    else
        return false;

    return true;
}
