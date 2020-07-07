#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"

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
    std::wstring id;
};

// Описание информации о произведении. Тэг title-info, src-title-info
struct STitleInfo
{
    std::vector<std::wstring> m_arGenres; // Жанры
    std::vector<SAuthor> m_arAuthors;     // Авторы
    std::vector<SAuthor> m_arTranslator;  // Переводчики

    std::wstring m_sBookTitle; // Название
    std::wstring m_sLang;      // Язык после перевода

    std::wstring* m_pAnnotation;                    // Аннотация (ФОРМАТИРОВАННЫЙ ТЕКСТ)
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

// Описание информации о fb2-документе. Тэг document-info
struct SDocumentInfo
{
    std::vector<SAuthor> m_arAuthors;     // Авторы
    std::vector<std::wstring> m_arSrcUrl; // URL страницы

    std::pair<std::wstring, std::wstring> m_pDate; // Дата
    std::wstring m_sId;                            // Идентификатор
    std::wstring m_sVersion;                       // Версия документа

    std::wstring* m_pProgramUsed; // Использованные программы
    std::wstring* m_pSrcOcr;      // Автор текста
    std::wstring* m_pHistory;     // История (ФОРМАТИРОВАННЫЙ ТЕКСТ)

    SDocumentInfo()
    {
        m_pProgramUsed = NULL;
        m_pSrcOcr = NULL;
        m_pHistory = NULL;
    }

    ~SDocumentInfo()
    {
        m_arAuthors.clear();
        m_arSrcUrl.clear();

        RELEASEARRAYOBJECTS(m_pProgramUsed);
        RELEASEARRAYOBJECTS(m_pSrcOcr);
        RELEASEARRAYOBJECTS(m_pHistory);
    }
};

// Информация о бумажном издании. Тэг publish-info
struct SPublishInfo
{
    std::wstring* m_pBookName;  // Название
    std::wstring* m_pPublisher; // Издательство
    std::wstring* m_pCity;      // Место
    std::wstring* m_pYear;      // Год
    std::wstring* m_pIsbn;      // ISBN

    std::map<std::wstring, std::wstring> m_mSequence; // Серии книг

    SPublishInfo()
    {
        m_pBookName = NULL;
        m_pPublisher = NULL;
        m_pCity = NULL;
        m_pYear = NULL;
        m_pIsbn = NULL;
    }

    ~SPublishInfo()
    {
        m_mSequence.clear();

        RELEASEARRAYOBJECTS(m_pBookName);
        RELEASEARRAYOBJECTS(m_pPublisher);
        RELEASEARRAYOBJECTS(m_pCity);
        RELEASEARRAYOBJECTS(m_pYear);
        RELEASEARRAYOBJECTS(m_pIsbn);
    }
};

class CFb2File_Private
{
public:
    XmlUtils::CXmlLiteReader m_oLightReader; // SAX Reader
    std::wstring m_sFile;                    // Имя файла
    std::wstring m_sDstFolder;               // Путь к результату
    std::wstring m_sBody;                    // Текст
    STitleInfo m_oTitleInfo;                 // Данные о книге
    SDocumentInfo m_oDocumentInfo;           // Информация об fb2-документе

    STitleInfo* m_pSrcTitleInfo;  // Данные об исходнике книги
    SPublishInfo* m_pPublishInfo; // Сведения об издании книги

