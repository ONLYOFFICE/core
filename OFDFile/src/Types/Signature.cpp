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

#include "Signature.h"

#include "../Utils/Utils.h"

#include "../OFDFile_Private.h"

#include "../../../OfficeUtils/src/ZipFolder.h"

namespace OFD
{
CSignature::CSignature()
{}

CSignature* CSignature::Read(const std::wstring& wsFilePath, IFolder* pFolder)
{
	if (wsFilePath.empty() || !CanUseThisPath(wsFilePath, pFolder->getFullFilePath(L"")))
		return nullptr;

	CXmlReader oLiteReader;

	if (!pFolder->getReaderFromFile(wsFilePath, oLiteReader) || !oLiteReader.ReadNextNode() || L"ofd:Signature" != oLiteReader.GetName() || oLiteReader.IsEmptyNode())
		return nullptr;

	CSignature *pSignature = new CSignature();

	pSignature->m_wsRootPath = pFolder->getFullFilePath(L"");

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:SignedInfo" == oLiteReader.GetNameA())
			pSignature->m_oSignedInfo.Read(oLiteReader);
		else if ("ofd:SignedValue" == oLiteReader.GetNameA())
			pSignature->m_wsSignedValue = CombinePaths(pSignature->m_wsRootPath, oLiteReader.GetText2());
	}

	return pSignature;
}

bool CSignature::Draw(IRenderer* pRenderer, unsigned int unPageIndex, NSFonts::IApplicationFonts* pFonts) const
{
	if (nullptr == pRenderer || m_wsSignedValue.empty() ||
	    m_oSignedInfo.m_oStampAnnot.m_unPageRef - 1 != unPageIndex ||
	    m_oSignedInfo.m_oStampAnnot.m_oBoundary.Empty())
		return false;

	COFDFile_Private oFile(pFonts);
	oFile.SetTempDir(m_wsRootPath);

	oFile.LoadFromFile(m_wsSignedValue);

	oFile.DrawPage(pRenderer, 0,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dX,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dY,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dWidth,
	               m_oSignedInfo.m_oStampAnnot.m_oBoundary.m_dHeight);

	return false;
}

void TProvider::Read(CXmlReader& oLiteReader)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ProviderName" == sAttributeName)
			m_wsProviderName = oLiteReader.GetText();
		else if ("Version" == sAttributeName)
			m_wsVersion = oLiteReader.GetText();
		else if ("Company" == sAttributeName)
			m_wsCompany = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

TReference* TReference::Read(CXmlReader& oLiteReader)
{
	if ("ofd:Reference" != oLiteReader.GetNameA() || oLiteReader.IsEmptyElement() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return nullptr;

	TReference* pElement = new TReference();

	do
	{
		if ("FileRef" == oLiteReader.GetNameA())
			pElement->m_wsFileRef = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		if ("ofd:CheckValue" == oLiteReader.GetNameA())
		{
			pElement->m_wsCheckValue = oLiteReader.GetText2();
			break;
		}
	}

	return pElement;
}

TReferences::~TReferences()
{
	ClearContainer(m_arValues);
}

void TReferences::Read(CXmlReader& oLiteReader)
{
	if (oLiteReader.IsEmptyElement() || 0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	do
	{
		if ("CheckMethod" == oLiteReader.GetNameA())
			m_wsCheckMethod = oLiteReader.GetText();
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();

	const int nDepth = oLiteReader.GetDepth();

	while (oLiteReader.ReadNextSiblingNode(nDepth))
		AddToContainer(TReference::Read(oLiteReader), m_arValues);
}

TStampAnnot::TStampAnnot()
	: m_unID(0), m_unPageRef(0)
{}

void TStampAnnot::Read(CXmlReader& oLiteReader)
{
	if (0 == oLiteReader.GetAttributesCount() || !oLiteReader.MoveToFirstAttribute())
		return;

	std::string sAttributeName;

	do
	{
		sAttributeName = oLiteReader.GetNameA();

		if ("ID" == sAttributeName)
			m_unID = oLiteReader.GetUInteger(true);
		else if ("PageRef" == sAttributeName)
			m_unPageRef = oLiteReader.GetUInteger(true);
		else if ("Boundary" == sAttributeName)
			m_oBoundary.Read(oLiteReader.GetTextA());
	} while (oLiteReader.MoveToNextAttribute());

	oLiteReader.MoveToElement();
}

void TSignedInfo::Read(CXmlReader& oLiteReader)
{
	if (oLiteReader.IsEmptyNode())
		return;

	const int nDepth = oLiteReader.GetDepth();

	std::string sNodeName;

	while (oLiteReader.ReadNextSiblingNode(nDepth))
	{
		sNodeName = oLiteReader.GetNameA();

		if ("ofd:Provider" == sNodeName)
			m_oProvider.Read(oLiteReader);
		else if ("ofd:SignatureMethod" == sNodeName)
			m_wsSignatureMethod = oLiteReader.GetText2();
		else if ("ofd:SignatureDateTime" == sNodeName)
			m_wsSignatureDateTime = oLiteReader.GetText2();
		else if ("ofd:References" == sNodeName)
			m_oReferences.Read(oLiteReader);
		else if ("ofd:StampAnnot" == sNodeName)
			m_oStampAnnot.Read(oLiteReader);
	}
}

}
