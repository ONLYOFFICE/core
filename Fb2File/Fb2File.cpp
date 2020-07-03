#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"

#include <vector>
#include <map>

// Тэг author
struct author
{
    std::wstring first_name;
    std::wstring middle_name;
    std::wstring last_name;
    std::wstring nickname;
    std::vector<std::wstring> home_page;
    std::vector<std::wstring> email;
};

class CFb2File_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader; // SAX Reader
    std::wstring m_sFile;                    // Имя файла
    std::wstring m_sDstFolder;               // Путь к результату
    // Контент файла
    std::vector<std::wstring> m_arGenres; // Жанры
    std::vector<author> m_arAuthors;      // Авторы
    std::vector<author> m_arTranslator;   // Переводчики

    std::wstring m_sBookTitle; // Название
    std::wstring m_sLang;      // Язык после перевода

    std::wstring* m_pAnnotation;                    // Аннотация
    std::wstring* m_pKeywords;                      // Ключевые слова
    std::pair<std::wstring, std::wstring>* m_pDate; // Дата
    std::wstring* m_pCoverpage;                     // Обложка
    std::wstring* m_pSrcLang;                       // Язык до перевода

    std::map<std::wstring, std::wstring> m_mXmlns;    // Ссылки
    std::map<std::wstring, std::wstring> m_mSequence; // Серии книг
    std::map<std::wstring, std::wstring> m_mImage;    // Картинки

public:
    CFb2File_Private()
    {
        m_pAnnotation = NULL;
        m_pKeywords = NULL;
        m_pDate = NULL;
        m_pCoverpage = NULL;
        m_pSrcLang = NULL;
    }

    ~CFb2File_Private()
    {
        m_oLightReader.Clear();
        m_arGenres.clear();
        m_arAuthors.clear();
        m_arTranslator.clear();
        m_mXmlns.clear();
        m_mSequence.clear();
        m_mImage.clear();
        RELEASEARRAYOBJECTS(m_pAnnotation);
        RELEASEARRAYOBJECTS(m_pKeywords);
        RELEASEARRAYOBJECTS(m_pDate);
        RELEASEARRAYOBJECTS(m_pCoverpage);
        RELEASEARRAYOBJECTS(m_pSrcLang);
    }

    // Читает и проверят соответствует ли текущий раздел ожиданиям
    bool isSection(const std::wstring& sName)
    {
        if(!m_oLightReader.ReadNextNode())
            return false;
        if(m_oLightReader.GetName() != sName)
            return false;
        return true;
    }

    // Читает поля автора
    bool readAuthor(std::vector<author>& arAuthor)
    {
        author oAuthor;
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            std::wstring sRes;
            if(sName == L"first-name")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.first_name = sRes;
            }
            else if(sName == L"middle-name")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.middle_name = sRes;
            }
            else if(sName == L"last-name")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.last_name = sRes;
            }
            else if(sName == L"nickname")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.nickname = sRes;
            }
            else if(sName == L"home-page")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.home_page.push_back(sRes);
            }
            else if(sName == L"email")
            {
                sRes = content();
                if(sRes == L"")
                    return false;
                oAuthor.email.push_back(sRes);
            }
            else
                return false;
        }
        arAuthor.push_back(oAuthor);
        return true;
    }

    // Возвращает содержание узла
    std::wstring content()
    {
        std::wstring sRes = L"";
        if(m_oLightReader.ReadNextSiblingNode2(m_oLightReader.GetDepth()))
            sRes = m_oLightReader.GetText();
        return sRes;
    }

    std::string contentA(std::wstring is)
    {
        std::string sRes = "";
        if(m_oLightReader.ReadNextSiblingNode2(m_oLightReader.GetDepth()))
            sRes = m_oLightReader.GetTextA();
        return sRes;
    }
};

CFb2File::CFb2File()
{
    m_internal = new CFb2File_Private();
}

CFb2File::~CFb2File()
{
    RELEASEOBJECT(m_internal);
}

// Проверяет, соответствует ли fb2 файл формату
bool CFb2File::IsFb2File(const std::wstring& sFile)
{
    if (!m_internal->m_oLightReader.FromFile(sFile))
        return false;

    // Читаем FictionBook
    if(!m_internal->isSection(L"FictionBook"))
        return false;
    while(m_internal->m_oLightReader.MoveToNextAttribute())
        m_internal->m_mXmlns.insert(std::make_pair(m_internal->m_oLightReader.GetName(), m_internal->m_oLightReader.GetText()));
    m_internal->m_sFile = sFile;
    return true;
}

// Выставление временной (рабочей) папки
void CFb2File::SetImpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sDstFolder = sFolder;
}

