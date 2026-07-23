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

#ifndef CSVGPARSER_H
#define CSVGPARSER_H

#include "../../../common/Directory.h"
#include "../../../graphics/pro/Fonts.h"

#include "SvgObjects/CContainer.h"
#include "SvgReader.h"

class CSvgFile;

namespace SVG
{
	class CSvgParser
	{
	public:
		CSvgParser(NSFonts::IFontManager* pFontManager = NULL);
		~CSvgParser();

		void SetFontManager(NSFonts::IFontManager* pFontManager);

		bool LoadFromFile(const std::wstring& wsFile, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;
		bool LoadFromString(const std::wstring& wsContent, CGraphicsContainer*& pContainer, CSvgFile* pFile) const;

		template <class ObjectType>
		bool ReadObject(CSvgReader& oReader, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;
	private:
		template <class ObjectType>
		bool ReadChildrens(CSvgReader& oReader, CContainer<ObjectType>* pContainer, CSvgFile* pFile, CRenderedObject* pParent = NULL) const;

		bool ScanStyles(CSvgReader& oReader, CSvgFile* pFile) const;
		void ParseStyles(const std::wstring& wsStyles, CSvgFile *pFile) const;

		template <class ObjectType>
		bool AddObject(ObjectType* pObject, CContainer<ObjectType>* pContainer) const;

		NSFonts::IFontManager  *m_pFontManager;
	};
}

#endif // CSVGPARSER_H
