/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "FontTTWriter.h"

#include <map>
#include <vector>
#include <list>

namespace PdfWriter
{
	BYTE GetMostCompressedOffsetSize(unsigned long inOffset)
	{
		if (inOffset < 256)
			return 1;
		if (inOffset < 65536)
			return 2;
		if (inOffset < 1<<24)
			return 3;
		return 4;
	}
	void WriteOffset(unsigned int nValue, BYTE nSizeOfOffset, CStream* pOutputStream)
	{
		switch (nSizeOfOffset)
		{
		case 1:
		{
			pOutputStream->WriteUChar(nValue & 0xff);
			break;
		}
		case 2:
		{
			pOutputStream->WriteUChar((nValue >> 8) & 0xff);
			pOutputStream->WriteUChar(nValue & 0xff);
			break;
		}
		case 3:
		{
			pOutputStream->WriteUChar((nValue >> 16) & 0xff);
			pOutputStream->WriteUChar((nValue >> 8) & 0xff);
			pOutputStream->WriteUChar(nValue & 0xff);
			break;
		}
		case 4:
		{
			pOutputStream->WriteUChar((nValue >> 24) & 0xff);
			pOutputStream->WriteUChar((nValue >> 16) & 0xff);
			pOutputStream->WriteUChar((nValue >> 8) & 0xff);
			pOutputStream->WriteUChar(nValue & 0xff);
			break;
		}
		}
	}
	BYTE* ReadIndexHeader(BYTE* pData, unsigned int** pOffsets, unsigned short& ushCount)
	{
		ushCount = (pData[0] << 8) | pData[1];
		pData += 2;
		BYTE nOffSize = *pData++;

		*pOffsets = new unsigned int[ushCount + 1];

		for (unsigned short i = 0; i <= ushCount; ++i)
		{
			unsigned int unOffset = 0;
			for (int i = 0; i < nOffSize; ++i)
				unOffset = (unOffset << 8) | pData[i];
			pData += nOffSize;
			(*pOffsets)[i] = unOffset;
		}

		return pData;
	}
	BYTE* SkipIndexHeader(BYTE* pData, unsigned short& ushCount)
	{
		ushCount = (pData[0] << 8) | pData[1];
		pData += 2;
		BYTE nOffSize = *pData++;

		pData += ushCount * nOffSize;
		int nOffsetEnd = 0;
		for (int i = 0; i < nOffSize; ++i)
			nOffsetEnd = (nOffsetEnd << 8) | pData[i];
		pData += nOffSize;
		pData += nOffsetEnd - 1;

		return pData;
	}
	struct CDictOperand
	{
		bool bIsInteger = true;
		int nIntegerValue = 0;
		int nRealFractalEnd = 0;
		double dRealValue = 0.0;
	};
	struct CCharString
	{
		unsigned int nStartPosition;
		unsigned int nEndPosition;
		unsigned short ushIndex;
	};
	struct CCharStrings
	{
		BYTE nCharStringsType;
		unsigned short ushCharStringsCount;
		CCharString* pCharStringsIndex;
	};
	struct CPrivateDictInfo
	{
		unsigned int nPrivateDictStart;
		unsigned int nPrivateDictEnd;
		std::map<unsigned short, std::vector<CDictOperand>> mPrivateDict;
		CCharStrings* mLocalSubrs;
	};
	struct CFontDictInfo
	{
		unsigned int nFontDictStart;
		unsigned int nFontDictEnd;
		std::map<unsigned short, std::vector<CDictOperand>> mFontDict;
		CPrivateDictInfo mPrivateDict;
	};
	struct CTopDictInfo
	{
		std::map<unsigned short, std::vector<CDictOperand>> mTopDict;
	};
	void ReadTopDICT(BYTE* pCFFData, BYTE* pCFFDataEnd, CTopDictInfo& TopDICT)
	{
		std::vector<CDictOperand> arrOperand;
		while (pCFFData <= pCFFDataEnd)
		{
			BYTE nOp = *pCFFData++;
			if (nOp <= 27 || nOp == 31)
			{ // operator
				unsigned short nOperator = 0;
				if (nOp == 12)
					nOperator = (nOp << 8) | *pCFFData++;
				TopDICT.mTopDict[nOperator] = arrOperand;
				arrOperand.clear();
			}
			else
			{ // operand
				CDictOperand oOperand;
				if (nOp == 30) // real
				{
					double powerPart = 0, fractionPart = 0, fractionDecimal = 1, integerPart = 0;
					bool notDone = true, hasFraction = false, hasPositivePower = false, hasNegativePower = false, hasNegative = false;
					do
					{
						BYTE nByte = *pCFFData++;
						BYTE nibble[2];
						nibble[0] = (nByte >> 4) & 0xf;
						nibble[1] = nByte & 0xf;
						for (int i = 0; i < 2; ++i)
						{
							switch (nibble[i])
							{
							case 0xa:
								hasFraction = true;
								break;
							case 0xb:
								hasPositivePower = true;
								break;
							case 0xc:
								hasNegativePower = true;
								break;
							case 0xd:
								break; // reserved
							case 0xe:
								hasNegative = true;
								break;
							case 0xf:
								notDone = false;
								break;
							default: // numbers
								if (hasPositivePower || hasNegativePower)
									powerPart = powerPart * 10 + nibble[i];
								else if (hasFraction)
								{
									fractionPart = fractionPart * 10 + nibble[i];
									fractionDecimal *= 10;
									++oOperand.nRealFractalEnd;
								}
								else
									integerPart = integerPart * 10 + nibble[i];
								break;
							}
						}
					} while (notDone);

					oOperand.bIsInteger = false;
					oOperand.dRealValue = integerPart + fractionPart / fractionDecimal;
					if (hasNegativePower || hasPositivePower)
						oOperand.dRealValue = oOperand.dRealValue * pow(10, hasNegativePower ? -powerPart : powerPart);
					if (hasNegative)
						oOperand.dRealValue = -oOperand.dRealValue;
				}
				else if (nOp == 28)
				{
					oOperand.nIntegerValue = (pCFFData[0] << 8) | pCFFData[1];
					pCFFData += 2;
				}
				else if (nOp == 29)
				{
					oOperand.nIntegerValue = (pCFFData[0] << 24) | (pCFFData[1] << 16) | (pCFFData[2] << 8) | pCFFData[3];
					pCFFData += 4;
				}
				else if (nOp >= 32 && nOp <= 246)
				{
					oOperand.nIntegerValue = nOp - 139;
				}
				else if (nOp >= 247 && nOp <= 250)
				{
					BYTE nByte = *pCFFData++;
					oOperand.nIntegerValue = (nOp - 247) * 256 + nByte + 108;
				}
				else if (nOp >= 251 && nOp <= 254)
				{
					BYTE nByte = *pCFFData++;
					oOperand.nIntegerValue = -(nOp - 251) * 256 - nByte - 108;
				}
				arrOperand.push_back(oOperand);
			}
		}
	}
	void SetOrWriteNibble(BYTE nValue, BYTE& bBuffer, bool& bUsedFirst, CStream* pOutputStream)
	{
		if (bUsedFirst)
		{
			bBuffer |= nValue;
			pOutputStream->WriteUChar(bBuffer);
			bBuffer = 0;
			bUsedFirst = false;
		}
		else
		{
			bBuffer = (nValue << 4) & 0xf0;
			bUsedFirst = true;
		}
	}
	void WriteIntegerOfReal(double dIntegerValue, BYTE& bBuffer, bool& bUsedFirst, CStream* pOutputStream)
	{
		if (dIntegerValue == 0)
			return;
		WriteIntegerOfReal(floor(dIntegerValue / 10.0), bBuffer, bUsedFirst, pOutputStream);
		SetOrWriteNibble(long(dIntegerValue) % 10, bBuffer, bUsedFirst, pOutputStream);
	}
	void WriteDictOperator(unsigned short nOperator, CStream* pOutputStream)
	{
		if (((nOperator >> 8) & 0xff) == 12)
		{
			pOutputStream->WriteUChar((nOperator >> 8) & 0xff);
			pOutputStream->WriteUChar(nOperator & 0xff);
		}
		else
			pOutputStream->WriteUChar(nOperator & 0xff);
	}
	void WriteIntegerOperand(int nIntegerValue, CStream* pOutputStream)
	{
		if (nIntegerValue >= -107 && nIntegerValue <= 107)
			pOutputStream->WriteUChar(nIntegerValue + 139);
		else if (nIntegerValue >= 108 && nIntegerValue <= 1131)
		{
			long nValue = nIntegerValue - 108;
			pOutputStream->WriteUChar(((nValue >> 8) & 0xff) + 247);
			pOutputStream->WriteUChar(nValue & 0xff);
		}
		else if (nIntegerValue >= -1131 && nIntegerValue <= -108)
		{
			long nValue = -(nIntegerValue + 108);
			pOutputStream->WriteUChar(((nValue >> 8) & 0xff) + 251);
			pOutputStream->WriteUChar(nValue & 0xff);
		}
		else if (nIntegerValue >= -32768 && nIntegerValue <= 32767)
		{
			pOutputStream->WriteUChar(28);
			pOutputStream->WriteUChar((nIntegerValue >> 8) & 0xff);
			pOutputStream->WriteUChar(nIntegerValue & 0xff);
		}
		else // oOperand.nIntegerValue >= -2^31 && oOperand.nIntegerValue <= 2^31-1
		{
			pOutputStream->WriteUChar(29);
			pOutputStream->WriteUChar((nIntegerValue >> 24) & 0xff);
			pOutputStream->WriteUChar((nIntegerValue >> 16) & 0xff);
			pOutputStream->WriteUChar((nIntegerValue >> 8) & 0xff);
			pOutputStream->WriteUChar(nIntegerValue & 0xff);
		}
	}
	void WriteTopDICT(unsigned short nOperator, const std::vector<CDictOperand>& arrOperand, CStream* pOutputStream)
	{
		for (int i = 0; i < arrOperand.size(); ++i)
		{
			CDictOperand oOperand = arrOperand[i];
			if (oOperand.bIsInteger)
				WriteIntegerOperand(oOperand.nIntegerValue, pOutputStream);
			else
			{
				double dValue = oOperand.dRealValue;
				bool bMinusSign = dValue < 0, bMinusExponent = false, bPlusExponent = false;
				unsigned short usExponentSize = 0;

				if (bMinusSign)
					dValue = -dValue;
				double dIntegerValue = floor(dValue);
				double dFractalValue = dValue - dIntegerValue;

				if (dFractalValue == 0)
				{
					if (long(dIntegerValue) % 1000 == 0 && dIntegerValue >= 1000)
					{ // bother only if larger than 1000
						bPlusExponent = true;
						while (long(dIntegerValue) % 1000 == 0)
						{
							++usExponentSize;
							dIntegerValue = dIntegerValue / 10;
						}
					}
				}
				else if (dIntegerValue == 0)
				{
					if (dFractalValue <= 0.001)
					{ // bother only if < 0.001
						bMinusExponent = true;
						while (dFractalValue < 0.1)
						{
							++usExponentSize;
							dFractalValue = dFractalValue * 10;
						}
					}
				}

				pOutputStream->WriteUChar(30);

				BYTE bBuffer = bMinusSign ? 0xe0 : 0;
				bool bUsedFirst = bMinusSign;

				// Integer part
				if (dIntegerValue != 0)
					WriteIntegerOfReal(dIntegerValue, bBuffer, bUsedFirst, pOutputStream);
				else
					SetOrWriteNibble(0, bBuffer, bUsedFirst, pOutputStream);

				// Fractal part
				if (dFractalValue != 0 && oOperand.nRealFractalEnd > 0)
				{
					SetOrWriteNibble(0xa, bBuffer, bUsedFirst, pOutputStream);
					while (dFractalValue != 0 && oOperand.nRealFractalEnd > 0)
					{
						SetOrWriteNibble(floor(dFractalValue * 10), bBuffer, bUsedFirst, pOutputStream);
						dFractalValue = dFractalValue * 10 - floor(dFractalValue * 10);
						--oOperand.nRealFractalEnd;
					}
				}

				// Exponent part
				if (bMinusExponent)
				{
					SetOrWriteNibble(0xc, bBuffer, bUsedFirst, pOutputStream);
					WriteIntegerOfReal(usExponentSize, bBuffer, bUsedFirst, pOutputStream);
				}
				if (bPlusExponent)
				{
					SetOrWriteNibble(0xb, bBuffer, bUsedFirst, pOutputStream);
					WriteIntegerOfReal(usExponentSize, bBuffer, bUsedFirst, pOutputStream);
				}

				// Final part
				if (bUsedFirst)
					SetOrWriteNibble(0xf, bBuffer, bUsedFirst, pOutputStream);
				else
					pOutputStream->WriteUChar(0xff);
			}
		}
		WriteDictOperator(nOperator, pOutputStream);
	}
	void WritePad5Bytes(CStream* pOutputStream)
	{
		BYTE pBuffer[5] = { '0', '0', '0', '0', '0' };
		pOutputStream->Write(pBuffer, 5, false);
	}
	//----------------------------------------------------------------------------------------
	// CCFFReader
	//----------------------------------------------------------------------------------------
	struct CCFFReader
	{
		struct CFFHeader
		{
			BYTE major;
			BYTE minor;
			BYTE hdrSize;
			BYTE offSize;
		};
		typedef std::map<std::string,unsigned short> StringToUShort;
		struct DictOperand
		{
			bool IsInteger;
			long IntegerValue;
			double RealValue;
			long RealValueFractalEnd; // this fellow is here for writing, due to double being terribly inexact.
		};
		typedef std::list<DictOperand> DictOperandList;
		typedef std::map<unsigned short, DictOperandList> UShortToDictOperandListMap;
		enum ECharSetType
		{
			eCharSetISOAdobe = 0,
			eCharSetExpert,
			eCharSetExpertSubset,
			eCharSetCustom
		};
		struct IndexElement
		{
			IndexElement() { mStartPosition = 0; mEndPosition = 0; mIndex = 0; }

