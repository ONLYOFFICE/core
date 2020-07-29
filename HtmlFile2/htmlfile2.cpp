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
#include "../DesktopEditor/xml/include/xmlutils.h"

class CHtmlFile2_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader;        // SAX Reader
    std::wstring m_sTmp;                            // Temp папка для конфертации html в xhtml
    std::map<std::wstring, std::wstring> m_mStyles; // Стили в document.xml. Хранятся как (имя тэга, его стиль)
    NSStringUtils::CStringBuilder oStylesXml;       // Внутренность styles.xml
public:
    CHtmlFile2_Private()
    {

    }

    ~CHtmlFile2_Private()
    {
        m_oLightReader.Clear();
        m_mStyles.clear();
        oStylesXml.Clear();
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

    void CreateDocxEmpty(const std::wstring& sDst, Writers::FileWriter* pDocxWriter)
    {
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
        OOX::CCore* pCore = new OOX::CCore(NULL);
        if (pCore)
        {
            pCore->SetCreator(L"");
            pCore->SetLastModifiedBy(L"");
            pCore->write(pathDocProps + FILE_SEPARATOR_STR + L"core.xml", DocProps, oContentTypes);
            RELEASEOBJECT(pCore);
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
    }

    // Предварительное чтение стилей и картинок
    bool readSrc(const std::wstring& sPath)
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

        readStyle();

        return true;
    }

    // Читает стили
    void readStyle()
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
                    size_t nPos = sRef.rfind(L".");
                    // Если это css файл, то поведение аналогично тэгу style
                    // Кроме функции получения стилей
                    if(sRef.substr(nPos) == L".css")
                    {
                        // Получаем наборы стилей как <w:style>...</w:style>...
                        std::wstring sStyle = L""; // oCSS.GetStyleFromCSS(sRef);
                        oStylesXml += sStyle;
                    }
                }
            }
            m_oLightReader.MoveToElement();
        }
        // ищем атрибут style
        else
        {
            std::wstring sSName = m_oLightReader.GetName();
            while(m_oLightReader.MoveToNextAttribute())
            {
                if(m_oLightReader.GetName() == L"style")
                {
                    // Получаем стиль как <w:pPr>...</w:pPr> для записи в document.xml
                    std::wstring sStyle = L""; // oCSS.GetStyleDoc(content());

                    std::map<std::wstring, std::wstring>::iterator it = m_mStyles.find(sSName);
                    // Если для тэга уже есть стиль, то получаем среднее
                    if(it != m_mStyles.end())
                    {
                        it->second = L""; // oCSS.GetStyleCompromise(it->second, sStyle);
                    }
                    // Если впервые, то сохраняем как - имя тэга в файле и его стиль
                    else
                        m_mStyles.insert(std::make_pair(sSName, sStyle));
                }
            }
            m_oLightReader.MoveToElement();
        }
        // Читаем весь файл
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
            readStyle();
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

    // FileWriter - Писатель docx
    // sDst - место создания docx, L"" - директория fontTable для инициализации, true - директория fontTable не требуется,
    // 1 - версия стилей, false - не сохранять диаграммы как изображения, NULL - кастомный конвертор связанный с pptx, L"" - пустая тема
    Writers::FileWriter* pDocxWriter =  new Writers::FileWriter(sDst, L"", true, 1, false, NULL, L"");
    if (pDocxWriter == NULL)
        return S_FALSE;

    m_internal->CreateDocxEmpty(sDst, pDocxWriter);

    if(!m_internal->readSrc(sSource))
        return S_FALSE;

    RELEASEOBJECT(pDocxWriter);
    return S_OK;
}
