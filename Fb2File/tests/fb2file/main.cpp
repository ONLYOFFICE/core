#include <iostream>
#include <vector>
#include <map>
#include <fstream>

#include <comdef.h>

#include "../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../DesktopEditor/common/Base64.h"
#include "../../../DesktopEditor/common/File.h"

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

// Чтение и конвертация fb2-файлов
class CFb2File
{
private:
    XmlUtils::CXmlLiteReader oLightReader; // SAX Reader
    std::wstring file;                     // Путь к файлу
    std::wstring save;                     // Путь к сохранению

    // Контент файла
    std::vector<std::wstring> genres; // Жанры
    std::vector<author> authors;      // Авторы
    std::wstring book_title;          // Название книги

    std::map<std::wstring, std::wstring> xmlns;  // Ссылки
    std::map<std::wstring, std::wstring> binary; // Картинки

    // Читает и проверят соответствует ли текущий раздел ожиданиям
    bool isSection(std::wstring name)
    {
        if(oLightReader.ReadNextNode())
        {
            if(oLightReader.GetName() != name)
            {
                std::wcout << L"section isn't " << name << std::endl;
                return false;
            }
        }
        else
        {
            std::wcout << name << L" section missing" << std::endl;
            return false;
        }
        return true;
    }

    // Возвращает содержание узла
    std::wstring content(std::wstring is)
    {
        std::wstring res;
        if(oLightReader.ReadNextSiblingNode2(oLightReader.GetDepth()))
        {
            res = oLightReader.GetText();
        }
        else
        {
            std::wcout << is << L" not specified" << std::endl;
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
            std::wcout << is << L" not specified" << std::endl;
            res = "";
        }
        return res;
    }

public:
    // Проверяет, соответствует ли fb2 файл формату
    bool IsFb2File(std::wstring f)
    {
        file = f;
        if (!oLightReader.FromFile(file))
        {
            std::cout << "File don't open" << std::endl;
            return false;
        }

        // Читаем FictionBook
        if(!isSection(L"FictionBook"))
            return false;
        while(oLightReader.MoveToNextAttribute())
        {
            xmlns.insert(std::make_pair(oLightReader.GetName(), oLightReader.GetText()));
        }

        // Читаем description
        if(!isSection(L"description"))
            return false;

        // Читаем наполнение description
        // Читаем title-info
        if(!isSection(L"title-info"))
            return false;

        // Читаем наполнение title-info
        // Читаем genre (один или более)
        if(!isSection(L"genre"))
            return false;
        do
        {
            std::wstring genre;
            genre = content(L"genre");
            if(genre == L"")
                return false;
            genres.push_back(genre);
        } while(isSection(L"genre"));

        // Читаем author (один или более)
        do
        {
            author _author;
            int depth = oLightReader.GetDepth();
            while(oLightReader.ReadNextSiblingNode(depth))
            {
                std::wstring name = oLightReader.GetName();
                std::wstring res;
                if(name == L"first-name")
                {
                    res = content(L"first-name");
                    if(res == L"")
                        return false;
                    _author.first_name = res;
                }
                else if(name == L"middle-name")
                {
                    res = content(L"middle-name");
                    if(res == L"")
                        return false;
                    _author.middle_name = res;
                }
                else if(name == L"last-name")
                {
                    res = content(L"last-name");
                    if(res == L"")
                        return false;
                    _author.last_name = res;
                }
                else if(name == L"nickname")
                {
                    res = content(L"nickname");
                    if(res == L"")
                        return false;
                    _author.nickname = res;
                }
                else if(name == L"home-page")
                {
                    res = content(L"home-page");
                    if(res == L"")
                        return false;
                    _author.home_page.push_back(res);
                }
                else if(name == L"email")
                {
                    res = content(L"email");
                    if(res == L"")
                        return false;
                    _author.email.push_back(res);
                }
                else
                {
                    std::wcout << "author content not specified" << std::endl;
                    return false;
                }
            }
            authors.push_back(_author);
        } while(isSection(L"author"));

        // Читаем book-title
        if(oLightReader.GetName() != L"book-title")
        {
            std::wcout << L"section isn't book-title" << std::endl;
            return false;
        }
        book_title = content(L"book-title");
        if(book_title == L"")
            return false;

        // ПРОПУСКАЕМ body

        while(oLightReader.ReadNextNode())
        {
            // Читает картинки
            if(oLightReader.GetName() == L"binary")
            {
                std::wstring id;
                std::wstring content_type;
                // Читает id
                if(oLightReader.MoveToNextAttribute())
                {
                    id = oLightReader.GetText();
                }
                else
                {
                    std::wcout << L"missing id" << std::endl;
                    return false;
                }

                // Читает content-type
                if(oLightReader.MoveToNextAttribute())
                {
                    content_type = oLightReader.GetText();
                }
                else
                {
                    std::wcout << L"missing content-type" << std::endl;
                    return false;
                }
                binary.insert(std::make_pair(id, content_type));
                oLightReader.MoveToElement();

                NSFile::CFileBinary oImageWriter;
                if (oImageWriter.CreateFileW(save + id))
                {
                    std::string base64 = contentA(id);
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

    // Выставление временной (рабочей) папки
    void SetImpDirectory(std::wstring s)
    {
        save = s;
    }

    // Конвертация в структуру docx
    int Convert (std::wstring sPath, std::wstring sDirectory)
    {
        return 0;
    }
};

int main()
{
    CFb2File file;
    file.SetImpDirectory(L"../../../media/");
    bool bCheck = file.IsFb2File(L"../../../../test_TheLastWish.fb2");
    if (!bCheck)
    {
        std::cout << "This isn't a fb2 file" << std::endl;
        return 1;
    }

    file.Convert(L"path", L"directory");

    return 0;
}
