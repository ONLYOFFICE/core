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

#include "Ole1FormatReader.h"

CDataStream::CDataStream() : pBuffer(NULL)
{
}
CDataStream::CDataStream(BYTE* pBuf, unsigned int unSize)
{
	SetStream(pBuf, unSize);
}
CDataStream::~CDataStream()
{
}
void CDataStream::SetStream(BYTE* pBuf, unsigned int unSize)
{
	pBuffer = pBuf;
	pCur    = pBuf;
	pEnd    = pBuf + unSize + 1;
};
BYTE* CDataStream::GetCurPtr()
{
	return pCur;
}

unsigned char CDataStream::ReadUChar()
{
	if (pCur + 1 >= pEnd)
		return 0;

	unsigned char unResult = pCur[0];
	pCur++;
	return unResult;
};
void CDataStream::WriteUChar(unsigned char val)
{
	if (pCur + 1 >= pEnd)
		return;

	pCur[0] = val;
	pCur++;
};
unsigned short CDataStream::ReadUShort()
{
	if (pCur + 2 >= pEnd)
		return 0;

	unsigned short ushResult = (pCur[0]) | ((pCur[1]) << 8);
	pCur += 2;
	return ushResult;
};
void CDataStream::WriteUShort(unsigned short val)
{
	if (pCur + 2 >= pEnd)
		return;

	((unsigned short*) pCur)[0] = val;
	pCur += 2;
};
_UINT32 CDataStream::ReadULong()
{
	if (pCur + 4 >= pEnd)
		return 0;

	_UINT32 unResult = (_UINT32)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
	pCur += 4;
	return unResult;
};
void CDataStream::WriteULong(_UINT32 val)
{
	if (pCur + 4 >= pEnd)
		return;

	((_UINT32*)pCur)[0] = val;
	pCur += 4;
};
double CDataStream::ReadDouble()
{
	if (pCur + 4 >= pEnd)
		return 0;

	float output;

	*((unsigned char*)(&output) + 0) = pCur[0];
	*((unsigned char*)(&output) + 1) = pCur[1];
	*((unsigned char*)(&output) + 2) = pCur[2];
	*((unsigned char*)(&output) + 3) = pCur[3];

	pCur += 4;

	return output;

	int lIntValue  = (int)((pCur[0] << 16) | ((pCur[1]) << 8) | ((pCur[2]) << 0));
	int lFracValue = (int)(pCur[3]);
	pCur += 4;
	return (double)(lIntValue + (lFracValue / 16.0));
};
char CDataStream::ReadChar()
{
	return (char)ReadUChar();
}
void CDataStream::WriteChar(char val)
{
	WriteUChar((unsigned char)val);
}
short CDataStream::ReadShort()
{
	return (short)ReadUShort();
}
void CDataStream::WriteShort(short val)
{
	WriteUShort((unsigned short) val);
}
_INT32 CDataStream::ReadLong()
{
	return (_INT32)ReadULong();
}
void CDataStream::WriteLong(_INT32 val)
{
	WriteULong((_UINT32)val);
}
void CDataStream::ReadBytes(char*  pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur);
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		pBuffer[ulIndex] = ReadChar();
	}
}
void CDataStream::WriteBytes(char*  pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur);
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		WriteChar(pBuffer[ulIndex]);
	}
}
void CDataStream::ReadBytes(unsigned char*  pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur);
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		pBuffer[ulIndex] = ReadUChar();
	}
}
void CDataStream::WriteBytes(unsigned char*  pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur);
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		WriteUChar(pBuffer[ulIndex]);
	}
}
void CDataStream::ReadBytes(unsigned short* pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur) / 2;
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		pBuffer[ulIndex] = ReadUShort();
	}
}
void CDataStream::ReadBytes(short* pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur) / 2;
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		pBuffer[ulIndex] = ReadShort();
	}
}
void CDataStream::ReadBytes(_UINT32*  pBuffer, unsigned int ulSize)
{
	size_t ulRemainSize = (pEnd - pCur) / 4;
	size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

	for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
	{
		pBuffer[ulIndex] = ReadULong();
	}
}
CDataStream& CDataStream::operator>>(unsigned char&  nValue)
{
	nValue = ReadUChar();
	return *this;
}
CDataStream& CDataStream::operator<<(unsigned char nValue)
{
	WriteUChar(nValue);
	return *this;
}
CDataStream& CDataStream::operator>>(unsigned short& nValue)
{
	nValue = ReadUShort();
	return *this;
}
CDataStream& CDataStream::operator<<(unsigned short nValue)
{
	WriteUShort(nValue);
	return *this;
}
CDataStream& CDataStream::operator>>(_UINT32&  nValue)
{
	nValue = ReadULong();
	return *this;
}
CDataStream& CDataStream::operator<<(_UINT32  nValue)
{
	WriteULong(nValue);
	return *this;
}
CDataStream& CDataStream::operator>>(double& dValue)
{
	dValue = ReadDouble();
	return *this;
}
CDataStream& CDataStream::operator>>(char&  nValue)
{
	nValue = ReadChar();
	return *this;
}
CDataStream& CDataStream::operator<<(char  nValue)
{
	WriteChar(nValue);
	return *this;
}
CDataStream& CDataStream::operator>>(bool&  nValue)
{
	nValue = !!ReadChar();
	return *this;
}
CDataStream& CDataStream::operator>>(short& nValue)
{
	nValue = ReadShort();
	return *this;
}
CDataStream& CDataStream::operator<<(short nValue)
{
	WriteShort(nValue);
	return *this;
}
CDataStream& CDataStream::operator>>(_INT32&  nValue)
{
	nValue = ReadLong();
	return *this;
}
CDataStream& CDataStream::operator<<(_INT32 nValue)
{
	WriteLong(nValue);
	return *this;
}

