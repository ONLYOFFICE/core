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
#ifndef _XPS_XPSLIB_DOCUMENT_H
#define _XPS_XPSLIB_DOCUMENT_H

#include "FontList.h"
#include "XpsPage.h"
#include <map>
#include <vector>

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "../../OfficeUtils/src/ZipFolder.h"

namespace XPS
{
	class CPath;
	class CStaticResource;
	class CDocument
	{
	public:
        CDocument(NSFonts::IFontManager* pFontManager);
		~CDocument();

		bool Read(IFolder* pFolder);
		int  GetPageCount() const;
		void GetPageSize(int nPageIndex, int& nW, int& nH);
		void DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak);
		void Close();
		CStaticResource* GetStaticResource(const wchar_t* wsPath);
		std::wstring GetInfo();

		struct CDocumentStructure
		{
			int nLevel;
			int nPage;
			double dY;
			std::string sDescription;
			std::wstring wsTarget;
		};
		BYTE* GetStructure();
		BYTE* GetPageLinks (int nPageIndex);
		std::vector<CDocumentStructure>          m_vStructure;
		std::map<std::wstring, int>              m_mInternalLinks;
	private:
									        
		IFolder*                                 m_wsPath;
		std::map<int, XPS::Page*>                m_mPages;
		CFontList                                m_oFontList;
        NSFonts::IFontManager*                   m_pFontManager;
		std::map<std::wstring, CStaticResource*> m_mStaticResources;
	};	
}

#endif //_XPS_XPSLIB_DOCUMENT_H
