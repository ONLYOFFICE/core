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

#ifndef ANNOTATION_H
#define ANNOTATION_H

#include <string>
#include <vector>

#include "Content/PageBlock.h"

namespace OFD
{
class CParameter
{
public:
	CParameter(CXmlReader& oLiteReader);
private:
	std::wstring m_wsName;
	std::wstring m_wsValue;
};

typedef CPageBlock CAppearance;

enum class EAnnotType
{
	Link,
	Path,
	Highlight,
	Stamp,
	Watermark
};

class CAnnot
{
public:
	CAnnot(CXmlReader& oLiteReader, IFolder *pFolder);
	~CAnnot();

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:
	EAnnotType m_eType;
	bool m_bVisible;
	bool m_bPrint;
	bool m_bNoZoom;
	bool m_bNoRotate;
	bool m_bReadOnly;

	std::vector<CParameter*>  m_arParameters;
	std::vector<CAppearance*> m_arAppearances;
};

class CPageAnnot
{
public:
	CPageAnnot();

	static CPageAnnot* Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath, IFolder* pFolder);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:

	std::vector<CAnnot*> m_arAnnots;
};

class CAnnotation
{
public:
	CAnnotation();
	~CAnnotation();

	bool Read(const std::wstring& wsFilePath, const std::wstring& wsRootPath, IFolder* pFolder);

	void Draw(IRenderer* pRenderer, const CCommonData& oCommonData, EPageType ePageType) const;
private:
	CPageAnnot *m_pPageAnnot;
};
}

#endif // ANNOTATION_H
