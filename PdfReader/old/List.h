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
#ifndef _PDF_READER_LIST_H
#define _PDF_READER_LIST_H

namespace PdfReader
{
	//------------------------------------------------------------------------
	// CList
	//------------------------------------------------------------------------

	class CList
	{

	public:

		// Создаем пустой список.
		CList();

		// Создаем пустой список с с выделенной памятью под <nSize> элементов.
		CList(int nSize);

		// Деструктор.
		~CList();

		int GetLength()
		{
			return m_nCount;
		}

		// Возвращает <nIndex>Ый элемент.
		// Если 0 <= nIndex < m_nCount, возвращает NULL.
		void *GetByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nCount)
				return NULL;
			return m_ppData[nIndex];
		}

		// Добавляем элемент в окнец списка.
		void Append(void *pItem);

		// Добавляем другой список в конец данного.
		void Append(CList *pList);

		// Вставляем элемент на место <nIndex>.
		// Если !(0 <= nIndex <= m_nCount), ничего не делаем.
		void Insert(int nIndex, void *pItem);

		// Удаляем из списка и возвращаем ссылку на элемент.
		// Если !(0 <= nIndex <= m_nCount), ничего не делаем.
		void *Delete(int nIndex);

		// Сортируем список, в соответствии с данной функцией
		// сранвения.
		void Sort(int(*CompareFunc)(const void *pItem1, const void *pItem2));

		// Если m_nIncreament > 0, тогда при расширении списка ровно
		// m_nIncreament элементов будет добавляться. Если m_nIncreament <= 0,
		// тогда список будем удваивать при расширении.
		void SetAllocationIncreament(int nIncreament)
		{
			m_nIncreament = nIncreament;
		}

	private:

		void Expand();
		void Shrink();

	private:

		void **m_ppData;      // список элементов
		int    m_nItemSize;   // размер данных в массиве
		int    m_nCount;      // количестов элементов в списке
		int    m_nIncreament; // на сколько будем увеличивать список
	};

#define DeleteCList(list, T)                        \
  do {                                              \
    CList *_list = (list);                          \
	{                                               \
      int _i;                                       \
      for (_i = 0; _i < _list->GetLength(); ++_i) { \
        delete (T*)_list->GetByIndex(_i);           \
      }                                             \
      delete _list;                                 \
    }                                               \
  } while (0)

}
#endif // _PDF_READER_LIST_H
