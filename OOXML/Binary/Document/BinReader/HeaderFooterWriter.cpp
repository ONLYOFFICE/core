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