			long long mStartPosition;
			long long mEndPosition;
			unsigned short mIndex;
		};
		typedef IndexElement CharString;
		typedef std::map<unsigned short, CharString*> UShortToCharStringMap;
		struct CharSetInfo
		{
			CharSetInfo() {mSIDs = NULL;}

			ECharSetType mType;
			UShortToCharStringMap mSIDToGlyphMap;
			unsigned short* mSIDs; // count is like glyphs count
		};
		enum EEncodingType
		{
			eEncodingStandard = 0,
			eEncodingExpert,
			eEncodingCustom
		};
		typedef std::list<BYTE> ByteList;
		typedef std::map<unsigned short, ByteList> UShortToByteList;
		struct EncodingsInfo
		{
			EncodingsInfo() {mEncoding = NULL;}

			long long mEncodingStart;
			long long mEncodingEnd;

			EEncodingType mType;
			BYTE mEncodingsCount;
			BYTE* mEncoding;
			UShortToByteList mSupplements;

		};
		typedef CharString* CharStringsIndex;
		struct CharStrings
		{
			CharStrings(){mCharStringsIndex = NULL; mCharStringsType = 0; mCharStringsCount = 0;}

			BYTE mCharStringsType;
			unsigned short mCharStringsCount;
			CharStringsIndex mCharStringsIndex;
		};
		struct PrivateDictInfo
		{
			PrivateDictInfo() {mPrivateDictStart=0;mPrivateDictEnd=0;mLocalSubrs=NULL;}

			long long mPrivateDictStart;
			long long mPrivateDictEnd;
			UShortToDictOperandListMap mPrivateDict;
			CharStrings* mLocalSubrs;

		};
		struct FontDictInfo
		{
			long long mFontDictStart;
			long long mFontDictEnd;
			UShortToDictOperandListMap mFontDict;
			PrivateDictInfo mPrivateDict;
		};
		struct TopDictInfo
		{
			TopDictInfo()
			{
				mFDArray = NULL;
				mFDSelect = NULL;
				mCharSet = NULL;
				mEncoding = NULL;
			}

			UShortToDictOperandListMap mTopDict;
			CharSetInfo* mCharSet;
			EncodingsInfo* mEncoding;
			FontDictInfo* mFDArray;
			FontDictInfo** mFDSelect; // size is like glyphsize. each cell references the relevant FontDict
		};

		long long mCFFOffset;

		CFFHeader mHeader;
		unsigned short mFontsCount;
		std::list<std::string> mName;
		TopDictInfo* mTopDictIndex; // count is same as fonts count

		CMemoryStream* mPrimitivesReader; // внешний, освобождать не надо
		StringToUShort mNameToIndex;
		long long mNameIndexPosition;
		long long mTopDictIndexPosition;
	public:
		CCFFReader();
		~CCFFReader();

		// parses the whole CFF file, with all contained fonts
		bool ReadCFFFile(CMemoryStream* inCFFFile);

		void FreeData();
		void Reset();

		bool ReadIntegerOperand(BYTE inFirstByte, long& outValue);
		bool ReadRealOperand(double& outValue, long& outRealValueFractalEnd);
		bool IsDictOperator(BYTE inCandidate);
		bool ReadDictOperator(BYTE inFirstByte, unsigned short& outOperator);
		bool ReadDictOperand(BYTE inFirstByte, DictOperand& outOperand);

