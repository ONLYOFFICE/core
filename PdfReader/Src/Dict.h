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
