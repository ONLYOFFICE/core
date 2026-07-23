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

#include "../../common/File.h"

namespace Jpeg2000
{
	//-------------------------------------------------------------------------------------------------------------------------------
	// Byte input-output (Stream IO)
	//-------------------------------------------------------------------------------------------------------------------------------

#define STREAM_READ	 0x0001 // Stream is open for reading
#define STREAM_WRITE 0x0002 // Stream is open for writing

	typedef struct TByteIO
	{
		int            nMode;   // Read or write ( STREAM_READ or OPJ_STREAM_WRITE )
		unsigned char *pBuffer; // Pointer to buffer start
		int            nLength; // Buffer size in bytes

		unsigned char *pStart;  // Pointer to stream start
		unsigned char *pEnd;    // Pointer to stream end
		unsigned char *pCurPos; // Pointer to current position in stream
	} ByteIO;


	class CReader
	{
	public:
		virtual ~CReader(void){}

		virtual bool         Open(void* sStream, int nSizeStream){ return false; }
		virtual void         Close(){}
		virtual void         Read(BYTE* pData, int nSize){}
		virtual unsigned int Read(int nSize){ return 0; }
		virtual void         Skip(int nSize){}
		virtual void         Seek(int nPosition){}
		virtual unsigned int Write(unsigned int nValue, int nLen){ return 0; }
		virtual int          Tell(){ return 0; }
		virtual int          GetLeftSize(){ return 0; }
		virtual void*        GetOwner(){ return NULL; }
	};

	class CReaderStream : CReader
	{
	public:
		CReaderStream(void);
		CReaderStream(void* sStream, int nSizeStream);
		virtual ~CReaderStream(void);

		bool         Open(void* sStream, int nSizeStream);
		void         Close();
		void         Read(BYTE* pData, int nSize);
		unsigned int Read(int nSize);
		void         Skip(int nSize);
		void         Seek(int nPosition);
		unsigned int Write(unsigned int nValue, int nLen);
		int          Tell();
		int          GetLeftSize();
		void*        GetOwner();

	protected:
		ByteIO* m_pStream;
		int     m_nPosition;
		int     m_nSkiped;
		int     m_nSize;
	};

	class CReaderFile : CReader
	{
	public:
		CReaderFile(void);
		CReaderFile(const std::wstring& wsFileName, long lMode);
		virtual ~CReaderFile(void);

		bool         Open(const std::wstring& wsFileName, int nSizeStream);
		void         Close();
		void         Read(BYTE* pData, int nSize);
		unsigned int Read(int nSize);
		void         Skip(int nSize);
		void         Seek(int nPosition);
		unsigned int Write(unsigned int nValue, int nLen);
		int          Tell();
		int          GetLeftSize();
		void*        GetOwner();

	protected:

		NSFile::CFileBinary m_oFile;
		FILE*               m_pFile;
		int                 m_nPosition;
		int                 m_nSkiped;
		int                 m_nSize;
	};
}