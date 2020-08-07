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
    std::wstring m_sGenres;      // Жанры
    std::wstring m_sAuthors;     // Авторы
    std::wstring m_sBookTitle;   // Название
    std::wstring m_sDate;        // Дата
    std::wstring m_sDescription; // описание

    void SetDate(std::wstring sDate)
    {
        m_sDate = sDate;
    }

    void SetDescription(std::wstring sDescription)
    {
        m_sDescription = sDescription;
    }

    void SetGenres(std::wstring sGenres)
    {
        m_sGenres = sGenres;
    }

    void SetAuthors(std::wstring sAuthors)
    {
        m_sAuthors = sAuthors;
    }

    void SetTitle(std::wstring sTitle)
    {
        m_sBookTitle = sTitle;
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
