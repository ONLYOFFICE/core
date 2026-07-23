/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

		LONG64 m_lBufferSize;							// Buffer size
		BYTE* m_lpBuffer;								// The buffer itself
		LONG64 m_lWritePointer;							// Position for writing data to the buffer

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
