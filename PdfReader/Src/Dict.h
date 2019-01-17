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
#ifndef _PDF_READER_DICT_H
#define _PDF_READER_DICT_H

#include "Object.h"

namespace PdfReader
{
	//------------------------------------------------------------------------
	// Dict
	//------------------------------------------------------------------------
	struct DictEntry
	{
		char  *sKey;
		Object oValue;
	};

	class Dict
	{
	public:

		Dict(XRef *pXref);

		~Dict();

		// Счетчик ссылок.
		int AddRef()
		{
			return ++m_nRef;
		}
		int Release()
		{
			return --m_nRef;
		}


		int GetEntryCount()
		{
			return m_nEntriesCount;
		}

		// Добавляем элемент в словарь. При этом имя не копируется.
		void AddItem(char *sKey, Object *pValue);

		// Проверяем тип обэекта по имени sType.
		bool CheckType(char *sType);

		// Ищем элемент словаря. Возвращается указатель на объект.
		Object *Search(char *sKey, Object *pObject);
		// Ищем элемент словаря. Возвращается копия объекта.
		Object *SearchAndCopy(char *sKey, Object *pObject);


		char   *GetKey(int nIndex);
		Object *GetValue(int nIndex, Object *pObj);
		Object *GetValueCopy(int nIndex, Object *pObj);

		// Устанавливаем указатель на таблицу Xref. Данная функция используется
		// только в одном случае: словарь Trailer читается до таблицы Xref, поэтому
		// для данного словаря этот указатель нужно устанавливать.
		void SetXRef(XRef *pXref)
		{
			m_pXref = pXref;
		}

	private:

		DictEntry *Find(char *sKey);

	private:

		XRef      *m_pXref;         // таблица Xref текущего PDF файла
		DictEntry *m_arrEntries;    // Соержимое словаря
		int        m_nEntriesSize;  // размер элемента в словаре
		int        m_nEntriesCount; // количество элементов в словаре

		int        m_nRef;          // Счетчик ссылок
	};
}

#endif // _PDF_READER_DICT_H
