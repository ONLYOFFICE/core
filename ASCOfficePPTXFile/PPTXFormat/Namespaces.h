﻿/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#ifndef PPTXOOX_NAMESPACES_INCLUDE_H_
#define PPTXOOX_NAMESPACES_INCLUDE_H_

#include "../../Common/DocxFormat/Source/Base/Base.h"

namespace PPTX
{
	class Namespace
	{
	public:
		std::wstring m_strName;
		std::wstring m_strLink;

	public:
        Namespace(const wchar_t* sName, const wchar_t* sLink) : m_strName(sName), m_strLink(sLink)
		{
		}
	};
	
	class Namespaces
	{
	public:
		Namespaces() : 
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

	public:
		const Namespace a;
		const Namespace b;
		const Namespace cdr;
		const Namespace cp;
		const Namespace cup;
		const Namespace dc;
		const Namespace dchrt;
		const Namespace dcmitype;
		const Namespace dcterms;
		const Namespace ddgrm;
		const Namespace dgm;
		const Namespace dlckcnv;
		const Namespace dpct;
		const Namespace ds;
		const Namespace m;
		const Namespace o;
		const Namespace p;
		const Namespace pic;
		const Namespace pvml;
		const Namespace r;
		const Namespace s;
		const Namespace sl;
		const Namespace v;
		const Namespace ve;
		const Namespace vp;
		const Namespace vt;
		const Namespace w;
		const Namespace w10;
		const Namespace wne;
		const Namespace wp;
		const Namespace x;
		const Namespace xdr;
		const Namespace xmlns;
		const Namespace xsd;
		const Namespace xsi;
		const Namespace p14;
	};

	static Namespaces g_Namespaces;
} // namespace PPTX

#endif // PPTXOOX_NAMESPACES_INCLUDE_H_
