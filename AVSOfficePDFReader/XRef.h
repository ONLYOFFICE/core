#ifndef _XREF_H
#define _XREF_H

#include "Object.h"

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

	BOOL CheckValidate() 
	{ 
		return m_bValidXref; 
	}

	// Если CheckValidate вернул FALSE
	int GetErrorCode() 
	{ 
		return m_nErrorCode; 
	}

	void SetEncryption(int nPermissionFlags, BOOL bOwnerPassword, unsigned char *sDecryptKey, int nKeyLength, int nEncryptVersion, CryptAlgorithm eEncryptAlgorithm);

	BOOL CheckEncrypted() 
	{ 
		return m_bEncrypted; 
	}

	// Проверяем ограничения.
	BOOL CheckPrint(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckChange(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckCopy(BOOL bIgnoreOwnerPassword = FALSE);
	BOOL CheckAddNotes(BOOL bIgnoreOwnerPassword = FALSE);

	Object *GetCatalog(Object *pObject) 
	{ 
		return Fetch( m_nRootNum, m_nRootGen, pObject); 
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
	// Возвращаем FALSE, если позиция неизвестна или файл не поврежден.
	BOOL GetStreamEnd(unsigned int nStreamStart, unsigned int *pnStreamEnd);

	int GetSize() 
	{ 
		return m_nEntrySize; 
	}
	XRefEntry *GetEntry(int nIndex) 
	{ 
		return &m_arrEntries[ nIndex ]; 
	}
	Object *GetTrailerDict() 
	{ 
		return &m_oTrailerDict; 
	}

private:

	unsigned int GetStartXref();
	BOOL ReadXRef(unsigned int *punPos);
	BOOL ReadXRefTable(Parser *pParser, unsigned int *punPos);
	BOOL ReadXRefStreamSection(Stream *pXrefStream, int *arrW, int nFirst, int nCount);
	BOOL ReadXRefStream(Stream *pXrefStream, unsigned int *punPos);
	BOOL ConstructXRef();
	unsigned int StrintToUInt(char *sString);

private:

	BaseStream    *m_pStream;           // Основной поток
	unsigned int   m_nStart;            // Сдвиг в потоке 

	XRefEntry     *m_arrEntries;        // Элементы таблицы Xref
	int            m_nEntrySize;        // Размер элемента в списке m_arrEntries
	int            m_nRootNum;          // Номер объекта Root (Catalog)
	int            m_nRootGen;          // Номер версии объекта Root (Catalog)

	BOOL           m_bValidXref;        // Проверяем корректность таблицы Xref
	int            m_nErrorCode;        // Номер ошибки, если m_bValidXref = FALSE

	Object         m_oTrailerDict;      // Словарь Trailer

	unsigned int   m_unLastXRefOffset;  // Сдвиг последней таблицы Xref
	unsigned int  *m_punStreamEnds;     // Позиция конца потока - используется только для поврежденных файлов

	int            m_nStreamEndsCount;  // Количество корректных элементов в m_punStreamEnds
	ObjectStream  *m_pObjectStream;     // Object Stream
	BOOL           m_bEncrypted;        // Поток зашифрован или нет?
	int            m_nPermissionFlags;  // Различные ограничения
	BOOL           m_bOwnerPassword;    // Проверяем правильный ли был введен пароль владельца файла
	unsigned char  m_arrDecryptKey[16]; // Ключ для расшифровки
	int            m_nKeyLength;        // Размер ключа в байтах
	int            m_nEncryptVersion;   // Версия шифровки
	CryptAlgorithm m_eEncryptAlgorithm;	// Алгоритм шифрования

	CRITICAL_SECTION m_oCS;
};

#endif /* _XREF_H */
