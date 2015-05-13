#ifndef _PDF_READER_UNICODE_MAP_H
#define _PDF_READER_UNICODE_MAP_H

#include "CharTypes.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
	class StringExt;

	//-------------------------------------------------------------------------------------------------------------------------------

	enum UnicodeMapType
	{
		unicodeMapUser,      // Чтение из файла
		unicodeMapResident,  // Статический список
		unicodeMapFunc       // Указатель на функцию
	};

	typedef int(*UnicodeMapFunc)(Unicode nUnicode, char *sBuffer, int nBufferSize);

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
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapRange *pRanges, int nLen);

		// Создаем Resident UnicodeMap, который использует функцию вместо диапазона значений.
		UnicodeMap(char *sEncodingName, bool bUnicodeOut, UnicodeMapFunc pFunction);

		~UnicodeMap();

		// Счетчик ссылок
		void AddRef();
		void Release();

		StringExt *GetEncodingName()
		{
			return m_seEncodingName;
		}

		bool IsUnicode()
		{
			return m_bUnicodeOut;
		}


		// Совпадают ли названия кодировок?
		bool Match(StringExt *seEncodingName);

		// Находим образ текущего юникодного значения в кодировке. Заполняем <sBuffer> результатом и возвращаем
		// количество используемых байт. Никаких строковых разделителей (типа \0) не пишем. 
		int MapUnicode(Unicode nUnicode, char *sBuffer, int nBufferSize);

	private:

		UnicodeMap(StringExt *seEncodingName);

	private:

		StringExt     *m_seEncodingName;
		UnicodeMapType m_eType;
		bool           m_bUnicodeOut;
		union
		{
			UnicodeMapRange *m_pRanges;	  // Для типов: User, Resident
			UnicodeMapFunc   m_pFunction; // Для типа Func
		};
		int            m_nLen;      // User, Resident
		UnicodeMapExt *m_pEMaps;    // User
		int            m_nEMapsLen; // User

		int            m_nRefCount;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
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
}

#endif // _PDF_READER_UNICODE_MAP_H
