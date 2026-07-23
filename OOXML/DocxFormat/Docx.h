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
#pragma once

#include "IFileContainer.h"
#include "../../DesktopEditor/common/Directory.h"

#if !defined(_WIN32) && !defined (_WIN64)
#include <sys/stat.h>
#endif

namespace PPTX
{
	class Theme;
}
namespace OOX
{
	class CApp;
	class CCore;
	class CDocument;
	class CFontTable;
	class CNumbering;
	class CStyles;
	class CFootnotes;
	class CEndnotes;
	class CSettings;
	class CComments;
	class CCommentsExt;
	class CCommentsExtensible;
	class CCommentsUserData;
	class CCommentsIds;
	class CPeople;
	class CDocumentComments;
	class CDocumentCommentsExt;
	class CDocumentCommentsExtensible;
	class CDocumentPeople;
	class CDocumentCommentsIds;
	class VbaProject;
	class JsaProject;
	class CHdrFtr;

	class CDocx : public OOX::Document, public OOX::IFileContainer
	{
	public:
		struct _part_summary
		{
			OOX::CDocument		*document = NULL;					// Main document     /document.xml
			OOX::CFontTable		*fontTable = NULL;					// Font table        /fonttable.xml
			OOX::CNumbering		*numbering = NULL;					// Numbering         /numbering.xml
			OOX::CStyles		*styles = NULL;						// Styles            /styles.xml
			OOX::CFootnotes		*footnotes = NULL;					// Footnotes         /footnotes.xml
			OOX::CEndnotes		*endnotes = NULL;					// Endnotes          /endnotes.xml
			OOX::CSettings		*settings = NULL;					// Settings          /settings.xml
			OOX::CComments		*comments = NULL;					// Comments          /comments.xml

			void init();
		};

		CDocx();
		CDocx(const CPath& oFilePath);
		virtual ~CDocx();

        bool Read(const CPath& oFilePath);
		void FixAfterRead();
		bool Write(const CPath& oFilePath);

		OOX::CHdrFtr *GetHeaderOrFooter(const OOX::RId& rId, bool glossary = false) const;
		const std::wstring GetCustomSettings() const;
		
		OOX::CApp			*m_pApp;
		OOX::CCore			*m_pCore;

		_part_summary		m_oMain;			// Main document word/
		_part_summary		m_oGlossary;		// Glossary Document word/glossary/

		// todooo create structural object - main document and subordinates - as soon as something other than glossary appears
		bool m_bGlossaryRead;

		OOX::CCommentsExt	*m_pCommentsExt;				// word/commentsExtended.xml
		OOX::CCommentsExtensible	*m_pCommentsExtensible;	// word/commentsExtensible.xml
		OOX::CCommentsUserData		*m_pCommentsUserData;	// word/commentsUserData.xml
		OOX::CCommentsIds			*m_pCommentsIds;		// word/commentsIds.xml
		OOX::CPeople				*m_pPeople;				// word/people.xml
		OOX::CDocumentComments		*m_pDocumentComments;   // word/documentComments.xml
		OOX::CDocumentCommentsExt	*m_pDocumentCommentsExt;// word/documentCommentsExtended.xml
		OOX::CDocumentCommentsExtensible	*m_pDocumentCommentsExtensible;//	word/documentCommentsExtensible.xml
		OOX::CDocumentPeople		*m_pDocumentPeople;		// word/documentPeople.xml
		OOX::CDocumentCommentsIds	*m_pDocumentCommentsIds;// word/documentCommentsIds.xml
		
		OOX::VbaProject		*m_pVbaProject;
		OOX::JsaProject		*m_pJsaProject;
		
		PPTX::Theme			*m_pTheme;

private:
		void init();
	};
} // OOX
