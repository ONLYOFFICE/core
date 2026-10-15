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

#include "OFDFile.h"
#include "src/OFDFile_Private.h"

#ifndef DISABLE_PDF_CONVERTATION
#include "../PdfFile/PdfFile.h"
#endif

COFDFile::COFDFile(NSFonts::IApplicationFonts* pFonts)
	: m_pInternal(new COFDFile_Private(pFonts))
{}

COFDFile::~COFDFile()
{
	COFDFile::Close();

	if (nullptr != m_pInternal)
		delete m_pInternal;
}

bool COFDFile::LoadFromFile(const std::wstring& file, const std::wstring& options, const wchar_t* owner_password, const wchar_t* user_password)
{
	if (nullptr == m_pInternal)
		return false;

	Close();

	return m_pInternal->LoadFromFile(file);
}

bool COFDFile::LoadFromMemory(unsigned char* data, unsigned long length, const std::wstring& options, const wchar_t* owner_password, const wchar_t* user_password)
{
	if (nullptr == m_pInternal)
		return false;

	Close();

	return m_pInternal->LoadFromMemory(data, length);
}

void COFDFile::Close()
{
	if (nullptr != m_pInternal)
		m_pInternal->Close();
}

NSFonts::IApplicationFonts* COFDFile::GetFonts()
{
	return (nullptr != m_pInternal) ? m_pInternal->GetFonts() : nullptr;
}

OfficeDrawingFileType COFDFile::GetType()
{
	return odftOFD;
}

std::wstring COFDFile::GetTempDirectory()
{
	if (nullptr != m_pInternal)
		return m_pInternal->GetTempDir();

	return std::wstring();
}

void COFDFile::SetTempDirectory(const std::wstring& directory)
{
	if (nullptr != m_pInternal)
		m_pInternal->SetTempDir(directory);
}

int COFDFile::GetPagesCount()
{
	if (nullptr != m_pInternal)
		return m_pInternal->GetPageCount();

	return 0;
}

void COFDFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	if (nullptr == m_pInternal)
		return;

	m_pInternal->GetPageSize(nPageIndex, *pdWidth, *pdHeight);

	*pdDpiX = 25.4;
	*pdDpiY = 25.4;
}

void COFDFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak, COfficeDrawingPageParams* pParams)
{
	if (nullptr == pRenderer || nullptr == m_pInternal)
		return;

	m_pInternal->DrawPage(pRenderer, nPageIndex);
}

#ifndef DISABLE_PDF_CONVERTATION
void COFDFile::ConvertToPdf(const std::wstring& wsDstPath)
{
	CPdfFile oPdf(GetFonts());
	oPdf.CreatePdf();
	bool bBreak = false;

	int nPagesCount = GetPagesCount();
	for (int nPageIndex = 0; nPageIndex < nPagesCount; nPageIndex++)
	{
		oPdf.NewPage();
		oPdf.BeginCommand(c_nPageType);

		double dPageDpiX = 96., dPageDpiY = 96.;
		double dWidth = 0., dHeight = 0.;

		GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

		dWidth  *= 25.4 / dPageDpiX;
		dHeight *= 25.4 / dPageDpiY;

		oPdf.put_Width(dWidth);
		oPdf.put_Height(dHeight);

		DrawPageOnRenderer(&oPdf, nPageIndex, &bBreak);

		oPdf.EndCommand(c_nPageType);

#ifdef _DEBUG
		printf("page %d / %d\n", nPageIndex + 1, nPagesCount);
#endif
	}

	oPdf.SaveToFile(wsDstPath);
}

std::wstring COFDFile::GetInfo()
{
	return (nullptr != m_pInternal) ? m_pInternal->GetInfo() : std::wstring();
}

unsigned char* COFDFile::GetStructure()
{
	return (nullptr != m_pInternal) ? m_pInternal->GetStructure() : nullptr;
}

unsigned char* COFDFile::GetLinks(int nPageIndex)
{
	return (nullptr != m_pInternal) ? m_pInternal->GetLinks(nPageIndex) : nullptr;
}

void COFDFile::SetPainter(IOfficeDrawingFilePainter* pPainter)
{}
#endif
