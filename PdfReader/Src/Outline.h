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
#ifndef _PDF_READER_OUTLINE_H
#define _PDF_READER_OUTLINE_H

#include "Object.h"
#include "CharTypes.h"

namespace PdfReader
{
	class StringExt;
	class GList;
	class XRef;
	class LinkAction;

	//-------------------------------------------------------------------------------------------------------------------------------
	// Outline
	//-------------------------------------------------------------------------------------------------------------------------------

	class Outline
	{
	public:

		Outline(Object *pOutlineObject, XRef *pXref);
		~Outline();

		CList *GetItems()
		{
			return m_pItems;
		}

	private:

		CList *m_pItems; // NULL, если у документа нет Outline
		// [OutlineItem]
	};

	//-------------------------------------------------------------------------------------------------------------------------------
	// OutlineItem
	//-------------------------------------------------------------------------------------------------------------------------------

	class OutlineItem
	{
	public:

		OutlineItem(Dict *pDict, XRef *pXref);
		~OutlineItem();

		static CList *ReadItemList(Object *pFirstItemRef, Object *pLastItemRef, XRef *pXref);

		void Open();
		void Close();

		Unicode *GetTitle()
		{
			return m_pTitle;
		}
		int GetTitleLength()
		{
			return m_nTitleLen;
		}
		LinkAction *GetAction()
		{
			return m_pAction;
		}
		bool IsOpen()
		{
			return m_bOpen;
		}
		bool HasKids()
		{
			return m_oFirstRef.IsRef();
		}
		CList *GetKids()
		{
			return m_pKids;
		}

	private:

		XRef       *m_pXref;          // Таблица Xref данного PDF-документа
		Unicode    *m_pTitle;         // Название данной закладки
		int         m_nTitleLen;      // Длина названия
		LinkAction *m_pAction;        // LinkAction

		Object      m_oFirstRef;      // Ссылка(Ref) на First
		Object      m_oLastRef;       // Ссылка(Ref) на Last
		Object      m_oNextRef;       // Ссылка(Ref) на Next

		bool        m_bOpen;          // Будет ли вкладка открытой при открытии документа
		CList      *m_pKids;          // NULL, если данный пункт не открыт [OutlineItem]
	};
}

#endif // _PDF_READER_OUTLINE_H
