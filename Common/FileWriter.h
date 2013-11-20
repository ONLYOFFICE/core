#pragma once

namespace NFileWriter
{
	//------------------------------------------------------------------------------------------------------
	// CBufferedFileWriter
	//------------------------------------------------------------------------------------------------------
	// Абстрактный класс, который содержит в себе объявления функций
	//------------------------------------------------------------------------------------------------------

	class CFileWriter
	{
	public :

		// Принимает данные на запись
		virtual VOID Write ( LPBYTE lpData, LONG64 lDataLength ) = 0;
		// Флаш ( дописываем все данные )
		virtual VOID Flush () = 0;
		// Сик на указанную позицию по файлу
		virtual VOID Seek ( LONG64 lPosition, DWORD dwFrom = FILE_CURRENT ) = 0;
		// Текущая позиция 
		virtual VOID GetPosition(ULONGLONG& nPos) = 0;
		// Размер записанного файла
		virtual VOID GetSize(ULONGLONG& nLen) = 0;

	public :

		// Конструктор
		CFileWriter ()
		{
		}
		// Виртуальный деструктор
		virtual ~CFileWriter ()
		{
		}
	};

	//------------------------------------------------------------------------------------------------------
	// CBufferedFileWriter
	//------------------------------------------------------------------------------------------------------
	// Класс, который занимается записью файла на диск
	//------------------------------------------------------------------------------------------------------

	class CBufferedFileWriter : public CFileWriter
	{
	private :

		HANDLE m_hFile;									// Хендл на файл, в который идет запись

		LONG64 m_lBufferSize;							// Размер буффера

		LPBYTE m_lpBuffer;								// Сам буффер

		LONG64 m_lWritePointer;							// Позиция для записи данных в буффер

	public :

