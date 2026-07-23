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

#ifndef BASE_H
#define BASE_H

#include "Document.h"
#include "Types/Signature.h"

namespace OFD
{
enum class EDocUsege
{
	Normal,
	EBook,
	ENewsPaper,
	EMagzine
};

class CDocInfo
{
	std::wstring m_wsDocId;
	std::wstring m_wsTitle;
	std::wstring m_wsAuthor;
	std::wstring m_wsSubject;
	std::wstring m_wsAbstruct;

	std::wstring m_wsCreationDate;
	std::wstring m_wsModDate;

	EDocUsege    m_eDocUsage;
	std::wstring m_wsCover;
	std::vector<std::wstring> m_arKeywords;

	std::wstring m_wsCreator;
	std::wstring m_wsCreatorVersion;

	std::vector<std::pair<std::wstring, std::wstring>> m_arCustomData;
public:
	CDocInfo();
	bool Read(CXmlReader& oLiteReader);

	std::wstring GetInfo() const;
};

class CDocBody
{
	CDocInfo  m_oDocInfo;
	CDocument m_oDocument;
	// std::wstring m_wsVersions;
	std::vector<CSignature*> m_arSignatures;

	bool ReadSignatures(const std::wstring& wsFilePath, IFolder* pFolder);

public:
	CDocBody();
	~CDocBody();

	static CDocBody* Read(CXmlReader& oLiteReader, IFolder* pFolder);

	bool DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	bool GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;

	void UpdateFonts(CFontChecker* pFontChecker);

	std::wstring GetInfo() const;

	#ifdef BUILDING_WASM_MODULE
	void GetStructure(UINT& unMaxNumberPage, NSWasm::CData& oRes) const;
	void GetLinks(UINT unPageIndex, NSWasm::CData& oRes) const;
	#endif
};

class CBase
{
	std::vector<CDocBody*> m_arDocBodies;
public:
	CBase();
	~CBase();

	bool Read(IFolder* pFolder);
	void DrawPage(IRenderer* pRenderer, int nPageIndex) const;

	unsigned int GetPageCount() const;
	void GetPageSize(int nPageIndex, double& dWidth, double& dHeight) const;

	void UpdateFonts(CFontChecker* pFontChecker);

	std::wstring GetInfo() const;

	#ifdef BUILDING_WASM_MODULE
	void GetStructure(UINT& unMaxNumberPage, NSWasm::CData& oRes) const;
	void GetLinks(UINT unPageIndex, NSWasm::CData& oRes) const;
	#endif
};
}

#endif // BASE_H