    std::map<std::wstring, std::wstring> m_mXmlns;      // Ссылки
    std::map<std::wstring, std::wstring> m_mImage;      // Картинки
    std::map<std::wstring, std::wstring> m_mCustomInfo; // Произвольная информация

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
    bool getAuthor(std::vector<SAuthor>& arAuthor)
    {
        SAuthor oAuthor;
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"first-name")
                oAuthor.first_name = content();
            else if(sName == L"middle-name")
                oAuthor.middle_name = content();
            else if(sName == L"last-name")
                oAuthor.last_name = content();
            else if(sName == L"nickname")
                oAuthor.nickname = content();
            else if(sName == L"home-page")
                oAuthor.home_page.push_back(content());
            else if(sName == L"email")
                oAuthor.email.push_back(content());
            else if(sName == L"id")
                oAuthor.id = content();
            else
                return false;
        }
        arAuthor.push_back(oAuthor);
        return true;
    }

    // Читает title
    bool readTitle()
    {
        m_sBody += L"title\n";
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            if(m_oLightReader.GetName() == L"p")
            {
                if(!readP())
                    return false;
            }
            else if(m_oLightReader.GetName() == L"empty-line")
                m_sBody += L"\n";
            else
                return false;
        }
        return true;
    }

    // Читает epigraph
    bool readEpigraph()
    {
        m_sBody += L"epigraph\n";
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                if(!readP())
                    return false;
            }
            else if(sName == L"poem")
            {
                if(!readPoem())
                    return false;
            }
            else if(sName == L"cite")
            {
                if(!readCite())
                    return false;
            }
            else if(sName == L"empty-line")
                m_sBody += L"\n";
            else if(sName == L"text-author")
                break;
            else
                return false;
        }
        // Читаем text-author (любое)
        if(m_oLightReader.GetName() == L"text-author")
        {
            do
            {
                if(m_oLightReader.GetName() != L"text-author")
                    return false;
                m_sBody += L"text-author\n";
                if(!readP())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        return true;
    }

    // Читает p
    bool readP()
    {
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        m_oLightReader.MoveToElement();
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode2(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"#text")
                m_sBody += L"text " + m_oLightReader.GetText() + L"\n";
            else if (sName == L"strong")
            {
                m_sBody += L"strong\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"emphasis")
            {
                m_sBody += L"emphasis\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"style")
            {
                m_sBody += L"style\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"a")
            {
                m_sBody += L"a\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"strikethrough")
            {
                m_sBody += L"strikethrough\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"sub")
            {
                m_sBody += L"sub\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"sup")
            {
                m_sBody += L"sup\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"code")
            {
                m_sBody += L"code\n";
                return readP();
            }
            else if(sName == L"image")
            {
                m_sBody += L"image\n";
                while(m_oLightReader.MoveToNextAttribute())
                    m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
            }
            else
                return false;
        }
        return true;
    }

    // Читает poem
    bool readPoem()
    {
        m_sBody += L"poem\n";
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        if(!m_oLightReader.ReadNextNode())
            return false;

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            if(!readTitle())
                return false;
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(!readEpigraph())
                    return false;
            } while(isSection(L"epigraph"));
        }

        // Читаем stanza (один или более)
        if(m_oLightReader.GetName() != L"stanza")
            return false;
        int nDeath = m_oLightReader.GetDepth() - 1;
        do
        {
            if(m_oLightReader.GetName() != L"stanza")
                break;

            // Читаем title (ноль или один)
            if(isSection(L"title"))
            {
                if(!readTitle())
                    return false;
                if(!m_oLightReader.ReadNextNode())
                    return false;
            }

            // Читаем subtitle (ноль или один)
            if(m_oLightReader.GetName() == L"subtitle")
            {
                m_sBody += L"subtitle\n";
                if(!readP())
                    return false;
                if(!m_oLightReader.ReadNextNode())
                    return false;
            }

            // Читаем v (один или более)
            if(m_oLightReader.GetName() != L"v")
                return false;
            int nVDeath = m_oLightReader.GetDepth() - 1;
            do
            {
                if(m_oLightReader.GetName() != L"v")
                    return false;
                m_sBody += L"v\n";
                if(!readP())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nVDeath));
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));

        // Читаем text-author (любое)
        if(m_oLightReader.GetName() == L"text-author")
        {
            do
            {
                if(m_oLightReader.GetName() != L"text-author")
                    break;
                m_sBody += L"text-author\n";
                if(!readP())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        // Читаем date (ноль или один)
        if(m_oLightReader.GetName() == L"date")
        {
            m_sBody += L"date\n";
            while(m_oLightReader.MoveToNextAttribute())
                m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
            m_oLightReader.MoveToElement();
        }

        return true;
    }

    // Читает cite
    bool readCite()
    {
        m_sBody += L"cite\n";
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                if(!readP())
                    return false;
            }
            else if(sName == L"subtitle")
            {
                m_sBody += L"subtitle\n";
                if(!readP())
                    return false;
            }
            else if(sName == L"empty-line")
                m_sBody += L"\n";
            else if(sName == L"poem")
            {
                if(!readPoem())
                    return false;
            }
            else if(sName == L"table")
            {
                if(!readTable())
                    return false;
            }
            else if(sName == L"text-author")
                break;
            else
                return false;
        }
        // Читаем text-author (любое)
        if(m_oLightReader.GetName() == L"text-author")
        {
            do
            {
                if(m_oLightReader.GetName() != L"text-author")
                    return false;
                m_sBody += L"text-author\n";
                if(!readP())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        return true;
    }

    // Читает table
    bool readTable()
    {
        m_sBody += L"table\n";
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        m_oLightReader.MoveToElement();
        int nDeath = m_oLightReader.GetDepth();
        if(!m_oLightReader.ReadNextSiblingNode(nDeath))
            return false;

        // Читаем tr (один или более)
        if(m_oLightReader.GetName() != L"tr")
            return false;
        do
        {
            if(m_oLightReader.GetName() != L"tr")
                return false;
            m_sBody += L"tr\n";
            int nTrDeath = m_oLightReader.GetDepth();
            while(m_oLightReader.ReadNextSiblingNode(nTrDeath))
            {
                // Читаем th (любое)
                if(m_oLightReader.GetName() == L"th")
                {
                    m_sBody += L"th\n";
                    if(!readP())
                        return false;
                }
                // Читаем td (любое)
                else if(m_oLightReader.GetName() == L"td")
                {
                    m_sBody += L"td\n";
                    if(!readP())
                        return false;
                }
                else
                    return false;
            }
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        return true;
    }

    // Читает section
    bool readSection()
    {
        m_sBody += L"section\n";
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        if(!m_oLightReader.ReadNextNode())
            return false;

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            if(!readTitle())
                return false;
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(!readEpigraph())
                    return false;
            } while(isSection(L"epigraph"));
        }

        // Читаем image (ноль или один)
        if(m_oLightReader.GetName() == L"image")
        {
            m_sBody += L"image\n";
            while(m_oLightReader.MoveToNextAttribute())
                m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем annotation (ноль или один)
        if(m_oLightReader.GetName() == L"annotation")
        {
            m_sBody += L"annotation\n";
            int nDeath = m_oLightReader.GetDepth();
            while(m_oLightReader.ReadNextSiblingNode(nDeath))
            {
                std::wstring sAnName = m_oLightReader.GetName();
                if(sAnName == L"p")
                {
                    if(!readP())
                        return false;
                }
                else if(sAnName == L"poem")
                {
                    if(!readPoem())
                        return false;
                }
                else if(sAnName == L"cite")
                {
                    if(!readCite())
                        return false;
                }
                else if(sAnName == L"subtitle")
                {
                    m_sBody += L"subtitle\n";
                    if(!readP())
                        return false;
                }
                else if(sAnName == L"empty-line")
                    m_sBody += L"\n";
                else if(sAnName == L"table")
                {
                    if(!readTable())
                        return false;
                }
                else
                    return false;
            }
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем вложенные section (любое)
        if(m_oLightReader.GetName() == L"section")
        {
            int nDeath = m_oLightReader.GetDepth() - 1;
            do
            {
                if(m_oLightReader.GetName() != L"section")
                    return false;
                if(!readSection())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        // Читаем произвольный набор
        else
        {
            int nDeath = m_oLightReader.GetDepth() - 1;
            do
            {
                std::wstring sName = m_oLightReader.GetName();
                if(sName == L"p")
                {
                    if(!readP())
                        return false;
                }
                else if(sName == L"image")
                {
                    m_sBody += L"image\n";
                    while(m_oLightReader.MoveToNextAttribute())
                        m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
                    m_oLightReader.MoveToElement();
                }
                else if(sName == L"poem")
                {
                    if(!readPoem())
                        return false;
                }
                else if(sName == L"subtitle")
                {
                    m_sBody += L"subtitle\n";
                    if(!readP())
                        return false;
                }
                else if(sName == L"cite")
                {
                    if(!readCite())
                        return false;
                }
                else if(sName == L"empty-line")
                    m_sBody += L"\n";
                else if(sName == L"table")
                {
                    if(!readTable())
                        return false;
                }
                else
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        return true;
    }

    // Читает body
    bool readBody()
    {
        // Читаем image (ноль или один)
        if(isSection(L"image"))
        {
            m_sBody += L"image\n";
            while(m_oLightReader.MoveToNextAttribute())
                m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            if(!readTitle())
                return false;
            if(!m_oLightReader.ReadNextNode())
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(!readEpigraph())
                    return false;
            } while(isSection(L"epigraph"));
        }

        // Читаем section (один или более)
        if(m_oLightReader.GetName() != L"section")
            return false;
        int nDeath = m_oLightReader.GetDepth() - 1;
        do
        {
            if(m_oLightReader.GetName() != L"section")
                return false;
            if(!readSection())
                return false;
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        return true;
    }

    // Читает custom-info
    bool getCustomInfo()
    {
        if(!m_oLightReader.MoveToNextAttribute())
            return false;
        std::wstring sIntoType = m_oLightReader.GetText();
        if(!m_oLightReader.MoveToElement())
            return false;
        std::wstring sCustomInfo = content();
        if(sCustomInfo == L"")
            return false;
        m_mCustomInfo.insert(std::make_pair(sIntoType, sCustomInfo));
    }

    // Читает publish-info
    bool getPublishInfo()
    {
        // Читаем до body
        while(!isSection(L"body"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем book-name (ноль или один)
            if(sName == L"book-name")
            {
                if(m_pPublishInfo->m_pBookName)
                    return false;
                m_pPublishInfo->m_pBookName = new std::wstring[1];
                *m_pPublishInfo->m_pBookName = content();
            }
            // Читаем publisher (ноль или один)
            else if(sName == L"publisher")
            {
                if(m_pPublishInfo->m_pPublisher)
                    return false;
                m_pPublishInfo->m_pPublisher = new std::wstring[1];
                *m_pPublishInfo->m_pPublisher = content();
            }
            // Читаем city (ноль или один)
            else if(sName == L"city")
            {
                if(m_pPublishInfo->m_pCity)
                    return false;
                m_pPublishInfo->m_pCity = new std::wstring[1];
                *m_pPublishInfo->m_pCity = content();
            }
            // Читаем year (ноль или один)
            else if(sName == L"year")
            {
                if(m_pPublishInfo->m_pYear)
                    return false;
                m_pPublishInfo->m_pYear = new std::wstring[1];
                *m_pPublishInfo->m_pYear = content();
            }
            // Читаем isbn (ноль или один)
            else if(sName == L"isbn")
            {
                if(m_pPublishInfo->m_pIsbn)
                    return false;
                m_pPublishInfo->m_pIsbn = new std::wstring[1];
                *m_pPublishInfo->m_pIsbn = content();
            }
            // Читаем sequence (любое)
            else if(sName == L"sequence")
            {
                if(!m_oLightReader.MoveToNextAttribute())
                    return false;
                std::wstring sSName = m_oLightReader.GetText();
                std::wstring sSNumber = L"";
                if(m_oLightReader.MoveToNextAttribute())
                    sSNumber = m_oLightReader.GetText();
                m_pPublishInfo->m_mSequence.insert(std::make_pair(sSName, sSNumber));
            }
            else if(sName == L"custom-info")
            {
                break;
            }
        }
        return true;
    }

    // Читает document-info
    bool getDocumentInfo()
    {
        // Читаем author (один или более)
        if(!isSection(L"author"))
            return false;
        do
        {
            if(!getAuthor(m_oDocumentInfo.m_arAuthors))
                return false;
        } while(isSection(L"author"));

        // Читаем program-used (ноль или один)
        if(m_oLightReader.GetName() == L"program-used")
        {
            m_oDocumentInfo.m_pProgramUsed = new std::wstring[1];
            *m_oDocumentInfo.m_pProgramUsed = content();
        }

        // Читаем date
        if(m_oLightReader.GetName() != L"date")
            if(!isSection(L"date"))
                return false;
        std::wstring sDate = L"";
        if(m_oLightReader.MoveToNextAttribute())
        {
            sDate = m_oLightReader.GetText();
            m_oLightReader.MoveToElement();
        }
        m_oDocumentInfo.m_pDate = make_pair(sDate, content());

        // Читаем до id
        while(!isSection(L"id"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем src-url (любое)
            if(sName == L"src-url")
            {
                m_oDocumentInfo.m_arSrcUrl.push_back(content());
            }
            // Читаем src-ocr (ноль или один)
            else if(sName == L"src-ocr")
            {
                if(m_oDocumentInfo.m_pSrcOcr)
                    return false;
                m_oDocumentInfo.m_pSrcOcr = new std::wstring[1];
                *m_oDocumentInfo.m_pSrcOcr = content();
            }
        }

        // Читаем id
        if(m_oLightReader.GetName() != L"id")
            return false;
        m_oDocumentInfo.m_sId = content();

        // Читаем version
        if(!isSection(L"version"))
            return false;
        m_oDocumentInfo.m_sVersion = content();

        // Читаем до body
        while(!isSection(L"body"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем history (ноль или один)
            if(sName == L"history")
            {
                // Содержит форматированный текст - НЕ РЕАЛИЗОВАНО
                if(m_oDocumentInfo.m_pHistory)
                    return false;
                m_oDocumentInfo.m_pHistory = new std::wstring[1];
                *m_oDocumentInfo.m_pHistory = L"";
                int nDepth = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nDepth))
                {
                    *m_oDocumentInfo.m_pHistory += content();
                }
            }
            else if(sName == L"publish-info")
            {
                break;
            }
            else if(sName == L"custom-info")
            {
                break;
            }
        }
        return true;
    }

    // Читает title-info и src-title-info
    bool getTitleInfo(STitleInfo& oTitleInfo)
    {
        // Читаем genre (один или более)
        if(!isSection(L"genre"))
            return false;
        do
        {
            oTitleInfo.m_arGenres.push_back(content());
        } while(isSection(L"genre"));

        // Читаем author (один или более)
        if(m_oLightReader.GetName() != L"author")
            return false;
        do
        {
            if(!getAuthor(oTitleInfo.m_arAuthors))
                return false;
        } while(isSection(L"author"));

        // Читаем book-title
        if(m_oLightReader.GetName() != L"book-title")
            return false;
        oTitleInfo.m_sBookTitle = content();

        // Читаем до lang
        while(!isSection(L"lang"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
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
                *oTitleInfo.m_pKeywords = content();
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
                *oTitleInfo.m_pDate = make_pair(sDate, content());
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
                *oTitleInfo.m_pCoverpage = m_oLightReader.GetText();
                m_oLightReader.MoveToElement();
            }
        }

        // Читаем lang
        if(m_oLightReader.GetName() != L"lang")
            return false;
        oTitleInfo.m_sLang = content();

        // Читаем до document-info
        while(!isSection(L"document-info"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем src-lang (ноль или один)
            if(sName == L"src-lang")
            {
                if(oTitleInfo.m_pSrcLang)
                    return false;
                oTitleInfo.m_pSrcLang = new std::wstring[1];
                *oTitleInfo.m_pSrcLang = content();
            }
            // Читаем translator (любое)
            else if(sName == L"translator")
            {
                if(!getAuthor(oTitleInfo.m_arTranslator))
                    return false;
            }
            // Читаем sequence (любое)
            else if(sName == L"sequence")
            {
                if(!m_oLightReader.MoveToNextAttribute())
                    return false;
                std::wstring sSName = m_oLightReader.GetText();
                std::wstring sSNumber = L"";
                if(m_oLightReader.MoveToNextAttribute())
                    sSNumber = m_oLightReader.GetText();
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

    std::string contentA()
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
void CFb2File::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sDstFolder = sFolder;
}

// Функция для теста. Возвращает содержание body
std::wstring CFb2File::GetText()
{
    return m_internal->m_sBody;
}

// Проверяет, соответствует ли fb2 файл формату
// sPath - путь к сохраненному файлу, sDirectory - путь к сохраненным картинкам
int CFb2File::Convert (const std::wstring& sPath, const std::wstring& sDirectory)
{
    // Читаем description
    if(!m_internal->isSection(L"description"))
        return false;

    // Читаем title-info
    if(!m_internal->isSection(L"title-info"))
        return false;
    if(!m_internal->getTitleInfo(m_internal->m_oTitleInfo))
        return false;

    // Читаем src-title-info (ноль или один)
    if(m_internal->m_oLightReader.GetName() == L"src-title-info")
    {
        m_internal->m_pSrcTitleInfo = new STitleInfo();
        if(!m_internal->getTitleInfo(*m_internal->m_pSrcTitleInfo))
            return false;
    }

    // Читаем document-info
    if(m_internal->m_oLightReader.GetName() != L"document-info")
        return false;
    if(!m_internal->getDocumentInfo())
        return false;

    // Читаем publish-info (ноль или один)
    if(m_internal->m_oLightReader.GetName() == L"publish-info")
    {
        m_internal->m_pPublishInfo = new SPublishInfo();
        if(!m_internal->getPublishInfo())
            return false;
    }

    // Читаем custom-info (любое)
    if(m_internal->m_oLightReader.GetName() == L"custom-info")
    {
        do
        {
            if(!m_internal->getCustomInfo())
                return false;
        } while(m_internal->isSection(L"custom-info"));
    }

    // Читаем body
    if(m_internal->m_oLightReader.GetName() != L"body")
        return false;
    m_internal->m_sBody = L"";
    do
    {
        /*
        if(m_internal->m_oLightReader.MoveToNextAttribute())
        {
            if(m_internal->m_oLightReader.GetName() == L"name" &&
               m_internal->m_oLightReader.GetText() == L"notes")
            {

            }
        }
        */
        if(!m_internal->readBody())
            return false;
    } while(m_internal->isSection(L"body"));

    std::wstring sMediaDirectory = sDirectory + L"/media";
    NSDirectory::CreateDirectory(sMediaDirectory);
    while(m_internal->m_oLightReader.ReadNextNode())
    {
        // Читает картинки
        if(m_internal->m_oLightReader.GetName() == L"binary")
        {
            std::wstring sId;
            std::wstring sContentType;

            // Читает первый атрибут
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            std::wstring sName = m_internal->m_oLightReader.GetName();
            // Читает id
            if(sName == L"id")
                sId = m_internal->m_oLightReader.GetText();
            // Читает content-type
            else if(sName == L"content-type")
                sContentType = m_internal->m_oLightReader.GetText();
            else
                return false;

            // Читает второй атрибут
            if(!m_internal->m_oLightReader.MoveToNextAttribute())
                return false;
            sName = m_internal->m_oLightReader.GetName();
            // Читает id
            if(sName == L"id")
                sId = m_internal->m_oLightReader.GetText();
            // Читает content-type
            else if(sName == L"content-type")
                sContentType = m_internal->m_oLightReader.GetText();
            else
                return false;

            m_internal->m_mImage.insert(std::make_pair(sId, sContentType));
            m_internal->m_oLightReader.MoveToElement();

            // Пишет картинку в файл
            NSFile::CFileBinary oImageWriter;
            if (!oImageWriter.CreateFileW(sMediaDirectory + L"/" + sId))
                return false;
            std::string sBase64 = m_internal->contentA();
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