		// Конструктор ( от имени файла, сам контрол открывает файл )
		CBufferedFileWriter ( TCHAR *lpszFile, LONG64 lBufferSize = 10 * 1024 * 1024 ) : CFileWriter ()
		{
			// Создаем хендл на файл, в который будет осуществляться запись, и инициализируем хендл, который следует закрыть
			m_hFile = CreateFile (lpszFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

			// Если не получилось открыть файл
			if ( INVALID_HANDLE_VALUE == m_hFile )
			{
				// Выдаем исключение
				throw 1;
			}

			// Копируем данные
			m_lBufferSize		= lBufferSize;

			// Обнуляем переменные
			m_lWritePointer		= 0;

			// Выделяем буфер.
			m_lpBuffer = ( LPBYTE ) ::HeapAlloc ( GetProcessHeap (), HEAP_ZERO_MEMORY, (size_t) m_lBufferSize );

			// Если буффер не выделился, возвращаем Exception
			if ( !m_lpBuffer )
			{
				// Выдаем исключение
				throw 1;
			}

			// Все отлично!!!
		}

		// Деструктор
		virtual ~CBufferedFileWriter ()
		{
			// Если нужно закрыть файл, закрываем его
			if ( m_hFile && m_hFile != INVALID_HANDLE_VALUE )
			{
				// Дописываем сначала все данные оставшиеся, а затем удаляем
				try
				{
					// Это на случай, если писать больше не можем, но файл-то удалить нужно
					Flush ();
				}
				catch ( ... )
				{
				}
				CloseHandle ( m_hFile );
				m_hFile = NULL;
			}

			// Удаляем буффер
			if ( m_lpBuffer )
			{
				::HeapFree ( GetProcessHeap (), 0, m_lpBuffer );
				m_lpBuffer = NULL;
			}
		}
		// Принимает данные на запись
		virtual VOID Write ( LPBYTE lpData, LONG64 lDataLength )
		{
			// Пока размер данных не исчерпан
			while ( 0 < lDataLength )
			{
				// Размер свободной памяти в буффере
				LONG64 lBufferFreeLength = 0;

				// Цикл, пока не появится свободное место для записи
				while ( 0 >= ( lBufferFreeLength = m_lBufferSize - m_lWritePointer ) )
				{
					// Пишем данные, если не получилось, то генерируем исключение
					if ( FALSE == WriteBuffer ( m_lBufferSize ) )
						throw 1;
				}

				// Проверяем достаточно ли места, чтобы записать весь пришедший буфер в память
				if ( lBufferFreeLength > lDataLength )
					lBufferFreeLength = lDataLength;

				// Копируем данные в буффер
				memcpy ( m_lpBuffer + m_lWritePointer, lpData, (size_t) lBufferFreeLength);

				// Уменьшаем размер данных ровно на столько, сколько записали в буффер
				lDataLength -= lBufferFreeLength;
				// Сдвигаем указатель на пришедшие данные
				lpData = lpData + lBufferFreeLength;
				// Сдвигаем позицию для записи данных в буффер
				m_lWritePointer += lBufferFreeLength;
			}
		}
		// Флаш ( дописываем все данные )
		virtual VOID Flush ()
		{
			// Пишем все данные, которые есть
			if ( 0 < m_lWritePointer )
			{
				// Если пришла ошибка, то генерируем исключение
				if ( FALSE == WriteBuffer ( m_lWritePointer ) )
					throw 1;
			}
		}
		// Сик на указанную позицию по файлу
		virtual VOID Seek ( LONG64 lPosition, DWORD dwFrom = FILE_CURRENT )
		{

			// Допишем сначала все данные
			Flush ();

			// Делаем сик по файлу
			LARGE_INTEGER liOffset;
			liOffset.QuadPart = lPosition;
			DWORD nNewPos = ::SetFilePointer(m_hFile, liOffset.LowPart, &liOffset.HighPart, dwFrom);
			
			if (nNewPos == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}

		}
		// Текущая позиция 
		virtual VOID GetPosition(ULONGLONG& nPos)
		{
			nPos=0;
			if(m_hFile == NULL)return;

			LARGE_INTEGER liOffset;
			liOffset.QuadPart = 0;
			liOffset.LowPart = ::SetFilePointer(m_hFile, 0, &liOffset.HighPart, FILE_CURRENT);
			if (liOffset.LowPart == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}
			nPos = liOffset.QuadPart + m_lWritePointer;
		}
		// Размер записанного файла
		virtual void GetSize(ULONGLONG& nLen) 
		{
			nLen=0;
			if(m_hFile == NULL)return;
			
			LARGE_INTEGER liOffset;
			ULARGE_INTEGER liFileSize;
			
			liOffset.QuadPart = 0;
			liOffset.LowPart = ::SetFilePointer(m_hFile, 0, &liOffset.HighPart, FILE_CURRENT);

			liFileSize.LowPart = ::GetFileSize(m_hFile, &liFileSize.HighPart);
			
			if (liFileSize.LowPart == INVALID_FILE_SIZE || liOffset.LowPart == INVALID_SET_FILE_POINTER)
			{
				HRESULT hr;

				hr = AtlHresultFromLastError();
				if (FAILED(hr))
					throw 1;
			}

			nLen = liFileSize.QuadPart;

			if (liFileSize.QuadPart==liOffset.QuadPart) //находимся в конце файла то есть учитываем буфер
			{
				nLen += m_lWritePointer;
			}
		}
	private :

		// Сама запись на диск
		BOOL WriteBuffer ( LONG64 lSize )
		{
			// Проверка
			if ( INVALID_HANDLE_VALUE == m_hFile || 0 == lSize )
				return FALSE;

			// Сколько байт записалось на диск
			DWORD dwBytesWrite = 0;

			// Пишем на диск
			if ( !WriteFile ( m_hFile, m_lpBuffer, ( DWORD ) lSize, &dwBytesWrite, NULL ) )
			{
				// Если что-то произошло не так, возвращаем FALSE
				return FALSE;
			}

			// Проверка, если записалось меньше, то это ошибка
			if ( ( LONG64 ) dwBytesWrite != lSize )
				return FALSE;

			// Сбрасываем в 0 указатель на позицию данных в буффере
			m_lWritePointer = 0;
			return TRUE;
		}
	};
}