/***************************************************************

	CShareMemArray 

		шаблон класса для работы с именованной 
		Shared-памятью. Реализаваны загрузка массива
		из памяти и сохранение в память.

***************************************************************/

#pragma once
#pragma warning( disable : 4996 4244)	// No CRT-secure warning

#include <atlcoll.h>
#include "AVSUtils.h"	// CSyncAccess class definition

#define AVS_USER_NAME_LEN 1024

// Статус хранилища (ошибка, уже создано, новое)
enum TSMAStatus {SMAS_ERROR, SMAS_ALREADYEXISTS, SMAS_NEW};

// Хранилище
template <typename STOR_TYPE>
class CShareMemArray
{
protected:
	HANDLE		m_hAccessMutex;		// Мьютекс для безопасного доступа к Shared-Memory
	HANDLE		m_hMapFile;			// Хендл на map таблицы
	STOR_TYPE	*m_pArray;			// Указатель на массив
	LONG64		m_nSize;			// Размер таблицы
	CString		m_sMutexName;		// Имя мьютекса
	CString		m_sMapName;			// Имя маппа
	TSMAStatus	m_sStatus;

protected:
	// Читаем из памяти
	bool ReadFromSharedMem(LONG64 nIndex, STOR_TYPE &nValue)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return false;
		}

		__try
		{
			STOR_TYPE *pTable = (STOR_TYPE *) (((BYTE *) m_pArray) + sizeof(LONG64));	// sizeof(LONG64) - размер таблицы
			nValue = pTable[nIndex];
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("CIndexerStorage::ReadFromSharedMem()\n");
			return false;
		}
		return true;
	}

	// Пишем в память
	bool WriteToSharedMem(LONG64 nIndex, STOR_TYPE aValue)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return false;
		}

		__try
		{
			STOR_TYPE *pTable = (STOR_TYPE *) (((BYTE *) m_pArray) + sizeof(LONG64));	// sizeof(LONG64) - размер таблицы
			pTable[nIndex] = aValue;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("Error CIndexerStorage::WriteToSharedMem(#i)\n", nIndex);
			return false;
		}
		return true;
	}

	// Загрузка таблицы индексов из SharedMemory
	bool SaveTable_unsync(CAtlArray<STOR_TYPE> &aTable)
	{
		if ((NULL == m_pArray) || (NULL == m_hMapFile)) 
		{
			m_sStatus = SMAS_ERROR;
			return false;	// Защита от дурака
		}
	
		bool bRes = true;

		// Защита от дурака
		LONG64 nCopyCount = (m_nSize <= (LONG64) aTable.GetCount()) ? m_nSize : aTable.GetCount();
		
		// Сохраняем размер таблицы
		Size_unsync(m_nSize);

		// копируем из памяти в массив (safe)
		for (LONG64 nIndex = 0; nIndex < nCopyCount; nIndex++)
		{
			bRes &= WriteToSharedMem (nIndex, aTable[nIndex]);
		}

		return bRes;
	}

	// Сохранение таблицы индексов из SharedMemory
	bool LoadTable_unsync(CAtlArray<STOR_TYPE> &aTable)
	{
		if ((NULL == m_pArray) || (NULL == m_hMapFile)) 
		{
			m_sStatus = SMAS_ERROR;
			return false;	// Защита от дурака
		}
	
		aTable.RemoveAll();
		
		// Определяем размер
		m_nSize = Size_unsync();

		STOR_TYPE nValue;

		// копируем из памяти в массив
		for (DWORD nIndex = 0; nIndex < m_nSize; nIndex++)
		{	
			if (ReadFromSharedMem(nIndex, nValue))
			{
				// Добавляем в таблицу
				aTable.Add(nValue);
			}
		}
		return true;
	}

	// Размер
	LONG64 Size_unsync()
	{
		LONG64 nValue = -1;

		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return nValue;
		}

		__try
		{
			LONG64 *pSize = (LONG64 *) m_pArray;
			nValue = *pSize;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("CIndexerStorage::Size_unsync()\n");
			return -1;
		}
		return nValue;
	}

	void Size_unsync(LONG64 aSize)
	{
		if (NULL == m_pArray)
		{
			m_sStatus = SMAS_ERROR;
			return;
		}

		__try
		{
			LONG64 *pSize = (LONG64*) m_pArray;	// sizeof(LONG64) - размер таблицы
			*pSize = aSize;
		}
		__except(EXCEPTION_IN_PAGE_ERROR == GetExceptionCode() ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
		{
			// Failed to read from the view.
			ATLTRACE2("Error CIndexerStorage::Size_unsync(LONG64 aSize)\n");
		}
	}

public:
	// aSize - размер таблицы индексации (кол-во полусекунд), 
	// Id - дополнительный идектификатор, чтоб можно было создавать много сторейджей для одного файла
	CShareMemArray(CString &aFileName, LONG64 aSize, DWORD aId = ISID_DEFAULT):
	  m_hMapFile(NULL), m_nSize(aSize), m_pArray(NULL), m_sStatus(SMAS_ERROR)
	{
		// "Вытаскиваем" имя файла
		TCHAR aDrive[_MAX_DRIVE];
		TCHAR aDir[_MAX_DIR];
		TCHAR aFName[_MAX_FNAME];
		TCHAR aExt[_MAX_EXT];

		_tsplitpath (aFileName.GetBuffer(), aDrive, aDir, aFName, aExt);
		//_wsplitpath_s (aFileName.GetBuffer(), aDrive, _MAX_DRIVE, aDir, _MAX_DIR, aFName, _MAX_FNAME, aExt, _MAX_EXT);

		// Дополнительно формируем уникальные символы для этого пути, чтобы включить в имя файла
		DWORD dwPathID = 0;
		TCHAR tcPathIDItem = 0;

		// Подсчитываем контрольную сумму для пути
		for (int i = 0; i < (int) _tcslen(aDir); i++) 
		{
			tcPathIDItem ^= aDir[i];
			dwPathID ^= dwPathID << 1;
			dwPathID += (DWORD) tcPathIDItem;
		}

		// Подсчитываем контрольную сумму для расширения
		DWORD dwExtID = 0;
		TCHAR tcExtIDItem = 0;		
		for (int i = 0; i < (int) _tcslen(aExt); i++) 
		{
			tcExtIDItem ^= aExt[i];
			 dwExtID ^=  dwExtID << 1;
			 dwExtID += (DWORD) tcExtIDItem;
		}
		
		// Формируем имя мутекса и мапа
		// Этот код не работает если зашло два пользователя и каждый запускает свою копию приложения.
		// Фича в именовании мьютекса.
		// В начале имени мьютекса должно стоять "Global\"
		// ШульгаИван: "Global\" не проходит на Win7, если прога запущена не под админом
		m_sMutexName.Format(_T("Local\\avs_mutex%u_%s_%06x_%06I64x_%06x"), aId, aFName, dwPathID, aSize, dwExtID);
		m_sMapName.Format(_T("Local\\avs_storage%u_%s_%06x_%06I64x_%06x"), aId, aFName, dwPathID, aSize, dwExtID);

		// добавляем имя юзера
		TCHAR pBufferUserName[AVS_USER_NAME_LEN];
		DWORD dwBufferUserNameLen = AVS_USER_NAME_LEN;
		GetUserName(pBufferUserName, &dwBufferUserNameLen);
		
		CString strUserName(pBufferUserName, dwBufferUserNameLen);
		m_sMutexName	+= strUserName;
		m_sMapName		+= strUserName;

		// Создаем мьютекс
		m_hAccessMutex = CreateMutex(NULL, FALSE, m_sMutexName.GetBuffer());

		// Далее все делаем "под мутексом" :)
		CSynchAccess oAccess = m_hAccessMutex;
		
		// Создаем мап
		ATLTRACE2("CShareMemArray()::CShareMemArray(): m_nSize = %d\n", m_nSize);

		ULARGE_INTEGER nMappingSize;
		nMappingSize.QuadPart = m_nSize * sizeof(STOR_TYPE) + sizeof(LONG64);

		m_hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, nMappingSize.HighPart, nMappingSize.LowPart, m_sMapName.GetBuffer());
		if (NULL == m_hMapFile)
		{
			// Ошибка
			ATLTRACE2("CShareMemArray::CShareMemArray():CreateFileMapping() FAILS (0x%x)!\n", GetLastError());
			m_sStatus = SMAS_ERROR;
		}
		else 
		{
			// Все ок!
			m_sStatus = (GetLastError() == ERROR_ALREADY_EXISTS) ? SMAS_ALREADYEXISTS : SMAS_NEW;
			ATLTRACE2 (SMAS_ALREADYEXISTS == m_sStatus ? "CShareMemArray: open existing!\n" : "CShareMemArray: create new!\n");

			// создаем View of file
			m_pArray = (STOR_TYPE *) MapViewOfFile(m_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, (SIZE_T) nMappingSize.QuadPart);
			if (NULL == m_pArray)
			{
				// Ошибка
				ATLTRACE2("CShareMemArray::CShareMemArray():MapViewOfFile() FAILS (0x%x)!\n", GetLastError());
				m_sStatus = SMAS_ERROR;
			}
		}
	}
	
	virtual ~CShareMemArray()
	{
		if (m_pArray) UnmapViewOfFile(m_pArray);			// удаляем view
		if (NULL != m_hMapFile) CloseHandle(m_hMapFile);	// удаляем мап
		if (NULL != m_hAccessMutex) CloseHandle(m_hAccessMutex);	// Удаляем мьютекс
	}

public:
	
	// Сохранить таблицу в шаред-мемори
	bool Save(CAtlArray<STOR_TYPE> &aTable)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return SaveTable_unsync(aTable);
	}

	// Загрузить таблицу
	bool Load(CAtlArray<STOR_TYPE> &aTable)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return LoadTable_unsync(aTable);
	}

	// Размер получение
	LONG64 Size()
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return Size_unsync();
	}

	/*void Size(LONG64 aSize)
	{
		CSynchAccess oAccess = m_hAccessMutex;
		Size_unsync(aSize);
	}*/

	// Получение статуса
	TSMAStatus Status(void) const 
	{
		CSynchAccess oAccess = m_hAccessMutex;
		return m_sStatus;
	}
};

