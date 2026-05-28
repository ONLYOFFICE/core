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
#ifndef _PDF_WRITER_SRC_CATALOG_H
#define _PDF_WRITER_SRC_CATALOG_H

#include "Objects.h"

namespace PdfWriter
{
	class CPageTree;
	class CDestination;
	class CMetadata;
	class CInfoDict;
	class CAnnotation;

	enum class StructElemType
	{
		Link,
		Form,
		Annot
	};

	class CStructureTreeRoot : public CDictObject
	{
	public:
		CStructureTreeRoot(CXref* pXref);

		void AddAnnot(CAnnotation* pAnnot, CDictObject* pPage, StructElemType eType);

	private:
		CXref* m_pXref;
		int m_nParentTreeKey;
		CDictObject* m_pNamespace;
		CDictObject* m_pParentTree;
		CDictObject* m_pDocumentElem;
		CArrayObject* m_pDocumentK;
	};

	class CCatalog : public CDictObject
	{
	public:
		CCatalog(CXref* pXref);
		CCatalog();
		CPageTree*   GetRoot() const;
		EPageLayout  GetPageLayout() const;
		void         SetPageLayout(EPageLayout eLayout);
		EPageMode    GetPageMode() const;
		void         SetPageMode(EPageMode eMode);
		void         SetOpenAction(CDestination* pOpenAction);
		void         AddPageLabel(unsigned int unIndex, CDictObject* pPageLabel);
		CMetadata*   AddMetadata(CXref* pXref, CInfoDict* pInfo, const std::string& sAltTitle);
		CStructureTreeRoot* GetStructTreeRoot();
		EDictType    GetDictType()
		{
			return dict_type_CATALOG;
		}

	private:
		CStructureTreeRoot* m_pStructTreeRoot;
	};
}

#endif // _PDF_WRITER_SRC_CATALOG_H
