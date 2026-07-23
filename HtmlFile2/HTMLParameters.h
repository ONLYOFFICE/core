/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#ifndef HTMLPARAMETERS_H
#define HTMLPARAMETERS_H

#include <string>

namespace HTML
{
struct THTMLParameters
{
	std::wstring m_sGenres;      // Genres
	std::wstring m_sAuthors;     // Authors
	std::wstring m_sBookTitle;   // Title
	std::wstring m_sDate;        // Date
	std::wstring m_sDescription; // Description
	std::wstring m_sLanguage;    // Language
	bool m_bNeedPageBreakBefore; // New html from new page
	std::wstring m_sdocDefaults; // docDefaults style
	std::wstring m_sNormal;      // normal style

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
