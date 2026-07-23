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

#include "Document.h"

#include "Utils/Utils.h"

#include "../../DesktopEditor/common/Path.h"
#include "../../OfficeUtils/src/ZipFolder.h"

#ifdef BUILDING_WASM_MODULE
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#endif

namespace OFD
{
CPermission::CPermission()
	: m_bEdit(true), m_bAnnot(true), m_bExport(true),
	  m_bSignature(true), m_bWatermark(true), m_bPrintScreen(true)
{}

bool CPermission::Read(CXmlReader& oLiteReader)
{
	if (L"ofd:Permission" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::wstring wsNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		wsNodeName = oLiteReader.GetName();

		if (L"ofd:Edit" == wsNodeName)
			m_bEdit = oLiteReader.GetBoolean();
	}

	return true;
}

CDocument::CDocument()
{}

CDocument::~CDocument()
{
	for (std::pair<int, const CPage*> oElement : m_mPages)
		delete oElement.second;

	for (const COutlineElem* pOutlineElem : m_arOutlines)
		delete pOutlineElem;

	for (const CBookmark* pBookmark : m_arBookmarks)
		delete pBookmark;
}

bool CDocument::Empty() const
{
	return m_mPages.empty();
}

bool CDocument::Read(const std::wstring& wsFilePath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, pFolder->getFullFilePath(L"")))
		return false;

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(wsFilePath, oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:Document" != oLiteReader.GetName())
		return false;

	const std::wstring wsCoreDirectory{pFolder->getFullFilePath(NSSystemPath::GetDirectoryName(wsFilePath))};
	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:CommonData" == sNodeName)
			m_oCommonData.Read(oLiteReader, wsCoreDirectory, pFolder);
		else if ("ofd:Pages" == sNodeName)
		{
			const int nPagesDepth = oLiteReader.GetDepth();

			int nID = -1;
			std::wstring wsBaseLoc;


			while (oLiteReader.ReadNextSiblingNode(nPagesDepth))
			{
				if (L"ofd:Page" != oLiteReader.GetName() || 2 > oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
					continue;

				do
				{
					if (L"ID" == oLiteReader.GetName())
						nID = oLiteReader.GetInteger(true);
					else if (L"BaseLoc" == oLiteReader.GetName())
						wsBaseLoc = oLiteReader.GetText();
				}while (oLiteReader.MoveToNextAttribute());

				if (wsBaseLoc.empty())
					continue;

				if (-1 == nID)
					nID = m_mPages.size() + 1;

				CPage* pPage = CPage::Read(wsBaseLoc, wsCoreDirectory, pFolder);

				if (nullptr != pPage)
					m_mPages.insert(std::make_pair(m_mPages.size(), pPage));

				wsBaseLoc.clear();
				oLiteReader.MoveToElement();
			}
		}
		else if ("ofd:Permissions" == sNodeName)
			m_oPermission.Read(oLiteReader);
		else if ("ofd:Annotations" == sNodeName)
			m_oAnnotation.Read(oLiteReader.GetText2(), wsCoreDirectory, pFolder);
		else if ("ofd:Outlines" == sNodeName)
		{
			const int nOutlineDepth{oLiteReader.GetDepth()};

			while (oLiteReader.ReadNextSiblingNode2(nOutlineDepth))
			{
				if ("ofd:OutlineElem" == oLiteReader.GetNameA())
					AddOutlineElem(new COutlineElem(oLiteReader));
			}
		}
		else if ("ofd:Bookmarks" == sNodeName)
		{
			const int nBookmarksDepth{oLiteReader.GetDepth()};

			while (oLiteReader.ReadNextSiblingNode2(nBookmarksDepth))
			{
				if ("ofd:Bookmark" == sNodeName)
					AddBookmark(new CBookmark(oLiteReader));
			}
		}
	}

	return true;
}

bool CDocument::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	if (nullptr == pRenderer)
		return false;

	std::map<unsigned int, const CPage*>::const_iterator itFound = m_mPages.find(nPageIndex);

	if (itFound == m_mPages.cend())
		return false;

	itFound->second->Draw(pRenderer, m_oCommonData, EPageType::Page);

	m_oAnnotation.Draw(pRenderer, m_oCommonData, EPageType::Anotation);

	return true;
}

unsigned int CDocument::GetPageCount() const
{
	return m_mPages.size();
}

bool CDocument::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	m_oCommonData.GetPageSize(dWidth, dHeight);

	std::map<unsigned int, const CPage*>::const_iterator itFound = m_mPages.find(nPageIndex);

	if (itFound == m_mPages.cend())
		return false;

	itFound->second->GetPageSize(dWidth, dHeight);

	return true;
}

void CDocument::UpdateFonts(CFontChecker* pFontChecker)
{
	m_oCommonData.UpdateFonts(pFontChecker);
}

#ifdef BUILDING_WASM_MODULE
UINT GetNumberPage(const COutlineElem* pOutlineElem, UINT& unMaxNumberPage)
{
	for (const CAction* pActionElement : pOutlineElem->GetActions())
	{
		const IAction* pAction{pActionElement->GetAction()};

		if (nullptr == pAction || EActionType::Goto != pAction->GetType())
			continue;

		const TDest *pDest{((const CGoto*)pAction)->GetDest()};

		if (nullptr == pDest)
			continue;

		unMaxNumberPage = (std::max)(unMaxNumberPage, pDest->m_unPageID);

		return pDest->m_unPageID;
	}

	return unMaxNumberPage;
}

void WriteOutlineElem(const std::vector<const COutlineElem*> arOutlineElems, NSWasm::CData& oRes, UINT unLevel, UINT& unMaxNumberPage)
{
	for (const COutlineElem* pElem : arOutlineElems)
	{
		oRes.AddInt(GetNumberPage(pElem, unMaxNumberPage));
		oRes.AddInt(unLevel);
		oRes.AddDouble(0.);
		oRes.WriteString(pElem->GetTitle());

		WriteOutlineElem(pElem->GetOutlines(), oRes, unLevel + 1, unMaxNumberPage);
	}
}

void CDocument::GetStructure(UINT& unMaxNumberPage, NSWasm::CData& oRes) const
{
	WriteOutlineElem(m_arOutlines, oRes, 1, unMaxNumberPage);
}

void CDocument::GetLinks(UINT unPageIndex, NSWasm::CData& oRes) const
{
	std::map<unsigned int, const CPage*>::const_iterator itFound = m_mPages.find(unPageIndex);

	if (itFound == m_mPages.cend())
		return ;

	itFound->second->GetLinks(oRes);
}
#endif

void CDocument::AddOutlineElem(const COutlineElem* pOutlineElem)
{
	if (nullptr != pOutlineElem)
		m_arOutlines.push_back(pOutlineElem);
}

void CDocument::AddBookmark(const CBookmark* pBookmark)
{
	if (nullptr != pBookmark)
		m_arBookmarks.push_back(pBookmark);
}
}
