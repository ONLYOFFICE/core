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

#ifndef FONT_H
#define FONT_H

#include "../IOFDElement.h"

class IFolder;
class IRenderer;

#ifdef BUILDING_WASM_MODULE
#define FONTS_USE_ONLY_MEMORY_STREAMS
namespace NSFonts { class IFontManager; }
#endif

namespace OFD
{
class CFont : public IOFDElement
{
public:
	CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder *pFolder);

	void Apply(IRenderer* pRenderer) const;

	#ifdef BUILDING_WASM_MODULE
	NSFonts::IFontManager* GetFontManager() const;
	#endif
private:
	std::wstring m_wsFontName;
	std::wstring m_wsFamilyName;
	std::wstring m_wsCharset;
	bool         m_bItalic;
	bool         m_bBold;
	bool         m_bSerif;
	bool         m_bFixedWidth;

	std::wstring m_wsFilePath;

	#ifdef BUILDING_WASM_MODULE
	std::wstring           m_wsSelectedFont;
	NSFonts::IFontManager* m_pFontManager;
	#else
	bool         m_bSupportExternalFont;
	#endif

	friend class CFontChecker;
};
}

#endif // FONT_H
