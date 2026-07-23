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
#ifndef _BUILD_APPLICATIONFONTSWORKER_H_
#define _BUILD_APPLICATIONFONTSWORKER_H_

#include <string>
#include <vector>
#include "../graphics/pro/Fonts.h"

#define ONLYOFFICE_FONTS_VERSION     15
#define ONLYOFFICE_ALL_FONTS_VERSION 2

class CApplicationFontsWorkerBreaker
{
public:
	virtual bool IsFontsWorkerRunned() { return true; }
};

class CApplicationFontsWorker_private;
class GRAPHICS_DECL CApplicationFontsWorker
{
public:
	// whether to use system fonts
	bool                        m_bIsUseSystemFonts;
	bool                        m_bIsUseSystemUserFonts;

	// additional font folders
	std::vector<std::wstring>   m_arAdditionalFolders;

	// working directory (all artifacts are stored here)
	std::wstring                m_sDirectory;

	// whether to support OpenType fonts
	bool                        m_bIsUseOpenType;

	// whether to support all AllFonts.js versions
	bool                        m_bIsUseAllVersions;

	// whether thumbnails are needed
	bool                        m_bIsNeedThumbnails;
	// if a thumbnail already exists, should it be deleted and regenerated anyway?
	bool                        m_bIsRemoveOldThumbnails;
	// generate thumbnails separately
	bool                        m_bSeparateThumbnails;
	// which scales are needed for thumbnails
	std::vector<double>         m_arThumbnailsScales;
	bool                        m_bIsGenerateThumbnailsEA;

	// WEB version ----
	std::wstring                m_sThumbnailsDirectory;

	std::wstring                m_sAllFontsJSPath;
	std::wstring                m_sWebAllFontsJSPath;

	std::wstring                m_sWebFontsDirectory;
	// ---------------

	bool                        m_bIsCleanDirectory;

private:
	CApplicationFontsWorker_private* m_pInternal;

public:
	CApplicationFontsWorker();
	~CApplicationFontsWorker();

	NSFonts::IApplicationFonts* Check();
	void CheckThumbnails();

	void SetBreaker(CApplicationFontsWorkerBreaker* pChecker);

	std::string GetAllFonts();

	static std::vector<std::wstring> GetFontNames(NSFonts::IApplicationFonts* pFonts);
	static std::vector<std::wstring> GetFontNamesWithExcludes(NSFonts::IApplicationFonts* pFonts, std::vector<std::wstring> arExcludes);
};

#endif // _BUILD_APPLICATIONFONTSWORKER_H_
