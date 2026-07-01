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
#ifndef _PDF_WRITER_SRC_STREAMS_H
#define _PDF_WRITER_SRC_STREAMS_H

#include "Types.h"
#include "../../DesktopEditor/common/File.h"

#define STREAM_FILTER_NONE          0x0000
#define STREAM_FILTER_ALREADY_DECODE 0x0001
#define STREAM_FILTER_ASCIIHEX      0x0100
#define STREAM_FILTER_ASCII85       0x0200
#define STREAM_FILTER_FLATE_DECODE  0x0400
#define STREAM_FILTER_DCT_DECODE    0x0800
#define STREAM_FILTER_JPX_DECODE    0x1000
#define STREAM_FILTER_JBIG2_DECODE  0x2000
#define STREAM_FILTER_LZW_DECODE    0x4000
#define STREAM_FILTER_CCITT_DECODE  0x8000

#define STREAM_PREDICTOR_NONE       0x0000
#define STREAM_PREDICTOR_FLATE_TIFF 0x0001

namespace PdfWriter
{
	typedef enum
	{
		SeekSet   = 0,
		SeekCur   = 1,
		SeekEnd   = 2,
		SeekWrite = 3
	} EWhenceMode;
	typedef enum
	{
		StreamUnknown  = 0,
		StreamCallBack = 1,
		StreamFile     = 2,
		StreamMemory   = 3,
		StreamTempFile = 4
	} EStreamType;

	class CNullObject;
	class CBoolObject;
	class CNumberObject;
	class CRealObject;
	class CNameObject;
	class CStringObject;
	class CBinaryObject;
	class CArrayObject;
	class CDictObject;
	class CObjectBase;

	class CEncrypt;
	class CStream
	{
	public:

		CStream()
		{
			m_unCheckSum = 0;
		}
		virtual ~CStream()
		{
		}
		virtual bool         IsEof() = 0;
		virtual void         Write(const BYTE* pBuffer, unsigned int unSize) = 0;
		virtual void         Read(BYTE* pBuffer, unsigned int* punSize) = 0;
		virtual void         Seek(int nPos, EWhenceMode eMode) = 0;
		virtual int          Tell() = 0;
		virtual void         Close() = 0;
		virtual unsigned int Size() = 0;
		virtual void         Clear() {}
		virtual EStreamType  GetType()
		{
			return StreamUnknown;
		}
		virtual bool OpenFile(const std::wstring& wsFilePath, bool bWrite = false) { return false; }

		unsigned int   ReadUInt();
		unsigned char  ReadUChar();
		char           ReadChar();
		unsigned short ReadUShort();
		double         ReadFixed();
		long long      ReadLongDateTime();
		unsigned int   ReadOffset(BYTE nOffset);

		void Write(const BYTE* pBuffer, unsigned int unSize, bool bCalcCheckSum);
		void WriteChar (char nChar);
		void WriteStr  (const char* sString);
		void WriteUChar(unsigned char unChar);
		void WriteInt  (int nValue);
		void WriteUInt (unsigned int nValue);
		void WriteHex  (int nValue, int nLen);
		void WriteReal (float fValue);
		void WriteReal (double dValue);
		void WriteEscapeName(const char* sValue);
        void WriteEscapeText(const BYTE* sText, unsigned int unLen, bool isUTF16 = false, bool isDictValue = false);
		void WriteBinary(const BYTE* pData, unsigned int unLen, CEncrypt* pEncrypt);
		void WriteStream(CStream* pStream, unsigned int unFilter, CEncrypt* pEncrypt);

		void Write(CNullObject* pNull);
		void Write(CBoolObject* pBool);
		void Write(CNumberObject* pNumber);
		void Write(CRealObject* pReal);
		void Write(CNameObject* pName);
		void Write(CStringObject* pString, CEncrypt* pEncrypt);
		void Write(CBinaryObject* pBinary, CEncrypt* pEncrypt);
		void Write(CArrayObject* pBinary, CEncrypt* pEncrypt);
		void Write(CDictObject* pDict, CEncrypt* pEncrypt);
		void Write(CObjectBase* pObject, CEncrypt* pEncrypt);

