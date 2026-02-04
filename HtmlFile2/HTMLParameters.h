#ifndef HTMLPARAMETERS_H
#define HTMLPARAMETERS_H

#include <string>

namespace HTML
{
struct THTMLParameters
{
	std::wstring m_sGenres;      // Жанры
	std::wstring m_sAuthors;     // Авторы
	std::wstring m_sBookTitle;   // Название
	std::wstring m_sDate;        // Дата
	std::wstring m_sDescription; // описание
	std::wstring m_sLanguage;    // Язык
	bool m_bNeedPageBreakBefore; // Новый html с новой страницы
	std::wstring m_sdocDefaults; // Стиль docDefaults
	std::wstring m_sNormal;      // Стиль normal

	THTMLParameters() : m_bNeedPageBreakBefore(false) {}

	void SetNormal(const std::wstring& sStyle)
	{
		m_sNormal = sStyle;
	}

	void SetDocDefaults(const std::wstring& sStyle)
	{
		m_sdocDefaults = sStyle;
	}

	void SetPageBreakBefore(bool bNeed)
	{
		m_bNeedPageBreakBefore = bNeed;
	}

	void SetDate(const std::wstring& sDate)
	{
		m_sDate = sDate;
	}

	void SetDescription(const std::wstring& sDescription)
	{
		m_sDescription = sDescription;
	}

	void SetGenres(const std::wstring& sGenres)
	{
		m_sGenres = sGenres;
	}

	void SetAuthors(const std::wstring& sAuthors)
	{
		m_sAuthors = sAuthors;
	}

	void SetTitle(const std::wstring& sTitle)
	{
		m_sBookTitle = sTitle;
	}

	void SetLanguage(const std::wstring& sLanguage)
	{
		m_sLanguage = sLanguage;
	}
};
}

#endif // HTMLPARAMETERS_H
