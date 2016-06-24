/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _PDF_READER_ARRAY_H
#define _PDF_READER_ARRAY_H

#include "Object.h"

namespace PdfReader
{
	class Object;
	class XRef;

	//------------------------------------------------------------------------
	// Array
	//------------------------------------------------------------------------

	class Array
	{
	public:

		Array(XRef *pXRef);

		~Array();

		int AddRef()
		{
			return ++m_nRef;
		}
		int Release()
		{
			return --m_nRef;
		}


		int GetCount()
		{
			return m_nCount;
		}


		void Add(Object *pItem);

		Object *Get(int nIndex, Object *pObject);
		Object *GetCopy(int nIndex, Object *pObject);

	private:

		XRef   *m_pXRef;     // Таблица Xref для данного PDF документа
		Object *m_arrItems;  // Массив элементов
		int     m_nItemSize; // Размер элемента
		int     m_nCount;    // Чилсо элементов в массиве

		int     m_nRef;      // Счетчик ссылок
	};
}

#endif // _PDF_READER_ARRAY_H
