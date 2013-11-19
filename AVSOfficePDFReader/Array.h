#ifndef _ARRAY_H
#define _ARRAY_H

#include "Object.h"

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

#endif /* _ARRAY_H */
