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

#include "../../../../DesktopEditor/common/Path.h"
#include "../../../DocxFormat/Media/VbaProject.h"

#include "NumberingWriter.h"
#include "fontTableWriter.h"
#include "HeaderFooterWriter.h"
#include "DocumentWriter.h"
#include "MediaWriter.h"
#include "StylesWriter.h"
#include "SettingWriter.h"
#include "CommentsWriter.h"
#include "ChartWriter.h"
#include "DocumentRelsWriter.h"
#include "webSettingsWriter.h"
#include "DefaultThemeWriter.h"
#include "CustomXmlWriter.h"

namespace BinDocxRW 
{
	class CComments;
}

namespace NSBinPptxRW
{
	class CDrawingConverter;
}
namespace OOX
{
	class CApp;
	class CCore;
	class CSettings;
}
namespace PPTX
{
	class CustomProperties;
}

namespace Writers
{
	struct _part_summary_writers
	{
		_part_summary_writers(std::wstring sDirOutput, std::wstring sFontDir, bool bNoFontDir, int nVersion)
			:
			font_table(sDirOutput, sFontDir, bNoFontDir),
			headers_footers(sDirOutput),
			document(sDirOutput, headers_footers),
			styles(sDirOutput, nVersion),
			numbering(sDirOutput),
			footnotes(sDirOutput),
			endnotes(sDirOutput),
			settings(sDirOutput),
			comments(sDirOutput),
			web_settings(sDirOutput)
		{}

		void Write(bool bGlossary = false)
		{
			comments.Write(bGlossary);
			styles.Write(bGlossary);
			numbering.Write(bGlossary);
			font_table.Write(bGlossary);
			headers_footers.Write(bGlossary);
			footnotes.Write(bGlossary);
			endnotes.Write(bGlossary);
			//Write settings after HeaderFooter to fill evenAndOddHeaders
			settings.Write(bGlossary);
			web_settings.Write(bGlossary);
			//Write document after HeaderFooter to fill sectPr
			document.Write(bGlossary);

		}

		DocumentWriter			document;
		NumberingWriter			numbering;
		StylesWriter			styles;
		FootnotesWriter			footnotes;
		EndnotesWriter			endnotes;
		HeaderFooterWriter		headers_footers;
		SettingWriter			settings;
		WebSettingsWriter		web_settings;
		FontTableWriter			font_table;
		CommentsWriter			comments;
	};

	class FileWriter
	{
	private:
		_part_summary_writers	m_oMain;
		_part_summary_writers	m_oGlossary;
	public:

		FileWriter(std::wstring sDirOutput, std::wstring sFontDir, bool bNoFontDir, int nVersion, NSBinPptxRW::CDrawingConverter* pDrawingConverter, std::wstring sThemePath);
		~FileWriter();

		FontTableWriter&		get_font_table_writer()		{ return m_bGlossaryMode ? m_oGlossary.font_table : m_oMain.font_table; }
		DocumentWriter&			get_document_writer()		{ return m_bGlossaryMode ? m_oGlossary.document : m_oMain.document; }
		FootnotesWriter&		get_footnotes_writer()		{ return m_bGlossaryMode ? m_oGlossary.footnotes : m_oMain.footnotes; }
		EndnotesWriter&			get_endnotes_writer()		{ return m_bGlossaryMode ? m_oGlossary.endnotes : m_oMain.endnotes; }
		HeaderFooterWriter&		get_headers_footers_writer(){ return m_bGlossaryMode ? m_oGlossary.headers_footers : m_oMain.headers_footers; }
		SettingWriter&			get_settings_writer()		{ return m_bGlossaryMode ? m_oGlossary.settings : m_oMain.settings; }
		CommentsWriter&			get_comments_writer()		{ return m_bGlossaryMode ? m_oGlossary.comments : m_oMain.comments; }
		NumberingWriter&		get_numbering_writer()		{ return m_bGlossaryMode ? m_oGlossary.numbering : m_oMain.numbering; }
		StylesWriter&			get_style_writers()			{ return m_bGlossaryMode ? m_oGlossary.styles : m_oMain.styles; }
		WebSettingsWriter&		get_web_settings_writer()	{ return m_bGlossaryMode ? m_oGlossary.web_settings : m_oMain.web_settings; }

		int getNextDocPr();

		void AddSetting(std::wstring sSetting);

		void Write();
		void WriteGlossary();

		bool IsEmptyGlossary();

		MediaWriter				m_oMediaWriter;
		ChartWriter				m_oChartWriter;
		DefaultThemeWriter		m_oTheme;
		CustomXmlWriter			m_oCustomXmlWriter;		
		DocumentRelsWriter		m_oDocumentRelsWriter;

		smart_ptr<OOX::VbaProject>		m_pVbaProject;
	
 		NSBinPptxRW::CDrawingConverter* m_pDrawingConverter;

		std::wstring					m_sThemePath;
		int								m_nDocPrIndex;
		BinDocxRW::CComments*			m_pComments;
		OOX::CApp*						m_pApp;
		OOX::CCore*						m_pCore;
		OOX::CSettings*					m_pCurrentSettings;
		PPTX::CustomProperties*			m_pCustomProperties;

		bool m_bGlossaryMode = false;
	};
}
