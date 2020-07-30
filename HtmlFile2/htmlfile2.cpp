#include <string>
#include <map>

#include "htmlfile2.h"
#include "../Common/3dParty/html/htmltoxhtml.h"
#include "../ASCOfficeDocxFile2/BinReader/FileWriter.h"
#include "../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../DesktopEditor/common/SystemUtils.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/xml/include/xmlutils.h"

class CHtmlFile2_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader;        // SAX Reader
    std::wstring m_sTmp;                            // Temp папка для конфертации html в xhtml
    std::wstring m_sBase;                           // Полный базовый адрес
    std::map<std::wstring, std::wstring> m_mStyles; // Стили в document.xml. Хранятся как (имя тэга, его стиль)

    int m_nImageId; // ID картинки
public:
    CHtmlFile2_Private()
    {
        m_nImageId = 1;
        m_sBase = L"";
    }

    ~CHtmlFile2_Private()
    {
        m_oLightReader.Clear();
        m_mStyles.clear();
    }

    // Проверяет наличие тэга html
    bool isHtml()
    {
        if(!m_oLightReader.ReadNextNode())
            return false;
        if(m_oLightReader.GetName() != L"html")
            return false;
        return true;
    }

    void CreateDocxEmpty(const std::wstring& sDst)
    {
        Writers::FileWriter* pDocxWriter; // Писатель скелета docx
        // FileWriter - Писатель docx
        // sDst - место создания docx, L"" - директория fontTable для инициализации, true - директория fontTable не требуется,
        // 1 - версия стилей, false - не сохранять диаграммы как изображения, NULL - кастомный конвертор связанный с pptx, L"" - пустая тема
        pDocxWriter =  new Writers::FileWriter(sDst, L"", true, 1, false, NULL, L"");
        // Создаем пустые папки
        std::wstring strDirectory = sDst;
        // rels
        OOX::CPath pathRels = strDirectory + FILE_SEPARATOR_STR + L"_rels";
        NSDirectory::CreateDirectory(pathRels.GetPath());
        // docProps
        OOX::CPath pathDocProps = strDirectory + FILE_SEPARATOR_STR + L"docProps";
        NSDirectory::CreateDirectory(pathDocProps.GetPath());
        // word
        OOX::CPath pathWord = strDirectory + FILE_SEPARATOR_STR + L"word";
        NSDirectory::CreateDirectory(pathWord.GetPath());
        // documentRels
        OOX::CPath pathWordRels = pathWord + FILE_SEPARATOR_STR + L"_rels";
        NSDirectory::CreateDirectory(pathWordRels.GetPath());
        // media
        OOX::CPath pathMedia = pathWord + FILE_SEPARATOR_STR + L"media";
        NSDirectory::CreateDirectory(pathMedia.GetPath());
        // theme
        OOX::CPath pathTheme = pathWord + FILE_SEPARATOR_STR + L"theme";
        NSDirectory::CreateDirectory(pathTheme.GetPath());

        // Пишем стандартные файлы
        pathTheme = pathTheme + FILE_SEPARATOR_STR + L"theme1.xml";
        pDocxWriter->m_oTheme.Write(pathTheme.GetPath());
        OOX::CPath DocProps = L"docProps";
        OOX::CContentTypes oContentTypes;
        OOX::CApp* pApp = new OOX::CApp(NULL);
        if (pApp)
        {
            std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
            if (sApplication.empty())
                sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
            pApp->SetApplication(sApplication);
            #if defined(INTVER)
            pApp->SetAppVersion(VALUE2STR(INTVER));
            #endif
            pApp->SetDocSecurity(0);
            pApp->SetScaleCrop(false);
            pApp->SetLinksUpToDate(false);
            pApp->SetSharedDoc(false);
            pApp->SetHyperlinksChanged(false);

            pApp->write(pathDocProps + FILE_SEPARATOR_STR + L"app.xml", DocProps, oContentTypes);
            RELEASEOBJECT(pApp);
        }

        pDocxWriter->m_oTheme.Write(strDirectory);
        pDocxWriter->m_oStylesWriter.Write();
        pDocxWriter->m_oFontTableWriter.Write();
        pDocxWriter->m_oSettingWriter.Write();
        pDocxWriter->m_oWebSettingsWriter.Write();
        pDocxWriter->m_oDocumentWriter.Write();
        pDocxWriter->m_oDocumentRelsWriter.Write();

        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml", OOX::CPath(L"/word"),       OOX::CPath(L"document.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",        OOX::CPath(L"/word"),       OOX::CPath(L"styles.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",      OOX::CPath(L"/word"),       OOX::CPath(L"settings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",   OOX::CPath(L"/word"),       OOX::CPath(L"webSettings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",     OOX::CPath(L"/word"),       OOX::CPath(L"fontTable.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",                          OOX::CPath(L"/word/theme"), OOX::CPath(L"theme1.xml"));
        oContentTypes.Write(strDirectory);

        RELEASEOBJECT(pDocxWriter);
    }

    // Предварительное чтение стилей и картинок
    // sPath - файл после конвертации в xhtml
    bool readSrc(const std::wstring& sPath, const std::wstring& sSrc, const std::wstring& sDst, NSStringUtils::CStringBuilder& oStylesXml, NSStringUtils::CStringBuilder& oDocXmlRels)
    {
        if(!m_oLightReader.IsValid())
        {
            // Открывает файл на проверку
            if (!m_oLightReader.FromFile(sPath))
                return false;
            // Читаем html
            if(!isHtml())
                return false;
        }

        readFile(sSrc, sDst, oStylesXml, oDocXmlRels);

        return true;
    }

    // Читает стили
    // sSrc - директория с исходником до конвертации, относительно которой указываются пути до картинок
    void readFile(const std::wstring& sSrc, const std::wstring& sDst, NSStringUtils::CStringBuilder& oStylesXml, NSStringUtils::CStringBuilder& oDocXmlRels)
    {
        std::wstring sName = m_oLightReader.GetName();
        // тэг style содержит стили для styles.xml
        if(sName == L"style")
        {
            // Получаем наборы стилей как <w:style>...</w:style>...
            std::wstring sStyle = L""; // oCSS.GetStyleXml(content());
            // Дописываем в styles.xml
            oStylesXml += sStyle;
        }
        // Стиль по ссылке
        else if(sName == L"link")
        {
            while(m_oLightReader.MoveToNextAttribute())
            {
                if(m_oLightReader.GetName() == L"href")
                {
                    std::wstring sRef = m_oLightReader.GetText();
                    // Если это css файл, то поведение аналогично тэгу style
                    // Кроме функции получения стилей
                    sRef = NSFile::GetFileExtention(sRef);
                    if(sRef == L"css")
                    {
                        // Получаем наборы стилей как <w:style>...</w:style>...
                        std::wstring sStyle = L""; // oCSS.GetStyleFromCSS(sRef);
                        oStylesXml += sStyle;
                    }
                }
            }
            m_oLightReader.MoveToElement();
        }
        // Стиль по атрибуту
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"style")
            {
                // Получаем стиль как <w:pPr>...</w:pPr> для записи в document.xml
                std::wstring sStyle = L""; // oCSS.GetStyleDoc(m_oLightReader.GetText());

                std::map<std::wstring, std::wstring>::iterator it = m_mStyles.find(sName);
                // Если для тэга уже есть стиль, то получаем среднее
                if(it != m_mStyles.end())
                {
                    it->second = L""; // oCSS.GetStyleCompromise(it->second, sStyle);
                }
                // Если впервые, то сохраняем как - имя тэга в файле и его стиль
                else
                    m_mStyles.insert(std::make_pair(sName, sStyle));
            }
        }
        m_oLightReader.MoveToElement();

        // Картинки
        if(sName == L"img" || sName == L"image")
            readImage(sSrc, sDst + L"/word/media/", oDocXmlRels);
        // Заголовок документа
        else if(sName == L"title")
            readTitle(sDst);
        // Базовый адрес
        else if(sName == L"base")
        {
            while(m_oLightReader.MoveToNextAttribute())
            {
                if(m_oLightReader.GetName() == L"href")
                {
                    m_sBase = m_oLightReader.GetText();
                }
            }
            m_oLightReader.MoveToElement();
        }
        // Ссылки
        else if(sName == L"a")
        {
            while(m_oLightReader.MoveToNextAttribute())
            {
                if(m_oLightReader.GetName() == L"href")
                {
                    std::wstring sRef = m_oLightReader.GetText();
                    size_t nLen = (sRef.length() > 4 ? 4 : 0);
                    // Ссылка на сайт
                    if(sRef.substr(0, nLen) == L"http")
                    {

                    }
                    // Ссылка на документ, который нужно обработать
                    else
                    {

                    }
                }
            }
            m_oLightReader.MoveToElement();
        }
        // Абзац текста. Содержит фразовый контент
        else if(sName == L"p")
        {
            readP();
            return;
        }

        // Читаем весь файл
        if(m_oLightReader.IsEmptyNode())
            return;
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
            readFile(sSrc, sDst, oStylesXml, oDocXmlRels);
    }

    void htmlXhtml(const std::wstring& sSrc)
    {
        NSFile::CFileBinary oXhtmlWriter;
        if (oXhtmlWriter.CreateFileW(m_sTmp + L"/res.xhtml"))
        {
            // htmlToXhtml возвращает текст файла в кодировке UTF-8
            oXhtmlWriter.WriteStringUTF8(htmlToXhtml(sSrc));
            oXhtmlWriter.CloseFile();
        }
    }
