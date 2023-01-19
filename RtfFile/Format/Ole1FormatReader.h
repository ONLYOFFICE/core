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
