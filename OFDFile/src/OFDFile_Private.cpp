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

#include "OFDFile_Private.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../OfficeUtils/src/ZipFolder.h"

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/IRenderer.h"

#include "Utils/CFontChecker.h"
#include "Utils/Utils.h"

#ifdef BUILDING_WASM_MODULE
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#endif

COFDFile_Private::COFDFile_Private(NSFonts::IApplicationFonts* pFonts)
	: m_pFolder(nullptr), m_bIsTempDirOwner(false)
{
	m_pFontChecker = new OFD::CFontChecker(pFonts, m_pFolder);
}

COFDFile_Private::~COFDFile_Private()
{
	Close();

	if (m_bIsTempDirOwner && !m_wsTempDir.empty())
		NSDirectory::DeleteDirectory(m_wsTempDir);

	if (nullptr != m_pFontChecker)
		delete m_pFontChecker;
}

void COFDFile_Private::Close()
{
	if (nullptr != m_pFolder)
	{
		delete m_pFolder;
		m_pFolder = nullptr;
	}

	if (nullptr != m_pFontChecker)
		m_pFontChecker->Clear();
}

void COFDFile_Private::SetTempDir(const std::wstring& wsPath)
{
	m_wsTempDir       = wsPath;
	m_bIsTempDirOwner = m_wsTempDir.empty();
}

std::wstring COFDFile_Private::GetTempDir() const
{
	return m_wsTempDir;
}

bool COFDFile_Private::Read()
{
	if (nullptr == m_pFolder)
		return false;

	return m_oBase.Read(m_pFolder);
}

bool COFDFile_Private::LoadFromFile(const std::wstring& wsFilePath)
{
	Close();

	if (wsFilePath.empty())
		return false;

	if (m_wsTempDir.empty())
		m_wsTempDir = NSDirectory::CreateDirectoryWithUniqueName(NSDirectory::GetTempPath());

	COfficeUtils oUtils(NULL);

	if (S_OK != oUtils.ExtractToDirectory(wsFilePath, m_wsTempDir, NULL, 0))
		return false;

	m_pFolder = new CFolderSystem(m_wsTempDir);

	return Read();
}

bool COFDFile_Private::LoadFromMemory(BYTE* pData, DWORD ulLength)
{
	Close();

	if (nullptr == pData || 0 == ulLength)
		return false;

	m_pFolder = new CZipFolderMemory(pData, ulLength);

	return Read();
}

unsigned int COFDFile_Private::GetPageCount() const
{
	return m_oBase.GetPageCount();
}

void COFDFile_Private::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	m_oBase.GetPageSize(nPageIndex, dWidth, dHeight);
}

void COFDFile_Private::DrawPage(IRenderer* pRenderer, int nPageIndex)
{
	m_oBase.UpdateFonts(m_pFontChecker);

	m_oBase.DrawPage(pRenderer, nPageIndex);
}

void COFDFile_Private::DrawPage(IRenderer* pRenderer, int nPageIndex, const double& dX, const double& dY, const double& dWidth, const double& dHeight)
{
	if (nullptr == pRenderer)
		return;

	double dPageWidth = 0., dPageHeight = 0.;

	GetPageSize(nPageIndex, dPageWidth, dPageHeight);

	if (OFD::IsZeroValue(dPageWidth) || OFD::IsZeroValue(dPageHeight))
		return;

	double dM11, dM12, dM21, dM22, dDx, dDy;
	pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);

	Aggplus::CMatrix oTransform(dM11, dM12, dM21, dM22, dDx, dDy);

	oTransform.Scale(dWidth / dPageWidth, dHeight / dPageHeight);
	oTransform.Translate(dX, dY);

	pRenderer->SetTransform(oTransform.sx(), oTransform.shy(), oTransform.shx(), oTransform.sy(), oTransform.tx(), oTransform.ty());

	m_oBase.UpdateFonts(m_pFontChecker);

	m_oBase.DrawPage(pRenderer, nPageIndex);

	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
}

NSFonts::IApplicationFonts* COFDFile_Private::GetFonts() const
{
	return (nullptr != m_pFontChecker) ? m_pFontChecker->GetFonts() : nullptr;
}

std::wstring COFDFile_Private::GetInfo() const
{
	std::wstring wsInfo{L"{"};

	double dWidth{0.}, dHeight{0.};
	GetPageSize(0, dWidth, dHeight);

	wsInfo += L"\"PageWidth\":" + std::to_wstring((int)(dWidth * 100)) +
	          L",\"PageHeight\":" + std::to_wstring((int)(dHeight * 100)) +
	          L",\"NumberOfPages\":" + std::to_wstring(GetPageCount());

	const std::wstring wsBaseInfo{m_oBase.GetInfo()};

	return wsInfo + ((!wsBaseInfo.empty()) ? (L',' + wsBaseInfo) : L"") + L'}';
}

BYTE* COFDFile_Private::GetStructure() const
{
	#ifdef BUILDING_WASM_MODULE
	UINT unMaxNumberPage{0};

	NSWasm::CData oRes;
	oRes.SkipLen();

	m_oBase.GetStructure(unMaxNumberPage, oRes);

	oRes.WriteLen();

	BYTE* pRes{oRes.GetBuffer()};
	oRes.ClearWithoutAttack();

	return pRes;
	#endif

	return nullptr;
}

BYTE* COFDFile_Private::GetLinks(int nPageIndex) const
{
	if (nPageIndex < 0)
		return nullptr;

	#ifdef BUILDING_WASM_MODULE
	NSWasm::CData oRes;
	oRes.SkipLen();

	m_oBase.GetLinks(nPageIndex, oRes);

	oRes.WriteLen();

	BYTE* pRes{oRes.GetBuffer()};
	oRes.ClearWithoutAttack();

	return pRes;
	#endif
	return nullptr;
}
