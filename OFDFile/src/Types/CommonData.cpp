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

#include "CommonData.h"
#include "../Utils/Utils.h"

namespace OFD
{
CCommonData::CCommonData()
	: m_unMaxUnitID(0), m_pPublicRes(nullptr), m_pDocumentRes(nullptr)
{}

CCommonData::~CCommonData()
{
	if (nullptr != m_pPublicRes)
		delete m_pPublicRes;

	if (nullptr != m_pDocumentRes)
		delete m_pDocumentRes;
}

bool CCommonData::Read(CXmlReader& oLiteReader, const std::wstring& wsRootPath, IFolder* pFolder)
{
	if ("ofd:CommonData" != oLiteReader.GetNameA())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:PageArea" == sNodeName)
			m_oPageArea.Read(oLiteReader);
		else if ("ofd:PublicRes" == sNodeName)
		{
			if (nullptr == m_pPublicRes)
				m_pPublicRes = new CRes();

			m_pPublicRes->Read(oLiteReader.GetText2(), wsRootPath, pFolder);
		}
		else if ("ofd:DocumentRes" == sNodeName)
		{
			if(nullptr == m_pDocumentRes)
				m_pDocumentRes = new CRes();

			m_pDocumentRes->Read(oLiteReader.GetText2(), wsRootPath, pFolder);
		}
		else if ("ofd:MaxUnitID" == sNodeName)
			m_unMaxUnitID = oLiteReader.GetUInteger();
		else if ("ofd:TemplatePage" == sNodeName)
			AddToContainer(new const CTemplatePage(oLiteReader, wsRootPath, pFolder), m_arTemplatePages);
		// else if (L"ofd:DefaultCS" == wsNodeName)
	}

	return true;
}

void CCommonData::GetPageSize(double& dWidth, double& dHeight) const
{
	TBox oPhysicalBox{m_oPageArea.GetPhysicalBox()};

	if (oPhysicalBox.Empty())
		return;

	dWidth  = oPhysicalBox.m_dWidth;
	dHeight = oPhysicalBox.m_dHeight;
}

const CRes* CCommonData::GetPublicRes() const
{
	return m_pPublicRes;
}

const CRes* CCommonData::GetDocumentRes() const
{
	return m_pDocumentRes;
}

const CTemplatePage* CCommonData::GetTemplatePage(unsigned int unTemplateID, EZOrder eZOrder) const
{
	for (const CTemplatePage* pTemplatePage : m_arTemplatePages)
	{
		if (unTemplateID == pTemplatePage->GetID() && eZOrder == pTemplatePage->GetZOrder())
			return pTemplatePage;
	}

	return nullptr;
}

void CCommonData::UpdateFonts(CFontChecker* pFontChecker)
{
	if (nullptr != m_pPublicRes)
		m_pPublicRes->UpdateFonts(pFontChecker);

	if (nullptr != m_pDocumentRes)
		m_pDocumentRes->UpdateFonts(pFontChecker);
}
}
