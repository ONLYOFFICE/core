#include "Fb2File.h"
#include "../DesktopEditor/xml/include/xmlutils.h"
#include "../DesktopEditor/xml/include/xmlencoding.h"
#include "../DesktopEditor/common/Base64.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"
#include "../DesktopEditor/common/StringBuilder.h"
#include "../DesktopEditor/common/SystemUtils.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../OfficeUtils/src/OfficeUtils.h"
#include "./template/template.h"

#include <vector>
#include <map>

#ifndef VALUE2STR
#define VALUE_TO_STRING(x) #x
#define VALUE2STR(x) VALUE_TO_STRING(x)
#endif

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

    // Преобразование закодированного жанра
    static std::wstring definitionGenre(std::wstring sGenre)
    {
        std::wstring sRes = L"";
        if(sGenre == L"sf_fantasy")
            sRes = L"Фэнтези";
        return sRes;
    }

    // Разделитель не важен ,
    std::wstring getGenres()
    {
        std::wstring sRes = L"";
        for(std::wstring& vElem : m_arGenres)
            sRes += vElem + L", ";
        return sRes;
    }

    // Разделитель ;
    std::wstring getAuthors()
    {
        std::wstring sRes = L"";
        for(SAuthor& vElem : m_arAuthors)
            sRes += vElem.middle_name + L" " + vElem.first_name + L" " + vElem.last_name + L";";
        return sRes;
    }
};

// Описание информации о fb2-документе. Тэг document-info
struct SDocumentInfo
{
    std::vector<SAuthor> m_arAuthors;     // Авторы
    std::vector<std::wstring> m_arSrcUrl; // URL страницы

    std::wstring m_pDate; // Дата
    std::wstring m_sId;                            // Идентификатор
    std::wstring m_sVersion;                       // Версия документа

    std::wstring* m_pProgramUsed; // Использованные программы
    std::wstring* m_pSrcOcr;      // Автор текста
    std::wstring* m_pHistory;     // История

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
    std::wstring m_sTmpFolder;                // Рабочая папка

    int m_nContentsId;       // ID содержания
    int m_nCrossReferenceId; // ID перекрестной ссылки

    STitleInfo* m_pSrcTitleInfo;  // Данные об исходнике книги
    SPublishInfo* m_pPublishInfo; // Сведения об издании книги

    std::map<std::wstring, std::wstring> m_mFootnotes;           // Сноски
    std::map<std::wstring, std::vector<std::wstring>> m_mImages; // Картинки
    std::map<std::wstring, std::wstring> m_mCustomInfo;          // Произвольная информация

public:
    CFb2File_Private()
    {
        m_pSrcTitleInfo = NULL;
        m_pPublishInfo = NULL;
        m_nContentsId = 1;
        m_nCrossReferenceId = 1;
        m_sTmpFolder = L"";
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

    // wrapper для чтения из файла
    bool OpenFromFile(const std::wstring& sFile)
    {
        // считаем, что это хмл, так как проверка на формат будет раньше (x2t)
        std::string sFileContentUtf8 = XmlUtils::GetXmlContentAsUTF8(sFile);
        return m_oLightReader.FromStringA(sFileContentUtf8);
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
    void getAuthor(std::vector<SAuthor>& arAuthor)
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
        }
        arAuthor.push_back(oAuthor);
    }

    // Читает image
    // НЕ имеет право писать p
    void readImage(NSStringUtils::CStringBuilder& oBuilder)
    {
        // Читаем href
        std::wstring sImageName = L"";
        while(m_oLightReader.MoveToNextAttribute())
        {
            std::wstring sName = m_oLightReader.GetName();
            size_t nLen = (sName.length() > 4 ? sName.length() - 4 : 0);
            if(sName.substr(nLen) == L"href")
                sImageName = m_oLightReader.GetText().substr(1);
        }
        m_oLightReader.MoveToElement();

        readCrossReference(oBuilder);

        std::map<std::wstring, std::vector<std::wstring>>::iterator it = m_mImages.find(sImageName);
        if(it != m_mImages.end())
        {
            // Пишем картинку в файл
            oBuilder += L"<w:r><w:drawing><wp:inline distT=\"0\" distB=\"0\" distL=\"0\" distR=\"0\">";
            // extent
            oBuilder += L"<wp:extent cx=\"";
            oBuilder += it->second[1];
            oBuilder += L"\" cy=\"";
            oBuilder += it->second[2];
            oBuilder += L"\"/>";
            // docPr
            oBuilder += L"<wp:docPr id=\"";
            oBuilder += it->second[0];
            oBuilder += L"\" name=\"\"/>";
            // graphic
            oBuilder += L"<a:graphic xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\"><a:graphicData uri=\"http://schemas.openxmlformats.org/drawingml/2006/picture\"><pic:pic xmlns:pic=\"http://schemas.openxmlformats.org/drawingml/2006/picture\">";
            // pic:nvPicPr
            oBuilder += L"<pic:nvPicPr><pic:cNvPr id=\"";
            oBuilder += it->second[0];
            oBuilder += L"\" name=\"\"/><pic:cNvPicPr></pic:cNvPicPr></pic:nvPicPr>";
            // pic:blipFill
            // рельсы rPic + id
            oBuilder += L"<pic:blipFill><a:blip r:embed=\"rPic";
            oBuilder += it->second[0];
            oBuilder += L"\"/><a:stretch/></pic:blipFill>";
            // pic:spPr
            oBuilder += L"<pic:spPr bwMode=\"auto\"><a:xfrm><a:off x=\"0\" y=\"0\"/><a:ext cx=\"";
            oBuilder += it->second[1];
            oBuilder += L"\" cy=\"";
            oBuilder += it->second[2];
            oBuilder += L"\"/></a:xfrm><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></pic:spPr>";
            // Конец записи
            oBuilder += L"</pic:pic></a:graphicData></a:graphic></wp:inline></w:drawing></w:r>";
        }
    }

