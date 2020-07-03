#include "Fb2File.h"
#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/Base64.h"
#include "../../../DesktopEditor/common/File.h"

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
    XmlUtils::CXmlLiteReader oLightReader; // SAX Reader
    std::wstring m_sFile;                  // Имя файла
    std::wstring m_sDstFolder;             // Путь к результату
    // Контент файла
    std::vector<std::wstring> m_arGenres; // Жанры
    std::vector<author> m_arAuthors;      // Авторы
    std::wstring m_sBookTitle;          // Название книги

    std::map<std::wstring, std::wstring> m_mXmlns;  // Ссылки
    std::map<std::wstring, std::wstring> m_mImage; // Картинки

public:
    CFb2File_Private()
    {
        oLightReader = XmlUtils::CXmlLiteReader();
        m_sFile = L"";
        m_sDstFolder = L"";
        m_arGenres = std::vector<std::wstring>();
        m_arAuthors = std::vector<author>();
        m_sBookTitle = L"";
        m_mXmlns = std::map<std::wstring, std::wstring>();
        m_mImage = std::map<std::wstring, std::wstring>();
    }

    ~CFb2File_Private()
    {
        oLightReader.Clear();
        m_arGenres.clear();
        m_arAuthors.clear();
        m_mXmlns.clear();
        m_mImage.clear();
    }

    // Читает и проверят соответствует ли текущий раздел ожиданиям
    bool isSection(std::wstring name)
    {
        if(oLightReader.ReadNextNode())
        {
            if(oLightReader.GetName() != name)
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    // Возвращает содержание узла
    std::wstring content()
    {
        std::wstring res;
        if(oLightReader.ReadNextSiblingNode2(oLightReader.GetDepth()))
        {
            res = oLightReader.GetText();
        }
        else
        {
            res = L"";
        }
        return res;
    }

    std::string contentA(std::wstring is)
    {
        std::string res;
        if(oLightReader.ReadNextSiblingNode2(oLightReader.GetDepth()))
        {
            res = oLightReader.GetTextA();
        }
        else
        {
            res = "";
        }
        return res;
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
    if (!m_internal->oLightReader.FromFile(sFile))
    {
        return false;
    }

    // Читаем FictionBook
    if(!m_internal->isSection(L"FictionBook"))
        return false;
    while(m_internal->oLightReader.MoveToNextAttribute())
    {
        m_internal->m_mXmlns.insert(std::make_pair(m_internal->oLightReader.GetName(), m_internal->oLightReader.GetText()));
    }
    m_internal->m_sFile = sFile;
    return true;
}

// Выставление временной (рабочей) папки
void CFb2File::SetImpDirectory(const std::wstring& sFolder)
{
    m_internal->m_sDstFolder = sFolder;
}

// Проверяет, соответствует ли fb2 файл формату
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
        std::wstring genre;
        genre = m_internal->content();
        if(genre == L"")
            return false;
        m_internal->m_arGenres.push_back(genre);
    } while(m_internal->isSection(L"genre"));

    // Читаем author (один или более)
    do
    {
        author _author;
        int depth = m_internal->oLightReader.GetDepth();
        while(m_internal->oLightReader.ReadNextSiblingNode(depth))
        {
            std::wstring name = m_internal->oLightReader.GetName();
            std::wstring res;
            if(name == L"first-name")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.first_name = res;
            }
            else if(name == L"middle-name")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.middle_name = res;
            }
            else if(name == L"last-name")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.last_name = res;
            }
            else if(name == L"nickname")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.nickname = res;
            }
            else if(name == L"home-page")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.home_page.push_back(res);
            }
            else if(name == L"email")
            {
                res = m_internal->content();
                if(res == L"")
                    return false;
                _author.email.push_back(res);
            }
            else
            {
                return false;
            }
        }
        m_internal->m_arAuthors.push_back(_author);
    } while(m_internal->isSection(L"author"));

    // Читаем book-title
    if(m_internal->oLightReader.GetName() != L"book-title")
    {
        return false;
    }
    m_internal->m_sBookTitle = m_internal->content();
    if(m_internal->m_sBookTitle == L"")
        return false;

    // ПРОПУСКАЕМ body

    while(m_internal->oLightReader.ReadNextNode())
    {
        // Читает картинки
        if(m_internal->oLightReader.GetName() == L"binary")
        {
            std::wstring id;
            std::wstring content_type;
            // Читает id
            if(m_internal->oLightReader.MoveToNextAttribute())
            {
                id = m_internal->oLightReader.GetText();
            }
            else
            {
                return false;
            }

            // Читает content-type
            if(m_internal->oLightReader.MoveToNextAttribute())
            {
                content_type = m_internal->oLightReader.GetText();
            }
            else
            {
                return false;
            }
            m_internal->m_mImage.insert(std::make_pair(id, content_type));
            m_internal->oLightReader.MoveToElement();

            NSFile::CFileBinary oImageWriter;
            if (oImageWriter.CreateFileW(sDirectory + id))
            {
                std::string base64 = m_internal->contentA(id);
                int nSrcLen = (int)base64.length();
                int nDecodeLen = NSBase64::Base64DecodeGetRequiredLength(nSrcLen);
                BYTE* pImageData = new BYTE[nDecodeLen];
                if (TRUE == NSBase64::Base64Decode(base64.c_str(), nSrcLen, pImageData, &nDecodeLen))
                {
                    oImageWriter.WriteFile(pImageData, (DWORD)nDecodeLen);
                }
                RELEASEARRAYOBJECTS(pImageData);
                oImageWriter.CloseFile();
            }
        }
    }
    return true;
}


