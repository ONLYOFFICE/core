#pragma once

struct IBinaryReader
{
	virtual ~IBinaryReader() {}
	virtual unsigned short ReadUInt16() = 0;
	virtual short ReadInt16() = 0;
	virtual int ReadInt32() = 0;
	virtual unsigned int ReadUInt32() = 0;
	virtual unsigned char ReadByte() = 0;
	virtual unsigned char* ReadBytes( unsigned int count, bool isResultNeeded ) = 0;
	virtual unsigned long GetPosition() const = 0;
	virtual unsigned long GetSize() const = 0;
	virtual int Seek( int offset, int origin ) = 0;
};