    // Читает title
    // имеет право писать p
    void readTitle(std::wstring sLevel, NSStringUtils::CStringBuilder& oBuilder)
    {
        bool bFirstP = true;
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            // Пишем заголовок
            if(m_oLightReader.GetName() == L"p")
            {
                // Пишем title + sLevel
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"";
                oBuilder += sLevel;
                oBuilder += L"\"/>";
                // Заголовок книги с новой страницы
                if(sLevel == L"title" && bFirstP)
                {
                    oBuilder += L"<w:pageBreakBefore/>";
                    bFirstP = false;
                }
                oBuilder += L"</w:pPr>";
                // Пишем ссылку от оглавления
                std::wstring sContentsId;
                if(sLevel == L"title1")
                {
                    sContentsId = std::to_wstring(m_nContentsId++);
                    oBuilder += L"<w:bookmarkStart w:id=\"";
                    oBuilder += sContentsId;
                    oBuilder += L"\" w:name=\"_Toc";
                    oBuilder += sContentsId;
                    oBuilder += L"\"/>";
                }
                readP(L"", oBuilder);
                // Пишем ссылку от оглавления
                if(sLevel == L"title1")
                {
                    oBuilder += L"<w:bookmarkEnd w:id=\"";
                    oBuilder += sContentsId;
                    oBuilder += L"\"/>";
                }
                oBuilder += L"</w:p>";
            }
        }
    }

    // Читает epigraph
    // имеет право писать p
    void readEpigraph(NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr>";
                readP(L"", oBuilder);
                oBuilder += L"</w:p>";
            }
            else if(sName == L"poem")
                readPoem(oBuilder);
            else if(sName == L"cite")
                readCite(oBuilder);
            else if(sName == L"empty-line")
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr></w:p>";
            else if(sName == L"text-author")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"epigraph-p\"/></w:pPr>";
                readP(L"<w:b/>", oBuilder);
                oBuilder += L"</w:p>";
            }
        }
    }

    // Читает p
    // НЕ имеет право писать p
    // sRStyle - накопленный стиль
    void readP(std::wstring sRStyle, NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

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
                oBuilder.WriteEncodeXmlString(m_oLightReader.GetText());
                oBuilder += L"</w:t></w:r>";

            }
            // Читаем полужирный текст
            else if (sName == L"strong")
                readP(sRStyle + L"<w:b/>", oBuilder);
            // Читаем курсивный текст
            else if(sName == L"emphasis")
                readP(sRStyle + L"<w:i/>", oBuilder);
            // Читаем стилизованный текст
            else if(sName == L"style")
                readP(sRStyle, oBuilder);
            // Читаем ссылку
            else if(sName == L"a")
            {
                // Читаем href
                std::wstring sFootnoteName = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    std::wstring sTName = m_oLightReader.GetName();
                    size_t nLen = (sTName.length() > 4 ? sTName.length() - 4 : 0);
                    if(sTName.substr(nLen) == L"href")
                    {
                        sFootnoteName = m_oLightReader.GetText().substr(1);
                        break;
                    }
                }
                m_oLightReader.MoveToElement();

                std::map<std::wstring, std::wstring>::iterator it = m_mFootnotes.find(sFootnoteName);
                if(it != m_mFootnotes.end())
                {
                    // Пробел перед текстом внутри сноски
                    oBuilder += L"<w:r><w:t xml:space=\"preserve\"> </w:t></w:r>";
                    // Читаем текст внутри сноски
                    readP(sRStyle, oBuilder);
                    // Стиль сноски
                    oBuilder += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr><w:footnoteReference w:id=\"";
                    oBuilder += it->second;
                    oBuilder += L"\"/></w:r>";
                }
                // Перекрестная ссылка
                else
                {
                    oBuilder += L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"";
                    oBuilder += sFootnoteName;
                    oBuilder += L"\">";
                    // Читаем текст внутри ссылки
                    readP(sRStyle + L"<w:rStyle w:val=\"cross\"/>", oBuilder);
                    oBuilder += L"</w:hyperlink>";
                }
            }
            // Читаем вычеркнутый текст
            else if(sName == L"strikethrough")
                readP(sRStyle + L"<w:strike/>", oBuilder);
            // Читает нижний текст
            else if(sName == L"sub")
                readP(sRStyle + L"<w:vertAlign w:val=\"subscript\"/>", oBuilder);
            // Читает верхний текст
            else if(sName == L"sup")
                readP(sRStyle + L"<w:vertAlign w:val=\"superscript\"/>", oBuilder);
            // Читает код
            else if(sName == L"code")
                readP(sRStyle + L"<w:rStyle w:val=\"code\"/>", oBuilder);
            // Читает картинку в тексте
            else if(sName == L"image")
                readImage(oBuilder);
        }
    }

    // Читает poem
    // имеет право писать p
    void readPoem(NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем title (ноль или один)
            if(sName == L"title")
                readTitle(L"title4", oBuilder);
            // Читаем epigraph (любое)
            else if(sName == L"epigraph")
                readEpigraph(oBuilder);
            // Читаем stanza (один или более)
            else if(sName == L"stanza")
            {
                int nSDeath = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nSDeath))
                {
                    std::wstring sSName = m_oLightReader.GetName();
                    // Читаем title (ноль или один)
                    if(sSName == L"title")
                        readTitle(L"title5", oBuilder);
                    // Читаем subtitle (ноль или один)
                    // Читаем v (один или более)
                    else if(sSName == L"v" || sSName == L"subtitle")
                    {
                        oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr>";
                        readP(L"", oBuilder);
                        oBuilder += L"</w:p>";
                    }
                }
            }
            // Читаем text-author (любое)
            else if(sName == L"text-author")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr>";
                readP(L"<w:b/>", oBuilder);
                oBuilder += L"</w:p>";
            }
            // Читаем date (ноль или один)
            else if(sName == L"date")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"v-stanza\"/></w:pPr><w:r><w:t>";
                oBuilder.WriteEncodeXmlString(content());
                oBuilder += L"</w:t></w:r></w:p>";
            }
        }
    }

    // Читает cite
    // имеет право писать p
    void readCite(NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            if(sName == L"p" || sName == L"subtitle")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr>";
                readP(L"", oBuilder);
                oBuilder += L"</w:p>";
            }
            else if(sName == L"empty-line")
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr></w:p>";
            else if(sName == L"poem")
                readPoem(oBuilder);
            else if(sName == L"table")
                readTable(oBuilder);
            else if(sName == L"text-author")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"cite-p\"/></w:pPr>";
                readP(L"<w:b/>", oBuilder);
                oBuilder += L"</w:p>";
            }
        }
    }

    // Читает table
    void readTable(NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        // Стиль таблицы
        oBuilder += L"<w:tbl><w:tblPr><w:tblStyle w:val=\"table-t\"/><w:tblW w:w=\"0\" w:type=\"auto\"/><w:tblLayout w:type=\"fixed\"/></w:tblPr>";

        NSStringUtils::CStringBuilder oTmpBuilder;
        int nGridCol = 0;
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            if(m_oLightReader.GetName() == L"tr")
            {
                int nTCol = 0;
                oTmpBuilder += L"<w:tr>";
                int nTrDeath = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nTrDeath))
                {
                    oTmpBuilder += L"<w:tc><w:tcPr><w:textDirection w:val=\"lrTb\"/><w:noWrap w:val=\"false\"/></w:tcPr><w:p>";

                    // Читаем th (любое)
                    if(m_oLightReader.GetName() == L"th")
                    {
                        if(++nTCol > nGridCol)
                            nGridCol = nTCol;
                        oTmpBuilder += L"<w:pPr><w:jc w:val=\"center\"/></w:pPr>";
                        readP(L"<w:b/>", oTmpBuilder);
                    }
                    // Читаем td (любое)
                    else if(m_oLightReader.GetName() == L"td")
                    {
                        if(++nTCol > nGridCol)
                            nGridCol = nTCol;
                        oTmpBuilder += L"<w:pPr><w:jc w:val=\"right\"/></w:pPr>";
                        readP(L"", oTmpBuilder);
                    }
                    oTmpBuilder += L"</w:p></w:tc>";
                }
                oTmpBuilder += L"</w:tr>";
            }
        }
        // Размеры таблицы
        std::wstring sGridCol = L"";
        if(nGridCol != 0)
            sGridCol = std::to_wstring((int)(9570.0 / (double)nGridCol));
        oBuilder += L"<w:tblGrid>";
        for(int i = 0; i < nGridCol; i++)
        {
            oBuilder += L"<w:gridCol w:w=\"";
            oBuilder += sGridCol;
            oBuilder += L"\"/>";
        }
        oBuilder += L"</w:tblGrid>";
        // Конец таблицы
        oBuilder += oTmpBuilder.GetData();
        oBuilder += L"</w:tbl>";
        // Пустая строка после таблицы
        oBuilder += L"<w:p></w:p>";
    }

    // Читает annotation
    // имеет право писать p
    void readAnnotation(NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        int nADeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nADeath))
        {
            std::wstring sAnName = m_oLightReader.GetName();
            if(sAnName == L"p" || sAnName == L"subtitle")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"annotation\"/></w:pPr>";
                readP(L"", oBuilder);
                oBuilder += L"</w:p>";
            }
            else if(sAnName == L"poem")
                readPoem(oBuilder);
            else if(sAnName == L"cite")
                readCite(oBuilder);
            else if(sAnName == L"empty-line")
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"annotation\"/></w:pPr></w:p>";
            else if(sAnName == L"table")
                readTable(oBuilder);
        }
    }

    // Читает перекрестные ссылки
    void readCrossReference(NSStringUtils::CStringBuilder& oBuilder)
    {
        // id для перекрестных ссылок
        while(m_oLightReader.MoveToNextAttribute())
        {
            if(m_oLightReader.GetName() == L"id")
            {
                std::wstring sCrossId = std::to_wstring(m_nCrossReferenceId++);
                oBuilder += L"<w:bookmarkStart w:id=\"";
                oBuilder += sCrossId;
                oBuilder += L"\" w:name=\"";
                oBuilder += m_oLightReader.GetText();
                oBuilder += L"\"/><w:bookmarkEnd w:id=\"";
                oBuilder += sCrossId;
                oBuilder += L"\"/>";
                break;
            }
        }
        m_oLightReader.MoveToElement();
    }

    // Читает section
    // имеет право писать p
    void readSection(unsigned int nLevel, NSStringUtils::CStringBuilder& oBuilder)
    {
        readCrossReference(oBuilder);

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем title (ноль или один)
            if(sName == L"title")
            {
                std::wstring sTitle = L"";
                if(nLevel < 10)
                    sTitle = L"title" + std::to_wstring(nLevel);
                readTitle(sTitle, oBuilder);
            }
            // Читаем epigraph (любое)
            else if(sName == L"epigraph")
                readEpigraph(oBuilder);
            // Читаем image (ноль или один)
            else if(sName == L"image")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
                readImage(oBuilder);
                oBuilder += L"</w:p>";
            }
            // Читаем annotation (ноль или один)
            else if(sName == L"annotation")
                readAnnotation(oBuilder);
            // Читаем вложенные section (любое)
            else if(sName == L"section")
                readSection(nLevel + 1, oBuilder);
            // Читаем произвольный набор
            else if(sName == L"p")
            {
                // Стиль section-p
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-p\"/></w:pPr>";
                readP(L"", oBuilder);
                oBuilder += L"</w:p>";
            }
            else if(sName == L"subtitle")
            {
                // Стиль section-subtitle
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-subtitle\"/></w:pPr>";
                readP(L"", oBuilder);
                oBuilder += L"</w:p>";
            }
            else if(sName == L"poem")
                readPoem(oBuilder);
            else if(sName == L"cite")
                readCite(oBuilder);
            else if(sName == L"empty-line")
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"section-p\"/></w:pPr></w:p>";
            else if(sName == L"table")
               readTable(oBuilder);
        }
    }

    // Читает body
    // имеет право писать p
    void readBody(NSStringUtils::CStringBuilder& oBuilder)
    {
        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем image (ноль или один)
            if(sName == L"image")
            {
                oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
                readImage(oBuilder);
                oBuilder += L"</w:p>";
            }
            // Читаем title (ноль или один)
            // 0 - заголовок книги
            else if(sName == L"title")
                readTitle(L"title", oBuilder);
            // Читаем epigraph (любое)
            else if(sName == L"epigraph")
                readEpigraph(oBuilder);
            // Читаем section (один или более)
            else if(sName == L"section")
                readSection(1, oBuilder);
        }
    }

    // Читает содержание, binary, body, сноски, description
    bool readText(const std::wstring& sPath, const std::wstring& sMediaDirectory, NSStringUtils::CStringBuilder& oContents, NSStringUtils::CStringBuilder& oRels, NSStringUtils::CStringBuilder& oFootnotes)
    {
        if(!m_oLightReader.IsValid())
        {
            // Открывает файл на проверку
            if (!OpenFromFile(sPath))
                return false;
            // Читаем FictionBook
            if(!isSection(L"FictionBook"))
                return false;
        }

        int nContentsId = 1;
        int nImageId = 1;
        int nFootnoteId = 1;
        oContents += L"<w:sdt><w:sdtPr><w:docPartObj><w:docPartGallery w:val=\"Table of Contents\"/><w:docPartUnique/></w:docPartObj></w:sdtPr><w:sdtContent>";

        int nDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем body
            if(sName == L"body")
            {
                // Сноски
                bool bNotes = false;
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"name" &&
                       m_oLightReader.GetText() == L"notes")
                        bNotes = true;
                }
                m_oLightReader.MoveToElement();
                // Читаем сноски
                if(bNotes)
                    readNotes(nFootnoteId, oFootnotes);
                // Читаем содержание
                else
                    readContents(nContentsId, oContents);
            }
            // Читаем картинки
            else if(sName == L"binary")
            {
                std::wstring sImageId = std::to_wstring(nImageId++);
                getImage(sImageId, sMediaDirectory, oRels);
            }
        }
        oContents += L"</w:sdtContent></w:sdt>";
        return true;
    }

    // Читает содержание
    void readContents(int& nContentsId, NSStringUtils::CStringBuilder& oContents)
    {
        bool bFirstP = true;
        int nBDeath = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nBDeath))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем section
            if(sName == L"section")
            {
                int nSDeath = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nSDeath))
                {
                    // Читаем title
                    if(m_oLightReader.GetName() == L"title")
                    {
                        std::wstring sContentsId = std::to_wstring(nContentsId++);
                        oContents += L"<w:p><w:pPr><w:pStyle w:val=\"contents\"/><w:tabs><w:tab w:val=\"right\" w:pos=\"9355\" w:leader=\"none\"/></w:tabs>";
                        // Абзац с новой страницы
                        if(bFirstP)
                            oContents += L"<w:pageBreakBefore/>";
                        oContents += L"</w:pPr>";
                        if(bFirstP)
                        {
                            oContents += L"<w:r><w:fldChar w:fldCharType=\"begin\"/><w:instrText xml:space=\"preserve\">TOC \\n \\h </w:instrText><w:fldChar w:fldCharType=\"separate\"/></w:r>";
                            bFirstP = false;
                        }
                        oContents += L"<w:hyperlink w:tooltip=\"Current Document\" w:anchor=\"_Toc";
                        oContents += sContentsId;
                        oContents += L"\" w:history=\"1\">";

                        int nTDeath = m_oLightReader.GetDepth();
                        while(m_oLightReader.ReadNextSiblingNode(nTDeath))
                        {
                            // Читаем p
                            if(m_oLightReader.GetName() == L"p")
                                readP(L"<w:rStyle w:val=\"contents-p\"/>", oContents);
                            // Вставляем пробел между словами
                            oContents += L"<w:r><w:t xml:space=\"preserve\"> </w:t></w:r>";
                        }
                        oContents += L"</w:hyperlink></w:p>";
                    }
                }
            }
        }
        if(!bFirstP)
            oContents += L"<w:p><w:r><w:fldChar w:fldCharType=\"end\"/></w:r></w:p>";
    }

    // Читает сноски
    void readNotes(int& nFootnoteId, NSStringUtils::CStringBuilder& oFootnotes)
    {
        int nBDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nBDepth))
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
                m_oLightReader.MoveToElement();
                if(sFootnoteName == L"")
                    continue;

                std::wstring sFootnoteId = std::to_wstring(nFootnoteId++);
                m_mFootnotes.insert(std::make_pair(sFootnoteName, sFootnoteId));
                // Пишем сноску
                oFootnotes += L"<w:footnote w:id=\"";
                oFootnotes += sFootnoteId;
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
                                oFootnotes += L"<w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                                oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                                readP(L"", oFootnotes);
                                oFootnotes += L"</w:p>";
                            }
                        }
                    }
                    else if(sName == L"p" || sName == L"subtitle")
                    {
                        // Стиль section-p
                        oFootnotes += L"<w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                        oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                        readP(L"", oFootnotes);
                        oFootnotes += L"</w:p>";
                    }
                    else if(sName == L"poem")
                    {
                        int nPDepth = m_oLightReader.GetDepth();
                        while(m_oLightReader.ReadNextSiblingNode(nPDepth))
                        {
                            std::wstring sPName = m_oLightReader.GetName();
                            // Читаем stanza (один или более)
                            if(sPName == L"stanza")
                            {
                                int nSDeath = m_oLightReader.GetDepth();
                                while(m_oLightReader.ReadNextSiblingNode(nSDeath))
                                {
                                    // Читаем v (один или более)
                                    if(m_oLightReader.GetName() == L"v")
                                    {
                                        oFootnotes += L"<w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                                        oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                                        readP(L"", oFootnotes);
                                        oFootnotes += L"</w:p>";
                                    }
                                }
                            }
                            // Читаем text-author (любое)
                            else if(sPName == L"text-author")
                            {
                                oFootnotes += L"<w:p><w:pPr><w:pStyle w:val=\"footnote-p\"/></w:pPr>";
                                oFootnotes += L"<w:r><w:rPr><w:rStyle w:val=\"footnote\"/></w:rPr></w:r>";
                                readP(L"", oFootnotes);
                                oFootnotes += L"</w:p>";
                            }
                        }
                    }
                }
                oFootnotes += L"</w:footnote>";
            }
        }
    }

    // Читает binary
    void getImage(const std::wstring& sImageId, const std::wstring& sMediaDirectory, NSStringUtils::CStringBuilder& oRels)
    {
        std::wstring sId = L"";
        while(m_oLightReader.MoveToNextAttribute())
        {
            // Читает id
            if(m_oLightReader.GetName() == L"id")
                sId = m_oLightReader.GetText();
        }
        m_oLightReader.MoveToElement();
        if(sId == L"")
            return;

        // Пишет картинку в файл
        NSFile::CFileBinary oImageWriter;
        if(oImageWriter.CreateFileW(sMediaDirectory + L"/" + sId))
        {
            std::string sBase64 = contentA();
            int nSrcLen = (int)sBase64.length();
            int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
            BYTE* pImageData = new BYTE[nDecodeLen];
            if (TRUE == NSBase64::Base64Decode(sBase64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
            RELEASEARRAYOBJECTS(pImageData);
            oImageWriter.CloseFile();

            // Получаем размеры картинки
            CBgraFrame oBgraFrame;
            oBgraFrame.OpenFile(sMediaDirectory + L"/" + sId);
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
                    nWx = nW;
                }
                else
                {
                    nWx = nW;
                    nHy = nH;
                }
            }
            std::vector<std::wstring> vImage;
            vImage.push_back(sImageId);
            vImage.push_back(std::to_wstring(nWx));
            vImage.push_back(std::to_wstring(nHy));

            m_mImages.insert(std::make_pair(sId, vImage));
            // Запись картинок в рельсы
            oRels += L"<Relationship Id=\"rPic";
            oRels += sImageId;
            oRels += L"\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image\" Target=\"media/";
            oRels += sId;
            oRels += L"\"/>";
        }
    }

    // Читает description
    void readDescription(NSStringUtils::CStringBuilder& oBuilder)
    {
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем title-info
            if(sName == L"title-info")
                getTitleInfo(m_oTitleInfo, oBuilder);
            /*
            // Читаем src-title-info (ноль или один)
            else if(sName == L"src-title-info")
            {
                m_pSrcTitleInfo = new STitleInfo();
                getTitleInfo(*m_pSrcTitleInfo);
            }
            // Читаем document-info
            else if(sName == L"document-info")
                getDocumentInfo();
            // Читаем publish-info (ноль или один)
            else if(sName == L"publish-info")
            {
                m_pPublishInfo = new SPublishInfo();
                getPublishInfo();
            }
            // Читаем custom-info (любое)
            else if(sName == L"custom-info")
                getCustomInfo();
            */
        }
    }

    // Читает custom-info
    void getCustomInfo()
    {
        std::wstring sIntoType = L"";
        if(m_oLightReader.MoveToNextAttribute())
            sIntoType = m_oLightReader.GetText();
        m_oLightReader.MoveToElement();
        std::wstring sCustomInfo = content();
        m_mCustomInfo.insert(std::make_pair(sIntoType, sCustomInfo));
    }

    // Читает publish-info
    void getPublishInfo()
    {
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем book-name (ноль или один)
            if(sName == L"book-name")
            {
                if(m_pPublishInfo->m_pBookName)
                    delete[] m_pPublishInfo->m_pBookName;
                m_pPublishInfo->m_pBookName = new std::wstring[1];
                *m_pPublishInfo->m_pBookName = content();
            }
            // Читаем publisher (ноль или один)
            else if(sName == L"publisher")
            {
                if(m_pPublishInfo->m_pPublisher)
                    delete[] m_pPublishInfo->m_pPublisher;
                m_pPublishInfo->m_pPublisher = new std::wstring[1];
                *m_pPublishInfo->m_pPublisher = content();
            }
            // Читаем city (ноль или один)
            else if(sName == L"city")
            {
                if(m_pPublishInfo->m_pCity)
                    delete[] m_pPublishInfo->m_pCity;
                m_pPublishInfo->m_pCity = new std::wstring[1];
                *m_pPublishInfo->m_pCity = content();
            }
            // Читаем year (ноль или один)
            else if(sName == L"year")
            {
                if(m_pPublishInfo->m_pYear)
                    delete[] m_pPublishInfo->m_pYear;
                m_pPublishInfo->m_pYear = new std::wstring[1];
                *m_pPublishInfo->m_pYear = content();
            }
            // Читаем isbn (ноль или один)
            else if(sName == L"isbn")
            {
                if(m_pPublishInfo->m_pIsbn)
                    delete[] m_pPublishInfo->m_pIsbn;
                m_pPublishInfo->m_pIsbn = new std::wstring[1];
                *m_pPublishInfo->m_pIsbn = content();
            }
            // Читаем sequence (любое)
            else if(sName == L"sequence")
            {
                std::wstring sSName = L"";
                std::wstring sSNumber = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"name")
                        sSName = m_oLightReader.GetText();
                    else if(m_oLightReader.GetName() == L"number")
                        sSNumber = m_oLightReader.GetText();
                }
                m_oLightReader.MoveToElement();
                m_pPublishInfo->m_mSequence.insert(std::make_pair(sSName, sSNumber));
            }
        }
    }

    // Читает document-info
    void getDocumentInfo()
    {
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем author (один или более)
            if(sName == L"author")
                getAuthor(m_oDocumentInfo.m_arAuthors);
            // Читаем program-used (ноль или один)
            else if(sName == L"program-used")
            {
                if(m_oDocumentInfo.m_pProgramUsed)
                    delete[] m_oDocumentInfo.m_pProgramUsed;
                m_oDocumentInfo.m_pProgramUsed = new std::wstring[1];
                *m_oDocumentInfo.m_pProgramUsed = content();
            }
            // Читаем date
            else if(sName == L"date")
                m_oDocumentInfo.m_pDate = content();
            // Читаем src-url (любое)
            else if(sName == L"src-url")
                m_oDocumentInfo.m_arSrcUrl.push_back(content());
            // Читаем src-ocr (ноль или один)
            else if(sName == L"src-ocr")
            {
                if(m_oDocumentInfo.m_pSrcOcr)
                    delete[] m_oDocumentInfo.m_pSrcOcr;
                m_oDocumentInfo.m_pSrcOcr = new std::wstring[1];
                *m_oDocumentInfo.m_pSrcOcr = content();
            }
            // Читаем id
            else if(sName == L"id")
                m_oDocumentInfo.m_sId = content();
            // Читаем version
            else if(sName == L"version")
                m_oDocumentInfo.m_sVersion = content();
            // Читаем history (ноль или один)
            else if(sName == L"history")
            {
                if(m_oDocumentInfo.m_pHistory)
                    delete[] m_oDocumentInfo.m_pHistory;
                m_oDocumentInfo.m_pHistory = new std::wstring[1];
                *m_oDocumentInfo.m_pHistory = L"";
            }
        }
    }

    // Читает title-info и src-title-info
    void getTitleInfo(STitleInfo& oTitleInfo, NSStringUtils::CStringBuilder& oBuilder)
    {
        int nDepth = m_oLightReader.GetDepth();
        while(m_oLightReader.ReadNextSiblingNode(nDepth))
        {
            std::wstring sName = m_oLightReader.GetName();
            // Читаем annotation (ноль или один)
            if(sName == L"annotation")
                readAnnotation(oBuilder);
            // Читаем coverpage (ноль или один)
            else if(sName == L"coverpage")
            {
                // Читаем image (один или более)
                int nCDepth = m_oLightReader.GetDepth();
                while(m_oLightReader.ReadNextSiblingNode(nCDepth))
                {
                    if(m_oLightReader.GetName() == L"image")
                    {
                        oBuilder += L"<w:p><w:pPr><w:pStyle w:val=\"image\"/></w:pPr>";
                        readImage(oBuilder);
                        oBuilder += L"</w:p>";
                    }
                }
            }
            // Читаем genre (один или более)
            else if(sName == L"genre")
                oTitleInfo.m_arGenres.push_back(content());
            // Читаем author (один или более)
            else if(sName == L"author")
               getAuthor(oTitleInfo.m_arAuthors);
            // Читаем book-title
            else if(sName == L"book-title")
                oTitleInfo.m_sBookTitle = content();
            /*
            // Читаем keywords (ноль или один)
            else if(sName == L"keywords")
            {
                if(oTitleInfo.m_pKeywords)
                    delete[] oTitleInfo.m_pKeywords;
                oTitleInfo.m_pKeywords = new std::wstring[1];
                *oTitleInfo.m_pKeywords = content();
            }
            // Читаем date (ноль или один)
            else if(sName == L"date")
            {
                if(oTitleInfo.m_pDate)
                    delete[] oTitleInfo.m_pDate;
                oTitleInfo.m_pDate = new std::pair<std::wstring, std::wstring>[1];
                std::wstring sDate = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"name")
                        sDate = m_oLightReader.GetText();
                }
                m_oLightReader.MoveToElement();
                *oTitleInfo.m_pDate = make_pair(sDate, content());
            }
            // Читаем lang
            else if(sName == L"lang")
                oTitleInfo.m_sLang = content();
            // Читаем src-lang (ноль или один)
            else if(sName == L"src-lang")
            {
                if(oTitleInfo.m_pSrcLang)
                    delete[] oTitleInfo.m_pSrcLang;
                oTitleInfo.m_pSrcLang = new std::wstring[1];
                *oTitleInfo.m_pSrcLang = content();
            }
            // Читаем translator (любое)
            else if(sName == L"translator")
                getAuthor(oTitleInfo.m_arTranslator);
            // Читаем sequence (любое)
            else if(sName == L"sequence")
            {
                std::wstring sSName = L"";
                std::wstring sSNumber = L"";
                while(m_oLightReader.MoveToNextAttribute())
                {
                    if(m_oLightReader.GetName() == L"name")
                        sSName = m_oLightReader.GetText();
                    else if(m_oLightReader.GetName() == L"number")
                        sSNumber = m_oLightReader.GetText();
                }
                m_oLightReader.MoveToElement();
                oTitleInfo.m_mSequence.insert(std::make_pair(sSName, sSNumber));
            }
            */
        }
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
    if (!m_internal->OpenFromFile(sFile))
        return false;
    // Читаем FictionBook
    if(!m_internal->isSection(L"FictionBook"))
        return false;
    return true;
}

