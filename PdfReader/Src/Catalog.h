/*
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
#ifndef _PDF_READER_CATALOG_H
#define _PDF_READER_CATALOG_H

#include "GlobalParams.h"

namespace PdfReader
{
	class XRef;
	class Object;
	class Page;
	class PageAttrs;
	struct Ref;
	class LinkDestination;

	//------------------------------------------------------------------------
	// Catalog
	//------------------------------------------------------------------------

	class Catalog
	{
	public:

		Catalog(GlobalParams *pGlobalParams, XRef *pXref);

		~Catalog();

		bool CheckValidation()
		{
			return m_bValid;
		}

		int GetPagesCount()
		{
			return m_nPagesCount;
		}

		Page *GetPage(int nIndex)
		{
			return m_ppPages[nIndex - 1];
		}

		Ref *GetPageRef(int nIndex)
		{
			return &m_pPageRefs[nIndex - 1];
		}

		StringExt *GetBaseURI()
		{
			return m_seBaseURI;
		}

		StringExt *ReadMetadata();

		Object *GetStructTreeRoot()
		{
			return &m_oStructTreeRoot;
		}

		// Ищем номер страницы в списке, по ее объектным номерам.
		int FindPage(int nNum, int nGen);

		// Ищем Named destination.
		LinkDestination *FindDest(StringExt *seName);

		Object *GetDests()
		{
			return &m_oDests;
		}

		Object *GetNameTree()
		{
			return &m_oNameTree;
		}

		Object *GetOutline()
		{
			return &m_oOutline;
		}

		Object *GetAcroForm()
		{
			return &m_oAcroForm;
		}

	private:

		int ReadPageTree(Dict *pPages, PageAttrs *pAttrs, int nStart, char *sAlreadyRead);
		Object *FindDestInTree(Object *pTree, StringExt *seName, Object *pObject);

	private:

		XRef      *m_pXref;           // Таблица xref для данного PDF файла
		Page     **m_ppPages;         // Массив страниц
		Ref       *m_pPageRefs;       // Объектные номера всех страниц
		int        m_nPagesCount;     // Число страниц
		int        m_nPagesSize;      // Размер массива страниц(в байтах)
		Object     m_oDests;          // Destination dictionary
		Object     m_oNameTree;       // Name tree
		StringExt *m_seBaseURI;       // Для ссылок URI
		Object     m_oMetadata;       // Metadata
		Object     m_oStructTreeRoot; // Структура страниц
		Object     m_oOutline;        // Outline
		Object     m_oAcroForm;       // AcroForm

		bool       m_bValid;          // True, если Сatalog - корретный

		GlobalParams *m_pGlobalParams;
	};
}

#endif // _PDF_READER_CATALOG_H
