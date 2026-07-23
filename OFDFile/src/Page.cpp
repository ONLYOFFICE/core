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

#include "Page.h"

#include "Utils/Utils.h"

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/common/File.h"
#include "../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
CPage::CPage()
    : m_parTemplatePage{0, EZOrder::Background}
{}

CPage::~CPage()
{}

CPage* CPage::Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, wsRootPath))
		return nullptr;

	std::wstring wsNormalizedPath = CombinePaths(wsRootPath, wsFilePath);

	if (L"xml" != NSFile::GetFileExtention(wsNormalizedPath))
		wsNormalizedPath = CombinePaths(wsNormalizedPath, L"Content.xml");

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(wsNormalizedPath, oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:Page" != oLiteReader.GetName())
		return nullptr;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	CPage *pPage = new CPage();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Content" == wsNodeName)
			pPage->m_oContent.Read(oLiteReader, pFolder);
		else if (L"ofd:Area" == wsNodeName)
			pPage->m_oArea.Read(oLiteReader);
		else if (L"ofd:Template" == wsNodeName && 0 != oLiteReader.GetAttributesCount() && oLiteReader.MoveToFirstAttribute())
		{
			std::string sAttributeName;

			do
			{
				sAttributeName = oLiteReader.GetNameA();

				if ("ZOrder" == sAttributeName)
					pPage->m_parTemplatePage.second = GetZOrderFromString(oLiteReader.GetTextA());
				else if ("TemplateID" == sAttributeName)
					pPage->m_parTemplatePage.first = oLiteReader.GetUInteger(true);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();
		}
	}

	return pPage;
}

void CPage::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer)
		return;

	pRenderer->BeginCommand(c_nImageType);

	if (0 != m_parTemplatePage.first)
	{
		const CTemplatePage *pTemplatePage = oCommonData.GetTemplatePage(m_parTemplatePage.first, m_parTemplatePage.second);

		if (nullptr != pTemplatePage && EZOrder::Background == pTemplatePage->GetZOrder() && nullptr != pTemplatePage->GetPage())
			pTemplatePage->GetPage()->Draw(pRenderer, oCommonData, EPageType::TemplatePage);
	}

	m_oContent.Draw(pRenderer, oCommonData, ePageType);

	pRenderer->EndCommand(c_nImageType);
}

void CPage::GetPageSize(double& dWidth, double& dHeight) const
{
	TBox oPhysicalBox{m_oArea.GetPhysicalBox()};

	if (oPhysicalBox.Empty())
		return;

	dWidth  = oPhysicalBox.m_dWidth;
	dHeight = oPhysicalBox.m_dHeight;
}

#ifdef BUILDING_WASM_MODULE
void CPage::GetLinks(NSWasm::CData& oRes) const
{
	m_oContent.GetLinks(oRes);
}
#endif
}
