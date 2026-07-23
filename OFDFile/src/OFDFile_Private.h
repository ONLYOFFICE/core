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

#ifndef OFDFILE_PRIVATE_H
#define OFDFILE_PRIVATE_H

#include "Base.h"

namespace NSFonts { class IFontManager; }

class COFDFile_Private
{
	IFolder*           m_pFolder;

	OFD::CFontChecker* m_pFontChecker;

	std::wstring       m_wsTempDir;
	bool               m_bIsTempDirOwner;

	OFD::CBase         m_oBase;

	bool Read();
public:
	COFDFile_Private(NSFonts::IApplicationFonts* pFonts);
	~COFDFile_Private();

	void Close();

	void SetTempDir(const std::wstring& wsPath);
	std::wstring GetTempDir() const;

	bool LoadFromFile(const std::wstring& wsFilePath);
	bool LoadFromMemory(BYTE* pData, DWORD ulLength);

	unsigned int GetPageCount() const;
	void GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;

	void DrawPage(IRenderer* pRenderer, int nPageIndex);
	void DrawPage(IRenderer* pRenderer, int nPageIndex, const double& dX, const double& dY, const double& dWidth, const double& dHeight);

	NSFonts::IApplicationFonts* GetFonts() const;

	std::wstring GetInfo() const;
	BYTE* GetStructure() const;
	BYTE* GetLinks(int nPageIndex) const;
};

#endif // OFDFILE_PRIVATE_H