		bool ReadHeader();
		bool ReadIndexHeader(unsigned long** outOffsets,unsigned short& outItemsCount);
		bool ReadNameIndex();
		bool ReadTopDictIndex();
		bool ReadDict(unsigned long inReadAmount, UShortToDictOperandListMap& outDict);
	};
	CCFFReader::CCFFReader()
	{

	}
	CCFFReader::~CCFFReader()
	{
		FreeData();
	}
	void CCFFReader::FreeData()
	{
		// TODO
	}
	void CCFFReader::Reset()
	{
		FreeData();
	}

	bool CCFFReader::ReadIntegerOperand(BYTE inFirstByte, long& outValue)
	{
		BYTE byte0, byte1;
		bool status = true;

		byte0 = inFirstByte;

		if (byte0 >= 32 && byte0 <= 246)
		{
			outValue = (long)byte0 - 139;
		}
		else if (byte0 >= 247 && byte0 <= 250)
		{
			byte1 = mPrimitivesReader->ReadUChar();
			if (mPrimitivesReader->IsEof())
				return false;

			outValue = (byte0 - 247) * 256 + byte1 + 108;
		}
		else if (byte0 >= 251 && byte0 <= 254)
		{
			byte1 = mPrimitivesReader->ReadUChar();
			if (mPrimitivesReader->IsEof())
				return false;

			outValue = -(long)((long)byte0 - 251) * 256 - byte1 - 108;
		}
		else if (28 == byte0)
		{
			short buffer = 0;
			status = Read2ByteSigned(buffer);
			outValue = buffer;
		}
		else if(29 == byte0)
		{
			status = Read4ByteSigned(outValue);
		}
		else
			status = false;

		return status;
	}
	bool CCFFReader::ReadRealOperand(double& outValue, long& outRealValueFractalEnd)
	{
		double integerPart = 0;
		double fractionPart = 0;
		double powerPart = 0;
		double result;
		bool hasNegative = false;
		bool hasFraction = false;
		bool hasPositivePower = false;
		bool hasNegativePower = false;
		bool notDone = true;
		double fractionDecimal = 1;
		outRealValueFractalEnd = 0;
		BYTE buffer;
		BYTE nibble[2];
		bool status = true;

		do
		{
			buffer = mPrimitivesReader->ReadUChar();
			if (mPrimitivesReader->IsEof())
				return false;

			nibble[0] = (buffer >> 4) & 0xf;
			nibble[1] = buffer & 0xf;

			for (int i = 0; i < 2; ++i)
			{
				switch (nibble[i])
				{
				case 0xa:
					hasFraction = true;
					break;
				case 0xb:
					hasPositivePower = true;
					break;
				case 0xc:
					hasNegativePower = true;
					break;
				case 0xd:
					// reserved
					break;
				case 0xe:
					hasNegative = true;
					break;
				case 0xf:
					notDone = false;
					break;
				default: // numbers
					if (hasPositivePower || hasNegativePower)
					{
						powerPart = powerPart * 10 + nibble[i];
					}
					else if(hasFraction)
					{
						fractionPart = fractionPart * 10 + nibble[i];
						fractionDecimal *= 10;
						++outRealValueFractalEnd;
					}
					else
						integerPart = integerPart * 10 + nibble[i];

				}
			}
		} while(notDone);

		if (status)
		{
			result = integerPart + fractionPart/fractionDecimal;
			if(hasNegativePower || hasPositivePower)
				result = result * pow(10,hasNegativePower ? -powerPart : powerPart);
			if(hasNegative)
				result = -1*result;
			outValue = result;
		}
		return status;
	}
	bool CCFFReader::IsDictOperator(BYTE inCandidate)
	{
		return (inCandidate <= 27 || 31 == inCandidate);
	}
	bool CCFFReader::ReadDictOperator(BYTE inFirstByte, unsigned short& outOperator)
	{
		if (12 == inFirstByte)
		{
			BYTE buffer = mPrimitivesReader->ReadUChar();
			if (!mPrimitivesReader->IsEof())
			{
				outOperator = ((unsigned short)inFirstByte << 8) | buffer;
				return true;
			}
			return false;
		}
		outOperator = inFirstByte;
		return true;
	}
	bool CCFFReader::ReadDictOperand(BYTE inFirstByte, DictOperand& outOperand)
	{
		if (30 == inFirstByte) // real
		{
			outOperand.IsInteger = false;
			return ReadRealOperand(outOperand.RealValue, outOperand.RealValueFractalEnd);
		}
		else if (28 == inFirstByte ||
				 29 == inFirstByte ||
				(32 <= inFirstByte  && inFirstByte <= 246) ||
				(247 <= inFirstByte && inFirstByte <= 250) ||
				(251 <= inFirstByte && inFirstByte <= 254))
		{
			outOperand.IsInteger = true;
			return ReadIntegerOperand(inFirstByte, outOperand.IntegerValue);
		}
		return false; // not an operand
	}

