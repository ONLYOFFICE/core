#ifndef _PDF_READER_CHARCODE_TO_UNICODE_H
#define _PDF_READER_CHARCODE_TO_UNICODE_H

#include "CharTypes.h"
#include "GlobalParams.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace PdfReader
{
	struct CharCodeToUnicodeString;

	//-------------------------------------------------------------------------------------------------------------------------------

	class CharCodeToUnicode
	{
	public:

		static CharCodeToUnicode *ParseCIDToUnicode(StringExt *sFileName, StringExt *seCollection);

		static CharCodeToUnicode *ParseUnicodeToUnicode(StringExt *sFileName);

		static CharCodeToUnicode *Make8BitToUnicode(Unicode *pToUnicode);

		static CharCodeToUnicode *ParseCMap(StringExt *sBuffer, int nBitCount, GlobalParams *pGlobalParams);

		void MergeCMap(StringExt *sBuffer, int nBitCount, GlobalParams *pGlobalParams);

		~CharCodeToUnicode();

		// —читаем ссылки
		void AddRef();
		void Release();

		// —равниваем по данному тэгу
		bool Match(StringExt *seTag);

		void SetMapping(CharCode nCode, Unicode *pUnicode, int nLen);

		int  MapToUnicode(CharCode nCode, Unicode *pUnicode, int nSize);

		CharCode GetLength()
		{
			return m_nMapLen;
		}

	private:

		void ParseCMap1(int(*GetCharFunc)(void *), void *pData, int nBitCount, GlobalParams *pGlobalParams);
		void AddMapping(CharCode nCode, char *sUnicodeString, int nLen, int nOffset);
		CharCodeToUnicode(StringExt *sTag);
		CharCodeToUnicode(StringExt *sTag, Unicode *pMap, CharCode nMapLen, bool bCopyMap, CharCodeToUnicodeString *pSMap, int nSMapLen, int nSMapSize);

	private:

		StringExt              *m_seTag;
		Unicode                *m_pMap;
		CharCode                m_nMapLen;
		CharCodeToUnicodeString*m_pSMap;
		int                     m_nSMapLen;
		int                     m_nSMapSize;
								
		int                     m_nRef;

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};

	//-------------------------------------------------------------------------------------------------------------------------------

	class CharCodeToUnicodeCache
	{
	public:

		CharCodeToUnicodeCache(int nSize);
		~CharCodeToUnicodeCache();

		CharCodeToUnicode *GetCharCodeToUnicode(StringExt *seTag);

		void Add(CharCodeToUnicode *pCharCodeToUnicode);

	private:

		CharCodeToUnicode **m_ppCache;
		int                 m_nSize;
	};
}

#endif // _PDF_READER_CHARCODE_TO_UNICODE_H