private:
    void readImage(const std::wstring& sSrc, const std::wstring& sMedia, NSStringUtils::CStringBuilder& oDocXmlRels)
    {
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sAName = m_oLightReader.GetName();
            if(sAName == L"src" || sAName == L"href")
            {
                std::wstring sSrcM = m_oLightReader.GetText();
                size_t nLen = (sSrcM.length() > 4 ? 4 : 0);
                // Картинка Base64
                if(sSrcM.substr(0, nLen) == L"data")
                {
                    size_t nBase = sSrcM.find(L"/", nLen) + 1;
                    std::wstring sType = sSrcM.substr(nBase, sSrcM.find(L";", nBase) - nBase);
                    NSFile::CFileBinary oImageWriter;
                    if(oImageWriter.CreateFileW(sMedia + std::to_wstring(m_nImageId) + L"." + sType))
                    {
                        size_t nBase = sSrcM.find(L"base64", nLen) + 7;
                        std::string sBase64 = m_oLightReader.GetTextA().substr(nBase);
                        int nSrcLen = (int)sBase64.length();
                        int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
                        BYTE* pImageData = new BYTE[nDecodeLen];
                        if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                            oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
                        RELEASEARRAYOBJECTS(pImageData);
                        oImageWriter.CloseFile();
                        // Прописать рельсы
                        oDocXmlRels += L"<Relationship Id=\"rPic";
                        oDocXmlRels += std::to_wstring(m_nImageId);
                        oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                        oDocXmlRels += std::to_wstring(m_nImageId++) + L"." + sType;
                        oDocXmlRels += L"\"/>";
                    }
                }
                // Картинка в сети
                else if(sSrcM.substr(0, nLen) == L"http" || !m_sBase.empty())
                {
                    CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
                    oDownloadImg.SetFilePath(sMedia + NSFile::GetFileName(sSrcM));
                    bool bRes = oDownloadImg.DownloadSync();
                    // Прописать рельсы
                    if(bRes)
                    {
                        oDocXmlRels += L"<Relationship Id=\"rPic";
                        oDocXmlRels += std::to_wstring(m_nImageId++);
                        oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                        oDocXmlRels += NSFile::GetFileName(sSrcM);
                        oDocXmlRels += L"\"/>";
                    }
                }
                // Картинка по относительному пути
                else
                {
                    size_t nSrcM = sSrcM.rfind(L"/") + 1;
                    bool bRes = NSFile::CFileBinary::Copy(sSrc + L"/" + sSrcM, sMedia + sSrcM.substr(nSrcM));
                    // Прописать рельсы
                    if(bRes)
                    {
                        oDocXmlRels += L"<Relationship Id=\"rPic";
                        oDocXmlRels += std::to_wstring(m_nImageId++);
                        oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                        oDocXmlRels += sSrcM.substr(nSrcM);
                        oDocXmlRels += L"\"/>";
                    }
                }
            }
        }
        m_oLightReader.MoveToElement();
    }

    void readTitle(const std::wstring& sDst)
    {
        OOX::CPath DocProps = L"docProps";
        OOX::CContentTypes oContentTypes;
        OOX::CCore* pCore = new OOX::CCore(NULL);
        if (pCore)
        {
            pCore->m_sTitle = content();
            pCore->SetCreator(L"");
            pCore->SetLastModifiedBy(L"");
            pCore->write(OOX::CPath(sDst + L"/docProps/core.xml"), DocProps, oContentTypes);
            RELEASEOBJECT(pCore);
        }
    }

    void readP()
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
            readP();
    }

    std::wstring content()
    {
        std::wstring sRes = L"";
        if(m_oLightReader.IsEmptyNode())
            return sRes;
        if(m_oLightReader.ReadNextSiblingNode2(m_oLightReader.GetDepth()))
            sRes = m_oLightReader.GetText();
        return sRes;
    }

};