	private:

		void WriteStreamWithDeflate(CStream* pStream, CEncrypt* pEncrypt);
		bool CheckSize(unsigned int unSize);

	protected:

		unsigned int m_unCheckSum;
	};
	class CMemoryStream : public CStream
	{
	public:

		CMemoryStream();
		CMemoryStream(unsigned int unBufferSize);
		CMemoryStream(BYTE* pBuffer, unsigned int unSize, bool bFree = false);
		~CMemoryStream();
		bool         IsEof();
		void         Write(const BYTE* pBuffer, unsigned int unSize);
		void         Read(BYTE* pBuffer, unsigned int* punSize);
		void         Seek(int nPos, EWhenceMode eMode);
		int          Tell();
		void         Close();
		unsigned int Size();
		void         Clear();
		EStreamType  GetType()
		{
			return StreamMemory;
		}
		BYTE* GetBuffer();
		BYTE* GetCurBuffer();
		void ClearWithoutAttack();

		void SetForWasm(bool bForWasm);
		unsigned int GetHeaderOffset() const;
		int RealTell() const;
		void SkipLen();
		void WriteLen();

	private:

		void Shrink(unsigned int unSize);

	private:
		bool         m_bFree;
		bool         m_bForWasm = false;

		BYTE*        m_pBuffer;
		int          m_nBufferSize;
		BYTE*        m_pCur;
		unsigned int m_unSize;
	};
	class CFileStream : public CStream
	{
	public:

		CFileStream();
		~CFileStream();
		bool         IsEof();
		void         Write(const BYTE* pBuffer, unsigned int unSize);
		void         Read(BYTE* pBuffer, unsigned int* punSize);
		void         Seek(int nPos, EWhenceMode eMode);
		int          Tell();
		void         Close();
		unsigned int Size();
		EStreamType  GetType()
		{
			return StreamFile;
		}
		bool OpenFile(const std::wstring& wsFilePath, bool bWrite = false);

	private:

		NSFile::CFileBinary m_oFile;
		std::wstring        m_wsFilePath;
	};
	class CImageFileStream : public CStream
	{
	public:

		CImageFileStream();
		~CImageFileStream();
		bool         IsEof();
		void         Write(const BYTE* pBuffer, unsigned int unSize);
		void         Read(BYTE* pBuffer, unsigned int* punSize);
		void         Seek(int nPos, EWhenceMode eMode);
		int          Tell();
		void         Close();
		unsigned int Size();
		EStreamType  GetType()
		{
			return StreamFile;
		}
		bool OpenFile(const std::wstring& wsFilePath, bool bWrite = false);

	private:

		bool OpenFile();
		void CloseFile();
		
	private:

		NSFile::CFileBinary m_oFile;
		std::wstring        m_wsFilePath;
		int                 m_nFilePos;
		int                 m_nFileSize;
	};
	class CImageMemoryStream : public CStream
	{
	public:
		CImageMemoryStream();
		~CImageMemoryStream();

		bool         IsEof();
		void         Write(const BYTE* pBuffer, unsigned int unSize);
		void         Read(BYTE* pBuffer, unsigned int* punSize);
		void         Seek(int nPos, EWhenceMode eMode);
		int          Tell();
		void         Close();
		unsigned int Size();
		EStreamType  GetType()
		{
			return StreamMemory;
		}
		bool OpenFile(const std::wstring& wsFilePath, bool bWrite = false);

	private:

		BYTE* m_pData;
		LONG  m_lLength;
		int   m_nFilePos;
	};

	static CStream* CreateImageStream()
	{
	#ifdef BUILDING_WASM_MODULE
		return new(std::nothrow) CImageMemoryStream();
	#else
		return new(std::nothrow) CImageFileStream();
	#endif
	}
}

#endif // _PDF_WRITER_SRC_STREAMS_H
