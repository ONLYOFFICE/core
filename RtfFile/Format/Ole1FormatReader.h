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

#include "../../OOXML/Base/Base.h"
#include "../../DesktopEditor/common/Types.h"

class CDataStream
{
public:
	CDataStream();
	CDataStream(BYTE* pBuf, unsigned int unSize);
	~CDataStream();

	void SetStream(BYTE* pBuf, unsigned int unSize);
	BYTE* GetCurPtr();

	unsigned char ReadUChar();
	void WriteUChar(unsigned char val);
	unsigned short ReadUShort();
	void WriteUShort(unsigned short val);
	_UINT32 ReadULong();
	void WriteULong(_UINT32 val);
	double ReadDouble();
	char ReadChar();
	void WriteChar(char val);
	short ReadShort();
	void WriteShort(short val);
	_INT32 ReadLong();
	void WriteLong(_INT32 val);
	void ReadBytes(char*  pBuffer, unsigned int ulSize);
	void WriteBytes(char*  pBuffer, unsigned int ulSize);
	void ReadBytes(unsigned char*  pBuffer, unsigned int ulSize);
	void WriteBytes(unsigned char*  pBuffer, unsigned int ulSize);
	void ReadBytes(unsigned short* pBuffer, unsigned int ulSize);
	void ReadBytes(short* pBuffer, unsigned int ulSize);
	void ReadBytes(_UINT32*  pBuffer, unsigned int ulSize);

	CDataStream& operator>>(unsigned char&  nValue);
	CDataStream& operator<<(unsigned char nValue);
	CDataStream& operator>>(unsigned short& nValue);
	CDataStream& operator<<(unsigned short nValue);
	CDataStream& operator>>(_UINT32&  nValue);
	CDataStream& operator<<(_UINT32  nValue);
	CDataStream& operator>>(double& dValue);
	CDataStream& operator>>(char&  nValue);
	CDataStream& operator<<(char  nValue);
	CDataStream& operator>>(bool&  nValue);
	CDataStream& operator>>(short& nValue);
	CDataStream& operator<<(short nValue);
	CDataStream& operator>>(_INT32&  nValue);
	CDataStream& operator<<(_INT32 nValue);

	bool IsValid() const;
	bool IsEof() const;
	unsigned int Tell();
	void Skip(unsigned int ulSkip);
	void SeekBack(unsigned int ulSkipBack);
	void SeekToStart();
	unsigned int CanReadWrite();

private:
	BYTE *pBuffer;
	BYTE *pCur;
	BYTE *pEnd;
};

struct LengthPrefixedAnsiString
{
	_UINT32		size = 0;
	std::string val;
};

static CDataStream & operator >> (CDataStream & strm, LengthPrefixedAnsiString & str);
static CDataStream & operator << (CDataStream & strm, LengthPrefixedAnsiString str);

struct ObjectHeader
{
	_UINT32 OLEVersion	= 1281;
	_UINT32 FormatID	= 2;

	LengthPrefixedAnsiString ClassName;

	_UINT32 Width	= 0;
	_UINT32 Height	= 0;
};

class Ole1FormatReaderWriter
{
public: 
	Ole1FormatReaderWriter();
	Ole1FormatReaderWriter(BYTE *pData, int Size);
	virtual ~Ole1FormatReaderWriter();

	void Read(BYTE *pData, int Size);
	void Write(BYTE *pData, int &Size);

	ObjectHeader				Header;
	
	_UINT32						NativeDataSize;
	BYTE						*NativeData;

	_UINT32						OtherDataSize;
	BYTE						*OtherData;
	
	//PresentationObjectHeader	PresentationHeader;
};
