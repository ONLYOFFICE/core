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

#include "ImageObject.h"

#include "../../../DesktopEditor/graphics/Image.h"
#include "../../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
CImageObject::CImageObject(CXmlReader& oReader, IFolder* pFolder)
    : IPageBlock(oReader), CGraphicUnit(oReader), m_unMultiMediaID(0), m_pFolder(pFolder)
{
	if ("ofd:ImageObject" != oReader.GetNameA() || 0 == oReader.GetAttributesCount() || !oReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oReader.GetNameA();

		if ("ResourceID" == sAttributeName)
		{
			m_unMultiMediaID = oReader.GetUInteger(true);
			break;
		}
	} while(oReader.MoveToNextAttribute());

	oReader.MoveToElement();

	const int nDepth{oReader.GetDepth()};

	while(oReader.ReadNextSiblingNode2(nDepth))
	{
		ReadChildren(oReader);
	}
}

void CImageObject::Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const
{
	if (nullptr == pRenderer || nullptr == m_pFolder || nullptr == oCommonData.GetDocumentRes())
		return;

	const CMultiMedia* pMultiMedia = oCommonData.GetDocumentRes()->GetMultiMedia(m_unMultiMediaID);

	if (nullptr == pMultiMedia)
		return;

	TMatrix oOldTransform;
	CGraphicUnit::Apply(pRenderer, oOldTransform);

	const std::wstring wsFilePath = pMultiMedia->GetFilePath();

	if (wsFilePath.empty() || !m_pFolder->exists(wsFilePath))
		return;

	//TODO::It can be done without type determination. In all cases, just use Aggplus::CImage
	switch (m_pFolder->getType())
	{
		case IFolder::iftFolder:
		{
			pRenderer->DrawImageFromFile(m_pFolder->getFullFilePath(wsFilePath), 0, 0, 1, 1);
			break;
		}
		case IFolder::iftZip:
		{
			IFolder::CBuffer *pBuffer;
			if (m_pFolder->read(wsFilePath, pBuffer))
			{
				Aggplus::CImage oImage;
				oImage.Decode(pBuffer->Buffer, pBuffer->Size);
				delete pBuffer;

				pRenderer->DrawImage(&oImage, 0, 0, 1, 1);
			}
			break;
		}
	}

	pRenderer->SetTransform(oOldTransform.m_dM11, oOldTransform.m_dM12, oOldTransform.m_dM21, oOldTransform.m_dM22, oOldTransform.m_dDx, oOldTransform.m_dDy);
}

#ifdef BUILDING_WASM_MODULE
void CImageObject::GetLinks(NSWasm::CData& oRes) const
{
	CGraphicUnit::GetLinks(oRes);
}
#endif
}
