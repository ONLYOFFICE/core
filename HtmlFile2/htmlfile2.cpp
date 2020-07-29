#include <string>

#include "htmlfile2.h"
#include "../Common/3dParty/html/htmltoxhtml.h"
#include "../ASCOfficeDocxFile2/BinReader/FileWriter.h"
#include "../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../Common/DocxFormat/Source/DocxFormat/App.h"
#include "../Common/DocxFormat/Source/DocxFormat/Core.h"
#include "../DesktopEditor/common/SystemUtils.h"

class CHtmlFile2_Private
{
public:

public:
    CHtmlFile2_Private()
    {

    }

    ~CHtmlFile2_Private()
    {

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
};

CHtmlFile2::CHtmlFile2()
{

}

CHtmlFile2::~CHtmlFile2()
{

}

bool CHtmlFile2::IsHtmlFile(const std::wstring& sFile)
{
    return false;
}

void CHtmlFile2::SetTmpDirectory(const std::wstring& sFolder)
{

}

HRESULT CHtmlFile2::Open(const std::wstring& sSrc, const std::wstring& sDst, CHtmlParams* oParams)
{
    NSFile::CFileBinary oXhtmlWriter;
    if (oXhtmlWriter.CreateFileW(sDst + L"/res.xhtml"))
    {
        // htmlToXhtml возвращает текст файла в кодировке UTF-8
        oXhtmlWriter.WriteStringUTF8(htmlToXhtml(sSrc));
        oXhtmlWriter.CloseFile();
    }

    // FileWriter - Писатель docx
    // sDst - место создания docx, L"" - директория fontTable для инициализации, true - директория fontTable не требуется,
    // 1 - версия стилей, false - не сохранять диаграммы как изображения, NULL - кастомный конвертор связанный с pptx, L"" - пустая тема
    Writers::FileWriter* pDocxWriter =  new Writers::FileWriter(sDst, L"", true, 1, false, NULL, L"");
    if (pDocxWriter == NULL)
        return S_FALSE;

    m_internal->CreateDocxEmpty(sDst, pDocxWriter);

    RELEASEOBJECT(pDocxWriter);
    return S_FALSE;
}
