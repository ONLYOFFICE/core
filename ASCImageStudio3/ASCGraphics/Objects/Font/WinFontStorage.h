#pragma once

#include "../../../../../Common/RSA/XmlDecoder.h"
#include "../../../../../Common/ShareMemArray.h"

// Идентификаторы хранилищ
#define ISID_DEFAULT                 0x0000 // По умолчанию

#define ISID_WIN_FONT_STATUS_STORAGE 0x0001 // Таблица для статуса
#define ISID_WIN_FONT_INFO_STORAGE   0x0002 // Таблица для Информации

// Макс. длина имени таблицы
#define STORAGE_TABLE_NAME_LEN	1024

#define STATUS_STORAGE_NAME CString( _T("AVS_LOADED_WIN_FONTS") )

namespace NSStrings
{
	static CString ConvertCStringWToCString(CStringW& strW)
	{
		BSTR bstr = strW.AllocSysString();
		CString str(bstr);
		SysFreeString(bstr);

		return str;
	}

	static CStringW GetCStringWFromUTF8( BYTE* pBuffer, LONG64 lCount )
	{
		LONG64 lLenght = 0;
		CStringW strRes = L"";
		for (LONG lIndex = 0; lIndex < lCount; ++lIndex)
		{
			if (0x00 == (0x80 & pBuffer[lIndex]))
			{
				strRes += (WCHAR)pBuffer[lIndex];
				continue;
			}
			else if (0x00 == (0x20 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x1F) << 6) + (pBuffer[lIndex + 1] & 0x3F));
				strRes += mem;

				lIndex += 1;
			}
			else if (0x00 == (0x10 & pBuffer[lIndex]))
			{
				WCHAR mem = (WCHAR)(((pBuffer[lIndex] & 0x0F) << 12) + ((pBuffer[lIndex + 1] & 0x3F) << 6) + (pBuffer[lIndex + 2] & 0x3F));
				strRes += mem;

				lIndex += 2;
			}
		}
		return strRes;
	}
	
	static CString GetCStringFromUTF8( BYTE* pBuffer, LONG64 lCount )
	{
		CStringW strRes = GetCStringWFromUTF8(pBuffer, lCount);
		return ConvertCStringWToCString(strRes);
	}
};





// статус таблицы установленных шрифтов
enum WinFontsTableStatus {STIF_ERROR, STIF_AVAILABLE, STIF_CREATING, STIF_BROKEN};

enum StringCoding 
{	
	scASCII		= 0, 
	scUNICODE	= 1, 
	scUTF8		= 2
};


// Структура для определения статуса таблицы хранилища
struct WinFontsStatusStorage
{
	// Общие данные
	WinFontsTableStatus	m_sStatus;					// Статус
	LONG64              m_lLength;                  // Размер таблицы

	WinFontsStatusStorage() 
	{
		Reset();
	}

	void Reset()
	{
		// Общие данные
		m_sStatus = STIF_ERROR; // Статус
		m_lLength = 0;
	}
};

// Структура для определения информации
struct WinFontsInfoStorage
{
	LONG		m_lCount;
	
	CStringA    m_strXml;
	
	BYTE*		m_pBuffer;
	LONG		m_lCountData;

	WinFontsInfoStorage() 
	{
		Reset();
	}

	~WinFontsInfoStorage()
	{
	}

	void Reset()
	{
		m_lCount			= 0;

		m_strXml			= "";
		m_pBuffer			= NULL;

	}

	void GenerateInfo(BSTR bstrXML)
	{
		m_strXml			= CStringA(bstrXML);

		m_pBuffer			= NULL;
		m_lCountData		= 0;
	}
	void GenerateInfo(BYTE* pBuffer, LONG lCountData)
	{
		m_pBuffer		= pBuffer;
		m_lCountData	= lCountData;
	}

	void Save(BYTE* pBuffer)
	{
		if (NULL == pBuffer)
			return;

		if (0 == m_lCountData)
		{
			memcpy(pBuffer, (BYTE*)m_strXml.GetBuffer(), m_strXml.GetLength());
		}
		else
		{
			memcpy(pBuffer, m_pBuffer, m_lCountData);
		}
	}

	LONG64 GetLength()
	{
		if (0 == m_lCountData)
		{
			return m_strXml.GetLength();
		}
		return m_lCountData;
	}

	void Load(BYTE* pBuffer, LONG64 lLength)
	{
		//m_pBuffer		= pBuffer;
		m_lCountData	= (LONG)lLength;

		m_pBuffer = new BYTE[m_lCountData];
		memcpy(m_pBuffer, pBuffer, m_lCountData);
	}
};

// Класс хранилища таблицы статуса
class CWinFontsStatusStorage: public CShareMemArray<BYTE>
{
protected:

