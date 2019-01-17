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
#include <stdlib.h>
#include <limits.h>
#include "List.h"
#include "JArithmeticDecoder.h"
#include "JBIG2Stream.h"

//~ share these tables
#include "CCITT-Tables.h"

namespace PdfReader
{
	static int c_arrContextSize[4]    ={ 16, 13, 10, 10 };
	static int c_arrRefContextSize[2] ={ 13, 10 };

	//------------------------------------------------------------------------
	// JBIG2HuffmanTable
	//------------------------------------------------------------------------

#define jbig2HuffmanLOW 0xfffffffd
#define jbig2HuffmanOOB 0xfffffffe
#define jbig2HuffmanEOT 0xffffffff

	struct JBIG2HuffmanTable
	{
		int          nValue;
		unsigned int unPrefixLen;
		unsigned int unRangeLen;  // can also be LOW, OOB, or EOT
		unsigned int unPrefix;
	};

	JBIG2HuffmanTable c_oHuffTableA[] =
	{
		{ 0, 1, 4, 0x000 },
		{ 16, 2, 8, 0x002 },
		{ 272, 3, 16, 0x006 },
		{ 65808, 3, 32, 0x007 },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableB[] =
	{
		{ 0, 1, 0, 0x000 },
		{ 1, 2, 0, 0x002 },
		{ 2, 3, 0, 0x006 },
		{ 3, 4, 3, 0x00e },
		{ 11, 5, 6, 0x01e },
		{ 75, 6, 32, 0x03e },
		{ 0, 6, jbig2HuffmanOOB, 0x03f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableC[] =
	{
		{ 0, 1, 0, 0x000 },
		{ 1, 2, 0, 0x002 },
		{ 2, 3, 0, 0x006 },
		{ 3, 4, 3, 0x00e },
		{ 11, 5, 6, 0x01e },
		{ 0, 6, jbig2HuffmanOOB, 0x03e },
		{ 75, 7, 32, 0x0fe },
		{ -256, 8, 8, 0x0fe },
		{ -257, 8, jbig2HuffmanLOW, 0x0ff },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableD[] =
	{
		{ 1, 1, 0, 0x000 },
		{ 2, 2, 0, 0x002 },
		{ 3, 3, 0, 0x006 },
		{ 4, 4, 3, 0x00e },
		{ 12, 5, 6, 0x01e },
		{ 76, 5, 32, 0x01f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableE[] =
	{
		{ 1, 1, 0, 0x000 },
		{ 2, 2, 0, 0x002 },
		{ 3, 3, 0, 0x006 },
		{ 4, 4, 3, 0x00e },
		{ 12, 5, 6, 0x01e },
		{ 76, 6, 32, 0x03e },
		{ -255, 7, 8, 0x07e },
		{ -256, 7, jbig2HuffmanLOW, 0x07f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableF[] =
	{
		{ 0, 2, 7, 0x000 },
		{ 128, 3, 7, 0x002 },
		{ 256, 3, 8, 0x003 },
		{ -1024, 4, 9, 0x008 },
		{ -512, 4, 8, 0x009 },
		{ -256, 4, 7, 0x00a },
		{ -32, 4, 5, 0x00b },
		{ 512, 4, 9, 0x00c },
		{ 1024, 4, 10, 0x00d },
		{ -2048, 5, 10, 0x01c },
		{ -128, 5, 6, 0x01d },
		{ -64, 5, 5, 0x01e },
		{ -2049, 6, jbig2HuffmanLOW, 0x03e },
		{ 2048, 6, 32, 0x03f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableG[] =
	{
		{ -512, 3, 8, 0x000 },
		{ 256, 3, 8, 0x001 },
		{ 512, 3, 9, 0x002 },
		{ 1024, 3, 10, 0x003 },
		{ -1024, 4, 9, 0x008 },
		{ -256, 4, 7, 0x009 },
		{ -32, 4, 5, 0x00a },
		{ 0, 4, 5, 0x00b },
		{ 128, 4, 7, 0x00c },
		{ -128, 5, 6, 0x01a },
		{ -64, 5, 5, 0x01b },
		{ 32, 5, 5, 0x01c },
		{ 64, 5, 6, 0x01d },
		{ -1025, 5, jbig2HuffmanLOW, 0x01e },
		{ 2048, 5, 32, 0x01f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableH[] =
	{
		{ 0, 2, 1, 0x000 },
		{ 0, 2, jbig2HuffmanOOB, 0x001 },
		{ 4, 3, 4, 0x004 },
		{ -1, 4, 0, 0x00a },
		{ 22, 4, 4, 0x00b },
		{ 38, 4, 5, 0x00c },
		{ 2, 5, 0, 0x01a },
		{ 70, 5, 6, 0x01b },
		{ 134, 5, 7, 0x01c },
		{ 3, 6, 0, 0x03a },
		{ 20, 6, 1, 0x03b },
		{ 262, 6, 7, 0x03c },
		{ 646, 6, 10, 0x03d },
		{ -2, 7, 0, 0x07c },
		{ 390, 7, 8, 0x07d },
		{ -15, 8, 3, 0x0fc },
		{ -5, 8, 1, 0x0fd },
		{ -7, 9, 1, 0x1fc },
		{ -3, 9, 0, 0x1fd },
		{ -16, 9, jbig2HuffmanLOW, 0x1fe },
		{ 1670, 9, 32, 0x1ff },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableI[] =
	{
		{ 0, 2, jbig2HuffmanOOB, 0x000 },
		{ -1, 3, 1, 0x002 },
		{ 1, 3, 1, 0x003 },
		{ 7, 3, 5, 0x004 },
		{ -3, 4, 1, 0x00a },
		{ 43, 4, 5, 0x00b },
		{ 75, 4, 6, 0x00c },
		{ 3, 5, 1, 0x01a },
		{ 139, 5, 7, 0x01b },
		{ 267, 5, 8, 0x01c },
		{ 5, 6, 1, 0x03a },
		{ 39, 6, 2, 0x03b },
		{ 523, 6, 8, 0x03c },
		{ 1291, 6, 11, 0x03d },
		{ -5, 7, 1, 0x07c },
		{ 779, 7, 9, 0x07d },
		{ -31, 8, 4, 0x0fc },
		{ -11, 8, 2, 0x0fd },
		{ -15, 9, 2, 0x1fc },
		{ -7, 9, 1, 0x1fd },
		{ -32, 9, jbig2HuffmanLOW, 0x1fe },
		{ 3339, 9, 32, 0x1ff },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableJ[] =
	{
		{ -2, 2, 2, 0x000 },
		{ 6, 2, 6, 0x001 },
		{ 0, 2, jbig2HuffmanOOB, 0x002 },
		{ -3, 5, 0, 0x018 },
		{ 2, 5, 0, 0x019 },
		{ 70, 5, 5, 0x01a },
		{ 3, 6, 0, 0x036 },
		{ 102, 6, 5, 0x037 },
		{ 134, 6, 6, 0x038 },
		{ 198, 6, 7, 0x039 },
		{ 326, 6, 8, 0x03a },
		{ 582, 6, 9, 0x03b },
		{ 1094, 6, 10, 0x03c },
		{ -21, 7, 4, 0x07a },
		{ -4, 7, 0, 0x07b },
		{ 4, 7, 0, 0x07c },
		{ 2118, 7, 11, 0x07d },
		{ -5, 8, 0, 0x0fc },
		{ 5, 8, 0, 0x0fd },
		{ -22, 8, jbig2HuffmanLOW, 0x0fe },
		{ 4166, 8, 32, 0x0ff },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableK[] =
	{
		{ 1, 1, 0, 0x000 },
		{ 2, 2, 1, 0x002 },
		{ 4, 4, 0, 0x00c },
		{ 5, 4, 1, 0x00d },
		{ 7, 5, 1, 0x01c },
		{ 9, 5, 2, 0x01d },
		{ 13, 6, 2, 0x03c },
		{ 17, 7, 2, 0x07a },
		{ 21, 7, 3, 0x07b },
		{ 29, 7, 4, 0x07c },
		{ 45, 7, 5, 0x07d },
		{ 77, 7, 6, 0x07e },
		{ 141, 7, 32, 0x07f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableL[] =
	{
		{ 1, 1, 0, 0x000 },
		{ 2, 2, 0, 0x002 },
		{ 3, 3, 1, 0x006 },
		{ 5, 5, 0, 0x01c },
		{ 6, 5, 1, 0x01d },
		{ 8, 6, 1, 0x03c },
		{ 10, 7, 0, 0x07a },
		{ 11, 7, 1, 0x07b },
		{ 13, 7, 2, 0x07c },
		{ 17, 7, 3, 0x07d },
		{ 25, 7, 4, 0x07e },
		{ 41, 8, 5, 0x0fe },
		{ 73, 8, 32, 0x0ff },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableM[] =
	{
		{ 1, 1, 0, 0x000 },
		{ 2, 3, 0, 0x004 },
		{ 7, 3, 3, 0x005 },
		{ 3, 4, 0, 0x00c },
		{ 5, 4, 1, 0x00d },
		{ 4, 5, 0, 0x01c },
		{ 15, 6, 1, 0x03a },
		{ 17, 6, 2, 0x03b },
		{ 21, 6, 3, 0x03c },
		{ 29, 6, 4, 0x03d },
		{ 45, 6, 5, 0x03e },
		{ 77, 7, 6, 0x07e },
		{ 141, 7, 32, 0x07f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableN[] =
	{
		{ 0, 1, 0, 0x000 },
		{ -2, 3, 0, 0x004 },
		{ -1, 3, 0, 0x005 },
		{ 1, 3, 0, 0x006 },
		{ 2, 3, 0, 0x007 },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	JBIG2HuffmanTable c_oHuffTableO[] =
	{
		{ 0, 1, 0, 0x000 },
		{ -1, 3, 0, 0x004 },
		{ 1, 3, 0, 0x005 },
		{ -2, 4, 0, 0x00c },
		{ 2, 4, 0, 0x00d },
		{ -4, 5, 1, 0x01c },
		{ 3, 5, 1, 0x01d },
		{ -8, 6, 2, 0x03c },
		{ 5, 6, 2, 0x03d },
		{ -24, 7, 4, 0x07c },
		{ 9, 7, 4, 0x07d },
		{ -25, 7, jbig2HuffmanLOW, 0x07e },
		{ 25, 7, 32, 0x07f },
		{ 0, 0, jbig2HuffmanEOT, 0 }
	};

	//------------------------------------------------------------------------
	// JBIG2HuffmanDecoder
	//------------------------------------------------------------------------

	class JBIG2HuffmanDecoder
	{
	public:

		JBIG2HuffmanDecoder()
		{
			m_pStream = NULL;
			Reset();
		}

		~JBIG2HuffmanDecoder()
		{
		}
		void SetStream(Stream *pStream)
		{
			m_pStream = pStream;
		}
		void Reset()
		{
			unBuffer = 0;
			unBufferLen = 0;
		}

		// Returns false for OOB, otherwise sets *<pnValue> and returns true.
		//~ optimize this
		bool DecodeInt(int *pnValue, JBIG2HuffmanTable *pTable)
		{
			unsigned int unIndex = 0, unPrefix = 0, unLen = 0;
			while (pTable[unIndex].unRangeLen != jbig2HuffmanEOT)
			{
				while (unLen < pTable[unIndex].unPrefixLen)
				{
					unPrefix = (unPrefix << 1) | ReadBit();
					++unLen;
				}
				if (unPrefix == pTable[unIndex].unPrefix)
				{
					if (pTable[unIndex].unRangeLen == jbig2HuffmanOOB)
					{
						return false;
					}
					if (pTable[unIndex].unRangeLen == jbig2HuffmanLOW)
					{
						*pnValue = pTable[unIndex].nValue - ReadBits(32);
					}
					else if (pTable[unIndex].unRangeLen > 0)
					{
						*pnValue = pTable[unIndex].nValue + ReadBits(pTable[unIndex].unRangeLen);
					}
					else
					{
						*pnValue = pTable[unIndex].nValue;
					}
					return true;
				}
				++unIndex;
			}
			return false;
		}
		unsigned int ReadBits(unsigned int unCount)
		{
			unsigned int unMask = (unCount == 32) ? 0xffffffff : ((1 << unCount) - 1);
			unsigned int unValue;
			if (unBufferLen >= unCount)
			{
				unValue = (unBuffer >> (unBufferLen - unCount)) & unMask;
				unBufferLen -= unCount;
			}
			else
			{
				unValue = unBuffer & ((1 << unBufferLen) - 1);
				unsigned int unLeftCount = unCount - unBufferLen;
				unBufferLen = 0;
				while (unLeftCount >= 8)
				{
					unValue = (unValue << 8) | (m_pStream->GetChar() & 0xff);
					unLeftCount -= 8;
				}
				if (unLeftCount > 0)
				{
					unBuffer = m_pStream->GetChar();
					unBufferLen = 8 - unLeftCount;
					unValue = (unValue << unLeftCount) | ((unBuffer >> unBufferLen) & ((1 << unLeftCount) - 1));
				}
			}
			return unValue;
		}

		unsigned int ReadBit()
		{
			if (unBufferLen == 0)
			{
				unBuffer = m_pStream->GetChar();
				unBufferLen = 8;
			}

			--unBufferLen;
			return (unBuffer >> unBufferLen) & 1;
		}


		// Sort the table by prefix length and assign prefix values.
		void BuildTable(JBIG2HuffmanTable *pTable, unsigned int unLen)
		{
			// stable selection sort:
			// - entries with prefixLen > 0, in ascending prefixLen order
			// - entry with prefixLen = 0, rangeLen = EOT
			// - all other entries with prefixLen = 0
			// (on entry, table[len] has prefixLen = 0, rangeLen = EOT)

			unsigned int unI, unJ, unK;
			for (unI = 0; unI < unLen; ++unI)
			{
				for (unJ = unI; unJ < unLen && pTable[unJ].unPrefixLen == 0; ++unJ);
				if (unJ == unLen)
				{
					break;
				}
				for (unK = unJ + 1; unK < unLen; ++unK)
				{
					if (pTable[unK].unPrefixLen > 0 && pTable[unK].unPrefixLen < pTable[unJ].unPrefixLen)
					{
						unJ = unK;
					}
				}
				if (unJ != unI)
				{
					JBIG2HuffmanTable oTable = pTable[unJ];
					for (unK = unJ; unK > unI; --unK)
					{
						pTable[unK] = pTable[unK - 1];
					}
					pTable[unI] = oTable;
				}
			}
			pTable[unI] = pTable[unLen];

			// assign prefixes
			unI = 0;
			unsigned int unPrefix = 0;
			pTable[unI++].unPrefix = unPrefix++;
			for (; pTable[unI].unRangeLen != jbig2HuffmanEOT; ++unI)
			{
				unPrefix <<= pTable[unI].unPrefixLen - pTable[unI - 1].unPrefixLen;
				pTable[unI].unPrefix = unPrefix++;
			}
		}


	private:

		Stream      *m_pStream;
		unsigned int unBuffer;
		unsigned int unBufferLen;
	};

	//------------------------------------------------------------------------
	// JBIG2MMRDecoder
	//------------------------------------------------------------------------

	class JBIG2MMRDecoder
	{
	public:

		JBIG2MMRDecoder()
		{
			m_pStream = NULL;
			Reset();
		}
		~JBIG2MMRDecoder()
		{
		}
		void SetStream(Stream *pStream)
		{
			m_pStream = pStream;
		}
		void Reset()
		{
			unBuffer         = 0;
			unBufferLen      = 0;
			unBytesReadCount = 0;
		}
		int  Get2DCode()
		{
			CCITTCode *pCode = NULL;

			if (unBufferLen == 0)
			{
				unBuffer = m_pStream->GetChar() & 0xff;
				unBufferLen = 8;
				++unBytesReadCount;
				pCode = &c_arrTable2D[(unBuffer >> 1) & 0x7f];
			}
			else if (unBufferLen == 8)
			{
				pCode = &c_arrTable2D[(unBuffer >> 1) & 0x7f];
			}
			else
			{
				pCode = &c_arrTable2D[(unBuffer << (7 - unBufferLen)) & 0x7f];
				if (pCode->nBitsCount < 0 || pCode->nBitsCount >(int)unBufferLen)
				{
					unBuffer = (unBuffer << 8) | (m_pStream->GetChar() & 0xff);
					unBufferLen += 8;
					++unBytesReadCount;
					pCode = &c_arrTable2D[(unBuffer >> (unBufferLen - 7)) & 0x7f];
				}
			}

			if (pCode->nBitsCount < 0)
			{
				return 0;
			}
			unBufferLen -= pCode->nBitsCount;
			return pCode->nCode;
		}

		int  GetWhiteCode()
		{
			if (unBufferLen == 0)
			{
				unBuffer = m_pStream->GetChar() & 0xff;
				unBufferLen = 8;
				++unBytesReadCount;
			}
			while (1)
			{
				CCITTCode *pCode = NULL;
				if (unBufferLen >= 7 && ((unBuffer >> (unBufferLen - 7)) & 0x7f) == 0)
				{
					unsigned int unCode;
					if (unBufferLen <= 12)
					{
						unCode = unBuffer << (12 - unBufferLen);
					}
					else
					{
						unCode = unBuffer >> (unBufferLen - 12);
					}
					pCode = &c_arrWhiteTable1[unCode & 0x1f];
				}
				else
				{
					unsigned int unCode;
					if (unBufferLen <= 9)
					{
						unCode = unBuffer << (9 - unBufferLen);
					}
					else
					{
						unCode = unBuffer >> (unBufferLen - 9);
					}
					pCode = &c_arrWhiteTable2[unCode & 0x1ff];
				}

				if (pCode->nBitsCount > 0 && pCode->nBitsCount <= (int)unBufferLen)
				{
					unBufferLen -= pCode->nBitsCount;
					return pCode->nCode;
				}
				if (unBufferLen >= 12)
				{
					break;
				}

				unBuffer = (unBuffer << 8) | (m_pStream->GetChar() & 0xff);
				unBufferLen += 8;
				++unBytesReadCount;
			}
			// eat a bit and return a positive number so that the caller doesn't
			// go into an infinite loop
			--unBufferLen;
			return 1;
		}
		int  GetBlackCode()
		{
			if (unBufferLen == 0)
			{
				unBuffer = m_pStream->GetChar() & 0xff;
				unBufferLen = 8;
				++unBytesReadCount;
			}
			while (1)
			{
				CCITTCode *pCode = NULL;
				if (unBufferLen >= 6 && ((unBuffer >> (unBufferLen - 6)) & 0x3f) == 0)
				{
					unsigned int unCode;
					if (unBufferLen <= 13)
					{
						unCode = unBuffer << (13 - unBufferLen);
					}
					else
					{
						unCode = unBuffer >> (unBufferLen - 13);
					}
					pCode = &c_arrBlackTable1[unCode & 0x7f];
				}
				else if (unBufferLen >= 4 && ((unBuffer >> (unBufferLen - 4)) & 0x0f) == 0)
				{
					unsigned int unCode;
					if (unBufferLen <= 12)
					{
						unCode = unBuffer << (12 - unBufferLen);
					}
					else
					{
						unCode = unBuffer >> (unBufferLen - 12);
					}
					pCode = &c_arrBlackTable2[(unCode & 0xff) - 64];
				}
				else
				{
					unsigned int unCode;
					if (unBufferLen <= 6)
					{
						unCode = unBuffer << (6 - unBufferLen);
					}
					else
					{
						unCode = unBuffer >> (unBufferLen - 6);
					}
					pCode = &c_arrBlackTable3[unCode & 0x3f];
				}

				if (pCode->nBitsCount > 0 && pCode->nBitsCount <= (int)unBufferLen)
				{
					unBufferLen -= pCode->nBitsCount;
					return pCode->nCode;
				}

				if (unBufferLen >= 13)
				{
					break;
				}
				unBuffer = (unBuffer << 8) | (m_pStream->GetChar() & 0xff);
				unBufferLen += 8;
				++unBytesReadCount;
			}
			// eat a bit and return a positive number so that the caller doesn't
			// go into an infinite loop
			--unBufferLen;
			return 1;
		}

		unsigned int Get24Bits()
		{
			while (unBufferLen < 24)
			{
				unBuffer = (unBuffer << 8) | (m_pStream->GetChar() & 0xff);
				unBufferLen += 8;
				++unBytesReadCount;
			}
			return (unBuffer >> (unBufferLen - 24)) & 0xffffff;
		}
		void SkipTo(unsigned int unLength)
		{
			while (unBytesReadCount < unLength)
			{
				m_pStream->GetChar();
				++unBytesReadCount;
			}
		}


	private:

		Stream      *m_pStream;
		unsigned int unBuffer;
		unsigned int unBufferLen;
		unsigned int unBytesReadCount;
	};

	//------------------------------------------------------------------------
	// JBIG2Segment
	//------------------------------------------------------------------------

	enum JBIG2SegmentType
	{
		jbig2SegBitmap,
		jbig2SegSymbolDict,
		jbig2SegPatternDict,
		jbig2SegCodeTable
	};

	class JBIG2Segment
	{
	public:

		JBIG2Segment(unsigned int unSegNum)
		{
			m_unSegNum = unSegNum;
		}
		virtual ~JBIG2Segment()
		{
		}
		void SetSegNum(unsigned int unSegNum)
		{
			m_unSegNum = unSegNum;
		}
		unsigned int GetSegNum()
		{
			return m_unSegNum;
		}
		virtual JBIG2SegmentType GetType() = 0;

	private:

		unsigned int m_unSegNum;
	};

	//------------------------------------------------------------------------
	// JBIG2Bitmap
	//------------------------------------------------------------------------

	struct JBIG2BitmapPtr
	{
		unsigned char *pDataPtr;
		int            nShift;
		int            nX;
	};

	class JBIG2Bitmap : public JBIG2Segment
	{
	public:

		JBIG2Bitmap(unsigned int unSegNum, int nW, int nH) : JBIG2Segment(unSegNum)
		{
			m_nW    = nW;
			m_nH    = nH;
			m_nLine = (nW + 7) >> 3;

			if (m_nW <= 0 || m_nH <= 0 || m_nLine <= 0 || m_nH >= (INT_MAX - 1) / m_nLine)
			{
				m_pData = NULL;
				return;
			}

			// need to allocate one extra guard byte for use in Combine()
			m_pData = (unsigned char *)MemUtilsMalloc(m_nH * m_nLine + 1);
			m_pData[m_nH * m_nLine] = 0;
		}
		virtual ~JBIG2Bitmap()
		{
			MemUtilsFree(m_pData);
		}

		virtual JBIG2SegmentType GetType()
		{
			return jbig2SegBitmap;
		}
		JBIG2Bitmap *Copy()
		{
			return new JBIG2Bitmap(0, this);
		}
		//~ optimize this
		JBIG2Bitmap *GetSlice(unsigned int unX, unsigned int unY, unsigned int unW, unsigned int unH)
		{
			JBIG2Bitmap *pSlice = new JBIG2Bitmap(0, unW, unH);
			pSlice->ClearToZero();

			for (unsigned int unCurY = 0; unCurY < unH; ++unCurY)
			{
				for (unsigned int unCurX = 0; unCurX < unW; ++unCurX)
				{
					if (GetPixel(unX + unCurX, unY + unCurY))
					{
						pSlice->SetPixel(unCurX, unCurY);
					}
				}
			}

			return pSlice;
		}
		void Expand(int nNewH, unsigned int unPixel)
		{
			if (nNewH <= m_nH || m_nLine <= 0 || nNewH >= (INT_MAX - 1) / m_nLine)
			{
				return;
			}

			// need to allocate one extra guard byte for use in combine()
			m_pData = (unsigned char *)MemUtilsRealloc(m_pData, nNewH * m_nLine + 1);
			if (unPixel)
			{
				memset(m_pData + m_nH * m_nLine, 0xff, (nNewH - m_nH) * m_nLine);
			}
			else
			{
				memset(m_pData + m_nH * m_nLine, 0x00, (nNewH - m_nH) * m_nLine);
			}

			m_nH = nNewH;
			m_pData[m_nH * m_nLine] = 0;
		}

		void ClearToZero()
		{
			memset(m_pData, 0, m_nH * m_nLine);
		}
		void ClearToOne()
		{
			memset(m_pData, 0xff, m_nH * m_nLine);
		}
		int  GetWidth()
		{
			return m_nW;
		}
		int  GetHeight()
		{
			return m_nH;
		}
		int  GetPixel(int nX, int nY)
		{
			return (nX < 0 || nX >= m_nW || nY < 0 || nY >= m_nH) ? 0 : (m_pData[nY * m_nLine + (nX >> 3)] >> (7 - (nX & 7))) & 1;
		}
		void SetPixel(int nX, int nY)
		{
			m_pData[nY * m_nLine + (nX >> 3)] |= 1 << (7 - (nX & 7));
		}
		void ClearPixel(int nX, int nY)
		{
			m_pData[nY * m_nLine + (nX >> 3)] &= 0x7f7f >> (nX & 7);
		}
		inline void GetPixelPtr(int nX, int nY, JBIG2BitmapPtr *pPtr)
		{
			if (nY < 0 || nY >= m_nH || nX >= m_nW)
			{
				pPtr->pDataPtr = NULL;
			}
			else if (nX < 0)
			{
				pPtr->pDataPtr = &m_pData[nY * m_nLine];
				pPtr->nShift   = 7;
				pPtr->nX       = nX;
			}
			else
			{
				pPtr->pDataPtr = &m_pData[nY * m_nLine + (nX >> 3)];
				pPtr->nShift   = 7 - (nX & 7);
				pPtr->nX       = nX;
			}
		}
		inline int  NextPixel(JBIG2BitmapPtr *pPtr)
		{
			int nPixel;

			if (!pPtr->pDataPtr)
			{
				nPixel = 0;
			}
			else if (pPtr->nX < 0)
			{
				++pPtr->nX;
				nPixel = 0;
			}
			else
			{
				nPixel = (*pPtr->pDataPtr >> pPtr->nShift) & 1;
				if (++pPtr->nX == m_nW)
				{
					pPtr->pDataPtr = NULL;
				}
				else if (pPtr->nShift == 0)
				{
					++pPtr->pDataPtr;
					pPtr->nShift = 7;
				}
				else
				{
					--pPtr->nShift;
				}
			}
			return nPixel;
		}

		void DuplicateRow(int nDstY, int nSrcY)
		{
			memcpy(m_pData + nDstY * m_nLine, m_pData + nSrcY * m_nLine, m_nLine);
		}
		void Combine(JBIG2Bitmap *pBitmap, int nX, int nY, unsigned int unCombOp)
		{
			int nY0;
			if (nY < 0)
			{
				nY0 = -nY;
			}
			else
			{
				nY0 = 0;
			}

			int nY1;
			if (nY + pBitmap->m_nH > m_nH)
			{
				nY1 = m_nH - nY;
			}
			else
			{
				nY1 = pBitmap->m_nH;
			}

			if (nY0 >= nY1)
			{
				return;
			}

			int nX0;
			if (nX >= 0)
			{
				nX0 = nX & ~7;
			}
			else
			{
				nX0 = 0;
			}

			int nX1 = nX + pBitmap->m_nW;
			if (nX1 > m_nW)
			{
				nX1 = m_nW;
			}
			if (nX0 >= nX1)
			{
				return;
			}

			unsigned int unS1 = nX & 7;
			unsigned int unS2 = 8 - unS1;

			unsigned int unM1 =  0xff >> (nX1 & 7);
			unsigned int unM2 =  0xff << (((nX1 & 7) == 0) ? 0 : 8 - (nX1 & 7));
			unsigned int unM3 = (0xff >> unS1) & unM2;

			bool bOneByte = (nX0 == ((nX1 - 1) & ~7));

			unsigned char *pSrcPtr, *pDstPtr;
			for (int nCurY = nY0; nCurY < nY1; ++nCurY)
			{
				// one byte per line -- need to mask both left and right side
				if (bOneByte)
				{
					unsigned int unSrc1, unDst;
					if (nX >= 0)
					{
						pDstPtr = m_pData + (nY + nCurY) * m_nLine + (nX >> 3);
						pSrcPtr = pBitmap->m_pData + nCurY * pBitmap->m_nLine;

						unDst  = *pDstPtr;
						unSrc1 = *pSrcPtr;

						switch (unCombOp)
						{
							case 0: // or
							unDst |= (unSrc1 >> unS1) & unM2;
							break;
							case 1: // and
							unDst &= ((0xff00 | unSrc1) >> unS1) | unM1;
							break;
							case 2: // xor
							unDst ^= (unSrc1 >> unS1) & unM2;
							break;
							case 3: // xnor
							unDst ^= ((unSrc1 ^ 0xff) >> unS1) & unM2;
							break;
							case 4: // replace
							unDst = (unDst & ~unM3) | ((unSrc1 >> unS1) & unM3);
							break;
						}
						*pDstPtr = unDst;
					}
					else
					{
						pDstPtr = m_pData + (nY + nCurY) * m_nLine;
						pSrcPtr = pBitmap->m_pData + nCurY * pBitmap->m_nLine + (-nX >> 3);

						unDst  = *pDstPtr;
						unSrc1 = *pSrcPtr;

						switch (unCombOp)
						{
							case 0: // or
							unDst |= unSrc1 & unM2;
							break;
							case 1: // and
							unDst &= unSrc1 | unM1;
							break;
							case 2: // xor
							unDst ^= unSrc1 & unM2;
							break;
							case 3: // xnor
							unDst ^= (unSrc1 ^ 0xff) & unM2;
							break;
							case 4: // replace
							unDst = (unSrc1 & unM2) | (unDst & unM1);
							break;
						}
						*pDstPtr = unDst;
					}
				}
				else // multiple bytes per line -- need to mask left side of left-most byte and right side of right-most byte
				{
					int nCurX;
					unsigned int unSrc0, unSrc1, unSrc, unDst;
					if (nX >= 0) // left-most byte
					{
						pDstPtr = m_pData + (nY + nCurY) * m_nLine + (nX >> 3);
						pSrcPtr = pBitmap->m_pData + nCurY * pBitmap->m_nLine;

						unSrc1 = *pSrcPtr++;
						unDst  = *pDstPtr;

						switch (unCombOp)
						{
							case 0: // or
							unDst |= unSrc1 >> unS1;
							break;
							case 1: // and
							unDst &= (0xff00 | unSrc1) >> unS1;
							break;
							case 2: // xor
							unDst ^= unSrc1 >> unS1;
							break;
							case 3: // xnor
							unDst ^= (unSrc1 ^ 0xff) >> unS1;
							break;
							case 4: // replace
							unDst = (unDst & (0xff << unS2)) | (unSrc1 >> unS1);
							break;
						}
						*pDstPtr++ = unDst;
						nCurX = nX0 + 8;
					}
					else
					{
						pDstPtr = m_pData + (nY + nCurY) * m_nLine;
						pSrcPtr = pBitmap->m_pData + nCurY * pBitmap->m_nLine + (-nX >> 3);

						unSrc1 = *pSrcPtr++;
						nCurX = nX0;
					}

					// middle bytes
					for (; nCurX < nX1 - 8; nCurX += 8)
					{
						unDst  = *pDstPtr;
						unSrc0 = unSrc1;
						unSrc1 = *pSrcPtr++;
						unSrc = (((unSrc0 << 8) | unSrc1) >> unS1) & 0xff;

						switch (unCombOp)
						{
							case 0: // or
							unDst |= unSrc;
							break;
							case 1: // and
							unDst &= unSrc;
							break;
							case 2: // xor
							unDst ^= unSrc;
							break;
							case 3: // xnor
							unDst ^= unSrc ^ 0xff;
							break;
							case 4: // replace
							unDst = unSrc;
							break;
						}
						*pDstPtr++ = unDst;
					}

					// right-most byte
					// note: this last byte (src1) may not actually be used, depending
					// on the values of s1, m1, and m2 - and in fact, it may be off
					// the edge of the source bitmap, which means we need to allocate
					// one extra guard byte at the end of each bitmap
					unDst  = *pDstPtr;
					unSrc0 = unSrc1;
					unSrc1 = *pSrcPtr++;
					unSrc  = (((unSrc0 << 8) | unSrc1) >> unS1) & 0xff;

					switch (unCombOp)
					{
						case 0: // or
						unDst |= unSrc & unM2;
						break;
						case 1: // and
						unDst &= unSrc | unM1;
						break;
						case 2: // xor
						unDst ^= unSrc & unM2;
						break;
						case 3: // xnor
						unDst ^= (unSrc ^ 0xff) & unM2;
						break;
						case 4: // replace
						unDst = (unSrc & unM2) | (unDst & unM1);
						break;
					}
					*pDstPtr = unDst;
				}
			}
		}

		unsigned char *GetDataPtr()
		{
			return m_pData;
		}
		int GetDataSize()
		{
			return m_nH * m_nLine;
		}

	private:

		JBIG2Bitmap(unsigned int unSegNum, JBIG2Bitmap *pBitmap) : JBIG2Segment(unSegNum)
		{
			m_nW    = pBitmap->m_nW;
			m_nH    = pBitmap->m_nH;
			m_nLine = pBitmap->m_nLine;

			if (m_nW <= 0 || m_nH <= 0 || m_nLine <= 0 || m_nH >= (INT_MAX - 1) / m_nLine)
			{
				m_pData = NULL;
				return;
			}

			// need to allocate one extra guard byte for use in Combine()
			m_pData = (unsigned char *)MemUtilsMalloc(m_nH * m_nLine + 1);
			memcpy(m_pData, pBitmap->m_pData, m_nH * m_nLine);
			m_pData[m_nH * m_nLine] = 0;
		}

	private:

		int            m_nW;
		int            m_nH;
		int            m_nLine;
		unsigned char *m_pData;
	};

	//------------------------------------------------------------------------
	// JBIG2SymbolDict
	//------------------------------------------------------------------------

	class JBIG2SymbolDict : public JBIG2Segment
	{
	public:

		JBIG2SymbolDict(unsigned int unSegNum, unsigned int unSize) : JBIG2Segment(unSegNum)
		{
			m_unSize    = unSize;
			m_ppBitmaps = (JBIG2Bitmap **)MemUtilsMallocArray(m_unSize, sizeof(JBIG2Bitmap *));

			m_pGenericRegionStats    = NULL;
			m_pRefinementRegionStats = NULL;
		}
		virtual ~JBIG2SymbolDict()
		{
			for (unsigned int unIndex = 0; unIndex < m_unSize; ++unIndex)
			{
				delete m_ppBitmaps[unIndex];
			}

			MemUtilsFree(m_ppBitmaps);

			if (m_pGenericRegionStats)
			{
				delete m_pGenericRegionStats;
			}
			if (m_pRefinementRegionStats)
			{
				delete m_pRefinementRegionStats;
			}
		}
		virtual JBIG2SegmentType GetType()
		{
			return jbig2SegSymbolDict;
		}
		unsigned int GetSize()
		{
			return m_unSize;
		}
		void SetBitmap(unsigned int unIndex, JBIG2Bitmap *pBitmap)
		{
			m_ppBitmaps[unIndex] = pBitmap;
		}
		JBIG2Bitmap *GetBitmap(unsigned int unIndex)
		{
			return m_ppBitmaps[unIndex];
		}
		void SetGenericRegionStats(JArithmeticDecoderStats *pStats)
		{
			m_pGenericRegionStats = pStats;
		}
		void SetRefinementRegionStats(JArithmeticDecoderStats *pStats)
		{
			m_pRefinementRegionStats = pStats;
		}
		JArithmeticDecoderStats *GetGenericRegionStats()
		{
			return m_pGenericRegionStats;
		}
		JArithmeticDecoderStats *GetRefinementRegionStats()
		{
			return m_pRefinementRegionStats;
		}

	private:

		unsigned int             m_unSize;
		JBIG2Bitmap            **m_ppBitmaps;
		JArithmeticDecoderStats *m_pGenericRegionStats;
		JArithmeticDecoderStats *m_pRefinementRegionStats;
	};

	//------------------------------------------------------------------------
	// JBIG2PatternDict
	//------------------------------------------------------------------------

	class JBIG2PatternDict : public JBIG2Segment
	{
	public:

		JBIG2PatternDict(unsigned int unSegNum, unsigned int unSize) : JBIG2Segment(unSegNum)
		{
			m_unSize    = unSize;
			m_ppBitmaps = (JBIG2Bitmap **)MemUtilsMallocArray(m_unSize, sizeof(JBIG2Bitmap *));
		}
		virtual ~JBIG2PatternDict()
		{
			for (unsigned int unIndex = 0; unIndex < m_unSize; ++unIndex)
			{
				delete m_ppBitmaps[unIndex];
			}
			MemUtilsFree(m_ppBitmaps);
		}
		virtual JBIG2SegmentType GetType() { return jbig2SegPatternDict; }
		unsigned int GetSize()
		{
			return m_unSize;
		}
		void SetBitmap(unsigned int unIndex, JBIG2Bitmap *pBitmap)
		{
			m_ppBitmaps[unIndex] = pBitmap;
		}
		JBIG2Bitmap *GetBitmap(unsigned int unIndex)
		{
			return m_ppBitmaps[unIndex];
		}

	private:

		unsigned int  m_unSize;
		JBIG2Bitmap **m_ppBitmaps;
	};

	//------------------------------------------------------------------------
	// JBIG2CodeTable
	//------------------------------------------------------------------------

	class JBIG2CodeTable : public JBIG2Segment
	{
	public:

		JBIG2CodeTable(unsigned int unSegNum, JBIG2HuffmanTable *pTable) : JBIG2Segment(unSegNum)
		{
			m_pTable = pTable;
		}
		virtual ~JBIG2CodeTable()
		{
			MemUtilsFree(m_pTable);
		}
		virtual JBIG2SegmentType GetType()
		{
			return jbig2SegCodeTable;
		}
		JBIG2HuffmanTable *GetHuffTable()
		{
			return m_pTable;
		}

	private:

		JBIG2HuffmanTable *m_pTable;
	};

	//------------------------------------------------------------------------
	// JBIG2Stream
	//------------------------------------------------------------------------

	JBIG2Stream::JBIG2Stream(Stream *pStream, Object *pGlobalsStream) :
		FilterStream(pStream)
	{
		m_pPageBitmap = NULL;

		m_pArithDecoder = new JArithmeticDecoder();

		m_pGenericRegionStats    = new JArithmeticDecoderStats(1 << 1);
		m_pRefinementRegionStats = new JArithmeticDecoderStats(1 << 1);

		m_pIadhStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIadwStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIaexStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIaaiStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIadtStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIaitStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIafsStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIadsStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIardxStats  = new JArithmeticDecoderStats(1 << 9);
		m_pIardyStats  = new JArithmeticDecoderStats(1 << 9);
		m_pIardwStats  = new JArithmeticDecoderStats(1 << 9);
		m_pIardhStats  = new JArithmeticDecoderStats(1 << 9);
		m_pIariStats   = new JArithmeticDecoderStats(1 << 9);
		m_pIaidStats   = new JArithmeticDecoderStats(1 << 1);

		m_pHuffDecoder = new JBIG2HuffmanDecoder();
		m_pMMrDecoder  = new JBIG2MMRDecoder();

		pGlobalsStream->Copy(&m_oGlobalsStream);

		m_pSegments = m_pGlobalSegments = NULL;
		m_pCurStream = NULL;
		m_pDataPtr = m_pDataEnd = NULL;
	}

	JBIG2Stream::~JBIG2Stream()
	{
		Close();
		m_oGlobalsStream.Free();

		delete m_pArithDecoder;
		delete m_pGenericRegionStats;
		delete m_pRefinementRegionStats;
		delete m_pIadhStats;
		delete m_pIadwStats;
		delete m_pIaexStats;
		delete m_pIaaiStats;
		delete m_pIadtStats;
		delete m_pIaitStats;
		delete m_pIafsStats;
		delete m_pIadsStats;
		delete m_pIardxStats;
		delete m_pIardyStats;
		delete m_pIardwStats;
		delete m_pIardhStats;
		delete m_pIariStats;
		delete m_pIaidStats;
		delete m_pHuffDecoder;
		delete m_pMMrDecoder;
		delete m_pStream;
	}

	void JBIG2Stream::Reset()
	{
		// read the globals stream
		m_pGlobalSegments = new CList();
		if (m_oGlobalsStream.IsStream())
		{
			m_pSegments = m_pGlobalSegments;

			m_pCurStream = m_oGlobalsStream.GetStream();
			m_pCurStream->Reset();

			m_pArithDecoder->SetStream(m_pCurStream);
			m_pHuffDecoder->SetStream(m_pCurStream);
			m_pMMrDecoder->SetStream(m_pCurStream);
			ReadSegments();
			m_pCurStream->Close();
		}

		// read the main stream
		m_pSegments = new CList();

		m_pCurStream = m_pStream;
		m_pCurStream->Reset();

		m_pArithDecoder->SetStream(m_pCurStream);
		m_pHuffDecoder->SetStream(m_pCurStream);
		m_pMMrDecoder->SetStream(m_pCurStream);

		ReadSegments();

		if (m_pPageBitmap)
		{
			m_pDataPtr = m_pPageBitmap->GetDataPtr();
			m_pDataEnd = m_pDataPtr + m_pPageBitmap->GetDataSize();
		}
		else
		{
			m_pDataPtr = m_pDataEnd = NULL;
		}
	}

	void JBIG2Stream::Close()
	{
		if (m_pPageBitmap)
		{
			delete m_pPageBitmap;
			m_pPageBitmap = NULL;
		}
		if (m_pSegments)
		{
			DeleteCList(m_pSegments, JBIG2Segment);
			m_pSegments = NULL;
		}
		if (m_pGlobalSegments)
		{
			DeleteCList(m_pGlobalSegments, JBIG2Segment);
			m_pGlobalSegments = NULL;
		}
		m_pDataPtr = m_pDataEnd = NULL;
		FilterStream::Close();
	}

	int JBIG2Stream::GetChar()
	{
		if (m_pDataPtr && m_pDataPtr < m_pDataEnd)
		{
			return (*m_pDataPtr++ ^ 0xff) & 0xff;
		}
		return EOF;
	}

	int JBIG2Stream::LookChar()
	{
		if (m_pDataPtr && m_pDataPtr < m_pDataEnd)
		{
			return (*m_pDataPtr ^ 0xff) & 0xff;
		}
		return EOF;
	}

	StringExt *JBIG2Stream::GetPSFilter(int nPSLevel, char *sIndent)
	{
		return NULL;
	}

	bool JBIG2Stream::IsBinary(bool bLast)
	{
		return m_pStream->IsBinary(true);
	}

	void JBIG2Stream::ReadSegments()
	{
		unsigned int unSegNum;
		while (ReadULong(&unSegNum))
		{
			// segment header flags
			unsigned int unSegFlags;
			if (!ReadUByte(&unSegFlags))
			{
				return;
			}

			unsigned int unSegType = unSegFlags & 0x3f;

			// referred-to segment count and retention flags
			unsigned int unRefFlags;
			if (!ReadUByte(&unRefFlags))
			{
				return;
			}
			unsigned int unRefSegsCount = unRefFlags >> 5;

			if (7 == unRefSegsCount)
			{
				int nChar1, nChar2, nChar3;
				if (EOF == (nChar1 = m_pCurStream->GetChar()) || EOF == (nChar2 = m_pCurStream->GetChar()) || EOF == (nChar3 = m_pCurStream->GetChar()))
				{
					return;
				}
				unRefFlags = (unRefFlags << 24) | (nChar1 << 16) | (nChar2 << 8) | nChar3;
				unRefSegsCount = unRefFlags & 0x1fffffff;
				for (unsigned int unIndex = 0; unIndex < (unRefSegsCount + 9) >> 3; ++unIndex)
				{
					nChar1 = m_pCurStream->GetChar();
				}
			}

			// referred-to segment numbers
			unsigned int *punRefSegs = (unsigned int *)MemUtilsMallocArray(unRefSegsCount, sizeof(unsigned int));
			if (unSegNum <= 256)
			{
				for (unsigned int unIndex = 0; unIndex < unRefSegsCount; ++unIndex)
				{
					if (!ReadUByte(&punRefSegs[unIndex]))
					{
						MemUtilsFree(punRefSegs);
						return;
					}
				}
			}
			else if (unSegNum <= 65536)
			{
				for (unsigned int unIndex = 0; unIndex < unRefSegsCount; ++unIndex)
				{
					if (!ReadUWord(&punRefSegs[unIndex]))
					{
						MemUtilsFree(punRefSegs);
						return;
					}
				}
			}
			else
			{
				for (unsigned int unIndex = 0; unIndex < unRefSegsCount; ++unIndex)
				{
					if (!ReadULong(&punRefSegs[unIndex]))
					{
						MemUtilsFree(punRefSegs);
						return;
					}
				}
			}

			// segment page association
			unsigned int unPage;
			if (unSegFlags & 0x40)
			{
				if (!ReadULong(&unPage))
				{
					MemUtilsFree(punRefSegs);
					return;
				}
			}
			else
			{
				if (!ReadUByte(&unPage))
				{
					MemUtilsFree(punRefSegs);
					return;
				}
			}

			// segment data length
			unsigned int unSegLength;
			if (!ReadULong(&unSegLength))
			{
				MemUtilsFree(punRefSegs);
				return;
			}

			unsigned int unDataStartPos = m_pCurStream->GetPos();

			// read the segment data
			switch (unSegType)
			{
				case 0:
				if (!ReadSymbolDictSegment(unSegNum, unSegLength, punRefSegs, unRefSegsCount))
				{
					MemUtilsFree(punRefSegs);
					return;
				}
				break;
				case 4:
				ReadTextRegionSegment(unSegNum, false, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 6:
				ReadTextRegionSegment(unSegNum, true, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 7:
				ReadTextRegionSegment(unSegNum, true, true, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 16:
				ReadPatternDictSegment(unSegNum, unSegLength);
				break;
				case 20:
				ReadHalftoneRegionSegment(unSegNum, false, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 22:
				ReadHalftoneRegionSegment(unSegNum, true, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 23:
				ReadHalftoneRegionSegment(unSegNum, true, true, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 36:
				ReadGenericRegionSegment(unSegNum, false, false, unSegLength);
				break;
				case 38:
				ReadGenericRegionSegment(unSegNum, true, false, unSegLength);
				break;
				case 39:
				ReadGenericRegionSegment(unSegNum, true, true, unSegLength);
				break;
				case 40:
				ReadGenericRefinementRegionSegment(unSegNum, false, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 42:
				ReadGenericRefinementRegionSegment(unSegNum, true, false, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 43:
				ReadGenericRefinementRegionSegment(unSegNum, true, true, unSegLength, punRefSegs, unRefSegsCount);
				break;
				case 48:
				ReadPageInfoSegment(unSegLength);
				break;
				case 50:
				ReadEndOfStripeSegment(unSegLength);
				break;
				case 52:
				ReadProfilesSegment(unSegLength);
				break;
				case 53:
				ReadCodeTableSegment(unSegNum, unSegLength);
				break;
				case 62:
				ReadExtensionSegment(unSegLength);
				break;
				default:
				for (unsigned int unIndex = 0; unIndex < unSegLength; ++unIndex)
				{
					int nChar;
					if (EOF == (nChar = m_pCurStream->GetChar()))
					{
						MemUtilsFree(punRefSegs);
						return;
					}
				}
				break;
			}

			MemUtilsFree(punRefSegs);

			if (unSegLength != 0xFFFFFFFF && m_pCurStream->GetPos() - unDataStartPos != unSegLength)
				m_pCurStream->SetPos(unDataStartPos + unSegLength);
		}

		return;
	}

	bool JBIG2Stream::ReadSymbolDictSegment(unsigned int unSegNum, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount)
	{
		// symbol dictionary flags
		unsigned int unFlags;
		if (!ReadUWord(&unFlags))
		{
			return false;
		}
		unsigned int unHuff            =  unFlags & 1;
		unsigned int unRefAgg          = (unFlags >> 1) & 1;
		unsigned int unHuffDH          = (unFlags >> 2) & 3;
		unsigned int unHuffDW          = (unFlags >> 4) & 3;
		unsigned int unHuffBMSize      = (unFlags >> 6) & 1;
		unsigned int unHuffAggInst     = (unFlags >> 7) & 1;
		unsigned int unContextUsed     = (unFlags >> 8) & 1;
		unsigned int unContextRetained = (unFlags >> 9) & 1;
		unsigned int unSDTemplate      = (unFlags >> 10) & 3;
		unsigned int unSDRTemplate     = (unFlags >> 12) & 1;

		// symbol dictionary AT flags
		int arrSDATx[4], arrSDATy[4];
		if (!unHuff)
		{
			if (unSDTemplate == 0)
			{
				if (!ReadByte(&arrSDATx[0]) || !ReadByte(&arrSDATy[0]) || !ReadByte(&arrSDATx[1]) || !ReadByte(&arrSDATy[1]) ||
					!ReadByte(&arrSDATx[2]) || !ReadByte(&arrSDATy[2]) || !ReadByte(&arrSDATx[3]) || !ReadByte(&arrSDATy[3]))
				{
					return false;
				}
			}
			else
			{
				if (!ReadByte(&arrSDATx[0]) || !ReadByte(&arrSDATy[0]))
				{
					return false;
				}
			}
		}

		// symbol dictionary refinement AT flags
		int arrSDRATx[2], arrSDRATy[2];
		if (unRefAgg && !unSDRTemplate)
		{
			if (!ReadByte(&arrSDRATx[0]) || !ReadByte(&arrSDRATy[0]) || !ReadByte(&arrSDRATx[1]) || !ReadByte(&arrSDRATy[1]))
			{
				return false;
			}
		}

		// SDNUMEXSYMS and SDNUMNEWSYMS
		unsigned int unNumExSyms, unNumNewSyms;
		if (!ReadULong(&unNumExSyms) || !ReadULong(&unNumNewSyms))
		{
			return false;
		}

		// get referenced segments: input symbol dictionaries and code tables
		CList *pCodeTables = new CList();
		unsigned int unNumInputSyms = 0, unI;
		for (unI = 0; unI < unRefSegsCount; ++unI)
		{
			JBIG2Segment *pSegment = FindSegment(punRefSegs[unI]);
			if (pSegment->GetType() == jbig2SegSymbolDict)
			{
				unNumInputSyms += ((JBIG2SymbolDict *)pSegment)->GetSize();
			}
			else if (pSegment->GetType() == jbig2SegCodeTable)
			{
				pCodeTables->Append(pSegment);
			}
		}

		// compute symbol code length
		unsigned int unSymCodeLen = 0;
		unI = 1;
		while (unI < unNumInputSyms + unNumNewSyms)
		{
			++unSymCodeLen;
			unI <<= 1;
		}

		// get the input symbol bitmaps
		JBIG2Bitmap **ppBitmaps = (JBIG2Bitmap **)MemUtilsMallocArray(unNumInputSyms + unNumNewSyms, sizeof(JBIG2Bitmap *));
		for (unI = 0; unI < unNumInputSyms + unNumNewSyms; ++unI)
		{
			ppBitmaps[unI] = NULL;
		}

		unsigned int unK = 0, unJ;
		JBIG2SymbolDict *pInputSymbolDict = NULL;
		for (unI = 0; unI < unRefSegsCount; ++unI)
		{
			JBIG2Segment *pSegment = FindSegment(punRefSegs[unI]);
			if (pSegment->GetType() == jbig2SegSymbolDict)
			{
				pInputSymbolDict = (JBIG2SymbolDict *)pSegment;
				for (unJ = 0; unJ < pInputSymbolDict->GetSize(); ++unJ)
				{
					ppBitmaps[unK++] = pInputSymbolDict->GetBitmap(unJ);
				}
			}
		}

		// get the Huffman tables
		JBIG2HuffmanTable *pHuffDHTable = NULL, *pHuffDWTable = NULL, *pHuffBMSizeTable = NULL, *pHuffAggInstTable = NULL;
		unI = 0;
		if (unHuff)
		{
			if (unHuffDH == 0)
			{
				pHuffDHTable = c_oHuffTableD;
			}
			else if (unHuffDH == 1)
			{
				pHuffDHTable = c_oHuffTableE;
			}
			else
			{
				pHuffDHTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unI++))->GetHuffTable();
			}

			if (unHuffDW == 0)
			{
				pHuffDWTable = c_oHuffTableB;
			}
			else if (unHuffDW == 1)
			{
				pHuffDWTable = c_oHuffTableC;
			}
			else
			{
				pHuffDWTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unI++))->GetHuffTable();
			}

			if (unHuffBMSize == 0)
			{
				pHuffBMSizeTable = c_oHuffTableA;
			}
			else
			{
				pHuffBMSizeTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unI++))->GetHuffTable();
			}

			if (unHuffAggInst == 0)
			{
				pHuffAggInstTable = c_oHuffTableA;
			}
			else
			{
				pHuffAggInstTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unI++))->GetHuffTable();
			}
		}
		delete pCodeTables;

		// set up the Huffman decoder
		if (unHuff)
		{
			m_pHuffDecoder->Reset();
		}
		else // set up the arithmetic decoder
		{
			if (unContextUsed && pInputSymbolDict)
			{
				ResetGenericStats(unSDTemplate, pInputSymbolDict->GetGenericRegionStats());
			}
			else
			{
				ResetGenericStats(unSDTemplate, NULL);
			}
			ResetIntStats(unSymCodeLen);
			m_pArithDecoder->Start();
		}

		// set up the arithmetic decoder for refinement/aggregation
		if (unRefAgg)
		{
			if (unContextUsed && pInputSymbolDict)
			{
				ResetRefinementStats(unSDRTemplate, pInputSymbolDict->GetRefinementRegionStats());
			}
			else
			{
				ResetRefinementStats(unSDRTemplate, NULL);
			}
		}

		// allocate symbol widths storage
		unsigned int *pSymWidths = NULL;
		if (unHuff && !unRefAgg)
		{
			pSymWidths = (unsigned int *)MemUtilsMallocArray(unNumNewSyms, sizeof(unsigned int));
		}

		unsigned int unSymHeight = 0;
		unI = 0;
		while (unI < unNumNewSyms)
		{
			// read the height class delta height
			int nDh;
			if (unHuff)
			{
				m_pHuffDecoder->DecodeInt(&nDh, pHuffDHTable);
			}
			else
			{
				m_pArithDecoder->DecodeInt(&nDh, m_pIadhStats);
			}
			if (nDh < 0 && (unsigned int)-nDh >= unSymHeight)
			{
				// SyntaxError
				for (unsigned int unIndex = 0; unIndex < unNumNewSyms; ++unIndex)
				{
					if (ppBitmaps[unNumInputSyms + unIndex])
					{
						delete ppBitmaps[unNumInputSyms + unIndex];
					}
				}

				MemUtilsFree(ppBitmaps);
				if (pSymWidths)
				{
					MemUtilsFree(pSymWidths);
				}
				return false;
			}
			unSymHeight += nDh;
			unsigned int unSymWidth = 0, unTotalWidth = 0;
			unJ = unI;

			// read the symbols in this height class
			while (1)
			{
				// read the delta width
				int nDw;
				if (unHuff)
				{
					if (!m_pHuffDecoder->DecodeInt(&nDw, pHuffDWTable))
					{
						break;
					}
				}
				else
				{
					if (!m_pArithDecoder->DecodeInt(&nDw, m_pIadwStats))
					{
						break;
					}
				}
				if (nDw < 0 && (unsigned int)-nDw >= unSymWidth)
				{
					// SyntaxError
					for (unsigned int unIndex = 0; unIndex < unNumNewSyms; ++unIndex)
					{
						if (ppBitmaps[unNumInputSyms + unIndex])
						{
							delete ppBitmaps[unNumInputSyms + unIndex];
						}
					}

					MemUtilsFree(ppBitmaps);
					if (pSymWidths)
					{
						MemUtilsFree(pSymWidths);
					}
					return false;
				}
				unSymWidth += nDw;

				// using a collective bitmap, so don't read a bitmap here
				if (unHuff && !unRefAgg)
				{
					pSymWidths[unI] = unSymWidth;
					unTotalWidth += unSymWidth;
				}
				else if (unRefAgg) // refinement/aggregate coding
				{
					int nRefAggNum;
					if (unHuff)
					{
						if (!m_pHuffDecoder->DecodeInt(&nRefAggNum, pHuffAggInstTable))
						{
							break;
						}
					}
					else
					{
						if (!m_pArithDecoder->DecodeInt(&nRefAggNum, m_pIaaiStats))
						{
							break;
						}
					}
#if 0 //~ This special case was added about a year before the final draft
					//~ of the JBIG2 spec was released.  I have encountered some old
					//~ JBIG2 images that predate it.
					if ( 0 ) 
					{
#else
					if (nRefAggNum == 1)
					{
#endif
						unsigned int unSymID;
						int nRefDX, nRefDY, nBMSize;
						if (unHuff)
						{
							unSymID = m_pHuffDecoder->ReadBits(unSymCodeLen);
							m_pHuffDecoder->DecodeInt(&nRefDX, c_oHuffTableO);
							m_pHuffDecoder->DecodeInt(&nRefDY, c_oHuffTableO);
							m_pHuffDecoder->DecodeInt(&nBMSize, c_oHuffTableA);
							m_pHuffDecoder->Reset();
							m_pArithDecoder->Start();
						}
						else
						{
							unSymID = m_pArithDecoder->DecodeIAID(unSymCodeLen, m_pIaidStats);
							m_pArithDecoder->DecodeInt(&nRefDX, m_pIardxStats);
							m_pArithDecoder->DecodeInt(&nRefDY, m_pIardyStats);
						}
						JBIG2Bitmap *pRefBitmap = ppBitmaps[unSymID];
						ppBitmaps[unNumInputSyms + unI] = ReadGenericRefinementRegion(unSymWidth, unSymHeight, unSDRTemplate, false, pRefBitmap, nRefDX, nRefDY, arrSDRATx, arrSDRATy);
					}
					else //~ do we need to use the bmSize value here (in Huffman mode)?
					{
						ppBitmaps[unNumInputSyms + unI] = ReadTextRegion(unHuff ? true : false, true, unSymWidth, unSymHeight, nRefAggNum, 0, unNumInputSyms + unI, NULL, unSymCodeLen, ppBitmaps, 0, 0, 0, 1, 0, c_oHuffTableF, c_oHuffTableH, c_oHuffTableK, c_oHuffTableO, c_oHuffTableO, c_oHuffTableO, c_oHuffTableO, c_oHuffTableA, unSDRTemplate, arrSDRATx, arrSDRATy);
					}
				}
				else // non-ref/agg coding
				{
					ppBitmaps[unNumInputSyms + unI] = ReadGenericBitmap(false, unSymWidth, unSymHeight, unSDTemplate, false, false, NULL, arrSDATx, arrSDATy, 0);
				}

				++unI;
			}

			// read the collective bitmap
			if (unHuff && !unRefAgg)
			{
				int nBMSize;
				m_pHuffDecoder->DecodeInt(&nBMSize, pHuffBMSizeTable);
				m_pHuffDecoder->Reset();

				JBIG2Bitmap *pCollBitmap;
				if (nBMSize == 0)
				{
					pCollBitmap = new JBIG2Bitmap(0, unTotalWidth, unSymHeight);
					nBMSize = unSymHeight * ((unTotalWidth + 7) >> 3);
					unsigned char *pDataPtr = pCollBitmap->GetDataPtr();
					for (unK = 0; unK < (unsigned int)nBMSize; ++unK)
					{
						*pDataPtr++ = m_pCurStream->GetChar();
					}
				}
				else
				{
					pCollBitmap = ReadGenericBitmap(true, unTotalWidth, unSymHeight, 0, false, false, NULL, NULL, NULL, nBMSize);
				}
				unsigned int unX = 0;
				for (; unJ < unI; ++unJ)
				{
					ppBitmaps[unNumInputSyms + unJ] = pCollBitmap->GetSlice(unX, 0, pSymWidths[unJ], unSymHeight);
					unX += pSymWidths[unJ];
				}
				delete pCollBitmap;
			}
		}

		// create the symbol dict object
		JBIG2SymbolDict *pSymbolDict = new JBIG2SymbolDict(unSegNum, unNumExSyms);

		// exported symbol list
		unI = unJ = 0;
		bool bEx = false;
		while (unI < unNumInputSyms + unNumNewSyms)
		{
			int nRun;
			if (unHuff)
			{
				m_pHuffDecoder->DecodeInt(&nRun, c_oHuffTableA);
			}
			else
			{
				m_pArithDecoder->DecodeInt(&nRun, m_pIaexStats);
			}
			if (bEx)
			{
				for (int nCounter = 0; nCounter < nRun; ++nCounter)
				{
					pSymbolDict->SetBitmap(unJ++, ppBitmaps[unI++]->Copy());
				}
			}
			else
			{
				unI += nRun;
			}
			bEx = !bEx;
		}

		for (unI = 0; unI < unNumNewSyms; ++unI)
		{
			delete ppBitmaps[unNumInputSyms + unI];
		}
		MemUtilsFree(ppBitmaps);
		if (pSymWidths)
		{
			MemUtilsFree(pSymWidths);
		}

		// save the arithmetic decoder stats
		if (!unHuff && unContextRetained)
		{
			pSymbolDict->SetGenericRegionStats(m_pGenericRegionStats->Copy());
			if (unRefAgg)
			{
				pSymbolDict->SetRefinementRegionStats(m_pRefinementRegionStats->Copy());
			}
		}

		// store the new symbol dict
		m_pSegments->Append(pSymbolDict);

		return true;
	}

	void JBIG2Stream::ReadTextRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount)
	{
		// region segment info field
		unsigned int unW, unH, unX, unY, unSegInfoFlags;
		if (!ReadULong(&unW) || !ReadULong(&unH) || !ReadULong(&unX) || !ReadULong(&unY) || !ReadUByte(&unSegInfoFlags))
		{
			return;
		}

		unsigned int unExtCombOp = unSegInfoFlags & 7;

		// rest of the text region header
		unsigned int unFlags;
		if (!ReadUWord(&unFlags))
		{
			return;
		}

		unsigned int unHuff       =  unFlags & 1;
		unsigned int unRefine     = (unFlags >> 1) & 1;
		unsigned int unLogStrips  = (unFlags >> 2) & 3;
		unsigned int unRefCorner  = (unFlags >> 4) & 3;
		unsigned int unTransposed = (unFlags >> 6) & 1;
		unsigned int unCombOp     = (unFlags >> 7) & 3;
		unsigned int unDefPixel   = (unFlags >> 9) & 1;
		int          nSOffset     = (unFlags >> 10) & 0x1f;
		unsigned int unTempl      = (unFlags >> 15) & 1;

		if (nSOffset & 0x10)
		{
			nSOffset |= -1 - 0x0f;
		}

		unsigned int unHuffFS = 0, unHuffDS = 0, unHuffDT = 0, unHuffRDW = 0, unHuffRDH = 0, unHuffRDX = 0, unHuffRDY = 0, unHuffRSize = 0;
		if (unHuff)
		{
			unsigned int unHuffFlags;
			if (!ReadUWord(&unHuffFlags))
			{
				return;
			}
			unHuffFS    =  unHuffFlags & 3;
			unHuffDS    = (unHuffFlags >> 2) & 3;
			unHuffDT    = (unHuffFlags >> 4) & 3;
			unHuffRDW   = (unHuffFlags >> 6) & 3;
			unHuffRDH   = (unHuffFlags >> 8) & 3;
			unHuffRDX   = (unHuffFlags >> 10) & 3;
			unHuffRDY   = (unHuffFlags >> 12) & 3;
			unHuffRSize = (unHuffFlags >> 14) & 1;
		}

		int arrATx[2], arrATy[2];
		if (unRefine && unTempl == 0)
		{
			if (!ReadByte(&arrATx[0]) || !ReadByte(&arrATy[0]) || !ReadByte(&arrATx[1]) || !ReadByte(&arrATy[1]))
			{
				return;
			}
		}

		unsigned int unNumInstances;
		if (!ReadULong(&unNumInstances))
		{
			return;
		}

		// get symbol dictionaries and tables
		CList *pCodeTables = new CList();
		unsigned int unNumSyms = 0;
		unsigned int unIndex;
		for (unIndex = 0; unIndex < unRefSegsCount; ++unIndex)
		{
			JBIG2Segment *pSegment = NULL;
			if ((pSegment = FindSegment(punRefSegs[unIndex])))
			{
				if (pSegment->GetType() == jbig2SegSymbolDict)
				{
					unNumSyms += ((JBIG2SymbolDict *)pSegment)->GetSize();
				}
				else if (pSegment->GetType() == jbig2SegCodeTable)
				{
					pCodeTables->Append(pSegment);
				}
			}
			else
			{
			}
		}

		unsigned int unSymCodeLen = 0;
		unIndex = 1;
		while (unIndex < unNumSyms)
		{
			++unSymCodeLen;
			unIndex <<= 1;
		}

		// get the symbol bitmaps
		JBIG2Bitmap **ppSyms = (JBIG2Bitmap **)MemUtilsMallocArray(unNumSyms, sizeof(JBIG2Bitmap *));
		unsigned int unSymsIndex = 0;
		for (unIndex = 0; unIndex < unRefSegsCount; ++unIndex)
		{
			JBIG2Segment *pSegment = NULL;
			if ((pSegment = FindSegment(punRefSegs[unIndex])))
			{
				if (pSegment->GetType() == jbig2SegSymbolDict)
				{
					JBIG2SymbolDict *pSymbolDict = (JBIG2SymbolDict *)pSegment;
					for (unsigned int unK = 0; unK < pSymbolDict->GetSize(); ++unK)
					{
						ppSyms[unSymsIndex++] = pSymbolDict->GetBitmap(unK);
					}
				}
			}
		}

		// get the Huffman tables
		JBIG2HuffmanTable *pHuffFSTable = NULL, *pHuffDSTable = NULL, *pHuffDTTable = NULL, *pHuffRDWTable = NULL, *pHuffRDHTable = NULL, *pHuffRDXTable =  NULL, *pHuffRDYTable = NULL, *pHuffRSizeTable = NULL;
		unIndex = 0;
		if (unHuff)
		{
			if (unHuffFS == 0)
			{
				pHuffFSTable = c_oHuffTableF;
			}
			else if (unHuffFS == 1)
			{
				pHuffFSTable = c_oHuffTableG;
			}
			else
			{
				pHuffFSTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}
			if (unHuffDS == 0)
			{
				pHuffDSTable = c_oHuffTableH;
			}
			else if (unHuffDS == 1)
			{
				pHuffDSTable = c_oHuffTableI;
			}
			else if (unHuffDS == 2)
			{
				pHuffDSTable = c_oHuffTableJ;
			}
			else
			{
				pHuffDSTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffDT == 0)
			{
				pHuffDTTable = c_oHuffTableK;
			}
			else if (unHuffDT == 1)
			{
				pHuffDTTable = c_oHuffTableL;
			}
			else if (unHuffDT == 2)
			{
				pHuffDTTable = c_oHuffTableM;
			}
			else
			{
				pHuffDTTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffRDW == 0)
			{
				pHuffRDWTable = c_oHuffTableN;
			}
			else if (unHuffRDW == 1)
			{
				pHuffRDWTable = c_oHuffTableO;
			}
			else
			{
				pHuffRDWTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffRDH == 0)
			{
				pHuffRDHTable = c_oHuffTableN;
			}
			else if (unHuffRDH == 1)
			{
				pHuffRDHTable = c_oHuffTableO;
			}
			else
			{
				pHuffRDHTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffRDX == 0)
			{
				pHuffRDXTable = c_oHuffTableN;
			}
			else if (unHuffRDX == 1)
			{
				pHuffRDXTable = c_oHuffTableO;
			}
			else
			{
				pHuffRDXTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffRDY == 0)
			{
				pHuffRDYTable = c_oHuffTableN;
			}
			else if (unHuffRDY == 1)
			{
				pHuffRDYTable = c_oHuffTableO;
			}
			else
			{
				pHuffRDYTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}

			if (unHuffRSize == 0)
			{
				pHuffRSizeTable = c_oHuffTableA;
			}
			else
			{
				pHuffRSizeTable = ((JBIG2CodeTable *)pCodeTables->GetByIndex(unIndex++))->GetHuffTable();
			}
		}
		delete pCodeTables;

		// symbol ID Huffman decoding table
		JBIG2HuffmanTable *pSymCodeTab;
		if (unHuff)
		{
			m_pHuffDecoder->Reset();
			JBIG2HuffmanTable arrRunLengthTab[36];

			for (unIndex = 0; unIndex < 32; ++unIndex)
			{
				arrRunLengthTab[unIndex].nValue      = unIndex;
				arrRunLengthTab[unIndex].unPrefixLen = m_pHuffDecoder->ReadBits(4);
				arrRunLengthTab[unIndex].unRangeLen  = 0;
			}
			arrRunLengthTab[32].nValue      = 0x103;
			arrRunLengthTab[32].unPrefixLen = m_pHuffDecoder->ReadBits(4);
			arrRunLengthTab[32].unRangeLen  = 2;

			arrRunLengthTab[33].nValue      = 0x203;
			arrRunLengthTab[33].unPrefixLen = m_pHuffDecoder->ReadBits(4);
			arrRunLengthTab[33].unRangeLen  = 3;

			arrRunLengthTab[34].nValue      = 0x20b;
			arrRunLengthTab[34].unPrefixLen = m_pHuffDecoder->ReadBits(4);
			arrRunLengthTab[34].unRangeLen  = 7;

			arrRunLengthTab[35].unPrefixLen = 0;
			arrRunLengthTab[35].unRangeLen  = jbig2HuffmanEOT;

			m_pHuffDecoder->BuildTable(arrRunLengthTab, 35);
			pSymCodeTab = (JBIG2HuffmanTable *)MemUtilsMallocArray(unNumSyms + 1, sizeof(JBIG2HuffmanTable));

			for (unIndex = 0; unIndex < unNumSyms; ++unIndex)
			{
				pSymCodeTab[unIndex].nValue     = unIndex;
				pSymCodeTab[unIndex].unRangeLen = 0;
			}
			unIndex = 0;

			int nJ;
			while (unIndex < unNumSyms)
			{
				m_pHuffDecoder->DecodeInt(&nJ, arrRunLengthTab);
				if (nJ > 0x200)
				{
					for (nJ -= 0x200; nJ && unIndex < unNumSyms; --nJ)
					{
						pSymCodeTab[unIndex++].unPrefixLen = 0;
					}
				}
				else if (nJ > 0x100)
				{
					for (nJ -= 0x100; nJ && unIndex < unNumSyms; --nJ)
					{
						pSymCodeTab[unIndex].unPrefixLen = pSymCodeTab[unIndex - 1].unPrefixLen;
						++unIndex;
					}
				}
				else
				{
					pSymCodeTab[unIndex++].unPrefixLen = nJ;
				}
			}
			pSymCodeTab[unNumSyms].unPrefixLen = 0;
			pSymCodeTab[unNumSyms].unRangeLen  = jbig2HuffmanEOT;
			m_pHuffDecoder->BuildTable(pSymCodeTab, unNumSyms);
			m_pHuffDecoder->Reset();
		}
		else // set up the arithmetic decoder
		{
			pSymCodeTab = NULL;
			ResetIntStats(unSymCodeLen);
			m_pArithDecoder->Start();
		}
		if (unRefine)
		{
			ResetRefinementStats(unTempl, NULL);
		}

		JBIG2Bitmap *pBitmap = ReadTextRegion(unHuff ? true : false, unRefine ? true : false, unW, unH, unNumInstances, unLogStrips, unNumSyms, pSymCodeTab, unSymCodeLen, ppSyms, unDefPixel, unCombOp, unTransposed, unRefCorner, nSOffset, pHuffFSTable, pHuffDSTable, pHuffDTTable, pHuffRDWTable, pHuffRDHTable, pHuffRDXTable, pHuffRDYTable, pHuffRSizeTable, unTempl, arrATx, arrATy);

		MemUtilsFree(ppSyms);

		// combine the region bitmap into the page bitmap
		if (bImm)
		{
			if (m_unPageH == 0xffffffff && unY + unH > m_unCurPageH)
			{
				m_pPageBitmap->Expand(unY + unH, m_unPageDefPixel);
			}
			m_pPageBitmap->Combine(pBitmap, unX, unY, unExtCombOp);
			delete pBitmap;
		}
		else // store the region bitmap
		{
			pBitmap->SetSegNum(unSegNum);
			m_pSegments->Append(pBitmap);
		}

		// clean up the Huffman decoder
		if (unHuff)
		{
			MemUtilsFree(pSymCodeTab);
		}

		return;
	}

	JBIG2Bitmap *JBIG2Stream::ReadTextRegion(bool bHuff, bool bRefine, int nW, int nH, unsigned int unNumInstances, unsigned int unLogStrips, int nNumSyms, JBIG2HuffmanTable *pSymCodeTab, unsigned int unSymCodeLen, JBIG2Bitmap **ppSyms, unsigned int unDefPixel, unsigned int unCombOp, unsigned int transposed, unsigned int refCorner, int nSOffset, JBIG2HuffmanTable *pHuffFSTable, JBIG2HuffmanTable *pHuffDSTable, JBIG2HuffmanTable *pHuffDTTable, JBIG2HuffmanTable *pHuffRDWTable, JBIG2HuffmanTable *pHuffRDHTable, JBIG2HuffmanTable *pHuffRDXTable, JBIG2HuffmanTable *pHuffRDYTable, JBIG2HuffmanTable *pHuffRSizeTable, unsigned int unTempl, int *pnATx, int *pnATy)
	{
		unsigned int unStrips = 1 << unLogStrips;

		// allocate the bitmap
		JBIG2Bitmap *pBitmap = new JBIG2Bitmap(0, nW, nH);
		if (unDefPixel)
		{
			pBitmap->ClearToOne();
		}
		else
		{
			pBitmap->ClearToZero();
		}

		// decode initial T value
		int nT;
		if (bHuff)
		{
			m_pHuffDecoder->DecodeInt(&nT, pHuffDTTable);
		}
		else
		{
			m_pArithDecoder->DecodeInt(&nT, m_pIadtStats);
		}
		nT *= -(int)unStrips;

		unsigned int unInstance = 0;
		int nSFirst = 0;
		while (unInstance < unNumInstances)
		{
			// decode delta-T
			int nDeltaT;
			if (bHuff)
			{
				m_pHuffDecoder->DecodeInt(&nDeltaT, pHuffDTTable);
			}
			else
			{
				m_pArithDecoder->DecodeInt(&nDeltaT, m_pIadtStats);
			}
			nT += nDeltaT * unStrips;

			// first S value
			int nDS;
			if (bHuff)
			{
				m_pHuffDecoder->DecodeInt(&nDS, pHuffFSTable);
			}
			else
			{
				m_pArithDecoder->DecodeInt(&nDS, m_pIafsStats);
			}
			nSFirst += nDS;

			int nCurS = nSFirst;

			// read the instances
			while (1)
			{
				// T value
				if (1 == unStrips)
				{
					nDeltaT = 0;
				}
				else if (bHuff)
				{
					nDeltaT = m_pHuffDecoder->ReadBits(unLogStrips);
				}
				else
				{
					m_pArithDecoder->DecodeInt(&nDeltaT, m_pIaitStats);
				}
				int nCurT = nT + nDeltaT;

				// symbol ID
				unsigned int unSymID;
				if (bHuff)
				{
					if (pSymCodeTab)
					{
						int nTemp;
						m_pHuffDecoder->DecodeInt(&nTemp, pSymCodeTab);
						unSymID = (unsigned int)nTemp;
					}
					else
					{
						unSymID = m_pHuffDecoder->ReadBits(unSymCodeLen);
					}
				}
				else
				{
					unSymID = m_pArithDecoder->DecodeIAID(unSymCodeLen, m_pIaidStats);
				}

				if (unSymID >= (unsigned int)nNumSyms)
				{
				}
				else
				{
					// get the symbol bitmap
					JBIG2Bitmap *pSymbolBitmap = NULL;
					int nReadInt;
					if (bRefine)
					{
						if (bHuff)
						{
							nReadInt = (int)m_pHuffDecoder->ReadBit();
						}
						else
						{
							m_pArithDecoder->DecodeInt(&nReadInt, m_pIariStats);
						}
					}
					else
					{
						nReadInt = 0;
					}

					if (nReadInt)
					{
						int nRDW, nRDH, nRDX, nRDY, nBMSize;
						if (bHuff)
						{
							m_pHuffDecoder->DecodeInt(&nRDW, pHuffRDWTable);
							m_pHuffDecoder->DecodeInt(&nRDH, pHuffRDHTable);
							m_pHuffDecoder->DecodeInt(&nRDX, pHuffRDXTable);
							m_pHuffDecoder->DecodeInt(&nRDY, pHuffRDYTable);
							m_pHuffDecoder->DecodeInt(&nBMSize, pHuffRSizeTable);
							m_pHuffDecoder->Reset();
							m_pArithDecoder->Start();
						}
						else
						{
							m_pArithDecoder->DecodeInt(&nRDW, m_pIardwStats);
							m_pArithDecoder->DecodeInt(&nRDH, m_pIardhStats);
							m_pArithDecoder->DecodeInt(&nRDX, m_pIardxStats);
							m_pArithDecoder->DecodeInt(&nRDY, m_pIardyStats);
						}

						int nRefDX = ((nRDW >= 0) ? nRDW : nRDW - 1) / 2 + nRDX;
						int nRefDY = ((nRDH >= 0) ? nRDH : nRDH - 1) / 2 + nRDY;

						pSymbolBitmap = ReadGenericRefinementRegion(nRDW + ppSyms[unSymID]->GetWidth(), nRDH + ppSyms[unSymID]->GetHeight(), unTempl, false, ppSyms[unSymID], nRefDX, nRefDY, pnATx, pnATy);
					}
					else //~ do we need to use the bmSize value here (in Huffman mode)?
					{
						pSymbolBitmap = ppSyms[unSymID];
					}

					// combine the symbol bitmap into the region bitmap
					//~ something is wrong here - refCorner shouldn't degenerate into
					//~   two cases
					unsigned int unBitmapW = pSymbolBitmap->GetWidth() - 1;
					unsigned int unBitmapH = pSymbolBitmap->GetHeight() - 1;
					if (transposed)
					{
						switch (refCorner)
						{
							case 0: // bottom left
							pBitmap->Combine(pSymbolBitmap, nCurT, nCurS, unCombOp);
							break;
							case 1: // top left
							pBitmap->Combine(pSymbolBitmap, nCurT, nCurS, unCombOp);
							break;
							case 2: // bottom right
							pBitmap->Combine(pSymbolBitmap, nCurT - unBitmapW, nCurS, unCombOp);
							break;
							case 3: // top right
							pBitmap->Combine(pSymbolBitmap, nCurT - unBitmapW, nCurS, unCombOp);
							break;
						}
						nCurS += unBitmapH;
					}
					else
					{
						switch (refCorner)
						{
							case 0: // bottom left
							pBitmap->Combine(pSymbolBitmap, nCurS, nCurT - unBitmapH, unCombOp);
							break;
							case 1: // top left
							pBitmap->Combine(pSymbolBitmap, nCurS, nCurT, unCombOp);
							break;
							case 2: // bottom right
							pBitmap->Combine(pSymbolBitmap, nCurS, nCurT - unBitmapH, unCombOp);
							break;
							case 3: // top right
							pBitmap->Combine(pSymbolBitmap, nCurS, nCurT, unCombOp);
							break;
						}
						nCurS += unBitmapW;
					}

					if (nReadInt)
					{
						delete pSymbolBitmap;
					}
				}

				// next instance
				++unInstance;

				// next S value
				if (bHuff)
				{
					if (!m_pHuffDecoder->DecodeInt(&nDS, pHuffDSTable))
					{
						break;
					}
				}
				else
				{
					if (!m_pArithDecoder->DecodeInt(&nDS, m_pIadsStats))
					{
						break;
					}
				}
				nCurS += nSOffset + nDS;
			}
		}

		return pBitmap;
	}

	void JBIG2Stream::ReadPatternDictSegment(unsigned int unSegNum, unsigned int unLength)
	{
		// halftone dictionary flags, pattern width and height, max gray value
		unsigned int unFlags, unPatternW, unPatternH, unGrayMax;
		if (!ReadUByte(&unFlags) || !ReadUByte(&unPatternW) || !ReadUByte(&unPatternH) || !ReadULong(&unGrayMax))
		{
			return;
		}
		unsigned int unTempl = (unFlags >> 1) & 3;
		unsigned int unMMR   =  unFlags & 1;

		// set up the arithmetic decoder
		if (!unMMR)
		{
			ResetGenericStats(unTempl, NULL);
			m_pArithDecoder->Start();
		}

		// read the bitmap
		int arrATx[4], arrATy[4];
		arrATx[0] = -(int)unPatternW; arrATy[0] =  0;
		arrATx[1] = -3;               arrATy[1] = -1;
		arrATx[2] =  2;               arrATy[2] = -2;
		arrATx[3] = -2;               arrATy[3] = -2;
		JBIG2Bitmap *pBitmap = ReadGenericBitmap(unMMR ? true : false, (unGrayMax + 1) * unPatternW, unPatternH, unTempl, false, false, NULL, arrATx, arrATy, unLength - 7);

		// create the pattern dict object
		JBIG2PatternDict *pPatternDict = new JBIG2PatternDict(unSegNum, unGrayMax + 1);

		// split up the bitmap
		unsigned int unX = 0;
		for (unsigned int unIndex = 0; unIndex <= unGrayMax; ++unIndex)
		{
			pPatternDict->SetBitmap(unIndex, pBitmap->GetSlice(unX, 0, unPatternW, unPatternH));
			unX += unPatternW;
		}

		// free memory
		delete pBitmap;

		// store the new pattern dict
		m_pSegments->Append(pPatternDict);

		return;
	}

	void JBIG2Stream::ReadHalftoneRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount)
	{
		// region segment info field
		unsigned int unW, unH, unX, unY, unSegInfoFlags;
		if (!ReadULong(&unW) || !ReadULong(&unH) || !ReadULong(&unX) || !ReadULong(&unY) || !ReadUByte(&unSegInfoFlags))
		{
			return;
		}
		unsigned int unExtCombOp = unSegInfoFlags & 7;

		// rest of the halftone region header
		unsigned int unFlags;
		if (!ReadUByte(&unFlags))
		{
			return;
		}
		unsigned int unMMR        =  unFlags & 1;
		unsigned int unTempl      = (unFlags >> 1) & 3;
		unsigned int unEnableSkip = (unFlags >> 3) & 1;
		unsigned int unCombOp     = (unFlags >> 4) & 7;

		unsigned int unGridW, unGridH, unStepX, unStepY;
		int nGridX, nGridY;
		if (!ReadULong(&unGridW) || !ReadULong(&unGridH) || !ReadLong(&nGridX) || !ReadLong(&nGridY) || !ReadUWord(&unStepX) || !ReadUWord(&unStepY))
		{
			return;
		}
		if (unW == 0 || unH == 0 || unW >= INT_MAX / unH)
		{
			return;
		}
		if (unGridH == 0 || unGridW >= INT_MAX / unGridH)
		{
			return;
		}

		// get pattern dictionary
		if (unRefSegsCount != 1)
		{
			return;
		}

		JBIG2Segment *pSegment = FindSegment(punRefSegs[0]);
		if (pSegment->GetType() != jbig2SegPatternDict)
		{
			return;
		}
		JBIG2PatternDict *pPatternDict = (JBIG2PatternDict *)pSegment;

		unsigned int unBPP = 0;
		unsigned int unIndex = 1;
		while (unIndex < pPatternDict->GetSize())
		{
			++unBPP;
			unIndex <<= 1;
		}

		unsigned int unPatternW = pPatternDict->GetBitmap(0)->GetWidth();
		unsigned int unPatternH = pPatternDict->GetBitmap(0)->GetHeight();

		// set up the arithmetic decoder
		if (!unMMR)
		{
			ResetGenericStats(unTempl, NULL);
			m_pArithDecoder->Start();
		}

		// allocate the bitmap
		JBIG2Bitmap *pBitmap = new JBIG2Bitmap(unSegNum, unW, unH);
		if (unFlags & 0x80) // HDEFPIXEL
		{
			pBitmap->ClearToOne();
		}
		else
		{
			pBitmap->ClearToZero();
		}

		// compute the skip bitmap
		JBIG2Bitmap *pSkipBitmap = NULL;
		if (unEnableSkip)
		{
			pSkipBitmap = new JBIG2Bitmap(0, unGridW, unGridH);
			pSkipBitmap->ClearToZero();

			for (unsigned int unM = 0; unM < unGridH; ++unM)
			{
				for (unsigned int unN = 0; unN < unGridW; ++unN)
				{
					int nCurX = nGridX + unM * unStepY + unN * unStepX;
					int nCurY = nGridY + unM * unStepX - unN * unStepY;
					if (((nCurX + (int)unPatternW) >> 8) <= 0 || (nCurX >> 8) >= (int)unW || ((nCurY + (int)unPatternH) >> 8) <= 0 || (nCurY >> 8) >= (int)unH)
					{
						pSkipBitmap->SetPixel(unN, unM);
					}
				}
			}
		}

		// read the gray-scale image
		unsigned int *pGrayImage = (unsigned int *)MemUtilsMallocArray(unGridW * unGridH, sizeof(unsigned int));
		memset(pGrayImage, 0, unGridW * unGridH * sizeof(unsigned int));

		int arrATx[4], arrATy[4];
		arrATx[0] = unTempl <= 1 ? 3 : 2; arrATy[0] = -1;
		arrATx[1] = -3;                   arrATy[1] = -1;
		arrATx[2] =  2;                   arrATy[2] = -2;
		arrATx[3] = -2;                   arrATy[3] = -2;

		for (int nBitIndex = unBPP - 1; nBitIndex >= 0; --nBitIndex)
		{
			JBIG2Bitmap *pGrayBitmap = ReadGenericBitmap(0 != unMMR, unGridW, unGridH, unTempl, false, 0 != unEnableSkip, pSkipBitmap, arrATx, arrATy, -1);
			unIndex = 0;
			for (unsigned int unM = 0; unM < unGridH; ++unM)
			{
				for (unsigned int unN = 0; unN < unGridW; ++unN)
				{
					int nBit = pGrayBitmap->GetPixel(unN, unM) ^ (pGrayImage[unIndex] & 1);
					pGrayImage[unIndex] = (pGrayImage[unIndex] << 1) | nBit;
					++unIndex;
				}
			}
			delete pGrayBitmap;
		}

		// decode the image
		unIndex = 0;
		for (unsigned int unM = 0; unM < unGridH; ++unM)
		{
			int nCurX = nGridX + unM * unStepY;
			int nCurY = nGridY + unM * unStepX;
			for (unsigned int unN = 0; unN < unGridW; ++unN)
			{
				if (!(unEnableSkip && pSkipBitmap->GetPixel(unN, unM)))
				{
					JBIG2Bitmap *pPatternBitmap = pPatternDict->GetBitmap(pGrayImage[unIndex]);
					pBitmap->Combine(pPatternBitmap, nCurX >> 8, nCurY >> 8, unCombOp);
				}
				nCurX += unStepX;
				nCurY -= unStepY;
				++unIndex;
			}
		}

		MemUtilsFree(pGrayImage);
		if (pSkipBitmap)
		{
			delete pSkipBitmap;
		}

		// combine the region bitmap into the page bitmap
		if (bImm)
		{
			if (m_unPageH == 0xffffffff && unY + unH > m_unCurPageH)
			{
				m_pPageBitmap->Expand(unY + unH, m_unPageDefPixel);
			}
			m_pPageBitmap->Combine(pBitmap, unX, unY, unExtCombOp);
			delete pBitmap;
		}
		else // store the region bitmap
		{
			m_pSegments->Append(pBitmap);
		}

		return;
	}

	void JBIG2Stream::ReadGenericRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength)
	{
		// region segment info field
		unsigned int unW, unH, unX, unY, unSegInfoFlags;
		if (!ReadULong(&unW) || !ReadULong(&unH) || !ReadULong(&unX) || !ReadULong(&unY) || !ReadUByte(&unSegInfoFlags))
		{
			return;
		}
		unsigned int unExtCombOp = unSegInfoFlags & 7;

		// rest of the generic region segment header
		unsigned int unFlags;
		if (!ReadUByte(&unFlags))
		{
			return;
		}
		unsigned int unMMR    =  unFlags & 1;
		unsigned int unTempl  = (unFlags >> 1) & 3;
		unsigned int unTpgdOn = (unFlags >> 3) & 1;

		// AT flags
		int arrATx[4], arrATy[4];
		if (!unMMR)
		{
			if (unTempl == 0)
			{
				if (!ReadByte(&arrATx[0]) || !ReadByte(&arrATy[0]) || !ReadByte(&arrATx[1]) || !ReadByte(&arrATy[1]) ||
					!ReadByte(&arrATx[2]) || !ReadByte(&arrATy[2]) || !ReadByte(&arrATx[3]) || !ReadByte(&arrATy[3]))
				{
					return;
				}
			}
			else
			{
				if (!ReadByte(&arrATx[0]) || !ReadByte(&arrATy[0]))
				{
					return;
				}
			}
		}

		// set up the arithmetic decoder
		if (!unMMR)
		{
			ResetGenericStats(unTempl, NULL);
			m_pArithDecoder->Start();
		}

		// read the bitmap
		JBIG2Bitmap *pBitmap = ReadGenericBitmap(0 != unMMR, unW, unH, unTempl, 0 != unTpgdOn, false, NULL, arrATx, arrATy, unMMR ? 0 : unLength - 18);

		// combine the region bitmap into the page bitmap
		if (bImm)
		{
			if (m_unPageH == 0xffffffff && unY + unH > m_unCurPageH)
			{
				m_pPageBitmap->Expand(unY + unH, m_unPageDefPixel);
			}
			m_pPageBitmap->Combine(pBitmap, unX, unY, unExtCombOp);
			delete pBitmap;
		}
		else // store the region bitmap
		{
			pBitmap->SetSegNum(unSegNum);
			m_pSegments->Append(pBitmap);
		}

		return;
	}

	JBIG2Bitmap *JBIG2Stream::ReadGenericBitmap(bool bMMR, int nW, int nH, int unTempl, bool bTpgdOn, bool bUseSkip, JBIG2Bitmap *pSkip, int *pnATx, int *pnATy, int nMMrDataLength)
	{
		JBIG2Bitmap *pBitmap = new JBIG2Bitmap(0, nW, nH);
		pBitmap->ClearToZero();

		if (bMMR) //----- MMR decode
		{
			m_pMMrDecoder->Reset();

			int *pRefLine    = (int *)MemUtilsMallocArray(nW + 2, sizeof(int));
			int *pCodingLine = (int *)MemUtilsMallocArray(nW + 2, sizeof(int));

			pCodingLine[0] = pCodingLine[1] = nW;

			for (int nY = 0; nY < nH; ++nY)
			{
				// copy coding line to ref line
				int nIndex;
				for (nIndex = 0; pCodingLine[nIndex] < nW; ++nIndex)
				{
					pRefLine[nIndex] = pCodingLine[nIndex];
				}
				pRefLine[nIndex] = pRefLine[nIndex + 1] = nW;

				// decode a line
				int nRefIndex    = 0; // nB1 = pRefLine[refI]
				int nCodingIndex = 0; // nA1 = pCodingLine[codingI]
				int nA0          = 0;

				do
				{
					int nCode1 = m_pMMrDecoder->Get2DCode();
					switch (nCode1)
					{
						case Pass_2D:

						if (pRefLine[nRefIndex] < nW)
						{
							nA0 = pRefLine[nRefIndex + 1];
							nRefIndex += 2;
						}
						break;

						case Horiz_2D:

						{
							int nCode2, nCode3;

							if (nCodingIndex & 1)
							{
								nCode1 = 0;
								do
								{
									nCode1 += nCode3 = m_pMMrDecoder->GetBlackCode();
								} while (nCode3 >= 64);

								nCode2 = 0;
								do
								{
									nCode2 += nCode3 = m_pMMrDecoder->GetWhiteCode();
								} while (nCode3 >= 64);
							}
							else
							{
								nCode1 = 0;
								do
								{
									nCode1 += nCode3 = m_pMMrDecoder->GetWhiteCode();
								} while (nCode3 >= 64);

								nCode2 = 0;
								do
								{
									nCode2 += nCode3 = m_pMMrDecoder->GetBlackCode();
								} while (nCode3 >= 64);
							}
							if (nCode1 > 0 || nCode2 > 0)
							{
								nA0 = pCodingLine[nCodingIndex++] = nA0 + nCode1;
								nA0 = pCodingLine[nCodingIndex++] = nA0 + nCode2;
								while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
								{
									nRefIndex += 2;
								}
							}
						}
						break;

						case Vert0_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex];
						if (pRefLine[nRefIndex] < nW)
						{
							++nRefIndex;
						}
						break;

						case VertR1_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] + 1;
						if (pRefLine[nRefIndex] < nW)
						{
							++nRefIndex;
							while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
							{
								nRefIndex += 2;
							}
						}
						break;

						case VertR2_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] + 2;
						if (pRefLine[nRefIndex] < nW)
						{
							++nRefIndex;
							while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
							{
								nRefIndex += 2;
							}
						}
						break;

						case VertR3_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] + 3;
						if (pRefLine[nRefIndex] < nW)
						{
							++nRefIndex;
							while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
							{
								nRefIndex += 2;
							}
						}
						break;

						case VertL1_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] - 1;
						if (nRefIndex > 0)
						{
							--nRefIndex;
						}
						else
						{
							++nRefIndex;
						}
						while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
						{
							nRefIndex += 2;
						}
						break;

						case VertL2_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] - 2;
						if (nRefIndex > 0)
						{
							--nRefIndex;
						}
						else
						{
							++nRefIndex;
						}
						while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
						{
							nRefIndex += 2;
						}
						break;

						case VertL3_2D:

						nA0 = pCodingLine[nCodingIndex++] = pRefLine[nRefIndex] - 3;
						if (nRefIndex > 0)
						{
							--nRefIndex;
						}
						else
						{
							++nRefIndex;
						}
						while (pRefLine[nRefIndex] <= nA0 && pRefLine[nRefIndex] < nW)
						{
							nRefIndex += 2;
						}
						break;

						default:

						break;

					}
				} while (nA0 < nW);
				pCodingLine[nCodingIndex++] = nW;

				// convert the run lengths to a bitmap line
				nIndex = 0;
				while (pCodingLine[nIndex] < nW)
				{
					for (int nX = pCodingLine[nIndex]; nX < pCodingLine[nIndex + 1]; ++nX)
					{
						pBitmap->SetPixel(nX, nY);
					}
					nIndex += 2;
				}
			}

			if (nMMrDataLength >= 0)
			{
				m_pMMrDecoder->SkipTo(nMMrDataLength);
			}
			else
			{
				if (m_pMMrDecoder->Get24Bits() != 0x001001)
				{
				}
			}

			MemUtilsFree(pRefLine);
			MemUtilsFree(pCodingLine);
		}
		else //----- arithmetic decode
		{
			// set up the typical row context
			unsigned int unLtpCX = 0;
			if (bTpgdOn)
			{
				switch (unTempl)
				{
					case 0:
					unLtpCX = 0x3953; // 001 11001 0101 0011
					break;
					case 1:
					unLtpCX = 0x079a; // 0011 11001 101 0
					break;
					case 2:
					unLtpCX = 0x0e3; // 001 1100 01 1
					break;
					case 3:
					unLtpCX = 0x18a; // 01100 0101 1
					break;
				}
			}

			bool bLtp = 0;
			unsigned int unCX = 0, unCX0 = 0, unCX1 = 0, unCX2 = 0;
			JBIG2BitmapPtr oCXPtr0, oCXPtr1;
			JBIG2BitmapPtr oATPtr0, oATPtr1, oATPtr2, oATPtr3;

			for (int nY = 0; nY < nH; ++nY)
			{
				// check for a "typical" (duplicate) row
				if (bTpgdOn)
				{
					if (m_pArithDecoder->DecodeBit(unLtpCX, m_pGenericRegionStats))
					{
						bLtp = !bLtp;
					}
					if (bLtp)
					{
						pBitmap->DuplicateRow(nY, nY - 1);
						continue;
					}
				}

				switch (unTempl)
				{
					case 0:

					// set up the context
					pBitmap->GetPixelPtr(0, nY - 2, &oCXPtr0);
					unCX0 = pBitmap->NextPixel(&oCXPtr0);
					unCX0 = (unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0);

					pBitmap->GetPixelPtr(0, nY - 1, &oCXPtr1);
					unCX1 = pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);

					unCX2 = 0;
					pBitmap->GetPixelPtr(pnATx[0], nY + pnATy[0], &oATPtr0);
					pBitmap->GetPixelPtr(pnATx[1], nY + pnATy[1], &oATPtr1);
					pBitmap->GetPixelPtr(pnATx[2], nY + pnATy[2], &oATPtr2);
					pBitmap->GetPixelPtr(pnATx[3], nY + pnATy[3], &oATPtr3);

					// decode the row
					for (int nX = 0; nX < nW; ++nX)
					{
						// build the context
						unCX = (unCX0 << 13) | (unCX1 << 8) | (unCX2 << 4) | (pBitmap->NextPixel(&oATPtr0) << 3) | (pBitmap->NextPixel(&oATPtr1) << 2) | (pBitmap->NextPixel(&oATPtr2) << 1) | pBitmap->NextPixel(&oATPtr3);

						int nPixel;
						if (bUseSkip && pSkip->GetPixel(nX, nY)) // check for a skipped pixel
						{
							nPixel = 0;
						}
						else if ((nPixel = m_pArithDecoder->DecodeBit(unCX, m_pGenericRegionStats))) // decode the pixel
						{
							pBitmap->SetPixel(nX, nY);
						}

						// update the context
						unCX0 = ((unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0)) & 0x07;
						unCX1 = ((unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1)) & 0x1f;
						unCX2 = ((unCX2 << 1) | nPixel) & 0x0f;
					}
					break;

					case 1:

					// set up the context
					pBitmap->GetPixelPtr(0, nY - 2, &oCXPtr0);
					unCX0 = pBitmap->NextPixel(&oCXPtr0);
					unCX0 = (unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0);
					unCX0 = (unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0);

					pBitmap->GetPixelPtr(0, nY - 1, &oCXPtr1);
					unCX1 = pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);
					unCX2 = 0;

					pBitmap->GetPixelPtr(pnATx[0], nY + pnATy[0], &oATPtr0);

					// decode the row
					for (int nX = 0; nX < nW; ++nX)
					{
						// build the context
						unCX = (unCX0 << 9) | (unCX1 << 4) | (unCX2 << 1) | pBitmap->NextPixel(&oATPtr0);

						int nPixel;
						if (bUseSkip && pSkip->GetPixel(nX, nY)) // check for a skipped pixel
						{
							nPixel = 0;
						}
						else if ((nPixel = m_pArithDecoder->DecodeBit(unCX, m_pGenericRegionStats)))  // decode the pixel
						{
							pBitmap->SetPixel(nX, nY);
						}

						// update the context
						unCX0 = ((unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0)) & 0x0f;
						unCX1 = ((unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1)) & 0x1f;
						unCX2 = ((unCX2 << 1) | nPixel) & 0x07;
					}
					break;

					case 2:

					// set up the context
					pBitmap->GetPixelPtr(0, nY - 2, &oCXPtr0);
					unCX0 = pBitmap->NextPixel(&oCXPtr0);
					unCX0 = (unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0);

					pBitmap->GetPixelPtr(0, nY - 1, &oCXPtr1);
					unCX1 = pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);
					unCX2 = 0;

					pBitmap->GetPixelPtr(pnATx[0], nY + pnATy[0], &oATPtr0);

					// decode the row
					for (int nX = 0; nX < nW; ++nX)
					{

						// build the context
						unCX = (unCX0 << 7) | (unCX1 << 3) | (unCX2 << 1) | pBitmap->NextPixel(&oATPtr0);

						int nPixel;
						if (bUseSkip && pSkip->GetPixel(nX, nY)) // check for a skipped pixel
						{
							nPixel = 0;
						}
						else if ((nPixel = m_pArithDecoder->DecodeBit(unCX, m_pGenericRegionStats))) // decode the pixel
						{
							pBitmap->SetPixel(nX, nY);
						}

						// update the context
						unCX0 = ((unCX0 << 1) | pBitmap->NextPixel(&oCXPtr0)) & 0x07;
						unCX1 = ((unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1)) & 0x0f;
						unCX2 = ((unCX2 << 1) | nPixel) & 0x03;
					}
					break;

					case 3:

					// set up the context
					pBitmap->GetPixelPtr(0, nY - 1, &oCXPtr1);
					unCX1 = pBitmap->NextPixel(&oCXPtr1);
					unCX1 = (unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1);
					unCX2 = 0;

					pBitmap->GetPixelPtr(pnATx[0], nY + pnATy[0], &oATPtr0);

					// decode the row
					for (int nX = 0; nX < nW; ++nX)
					{
						// build the context
						unCX = (unCX1 << 5) | (unCX2 << 1) | pBitmap->NextPixel(&oATPtr0);

						int nPixel;
						if (bUseSkip && pSkip->GetPixel(nX, nY)) // check for a skipped pixel
						{
							nPixel = 0;
						}
						else if ((nPixel = m_pArithDecoder->DecodeBit(unCX, m_pGenericRegionStats))) // decode the pixel
						{
							pBitmap->SetPixel(nX, nY);
						}
						// update the context
						unCX1 = ((unCX1 << 1) | pBitmap->NextPixel(&oCXPtr1)) & 0x1f;
						unCX2 = ((unCX2 << 1) | nPixel) & 0x0f;
					}
					break;
				}
			}
		}

		return pBitmap;
	}

	void JBIG2Stream::ReadGenericRefinementRegionSegment(unsigned int unSegNum, bool bImm, bool bLossless, unsigned int unLength, unsigned int *punRefSegs, unsigned int unRefSegsCount)
	{
		unsigned int unW, unH, unX, unY, unSegInfoFlags;

		// region segment info field
		if (!ReadULong(&unW) || !ReadULong(&unH) || !ReadULong(&unX) || !ReadULong(&unY) || !ReadUByte(&unSegInfoFlags))
		{
			return;
		}
		unsigned int unExtCombOp = unSegInfoFlags & 7;

		// rest of the generic refinement region segment header
		unsigned int unFlags;
		if (!ReadUByte(&unFlags))
		{
			return;
		}

		unsigned int unTempl  = unFlags & 1;
		unsigned int unTpgrOn = (unFlags >> 1) & 1;

		// AT flags
		int arrATx[2], arrATy[2];
		if (!unTempl)
		{
			if (!ReadByte(&arrATx[0]) || !ReadByte(&arrATy[0]) || !ReadByte(&arrATx[1]) || !ReadByte(&arrATy[1]))
			{
				return;
			}
		}

		// resize the page bitmap if needed
		if (unRefSegsCount == 0 || bImm)
		{
			if (m_unPageH == 0xffffffff && unY + unH > m_unCurPageH)
			{
				m_pPageBitmap->Expand(unY + unH, m_unPageDefPixel);
			}
		}

		// get referenced bitmap
		JBIG2Bitmap *pRefBitmap = NULL;
		if (unRefSegsCount > 1)
		{
			return;
		}
		if (unRefSegsCount == 1)
		{
			JBIG2Segment *pSegment = FindSegment(punRefSegs[0]);
			if (pSegment->GetType() != jbig2SegBitmap)
			{
				return;
			}
			pRefBitmap = (JBIG2Bitmap *)pSegment;
		}
		else
		{
			pRefBitmap = m_pPageBitmap->GetSlice(unX, unY, unW, unH);
		}

		// set up the arithmetic decoder
		ResetRefinementStats(unTempl, NULL);
		m_pArithDecoder->Start();

		// read
		JBIG2Bitmap *pBitmap = ReadGenericRefinementRegion(unW, unH, unTempl, 0 != unTpgrOn, pRefBitmap, 0, 0, arrATx, arrATy);

		if (bImm) // combine the region bitmap into the page bitmap
		{
			m_pPageBitmap->Combine(pBitmap, unX, unY, unExtCombOp);
			delete pBitmap;
		}
		else // store the region bitmap
		{
			pBitmap->SetSegNum(unSegNum);
			m_pSegments->Append(pBitmap);
		}

		// delete the referenced bitmap
		if (unRefSegsCount == 1)
		{
			DiscardSegment(punRefSegs[0]);
		}
		else
		{
			delete pRefBitmap;
		}

		return;
	}

	JBIG2Bitmap *JBIG2Stream::ReadGenericRefinementRegion(int nW, int nH, int nTempl, bool bTpgrOn, JBIG2Bitmap *pRefBitmap, int nRefDX, int nRefDY, int *pnAtx, int *pnAty)
	{
		JBIG2Bitmap *pBitmap = new JBIG2Bitmap(0, nW, nH);
		pBitmap->ClearToZero();

		// Typical row context
		unsigned int unLtpCX;
		if (nTempl)
		{
			unLtpCX = 0x008;
		}
		else
		{
			unLtpCX = 0x0010;
		}

		bool bLtp = 0;
		unsigned int unCx0, unCx2, unCx3, unCx4;
		JBIG2BitmapPtr oCxPtr0, oCxPtr1, oCxPtr2, oCxPtr3, oCxPtr4, oCxPtr5, oCxPtr6;
		for (int nY = 0; nY < nH; ++nY)
		{
			if (nTempl)
			{
				// set up the context
				pBitmap->GetPixelPtr(0, nY - 1, &oCxPtr0);
				unCx0 = pBitmap->NextPixel(&oCxPtr0);

				pBitmap->GetPixelPtr(-1, nY, &oCxPtr1);
				pRefBitmap->GetPixelPtr(-nRefDX, nY - 1 - nRefDY, &oCxPtr2);
				pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - nRefDY, &oCxPtr3);

				unCx3 = pRefBitmap->NextPixel(&oCxPtr3);
				unCx3 = (unCx3 << 1) | pRefBitmap->NextPixel(&oCxPtr3);

				pRefBitmap->GetPixelPtr(-nRefDX, nY + 1 - nRefDY, &oCxPtr4);
				unCx4 = pRefBitmap->NextPixel(&oCxPtr4);

				// set up the typical prediction context
				unsigned int unTpgrCX0 = 0, unTpgrCX1 = 0, unTpgrCX2 = 0;
				JBIG2BitmapPtr oTpgrCXPtr0, oTpgrCXPtr1, oTpgrCXPtr2;
				if (bTpgrOn)
				{

					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - 1 - nRefDY, &oTpgrCXPtr0);
					unTpgrCX0 = pRefBitmap->NextPixel(&oTpgrCXPtr0);
					unTpgrCX0 = (unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0);
					unTpgrCX0 = (unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0);

					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - nRefDY, &oTpgrCXPtr1);
					unTpgrCX1 = pRefBitmap->NextPixel(&oTpgrCXPtr1);
					unTpgrCX1 = (unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1);
					unTpgrCX1 = (unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1);

					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY + 1 - nRefDY, &oTpgrCXPtr2);
					unTpgrCX2 = pRefBitmap->NextPixel(&oTpgrCXPtr2);
					unTpgrCX2 = (unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2);
					unTpgrCX2 = (unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2);
				}

				for (int nX = 0; nX < nW; ++nX)
				{

					// update the context
					unCx0 = ((unCx0 << 1) | pBitmap->NextPixel(&oCxPtr0)) & 7;
					unCx3 = ((unCx3 << 1) | pRefBitmap->NextPixel(&oCxPtr3)) & 7;
					unCx4 = ((unCx4 << 1) | pRefBitmap->NextPixel(&oCxPtr4)) & 3;

					if (bTpgrOn)
					{
						// update the typical predictor context
						unTpgrCX0 = ((unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0)) & 7;
						unTpgrCX1 = ((unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1)) & 7;
						unTpgrCX2 = ((unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2)) & 7;

						// check for a "typical" pixel
						if (m_pArithDecoder->DecodeBit(unLtpCX, m_pRefinementRegionStats))
						{
							bLtp = !bLtp;
						}
						if (unTpgrCX0 == 0 && unTpgrCX1 == 0 && unTpgrCX2 == 0)
						{
							pBitmap->ClearPixel(nX, nY);
							continue;
						}
						else if (unTpgrCX0 == 7 && unTpgrCX1 == 7 && unTpgrCX2 == 7)
						{
							pBitmap->SetPixel(nX, nY);
							continue;
						}
					}

					// build the context
					unsigned int unCx = (unCx0 << 7) | (pBitmap->NextPixel(&oCxPtr1) << 6) | (pRefBitmap->NextPixel(&oCxPtr2) << 5) | (unCx3 << 2) | unCx4;

					// decode the pixel
					int nPix;
					if ((nPix = m_pArithDecoder->DecodeBit(unCx, m_pRefinementRegionStats)))
					{
						pBitmap->SetPixel(nX, nY);
					}
				}

			}
			else
			{

				// set up the context
				pBitmap->GetPixelPtr(0, nY - 1, &oCxPtr0);
				unCx0 = pBitmap->NextPixel(&oCxPtr0);

				pBitmap->GetPixelPtr(-1, nY, &oCxPtr1);

				pRefBitmap->GetPixelPtr(-nRefDX, nY - 1 - nRefDY, &oCxPtr2);
				unCx2 = pRefBitmap->NextPixel(&oCxPtr2);

				pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - nRefDY, &oCxPtr3);
				unCx3 = pRefBitmap->NextPixel(&oCxPtr3);
				unCx3 = (unCx3 << 1) | pRefBitmap->NextPixel(&oCxPtr3);

				pRefBitmap->GetPixelPtr(-1 - nRefDX, nY + 1 - nRefDY, &oCxPtr4);
				unCx4 = pRefBitmap->NextPixel(&oCxPtr4);
				unCx4 = (unCx4 << 1) | pRefBitmap->NextPixel(&oCxPtr4);

				pBitmap->GetPixelPtr(pnAtx[0], nY + pnAty[0], &oCxPtr5);

				pRefBitmap->GetPixelPtr(pnAtx[1] - nRefDX, nY + pnAty[1] - nRefDY, &oCxPtr6);

				// set up the typical prediction context
				unsigned int unTpgrCX0 = 0, unTpgrCX1 = 0, unTpgrCX2 = 0;
				JBIG2BitmapPtr oTpgrCXPtr0, oTpgrCXPtr1, oTpgrCXPtr2;
				if (bTpgrOn)
				{
					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - 1 - nRefDY, &oTpgrCXPtr0);
					unTpgrCX0 = pRefBitmap->NextPixel(&oTpgrCXPtr0);
					unTpgrCX0 = (unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0);
					unTpgrCX0 = (unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0);

					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY - nRefDY, &oTpgrCXPtr1);
					unTpgrCX1 = pRefBitmap->NextPixel(&oTpgrCXPtr1);
					unTpgrCX1 = (unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1);
					unTpgrCX1 = (unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1);

					pRefBitmap->GetPixelPtr(-1 - nRefDX, nY + 1 - nRefDY, &oTpgrCXPtr2);
					unTpgrCX2 = pRefBitmap->NextPixel(&oTpgrCXPtr2);
					unTpgrCX2 = (unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2);
					unTpgrCX2 = (unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2);
				}

				for (int nX = 0; nX < nW; ++nX)
				{

					// update the context
					unCx0 = ((unCx0 << 1) | pBitmap->NextPixel(&oCxPtr0)) & 3;
					unCx2 = ((unCx2 << 1) | pRefBitmap->NextPixel(&oCxPtr2)) & 3;
					unCx3 = ((unCx3 << 1) | pRefBitmap->NextPixel(&oCxPtr3)) & 7;
					unCx4 = ((unCx4 << 1) | pRefBitmap->NextPixel(&oCxPtr4)) & 7;

					if (bTpgrOn)
					{
						// update the typical predictor context
						unTpgrCX0 = ((unTpgrCX0 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr0)) & 7;
						unTpgrCX1 = ((unTpgrCX1 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr1)) & 7;
						unTpgrCX2 = ((unTpgrCX2 << 1) | pRefBitmap->NextPixel(&oTpgrCXPtr2)) & 7;

						// check for a "typical" pixel
						if (m_pArithDecoder->DecodeBit(unLtpCX, m_pRefinementRegionStats))
						{
							bLtp = !bLtp;
						}
						if (unTpgrCX0 == 0 && unTpgrCX1 == 0 && unTpgrCX2 == 0)
						{
							pBitmap->ClearPixel(nX, nY);
							continue;
						}
						else if (unTpgrCX0 == 7 && unTpgrCX1 == 7 && unTpgrCX2 == 7)
						{
							pBitmap->SetPixel(nX, nY);
							continue;
						}
					}

					// build the context
					unsigned int unCx = (unCx0 << 11) | (pBitmap->NextPixel(&oCxPtr1) << 10) | (unCx2 << 8) | (unCx3 << 5) | (unCx4 << 2) | (pBitmap->NextPixel(&oCxPtr5) << 1) | pRefBitmap->NextPixel(&oCxPtr6);

					// decode the pixel
					int nPix;
					if ((nPix = m_pArithDecoder->DecodeBit(unCx, m_pRefinementRegionStats)))
					{
						pBitmap->SetPixel(nX, nY);
					}
				}
			}
		}

		return pBitmap;
	}

	void JBIG2Stream::ReadPageInfoSegment(unsigned int unLength)
	{
		unsigned int unResX, unResY, unFlags, unStriping;

		if (!ReadULong(&m_unPageW) || !ReadULong(&m_unPageH) || !ReadULong(&unResX) || !ReadULong(&unResY) || !ReadUByte(&unFlags) || !ReadUWord(&unStriping))
		{
			return;
		}
		m_unPageDefPixel = (unFlags >> 2) & 1;
		m_unDefCombOp    = (unFlags >> 3) & 3;

		if (0xffffffff == m_unPageH)
		{
			m_unCurPageH = unStriping & 0x7fff;
		}
		else
		{
			m_unCurPageH = m_unPageH;
		}
		m_pPageBitmap = new JBIG2Bitmap(0, m_unPageW, m_unPageH);

		// Очищаем картинку
		if (m_unPageDefPixel)
		{
			m_pPageBitmap->ClearToOne();
		}
		else
		{
			m_pPageBitmap->ClearToZero();
		}

		return;
	}

	void JBIG2Stream::ReadEndOfStripeSegment(unsigned int unLength)
	{
		// Пропускаем данный сегмент
		for (unsigned int unIndex = 0; unIndex < unLength; ++unIndex)
		{
			m_pCurStream->GetChar();
		}
	}

	void JBIG2Stream::ReadProfilesSegment(unsigned int unLength)
	{
		// Пропускаем данный сегмент
		for (unsigned int unIndex = 0; unIndex < unLength; ++unIndex)
		{
			m_pCurStream->GetChar();
		}
	}
	void JBIG2Stream::ReadCodeTableSegment(unsigned int unSegNum, unsigned int unLength)
	{
		unsigned int unFlags;
		int nLowVal, nHighVal;
		if (!ReadUByte(&unFlags) || !ReadLong(&nLowVal) || !ReadLong(&nHighVal))
		{
			return;
		}
		unsigned int unOOB        = unFlags & 1;
		unsigned int unPrefixBits = ((unFlags >> 1) & 7) + 1;
		unsigned int unRangeBits  = ((unFlags >> 4) & 7) + 1;

		m_pHuffDecoder->Reset();

		unsigned int unHuffTabSize = 8;
		JBIG2HuffmanTable *pHuffTable = (JBIG2HuffmanTable *)MemUtilsMallocArray(unHuffTabSize, sizeof(JBIG2HuffmanTable));

		unsigned int unIndex = 0;
		int nVal = nLowVal;
		while (nVal < nHighVal)
		{
			if (unIndex == unHuffTabSize)
			{
				unHuffTabSize *= 2;
				pHuffTable = (JBIG2HuffmanTable *)MemUtilsReallocArray(pHuffTable, unHuffTabSize, sizeof(JBIG2HuffmanTable));
			}
			pHuffTable[unIndex].nValue      = nVal;
			pHuffTable[unIndex].unPrefixLen = m_pHuffDecoder->ReadBits(unPrefixBits);
			pHuffTable[unIndex].unRangeLen  = m_pHuffDecoder->ReadBits(unRangeBits);
			nVal += 1 << pHuffTable[unIndex].unRangeLen;
			++unIndex;
		}
		if (unIndex + unOOB + 3 > unHuffTabSize)
		{
			unHuffTabSize = unIndex + unOOB + 3;
			pHuffTable = (JBIG2HuffmanTable *)MemUtilsReallocArray(pHuffTable, unHuffTabSize, sizeof(JBIG2HuffmanTable));
		}
		pHuffTable[unIndex].nValue      = nLowVal - 1;
		pHuffTable[unIndex].unPrefixLen = m_pHuffDecoder->ReadBits(unPrefixBits);
		pHuffTable[unIndex].unRangeLen  = jbig2HuffmanLOW;
		++unIndex;

		pHuffTable[unIndex].nValue      = nHighVal;
		pHuffTable[unIndex].unPrefixLen = m_pHuffDecoder->ReadBits(unPrefixBits);
		pHuffTable[unIndex].unRangeLen  = 32;
		++unIndex;

		if (unOOB)
		{
			pHuffTable[unIndex].nValue      = 0;
			pHuffTable[unIndex].unPrefixLen = m_pHuffDecoder->ReadBits(unPrefixBits);
			pHuffTable[unIndex].unRangeLen  = jbig2HuffmanOOB;
			++unIndex;
		}

		pHuffTable[unIndex].nValue      = 0;
		pHuffTable[unIndex].unPrefixLen = 0;
		pHuffTable[unIndex].unRangeLen  = jbig2HuffmanEOT;
		m_pHuffDecoder->BuildTable(pHuffTable, unIndex);

		// Создаем и сохраняем новый сегмент таблицы
		m_pSegments->Append(new JBIG2CodeTable(unSegNum, pHuffTable));

		return;
	}

	void JBIG2Stream::ReadExtensionSegment(unsigned int unLength)
	{
		// Пропускаем данный сегмент
		for (unsigned int unIndex = 0; unIndex < unLength; ++unIndex)
		{
			m_pCurStream->GetChar();
		}
	}

	JBIG2Segment *JBIG2Stream::FindSegment(unsigned int unSegNum)
	{
		JBIG2Segment *pSegment = NULL;

		for (int nIndex = 0; nIndex < m_pGlobalSegments->GetLength(); ++nIndex)
		{
			pSegment = (JBIG2Segment *)m_pGlobalSegments->GetByIndex(nIndex);
			if (pSegment->GetSegNum() == unSegNum)
			{
				return pSegment;
			}
		}

		for (int nIndex = 0; nIndex < m_pSegments->GetLength(); ++nIndex)
		{
			pSegment = (JBIG2Segment *)m_pSegments->GetByIndex(nIndex);
			if (pSegment->GetSegNum() == unSegNum)
			{
				return pSegment;
			}
		}

		return NULL;
	}

	void          JBIG2Stream::DiscardSegment(unsigned int unSegNum)
	{
		JBIG2Segment *pSegment = NULL;

		for (int nIndex = 0; nIndex < m_pGlobalSegments->GetLength(); ++nIndex)
		{
			pSegment = (JBIG2Segment *)m_pGlobalSegments->GetByIndex(nIndex);
			if (pSegment->GetSegNum() == unSegNum)
			{
				m_pGlobalSegments->Delete(nIndex);
				return;
			}
		}
		for (int nIndex = 0; nIndex < m_pSegments->GetLength(); ++nIndex)
		{
			pSegment = (JBIG2Segment *)m_pSegments->GetByIndex(nIndex);
			if (pSegment->GetSegNum() == unSegNum)
			{
				m_pSegments->Delete(nIndex);
				return;
			}
		}
	}

	void JBIG2Stream::ResetGenericStats(unsigned int unTempl, JArithmeticDecoderStats *pPrevStats)
	{
		int nSize = c_arrContextSize[unTempl];
		if (pPrevStats && pPrevStats->GetContextSize() == nSize)
		{
			if (m_pGenericRegionStats->GetContextSize() == nSize)
			{
				m_pGenericRegionStats->CopyFrom(pPrevStats);
			}
			else
			{
				delete m_pGenericRegionStats;
				m_pGenericRegionStats = pPrevStats->Copy();
			}
		}
		else
		{
			if (m_pGenericRegionStats->GetContextSize() == nSize)
			{
				m_pGenericRegionStats->Reset();
			}
			else
			{
				delete m_pGenericRegionStats;
				m_pGenericRegionStats = new JArithmeticDecoderStats(1 << nSize);
			}
		}
	}

	void JBIG2Stream::ResetRefinementStats(unsigned int unTempl, JArithmeticDecoderStats *pPrevStats)
	{
		int nSize = c_arrRefContextSize[unTempl];
		if (pPrevStats && pPrevStats->GetContextSize() == nSize)
		{
			if (m_pRefinementRegionStats->GetContextSize() == nSize)
			{
				m_pRefinementRegionStats->CopyFrom(pPrevStats);
			}
			else
			{
				delete m_pRefinementRegionStats;
				m_pRefinementRegionStats = pPrevStats->Copy();
			}
		}
		else
		{
			if (m_pRefinementRegionStats->GetContextSize() == nSize)
			{
				m_pRefinementRegionStats->Reset();
			}
			else
			{
				delete m_pRefinementRegionStats;
				m_pRefinementRegionStats = new JArithmeticDecoderStats(1 << nSize);
			}
		}
	}

	void JBIG2Stream::ResetIntStats(int nSymCodeLen)
	{
		m_pIadhStats->Reset();
		m_pIadwStats->Reset();
		m_pIaexStats->Reset();
		m_pIaaiStats->Reset();
		m_pIadtStats->Reset();
		m_pIaitStats->Reset();
		m_pIafsStats->Reset();
		m_pIadsStats->Reset();
		m_pIardxStats->Reset();
		m_pIardyStats->Reset();
		m_pIardwStats->Reset();
		m_pIardhStats->Reset();
		m_pIariStats->Reset();

		if (m_pIaidStats->GetContextSize() == 1 << (nSymCodeLen + 1))
		{
			m_pIaidStats->Reset();
		}
		else
		{
			delete m_pIaidStats;
			m_pIaidStats = new JArithmeticDecoderStats(1 << (nSymCodeLen + 1));
		}
	}

	bool JBIG2Stream::ReadUByte(unsigned int *pBuffer)
	{
		int nChar;

		if (EOF == (nChar = m_pCurStream->GetChar()))
		{
			return false;
		}
		*pBuffer = (unsigned int)nChar;

		return true;
	}

	bool JBIG2Stream::ReadByte(int *pBuffer)
	{
		int nChar;

		if (EOF == (nChar = m_pCurStream->GetChar()))
		{
			return false;
		}

		*pBuffer = nChar;
		if (nChar & 0x80)
		{
			*pBuffer |= -1 - 0xff;
		}

		return true;
	}

	bool JBIG2Stream::ReadUWord(unsigned int *pBuffer)
	{
		int nChar0, nChar1;

		if (EOF == (nChar0 = m_pCurStream->GetChar()) || EOF == (nChar1 = m_pCurStream->GetChar()))
		{
			return false;
		}

		*pBuffer = (unsigned int)((nChar0 << 8) | nChar1);

		return true;
	}

	bool JBIG2Stream::ReadULong(unsigned int *pBuffer)
	{
		int nChar0, nChar1, nChar2, nChar3;

		if (EOF == (nChar0 = m_pCurStream->GetChar()) || EOF == (nChar1 = m_pCurStream->GetChar()) ||
			EOF == (nChar2 = m_pCurStream->GetChar()) || EOF == (nChar3 = m_pCurStream->GetChar()))
		{
			return false;
		}

		*pBuffer = (unsigned int)((nChar0 << 24) | (nChar1 << 16) | (nChar2 << 8) | nChar3);

		return true;
	}

	bool JBIG2Stream::ReadLong(int *pBuffer)
	{
		int nChar0, nChar1, nChar2, nChar3;

		if (EOF == (nChar0 = m_pCurStream->GetChar()) || EOF == (nChar1 = m_pCurStream->GetChar()) ||
			EOF == (nChar2 = m_pCurStream->GetChar()) || EOF == (nChar3 = m_pCurStream->GetChar()))
		{
			return false;
		}

		*pBuffer = ((nChar0 << 24) | (nChar1 << 16) | (nChar2 << 8) | nChar3);
		if (nChar0 & 0x80)
		{
			*pBuffer |= -1 - (int)0xffffffff;
		}

		return true;
	}
}
