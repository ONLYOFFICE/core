#include "../CEpubFile.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"
#include "../../HtmlFile2/htmlfile2.h"
#include "src/CBookInfo.h"
#include <iostream>

CEpubFile::CEpubFile()
{
}

CEpubFile::~CEpubFile()
{
    NSDirectory::DeleteDirectory(m_sTempDir);
}

HRESULT CEpubFile::IsEbubFile(const std::wstring &sFileName)
{
    if (sFileName.find_last_of(L'.') == std::wstring::npos ||
        sFileName.substr(sFileName.find_last_of(L'.') + 1) != L"epub")
        return S_FALSE;

    COfficeUtils oOfficeUtils;
    if (oOfficeUtils.IsArchive(sFileName) == S_OK &&
        oOfficeUtils.IsFileExistInArchive(sFileName, L"META-INF/container.xml") == S_OK)
    {
        this->m_sFileName = sFileName;
        return S_OK;
    }
    return S_FALSE;
}

void CEpubFile::SetTempDirectory(const std::wstring &sTempDir)
{
    this->m_sTempDir = sTempDir;
}

HRESULT CEpubFile::Convert(const std::wstring& sInputFile, const std::wstring& sOutputFile)
{

    NSDirectory::CreateDirectories(m_sTempDir);
//    NSDirectory::CreateDirectory(sOutputFile);
    COfficeUtils oOfficeUtils;

    wchar_t* password = NULL;
    if (oOfficeUtils.ExtractToDirectory(sInputFile, m_sTempDir.c_str(), password, 1) != S_OK)
        return S_FALSE;

    XmlUtils::CXmlLiteReader oXmlLiteReader;

    if (oXmlLiteReader.FromFile(m_sTempDir + L"/content.opf"))
    {
        oXmlLiteReader.ReadNextNode();

        int nParentDepth = oXmlLiteReader.GetDepth();
        while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
        {
            std::wstring sName = oXmlLiteReader.GetName();
            if (sName == L"metadata")
            {
                m_oBookInfo.ReadInfo(oXmlLiteReader);
                #ifdef _DEBUG
                    m_oBookInfo.ShowInfo();
                #endif
            }
            else if (sName == L"manifest")
            {
                int _nParentDepth = oXmlLiteReader.GetDepth();
                while (true)
                {
                    CBookItem oItem;
                    if (oItem.ReadItem(oXmlLiteReader, _nParentDepth))
                        m_mapRefs[oItem.GetID()] = oItem;
                    else
                        break;
                }
            }
            else if (sName == L"spine")
            {
                int _nParentDepth = oXmlLiteReader.GetDepth();
                while (true)
                {
                    CBookContentItem oContentItem;
                    if (oContentItem.ReadContentItem(oXmlLiteReader, _nParentDepth))
                        m_arContents.push_back(oContentItem);
                    else
                        break;
                }
            }
        }
    }
    else
        return S_FALSE;

    if (oXmlLiteReader.FromFile(m_sTempDir + L"\\toc.ncx"))
    {
        oXmlLiteReader.ReadNextNode();
        m_oToc.ReadToc(oXmlLiteReader);
        #ifdef _DEBUG
            m_oToc.ShowToc();
        #endif

//        CDocxFile oDocxFile;
//        oDocxFile.CreateTempFiles(sOutputFile, m_sTempDir);

//        oDocxFile.AddBookToc(&m_oToc);

//        std::wstring sTempDir = m_sTempDir + L"/docx";
//        std::wstring _sOutputFile = sOutputFile + L"/test.docx";

//        NSFile::CFileBinary oFileBinary;
//        oFileBinary.CreateFileW(_sOutputFile);
//        oFileBinary.CloseFile();

//        oDocxFile.SaveToFile();

        CHtmlFile2 oFile;
        CHtmlParams oFileParams;

        if (!m_oBookInfo.GetCreators().empty())
            oFileParams.AddAuthor(m_oBookInfo.GetCreators());
        if (!m_oBookInfo.GetSubjects().empty())
            oFileParams.AddGenre(m_oBookInfo.GetSubjects());
        if (!m_oBookInfo.GetTitle().empty())
            oFileParams.SetTitle(m_oBookInfo.GetTitle());
        if (!m_oBookInfo.GetDate().empty())
            oFileParams.SetDate(m_oBookInfo.GetDate());
        if (!m_oBookInfo.GetDescriptions().empty())
            oFileParams.SetDescription(m_oBookInfo.GetDescriptions());


        std::wstring sDocxFileTempDir = m_sTempDir + L"\\res";
        NSDirectory::CreateDirectory(sDocxFileTempDir);

        oFile.SetTmpDirectory(sDocxFileTempDir);

        std::vector<std::wstring> arFiles;

        for (size_t i = 0; i < m_arContents.size(); i++)
            arFiles.push_back(m_sTempDir + L"\\" + m_mapRefs[m_arContents[i].m_sID].GetRef());


        #ifdef _DEBUG
            std::wcout << L"---The conversion process from Epub to Docx...---" << std::endl;
            if (oFile.OpenBatch(arFiles, sDocxFileTempDir, &oFileParams) == S_OK)
                std::wcout << L"---Successful conversion of Epub to Docx---" << std::endl;
            else
                std::wcout << L"---Failed conversion of Epub to Docx---" << std::endl;
        #endif

        oOfficeUtils.CompressFileOrDirectory(sDocxFileTempDir, sOutputFile);
    }

    return S_OK;
}

void CEpubFile::Clear()
{
    m_sFileName.clear();
    m_sTempDir.clear();
    m_oBookInfo.Clear();
    m_mapRefs.clear();
    m_oToc.Clear();
    m_arContents.clear();
    NSDirectory::DeleteDirectory(m_sTempDir);
}

void CEpubFile::ShowMap()
{
    std::cout << "-----MAP-----" << std::endl;
    for (size_t i = 0; i < m_arContents.size(); i++)
        std::wcout << m_arContents[i].m_sID << " - " << m_mapRefs[m_arContents[i].m_sID].GetRef() << std::endl;
}
