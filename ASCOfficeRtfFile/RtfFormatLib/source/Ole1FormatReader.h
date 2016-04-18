#pragma once

class CDataStream
{
public:

	CDataStream() : pBuffer(NULL)
	{
	}
	CDataStream(BYTE* pBuf, unsigned int unSize)
	{
		SetStream(pBuf, unSize);
	}

	~CDataStream()
	{
	}

	void SetStream(BYTE* pBuf, unsigned int unSize)
	{
		pBuffer = pBuf;
		pCur    = pBuf;
		pEnd    = pBuf + unSize + 1;
	};
	BYTE* GetCurPtr()
	{
		return pCur;
	}

	unsigned char  ReadUChar()
	{
		if (pCur + 1 >= pEnd)
			return 0;

		unsigned char unResult = pCur[0];
		pCur++;
		return unResult;
	};
	unsigned short ReadUShort()
	{
		if (pCur + 2 >= pEnd)
			return 0;

		unsigned short ushResult = (pCur[0]) | ((pCur[1]) << 8);
		pCur += 2;
		return ushResult;
	};
	unsigned int ReadULong()
	{
		if (pCur + 4 >= pEnd)
			return 0;

		unsigned int unResult = (unsigned int)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
		pCur += 4;
		return unResult;
	};
	double ReadDouble()
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
	char ReadChar()
	{
		return (char)ReadUChar();
	};
	short          ReadShort()
	{
		return (short)ReadUShort();
	};
	int ReadLong()
	{
		return (int)ReadULong();
	};
	void ReadBytes(char*  pBuffer, unsigned int ulSize)
	{
		size_t ulRemainSize = (pEnd - pCur);
		size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

		for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
		{
			pBuffer[ulIndex] = ReadChar();
		}
	};
	void ReadBytes(unsigned char*  pBuffer, unsigned int ulSize)
	{
		size_t ulRemainSize = (pEnd - pCur);
		size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

		for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
		{
			pBuffer[ulIndex] = ReadChar();
		}
	};
	void ReadBytes(unsigned short* pBuffer, unsigned int ulSize)
	{
		size_t ulRemainSize = (pEnd - pCur) / 2;
		size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

		for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
		{
			pBuffer[ulIndex] = ReadUShort();
		}
	}
	void ReadBytes(short* pBuffer, unsigned int ulSize)
	{
		size_t ulRemainSize = (pEnd - pCur) / 2;
		size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

		for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
		{
			pBuffer[ulIndex] = ReadShort();
		}
	}
	void ReadBytes(unsigned int*  pBuffer, unsigned int ulSize)
	{
		size_t ulRemainSize = (pEnd - pCur) / 4;
		size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

		for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
		{
			pBuffer[ulIndex] = ReadULong();
		}
	}
	CDataStream& operator>>(unsigned char&  nValue)
	{
		nValue = ReadUChar();
		return *this;
	}
	CDataStream& operator>>(unsigned short& nValue)
	{
		nValue = ReadUShort();
		return *this;
	}
	CDataStream& operator>>(unsigned int&  nValue)
	{
		nValue = ReadULong();
		return *this;
	}
	CDataStream& operator>>(unsigned long&   nValue)
	{
		nValue = ReadULong();
		return *this;
	}
	CDataStream& operator>>(double&         dValue)
	{
		dValue = ReadDouble();
		return *this;
	}
	CDataStream& operator>>(char&  nValue)
	{
		nValue = ReadChar();
		return *this;
	}
	CDataStream& operator>>(bool&  nValue)
	{
		nValue = !!ReadChar();
		return *this;
	}
	CDataStream& operator>>(short& nValue)
	{
		nValue = ReadShort();
		return *this;
	}
	CDataStream& operator>>(int&  nValue)
	{
		nValue = ReadLong();
		return *this;
	}
	CDataStream& operator>>(long&   nValue)
	{
		nValue = ReadLong();
		return *this;
	}

	bool IsValid() const
	{
		if (NULL == pBuffer)
			return false;

		return true;
	}

	bool IsEof() const
	{
		if (pCur >= pEnd)
			return true;

		return false;
	}

	unsigned int Tell()
	{
		return (unsigned int)(pCur - pBuffer);
	}

	void Skip(unsigned int ulSkip)
	{
		pCur += ulSkip;
	}

	void SeekBack(unsigned int ulSkipBack)
	{
		pCur -= ulSkipBack;
	}

	void SeekToStart()
	{
		pCur = pBuffer;
	}

	unsigned int CanRead()
	{
		return (unsigned int)(pEnd - pCur);
	}


private:

	BYTE *pBuffer;
	BYTE *pCur;
	BYTE *pEnd;
};
struct LengthPrefixedAnsiString
{
	_UINT32		size;
	std::string val;
};
CDataStream & operator >> (CDataStream & strm, LengthPrefixedAnsiString & str)
{
	strm >> str.size;
	char *s = new char[str.size];
	strm.ReadBytes(s, str.size);
	str.val = std::string(s,str.size);
	delete []s;
	return strm;
}

struct ObjectHeader
{
	_UINT32 OLEVersion;
	_UINT32 FormatID;

	LengthPrefixedAnsiString ClassName;

	_UINT32 Width;
	_UINT32 Height;

};

class Ole1FormatReader
{
public: 

	Ole1FormatReader(BYTE *pData, int Size)
	{
		if (!pData || Size < 8) return;

		CDataStream stream(pData, Size);

		stream >> Header.OLEVersion >> Header.FormatID;

		if (Header.OLEVersion & 0x00000500)
		{
			stream >> Header.ClassName;
		}
		stream >> Header.Width >> Header.Height;
	
		stream >> NativeDataSize;
		
		NativeData = stream.GetCurPtr();
		stream.Skip(NativeDataSize);

		int sz = stream.CanRead();

		/// далее графическое представление 

	}
	ObjectHeader				Header;
	_UINT32						NativeDataSize;
	BYTE						*NativeData;
	//PresentationObjectHeader	PresentationHeader;

};
