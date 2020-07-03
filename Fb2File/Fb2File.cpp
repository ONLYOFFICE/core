#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"

#include <vector>
#include <map>

// Информация об авторе книги. Тэг author, translator
struct SAuthor
{
    std::wstring first_name;
    std::wstring middle_name;
    std::wstring last_name;
    std::wstring nickname;
    std::vector<std::wstring> home_page;
    std::vector<std::wstring> email;
};

// Описание информации о произведении. Тэг title-info, src-title-info
struct STitleInfo
{
    std::vector<std::wstring> m_arGenres; // Жанры
    std::vector<SAuthor> m_arAuthors;     // Авторы
    std::vector<SAuthor> m_arTranslator;  // Переводчики

    std::wstring m_sBookTitle; // Название
    std::wstring m_sLang;      // Язык после перевода

    std::wstring* m_pAnnotation;                    // Аннотация
    std::wstring* m_pKeywords;                      // Ключевые слова
    std::pair<std::wstring, std::wstring>* m_pDate; // Дата
    std::wstring* m_pCoverpage;                     // Обложка
    std::wstring* m_pSrcLang;                       // Язык до перевода

    std::map<std::wstring, std::wstring> m_mSequence; // Серии книг

    STitleInfo()
    {
        m_pAnnotation = NULL;
        m_pKeywords = NULL;
        m_pDate = NULL;
        m_pCoverpage = NULL;
        m_pSrcLang = NULL;
    }

    ~STitleInfo()
    {
        m_arGenres.clear();
        m_arAuthors.clear();
        m_arTranslator.clear();

        m_mSequence.clear();

        RELEASEARRAYOBJECTS(m_pAnnotation);
        RELEASEARRAYOBJECTS(m_pKeywords);
        RELEASEARRAYOBJECTS(m_pDate);
        RELEASEARRAYOBJECTS(m_pCoverpage);
        RELEASEARRAYOBJECTS(m_pSrcLang);
    }
};

class CFb2File_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader; // SAX Reader
    std::wstring m_sFile;                    // Имя файла
    std::wstring m_sDstFolder;               // Путь к результату

    STitleInfo m_oTitleInfo; // Данные о книге

    STitleInfo* m_pSrcTitleInfo; // Данные об исходнике книги
    /*
    std::vector<std::wstring> m_arGenres; // Жанры
    std::vector<SAuthor> m_arAuthors;      // Авторы
    std::vector<SAuthor> m_arTranslator;   // Переводчики

    std::wstring m_sBookTitle; // Название
    std::wstring m_sLang;      // Язык после перевода

    std::wstring* m_pAnnotation;                    // Аннотация
    std::wstring* m_pKeywords;                      // Ключевые слова
    std::pair<std::wstring, std::wstring>* m_pDate; // Дата
    std::wstring* m_pCoverpage;                     // Обложка
    std::wstring* m_pSrcLang;                       // Язык до перевода

    std::map<std::wstring, std::wstring> m_mSequence; // Серии книг
    */
    std::map<std::wstring, std::wstring> m_mXmlns;    // Ссылки
    std::map<std::wstring, std::wstring> m_mImage;    // Картинки