// Выставление рабочей папки
void CFb2File::SetTmpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sTmpFolder = sFolder;
}

// Проверяет, соответствует ли fb2 файл формату
// sPath - путь к файлу fb2, sDirectory - директория, где формируется и создается docx
HRESULT CFb2File::Open(const std::wstring& sPath, const std::wstring& sDirectory, CFb2Params* oParams)
{
    // Копирование шаблона
    if(!ExtractTemplate(sDirectory))
        return S_FALSE;

    // Начало файла
    NSStringUtils::CStringBuilder oDocument;
    oDocument += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
    oDocument += L"<w:document xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" mc:Ignorable=\"w14 w15 wp14\"><w:body>";

    // Создаем сноски
    NSStringUtils::CStringBuilder oFootnotes;
    oFootnotes += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
    oFootnotes += L"<w:footnotes xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" xmlns:o=\"urn:schemas-microsoft-com:office:office\" xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" xmlns:v=\"urn:schemas-microsoft-com:vml\" xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" xmlns:w10=\"urn:schemas-microsoft-com:office:word\" xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" mc:Ignorable=\"w14 w15 wp14\">";
    oFootnotes += L"<w:footnote w:type=\"separator\" w:id=\"-1\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:separator/></w:r></w:p></w:footnote>";
    oFootnotes += L"<w:footnote w:type=\"continuationSeparator\" w:id=\"0\"><w:p><w:pPr><w:spacing w:lineRule=\"auto\" w:line=\"240\" w:after=\"0\"/></w:pPr><w:r><w:continuationSeparator/></w:r></w:p></w:footnote>";

    // Создаем рельсы
    NSStringUtils::CStringBuilder oRels;
    oRels += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?><Relationships xmlns=\"http://schemas.openxmlformats.org/package/2006/relationships\">";
    oRels += L"<Relationship Id=\"rId1\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles\" Target=\"styles.xml\"/>";
    oRels += L"<Relationship Id=\"rId2\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/settings\" Target=\"settings.xml\"/>";
    oRels += L"<Relationship Id=\"rId3\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/webSettings\" Target=\"webSettings.xml\"/>";
    oRels += L"<Relationship Id=\"rId4\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/fontTable\" Target=\"fontTable.xml\"/>";
    oRels += L"<Relationship Id=\"rId5\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/theme\" Target=\"theme/theme1.xml\"/>";
    oRels += L"<Relationship Id=\"rId6\" Type=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships/footnotes\" Target=\"footnotes.xml\"/>";

    // Директория картинок
    std::wstring sMediaDirectory = sDirectory + L"/word/media";
    NSDirectory::CreateDirectory(sMediaDirectory);

    NSStringUtils::CStringBuilder oBuilder;
    NSStringUtils::CStringBuilder oDescription;

    // Читаем содержание, картинки, сноски
    bool bNeedContents = false;
    if(oParams)
        bNeedContents = oParams->bNeedContents;
    NSStringUtils::CStringBuilder oContents;
    if(!m_internal->readText(sPath, sMediaDirectory, oContents, oRels, oFootnotes))
        return S_FALSE;

    if(!m_internal->m_oLightReader.MoveToStart())
        return S_FALSE;

    int nDeath = m_internal->m_oLightReader.GetDepth();
    while(m_internal->m_oLightReader.ReadNextSiblingNode(nDeath))
    {
        std::wstring sName = m_internal->m_oLightReader.GetName();
        // Читаем description
        if(sName == L"description")
            m_internal->readDescription(oDescription);
        // Читаем body
        else if(sName == L"body")
        {
            bool bNotes = false;
            while(m_internal->m_oLightReader.MoveToNextAttribute())
            {
                if(m_internal->m_oLightReader.GetName() == L"name" &&
                   m_internal->m_oLightReader.GetText() == L"notes")
                    bNotes = true;
            }
            m_internal->m_oLightReader.MoveToElement();
            if(bNotes)
                continue;
            m_internal->readBody(oBuilder);
        }
    }

    // Конец сносок
    oFootnotes += L"</w:footnotes>";
    // Пишем сноски в файл
    std::wstring sFootnotesDirectory = sDirectory + L"/word";
    NSFile::CFileBinary oFootnotesWriter;
    if (oFootnotesWriter.CreateFileW(sFootnotesDirectory + L"/footnotes.xml"))
    {
        oFootnotesWriter.WriteStringUTF8(oFootnotes.GetData());
        oFootnotesWriter.CloseFile();
    }

    // Вставляем description
    oDocument += oDescription.GetData();
    // Вставляем содержание
    if(bNeedContents)
        oDocument += oContents.GetData();
    // Вставляем основное тело
    oDocument += oBuilder.GetData();
    // Конец документа
    oDocument += L"<w:sectPr><w:footnotePr/><w:type w:val=\"nextPage\"/><w:pgSz w:w=\"11906\" w:h=\"16838\" w:orient=\"portrait\"/><w:pgMar w:top=\"1134\" w:right=\"850\" w:bottom=\"1134\" w:left=\"1701\" w:header=\"709\" w:footer=\"709\" w:gutter=\"0\"/><w:cols w:num=\"1\" w:sep=\"0\" w:space=\"708\" w:equalWidth=\"1\"/><w:docGrid w:linePitch=\"360\"/></w:sectPr></w:body>";
    oDocument += L"</w:document>";
    // Пишем документ в файл
    NSFile::CFileBinary oDocumentXmlWriter;
    if (oDocumentXmlWriter.CreateFileW(sDirectory + L"/word/document.xml"))
    {
        oDocumentXmlWriter.WriteStringUTF8(oDocument.GetData());
        oDocumentXmlWriter.CloseFile();
    }

    // Конец рельсов
    oRels += L"</Relationships>";
    // Пишем рельсы в файл
    std::wstring sRelsDirectory = sDirectory + L"/word/_rels";
    NSFile::CFileBinary oRelsWriter;
    if (oRelsWriter.CreateFileW(sRelsDirectory + L"/document.xml.rels"))
    {
        oRelsWriter.WriteStringUTF8(oRels.GetData());
        oRelsWriter.CloseFile();
    }

    // Директория app и core
    std::wstring sDocPropsDirectory = sDirectory + L"/docProps";
    NSDirectory::CreateDirectory(sDocPropsDirectory);

    // Создаем core.xml
    NSStringUtils::CStringBuilder oCore;
    oCore += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
    oCore += L"<cp:coreProperties xmlns:cp=\"http://schemas.openxmlformats.org/package/2006/metadata/core-properties\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:dcterms=\"http://purl.org/dc/terms/\" xmlns:dcmitype=\"http://purl.org/dc/dcmitype/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">";
    // Заголовок
    oCore += L"<dc:title>";
    oCore.WriteEncodeXmlString(m_internal->m_oTitleInfo.m_sBookTitle);
    oCore += L"</dc:title>";
    // Жанры
    oCore += L"<dc:subject>";
    oCore.WriteEncodeXmlString(m_internal->m_oTitleInfo.getGenres());
    oCore += L"</dc:subject>";
    // Авторы
    oCore += L"<dc:creator>";
    oCore.WriteEncodeXmlString(m_internal->m_oTitleInfo.getAuthors());
    oCore += L"</dc:creator>";
    // Конец core
    oCore += L"<cp:revision>1</cp:revision></cp:coreProperties>";
    // Пишем core в файл
    NSFile::CFileBinary oCoreWriter;
    if (oCoreWriter.CreateFileW(sDocPropsDirectory + L"/core.xml"))
    {
        oCoreWriter.WriteStringUTF8(oCore.GetData());
        oCoreWriter.CloseFile();
    }

    // Получаем версию ONLYOFFICE
    std::wstring sApplication = NSSystemUtils::GetEnvVariable(NSSystemUtils::gc_EnvApplicationName);
    if (sApplication.empty())
        sApplication = NSSystemUtils::gc_EnvApplicationNameDefault;
#if defined(INTVER)
    std::string sVersion = VALUE2STR(INTVER);
#endif
    sApplication += L"/";
    sApplication += UTF8_TO_U(sVersion);
    // Создаем app.xml
    NSStringUtils::CStringBuilder oApp;
    oApp += L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
    oApp += L"<Properties xmlns=\"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties\" xmlns:vt=\"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes\">";
    oApp += L"<Application>";
    oApp += sApplication;
    oApp += L"</Application>";
    oApp += L"<Characters>0</Characters><CharactersWithSpaces>0</CharactersWithSpaces><DocSecurity>0</DocSecurity><HyperlinksChanged>false</HyperlinksChanged><Lines>1</Lines><LinksUpToDate>false</LinksUpToDate><Pages>1</Pages><Paragraphs>1</Paragraphs><ScaleCrop>false</ScaleCrop><SharedDoc>false</SharedDoc><TotalTime>1</TotalTime><Words>0</Words>";
    oApp += L"</Properties>";
    // Пишем app в файл
    NSFile::CFileBinary oAppWriter;
    if (oAppWriter.CreateFileW(sDocPropsDirectory + L"/app.xml"))
    {
        oAppWriter.WriteStringUTF8(oApp.GetData());
        oAppWriter.CloseFile();
    }

    // Архивим в docx
    bool bNeedDocx = false;
    if(oParams)
        bNeedDocx = oParams->bNeedDocx;
    if(bNeedDocx)
    {
        COfficeUtils oZip;
        HRESULT oRes = oZip.CompressFileOrDirectory(sDirectory, sDirectory + L"/" + NSFile::GetFileName(sPath) + L".docx");
        return oRes;
    }

    return S_OK;
}
