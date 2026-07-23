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

#include "PageBlock.h"

#include "TextObject.h"
#include "PathObject.h"
#include "ImageObject.h"

#include "../../../DesktopEditor/graphics/IRenderer.h"

namespace OFD
{
CPageBlock::CPageBlock(CXmlReader& oLiteReader, IFolder* pFolder)
	: IPageBlock(oLiteReader)
{
	if (oLiteReader.MoveToFirstAttribute())
	{
		do
		{
			if ("Boundary" != oLiteReader.GetNameA())
				continue;

			m_oBoundary.Read(oLiteReader.GetTextA());
		} while (oLiteReader.MoveToNextAttribute());

		oLiteReader.MoveToElement();
	}

	if (oLiteReader.IsEmptyNode())
		return;

	CPageBlock::ReadIntoContainer(oLiteReader, m_arPageBlocks, pFolder);
}

void CPageBlock::ReadIntoContainer(CXmlReader& oLiteReader, std::vector<IPageBlock*>& arPageBlocks, IFolder* pFolder)
{
	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	IPageBlock* pPageBlock = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();
		pPageBlock = nullptr;

		if (L"ofd:TextObject" == wsNodeName)
			pPageBlock = new CTextObject(oLiteReader);
		else if (L"ofd:PathObject" == wsNodeName)
			pPageBlock = new CPathObject(oLiteReader);
		else if (L"ofd:PageBlock" == wsNodeName)
			pPageBlock = new CPageBlock(oLiteReader, pFolder);
		else if (L"ofd:ImageObject" == wsNodeName)
			pPageBlock = new CImageObject(oLiteReader, pFolder);

		if (nullptr != pPageBlock)
			arPageBlocks.push_back(pPageBlock);
	}
}

void CPageBlock::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer)
		return;

	double dM11, dM12, dM21, dM22, dDx, dDy;

	pRenderer->GetTransform(&dM11, &dM12, &dM21, &dM22, &dDx, &dDy);
	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx + m_oBoundary.m_dX, dDy + m_oBoundary.m_dY);

	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->Draw(pRenderer, oCommonData, ePageType);

	pRenderer->SetTransform(dM11, dM12, dM21, dM22, dDx, dDy);
}

#ifdef BUILDING_WASM_MODULE
void CPageBlock::GetLinks(NSWasm::CData& oRes) const
{
	for (const IPageBlock* pPageBlock : m_arPageBlocks)
		pPageBlock->GetLinks(oRes);
}
#endif
}
