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

#include "Base.h"
#include "Utils/Utils.h"

#include "../../OfficeUtils/src/ZipFolder.h"

#ifdef BUILDING_WASM_MODULE
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#endif

namespace OFD
{
#define IF_CHECK_NODE(node_name, varible_name)\
if (node_name == sNodeName)\
	varible_name = oLiteReader.GetText2()

#define ELSE_IF_CHECK_NODE(node_name, varible_name)\
else if (node_name == sNodeName)\
	varible_name = oLiteReader.GetText2()

EDocUsege GetDocUsage(const std::wstring& wsValue)
{
	if (L"EBook" == wsValue)
		return EDocUsege::EBook;
	else if (L"ENewsPaper" == wsValue)
		return EDocUsege::ENewsPaper;
	else if (L"EMagnize" == wsValue)
		return EDocUsege::EMagzine;
	else
		return EDocUsege::Normal;
}

CDocInfo::CDocInfo()
	: m_eDocUsage(EDocUsege::Normal)
{}

bool CDocInfo::Read(CXmlReader& oLiteReader)
{
	if ("ofd:DocInfo" != oLiteReader.GetNameA())
		return false;

	const int nDepth{oLiteReader.GetDepth()};

	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		IF_CHECK_NODE("ofd:DocID", m_wsDocId);
		ELSE_IF_CHECK_NODE("ofd:Title", m_wsTitle);
		ELSE_IF_CHECK_NODE("ofd:Author", m_wsAuthor);
		ELSE_IF_CHECK_NODE("ofd:Subject", m_wsSubject);
		ELSE_IF_CHECK_NODE("ofd:Abstruct", m_wsAbstruct);
		ELSE_IF_CHECK_NODE("ofd:CreationDate", m_wsCreationDate);
		ELSE_IF_CHECK_NODE("ofd:ModDate", m_wsModDate);
		ELSE_IF_CHECK_NODE("ofd:Cover", m_wsCover);
		ELSE_IF_CHECK_NODE("ofd:Creator", m_wsCreator);
		ELSE_IF_CHECK_NODE("ofd:CreatorVersion", m_wsCreatorVersion);
		else if ("ofd:DocUsage" == sNodeName)
			m_eDocUsage = GetDocUsage(oLiteReader.GetText2());
		else if ("ofd:CustomDatas" == sNodeName)
		{
			const int nCustomDatasDepth{oLiteReader.GetDepth()};

			std::wstring wsName, wsData;

			while (oLiteReader.ReadNextSiblingNode(nCustomDatasDepth))
			{
				if ("ofd:CustomData" != oLiteReader.GetNameA() || !oLiteReader.MoveToFirstAttribute())
					continue;

				do
				{
					if ("Name" == oLiteReader.GetNameA())
					{
						wsName = oLiteReader.GetText();
						break;
					}
				}while(oLiteReader.MoveToNextAttribute());

				oLiteReader.MoveToElement();

				wsData = oLiteReader.GetText2();

				if (!wsName.empty() && !wsData.empty())
					m_arCustomData.push_back(std::make_pair(wsName, wsData));

				wsName.clear();
				wsData.clear();
			}
		}
	}

	return true;
}

std::wstring CDocInfo::GetInfo() const
{
	std::wstring wsInfo;

	#define STRINGIFY(str) L###str
	#define TO_WSTRING(str) STRINGIFY(str)
	#define WRITE_INFO(info_name)\
	if (!m_ws##info_name.empty())\
		wsInfo += L"\"" + std::wstring(TO_WSTRING(info_name)) + L"\":\"" + m_ws##info_name + L"\","

	WRITE_INFO(DocId);
	WRITE_INFO(Title);
	WRITE_INFO(Author);
	WRITE_INFO(Subject);
	WRITE_INFO(Abstruct);
	WRITE_INFO(CreationDate);
	WRITE_INFO(ModDate);
	WRITE_INFO(Cover);
	WRITE_INFO(Creator);
	WRITE_INFO(CreatorVersion);

	switch (m_eDocUsage)
	{
		case EDocUsege::Normal:     break;
		case EDocUsege::EBook:      wsInfo += L"\"DocUsage\":\"Book\",";      break;
		case EDocUsege::ENewsPaper: wsInfo += L"\"DocUsage\":\"NewsPaper\","; break;
		case EDocUsege::EMagzine:   wsInfo += L"\"DocUsage\":\"Magzine\",";   break;
	}

	if (m_arCustomData.empty())
	{
		wsInfo.pop_back();
		return wsInfo;
	}

	wsInfo += L"\"CustomDatas\":{";

	for (const std::pair<std::wstring, std::wstring>& oCustomData : m_arCustomData)
		wsInfo += L'"' + oCustomData.first + L"\":\"" + oCustomData.second + L"\",";

	wsInfo.pop_back();

	wsInfo += L'}';

	return wsInfo;
}

bool CDocBody::ReadSignatures(const std::wstring& wsFilePath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, pFolder->getFullFilePath(L"")))
		return false;

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(wsFilePath, oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:Signatures" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;
	unsigned int unMaxSignId = 0;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:MaxSignId" == sNodeName)
			unMaxSignId = oLiteReader.GetUInteger();
		else if ("ofd:Signature" == sNodeName)
		{
			if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
				continue;

			do
			{
				if ("BaseLoc" == oLiteReader.GetNameA())
					AddToContainer(CSignature::Read(oLiteReader.GetText(), pFolder), m_arSignatures);
			} while (oLiteReader.MoveToNextAttribute());

			oLiteReader.MoveToElement();
		}
	}

	return true;
}

