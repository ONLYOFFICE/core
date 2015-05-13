#ifndef _PDF_READER_XREF_H
#define _PDF_READER_XREF_H

#include "Object.h"
#include "Stream.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"
#include "ErrorConstants.h"

namespace PdfReader
{
	class Dict;
	class Stream;
	class Parser;
	class ObjectStream;

	//------------------------------------------------------------------------
	// XRef
	//------------------------------------------------------------------------

	enum XRefEntryType
	{
		xrefEntryFree,
		xrefEntryUncompressed,
		xrefEntryCompressed
	};

	struct XRefEntry
	{
		unsigned int  unOffset;
		int           nGen;
		XRefEntryType eType;
	};

	class XRef
	{
	public:

		XRef(BaseStream *pStream);

		~XRef();

		bool CheckValidate()
		{
			return m_bValidXref;
		}

		// Если CheckValidate вернул false
		EError GetErrorCode()
		{
			return m_eErrorCode;
		}

		void SetEncryption(int nPermissionFlags, bool bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm);

		bool CheckEncrypted()
		{
			return m_bEncrypted;
		}

		// Проверяем ограничения.
		bool CheckPrint(bool bIgnoreOwnerPassword = false);
		bool CheckChange(bool bIgnoreOwnerPassword = false);
		bool CheckCopy(bool bIgnoreOwnerPassword = false);
		bool CheckAddNotes(bool bIgnoreOwnerPassword = false);

		Object *GetCatalog(Object *pObject)
		{
			return Fetch(m_nRootNum, m_nRootGen, pObject);
		}

		// Вытаскиваем косвенный объект.
		Object *Fetch(int nNum, int nGen, Object *pObject);

		Object *GetDocInfo(Object *pObject);
		Object *GetDocInfoCopy(Object *pObject);

		int GetObjectsCount()
		{
			return m_nEntrySize;
		}

		unsigned int GetLastXRefPos()
		{
			return m_unLastXRefOffset;
		}


		// объект Root (Catalog)
		int GetRootNum()
		{
			return m_nRootNum;
		}
		int GetRootGen()
		{
			return m_nRootGen;
		}

		// Получаем конечную позицию в поврежденном файле.
		// Возвращаем false, если позиция неизвестна или файл не поврежден.
		bool GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd);

		int GetSize()
		{
			return m_nEntrySize;
		}
		XRefEntry *GetEntry(int nIndex)
		{
			return &m_arrEntries[nIndex];
		}
		Object *GetTrailerDict()
		{
			return &m_oTrailerDict;
		}

	private:

		unsigned int GetStartXref();
		bool ReadXRef(unsigned int *punPos);
		bool ReadXRefTable(Parser *pParser, unsigned int *punPos);
		bool ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount);
		bool ReadXRefStream(Stream *pXrefStream, unsigned int *punPos);
		bool ConstructXRef();
		unsigned int StrintToUInt(char *sString);

	private:

		BaseStream    *m_pStream;           // Основной поток
		unsigned int   m_nStart;            // Сдвиг в потоке 

		XRefEntry     *m_arrEntries;        // Элементы таблицы Xref
		int            m_nEntrySize;        // Размер элемента в списке m_arrEntries
		int            m_nRootNum;          // Номер объекта Root (Catalog)
		int            m_nRootGen;          // Номер версии объекта Root (Catalog)

		bool           m_bValidXref;        // Проверяем корректность таблицы Xref
		EError         m_eErrorCode;        // Номер ошибки, если m_bValidXref = false

		Object         m_oTrailerDict;      // Словарь Trailer

		unsigned int   m_unLastXRefOffset;  // Сдвиг последней таблицы Xref
		unsigned int  *m_punStreamEnds;     // Позиция конца потока - используется только для поврежденных файлов

		int            m_nStreamEndsCount;  // Количество корректных элементов в m_punStreamEnds
		ObjectStream  *m_pObjectStream;     // Object Stream
		bool           m_bEncrypted;        // Поток зашифрован или нет?
		int            m_nPermissionFlags;  // Различные ограничения
		bool           m_bOwnerPassword;    // Проверяем правильный ли был введен пароль владельца файла
		unsigned char  m_arrDecryptKey[16]; // Ключ для расшифровки
		int            m_nKeyLength;        // Размер ключа в байтах
		int            m_nEncryptVersion;   // Версия шифровки
		CryptAlgorithm m_eEncryptAlgorithm;	// Алгоритм шифрования

		NSCriticalSection::CRITICAL_SECTION m_oCS;
	};
}

#endif // _PDF_READER_XREF_H
