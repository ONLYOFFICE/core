#ifndef _METAFILE_WMF_EMF_COMMON_H
#define _METAFILE_WMF_EMF_COMMON_H

#ifndef BYTE
typedef unsigned char BYTE;
#endif

#ifndef NULL
#define NULL 0
#endif

#include <algorithm>

#include "Emf/EmfTypes.h"
#include "Wmf/WmfTypes.h"
#include "Wmf/WmfUtils.h"
#include "Emf/EmfObjects.h"

namespace Metafile
{
	class CDataStream
	{
	public:

		CDataStream() : pBuffer(NULL)
		{
		}

		~CDataStream()
		{
		}

		void SetStream(BYTE* pBuf, unsigned long unSize)
		{
			pBuffer = pBuf;
			pCur    = pBuf;
			pEnd    = pBuf + unSize + 1;
		};

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
		unsigned long  ReadULong()
		{
			if (pCur + 4 >= pEnd)
				return 0;

			unsigned long unResult = (unsigned long)((pCur[0] << 0) | ((pCur[1]) << 8) | ((pCur[2]) << 16) | ((pCur[3]) << 24));
			pCur += 4;
			return unResult;
		};
		double         ReadDouble()
		{
			if (pCur + 4 >= pEnd)
				return 0;

			long lIntValue  = (long)((pCur[0] << 16) | ((pCur[1]) << 8) | ((pCur[2]) << 0));
			long lFracValue = (long)(pCur[3]);
			pCur += 4;
			return (double)(lIntValue + (lFracValue / 16.0));
		};
		char           ReadChar()
		{
			return (char)ReadUChar();
		};
		short          ReadShort()
		{
			return (short)ReadUShort();
		};
		long           ReadLong()
		{
			return (long)ReadULong();
		};
		void           ReadBytes(BYTE* pBuffer, unsigned long ulSize)
		{
			size_t ulRemainSize = (pEnd - pCur);
			size_t ulFinalSize  = (ulRemainSize > ulSize ? ulSize : ulRemainSize);

			for (size_t ulIndex = 0; ulIndex < ulFinalSize; ulIndex++)
			{
				pBuffer[ulIndex] = ReadChar();
			}
		};

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
		CDataStream& operator>>(unsigned long&  nValue)
		{
			nValue = ReadULong();
			return *this;
		}
		CDataStream& operator>>(unsigned int&   nValue)
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
		CDataStream& operator>>(short& nValue)
		{
			nValue = ReadShort();
			return *this;
		}
		CDataStream& operator>>(long&  nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(int&   nValue)
		{
			nValue = ReadLong();
			return *this;
		}
		CDataStream& operator>>(TEmfRect& oRect)
		{
			*this >> oRect.shLeft;
			*this >> oRect.shTop;
			*this >> oRect.shRight;
			*this >> oRect.shBottom;

			return *this;
		}
		CDataStream& operator>>(TEmfRectL& oRect)
		{
			*this >> oRect.lLeft;
			*this >> oRect.lTop;
			*this >> oRect.lRight;
			*this >> oRect.lBottom;

			return *this;
		}
		CDataStream& operator>>(TEmfSizeL& oSize)
		{
			*this >> oSize.ulX;
			*this >> oSize.ulY;

			return *this;
		}
		CDataStream& operator>>(TEmfColor& oColor)
		{
			*this >> oColor.r;
			*this >> oColor.g;
			*this >> oColor.b;
			*this >> oColor.a;

			return *this;
		}
		CDataStream& operator>>(CEmfLogBrushEx& oBrush)
		{
			*this >> oBrush.BrushStyle;
			*this >> oBrush.Color;
			*this >> oBrush.BrushHatch;

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

		unsigned long Tell()
		{
			return (unsigned long)(pCur - pBuffer);
		}

		void Skip(unsigned long ulSkip)
		{
			pCur += ulSkip;
		}

		void SeekBack(unsigned long ulSkipBack)
		{
			pCur -= ulSkipBack;
		}

	private:

		BYTE *pBuffer;
		BYTE *pCur;
		BYTE *pEnd;
	};

	void ReadImage(BYTE* pHeaderBuffer, unsigned long ulHeaderBufferLen, BYTE* pImageBuffer, unsigned long ulImageBufferLen, BYTE** ppDstBuffer, unsigned long* pulWidth, unsigned long* pulHeight);
};

#endif //_METAFILE_WMF_EMF_COMMON_H