	bool CCFFReader::ReadCFFFile(CMemoryStream* inCFFFile)
	{
		FreeData();

		mPrimitivesReader = inCFFFile;
		mCFFOffset = mPrimitivesReader->Tell();

		bool status = ReadHeader();
		if (!status)
			return false;

		if (mHeader.hdrSize > 4)
			mPrimitivesReader->Seek(mHeader.hdrSize - 4, SeekCur);

		status = ReadNameIndex();
		if (!status)
			return false;

		status = ReadTopDictIndex();
		if (!status)
			return false;

		// TODO
		return false;
	}
	bool CCFFReader::ReadHeader()
	{
		mHeader.major = mPrimitivesReader->ReadUChar();
		mHeader.minor = mPrimitivesReader->ReadUChar();
		mHeader.hdrSize = mPrimitivesReader->ReadUChar();
		mHeader.offSize = mPrimitivesReader->ReadUChar();

		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadIndexHeader(unsigned long** outOffsets, unsigned short& outItemsCount)
	{
		outItemsCount = mPrimitivesReader->ReadUShort();
		if (!mPrimitivesReader->IsEof())
			return false;

		if (0 == outItemsCount)
		{
			*outOffsets = NULL;
			return true;
		}

		BYTE offSizeForIndex = mPrimitivesReader->ReadUChar();
		*outOffsets = new unsigned long[outItemsCount + 1];

		for (unsigned short i = 0; i <= outItemsCount && !mPrimitivesReader->IsEof(); ++i)
			(*outOffsets)[i] = mPrimitivesReader->ReadOffset(offSizeForIndex);

		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadNameIndex()
	{
		mNameIndexPosition = mPrimitivesReader->Tell();

		unsigned long* offsets = NULL;
		bool status = ReadIndexHeader(&offsets, mFontsCount);
		if (!status || !offsets)
			return false;

		if (offsets[0] != 1)
			mPrimitivesReader->Seek(offsets[0] - 1, SeekCur);

		BYTE* buffer;
		for (unsigned short i = 0; i < mFontsCount; ++i)
		{
			unsigned int nLength = offsets[i + 1] - offsets[i];
			buffer = new BYTE[nLength];
			mPrimitivesReader->Read(buffer, &nLength);
			std::string aName((char*)buffer, nLength);
			mName.push_back(aName);
			if (buffer[0] != 0) // put in map only valid names
				mNameToIndex.insert(StringToUShort::value_type(aName, i));
			delete[] buffer;
		}

		delete[] offsets;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadTopDictIndex()
	{
		mTopDictIndexPosition = mPrimitivesReader->Tell();

		unsigned long* offsets;
		unsigned short dictionariesCount;
		bool status = ReadIndexHeader(&offsets, dictionariesCount);
		if (!status || !offsets)
			return false;

		if (offsets[0] != 1)
			mPrimitivesReader->Seek(offsets[0] - 1, SeekCur);

		mTopDictIndex = new TopDictInfo[dictionariesCount];

		for (unsigned short i = 0; i < dictionariesCount && status == true; ++i)
			status = ReadDict(offsets[i+1] - offsets[i], mTopDictIndex[i].mTopDict);

		delete[] offsets;
		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadDict(unsigned long inReadAmount, UShortToDictOperandListMap& outDict)
	{
		long long dictStartPosition = mPrimitivesReader->Tell();
		DictOperandList operands;
		bool status = true;
		unsigned short anOperator;
		DictOperand anOperand;
		BYTE aBuffer;

		while (status && (mPrimitivesReader->Tell() - dictStartPosition < (long long)inReadAmount))
		{
			aBuffer = mPrimitivesReader->ReadUChar();
			if (!mPrimitivesReader->IsEof())
				return false;
			if (IsDictOperator(aBuffer))
			{ // operator
				status = ReadDictOperator(aBuffer, anOperator);
				if (!status)
					break;
				outDict.insert(UShortToDictOperandListMap::value_type(anOperator, operands));
				operands.clear();
			}
			else // operand
			{
				status = ReadDictOperand(aBuffer, anOperand);
				if (!status)
					break;
				operands.push_back(anOperand);
			}
		}

		return status;
	}
	//----------------------------------------------------------------------------------------
	// COpenTypeReader
	//----------------------------------------------------------------------------------------
	struct COpenTypeReader
	{
		enum class EOpenTypeInputType
		{
			EOpenTypeTrueType,
			EOpenTypeCFF
		};
		struct TableEntry
		{
			unsigned long CheckSum;
			unsigned long Offset;
			unsigned long Length;
		};
		typedef std::map<unsigned int, TableEntry> UIntToTableEntryMap;
		struct HeadTable
		{
			double TableVersionNumber;
			double FontRevision;
			unsigned long CheckSumAdjustment;
			unsigned long MagicNumber;
			unsigned short Flags;
			unsigned short UnitsPerEm;
			long long Created;
			long long Modified;
			short XMin;
			short YMin;
			short XMax;
			short YMax;
			unsigned short MacStyle;
			unsigned short LowerRectPPEM;
			short FontDirectionHint;
			short IndexToLocFormat;
			short GlyphDataFormat;
		};
		struct MaxpTable
		{
			double TableVersionNumber;
			unsigned short NumGlyphs;
			unsigned short MaxPoints;
			unsigned short MaxCountours;
			unsigned short MaxCompositePoints;
			unsigned short MaxCompositeContours;
			unsigned short MaxZones;
			unsigned short MaxTwilightPoints;
			unsigned short MaxStorage;
			unsigned short MaxFunctionDefs;
			unsigned short MaxInstructionDefs;
			unsigned short MaxStackElements;
			unsigned short MaxSizeOfInstructions;
			unsigned short MaxComponentElements;
			unsigned short MaxCompontentDepth;
		};
		struct HHeaTable
		{
			double TableVersionNumber;
			short Ascender;
			short Descender;
			short LineGap;
			unsigned short AdvanceWidthMax;
			short MinLeftSideBearing;
			short MinRightSideBearing;
			short XMaxExtent;
			short CaretSlopeRise;
			short CaretSlopeRun;
			short CaretOffset;
			short MetricDataFormat;
			unsigned short NumberOfHMetrics;
		};
		struct HMtxTableEntry
		{
			unsigned short AdvanceWidth;
			short LeftSideBearing;
		};
		typedef HMtxTableEntry* HMtxTable;
		struct OS2Table
		{
			unsigned short Version;
			short AvgCharWidth;
			unsigned short WeightClass;
			unsigned short WidthClass;
			unsigned short fsType;
			short SubscriptXSize;
			short SubscriptYSize;
			short SubscriptXOffset;
			short SubscriptYOffset;
			short SuperscriptXSize;
			short SuperscriptYSize;
			short SuperscriptXOffset;
			short SuperscriptYOffset;
			short StrikeoutSize;
			short StrikeoutPosition;
			short FamilyClass;
			BYTE Panose[10];
			unsigned long UnicodeRange1;
			unsigned long UnicodeRange2;
			unsigned long UnicodeRange3;
			unsigned long UnicodeRange4;
			char AchVendID[4];
			unsigned short FSSelection;
			unsigned short FirstCharIndex;
			unsigned short LastCharIndex;
			short TypoAscender;
			short TypoDescender;
			short TypoLineGap;
			unsigned short WinAscent;
			unsigned short WinDescent;
			unsigned long CodePageRange1;
			unsigned long CodePageRange2;
			short XHeight;
			short CapHeight;
			unsigned short DefaultChar;
			unsigned short BreakChar;
			unsigned short MaxContext;
		};
		struct NameTableEntry
		{
			unsigned short PlatformID;
			unsigned short EncodingID;
			unsigned short LanguageID;
			unsigned short NameID;
			unsigned short Length;
			unsigned short Offset;
			BYTE* String;
		};
		struct NameTable
		{
			unsigned short mNameEntriesCount;
			NameTableEntry* mNameEntries;
		};
		typedef unsigned long* LocaTable;
		// this time it's gonna be just what's intersting for my subsetting purposes - which is the dependencies ('n some other stuff)
		struct GlyphEntry
		{
			short NumberOfContours;
			short XMin;
			short YMin;
			short XMax;
			short YMax;
			std::list<unsigned int> mComponentGlyphs; // will be empty for simple glyphs, and with component glyph indexes for components
		};
		typedef GlyphEntry** GlyfTable;
		typedef std::map<unsigned short, GlyphEntry*> UShortToGlyphEntryMap;

		unsigned long mHeaderOffset;
		unsigned long mTableOffset;

		unsigned short mFaceIndex;

		HeadTable mHead;
		MaxpTable mMaxp;
		HHeaTable mHHea;
		HMtxTable mHMtx;
		OS2Table mOS2;
		NameTable mName;
		LocaTable mLoca;
		GlyfTable mGlyf;

		// OS2 (surprise may not always exist. in dfonts for instance)
		bool mOS2Exists;

		// not read, but can tell if they are there
		bool mCVTExists;
		bool mFPGMExists;
		bool mPREPExists;

		CCFFReader mCFF;

		CMemoryStream* mPrimitivesReader;
		EOpenTypeInputType mFontType;
		unsigned short mTablesCount;
		UIntToTableEntryMap mTables;
		UShortToGlyphEntryMap mActualGlyphs;

	public:
		COpenTypeReader();
		~COpenTypeReader();

		void FreeTables();
		unsigned short GetGlyphsCount();
		TableEntry* GetTableEntry(const char* inTagName);
		EOpenTypeInputType GetOpenTypeFontType();

		bool ReadOpenTypeFile(BYTE* pData, unsigned int nDataLength, unsigned short ushFaceIndex);
		bool ReadOpenTypeHeader();
		bool ReadOpenTypeSFNT();
		bool ReadOpenTypeSFNTFromDfont();
		bool ReadHead();
		bool ReadMaxP();
		bool ReadHHea();
		bool ReadHMtx();
		bool ReadOS2();
		bool ReadName();
		bool ReadLoca();
		bool ReadGlyfForDependencies();
		bool ReadCFF();
		unsigned long GetTag(const char* inTagName);
	};
	COpenTypeReader::COpenTypeReader()
	{
		mHeaderOffset = 0;
		mTableOffset = 0;
		mHMtx = NULL;
		mName.mNameEntries = NULL;
		mLoca = NULL;
		mGlyf = NULL;

		mPrimitivesReader = NULL;
		mFaceIndex = 0;
		mFontType = EOpenTypeInputType::EOpenTypeCFF;
		mOS2Exists = false;
	}
	COpenTypeReader::~COpenTypeReader()
	{
		FreeTables();
	}
	void COpenTypeReader::FreeTables()
	{
		RELEASEOBJECT(mPrimitivesReader);
		RELEASEARRAYOBJECTS(mHMtx);
		if (mName.mNameEntries)
		{
			for (int i = 0; i < mName.mNameEntriesCount; ++i)
				RELEASEARRAYOBJECTS(mName.mNameEntries[i].String);
		}
		RELEASEARRAYOBJECTS(mName.mNameEntries);
		RELEASEARRAYOBJECTS(mLoca);
		RELEASEARRAYOBJECTS(mGlyf);
		UShortToGlyphEntryMap::iterator it = mActualGlyphs.begin();
		for(; it != mActualGlyphs.end(); ++it)
			RELEASEOBJECT(it->second);
		mActualGlyphs.clear();
	}
	unsigned short COpenTypeReader::GetGlyphsCount()
	{
		return mMaxp.NumGlyphs;
	}
	COpenTypeReader::TableEntry* COpenTypeReader::GetTableEntry(const char* inTagName)
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag(inTagName));
		if (it == mTables.end())
			return NULL;
		else
			return &(it->second);
	}
	COpenTypeReader::EOpenTypeInputType COpenTypeReader::GetOpenTypeFontType()
	{
		return mFontType;
	}
	bool COpenTypeReader::ReadOpenTypeFile(BYTE* pData, unsigned int nDataLength, unsigned short ushFaceIndex)
	{
		mFaceIndex = ushFaceIndex;

		FreeTables();

		mPrimitivesReader = new CMemoryStream(nDataLength);
		mPrimitivesReader->Write(pData, nDataLength);
		mPrimitivesReader->Seek(0, SeekSet);

		mHeaderOffset = mPrimitivesReader->Tell();
		mTableOffset  = mPrimitivesReader->Tell();

		bool status = ReadOpenTypeHeader();
		if (!status)
			return false;

		status = ReadHead();
		if (!status)
			return false;

		status = ReadMaxP();
		if (!status)
			return false;

		status = ReadHHea();
		if (!status)
			return false;

		status = ReadHMtx();
		if (!status)
			return false;

		status = ReadOS2(); // Note that OS/2 is supposedly required, but some dfonts don't contain it...and it's fine
		if (!status)
			return false;

		status = ReadName();
		if (!status)
			return false;

		if (EOpenTypeInputType::EOpenTypeTrueType == mFontType)
		{
			// true type specifics
			status = ReadLoca();
			if (!status)
				return false;

			status = ReadGlyfForDependencies();
			if (!status)
				return false;
			mCVTExists  = mTables.find(GetTag("cvt ")) != mTables.end();
			mFPGMExists = mTables.find(GetTag("fpgm")) != mTables.end();
			mPREPExists = mTables.find(GetTag("prep")) != mTables.end();

			// zero cff items
			mCFF.Reset();
		}
		else
		{
			// CFF specifics
			status = ReadCFF();
			if (!status)
				return false;

			// zero true type items
			mCVTExists = false;
			mFPGMExists = false;
			mPREPExists = false;
			mGlyf = NULL;
			mLoca = NULL;
		}

		return status;
	}
	bool COpenTypeReader::ReadOpenTypeHeader()
	{
		bool status;
		TableEntry tableEntry;
		unsigned int tableTag;

		status = ReadOpenTypeSFNT();
		if (!status)
			return false;

		mPrimitivesReader->Seek(mHeaderOffset, SeekSet);
		unsigned int sfntVersion = mPrimitivesReader->ReadUInt();
		mTablesCount = mPrimitivesReader->ReadUShort();
		// skip the next 6
		mPrimitivesReader->Seek(6, SeekCur);

		for (unsigned short i = 0; i < mTablesCount; ++i)
		{
			tableTag = mPrimitivesReader->ReadUInt();
			tableEntry.CheckSum = mPrimitivesReader->ReadUInt();
			tableEntry.Offset = mPrimitivesReader->ReadUInt();
			tableEntry.Length = mPrimitivesReader->ReadUInt();
			tableEntry.Offset += mTableOffset;
			mTables.insert(UIntToTableEntryMap::value_type(tableTag, tableEntry));
		}
		status = !mPrimitivesReader->IsEof();

		return status;
	}
	bool COpenTypeReader::ReadOpenTypeSFNT()
	{
		mPrimitivesReader->Seek(mHeaderOffset, EWhenceMode::SeekSet);
		unsigned int sfntVersion = mPrimitivesReader->ReadUInt();

		if (mPrimitivesReader->IsEof())
			return false;

		if (0x74746366 /* ttcf */ == sfntVersion)
		{
			// mgubi: a TrueType composite font, just get to the right face table
			// for the format see http://www.microsoft.com/typography/otspec/otff.htm
			unsigned int ttcVersion = mPrimitivesReader->ReadUInt();
			unsigned int numFonts   = mPrimitivesReader->ReadUInt();

			if (mFaceIndex >= numFonts)
				return false;

			unsigned int offsetTable;
			for (int i = 0; i <= mFaceIndex; ++i)
				offsetTable = mPrimitivesReader->ReadUInt();

			mHeaderOffset = mHeaderOffset + offsetTable;

			return ReadOpenTypeSFNT();
		}
		else if ((0x10000 == sfntVersion) || (0x74727565 /* true */ == sfntVersion))
		{
			mFontType = EOpenTypeInputType::EOpenTypeTrueType;
			return true;
		}
		else if (0x4F54544F /* OTTO */ == sfntVersion)
		{
			mFontType = EOpenTypeInputType::EOpenTypeCFF;
			return true;
		}
		else if (ReadOpenTypeSFNTFromDfont())
			return true;
		return false;
	}
	bool COpenTypeReader::ReadOpenTypeSFNTFromDfont()
	{
		bool status = true;
		// mac resource fork header parsing
		// see: https://developer.apple.com/legacy/mac/library/documentation/mac/pdf/MoreMacintoshToolbox.pdf

		unsigned int rdata_pos, map_pos, rdata_len, map_offset;
		// verify that the header is composed as expected
		BYTE head[16], head2[16];

		mPrimitivesReader->Seek(mHeaderOffset, SeekSet);

		for (unsigned short i = 0; i < 16; i++)
			head[i] = mPrimitivesReader->ReadUChar();

		if (mPrimitivesReader->IsEof())
			return false;

		rdata_pos = ( head[0] << 24 )  | ( head[1] << 16 )  | ( head[2] <<  8 )  | head[3] ;
		map_pos   = ( head[4] << 24 )  | ( head[5] << 16 )  | ( head[6] <<  8 )  | head[7] ;
		rdata_len = ( head[8] << 24 )  | ( head[9] << 16 )  | ( head[10] << 8 )  | head[11];

		mPrimitivesReader->Seek(map_pos, SeekSet);

		for (unsigned short i = 0; i < 16; i++)
			head2[i] = mPrimitivesReader->ReadUChar();
		if (mPrimitivesReader->IsEof())
			return false;

		/* If we have reached this point then it is probably a mac resource */
		/* file.  Now, does it contain any interesting resources?           */

		mPrimitivesReader->Seek(4   /* skip handle to next resource map */
							   + 2  /* skip file resource number */
							   + 2  /* skip attributes */
							   , SeekCur);

		unsigned short type_list = mPrimitivesReader->ReadUShort();
		if (mPrimitivesReader->IsEof())
			return false;

		map_offset = map_pos + type_list;

		mPrimitivesReader->Seek(map_offset, SeekSet);

		// read the resource type list

		unsigned short cnt = mPrimitivesReader->ReadUShort();
		if (mPrimitivesReader->IsEof())
			return false;

		bool foundSfnt = false;

		for (int i = 0; i < cnt + 1 && !mPrimitivesReader->IsEof() && !foundSfnt; ++i)
		{
			unsigned short subcnt, rpos;
			int tag = mPrimitivesReader->ReadUInt();
			if (mPrimitivesReader->IsEof())
				break;
			subcnt = mPrimitivesReader->ReadUShort();
			if (mPrimitivesReader->IsEof())
				break;
			rpos = mPrimitivesReader->ReadUShort();
			if (mPrimitivesReader->IsEof())
				break;

			if ( (unsigned long)tag == GetTag("sfnt") )
			{
				mPrimitivesReader->Seek(map_offset + rpos, SeekSet);

				// read the reference list for the 'sfnt' resources
				// the map is used to order the references by reference id

				std::map<unsigned short, unsigned long> resOffsetsMap;

				for (int j = 0; j < subcnt + 1 && !mPrimitivesReader->IsEof(); ++j )
				{
					unsigned short res_id, res_name;
					unsigned int temp, mbz, res_offset;
					res_id = mPrimitivesReader->ReadUShort();
					if (mPrimitivesReader->IsEof())
						break;
					res_name = mPrimitivesReader->ReadUShort();
					if (mPrimitivesReader->IsEof())
						break;
					temp = mPrimitivesReader->ReadUInt();
					if (mPrimitivesReader->IsEof())
						break;
					mbz = mPrimitivesReader->ReadUInt();
					if (mPrimitivesReader->IsEof())
						break;
					res_offset = temp & 0xFFFFFFL;
					resOffsetsMap.insert(std::pair<unsigned short, unsigned long>(res_id,rdata_pos + res_offset));
				}
				if (mPrimitivesReader->IsEof())
					break;

				int face_index = mFaceIndex, cur_face = 0;
				unsigned long fontOffset = 0;

				for (std::map<unsigned short, unsigned long>::iterator it = resOffsetsMap.begin(); it != resOffsetsMap.end(); ++it, ++cur_face)
				{
					if (cur_face == face_index)
					{
						fontOffset = it->second;
						break;
					}
				}

				if (cur_face != face_index)
				{
					status = false;
					break;
				}


				mHeaderOffset = fontOffset + 4; // skip the size of the resource
				mTableOffset = mHeaderOffset;

				// try to open the resource as a TrueType font specification
				foundSfnt = true;
			}
		}

		if (status && foundSfnt)
			return ReadOpenTypeSFNT();
		return false;
	}
	unsigned long COpenTypeReader::GetTag(const char* inTagName)
	{
		BYTE buffer[4];
		unsigned short i = 0;

		for(; i < strlen(inTagName); ++i)
			buffer[i] = (BYTE)inTagName[i];
		for(; i < 4; ++i)
			buffer[i] = 0x20;

		return	((unsigned long)buffer[0] << 24) + ((unsigned long)buffer[1] << 16) +
				((unsigned long)buffer[2] << 8) + buffer[3];
	}
	bool COpenTypeReader::ReadHead()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("head"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);
		mHead.TableVersionNumber = mPrimitivesReader->ReadFixed();
		mHead.FontRevision       = mPrimitivesReader->ReadFixed();
		mHead.CheckSumAdjustment = mPrimitivesReader->ReadUInt();
		mHead.MagicNumber        = mPrimitivesReader->ReadUInt();
		mHead.Flags              = mPrimitivesReader->ReadUShort();
		mHead.UnitsPerEm         = mPrimitivesReader->ReadUShort();
		mHead.Created            = mPrimitivesReader->ReadLongDateTime();
		mHead.Modified           = mPrimitivesReader->ReadLongDateTime();
		mHead.XMin               = mPrimitivesReader->ReadUShort();
		mHead.YMin               = mPrimitivesReader->ReadUShort();
		mHead.XMax               = mPrimitivesReader->ReadUShort();
		mHead.YMax               = mPrimitivesReader->ReadUShort();
		mHead.MacStyle           = mPrimitivesReader->ReadUShort();
		mHead.LowerRectPPEM      = mPrimitivesReader->ReadUShort();
		mHead.FontDirectionHint  = mPrimitivesReader->ReadUShort();
		mHead.IndexToLocFormat   = mPrimitivesReader->ReadUShort();
		mHead.GlyphDataFormat    = mPrimitivesReader->ReadUShort();

		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadMaxP()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("maxp"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		memset(&mMaxp, 0, sizeof(MaxpTable)); // set all with 0's in case the table's too short, so we'll have nice lookin values

		mMaxp.TableVersionNumber = mPrimitivesReader->ReadFixed();
		mMaxp.NumGlyphs = mPrimitivesReader->ReadUShort();

		if (1.0 == mMaxp.TableVersionNumber)
		{
			mMaxp.MaxPoints             = mPrimitivesReader->ReadUShort();
			mMaxp.MaxCountours          = mPrimitivesReader->ReadUShort();
			mMaxp.MaxCompositePoints    = mPrimitivesReader->ReadUShort();
			mMaxp.MaxCompositeContours  = mPrimitivesReader->ReadUShort();
			mMaxp.MaxZones              = mPrimitivesReader->ReadUShort();
			mMaxp.MaxTwilightPoints     = mPrimitivesReader->ReadUShort();
			mMaxp.MaxStorage            = mPrimitivesReader->ReadUShort();
			mMaxp.MaxFunctionDefs       = mPrimitivesReader->ReadUShort();
			mMaxp.MaxInstructionDefs    = mPrimitivesReader->ReadUShort();
			mMaxp.MaxStackElements      = mPrimitivesReader->ReadUShort();
			mMaxp.MaxSizeOfInstructions = mPrimitivesReader->ReadUShort();
			mMaxp.MaxComponentElements  = mPrimitivesReader->ReadUShort();
			mMaxp.MaxCompontentDepth    = mPrimitivesReader->ReadUShort();
		}
		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadHHea()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("hhea"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		mHHea.TableVersionNumber  = mPrimitivesReader->ReadFixed();
		mHHea.Ascender            = mPrimitivesReader->ReadUShort();
		mHHea.Descender           = mPrimitivesReader->ReadUShort();
		mHHea.LineGap             = mPrimitivesReader->ReadUShort();
		mHHea.AdvanceWidthMax     = mPrimitivesReader->ReadUShort();
		mHHea.MinLeftSideBearing  = mPrimitivesReader->ReadUShort();
		mHHea.MinRightSideBearing = mPrimitivesReader->ReadUShort();
		mHHea.XMaxExtent          = mPrimitivesReader->ReadUShort();
		mHHea.CaretSlopeRise      = mPrimitivesReader->ReadUShort();
		mHHea.CaretSlopeRun       = mPrimitivesReader->ReadUShort();
		mHHea.CaretOffset         = mPrimitivesReader->ReadUShort();
		mPrimitivesReader->Seek(8, SeekCur);
		mHHea.MetricDataFormat    = mPrimitivesReader->ReadUShort();
		mHHea.NumberOfHMetrics    = mPrimitivesReader->ReadUShort();

		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadHMtx()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("hmtx"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		mHMtx = new HMtxTableEntry[mMaxp.NumGlyphs];

		unsigned int i = 0;
		for(; i < mHHea.NumberOfHMetrics; ++i)
		{
			mHMtx[i].AdvanceWidth    = mPrimitivesReader->ReadUShort();
			mHMtx[i].LeftSideBearing = mPrimitivesReader->ReadUShort();
		}

		for(; i < mMaxp.NumGlyphs; ++i)
		{
			mHMtx[i].AdvanceWidth = mHMtx[mHHea.NumberOfHMetrics - 1].AdvanceWidth;
			mHMtx[i].LeftSideBearing = mPrimitivesReader->ReadUShort();
		}

		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadOS2()
	{
		memset(&mOS2, 0, sizeof(OS2Table));

		UIntToTableEntryMap::iterator it = mTables.find(GetTag("OS/2"));
		if (it == mTables.end())
		{
			mOS2Exists = false;
			return true;
		}

		mOS2Exists = true;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		mOS2.Version      = mPrimitivesReader->ReadUShort();
		mOS2.AvgCharWidth = mPrimitivesReader->ReadUShort();
		mOS2.WeightClass  = mPrimitivesReader->ReadUShort();
		mOS2.WidthClass   = mPrimitivesReader->ReadUShort();
		mOS2.fsType       = mPrimitivesReader->ReadUShort();

		mOS2.SubscriptXSize     = mPrimitivesReader->ReadUShort();
		mOS2.SubscriptYSize     = mPrimitivesReader->ReadUShort();
		mOS2.SubscriptXOffset   = mPrimitivesReader->ReadUShort();
		mOS2.SubscriptYOffset   = mPrimitivesReader->ReadUShort();
		mOS2.SuperscriptXSize   = mPrimitivesReader->ReadUShort();
		mOS2.SuperscriptYSize   = mPrimitivesReader->ReadUShort();
		mOS2.SuperscriptXOffset = mPrimitivesReader->ReadUShort();
		mOS2.SuperscriptYOffset = mPrimitivesReader->ReadUShort();
		mOS2.StrikeoutSize      = mPrimitivesReader->ReadUShort();
		mOS2.StrikeoutPosition  = mPrimitivesReader->ReadUShort();
		mOS2.FamilyClass        = mPrimitivesReader->ReadUShort();
		for (int i = 0; i < 10; ++i)
			mOS2.Panose[i] = mPrimitivesReader->ReadUChar();
		mOS2.UnicodeRange1 = mPrimitivesReader->ReadUInt();
		mOS2.UnicodeRange2 = mPrimitivesReader->ReadUInt();
		mOS2.UnicodeRange3 = mPrimitivesReader->ReadUInt();
		mOS2.UnicodeRange4 = mPrimitivesReader->ReadUInt();
		for (int i = 0; i < 4; ++i)
			mOS2.AchVendID[i] = mPrimitivesReader->ReadUChar();
		mOS2.FSSelection      = mPrimitivesReader->ReadUShort();
		mOS2.FirstCharIndex   = mPrimitivesReader->ReadUShort();
		mOS2.LastCharIndex    = mPrimitivesReader->ReadUShort();
		mOS2.TypoAscender     = mPrimitivesReader->ReadUShort();
		mOS2.TypoDescender    = mPrimitivesReader->ReadUShort();
		mOS2.TypoLineGap      = mPrimitivesReader->ReadUShort();
		mOS2.WinAscent        = mPrimitivesReader->ReadUShort();
		mOS2.WinDescent       = mPrimitivesReader->ReadUShort();

		// version 1 OS/2 table may end here [see that there's enough to continue]
		if (it->second.Length >= (mPrimitivesReader->Tell() - it->second.Offset) + 18)
		{
			mOS2.CodePageRange1 = mPrimitivesReader->ReadUInt();
			mOS2.CodePageRange2 = mPrimitivesReader->ReadUInt();
			mOS2.XHeight        = mPrimitivesReader->ReadUShort();
			mOS2.CapHeight      = mPrimitivesReader->ReadUShort();
			mOS2.DefaultChar    = mPrimitivesReader->ReadUShort();
			mOS2.BreakChar      = mPrimitivesReader->ReadUShort();
			mOS2.MaxContext     = mPrimitivesReader->ReadUShort();
		}
		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadName()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("name"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset + 2, SeekSet);
		mName.mNameEntriesCount = mPrimitivesReader->ReadUShort();
		mName.mNameEntries = new NameTableEntry[mName.mNameEntriesCount];

		unsigned short stringOffset = mPrimitivesReader->ReadUShort();

		for (int i = 0; i < mName.mNameEntriesCount; ++i)
		{
			mName.mNameEntries[i].PlatformID = mPrimitivesReader->ReadUShort();
			mName.mNameEntries[i].EncodingID = mPrimitivesReader->ReadUShort();
			mName.mNameEntries[i].LanguageID = mPrimitivesReader->ReadUShort();
			mName.mNameEntries[i].NameID = mPrimitivesReader->ReadUShort();
			mName.mNameEntries[i].Length = mPrimitivesReader->ReadUShort();
			mName.mNameEntries[i].Offset = mPrimitivesReader->ReadUShort();
		}

		for (int i = 0; i < mName.mNameEntriesCount; ++i)
		{
			mName.mNameEntries[i].String = new BYTE[mName.mNameEntries[i].Length];
			mPrimitivesReader->Seek(it->second.Offset + stringOffset + mName.mNameEntries[i].Offset, SeekSet);
			unsigned int nLength = mName.mNameEntries[i].Length;
			mPrimitivesReader->Read(mName.mNameEntries[i].String, &nLength);
			mName.mNameEntries[i].Length = nLength;
		}

		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadLoca()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("loca"));
		if (it == mTables.end())
			return false;
		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		mLoca = new unsigned long[mMaxp.NumGlyphs + 1];

		if (0 == mHead.IndexToLocFormat)
		{
			unsigned short buffer;
			for (int i = 0; i < mMaxp.NumGlyphs + 1; ++i)
			{
				buffer = mPrimitivesReader->ReadUShort();
				mLoca[i] = buffer << 1;
			}
		}
		else
		{
			for (int i = 0; i < mMaxp.NumGlyphs + 1; ++i)
				mLoca[i] = mPrimitivesReader->ReadUInt();
		}
		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadGlyfForDependencies()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("glyf"));
		if (it == mTables.end())
			return false;

		// it->second.Offset, is the offset to the beginning of the table
		mGlyf = new GlyphEntry*[mMaxp.NumGlyphs];

		for (int i = 0; i < mMaxp.NumGlyphs; ++i)
		{
			if (mLoca[i + 1] == mLoca[i])
				mGlyf[i] = NULL;
			else
			{
				mGlyf[i] = new GlyphEntry;

				mPrimitivesReader->Seek(it->second.Offset + mLoca[i], SeekSet);
				mGlyf[i]->NumberOfContours = mPrimitivesReader->ReadUShort();
				mGlyf[i]->XMin = mPrimitivesReader->ReadUShort();
				mGlyf[i]->YMin = mPrimitivesReader->ReadUShort();
				mGlyf[i]->XMax = mPrimitivesReader->ReadUShort();
				mGlyf[i]->YMax = mPrimitivesReader->ReadUShort();

				// Now look for dependencies
				if (mGlyf[i]->NumberOfContours < 0)
				{
					bool hasMoreComponents;
					unsigned short flags;
					unsigned short glyphIndex;

					do
					{
						flags = mPrimitivesReader->ReadUShort();
						glyphIndex = mPrimitivesReader->ReadUShort();

						if (glyphIndex >= mMaxp.NumGlyphs)
							return false;

						mGlyf[i]->mComponentGlyphs.push_back(glyphIndex);
						if ((flags & 1) != 0)
							mPrimitivesReader->Seek(4, SeekCur); // skip 2 shorts, ARG_1_AND_2_ARE_WORDS
						else
							mPrimitivesReader->Seek(2, SeekCur); // skip 1 short, nah - they are bytes

						if ((flags & 8) != 0)
							mPrimitivesReader->Seek(2, SeekCur); // WE_HAVE_SCALE
						else if ((flags & 64) != 0)
							mPrimitivesReader->Seek(4, SeekCur); // WE_HAVE_AN_X_AND_Y_SCALE
						else if ((flags & 128) != 0)
							mPrimitivesReader->Seek(8, SeekCur); // WE_HAVE_A_TWO_BY_TWO

						hasMoreComponents = ((flags & 32) != 0);
					} while(hasMoreComponents);

				}

				mActualGlyphs.insert(UShortToGlyphEntryMap::value_type(i, mGlyf[i]));
			}
		}

		return !mPrimitivesReader->IsEof();
	}
	bool COpenTypeReader::ReadCFF()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("CFF "));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		return mCFF.ReadCFFFile(mPrimitivesReader);
	}
	//----------------------------------------------------------------------------------------
	// CFontFileTrueType
	//----------------------------------------------------------------------------------------
	void CFontFileTrueType::WriteCIDFontType0C(CStream* pOutputStream, unsigned short* pCodeToGID, unsigned int unCodesCount, unsigned char* pUseGlyfs, long lGlyfsCount)
	{
		if (!m_bOpenTypeCFF)
		{
			// Если шрифт не является OpenType CFF, завершаем
			return;
		}

		COpenTypeReader mOpenTypeFile;
		bool status = mOpenTypeFile.ReadOpenTypeFile(m_sFile, m_nLen, m_unFontIndex);
		if (!status)
			return;

		int nOS2Index = SeekTable("OS/2");
		if (-1 != nOS2Index && m_pTables[nOS2Index].nLen > 0)
		{
			unsigned int unOffset = m_pTables[nOS2Index].nOffset;
			int nFSType = GetS16BE(unOffset + 8, &m_bSuccess);
			if (nFSType == 0x2 || nFSType == 0x0200 || nFSType == 0x0202)
				return;
		}
		else
			return;

		// Получаем данные таблицы CFF
		int nCFFIndex = SeekTable("CFF ");
		if (nCFFIndex < 0)
			return;

		TrueTypeTable* pCFFTable = &m_pTables[nCFFIndex];
		BYTE* pCFFData = m_sFile + pCFFTable->nOffset;
		BYTE* pCFFDataBegin = pCFFData;
		int nCFFLength = pCFFTable->nLen;

		// Проверяем, что данные CFF корректны
		if (nCFFLength < 4 || pCFFData[0] != 1 || pCFFData[1] != 0)
			return;

		// ------
		// Read&Write CFF Header
		// ------

		pOutputStream->WriteUChar(pCFFData[0]); // major version - 1
		pOutputStream->WriteUChar(pCFFData[1]); // minor version - 0
		pOutputStream->WriteUChar(pCFFData[2]); // header size - 4
		pOutputStream->WriteUChar(pCFFData[3]); // offset size - offsets - размер смещений в INDEX-таблицах - максимальный из размеров под offset-ы 1-4
		pCFFData += 4;

		// ------
		// Write Name Index
		// ------

		BYTE nSizeOfOffset = GetMostCompressedOffsetSize(m_sName.size() + 1);
		// Количество имён в таблице
		pOutputStream->WriteUChar(0); // count (MSB)
		pOutputStream->WriteUChar(1); // count (LSB)
		pOutputStream->WriteUChar(nSizeOfOffset); // offset size - размер смещений для доступа к данным 1-4
		// Массив смещений начала каждого имени + конец данных
		WriteOffset(1, nSizeOfOffset, pOutputStream);; // offset to first name
		WriteOffset(m_sName.size() + 1, nSizeOfOffset, pOutputStream); // offset to end of name
		// Массив имён шрифтов
		pOutputStream->WriteStr(m_sName.c_str());

		// ------
		// Read Name Index
		// ------

		unsigned short ushFontsCount = 0;
		pCFFData = SkipIndexHeader(pCFFData, ushFontsCount);

		// ------
		// Read Top DICT Index
		// ------

		unsigned int* pOffsets = NULL;
		unsigned short ushDictCount = 0;
		pCFFData = ReadIndexHeader(pCFFData, &pOffsets, ushDictCount);

		CTopDictInfo* pTopDICT = new CTopDictInfo[ushDictCount];
		for (unsigned short i = 0; i < ushDictCount; ++i)
			ReadTopDICT(pCFFData + pOffsets[i] - 1, pCFFData + pOffsets[i + 1] - 1, pTopDICT[i]);
		pCFFData += pOffsets[ushDictCount] - 1;
		RELEASEARRAYOBJECTS(pOffsets);

		// ------
		// Read String Index
		// ------

		unsigned short ushStringCount = 0;
		pCFFData = ReadIndexHeader(pCFFData, &pOffsets, ushStringCount);

		BYTE** pStrings = new BYTE*[ushStringCount];
		for (unsigned short i = 0; i < ushStringCount; ++i)
		{
			unsigned int unLength = pOffsets[i + 1] - pOffsets[i];
			pStrings[i] = new BYTE[unLength + 1];
			memcpy(pStrings[i], pCFFData + pOffsets[i] - 1, unLength);
			pStrings[i][unLength] = 0;
		}
		BYTE* pCFFDataStringIndex = pCFFData;
		int nCFFDataStringIndex = pOffsets[ushStringCount] - 1;
		pCFFData += pOffsets[ushStringCount] - 1;
		RELEASEARRAYOBJECTS(pOffsets);

		// PrivateDictStart
		unsigned short scPrivate = 18;
		int nPrivateDictStart = 0;
		const std::map<unsigned short, std::vector<CDictOperand>>& TopDICT = pTopDICT[0].mTopDict;
		std::map<unsigned short, std::vector<CDictOperand>>::const_iterator it = TopDICT.find(scPrivate);
		if (it != TopDICT.end())
			nPrivateDictStart = it->second.back().nIntegerValue;

		// ------
		// Write Top DICT Index
		// ------

		// Подготовка Top DICT segment
		CStream* pTopDictSegment = new CMemoryStream();
		unsigned short scROS = 0xC1E;
		unsigned short scCharset = 15;
		unsigned short scEncoding = 16;
		unsigned short scCharstrings = 17;
		unsigned short scFDArray = 0xC24;
		unsigned short scFDSelect = 0xC25;
		unsigned short scEmbeddedPostscript = 0xC15;
		it = TopDICT.find(scROS);
		bool bIsCID = it != TopDICT.end();
		if (bIsCID)
			WriteTopDICT(it->first, it->second, pTopDictSegment);

		for (it = TopDICT.begin(); it != TopDICT.end(); ++it)
		{
			if (it->first != scROS && it->first != scCharset && it->first != scEncoding && it->first != scCharstrings && it->first != scPrivate && it->first != scFDArray && it->first != scFDSelect)
				WriteTopDICT(it->first, it->second, pTopDictSegment);
		}

		std::string sOptionalEmbeddedPostscript;
		int nStdStrings = 391;
		if (TopDICT.find(scEmbeddedPostscript) == TopDICT.end() && nOS2Index > 0)
		{
			TrueTypeTable* pOS2Table = &m_pTables[nOS2Index];
			BYTE* pOS2Data = m_sFile + pOS2Table->nOffset;
			pOS2Data += 8; // skip
			unsigned short usType = (pOS2Data[0] << 8) | pOS2Data[1];
			sOptionalEmbeddedPostscript = "/FSType " + std::to_string(usType) + " def";
			WriteIntegerOperand(ushStringCount + nStdStrings, pTopDictSegment);
			WriteDictOperator(scEmbeddedPostscript, pTopDictSegment);
		}

		// Placeholders
		int nCharsetPlaceHolderPos = pTopDictSegment->Tell();
		WritePad5Bytes(pTopDictSegment);
		WriteDictOperator(scCharset, pTopDictSegment);
		int nCharstringsPlaceHolderPos = pTopDictSegment->Tell();
		WritePad5Bytes(pTopDictSegment);
		WriteDictOperator(scCharstrings, pTopDictSegment);
		int nPrivatePlaceHolderPos = 0;
		if (nPrivateDictStart != 0)
		{
			nPrivatePlaceHolderPos = pTopDictSegment->Tell();
			WritePad5Bytes(pTopDictSegment);
			WritePad5Bytes(pTopDictSegment);
			WriteDictOperator(scPrivate, pTopDictSegment);
		}
		int nEncodingPlaceHolderPos = 0;
		int nFDArrayPlaceHolderPos  = 0;
		int nFDSelectPlaceHolderPos = 0;
		if (bIsCID)
		{
			nFDArrayPlaceHolderPos = pTopDictSegment->Tell();
			WritePad5Bytes(pTopDictSegment);
			WriteDictOperator(scFDArray, pTopDictSegment);
			nFDSelectPlaceHolderPos = pTopDictSegment->Tell();
			WritePad5Bytes(pTopDictSegment);
			WriteDictOperator(scFDSelect, pTopDictSegment);
		}
		else
		{
			nEncodingPlaceHolderPos = pTopDictSegment->Tell();
			WritePad5Bytes(pTopDictSegment);
			WriteDictOperator(scEncoding, pTopDictSegment);
		}

		nSizeOfOffset = GetMostCompressedOffsetSize(pTopDictSegment->Size() + 1);
		// Количество Top DICT в таблице
		pOutputStream->WriteUChar(0); // count (MSB)
		pOutputStream->WriteUChar(1); // count (LSB)
		pOutputStream->WriteUChar(nSizeOfOffset); // offset size
		// Массив смещений начала каждого Top DICT + конец данных
		WriteOffset(1, nSizeOfOffset, pOutputStream);; // offset to first Top DICT
		WriteOffset(pTopDictSegment->Size() + 1, nSizeOfOffset, pOutputStream); // offset to end of Top DICT
		int nTopDictOffset = pOutputStream->Tell();
		pOutputStream->WriteStream(pTopDictSegment, 0, NULL);
		RELEASEOBJECT(pTopDictSegment);

		// Смещения Top DICT placeholders в pOutputStream
		nCharsetPlaceHolderPos     += nTopDictOffset;
		nEncodingPlaceHolderPos    += nTopDictOffset;
		nCharstringsPlaceHolderPos += nTopDictOffset;
		nPrivatePlaceHolderPos     += nTopDictOffset;
		nFDArrayPlaceHolderPos     += nTopDictOffset;
		nFDSelectPlaceHolderPos    += nTopDictOffset;

		// ------
		// Write String Index
		// ------

		if (sOptionalEmbeddedPostscript.empty()) // Копирование
		{
			pOutputStream->Write(pCFFDataStringIndex, nCFFDataStringIndex);
		}
		else
		{
			int nStringsCount1 = ushStringCount + 1;
			pOutputStream->WriteUChar((nStringsCount1 >> 8) & 0xff); // count (MSB)
			pOutputStream->WriteUChar(nStringsCount1 & 0xff); // count (LSB)

			unsigned int unTotalSize = 0;
			for (unsigned short i = 0; i < ushStringCount; ++i)
				unTotalSize += strlen((char *)pStrings[i]);
			unTotalSize += sOptionalEmbeddedPostscript.size();
			nSizeOfOffset = GetMostCompressedOffsetSize(unTotalSize + 1);
			pOutputStream->WriteUChar(nSizeOfOffset); // offset size

			unsigned int unCurOffset = 1;
			for (unsigned short i = 0; i < ushStringCount; ++i)
			{
				WriteOffset(unCurOffset, nSizeOfOffset, pOutputStream);; // offset
				unCurOffset += strlen((char *)pStrings[i]);
			}
			WriteOffset(unCurOffset, nSizeOfOffset, pOutputStream);; // offset
			unCurOffset += sOptionalEmbeddedPostscript.size();
			WriteOffset(unCurOffset, nSizeOfOffset, pOutputStream);; // offset

			for (unsigned short i = 0; i < ushStringCount; ++i)
				pOutputStream->Write(pStrings[i], strlen((char *)pStrings[i]));
			pOutputStream->WriteStr(sOptionalEmbeddedPostscript.c_str());
		}

		// ------
		// Skip Global Subrs
		// ------

		pOutputStream->WriteUChar(0); // count (MSB)
		pOutputStream->WriteUChar(0); // count (LSB)

		// ------
		// Write Encodings
		// ------

		if (!bIsCID)
		{
			// TODO
		}
		else
			nEncodingPlaceHolderPos = 0;

		// ------
		// Write Charsets
		// ------

		pOutputStream->WriteUChar(0);
		if (bIsCID && pUseGlyfs)
		{
			for (int i = 1; i < unCodesCount; ++i)
			{
				pOutputStream->WriteUChar((pCodeToGID[i] >> 8) & 0xff);
				pOutputStream->WriteUChar(pCodeToGID[i] & 0xff);
			}
		}
		else
		{
			// TODO SIDS
		}

		// ------
		// Read FDArray & FDSelect
		// ------

		for (int i = 0; i < ushFontsCount; ++i)
		{
			it = pTopDICT[i].mTopDict.find(scROS);
			if (it != pTopDICT[i].mTopDict.end())
			{
				// FDArray
				int nFDArray = 0;
				it = pTopDICT[i].mTopDict.find(scFDArray);
				if (it != pTopDICT[i].mTopDict.end())
					nFDArray = it->second.front().nIntegerValue;
				BYTE* pFDArray = pCFFDataBegin + nFDArray;

				unsigned short ushCount = 0;
				ReadIndexHeader(pFDArray, &pOffsets, ushCount);

				// FDSelect
			}
		}

		// ------
		// Write FDArray & FDSelect
		// ------



		// ------
		// Skip Char Strings
		// ------

		// ------
		// Read Private Dicts
		// ------

		RELEASEARRAYOBJECTS(pTopDICT);
		for (unsigned short i = 0; i < ushStringCount; ++i)
			RELEASEARRAYOBJECTS(pStrings[i]);
		RELEASEARRAYOBJECTS(pStrings);
	}
}