CDocBody::CDocBody()
{}

CDocBody::~CDocBody()
{
	ClearContainer(m_arSignatures);
}

CDocBody* CDocBody::Read(CXmlReader& oLiteReader, IFolder* pFolder)
{
	if (L"ofd:DocBody" != oLiteReader.GetName())
		return nullptr;

	const int nDepth = oLiteReader.GetDepth();
	std::string sNodeName;

	CDocBody *pDocBody = new CDocBody();

	if (nullptr == pDocBody)
		return nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:DocInfo" == sNodeName)
		{
			if (!pDocBody->m_oDocInfo.Read(oLiteReader))
			{
				delete pDocBody;
				return nullptr;
			}
		}
		else if ("ofd:DocRoot" == sNodeName)
			pDocBody->m_oDocument.Read(oLiteReader.GetText2(), pFolder);
		else if ("ofd:Signatures" == sNodeName)
			pDocBody->ReadSignatures(oLiteReader.GetText2(), pFolder);
	}

	return pDocBody;
}

bool CDocBody::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	const bool bResult = m_oDocument.DrawPage(pRenderer, nPageIndex);

	for (const CSignature* pSignature : m_arSignatures)
		if (pSignature->Draw(pRenderer, nPageIndex, nullptr))
			break;

	return bResult;
}

unsigned int CDocBody::GetPageCount() const
{
	return m_oDocument.GetPageCount();
}

bool CDocBody::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	return m_oDocument.GetPageSize(nPageIndex, dWidth, dHeight);
}

void CDocBody::UpdateFonts(CFontChecker* pFontChecker)
{
	m_oDocument.UpdateFonts(pFontChecker);
}

std::wstring CDocBody::GetInfo() const
{
	return m_oDocInfo.GetInfo();
}

#ifdef BUILDING_WASM_MODULE
void CDocBody::GetStructure(UINT& unMaxNumberPage, NSWasm::CData& oRes) const
{
	m_oDocument.GetStructure(unMaxNumberPage, oRes);
}

void CDocBody::GetLinks(UINT unPageIndex, NSWasm::CData& oRes) const
{
	m_oDocument.GetLinks(unPageIndex, oRes);
}
#endif

CBase::CBase()
{}

CBase::~CBase()
{
	for (const CDocBody* pDocBody : m_arDocBodies)
		RELEASEOBJECT(pDocBody);
}

bool CBase::Read(IFolder* pFolder)
{
	if (nullptr == pFolder || !pFolder->existsXml(L"OFD.xml"))
		return false;

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(L"OFD.xml", oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:OFD" != oLiteReader.GetName())
		return false;

	const int nDepth = oLiteReader.GetDepth();

	CDocBody* pDocBody = nullptr;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		pDocBody = CDocBody::Read(oLiteReader, pFolder);
		if (nullptr != pDocBody)
			m_arDocBodies.push_back(pDocBody);
	}

	return !m_arDocBodies.empty();
}

void CBase::DrawPage(IRenderer* pRenderer, int nPageIndex) const
{
	for (const CDocBody* pDocBody : m_arDocBodies)
		if (pDocBody->DrawPage(pRenderer, nPageIndex))
			return;
}

unsigned int CBase::GetPageCount() const
{
	unsigned int unCount = 0;

	for (const CDocBody* pDocBody : m_arDocBodies)
		unCount += pDocBody->GetPageCount();

	return unCount;
}

void CBase::GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const
{
	for (const CDocBody* pDocBody : m_arDocBodies)
		if (pDocBody->GetPageSize(nPageIndex, dWidth, dHeight))
			return;
}

void CBase::UpdateFonts(CFontChecker* pFontChecker)
{
	for (CDocBody* pDocBody : m_arDocBodies)
		pDocBody->UpdateFonts(pFontChecker);
}

std::wstring CBase::GetInfo() const
{
	if (m_arDocBodies.empty())
		return std::wstring();

	if (1 == m_arDocBodies.size())
		return m_arDocBodies.front()->GetInfo();

	std::wstring wsInfo;

	for (size_t unIndex = 0; unIndex < m_arDocBodies.size(); ++unIndex)
		wsInfo += L"\"Document " + std::to_wstring(unIndex + 1) + L"\":{" + m_arDocBodies[unIndex]->GetInfo() + L"},";

	wsInfo.pop_back();

	return wsInfo;
}

#ifdef BUILDING_WASM_MODULE
void CBase::GetStructure(UINT& unMaxNumberPage, NSWasm::CData& oRes) const
{
	for(CDocBody* pDocBody : m_arDocBodies)
		pDocBody->GetStructure(unMaxNumberPage, oRes);
}

void CBase::GetLinks(UINT unPageIndex, NSWasm::CData& oRes) const
{
	for(CDocBody* pDocBody : m_arDocBodies)
		pDocBody->GetLinks(unPageIndex, oRes);
}
#endif
}
