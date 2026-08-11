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

#include "HWPFile.h"

#include "HwpDoc/Common/WriterContext.h"
#include "../DesktopEditor/common/File.h"

#include "HwpDoc/Conversion/Converter2OOXML.h"

CHWPFile::CHWPFile()
	: m_pInternal(new HWP::CWriterContext())
{}

CHWPFile::~CHWPFile()
{
	if (nullptr != m_pInternal)
		delete m_pInternal;
}

void CHWPFile::SetTempDirectory(const std::wstring& wsTempDirectory)
{
	m_wsTempDirectory = wsTempDirectory;
}

bool CHWPFile::OpenHWP(const std::wstring& wsFilePath, const std::wstring& wsPassword)
{
	if (nullptr == m_pInternal)
		return false;

	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWP, wsPassword);
}

bool CHWPFile::OpenHWPX(const std::wstring& wsFilePath, const std::wstring& wsPassword)
{
	if (nullptr == m_pInternal)
		return false;

	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWPX, wsPassword);
}

bool CHWPFile::IsEncrypted() const
{
	return (nullptr != m_pInternal) ? m_pInternal->Encrypted() : false;
}

bool CHWPFile::IsWrongPassword() const
{
	return (nullptr != m_pInternal) ? HWP::EHwpxCryptResult::WRONG_PASSWORD == m_pInternal->GetCryptResult() : false;
}

bool CHWPFile::OpenHWPML(const std::wstring &wsFilePath)
{
	if (nullptr == m_pInternal)
		return false;
	
	return m_pInternal->Open(wsFilePath, HWP::EHanType::HWPML);
}

void CHWPFile::Close()
{
	if (nullptr != m_pInternal)
		m_pInternal->Close();
}

bool CHWPFile::ConvertToOOXML(const std::wstring& wsFilePath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetContext(m_pInternal);
	oConverter.SetTempDirectory(m_wsTempDirectory);

	return oConverter.ConvertToFile(wsFilePath);
}

bool CHWPFile::ConvertToOOXML_Dir(const std::wstring& wsDirectoryPath)
{
	HWP::CConverter2OOXML oConverter;

	oConverter.SetContext(m_pInternal);

	return oConverter.ConvertToDir(wsDirectoryPath);
}

bool CHWPFile::IsHWPFormat(const std::wstring& wsFilePath)
{
	return HWP::EHanType::HWP == HWP::CWriterContext::DetectHancom(wsFilePath);
}

bool CHWPFile::IsHWPXFormat(const std::wstring& wsFilePath)
{
	return HWP::EHanType::HWPX == HWP::CWriterContext::DetectHancom(wsFilePath);
}

bool CHWPFile::IsHWPMLFormat(const std::wstring &wsFilePath)
{
	return HWP::EHanType::HWPML == HWP::CWriterContext::DetectHancom(wsFilePath);
}
