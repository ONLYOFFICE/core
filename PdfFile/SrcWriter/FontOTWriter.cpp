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
	// Reader
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

		unsigned long mHeaderOffset;
		unsigned long mTableOffset;

		unsigned short mFaceIndex;

		HeadTable mHead;
		MaxpTable mMaxp;
		HHeaTable mHHea;
		HMtxTable mHMtx;

		CMemoryStream* mPrimitivesReader;
		EOpenTypeInputType mFontType;
		unsigned short mTablesCount;
		UIntToTableEntryMap mTables;

	public:
		COpenTypeReader();
		~COpenTypeReader();

		bool ReadOpenTypeFile(BYTE* pData, unsigned int nDataLength, unsigned short ushFaceIndex);
		bool ReadOpenTypeHeader();
		bool ReadOpenTypeSFNT();
		bool ReadOpenTypeSFNTFromDfont();
		unsigned long GetTag(const char* inTagName);
		bool ReadHead();
		bool ReadMaxP();
		bool ReadHHea();
		bool ReadHMtx();
	};
	COpenTypeReader::COpenTypeReader()
	{
		mFaceIndex    = 0;
		mHeaderOffset = 0;
		mTableOffset  = 0;

		mPrimitivesReader = NULL;
		mFontType = EOpenTypeInputType::EOpenTypeCFF;
	}
	COpenTypeReader::~COpenTypeReader()
	{
		RELEASEOBJECT(mPrimitivesReader);
		RELEASEARRAYOBJECTS(mHMtx);
	}
	bool COpenTypeReader::ReadOpenTypeFile(BYTE* pData, unsigned int nDataLength, unsigned short ushFaceIndex)
	{
		mFaceIndex = ushFaceIndex;

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

		// TODO

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
