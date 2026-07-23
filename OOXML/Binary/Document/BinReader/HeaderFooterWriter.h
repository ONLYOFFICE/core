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

#include "../../Sheets/Common/Common.h"
#include "../../../Common/SimpleTypes_Word.h"

namespace Writers
{

static  std::wstring g_string_xml_start = L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
static  std::wstring g_string_xmlns = L"xmlns:wpc=\"http://schemas.microsoft.com/office/word/2010/wordprocessingCanvas\" \
xmlns:mc=\"http://schemas.openxmlformats.org/markup-compatibility/2006\" \
xmlns:o=\"urn:schemas-microsoft-com:office:office\" \
xmlns:r=\"http://schemas.openxmlformats.org/officeDocument/2006/relationships\" \
xmlns:m=\"http://schemas.openxmlformats.org/officeDocument/2006/math\" \
xmlns:v=\"urn:schemas-microsoft-com:vml\" \
xmlns:wp14=\"http://schemas.microsoft.com/office/word/2010/wordprocessingDrawing\" \
xmlns:wp=\"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing\" \
xmlns:w10=\"urn:schemas-microsoft-com:office:word\" \
xmlns:w=\"http://schemas.openxmlformats.org/wordprocessingml/2006/main\" \
xmlns:w14=\"http://schemas.microsoft.com/office/word/2010/wordml\" \
xmlns:w15=\"http://schemas.microsoft.com/office/word/2012/wordml\" \
xmlns:wpg=\"http://schemas.microsoft.com/office/word/2010/wordprocessingGroup\" \
xmlns:wpi=\"http://schemas.microsoft.com/office/word/2010/wordprocessingInk\" \
xmlns:wne=\"http://schemas.microsoft.com/office/word/2006/wordml\" \
xmlns:a=\"http://schemas.openxmlformats.org/drawingml/2006/main\" \
xmlns:wps=\"http://schemas.microsoft.com/office/word/2010/wordprocessingShape\" \
mc:Ignorable=\"w14 w15 wp14\">";

	static  std::wstring g_string_hdr_Start = g_string_xml_start + L"<w:hdr " + g_string_xmlns;
	static  std::wstring g_string_hdr_End = L"</w:hdr>";

	static  std::wstring g_string_ftr_Start = g_string_xml_start + L"<w:ftr " + g_string_xmlns;
	static  std::wstring g_string_ftr_End = L"</w:ftr>";

	static  std::wstring g_string_footnotes_Start = g_string_xml_start + L"<w:footnotes " + g_string_xmlns;
	static  std::wstring g_string_footnotes_End = L"</w:footnotes>";

	static  std::wstring g_string_endnotes_Start = g_string_xml_start + L"<w:endnotes " + g_string_xmlns;
	static  std::wstring g_string_endnotes_End = L"</w:endnotes>";
	class ContentWriter

	{
	public:		
		NSStringUtils::CStringBuilder	m_oBackground;
		NSStringUtils::CStringBuilder	m_oContent;
		NSStringUtils::CStringBuilder	m_oSecPr;
        NSStringUtils::CStringBuilderA	m_oContentutf8;
	};

	class HdrFtrItem
	{
	public:
		HdrFtrItem(SimpleTypes::EHdrFtr _eType);

		bool IsEmpty();

        std::wstring			m_sFilename;
		ContentWriter			Header;
        std::wstring			rId;
		SimpleTypes::EHdrFtr	eType;
	};

	class HeaderFooterWriter 
	{
         std::wstring	m_sDir;
	public:
		std::vector<HdrFtrItem*> m_aHeaders;
		std::vector<HdrFtrItem*> m_aFooters;

		HeaderFooterWriter( std::wstring sDir);
		~HeaderFooterWriter();

		void Write(bool bGlossary = false);
		void WriteItem( std::wstring sHeader,  std::wstring& sFilename, ContentWriter& m_oWriter, bool bHeader);
	};

	class FootnotesWriter
	{
         std::wstring m_sDir;

	public:
		ContentWriter	m_oNotesWriter;

		FootnotesWriter( std::wstring sDir );
		void Write(bool bGlossary = false);
		std::wstring getFilename();
		bool IsEmpty();
	};

	class EndnotesWriter
	{
		std::wstring m_sDir;

	public:
		ContentWriter	m_oNotesWriter;

		EndnotesWriter( std::wstring sDir );
		void Write(bool bGlossary = false);
		std::wstring getFilename();
		bool IsEmpty();
	};
}
