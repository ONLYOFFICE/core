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

#ifndef SIGNATURE_H
#define SIGNATURE_H

#include "../Utils/XmlReader.h"
#include "../Utils/Types.h"

class IFolder;
class IRenderer;

namespace NSFonts { class IApplicationFonts; }

namespace OFD
{
struct TProvider
{
	std::wstring m_wsProviderName;
	std::wstring m_wsVersion;
	std::wstring m_wsCompany;

	void Read(CXmlReader& oLiteReader);
};

struct TReference
{
	std::wstring m_wsFileRef;
	std::wstring m_wsCheckValue;

	static TReference* Read(CXmlReader& oLiteReader);
};

struct TReferences
{
	std::wstring m_wsCheckMethod;
	std::vector<TReference*> m_arValues;

	~TReferences();

	void Read(CXmlReader& oLiteReader);
};

struct TStampAnnot
{
	unsigned int m_unID;
	unsigned int m_unPageRef;
	TBox         m_oBoundary;

	TStampAnnot();

	void Read(CXmlReader& oLiteReader);
};

struct TSignedInfo
{
	TProvider    m_oProvider;
	std::wstring m_wsSignatureMethod;
	std::wstring m_wsSignatureDateTime;
	TReferences  m_oReferences;
	TStampAnnot  m_oStampAnnot;

	void Read(CXmlReader& oLiteReader);
};

class CSignature
{
	TSignedInfo m_oSignedInfo;
	std::wstring m_wsSignedValue;

	std::wstring m_wsRootPath;
public:
	CSignature();

	static CSignature* Read(const std::wstring& wsFilePath, IFolder* pFolder);

	bool Draw(IRenderer* pRenderer, unsigned int unPageIndex, NSFonts::IApplicationFonts* pFonts) const;
};
}

#endif // SIGNATURE_H
