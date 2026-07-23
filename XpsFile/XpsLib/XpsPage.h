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
#ifndef _XPS_XPSLIB_PAGE_H
#define _XPS_XPSLIB_PAGE_H

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

#include "FontList.h"
#include "Utils.h"
#include "ContextState.h"

#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

namespace XPS
{
	class CDocument;
	class CStaticResource;

	class Page
	{
	public:
        Page(const std::wstring& wsFile, IFolder* Path, CFontList* pFontList, NSFonts::IFontManager* pFontManager, CDocument* pDocument);
		~Page();

		void GetSize(int& nW, int& nH) const;
		void Draw(IRenderer* pRenderer, bool* pbBreak);

        NSWasm::CPageLink m_oLinks;

	private:

		void DrawCanvas      (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState, bool* pbBreak);
		bool ReadResource    (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawGlyph       (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void DrawPath        (XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		bool StrokeToRenderer(XmlUtils::CXmlLiteReader& oReader, IRenderer* pRenderer, CContextState* pState);
		void ReadPathData    (XmlUtils::CXmlLiteReader& oReader, CWString& wsData, CWString& wsTranform);
		
		bool ClipToRenderer     (const wchar_t* wsString, CContextState* pState);
		bool TransformToRenderer(const wchar_t* wsString, CContextState* pState);

	private:

        std::wstring            m_wsPagePath;
        IFolder*                m_wsRootPath;
        CFontList*              m_pFontList;
        NSFonts::IFontManager*  m_pFontManager;
        CDocument*              m_pDocument;
	};
}

#endif // _XPS_XPSLIB_PAGE_H
