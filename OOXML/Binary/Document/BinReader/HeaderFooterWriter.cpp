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

#include "HeaderFooterWriter.h"

namespace Writers
{
	HdrFtrItem::HdrFtrItem(SimpleTypes::EHdrFtr _eType)
	{
		eType = _eType;
	}

	bool HdrFtrItem::IsEmpty()
	{
		return m_sFilename.empty();
	}

	HeaderFooterWriter::HeaderFooterWriter(std::wstring sDir) : m_sDir(sDir)
	{
	}
	HeaderFooterWriter::~HeaderFooterWriter()
	{
		for (size_t i = 0, length = m_aHeaders.size(); i < length; ++i)
			delete m_aHeaders[i];
		m_aHeaders.clear();

		for (size_t i = 0, length = m_aFooters.size(); i < length; ++i)
			delete m_aFooters[i];
		m_aFooters.clear();
	}
	void HeaderFooterWriter::Write(bool bGlossary)
	{
		for (size_t i = 0, length = m_aHeaders.size(); i < length; ++i)
		{
			HdrFtrItem* pHeader = m_aHeaders[i];
			WriteItem(L"header", pHeader->m_sFilename, pHeader->Header, true);
		}
		for (size_t i = 0, length = m_aFooters.size(); i < length; ++i)
		{
			HdrFtrItem* pFooter = m_aFooters[i];
			WriteItem(L"footer", pFooter->m_sFilename, pFooter->Header, false);
		}
	}
	void HeaderFooterWriter::WriteItem( std::wstring sHeader,  std::wstring& sFilename, ContentWriter& m_oWriter, bool bHeader)
	{
		OOX::CPath filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + FILE_SEPARATOR_STR + sFilename;

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath.GetPath());

		if (bHeader)
			oFile.WriteStringUTF8(g_string_hdr_Start);
		else
			oFile.WriteStringUTF8(g_string_ftr_Start);
		oFile.WriteStringUTF8(m_oWriter.m_oContent.GetData());
		if (bHeader)
			oFile.WriteStringUTF8(g_string_hdr_End);
		else
			oFile.WriteStringUTF8(g_string_ftr_End);
		oFile.CloseFile();
	}

	FootnotesWriter::FootnotesWriter( std::wstring sDir ) : m_sDir(sDir)
	{
	}
	void FootnotesWriter::Write(bool bGlossary)
	{
		if(IsEmpty()) return;

		std::wstring sFilename = getFilename();

		 std::wstring filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + (bGlossary ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"") + FILE_SEPARATOR_STR + sFilename;

		NSFile::CFileBinary oFile;
		oFile.CreateFileW (filePath);
		oFile.WriteStringUTF8 (g_string_footnotes_Start);
		oFile.WriteStringUTF8 (m_oNotesWriter.m_oContent.GetData());
		oFile.WriteStringUTF8 (g_string_footnotes_End);
		oFile.CloseFile();
	}
	std::wstring FootnotesWriter::getFilename()
	{
		return L"footnotes.xml";
	}
	bool FootnotesWriter::IsEmpty()
	{
		return !(m_oNotesWriter.m_oContent.GetCurSize() > 0);
	}

	EndnotesWriter::EndnotesWriter( std::wstring sDir ) : m_sDir(sDir)
	{
	}
	void EndnotesWriter::Write(bool bGlossary)
	{
		if(IsEmpty()) return;

		std::wstring sFilename = getFilename();

		 std::wstring filePath = m_sDir + FILE_SEPARATOR_STR + L"word" + (bGlossary ? (FILE_SEPARATOR_STR + std::wstring(L"glossary")) : L"") + FILE_SEPARATOR_STR + sFilename;

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath);
		oFile.WriteStringUTF8(g_string_endnotes_Start);
		oFile.WriteStringUTF8(m_oNotesWriter.m_oContent.GetData());
		oFile.WriteStringUTF8(g_string_endnotes_End);
		oFile.CloseFile();
	}
	std::wstring EndnotesWriter::getFilename()
	{
		return L"endnotes.xml";
	}
	bool EndnotesWriter::IsEmpty()
	{
		return !(m_oNotesWriter.m_oContent.GetCurSize() > 0);
	}
}
