#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../OfficeUtils/src/OfficeUtils.h"

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

    std::wstring* m_pKeywords;                      // Ключевые слова
    std::pair<std::wstring, std::wstring>* m_pDate; // Дата
    std::wstring* m_pSrcLang;                       // Язык до перевода

    std::map<std::wstring, std::wstring> m_mSequence; // Серии книг

    STitleInfo()
    {
        m_pKeywords = NULL;
        m_pDate = NULL;
        m_pSrcLang = NULL;
    }

    ~STitleInfo()
    {
        m_arGenres.clear();
        m_arAuthors.clear();
        m_arTranslator.clear();

        m_mSequence.clear();

        RELEASEARRAYOBJECTS(m_pKeywords);
        RELEASEARRAYOBJECTS(m_pDate);
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
    XmlUtils::CXmlLiteReader m_oLightReader;  // SAX Reader
    STitleInfo m_oTitleInfo;                  // Данные о книге
    SDocumentInfo m_oDocumentInfo;            // Информация об fb2-документе
    NSStringUtils::CStringBuilder m_oBuilder; // Текст
    std::wstring m_sTmpFolder;                // Рабочая папка

    int m_nImageId;     // ID картинки
    int m_nFootnoteId; // ID ссылки

    STitleInfo* m_pSrcTitleInfo;  // Данные об исходнике книги
    SPublishInfo* m_pPublishInfo; // Сведения об издании книги

    std::map<std::wstring, std::wstring> m_mFootnotes;  // Ссылки
    std::map<std::wstring, std::wstring> m_mImages;     // Картинки
    std::map<std::wstring, std::wstring> m_mCustomInfo; // Произвольная информация

public:
    CFb2File_Private()
    {
        m_pSrcTitleInfo = NULL;
        m_pPublishInfo = NULL;
        m_nImageId = 1;
        m_nFootnoteId = 1;
    }

    ~CFb2File_Private()
    {
        m_oLightReader.Clear();
        m_mFootnotes.clear();
        m_mImages.clear();
        m_mCustomInfo.clear();
        if(m_pSrcTitleInfo)
            delete m_pSrcTitleInfo;
        if(m_pPublishInfo)
            delete m_pPublishInfo;
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

    // Читает image
    // НЕ имеет право писать p
    bool readImage(NSStringUtils::CStringBuilder& oBuilder)
    {
        // Читаем href
        std::wstring sImageName = L"";
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            size_t nLen = sName.length() - 4;
            if(sName.substr(nLen > 0 ? nLen : 0) == L"href")
            {
                sImageName = m_oLightReader.GetText().substr(1);
                break;
            }
        }
        if(sImageName == L"")
            return false;
        m_oLightReader.MoveToElement();

        // Пишем image в файл
        std::wstring sImageId = std::to_wstring(m_nImageId);
        oBuilder += L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">";
        // extent
        // РАЗМЕРЫ КАРТИНКИ
        oBuilder += L"<wp:extent cx=\"5000000\" cy=\"5000000\"/>";
        // docPr
        oBuilder += L"<wp:docPr id=\"";
        oBuilder += sImageId;
        oBuilder += L"\" name=\"\"/>";
        // graphic
        oBuilder += L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">";
        // pic:nvPicPr
        oBuilder += L"<pic:nvPicPr><pic:cNvPr id=\"";
        oBuilder += sImageId;
        oBuilder += L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr>";
        // pic:blipFill
        // РЕЛЬСЫ rPic + id
        oBuilder += L"<pic:blipFill><a:blip r:embed=\"rPic";
        oBuilder += sImageId;
        oBuilder += L"\"/><a:stretch/></pic:blipFill>";
        // pic:spPr
        oBuilder += L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/>";
        // РАЗМЕРЫ КАРТИНКИ
        oBuilder += L"<a:ext cx=\"5000000\" cy=\"5000000\"/>";
        oBuilder += L"</a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr>";

        oBuilder += L"</pic:pic></a:graphicData></a:graphic>";
        // Конец записи
        oBuilder += L"</wp:inline></w:drawing></w:r>";

        m_mImages.insert(std::make_pair(sImageName, L"rPic" + sImageId));
        m_nImageId++;
        return true;
    }

    // Читает title
    // имеет право писать p
    bool readTitle(std::wstring sLevel)
    {
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            m_oBuilder += L"<w:p>";
            // Пишем title + sLevel
            m_oBuilder += L"<w:pPr><w:pStyle w:val=\"";
            m_oBuilder += sLevel;
            m_oBuilder += L"\"/></w:pPr>";

            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                if(!readP(L"", m_oBuilder))
                    return false;
            }
            else if(sName == L"empty-line")
            {
                m_oBuilder += L"<w:r><w:t></w:t></w:r>";
            }
            else
                return false;
            m_oBuilder += L"</w:p>";
        }
        return true;
    }

    // Читает epigraph
    // имеет право писать p
    bool readEpigraph()
    {
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
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
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:r><w:t></w:t></w:r>";
                m_oBuilder += L"</w:p>";
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
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"epigraph-text-author\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        return true;
    }

    // Читает p
    // НЕ имеет право писать p
    // sRStyle - накопленный стиль
    bool readP(std::wstring sRStyle, NSStringUtils::CStringBuilder& oBuilder)
    {
        // Читаем id, style, xml:lang
        /*
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        m_oLightReader.MoveToElement();
        */

        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode2(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем обычный текст
            if(sName == L"#text")
            {
                oBuilder += L"<w:r>";
                // Стиль текста
                oBuilder += L"<w:rPr>";
                oBuilder += sRStyle;
                oBuilder += L"</w:rPr>";
                // Сам текст
                oBuilder += L"<w:t xml:space=\"preserve\">";
                oBuilder += m_oLightReader.GetText();
                oBuilder += L"</w:t>";
                oBuilder += L"</w:r>";

            }
            // Читаем полужирный текст
            else if (sName == L"strong")
            {
                if(!readP(sRStyle + L"<w:b/>", oBuilder))
                    return false;
            }
            // Читаем курсивный текст
            else if(sName == L"emphasis")
            {
                if(!readP(sRStyle + L"<w:i/>", oBuilder))
                    return false;
            }
            // Читаем стилизованный текст
            else if(sName == L"style")
            {
                // ИГНОРИРУЕМ
                // if(!readP(true, sRStyle + L"<w:rStyle w:val=\"ЗНАЧЕНИЕ\"/>"))
                if(!readP(sRStyle, oBuilder))
                    return false;
            }
            // Читаем ссылку
            else if(sName == L"a")
            {
                // Читаем href
                std::wstring sHyperName = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    std::wstring sTName = m_oLightReader.GetName();
                    size_t nLen = sTName.length() - 4;
                    if(sTName.substr(nLen > 0 ? nLen : 0) == L"href")
                    {
                        sHyperName = m_oLightReader.GetText().substr(1);
                        break;
                    }
                }
                if(sHyperName == L"")
                    return false;
                m_oLightReader.MoveToElement();

                // Читаем текст внутри ссылки
                if(!readP(sRStyle, oBuilder))
                    return false;

                // Пишем ссылку
                std::wstring sFootnoteId = std::to_wstring(m_nFootnoteId);
                oBuilder += L"<w:r>";
                // Стиль ссылки
                oBuilder += L"<w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr>";
                oBuilder += L"<w:footnoteReference w:id=\"";
                oBuilder += sFootnoteId;
                oBuilder += L"\"/></w:r>";

                m_mFootnotes.insert(std::make_pair(sHyperName, sFootnoteId));
                m_nFootnoteId++;
            }
            // Читаем вычеркнутый текст
            else if(sName == L"strikethrough")
            {
                if(!readP(sRStyle + L"<w:strike/>", oBuilder))
                    return false;
            }
            // Читает нижний текст
            else if(sName == L"sub")
            {
                if(!readP(sRStyle + L"<w:vertAlign w:val=\"subscript\"/>", oBuilder))
                    return false;
            }
            // Читает верхний текст
            else if(sName == L"sup")
            {
                if(!readP(sRStyle + L"<w:vertAlign w:val=\"superscript\"/>", oBuilder))
                    return false;
            }
            // Читает код
            else if(sName == L"code")
            {
                if(!readP(sRStyle + L"<w:rStyle w:val=\"code\"/>", oBuilder))
                    return false;
            }
            // Читает картинку в тексте
            else if(sName == L"image")
            {
                if(!readImage(oBuilder))
                    return false;
            }
            else
                return false;
        }
        return true;
    }

    // Читает poem
    // имеет право писать p
    bool readPoem()
    {
        /*
        while(m_oLightReader.MoveToNextAttribute())
            m_sBody += L"\t" + m_oLightReader.GetName() + L" " + m_oLightReader.GetText() + L"\n";
        if(!m_oLightReader.ReadNextNode())
            return false;
        */
        int nDeath = m_oLightReader.GetDepth();
        if(!m_oLightReader.ReadNextSiblingNode(nDeath))
            return false;

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            if(!readTitle(L"title-poem"))
                return false;
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(m_oLightReader.GetName() != L"epigraph")
                    break;
                if(!readEpigraph())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        // Читаем stanza (один или более)
        if(m_oLightReader.GetName() != L"stanza")
            return false;
        int nSDeath = m_oLightReader.GetDepth();
        do
        {
            if(m_oLightReader.GetName() != L"stanza")
                break;
            if(!m_oLightReader.ReadNextSiblingNode(nSDeath))
                return false;

            // Читаем title (ноль или один)
            if(m_oLightReader.GetName() == L"title")
            {
                if(!readTitle(L"title-stanza"))
                    return false;
                if(!m_oLightReader.ReadNextSiblingNode(nSDeath))
                    return false;
            }

            // Читаем subtitle (ноль или один)
            if(m_oLightReader.GetName() == L"subtitle")
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"stanza-subtitle\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
                if(!m_oLightReader.ReadNextSiblingNode(nSDeath))
                    return false;
            }

            // Читаем v (один или более)
            if(m_oLightReader.GetName() != L"v")
                return false;
            do
            {
                if(m_oLightReader.GetName() != L"v")
                    return false;
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            } while(m_oLightReader.ReadNextSiblingNode(nSDeath));
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));

        // Читаем text-author (любое)
        if(m_oLightReader.GetName() == L"text-author")
        {
            do
            {
                if(m_oLightReader.GetName() != L"text-author")
                    break;
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"poem-text-author\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        // Читаем date (ноль или один)
        if(m_oLightReader.GetName() == L"date")
        {
            m_oBuilder += L"<w:p>";
            m_oBuilder += L"<w:pPr><w:pStyle w:val=\"poem-date\"/></w:pPr>";
            m_oBuilder += L"<w:r><w:t>";
            m_oBuilder += content();
            m_oBuilder += L"</w:t></w:r>";
            m_oBuilder += L"</w:p>";
        }

        return true;
    }

    // Читает cite
    // имеет право писать p
    bool readCite()
    {
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            }
            else if(sName == L"subtitle")
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"cite-subtitle\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            }
            else if(sName == L"empty-line")
                m_oBuilder += L"<w:p><w:r><w:t></w:t></w:r></w:p>";
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
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"cite-text-author\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        return true;
    }

    // Читает table
    bool readTable()
    {
        // Стиль таблицы
        m_oBuilder += L"<w:tbl>";
        m_oBuilder += L"<w:tblPr><w:tblStyle w:val=\"table\"/><w:tblW w:w=\"0\" w:type=\"auto\"/></w:tblPr><w:tblGrid/>";

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
            m_oBuilder += L"<w:tr>";
            int nTrDeath = m_oLightReader.GetDepth();
            while(m_oLightReader.ReadNextSiblingNode(nTrDeath))
            {
                m_oBuilder += L"<w:tc><w:p>";

                // Читаем th (любое)
                if(m_oLightReader.GetName() == L"th")
                {
                    m_oBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
                    if(!readP(L"<w:b/>", m_oBuilder))
                        return false;
                }
                // Читаем td (любое)
                else if(m_oLightReader.GetName() == L"td")
                {
                    m_oBuilder += L"<w:pPr><w:jc w:val=\"right\"/></w:pPr>";
                    if(!readP(L"", m_oBuilder))
                        return false;
                }
                else
                    return false;

                m_oBuilder += L"</w:p></w:tc>";
            }
            m_oBuilder += L"</w:tr>";
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        m_oBuilder += L"</w:tbl>";
        // Пустая строка после таблицы
        m_oBuilder += L"<w:p></w:p>";
        return true;
    }

    // Читает annotation
    // имеет право писать p
    bool readAnnotation()
    {
        int nADeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nADeath))
        {
            std::wstring sAnName = m_oLightReader.GetName();
            if(sAnName == L"p")
            {
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"annotation\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
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
                m_oBuilder += L"<w:p>";
                m_oBuilder += L"<w:pPr><w:pStyle w:val=\"annotation-subtitle\"/></w:pPr>";
                if(!readP(L"", m_oBuilder))
                    return false;
                m_oBuilder += L"</w:p>";
            }
            else if(sAnName == L"empty-line")
                m_oBuilder += L"<w:p><w:r><w:t></w:t></w:r></w:p>";
            else if(sAnName == L"table")
            {
                if(!readTable())
                    return false;
            }
            else
                return false;
        }
        return true;
    }

    // Читает section
    // имеет право писать p
    bool readSection(unsigned int nLevel)
    {
        int nDeath = m_oLightReader.GetDepth();
        if(!m_oLightReader.ReadNextSiblingNode(nDeath))
            return false;

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            if(!readTitle(L"title" + (nLevel == 0 ? L"" : std::to_wstring(nLevel))))
                return false;
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(m_oLightReader.GetName() != L"epigraph")
                    break;
                if(!readEpigraph())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        // Читаем image (ноль или один)
        if(m_oLightReader.GetName() == L"image")
        {
            m_oBuilder += L"<w:p>";
            m_oBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
            if(!readImage(m_oBuilder))
                return false;
            m_oBuilder += L"</w:p>";
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем annotation (ноль или один)
        if(m_oLightReader.GetName() == L"annotation")
        {
            if(!readAnnotation())
                return false;
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем вложенные section (любое)
        if(m_oLightReader.GetName() == L"section")
        {
            do
            {
                if(m_oLightReader.GetName() != L"section")
                    return false;
                m_oBuilder += L"<w:p>";
                if(!readSection(nLevel + 1))
                    return false;
                m_oBuilder += L"</w:p>";
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }
        // Читаем произвольный набор
        else
        {
            do
            {
                std::wstring sName = m_oLightReader.GetName();
                if(sName == L"p")
                {
                    // Стиль section-p
                    m_oBuilder += L"<w:p>";
                    m_oBuilder += L"<w:pPr><w:pStyle w:val=\"section-p\"/></w:pPr>";
                    if(!readP(L"", m_oBuilder))
                        return false;
                    m_oBuilder += L"</w:p>";
                }
                else if(sName == L"image")
                {
                    // Стиль section-image
                    m_oBuilder += L"<w:p>";
                    m_oBuilder += L"<w:pPr><w:pStyle w:val=\"section-image\"/></w:pPr>";
                    if(!readImage(m_oBuilder))
                        return false;
                    m_oBuilder += L"</w:p>";
                }
                else if(sName == L"poem")
                {
                    if(!readPoem())
                        return false;
                }
                else if(sName == L"subtitle")
                {
                    // Стиль section-subtitle
                    m_oBuilder += L"<w:p>";
                    m_oBuilder += L"<w:pPr><w:pStyle w:val=\"section-subtitle\"/></w:pPr>";
                    if(!readP(L"", m_oBuilder))
                        return false;
                    m_oBuilder += L"</w:p>";
                }
                else if(sName == L"cite")
                {
                    if(!readCite())
                        return false;
                }
                else if(sName == L"empty-line")
                    m_oBuilder += L"<w:p><w:r><w:t></w:t></w:r></w:p>";
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
    // имеет право писать p
    bool readBody()
    {
        m_oBuilder += L"<w:body>";

        int nDeath = m_oLightReader.GetDepth();
        if(!m_oLightReader.ReadNextSiblingNode(nDeath))
            return false;

        // Читаем image (ноль или один)
        if(m_oLightReader.GetName() == L"image")
        {
            m_oBuilder += L"<w:p>";
            m_oBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
            if(!readImage(m_oBuilder))
                return false;
            m_oBuilder += L"</w:p>";
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем title (ноль или один)
        if(m_oLightReader.GetName() == L"title")
        {
            // 0 - заголовок книги
            if(!readTitle(L"title"))
                return false;
            if(!m_oLightReader.ReadNextSiblingNode(nDeath))
                return false;
        }

        // Читаем epigraph (любое)
        if(m_oLightReader.GetName() == L"epigraph")
        {
            do
            {
                if(m_oLightReader.GetName() != L"epigraph")
                    break;
                if(!readEpigraph())
                    return false;
            } while(m_oLightReader.ReadNextSiblingNode(nDeath));
        }

        // Читаем section (один или более)
        if(m_oLightReader.GetName() != L"section")
            return false;
        do
        {
            if(m_oLightReader.GetName() != L"section")
                return false;
            if(!readSection(1))
                return false;
        } while(m_oLightReader.ReadNextSiblingNode(nDeath));

        m_oBuilder += L"<w:sectPr><w:footnotePr/><w:type w:val=\"nextPage\"/><w:pgSz w:w=\"11906\" w:h=\"16838\" w:orient=\"portrait\"/><w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"709\" w:footer=\"709\" w:gutter=\"0\"/><w:cols w:num=\"1\" w:sep=\"0\" w:space=\"708\" w:equalWidth=\"1\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body>";
        return true;
    }

    // Читает body с именем notes
    bool readNotes(NSStringUtils::CStringBuilder& oFootnotes)
    {
        int nFootnote = 1;
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            // Читаем section (один или более)
            if(m_oLightReader.GetName() == L"section")
            {
                // Читаем id
                std::wstring sFootnoteName = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"id")
                    {
                        sFootnoteName = m_oLightReader.GetText();
                        break;
                    }
                }
                if(sFootnoteName == L"")
                    return false;
                m_oLightReader.MoveToElement();

                // Ищем сноску в списке требующихся
                std::map<std::wstring, std::wstring>::iterator it = m_mFootnotes.find(sFootnoteName);
                if(it != m_mFootnotes.end())
                {
                    // Пишем сноску
                    oFootnotes += L"<w:footnote w:id=\"";
                    oFootnotes += it->second;
                    oFootnotes += L"\">";

                    // Читаем внутренность section
                    int nSDepth = m_oLightReader.GetDepth();
                    while(m_oLightReader.ReadNextSiblingNode(nSDepth))
                    {
                        std::wstring sName = m_oLightReader.GetName();
                        if(sName == L"title")
                        {
                            int nTDepth = m_oLightReader.GetDepth();
                            while(m_oLightReader.ReadNextSiblingNode(nTDepth))
                            {
                                if(m_oLightReader.GetName() == L"p")
                                {
                                    // Стиль section-p
                                    oFootnotes += L"<w:p>";
                                    oFootnotes += L"<w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                                    oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                                    if(!readP(L"", oFootnotes))
                                        return false;
                                    oFootnotes += L"</w:p>";
                                }
                            }
                        }
                        else if(sName == L"p")
                        {
                            // Стиль section-p
                            oFootnotes += L"<w:p>";
                            oFootnotes += L"<w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                            oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                            if(!readP(L"", oFootnotes))
                                return false;
                            oFootnotes += L"</w:p>";
                        }
                    }

                    oFootnotes += L"</w:footnote>";

                    // Удаляем сноску из списка требующихся
                    m_mFootnotes.erase(it);
                }
            }
        }
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
                if(m_oDocumentInfo.m_pHistory)
                    return false;
                m_oDocumentInfo.m_pHistory = new std::wstring[1];
                *m_oDocumentInfo.m_pHistory = L"";
                int nDepth = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nDepth))
                {
                    // *m_oDocumentInfo.m_pHistory += content();
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

        bool bAnnotation = false;
        bool bCoverpage = false;
        // Читаем до lang
        while(!isSection(L"lang"))
        {
            if(m_oLightReader.GetDepth() == 0)
                return false;
            std::wstring sName = m_oLightReader.GetName();

            // Читаем annotation (ноль или один)
            if(sName == L"annotation")
            {
                if(bAnnotation)
                    return false;
                bAnnotation = true;

                m_oBuilder += L"<w:body>";
                if(!readAnnotation())
                    return false;
                m_oBuilder += L"<w:sectPr><w:footnotePr/><w:type w:val=\"nextPage\"/><w:pgSz w:w=\"11906\" w:h=\"16838\" w:orient=\"portrait\"/><w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"709\" w:footer=\"709\" w:gutter=\"0\"/><w:cols w:num=\"1\" w:sep=\"0\" w:space=\"708\" w:equalWidth=\"1\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body>";
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
                if(bCoverpage)
                    return false;
                bCoverpage = true;

                m_oBuilder += L"<w:body>";
                // Читаем image (один или более)
                int nDeath = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nDeath))
                {
                    if(m_oLightReader.GetName() != L"image")
                        return false;
                    m_oBuilder += L"<w:p>";
                    m_oBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
                    if(!readImage(m_oBuilder))
                        return false;
                    m_oBuilder += L"</w:p>";
                }
                // Разрыв страницы
                m_oBuilder += L"<w:p><w:r><w:br w:type=\"page\"/></w:r></w:p>";
                m_oBuilder += L"<w:sectPr><w:footnotePr/><w:type w:val=\"nextPage\"/><w:pgSz w:w=\"11906\" w:h=\"16838\" w:orient=\"portrait\"/><w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"709\" w:footer=\"709\" w:gutter=\"0\"/><w:cols w:num=\"1\" w:sep=\"0\" w:space=\"708\" w:equalWidth=\"1\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body>";
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
    // Открывает файл на проверку
    if (!m_internal->m_oLightReader.FromFile(sFile))
        return false;

    // Читаем FictionBook
    if(!m_internal->isSection(L"FictionBook"))
        return false;

    m_internal->m_oLightReader.Clear();
    return true;
}

// Выставление рабочей папки
void CFb2File::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmpFolder = CorrectPathW(sFolder);
}

// Проверяет, соответствует ли fb2 файл формату
// sPath - путь к файлу fb2, sDirectory - директория, где формируется и создается docx
int CFb2File::Convert(const std::wstring& sPath, const std::wstring& sDirectory, bool bNeedDocx)
{
    // Копирование шаблона
    // присутствует КОСТЫЛЬ - путь к шаблону относительно теста
    std::wstring sTemplate = sDirectory + L"/../../../../template/";
    NSDirectory::CopyDirectory(sTemplate, sDirectory);

    // Начало файла
    m_internal->m_oBuilder += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\">";

    // Открываем файл на чтение
    if (!m_internal->m_oLightReader.FromFile(sPath))
        return FALSE;

    // Читаем FictionBook
    if(!m_internal->isSection(L"FictionBook"))
        return false;

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

    // Создаем сноски
    NSStringUtils::CStringBuilder oFootnotes;
    oFootnotes += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
    oFootnotes += L"<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
    oFootnotes += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote>";
    oFootnotes += L"<w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

    // Читаем body
    int nDeath = m_internal->m_oLightReader.GetDepth() - 1;
    if(m_internal->m_oLightReader.GetName() != L"body")
        return false;
    do
    {
        if(m_internal->m_oLightReader.GetName() != L"body")
            break;
        bool bNotes = false;
        while(m_internal->m_oLightReader.MoveToNextAttribute())
        {
            if(m_internal->m_oLightReader.GetName() == L"name" &&
               m_internal->m_oLightReader.GetText() == L"notes")
                bNotes = true;
        }
        m_internal->m_oLightReader.MoveToElement();
        if(bNotes)
        {
            if(!m_internal->readNotes(oFootnotes))
                return false;
        }
        else if(!m_internal->readBody())
            return false;
    } while(m_internal->m_oLightReader.ReadNextSiblingNode(nDeath));

    // Конец сносок
    oFootnotes += L"</w:footnotes>";
    // Пишем сноски в файл
    std::wstring sFootnotesDirectory = sDirectory + L"/word";
    NSFile::CFileBinary oFootnotesWriter;
    if (!oFootnotesWriter.CreateFileW(sFootnotesDirectory + L"/footnotes.xml"))
        return false;
    oFootnotesWriter.WriteStringUTF8(oFootnotes.GetData());
    oFootnotesWriter.CloseFile();

    // Конец документа
    m_internal->m_oBuilder += L"</w:document>";
    // Пишем документ в файл
    NSFile::CFileBinary oDocumentXmlWriter;
    if (!oDocumentXmlWriter.CreateFileW(sDirectory + L"/word/document.xml"))
        return false;
    oDocumentXmlWriter.WriteStringUTF8(m_internal->m_oBuilder.GetData());
    oDocumentXmlWriter.CloseFile();

    // Создаем рельсы
    NSStringUtils::CStringBuilder oRels;
    oRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
    oRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
    oRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
    oRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
    oRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
    oRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
    oRels += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";

    // Читает картинки
    std::wstring sMediaDirectory = sDirectory + L"/word/media";
    NSDirectory::CreateDirectory(sMediaDirectory);
    do
    {
        if(m_internal->m_oLightReader.GetName() == L"binary")
        {
            std::wstring sId = L"";
            std::wstring sContentType;

            // Читает атрибуты
            while(m_internal->m_oLightReader.MoveToNextAttribute())
            {
                std::wstring sName = m_internal->m_oLightReader.GetName();
                // Читает id
                if(sName == L"id")
                    sId = m_internal->m_oLightReader.GetText();
                // Читает content-type
                else if(sName == L"content-type")
                    sContentType = m_internal->m_oLightReader.GetText();
                else
                    return false;
            }
            m_internal->m_oLightReader.MoveToElement();
            if(sId == L"")
                return false;

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

            // Ищем картинку в списке требующихся
            std::map<std::wstring, std::wstring>::iterator it = m_internal->m_mImages.find(sId);
            if(it != m_internal->m_mImages.end())
            {
                // Запись картинок в рельсы
                oRels += L"<Relationship Id=\"";
                oRels += it->second;
                oRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
                oRels += it->first;
                oRels += L"\"/>";
                // Удаляем картинку из списка требующихся
                m_internal->m_mImages.erase(it);
            }
        }
    } while(m_internal->m_oLightReader.ReadNextSiblingNode(nDeath));

    // Если остались неразрешенные ссылки на картинки в тексте
    if(!m_internal->m_mImages.empty())
        return false;

    // Конец рельсов
    oRels += L"</Relationships>";
    // Пишем рельсы в файл
    std::wstring sRelsDirectory = sDirectory + L"/word/_rels";
    NSFile::CFileBinary oRelsWriter;
    if (!oRelsWriter.CreateFileW(sRelsDirectory + L"/document.xml.rels"))
        return false;
    oRelsWriter.WriteStringUTF8(oRels.GetData());
    oRelsWriter.CloseFile();

    // Архивим в docx
    if(bNeedDocx)
    {
        COfficeUtils oZip;
        HRESULT oRes = oZip.CompressFileOrDirectory(sDirectory, sDirectory + L"/res.docx");
        if(oRes == S_FALSE)
            return false;
    }

    return TRUE;
}
