/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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
