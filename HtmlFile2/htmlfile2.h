#ifndef _HTMLFILE2_HTMLFILE2_H
#define _HTMLFILE2_HTMLFILE2_H

#include <string>
#include <vector>
#include "../DesktopEditor/common/Types.h"

#ifndef HTMLFILE2_USE_DYNAMIC_LIBRARY
#define HTMLFILE2_DECL_EXPORT
#else
#include "../DesktopEditor/common/base_export.h"
#define HTMLFILE2_DECL_EXPORT Q_DECL_EXPORT
#endif

struct CHtmlParams
{
    std::vector<std::wstring> m_arGenres;  // Жанры
    std::vector<std::wstring> m_arAuthors; // Авторы
    std::wstring m_sBookTitle;             // Название
    std::wstring m_sDate;                  // Дата
    std::wstring m_sDescription;           // описание

    void SetDate(std::wstring sDate)
    {
        m_sDate = sDate;
    }

    void SetDescription(std::wstring sDescription)
    {
        m_sDescription = sDescription;
    }

    void AddGenre(std::wstring sGenre)
    {
        m_arGenres.push_back(sGenre);
    }

    void AddAuthor(std::wstring sAuthor)
    {
        m_arAuthors.push_back(sAuthor);
    }

    void SetTitle(std::wstring sTitle)
    {
        m_sBookTitle = sTitle;
    }

    // Разделитель не важен ,
    std::wstring GetGenres()
    {
        std::wstring sRes = L"";
        for(std::wstring& vElem : m_arGenres)
            sRes += vElem + L", ";
        return sRes;
    }

    // Разделитель ;
    std::wstring GetAuthors()
    {
        std::wstring sRes = L"";
        for(std::wstring& vElem : m_arAuthors)
            sRes += vElem + L";";
        return sRes;
    }
};

class CHtmlFile2_Private;
class HTMLFILE2_DECL_EXPORT CHtmlFile2
{
private:
    CHtmlFile2_Private* m_internal;
public:
    CHtmlFile2();
    ~CHtmlFile2();

    bool IsHtmlFile(const std::wstring& sFile);
    void SetTmpDirectory(const std::wstring& sFolder);
    HRESULT Open(const std::wstring& sPath, const std::wstring& sDirectory, CHtmlParams* oParams = NULL);
    HRESULT OpenBatch(const std::vector<std::wstring>& sPath, const std::wstring& sDirectory, CHtmlParams* oParams = NULL);
};

#endif // _HTMLFILE2_HTMLFILE2_H