bool CDataStream::IsValid() const
{
	if (NULL == pBuffer)
		return false;

	return true;
}
bool CDataStream::IsEof() const
{
	if (pCur >= pEnd)
		return true;

	return false;
}
unsigned int CDataStream::Tell()
{
	return (unsigned int)(pCur - pBuffer);
}
void CDataStream::Skip(unsigned int ulSkip)
{
	pCur += ulSkip;
}
void CDataStream::SeekBack(unsigned int ulSkipBack)
{
	pCur -= ulSkipBack;
}
void CDataStream::SeekToStart()
{
	pCur = pBuffer;
}
unsigned int CDataStream::CanReadWrite()
{
	return (unsigned int)(pEnd - pCur);
}

static CDataStream & operator >> (CDataStream & strm, LengthPrefixedAnsiString & str)
{
	strm >> str.size;
	char *s = new char[str.size];
	strm.ReadBytes(s, str.size);
	str.val = std::string(s, str.size);
	delete []s;
	return strm;
}
static CDataStream & operator << (CDataStream & strm, LengthPrefixedAnsiString str)
{
	strm << str.size;

	strm.WriteBytes((char*)str.val.c_str(), str.size - 1);
	strm.WriteUChar(0);
	return strm;
}

Ole1FormatReaderWriter::Ole1FormatReaderWriter() : NativeData(NULL), NativeDataSize(0), OtherData(NULL), OtherDataSize(0)
{
}
Ole1FormatReaderWriter::Ole1FormatReaderWriter(BYTE *pData, int Size) : NativeData(NULL), NativeDataSize(0), OtherData(NULL), OtherDataSize(0)
{
	Read(pData, Size);
}
Ole1FormatReaderWriter::~Ole1FormatReaderWriter()
{
	if (OtherData)
		delete []OtherData;
	OtherData = NULL;
}
void Ole1FormatReaderWriter::Read(BYTE *pData, int Size)
{
	NativeDataSize = 0;
	if (!pData || Size < 8) return;

	CDataStream stream(pData, Size);

	stream >> Header.OLEVersion >> Header.FormatID;

	if (Header.FormatID == 2)
	{
		if (Header.OLEVersion & 0x00000500 ||
				Header.OLEVersion & 0x00010001)
		{
			stream >> Header.ClassName;
		}
		stream >> Header.Width >> Header.Height;
		
		stream >> NativeDataSize;

		NativeData = stream.GetCurPtr();
		stream.Skip(NativeDataSize);

		if (stream.IsEof())
			return;

		// далее графическое представление
		OtherDataSize = stream.CanReadWrite();

		OtherData = new BYTE[OtherDataSize];
		stream.ReadBytes(OtherData, OtherDataSize);
	}

}
void Ole1FormatReaderWriter::Write(BYTE *pData, int &Size)
{
	Size = 0;
	if (!pData) return;
	if (NativeDataSize < 1) return;

	CDataStream stream(pData, NativeDataSize + 2048);

	stream << Header.OLEVersion << Header.FormatID;

	stream << Header.ClassName;

	stream << Header.Width << Header.Height;

	stream << NativeDataSize;

	stream.WriteBytes(NativeData, NativeDataSize);

	/// далее графическое представление
	BYTE other[9] = {1, 5, 0, 0, 0, 0, 0, 0, 0};

	Size = stream.Tell() - 1;
	stream.WriteBytes(other, 9);
	Size = stream.Tell() - 1;

	//padding ???

	char padding[8];
	memset(padding, 0, 8);

	stream.WriteBytes(padding, (Size / 8 + 1 ) * 8 - Size);
	Size = stream.Tell() - 1;
}
