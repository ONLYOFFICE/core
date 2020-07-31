#include <string>
#include <map>
#include <algorithm>

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

    int m_nImageId;    // ID картинки
    int m_nFootnoteId; // ID сноски
public:
    CHtmlFile2_Private()
    {
        m_nImageId = 1;
        m_nFootnoteId = 1;
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
        pDocxWriter->m_oDocumentRelsWriter.Write();

        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml", OOX::CPath(L"/word"),       OOX::CPath(L"document.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",        OOX::CPath(L"/word"),       OOX::CPath(L"styles.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",      OOX::CPath(L"/word"),       OOX::CPath(L"settings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",   OOX::CPath(L"/word"),       OOX::CPath(L"webSettings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",     OOX::CPath(L"/word"),       OOX::CPath(L"fontTable.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",                          OOX::CPath(L"/word/theme"), OOX::CPath(L"theme1.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-package.core-properties+xml",                       OOX::CPath(L"/docProps"),   OOX::CPath(L"core.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml",     OOX::CPath(L"/word"),       OOX::CPath(L"footnotes.xml"));
        oContentTypes.Write(strDirectory);

        NSFile::CFileBinary oFootnotesWriter;
        if (oFootnotesWriter.CreateFileW(strDirectory + L"/word/_rels/footnotes.xml.rels"))
        {
            oFootnotesWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"></Relationships>");
            oFootnotesWriter.CloseFile();
        }

        RELEASEOBJECT(pDocxWriter);
    }

    // Предварительное чтение стилей и картинок
    // sPath - файл после конвертации в xhtml
    bool readSrc(const std::wstring& sPath, const std::wstring& sSrc, const std::wstring& sDst, NSStringUtils::CStringBuilder& oStylesXml, NSStringUtils::CStringBuilder& oDocXmlRels, NSStringUtils::CStringBuilder& oDocXml, NSStringUtils::CStringBuilder& oNoteXml)
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

        readFile(sSrc, sDst, oStylesXml, oDocXmlRels, oDocXml, oNoteXml);

        return true;
    }

    // Читает файл
    // sSrc - директория с исходником до конвертации, относительно которой указываются пути
    void readFile(const std::wstring& sSrc, const std::wstring& sDst, NSStringUtils::CStringBuilder& oStylesXml, NSStringUtils::CStringBuilder& oDocXmlRels, NSStringUtils::CStringBuilder& oDocXml, NSStringUtils::CStringBuilder& oNoteXml)
    {
        if(readStyle(oStylesXml))
            return;

        std::wstring sName = m_oLightReader.GetName();
        // Картинки
        if(sName == L"img" || sName == L"image")
        {
            oDocXml += L"<w:p>";
            readImage(sSrc, sDst + L"/word/media/", oDocXml, oDocXmlRels);
            oDocXml += L"</w:p>";
        }
        // Заголовок документа
        else if(sName == L"title")
            readTitle(sDst);
        // Базовый адрес
        else if(sName == L"base")
        {
            while(m_oLightReader.MoveToNextAttribute())
                if(m_oLightReader.GetName() == L"href")
                    m_sBase = m_oLightReader.GetText();
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
            oDocXml += L"<w:p>";
            readP(sSrc, sDst, sName, L"", false, oDocXmlRels, oDocXml, oNoteXml);
            oDocXml += L"</w:p>";
            return;
        }

        // Читаем весь файл
        if(m_oLightReader.IsEmptyNode())
            return;
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
            readFile(sSrc, sDst, oStylesXml, oDocXmlRels, oDocXml, oNoteXml);
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
    bool readStyle(NSStringUtils::CStringBuilder& oStylesXml)
    {
        bool isStyle = false;
        std::wstring sName = m_oLightReader.GetName();
        // тэг style содержит стили для styles.xml
        if(sName == L"style")
        {
            isStyle = true;
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
                        isStyle = true;
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
                // Получаем стиль как внутренность <w:pPr>...</w:pPr> или внутренность <w:rPr>...</w:rPr> для записи в document.xml
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
        return isStyle;
    }

    void readImage(const std::wstring& sSrc, const std::wstring& sMedia, NSStringUtils::CStringBuilder& oDocXml, NSStringUtils::CStringBuilder& oDocXmlRels)
    {
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sAName = m_oLightReader.GetName();
            if(sAName == L"src" || sAName == L"href")
            {
                bool bRes = false;
                std::wstring sSrcM = m_oLightReader.GetText();
                std::wstring sImageName = L"";
                std::wstring sImageId = std::to_wstring(m_nImageId);
                size_t nLen = (sSrcM.length() > 4 ? 4 : 0);
                // Картинка Base64
                if(sSrcM.substr(0, nLen) == L"data")
                {
                    size_t nBase = sSrcM.find(L"/", nLen) + 1;
                    std::wstring sType = sSrcM.substr(nBase, sSrcM.find(L";", nBase) - nBase);
                    sImageName = sImageId + L"." + sType;
                    NSFile::CFileBinary oImageWriter;
                    if(oImageWriter.CreateFileW(sMedia + sImageName))
                    {
                        bRes = true;
                        size_t nBase = sSrcM.find(L"base64", nLen) + 7;
                        std::string sBase64 = m_oLightReader.GetTextA().substr(nBase);
                        int nSrcLen = (int)sBase64.length();
                        int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
                        BYTE* pImageData = new BYTE[nDecodeLen];
                        if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                            oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
                        RELEASEARRAYOBJECTS(pImageData);
                        oImageWriter.CloseFile();
                    }
                }
                // Картинка в сети
                else if(sSrcM.substr(0, nLen) == L"http" || !m_sBase.empty())
                {
                    sImageName = NSFile::GetFileName(sSrcM);
                    CFileDownloader oDownloadImg(m_sBase + sSrcM, false);
                    oDownloadImg.SetFilePath(sMedia + sImageName);
                    bRes = oDownloadImg.DownloadSync();
                }
                // Картинка по относительному пути
                else
                {
                    size_t nSrcM = sSrcM.rfind(L"/") + 1;
                    sImageName = sSrcM.substr(nSrcM);
                    bRes = NSFile::CFileBinary::Copy(sSrc + L"/" + sSrcM, sMedia + sImageName);
                }

                if(bRes)
                {
                    m_nImageId++;
                    // Прописать рельсы
                    oDocXmlRels += L"<Relationship Id=\"rPic";
                    oDocXmlRels += sImageId;
                    oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                    oDocXmlRels += sImageName;
                    oDocXmlRels += L"\"/>";

                    // Получаем размеры картинки
                    CBgraFrame oBgraFrame;
                    oBgraFrame.OpenFile(sMedia + sImageName);
                    int nHy = oBgraFrame.get_Height();
                    int nWx = oBgraFrame.get_Width();
                    if(nWx > nHy)
                    {
                        int nW = nWx * 9525;
                        nW = (nW > 7000000 ? 7000000 : nW);
                        nHy = (int)((double)nHy * (double)nW / (double)nWx);
                        nWx = nW;
                    }
                    else
                    {
                        int nH = nHy * 9525;
                        nH = (nH > 9000000 ? 9000000 : nH);
                        int nW = (int)((double)nWx * (double)nH / (double)nHy);
                        if(nW > 7000000)
                        {
                            nW = 7000000;
                            nHy = (int)((double)nHy * (double)nW / (double)nWx);
                        }
                        else
                            nHy = nH;
                        nWx = nW;
                    }

                    // Пишем в document.xml
                    oDocXml += L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"";
                    oDocXml += std::to_wstring(nWx);
                    oDocXml += L"\" cy=\"";
                    oDocXml += std::to_wstring(nHy);
                    oDocXml += L"\"/><wp:docPr id=\"";
                    oDocXml += sImageId;
                    oDocXml += L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"";
                    oDocXml += sImageId;
                    oDocXml += L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic";
                    oDocXml += sImageId;
                    oDocXml += L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"";
                    oDocXml += std::to_wstring(nWx);
                    oDocXml += L"\" cy=\"";
                    oDocXml += std::to_wstring(nHy);
                    oDocXml += L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>";
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

    void readP(const std::wstring& sSrc, const std::wstring& sDst, const std::wstring& sPName, std::wstring sRStyle, bool bBdo, NSStringUtils::CStringBuilder& oDocXmlRels, NSStringUtils::CStringBuilder& oDocXml, NSStringUtils::CStringBuilder& oNoteXml)
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode2(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"#text")
            {
                std::wstring sText = m_oLightReader.GetText();
                if(bBdo)
                    std::reverse(sText.begin(), sText.end());

                oDocXml += L"<w:r><w:rPr>";
                oDocXml += sRStyle;
                oDocXml += L"</w:rPr><w:t xml:space=\"preserve\">";
                oDocXml.WriteEncodeXmlString(sText);
                oDocXml += L"</w:t></w:r>";

            }
            // Абревиатура, реализована как сноски
            else if(sName == L"abbr")
            {
                std::wstring sNote = L"";
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"title")
                        sNote = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();

                readP(sSrc, sDst, sPName, sRStyle, bBdo, oDocXmlRels, oDocXml, oNoteXml);

                oDocXml += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"";
                oDocXml += std::to_wstring(m_nFootnoteId);
                oDocXml += L"\"/></w:r>";

                oNoteXml += L"<w:footnote w:id=\"";
                oNoteXml += std::to_wstring(m_nFootnoteId++);
                oNoteXml += L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">";
                oNoteXml += sNote;
                oNoteXml += L"</w:t></w:r></w:p></w:footnote>";
            }
            // Полужирный текст
            else if(sName == L"b")
                readP(sSrc, sDst, sPName, sRStyle + L"<w:b/>", bBdo, oDocXmlRels, oDocXml, oNoteXml);
            // Направление текста
            else if(sName == L"bdo")
            {
                std::wstring sDir = L"";
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"dir")
                        sDir = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();

                if(sDir == L"ltr")
                    readP(sSrc, sDst, sPName, sRStyle, false, oDocXmlRels, oDocXml, oNoteXml);
                else if(sDir == L"rtl")
                    readP(sSrc, sDst, sPName, sRStyle, true, oDocXmlRels, oDocXml, oNoteXml);
                else
                    readP(sSrc, sDst, sPName, sRStyle, !bBdo, oDocXmlRels, oDocXml, oNoteXml);
            }
            // Перенос строки
            else if(sName == L"br")
                oDocXml += L"<w:r><w:br/></w:r>";
            // Цитата, обычно выделяется курсивом
            // Новый термин, обычно выделяется курсивом
            // Акцентированный текст, обычно выделяется курсивом
            // Курсивный текст
            else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i")
                readP(sSrc, sDst, sPName, sRStyle + L"<w:i/>", bBdo, oDocXmlRels, oDocXml, oNoteXml);
            // Код
            else if(sName == L"code")
                readP(sSrc, sDst, sPName, sRStyle + L"<w:rStyle w:val=\"code\"/>", bBdo, oDocXmlRels, oDocXml, oNoteXml);
            // Ссылка
            else if(sName == L"iframe")
            {

            }
            // Картинки
            else if(sName == L"img" || sName == L"image")
                readImage(sSrc, sDst + L"/word/media/", oDocXml, oDocXmlRels);
            else
                readP(sSrc, sDst, sPName, sRStyle, bBdo, oDocXmlRels, oDocXml, oNoteXml);
        }
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
    NSStringUtils::CStringBuilder oDocXml;     // document.xml
    NSStringUtils::CStringBuilder oNoteXml;    // footnotes.xml

    oDocXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
    oDocXmlRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
    oDocXmlRels += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";

    oDocXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";

    oNoteXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
    oNoteXml += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

    std::wstring sSrcFolder = NSSystemPath::GetDirectoryName(sSrc);
    if(!m_internal->readSrc(sSource, sSrcFolder, sDst, oStylesXml, oDocXmlRels, oDocXml, oNoteXml))
        return S_FALSE;

    oDocXmlRels += L"</Relationships>";
    NSFile::CFileBinary oRelsWriter;
    if (oRelsWriter.CreateFileW(sDst + L"/word/_rels/document.xml.rels"))
    {
        oRelsWriter.WriteStringUTF8(oDocXmlRels.GetData());
        oRelsWriter.CloseFile();
    }

    oDocXml += L"<w:sectPr/></w:body></w:document>";
    NSFile::CFileBinary oDocumentWriter;
    if (oDocumentWriter.CreateFileW(sDst + L"/word/document.xml"))
    {
        oDocumentWriter.WriteStringUTF8(oDocXml.GetData());
        oDocumentWriter.CloseFile();
    }

    oNoteXml += L"</w:footnotes>";
    NSFile::CFileBinary oFootnotesWriter;
    if (oFootnotesWriter.CreateFileW(sDst + L"/word/footnotes.xml"))
    {
        oFootnotesWriter.WriteStringUTF8(oNoteXml.GetData());
        oFootnotesWriter.CloseFile();
    }

    NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");

    return S_OK;
}
