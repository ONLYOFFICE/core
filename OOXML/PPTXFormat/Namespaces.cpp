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

#include "Namespaces.h"

namespace PPTX
{
	Namespace::Namespace(const wchar_t* sName, const wchar_t* sLink) : m_strName(sName), m_strLink(sLink)
	{
	}

	Namespaces::Namespaces() :
		a	(L"a",		L"http://schemas.openxmlformats.org/drawingml/2006/main"),
		b	(L"b",		L"http://schemas.openxmlformats.org/officeDocument/2006/bibliography"),
		cdr	(L"cdr",	L"http://schemas.openxmlformats.org/drawingml/2006/chartDrawing"),
		cp	(L"cp",		L"http://schemas.openxmlformats.org/package/2006/metadata/core-properties"),
		cup	(L"cp",		L"http://schemas.openxmlformats.org/officeDocument/2006/custom-properties"),
		dc	(L"dc",		L"http://purl.org/dc/elements/1.1/"),
		dchrt(L"dchrt", L"http://schemas.openxmlformats.org/drawingml/2006/chart"),
		dcmitype(L"dcmitype", L"http://purl.org/dc/dcmitype/"),
		dcterms(L"dcterms", L"http://purl.org/dc/terms/"),
		ddgrm(L"ddgrm", L"http://schemas.openxmlformats.org/drawingml/2006/diagram"),
		dgm	(L"dgm",	L"http://schemas.openxmlformats.org/drawingml/2006/diagram"),
		dlckcnv(L"dlckcnv", L"http://schemas.openxmlformats.org/drawingml/2006/lockedCanvas"),
		dpct(L"dpct",	L"http://schemas.openxmlformats.org/drawingml/2006/picture"),
		ds	(L"ds",		L"http://schemas.openxmlformats.org/officeDocument/2006/customXml"),
		m	(L"m",		L"http://schemas.openxmlformats.org/officeDocument/2006/math"),
		o	(L"o",		L"urn:schemas-microsoft-com:office:office"),
		p	(L"p",		L"http://schemas.openxmlformats.org/presentationml/2006/main"),
		pic	(L"pic",	L"http://schemas.openxmlformats.org/drawingml/2006/picture"),
		pvml(L"pvml",	L"urn:schemas-microsoft-com:office:powerpoint"),
		r	(L"r",		L"http://schemas.openxmlformats.org/officeDocument/2006/relationships"),
		s	(L"s",		L"http://schemas.openxmlformats.org/officeDocument/2006/sharedTypes"),
		sl	(L"sl",		L"http://schemas.openxmlformats.org/schemaLibrary/2006/main"),
		v	(L"v",		L"urn:schemas-microsoft-com:vml"),
		ve	(L"ve",		L"http://schemas.openxmlformats.org/markup-compatibility/2006"),
		vp	(L"vp",		L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"),
		vt	(L"vt",		L"http://schemas.openxmlformats.org/officeDocument/2006/docPropsVTypes"),
		w	(L"w",		L"http://schemas.openxmlformats.org/wordprocessingml/2006/main"),
		w10	(L"w10",	L"urn:schemas-microsoft-com:office:word"),
		wne	(L"wne",	L"http://schemas.microsoft.com/office/word/2006/wordml"),
		wp	(L"wp",		L"http://schemas.openxmlformats.org/drawingml/2006/wordprocessingDrawing"),
		x	(L"x",		L"urn:schemas-microsoft-com:office:excel"),
		xdr	(L"xdr",	L"http://schemas.openxmlformats.org/drawingml/2006/spreadsheetDrawing"),
		xmlns(L"xmlns",  L"http://schemas.openxmlformats.org/officeDocument/2006/extended-properties"),
		xsd	(L"xsd",	L"http://www.w3.org/2001/XMLSchema"),
		xsi	(L"xsi",	L"http://www.w3.org/2001/XMLSchema-instance"),
		p14	(L"p14",	L"http://schemas.microsoft.com/office/powerpoint/2010/main")
	{
	}
} // namespace PPTX