CHtmlFile2::CHtmlFile2()
{
    m_internal = new CHtmlFile2_Private();
}

CHtmlFile2::~CHtmlFile2()
{
    RELEASEOBJECT(m_internal);
}

bool CHtmlFile2::IsHtmlFile(const std::wstring& sFile)
{
    m_internal->htmlXhtml(sFile);

    // Открывает файл на проверку
    if (!m_internal->m_oLightReader.FromFile(m_internal->m_sTmp + L"/res.xhtml"))
        return false;
    // Читаем html
    if(!m_internal->isHtml())
        return false;
    return true;
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmp = sFolder;
}

HRESULT CHtmlFile2::Open(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    m_internal->htmlXhtml(sSrc);

    std::wstring sSource = m_internal->m_sTmp + L"/res.xhtml";



    m_internal->CreateDocxEmpty(sDst);

    NSStringUtils::CStringBuilder oStylesXml;  // styles.xml
    NSStringUtils::CStringBuilder oDocXmlRels; // document.xml.rels
    oDocXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
    oDocXmlRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";

    std::wstring sSrcFolder = NSSystemPath::GetDirectoryName(sSrc);
    if(!m_internal->readSrc(sSource, sSrcFolder, sDst, oStylesXml, oDocXmlRels))
        return S_FALSE;

    oDocXmlRels += L"</Relationships>";
    NSFile::CFileBinary oRelsWriter;
    if (oRelsWriter.CreateFileW(sDst + L"/word/_rels/document.xml.rels"))
    {
        oRelsWriter.WriteStringUTF8(oDocXmlRels.GetData());
        oRelsWriter.CloseFile();
    }

    return S_OK;
}
