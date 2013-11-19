#ifndef _UNICODE_MAP_H
#define _UNICODE_MAP_H

#include "CharTypes.h"
#include "TemporaryCS.h"

class StringExt;

//-------------------------------------------------------------------------------------------------------------------------------

enum UnicodeMapType
{
	unicodeMapUser,      // Чтение из файла
	unicodeMapResident,  // Статический список
	unicodeMapFunc       // Указатель на функцию
};

typedef int (*UnicodeMapFunc)(Unicode nUnicode, char *sBuffer, int nBufferSize);

struct UnicodeMapRange 
{
	Unicode      nStart;      // Диапазон юникодных символов
	Unicode      nEnd;        // 
	unsigned int unCode;      // Первый код
	unsigned int nBytesCount; //
};

struct UnicodeMapExt;

//-------------------------------------------------------------------------------------------------------------------------------
// UnicodeMap
//-------------------------------------------------------------------------------------------------------------------------------

class UnicodeMap 
{
public:

	// Создаем UnicodeMap по имени <seEncodingName>. Устанавливаем счетчик ссылок равный 1.
	static UnicodeMap *Parse(StringExt *seEncodingName, GlobalParams *pGlobalParams);

	// Создаем Resident UnicodeMap.
	UnicodeMap(char *sEncodingName, BOOL bUnicodeOut, UnicodeMapRange *pRanges, int nLen);

	// Создаем Resident UnicodeMap, который использует функцию вместо диапазона значений.
	UnicodeMap(char *sEncodingName, BOOL bUnicodeOut, UnicodeMapFunc pFunction);

	~UnicodeMap();

	// Счетчик ссылок
	void AddRef();
	void Release();

	StringExt *GetEncodingName() 
	{ 
		return m_seEncodingName; 
	}

	BOOL IsUnicode() 
	{ 
		return m_bUnicodeOut; 
	}


	// Совпадают ли названия кодировок?
	BOOL Match(StringExt *seEncodingName);

	// Находим образ текущего юникодного значения в кодировке. Заполняем <sBuffer> результатом и возвращаем
	// количество используемых байт. Никаких строковых разделителей (типа \0) не пишем. 
	int MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize);

private:

	UnicodeMap(StringExt *seEncodingName);

private:

	StringExt     *m_seEncodingName;
	UnicodeMapType m_eType;
	BOOL           m_bUnicodeOut;
	union 
	{
		UnicodeMapRange *m_pRanges;	  // Для типов: User, Resident
		UnicodeMapFunc   m_pFunction; // Для типа Func
	};
	int            m_nLen;      // User, Resident
	UnicodeMapExt *m_pEMaps;    // User
	int            m_nEMapsLen; // User

	int            m_nRefCount;

	CRITICAL_SECTION m_oCS;

};

//-------------------------------------------------------------------------------------------------------------------------------
// UnicodeMapCache
//-------------------------------------------------------------------------------------------------------------------------------

#define unicodeMapCacheSize 4

class UnicodeMapCache 
{
public:

	UnicodeMapCache();
	~UnicodeMapCache();

	UnicodeMap *GetUnicodeMap(StringExt *seEncodingName, GlobalParams *pGlobalParams);

private:

	UnicodeMap *m_ppCache[unicodeMapCacheSize];
};

#endif /* _UNICODE_MAP_H */