// Проверяет, соответствует ли fb2 файл формату
// sPath - путь к сохраненному файлу, sDirectory - путь к сохраненным картинкам
int CFb2File::Convert (const std::wstring& sPath, const std::wstring& sDirectory)
{
    // Читаем description
    if(!m_internal->isSection(L"description"))
        return false;

    // Читаем наполнение description
    // Читаем title-info
    if(!m_internal->isSection(L"title-info"))
        return false;

    // Читаем наполнение title-info
    // Читаем genre (один или более)
    if(!m_internal->isSection(L"genre"))
        return false;
    do
    {
        std::wstring sGenre;
        sGenre = m_internal->content();
        if(sGenre == L"")
            return false;
        m_internal->m_arGenres.push_back(sGenre);
    } while(m_internal->isSection(L"genre"));

    // Читаем author (один или более)
    do
    {
        if(!m_internal->readAuthor(m_internal->m_arAuthors))
            return false;
    } while(m_internal->isSection(L"author"));

    // Читаем book-title
    if(m_internal->m_oLightReader.GetName() != L"book-title")
        return false;
    std::wstring sBookTitle;
    sBookTitle = m_internal->content();
    if(sBookTitle == L"")
        return false;
    m_internal->m_sBookTitle = sBookTitle;

    // Читаем все до lang
    while(!m_internal->isSection(L"lang"))
    {
        if(m_internal->m_oLightReader.GetDepth() == 0)
            return false;
        std::wstring sRes;
        std::wstring sName = m_internal->m_oLightReader.GetName();

        // Читаем annotation (ноль или один)
        if(sName == L"annotation")
        {
            // Содержит форматированный текст - НЕ РЕАЛИЗОВАНО
            if(m_internal->m_pAnnotation)
                return false;
            m_internal->m_pAnnotation = new std::wstring[1];
            *m_internal->m_pAnnotation = L"";
            int nDepth = m_internal->m_oLightReader.GetDepth();
            while(m_internal->m_oLightReader.ReadNextSiblingNode(nDepth))
            {
                *m_internal->m_pAnnotation += m_internal->content();
            }
        }
        // Читаем keywords (ноль или один)
        else if(sName == L"keywords")
        {
            if(m_internal->m_pKeywords)
                return false;
            m_internal->m_pKeywords = new std::wstring[1];
            sRes = m_internal->content();
            if(sRes == L"")
                return false;
            *m_internal->m_pKeywords = sRes;
        }
        // Читаем date (ноль или один)
        else if(sName == L"date")
        {
            if(m_internal->m_pDate)
                return false;
            m_internal->m_pDate = new std::pair<std::wstring, std::wstring>[1];
            std::wstring sDate = L"";
            if(m_internal->m_oLightReader.MoveToNextAttribute())
            {
                sDate = m_internal->m_oLightReader.GetText();
                m_internal->m_oLightReader.MoveToElement();
            }
            sRes = m_internal->content();
            if(sRes == L"")
                return false;
            *m_internal->m_pDate = make_pair(sDate, sRes);
        }
        // Читаем coverpage (ноль или один)
        else if(sName == L"coverpage")
        {
            if(m_internal->m_pCoverpage)
                return false;
            m_internal->m_pCoverpage = new std::wstring[1];
            if(!m_internal->m_oLightReader.ReadNextSiblingNode(m_internal->m_oLightReader.GetDepth()))
                return false;
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            sRes = m_internal->m_oLightReader.GetText();
            if(sRes == L"")
                return false;
            m_internal->m_oLightReader.MoveToElement();
            *m_internal->m_pCoverpage = sRes;
        }
    }

    // Читаем lang
    if(m_internal->m_oLightReader.GetName() != L"lang")
        return false;
    std::wstring sLang;
    sLang = m_internal->content();
    if(sLang == L"")
        return false;
    m_internal->m_sLang = sLang;

    // Читаем все до document-info
    while(!m_internal->isSection(L"document-info"))
    {
        if(m_internal->m_oLightReader.GetDepth() == 0)
            return false;
        std::wstring sRes;
        std::wstring sName = m_internal->m_oLightReader.GetName();

        // Читаем src-lang (ноль или один)
        if(sName == L"src-lang")
        {
            if(m_internal->m_pSrcLang)
                return false;
            m_internal->m_pSrcLang = new std::wstring[1];
            sRes = m_internal->content();
            if(sRes == L"")
                return false;
            *m_internal->m_pSrcLang = sRes;
        }
        // Читаем translator (любое)
        else if(sName == L"translator")
        {
            if(!m_internal->readAuthor(m_internal->m_arTranslator))
                return false;
        }
        // Читаем sequence (любое)
        else if(sName == L"sequence")
        {
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            std::wstring sSName = m_internal->m_oLightReader.GetText();
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            std::wstring sSNumber = m_internal->m_oLightReader.GetText();
            m_internal->m_mSequence.insert(std::make_pair(sSName, sSNumber));
        }
    }

    // ПРОЧИТАН разде title-info

    // ПРОПУСКАЕМ body

    while(m_internal->m_oLightReader.ReadNextNode())
    {
        // Читает картинки
        if(m_internal->m_oLightReader.GetName() == L"binary")
        {
            std::wstring sId;
            std::wstring sContentType;

            // Читает id
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            sId = m_internal->m_oLightReader.GetText();

            // Читает content-type
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            sContentType = m_internal->m_oLightReader.GetText();

            m_internal->m_mImage.insert(std::make_pair(sId, sContentType));
            m_internal->m_oLightReader.MoveToElement();

            // Пишет картинку в файл
            NSFile::CFileBinary oImageWriter;
            if (!oImageWriter.CreateFileW(sDirectory + sId))
                return false;
            std::string sBase64 = m_internal->contentA(sId);
            int nSrcLen = (int)sBase64.length();
            int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
            BYTE* pImageData = new BYTE[nDecodeLen];
            if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
            RELEASEARRAYOBJECTS(pImageData);
            oImageWriter.CloseFile();
        }
    }
    return TRUE;
}
