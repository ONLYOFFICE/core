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

#include "../OOXML/SystemUtility/File.h"

namespace NFileWriter
{
	class CFileWriter
	{
	public :
		virtual void Write ( BYTE* lpData, LONG64 lDataLength ) = 0;
		virtual void Flush () = 0;
		virtual void Seek(LONG64 lPosition, DWORD dwFrom = 0) = 0;
		virtual void GetPosition(ULONG64& nPos) = 0;
		virtual void GetSize(ULONG64& nLen) = 0;
		CFileWriter ()
		{
		}
		virtual ~CFileWriter ()
		{
		}
	};

	class CBufferedFileWriter : public CFileWriter
	{
	private :

		NSFile::CFileBinary m_oFile;

		LONG64 m_lBufferSize;							// Размер буффера
		BYTE* m_lpBuffer;								// Сам буффер
		LONG64 m_lWritePointer;							// Позиция для записи данных в буффер

	public :

		CBufferedFileWriter ( std::wstring & sFileName, LONG64 lBufferSize = 10 * 1024 * 1024 ) : CFileWriter ()
		{
			if (false == m_oFile.CreateFileW(sFileName))
			{
				throw 1;
			}

			m_lBufferSize = lBufferSize;

			m_lWritePointer = 0;

			m_lpBuffer = ( BYTE* ) new BYTE[ m_lBufferSize ];

			if ( !m_lpBuffer )
			{
				throw 1;
			}
		}

		virtual ~CBufferedFileWriter ()
		{
			Flush();
			m_oFile.CloseFile();

			RELEASEARRAYOBJECTS(m_lpBuffer);
		}
		virtual void Write ( BYTE* lpData, LONG64 lDataLength )
		{
			while ( 0 < lDataLength )
			{
				LONG64 lBufferFreeLength = 0;

				while ( 0 >= ( lBufferFreeLength = m_lBufferSize - m_lWritePointer ) )
				{
					if ( false == WriteBuffer ( m_lBufferSize ) )
						throw 1;
				}

				if ( lBufferFreeLength > lDataLength )
					lBufferFreeLength = lDataLength;

				memcpy ( m_lpBuffer + m_lWritePointer, lpData, (size_t) lBufferFreeLength);

				lDataLength -= lBufferFreeLength;
				lpData = lpData + lBufferFreeLength;
				m_lWritePointer += lBufferFreeLength;
			}
		}
		virtual void Flush ()
		{
			if ( 0 < m_lWritePointer )
			{
				if ( false == WriteBuffer ( m_lWritePointer ) )
					throw 1;
			}
		}
		virtual void Seek ( LONG64 lPosition, DWORD dwFrom/* = FILE_CURRENT */)
		{
			Flush ();

			if (dwFrom == 1/*FILE_CURRENT*/)
			{
				dwFrom += (DWORD)m_oFile.GetPosition();
			}

			if (false == m_oFile.SetPosition((ULONG)lPosition))
				throw 1;

		}
		virtual void GetPosition(ULONG64& nPos)
		{
			nPos = m_oFile.GetPosition() + m_lWritePointer;

		}
		virtual void GetSize(ULONG64& nLen)
		{
			nLen = m_oFile.GetFileSize() + m_lWritePointer;	

		}
	private :

		bool WriteBuffer ( LONG64 lSize )
		{
			if (false == m_oFile.WriteFile(m_lpBuffer, ( DWORD ) lSize))
			{
				return false;
			}
			m_lWritePointer = 0;
			return true;
		}
	};
}
