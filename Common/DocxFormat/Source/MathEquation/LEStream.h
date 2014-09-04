#ifndef _MATHEQUATION_LESTREAM
#define _MATHEQUATION_LESTREAM

#include "..\CompoundDocument\pole.h"
#include "Types.h"

namespace MathEquation
{
template<class StreamBase>
class CLEStream
{
public :
	
	CLEStream(StreamBase *pStream) : pBuffer(NULL)
	{
		if (NULL == pStream)
			return;

		pStream->seek(0, std::ios_base::end);
		unsigned int unFileSize = (unsigned int)pStream->tellg();
		pStream->seek(0);

		pBuffer = new BYTE[unFileSize];
		if (!pBuffer)
			return;

		pStream->read(pBuffer, unFileSize);

		pCur = pBuffer;
		pEnd = pBuffer + unFileSize;
	}

	~CLEStream()
	{
		if (NULL != pBuffer)
			delete pBuffer;
	}

	void operator>>(uint8_t  &nValue)
	{
		nValue = ReadUInt8();
	}

	void operator>>(uint16_t &nValue)
	{
		nValue = ReadUInt16();
	}

	void operator>>(uint32_t &nValue)
	{
		nValue = ReadUInt32();
	}

	void operator>>(int8_t   &nValue)
	{
		nValue = ReadUInt8();
	}

	void operator>>(int16_t  &nValue)
	{
		nValue = Read_UInt16();
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

	uint8_t  ReadUInt8()
	{
		if (pCur + 1 >= pEnd)
			return 0;

		uint8_t unResult = pCur[0];
		pCur++;
		return unResult;
	}

	uint16_t ReadUInt16()
	{
		if (pCur + 2 >= pEnd)
			return 0;

		uint16_t ushResult = (pCur[0]) | ((pCur[1]) << 8);
		pCur += 2;
		return ushResult;
	}

	uint32_t ReadUInt32()
	{
		if (pCur + 4 >= pEnd)
			return 0;
		uint32_t unResult = (uint32_t)((pCur[0]<< 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
		pCur += 4;
		return unResult;
	}

	uint32_t Tell()
	{
		return (uint32_t)(pCur - pBuffer);
	}

	void SeekRel(uint32_t nPos)
	{
		pCur += nPos;
	}

private:

	BYTE *pBuffer;
	BYTE *pCur;
	BYTE *pEnd;
};

}

#endif