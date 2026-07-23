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

#include "Font.h"

#include "../Utils/Utils.h"
#include "../Utils/CFontChecker.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"
#include "../../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
CFont::CFont(CXmlReader& oXmlReader, const std::wstring& wsRootPath, IFolder* pFolder)
	: IOFDElement(oXmlReader),
	  m_wsCharset(L"unicode"), m_bItalic(false), m_bBold(false),
	  m_bSerif(false), m_bFixedWidth(false)
	#ifdef BUILDING_WASM_MODULE
	 , m_pFontManager(nullptr)
	#endif
{
	if (0 != oXmlReader.GetAttributesCount() && oXmlReader.MoveToFirstAttribute())
	{
		std::string sArgumentName;

		do
		{
			sArgumentName = oXmlReader.GetNameA();

			if ("FontName" == sArgumentName)
				m_wsFontName = oXmlReader.GetText();
			else if ("FamilyName" == sArgumentName)
				m_wsFamilyName = oXmlReader.GetText();
			else if ("Charset" == sArgumentName)
				m_wsCharset = oXmlReader.GetText();
			else if ("Italic" == sArgumentName)
				m_bItalic = oXmlReader.GetBoolean(true);
			else if ("Bold" == sArgumentName)
				m_bBold = oXmlReader.GetBoolean(true);
			else if ("Serif" == sArgumentName)
				m_bSerif = oXmlReader.GetBoolean(true);
			else if ("FixedWidth" == sArgumentName)
				m_bFixedWidth = oXmlReader.GetBoolean(true);
		} while (oXmlReader.MoveToNextAttribute());
	}

	oXmlReader.MoveToElement();

	if (!oXmlReader.IsEmptyNode())
	{
		const int nDepth = oXmlReader.GetDepth();

		while (oXmlReader.ReadNextSiblingNode(nDepth))
		{
			if ("ofd:FontFile" == oXmlReader.GetNameA())
			{
				const std::wstring wsPath{oXmlReader.GetText2()};

				if (CanUseThisPath(wsPath, wsRootPath))
					m_wsFilePath = CombinePaths(wsRootPath, wsPath);

				break;
			}
		}

		#ifndef FONTS_USE_ONLY_MEMORY_STREAMS
		if (nullptr != pFolder && IFolder::IFolderType::iftZip == pFolder->getType())
			m_bSupportExternalFont = false;
		#endif
	}
}

void CFont::Apply(IRenderer* pRenderer) const
{
	if (nullptr == pRenderer)
		return;

	int nFontStyle = 0;

	if (m_bBold)
		nFontStyle |= 0x01;
	if (m_bItalic)
		nFontStyle |= 0x02;

	pRenderer->put_FontStyle(nFontStyle);

#ifdef FONTS_USE_ONLY_MEMORY_STREAMS
	if (m_wsSelectedFont.empty())
		return;
	// put_FontName cannot be called, otherwise pRenderer will have uncontrolled font selection
	pRenderer->put_FontPath(m_wsSelectedFont); // The font has been added to the GlobalMemoryStorage fonts, so it can be put
#else
	pRenderer->put_FontName(m_wsFontName);
	if (m_bSupportExternalFont && !m_wsFilePath.empty())
		pRenderer->put_FontPath(m_wsFilePath);
#endif
}

#ifdef BUILDING_WASM_MODULE
NSFonts::IFontManager* CFont::GetFontManager() const
{
	return m_pFontManager;
}
#endif
}