public:
    CFb2File_Private()
    {
        m_pSrcTitleInfo = NULL;
    }

    ~CFb2File_Private()
    {
        m_oLightReader.Clear();
        m_mXmlns.clear();
        m_mImage.clear();
        if(m_pSrcTitleInfo)
            delete m_pSrcTitleInfo;
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
    bool readAuthor(std::vector<SAuthor>& arAuthor)
    {
        SAuthor oAuthor;
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

    bool getTitleInfo(STitleInfo& oTitleInfo)
    {
        // Читаем genre (один или более)
        if(!isSection(L"genre"))
            return false;
        do
        {
            std::wstring sGenre;
            sGenre = content();
            if(sGenre == L"")
                return false;
            oTitleInfo.m_arGenres.push_back(sGenre);
        } while(isSection(L"genre"));

        // Читаем author (один или более)
        do
        {
            if(!readAuthor(oTitleInfo.m_arAuthors))
                return false;
        } while(isSection(L"author"));

        // Читаем book-title
        if(m_oLightReader.GetName() != L"book-title")
            return false;
        std::wstring sBookTitle;
        sBookTitle = content();
        if(sBookTitle == L"")
            return false;
        oTitleInfo.m_sBookTitle = sBookTitle;

        // Читаем все до lang
        while(!isSection(L"lang"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sRes;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем annotation (ноль или один)
            if(sName == L"annotation")
            {
                // Содержит форматированный текст - НЕ РЕАЛИЗОВАНО
                if(oTitleInfo.m_pAnnotation)
                    return false;
                oTitleInfo.m_pAnnotation = new std::wstring[1];
                *oTitleInfo.m_pAnnotation = L"";
                int nDepth = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nDepth))
                {
                    *oTitleInfo.m_pAnnotation += content();
                }
            }
            // Читаем keywords (ноль или один)
            else if(sName == L"keywords")
            {
                if(oTitleInfo.m_pKeywords)
                    return false;
                oTitleInfo.m_pKeywords = new std::wstring[1];
                sRes = content();
                if(sRes == L"")
                    return false;
                *oTitleInfo.m_pKeywords = sRes;
            }
            // Читаем date (ноль или один)
            else if(sName == L"date")
            {
                if(oTitleInfo.m_pDate)
                    return false;
                oTitleInfo.m_pDate = new std::pair<std::wstring, std::wstring>[1];
                std::wstring sDate = L"";
                if(m_oLightReader.MoveToNextAttribute())
                {
                    sDate = m_oLightReader.GetText();
                    m_oLightReader.MoveToElement();
                }
                sRes = content();
                if(sRes == L"")
                    return false;
                *oTitleInfo.m_pDate = make_pair(sDate, sRes);
            }
            // Читаем coverpage (ноль или один)
            else if(sName == L"coverpage")
            {
                if(oTitleInfo.m_pCoverpage)
                    return false;
                oTitleInfo.m_pCoverpage = new std::wstring[1];
                if(!m_oLightReader.ReadNextSiblingNode(m_oLightReader.GetDepth()))
                    return false;
                if(!m_oLightReader.MoveToNextAttribute())
                    return false;
                sRes = m_oLightReader.GetText();
                if(sRes == L"")
                    return false;
                m_oLightReader.MoveToElement();
                *oTitleInfo.m_pCoverpage = sRes;
            }
        }

        // Читаем lang
        if(m_oLightReader.GetName() != L"lang")
            return false;
        std::wstring sLang;
        sLang = content();
        if(sLang == L"")
            return false;
        oTitleInfo.m_sLang = sLang;

        // Читаем все до document-info
        while(!isSection(L"document-info"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sRes;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем src-lang (ноль или один)
            if(sName == L"src-lang")
            {
                if(oTitleInfo.m_pSrcLang)
                    return false;
                oTitleInfo.m_pSrcLang = new std::wstring[1];
                sRes = content();
                if(sRes == L"")
                    return false;
                *oTitleInfo.m_pSrcLang = sRes;
            }
            // Читаем translator (любое)
            else if(sName == L"translator")
            {
                if(!readAuthor(oTitleInfo.m_arTranslator))
                    return false;
            }
            // Читаем sequence (любое)
            else if(sName == L"sequence")
            {
                if(!m_oLightReader.MoveToNextAttribute())
                    return false;
                std::wstring sSName = m_oLightReader.GetText();
                if(!m_oLightReader.MoveToNextAttribute())
                    return false;
                std::wstring sSNumber = m_oLightReader.GetText();
                oTitleInfo.m_mSequence.insert(std::make_pair(sSName, sSNumber));
            }
            else if(sName == L"src-title-info")
            {
                break;
            }
        }
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
    if(!m_internal->getTitleInfo(m_internal->m_oTitleInfo))
        return false;

    if(m_internal->m_oLightReader.GetName() == L"src-title-info")
    {
        m_internal->m_pSrcTitleInfo = new STitleInfo();
        if(!m_internal->getTitleInfo(*m_internal->m_pSrcTitleInfo))
            return false;
    }

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
