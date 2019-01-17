/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once

#include "./DocxFormat/Source/SystemUtility/File.h"

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
		virtual void Write ( BYTE* lpData, LONG64 lDataLength ) = 0;
		// Флаш ( дописываем все данные )
		virtual void Flush () = 0;
		// Сик на указанную позицию по файлу
                virtual void Seek ( LONG64 lPosition, DWORD dwFrom = 0 ) = 0;
		// Текущая позиция 
                virtual void GetPosition(ULONG64& nPos) = 0;
		// Размер записанного файла
                virtual void GetSize(ULONG64& nLen) = 0;

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

		CFile m_oFile;
		//HANDLE m_hFile;									// Хендл на файл, в который идет запись

		LONG64 m_lBufferSize;							// Размер буффера

		BYTE* m_lpBuffer;								// Сам буффер

		LONG64 m_lWritePointer;							// Позиция для записи данных в буффер

	public :

		// Конструктор ( от имени файла, сам контрол открывает файл )
                CBufferedFileWriter ( std::wstring & sFileName, LONG64 lBufferSize = 10 * 1024 * 1024 ) : CFileWriter ()
		{
			if (m_oFile.CreateFile(sFileName) != S_OK)
			// Если не получилось открыть файл
			{
				// Выдаем исключение
				throw 1;
			}

			// Копируем данные
			m_lBufferSize		= lBufferSize;

			// Обнуляем переменные
			m_lWritePointer		= 0;

			// Выделяем буфер.
			m_lpBuffer = ( BYTE* ) new BYTE[ m_lBufferSize ];

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
			Flush();
			m_oFile.CloseFile();

			// Удаляем буффер
			RELEASEARRAYOBJECTS(m_lpBuffer);
		}
		// Принимает данные на запись
		virtual void Write ( BYTE* lpData, LONG64 lDataLength )
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
                                        if ( false == WriteBuffer ( m_lBufferSize ) )
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
		virtual void Flush ()
		{
			// Пишем все данные, которые есть
			if ( 0 < m_lWritePointer )
			{
				// Если пришла ошибка, то генерируем исключение
                                if ( false == WriteBuffer ( m_lWritePointer ) )
					throw 1;
			}
		}
		// Сик на указанную позицию по файлу
		virtual void Seek ( LONG64 lPosition, DWORD dwFrom/* = FILE_CURRENT */)
		{

			// Допишем сначала все данные
			Flush ();

			if (dwFrom == 1/*FILE_CURRENT*/)
			{
				dwFrom += (DWORD)m_oFile.GetPosition();
			}

			if (m_oFile.SetPosition((ULONG)lPosition) != S_OK)
				throw 1;

		}
		// Текущая позиция 
                virtual void GetPosition(ULONG64& nPos)
		{
			nPos = m_oFile.GetPosition() + m_lWritePointer;

		}
		// Размер записанного файла
                virtual void GetSize(ULONG64& nLen)
		{
			nLen = m_oFile.GetFileSize() + m_lWritePointer;	

		}
	private :

		// Сама запись на диск
                bool WriteBuffer ( LONG64 lSize )
		{
			// Пишем на диск
			if (m_oFile.WriteFile(m_lpBuffer, ( DWORD ) lSize) != S_OK)
			{
				// Если что-то произошло не так, возвращаем FALSE
                                return false;
			}
			// Сбрасываем в 0 указатель на позицию данных в буффере
			m_lWritePointer = 0;
                        return true;
		}
	};
}
