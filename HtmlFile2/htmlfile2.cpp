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
    XmlUtils::CXmlLiteReader m_oLightReader; // SAX Reader

    std::wstring m_sTmp; // Temp папка для конфертации html в xhtml
    std::wstring m_sSrc; // Директория источника
    std::wstring m_sDst; // Директория назначения

private:
    std::map<std::wstring, std::wstring> m_mStyles; // Стили в document.xml. Хранятся как (имя тэга, его стиль)

    std::wstring m_sBase; // Полный базовый адрес

    int m_nImageId;     // ID картинки
    int m_nFootnoteId;  // ID сноски
    int m_nHyperlinkId; // ID ссылки

    NSStringUtils::CStringBuilder m_oStylesXml;  // styles.xml
    NSStringUtils::CStringBuilder m_oDocXmlRels; // document.xml.rels
    NSStringUtils::CStringBuilder m_oDocXml;     // document.xml
    NSStringUtils::CStringBuilder m_oNoteXml;    // footnotes.xml

public:
    CHtmlFile2_Private()
    {
        m_nImageId = 1;
        m_nFootnoteId = 1;
        m_nHyperlinkId = 1;
        m_sBase = L"";
    }

    ~CHtmlFile2_Private()
    {
        m_oLightReader.Clear();
        m_mStyles.clear();
        m_oStylesXml.Clear();
        m_oDocXmlRels.Clear();
        m_oDocXml.Clear();
        m_oNoteXml.Clear();
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

    void CreateDocxEmpty()
    {
        Writers::FileWriter* pDocxWriter; // Писатель скелета docx
        // FileWriter - Писатель docx
        // sDst - место создания docx, L"" - директория fontTable для инициализации, true - директория fontTable не требуется,
        // 1 - версия стилей, false - не сохранять диаграммы как изображения, NULL - кастомный конвертор связанный с pptx, L"" - пустая тема
        pDocxWriter =  new Writers::FileWriter(m_sDst, L"", true, 1, false, NULL, L"");
        // Создаем пустые папки
        std::wstring strDirectory = m_sDst;
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

        oContentTypes.AddDefault(L"jpg");
        oContentTypes.AddDefault(L"jfif");
        oContentTypes.AddDefault(L"jpe");
        oContentTypes.AddDefault(L"gif");
        oContentTypes.AddDefault(L"svg");
        oContentTypes.AddDefault(L"bmp");
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.document.main+xml", OOX::CPath(L"/word"),       OOX::CPath(L"document.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.styles+xml",        OOX::CPath(L"/word"),       OOX::CPath(L"styles.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.settings+xml",      OOX::CPath(L"/word"),       OOX::CPath(L"settings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.webSettings+xml",   OOX::CPath(L"/word"),       OOX::CPath(L"webSettings.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.fontTable+xml",     OOX::CPath(L"/word"),       OOX::CPath(L"fontTable.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.theme+xml",                          OOX::CPath(L"/word/theme"), OOX::CPath(L"theme1.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-package.core-properties+xml",                       OOX::CPath(L"/docProps"),   OOX::CPath(L"core.xml"));
        oContentTypes.Registration(L"application/vnd.openxmlformats-officedocument.wordprocessingml.footnotes+xml",     OOX::CPath(L"/word"),       OOX::CPath(L"footnotes.xml"));
        oContentTypes.Write(strDirectory);

        NSFile::CFileBinary oFootRelsWriter;
        if (oFootRelsWriter.CreateFileW(strDirectory + L"/word/_rels/footnotes.xml.rels"))
        {
            oFootRelsWriter.WriteStringUTF8(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\"></Relationships>");
            oFootRelsWriter.CloseFile();
        }

        RELEASEOBJECT(pDocxWriter);

        m_oDocXmlRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
        m_oDocXmlRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
        m_oDocXmlRels += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";

        m_oDocXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";

        m_oNoteXml += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
        m_oNoteXml += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote><w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

    }

    bool readSrc()
    {
        if(!m_oLightReader.IsValid())
        {
            if (!m_oLightReader.FromFile(m_sTmp + L"/res.xhtml"))
                return false;
            if(!isHtml())
                return false;
        }
        if(m_oLightReader.IsEmptyNode())
            return true;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"head")
                readHead();
            else if(sName == L"body")
                readBody(sName, L"", false);
        }
        return true;
    }

    void write()
    {
        m_oDocXmlRels += L"</Relationships>";
        NSFile::CFileBinary oRelsWriter;
        if (oRelsWriter.CreateFileW(m_sDst + L"/word/_rels/document.xml.rels"))
        {
            oRelsWriter.WriteStringUTF8(m_oDocXmlRels.GetData());
            oRelsWriter.CloseFile();
        }

        m_oDocXml += L"<w:sectPr/></w:body></w:document>";
        NSFile::CFileBinary oDocumentWriter;
        if (oDocumentWriter.CreateFileW(m_sDst + L"/word/document.xml"))
        {
            oDocumentWriter.WriteStringUTF8(m_oDocXml.GetData());
            oDocumentWriter.CloseFile();
        }

        m_oNoteXml += L"</w:footnotes>";
        NSFile::CFileBinary oFootnotesWriter;
        if (oFootnotesWriter.CreateFileW(m_sDst + L"/word/footnotes.xml"))
        {
            oFootnotesWriter.WriteStringUTF8(m_oNoteXml.GetData());
            oFootnotesWriter.CloseFile();
        }
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
    void readHead()
    {
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();

            // Базовый адрес
            if(sName == L"base")
            {
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"href")
                        m_sBase = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();
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
                            m_oStylesXml += sStyle;
                        }
                    }
                }
                m_oLightReader.MoveToElement();
            }
            // тэг style содержит стили для styles.xml
            else if(sName == L"style")
            {
                // Получаем наборы стилей как <w:style>...</w:style>...
                std::wstring sStyle = L""; // oCSS.GetStyleXml(content());
                // Дописываем в styles.xml
                m_oStylesXml += sStyle;
            }
            // Заголовок документа
            else if(sName == L"title")
                readTitle();
        }
    }

    void readBody(const std::wstring& sPName, std::wstring sRStyle, bool bBdo)
    {
        readStyle();
        if(m_oLightReader.IsEmptyNode())
            return;

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode2(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"#text")
            {
                std::wstring sText = m_oLightReader.GetText();
                if(bBdo)
                    std::reverse(sText.begin(), sText.end());

                m_oDocXml += L"<w:p><w:r><w:rPr>";
                m_oDocXml += sRStyle;
                m_oDocXml += L"</w:rPr><w:t xml:space=\"preserve\">";
                m_oDocXml.WriteEncodeXmlString(sText);
                m_oDocXml += L"</w:t></w:r></w:p>";

            }
            // Ссылки
            else if(sName == L"a")
            {
                m_oDocXml += L"<w:p>";
                readLink(sPName, sRStyle, bBdo);
                m_oDocXml += L"</w:p>";
            }
            // Абревиатура, реализована как сноски
            else if(sName == L"abbr")
            {
                m_oDocXml += L"<w:p>";
                readAbbr(sPName, sRStyle, bBdo);
                m_oDocXml += L"</w:p>";
            }
            // Адрес
            // Абзац текста. Содержит фразовый контент
            else if(sName == L"address" || sName == L"p")
            {
                m_oDocXml += L"<w:p>";
                readP(sName, sRStyle, bBdo);
                m_oDocXml += L"</w:p>";
            }
            // Статья
            // Боковой блок
            else if(sName == L"article" || sName == L"aside")
            {
                m_oDocXml += L"<w:p>";
                readBody(sName, sRStyle, bBdo);
                m_oDocXml += L"</w:p>";
            }
            // Картинки
            else if(sName == L"img" || sName == L"image")
            {
                m_oDocXml += L"<w:p>";
                readImage();
                m_oDocXml += L"</w:p>";
            }
            else
                readBody(sPName, sRStyle, bBdo);
        }
    }

    void readAbbr(const std::wstring& sPName, std::wstring sRStyle, bool bBdo)
    {
        std::wstring sNote = L"";
        while(m_oLightReader.MoveToNextAttribute())
            if(m_oLightReader.GetName() == L"title")
                sNote = m_oLightReader.GetText();
        m_oLightReader.MoveToElement();

        readP(sPName, sRStyle, bBdo);

        m_oDocXml += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"";
        m_oDocXml += std::to_wstring(m_nFootnoteId);
        m_oDocXml += L"\"/></w:r>";

        m_oNoteXml += L"<w:footnote w:id=\"";
        m_oNoteXml += std::to_wstring(m_nFootnoteId++);
        m_oNoteXml += L"\"><w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr><w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r><w:r><w:t xml:space=\"preserve\">";
        m_oNoteXml += sNote;
        m_oNoteXml += L"</w:t></w:r></w:p></w:footnote>";
    }

    void readLink(const std::wstring& sPName, std::wstring sRStyle, bool bBdo)
    {
        std::wstring sRef = L"";
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"href")
            {
                sRef = m_oLightReader.GetText();
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

        if(sRef.empty())
            return;

        // Пишем рельсы
        m_oDocXmlRels += L"<Relationship Id=\"rHyp";
        m_oDocXmlRels += std::to_wstring(m_nHyperlinkId);
        m_oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink\" Target=\"";
        m_oDocXmlRels += sRef;
        m_oDocXmlRels += L"\" TargetMode=\"External\"/>";

        // Пишем в document.xml
        m_oDocXml += L"<w:hyperlink w:tooltip=\"";
        m_oDocXml += sRef;
        m_oDocXml += L"\" r:id=\"rHyp";
        m_oDocXml += std::to_wstring(m_nHyperlinkId++);
        m_oDocXml += L"\">";
        readP(sPName, sRStyle += L"<w:rStyle w:val=\"link\"/>", bBdo);
        m_oDocXml += L"</w:hyperlink>";
    }

    void readStyle()
    {
        std::wstring sName = m_oLightReader.GetName();
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
    }

    void readImage()
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
                    if(oImageWriter.CreateFileW(m_sDst + L"/word/media/" + sImageName))
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
                    oDownloadImg.SetFilePath(m_sDst + L"/word/media/" + sImageName);
                    bRes = oDownloadImg.DownloadSync();
                }
                // Картинка по относительному пути
                else
                {
                    size_t nSrcM = sSrcM.rfind(L"/") + 1;
                    sImageName = sSrcM.substr(nSrcM);
                    bRes = NSFile::CFileBinary::Copy(m_sSrc + L"/" + sSrcM, m_sDst + L"/word/media/" + sImageName);
                }

                if(bRes)
                {
                    m_nImageId++;
                    // Прописать рельсы
                    m_oDocXmlRels += L"<Relationship Id=\"rPic";
                    m_oDocXmlRels += sImageId;
                    m_oDocXmlRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                    m_oDocXmlRels += sImageName;
                    m_oDocXmlRels += L"\"/>";

                    // Получаем размеры картинки
                    CBgraFrame oBgraFrame;
                    oBgraFrame.OpenFile(m_sDst + L"/word/media/" + sImageName);
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
                    m_oDocXml += L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\"><wp:extent cx=\"";
                    m_oDocXml += std::to_wstring(nWx);
                    m_oDocXml += L"\" cy=\"";
                    m_oDocXml += std::to_wstring(nHy);
                    m_oDocXml += L"\"/><wp:docPr id=\"";
                    m_oDocXml += sImageId;
                    m_oDocXml += L"\" name=\"\"/><a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:nvPicPr><pic:cNvPr id=\"";
                    m_oDocXml += sImageId;
                    m_oDocXml += L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr><pic:blipFill><a:blip r:embed=\"rPic";
                    m_oDocXml += sImageId;
                    m_oDocXml += L"\"/><a:stretch/></pic:blipFill><pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"";
                    m_oDocXml += std::to_wstring(nWx);
                    m_oDocXml += L"\" cy=\"";
                    m_oDocXml += std::to_wstring(nHy);
                    m_oDocXml += L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr></pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>";
                }
            }
        }
        m_oLightReader.MoveToElement();
    }

    void readTitle()
    {
        OOX::CPath DocProps = L"docProps";
        OOX::CContentTypes oContentTypes;
        OOX::CCore* pCore = new OOX::CCore(NULL);
        if (pCore)
        {
            pCore->m_sTitle = content();
            pCore->SetCreator(L"");
            pCore->SetLastModifiedBy(L"");
            pCore->write(OOX::CPath(m_sDst + L"/docProps/core.xml"), DocProps, oContentTypes);
            RELEASEOBJECT(pCore);
        }
    }

    void readP(const std::wstring& sPName, std::wstring sRStyle, bool bBdo)
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

                m_oDocXml += L"<w:r><w:rPr>";
                m_oDocXml += sRStyle;
                m_oDocXml += L"</w:rPr><w:t xml:space=\"preserve\">";
                m_oDocXml.WriteEncodeXmlString(sText);
                m_oDocXml += L"</w:t></w:r>";

            }
            // Ссылки
            else if(sName == L"a")
                readLink(sPName, sRStyle, bBdo);
            // Абревиатура, реализована как сноски
            else if(sName == L"abbr")
                readAbbr(sPName, sRStyle, bBdo);
            // Полужирный текст
            // Акцентированный текст
            else if(sName == L"b" || sName == L"strong")
                readP(sPName, sRStyle + L"<w:b/>", bBdo);
            // Направление текста
            else if(sName == L"bdo")
            {
                std::wstring sDir = L"";
                while(m_oLightReader.MoveToNextAttribute())
                    if(m_oLightReader.GetName() == L"dir")
                        sDir = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();

                if(sDir == L"ltr")
                    readP(sPName, sRStyle, false);
                else if(sDir == L"rtl")
                    readP(sPName, sRStyle, true);
                else
                    readP(sPName, sRStyle, !bBdo);
            }
            // Увеличивает размер шрифта
            else if(sName == L"big")
                readP(sPName, sRStyle + L"<w:sz w:val=\"26\"/>", bBdo);
            // Перенос строки
            else if(sName == L"br")
                m_oDocXml += L"<w:r><w:br/></w:r>";
            // Цитата, обычно выделяется курсивом
            // Новый термин, обычно выделяется курсивом
            // Акцентированный текст
            // Курсивный текст
            // Переменная, обычно выделяется курсивом
            else if(sName == L"cite" || sName == L"dfn" || sName == L"em" || sName == L"i" || sName == L"var")
                readP(sPName, sRStyle + L"<w:i/>", bBdo);
            // Код
            // Моноширинный шрифт, например, Consolas
            // Результат скрипта
            else if(sName == L"code" || sName == L"kbd" || sName == L"samp")
                readP(sPName, sRStyle + L"<w:rFonts w:ascii=\"Consolas\" w:hAnsi=\"Consolas\"/>", bBdo);
            // Ссылка
            // Объект для обработки
            else if(sName == L"iframe" || sName == L"object")
            {

            }
            // Картинки
            else if(sName == L"img" || sName == L"image")
                readImage();
            // Метка
            // Скрипты не поддерживаются
            // Выводится информация с помощью скриптов
            else if(sName == L"label" || sName == L"noscript" || sName == L"output")
                readP(sPName, sRStyle, bBdo);
            // Выделенный текст, обычно выделяется желтым
            else if(sName == L"mark")
                readP(sPName, sRStyle + L"<w:highlight w:val=\"yellow\"/>", bBdo);
            // Математическая формула
            else if(sName == L"math")
            {

            }
            // Цитата, выделенная кавычками, обычно выделяется курсивом
            else if(sName == L"q")
            {
                m_oDocXml += L"<w:r><w:t xml:space=\"preserve\">«</w:t></w:r>";
                readP(sPName, sRStyle + L"<w:i/>", bBdo);
                m_oDocXml += L"<w:r><w:t xml:space=\"preserve\">»</w:t></w:r>";
            }
            // Текст верхнего регистра
            else if(sName == L"rt" || sName == L"sup")
                readP(sPName, sRStyle + L"<w:vertAlign w:val=\"superscript\"/>", bBdo);
            // Текст при отсутствии поддержки rt игнорируется
            // Скрипт игнорируется
            else if(sName == L"rp" || sName == L"script")
                continue;
            // Уменьшает размер шрифта
            else if(sName == L"small")
                readP(sPName, sRStyle + L"<w:sz w:val=\"18\"/>", bBdo);
            // Текст нижнего регистра
            else if(sName == L"sub")
                readP(sPName, sRStyle + L"<w:vertAlign w:val=\"subscript\"/>", bBdo);
            // Векторная картинка
            else if(sName == L"svg")
                readSVG();
            // Текст с границами
            else if(sName == L"textarea")
            {
                m_oDocXml += L"<w:pPr><w:pBdr><w:left w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:top w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:right w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/><w:bottom w:val=\"single\" w:color=\"000000\" w:sz=\"8\" w:space=\"0\"/></w:pBdr></w:pPr>";
                readP(sPName, sRStyle, bBdo);
            }
            else
                readP(sPName, sRStyle, bBdo);
        }
    }

    void readSVG()
    {
        // Сохранить как .svg картинку
        NSStringUtils::CStringBuilder oSVG;
        bool bNeedXmlns = true;
        oSVG += L"<svg ";
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"xmlns")
                bNeedXmlns = false;
            oSVG += m_oLightReader.GetName();
            oSVG += L"=\"";
            oSVG += m_oLightReader.GetText();
            oSVG += L"\" ";
        }
        m_oLightReader.MoveToElement();
        if(bNeedXmlns)
            oSVG += L"xmlns=\"http://www.w3.org/2000/svg\"";
        oSVG += L">";
        oSVG += m_oLightReader.GetInnerXml();
        oSVG += L"</svg>";

        NSFile::CFileBinary oSVGWriter;
        std::wstring sImageId = std::to_wstring(m_nImageId++);
        if (oSVGWriter.CreateFileW(m_sDst + L"/word/media/" + sImageId + L".svg"))
        {
            oSVGWriter.WriteStringUTF8(oSVG.GetData());
            oSVGWriter.CloseFile();
        }

        // Прописать рельсы
        // Прописать в document.xml
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
    m_internal->m_sSrc = NSSystemPath::GetDirectoryName(sSrc);
    m_internal->m_sDst = sDst;
    m_internal->htmlXhtml(sSrc);
    m_internal->CreateDocxEmpty();
    if(!m_internal->readSrc())
        return S_FALSE;
    m_internal->write();
    NSFile::CFileBinary::Remove(m_internal->m_sTmp + L"/res.xhtml");
    return S_OK;
}