	bool ReadStruct_unsync(WinFontsStatusStorage *aData)		// Прочитать структуру unsync
	{
		__try
		{
			WinFontsStatusStorage *pTable = (WinFontsStatusStorage *) ( (BYTE *)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - размер таблицы
			memcpy(aData, pTable, sizeof(WinFontsStatusStorage)); 
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			//ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
			return false;
		}
		return true;
	}

	bool WriteStruct_unsync(WinFontsStatusStorage *aData)	// Записать структуру unsync
	{
		__try
		{
			WinFontsStatusStorage *pTable = (WinFontsStatusStorage *) ( (BYTE *)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - размер таблицы
			memcpy(pTable, aData, sizeof(WinFontsStatusStorage)); 
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			//ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
			return false;
		}
		return true;
	}

public:

	bool ReadStruct(WinFontsStatusStorage *aData)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return ReadStruct_unsync(aData);
	}
	bool WriteStruct(WinFontsStatusStorage *aData)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return WriteStruct_unsync(aData);
	}

public:

	// Получить статус индексатора (bForceControl - Попытаться стать мастером, если получится)
	// возвращаем *bForceControl == true - вызывающий объект стал мастером
	bool GetStatus(bool *bForceControl, WinFontsStatusStorage* aIS)
	{
		bool bTryToForceControl = bForceControl && (*bForceControl);

		if (NULL == aIS) return false;

		if (bForceControl) *bForceControl = false;	// Пока мы не можем стать мастером

		// Делаем все под мутексом :)
		CSynchAccess oAccess = m_hAccessMutex;
		
		if (!ReadStruct_unsync(aIS)) return false;	// Не получилось прочитать статус

		if ((STIF_BROKEN == aIS->m_sStatus) || (STIF_ERROR == aIS->m_sStatus))
		{
			// Таблица нарушена - говорим, что можно стать мастером (если нужно было)
			if (bTryToForceControl)
			{
				// Сбрасываем таблицу
				aIS->Reset();
				aIS->m_sStatus = STIF_CREATING;
				*bForceControl = WriteStruct_unsync(aIS);	// пишем в память и говорим, что стали мастером
			}
		}

		return true;
	}
	// Установить статус, как нарушенное хранилище. Мастерство может перехватываться другими объектами
	bool SetBroken()
	{
		CSynchAccess oAccess = m_hAccessMutex;
		WinFontsStatusStorage sStor;
		sStor.m_sStatus = STIF_BROKEN;
		// Пишем в ш-память
		return WriteStruct_unsync(&sStor);
	}

private:
	// Скрываем методы работы с массивами
	bool Save(CAtlArray<BYTE> &aTable)
	{
		return false;
	}

	bool Load(CAtlArray<BYTE> &aTable)
	{
		return false;
	}


public:
	CWinFontsStatusStorage(CString &aName): CShareMemArray(aName, sizeof(WinFontsStatusStorage), ISID_WIN_FONT_STATUS_STORAGE)
	{
		// Параметр aName должен быть AVS_LOAD_WIN_FONTS
		//ATLTRACE2("CStatusStorage created;\n");
	};	
	~CWinFontsStatusStorage()
	{
		//ATLTRACE2("CStatusStorage destroyed;\n");
	};
};

// Класс хранилища таблицы информации
class CWinFontsInfoStorage: public CShareMemArray<BYTE>
{
protected:

	bool ReadStruct_unsync(WinFontsInfoStorage *aData)		// Прочитать структуру unsync
	{
		__try
		{
			LONG64 lLength = Size();
			aData->Load((BYTE*)m_pArray + sizeof(LONG64), lLength);	// sizeof(LONG64) - размер таблицы
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			//ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
			return false;
		}
		return true;
	}

	bool WriteStruct_unsync(WinFontsInfoStorage *aData)	// Записать структуру unsync
	{
		__try
		{
			LONG64 *pSize = (LONG64*)m_pArray;	
			*pSize = m_nSize;

			aData->Save((BYTE*)m_pArray + sizeof(LONG64));	// sizeof(LONG64) - размер таблицы
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			//ATLTRACE2("CStatusStorage::ReadStruct_unsync() fails!\n");
			return false;
		}
		return true;
	}

public:

	bool ReadStruct(WinFontsInfoStorage *aData)
	{
		//CSynchAccess oAccess = m_hAccessMutex;
		return ReadStruct_unsync(aData);
	}
	bool WriteStruct(WinFontsInfoStorage *aData)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return WriteStruct_unsync(aData);
	}
	bool WriteCount(WinFontsInfoStorage *aData)
	{
		__try
		{
			BYTE* pData = (BYTE*)m_pArray + sizeof(LONG64);
			*((LONG*)pData) = aData->m_lCount;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			//ATLTRACE2("CWinFontsInfoStorage::ReadStruct_unsync() fails!\n");
			return false;
		}
		return true;
	}

private:
	// Скрываем методы работы с массивами
	bool Save(CAtlArray<BYTE> &aTable)
	{
		return false;
	}

	bool Load(CAtlArray<BYTE> &aTable)
	{
		return false;
	}


public:
	CWinFontsInfoStorage(CString &aFileName, LONG64 lSize): CShareMemArray(aFileName, lSize, ISID_WIN_FONT_INFO_STORAGE)
	{
		//ATLTRACE2("CWinFontsInfoStorage created;\n");
	};	
	~CWinFontsInfoStorage()
	{
		//ATLTRACE2("CWinFontsInfoStorage destroyed;\n");
	};
};
