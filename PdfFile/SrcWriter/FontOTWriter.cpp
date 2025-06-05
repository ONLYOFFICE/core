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
#define N_STD_STRINGS 391
	static const char* scStandardStrings[N_STD_STRINGS] = {
		".notdef","space","exclam","quotedbl","numbersign","dollar","percent","ampersand","quoteright","parenleft",
		"parenright","asterisk","plus","comma","hyphen","period","slash","zero","one","two",
		"three","four","five","six","seven","eight","nine","colon","semicolon","less",
		"equal","greater","question","at","A","B","C","D","E","F",
		"G","H","I","J","K","L","M","N","O","P",
		"Q","R","S","T","U","V","W","X","Y","Z",
		"bracketleft","backslash","bracketright","asciicircum","underscore","quoteleft","a","b","c","d",
		"e","f","g","h","i","j","k","l","m","n",
		"o","p","q","r","s","t","u","v","w","x",
		"y","z","braceleft","bar","braceright","asciitilde","exclamdown","cent","sterling","fraction",
		"yen","florin","section","currency","quotesingle","quotedblleft","guillemotleft","guilsinglleft","guilsinglright","fi",
		"fl","endash","dagger","daggerdbl","periodcentered","paragraph","bullet","quotesinglbase","quotedblbase","quotedblright",
		"guillemotright","ellipsis","perthousand","questiondown","grave","acute","circumflex","tilde","macron","breve",
		"dotaccent","dieresis","ring","cedilla","hungarumlaut","ogonek","caron","emdash","AE","ordfeminine",
		"Lslash","Oslash","OE","ordmasculine","ae","dotlessi","lslash","oslash","oe","germandbls",
		"onesuperior","logicalnot","mu","trademark","Eth","onehalf","plusminus","Thorn","onequarter","divide",
		"brokenbar","degree","thorn","threequarters","twosuperior","registered","minus","eth","multiply","threesuperior",
		"copyright","Aacute","Acircumflex","Adieresis","Agrave","Aring","Atilde","Ccedilla","Eacute","Ecircumflex",
		"Edieresis","Egrave","Iacute","Icircumflex","Idieresis","Igrave","Ntilde","Oacute","Ocircumflex","Odieresis",
		"Ograve","Otilde","Scaron","Uacute","Ucircumflex","Udieresis","Ugrave","Yacute","Ydieresis","Zcaron",
		"aacute","acircumflex","adieresis","agrave","aring","atilde","ccedilla","eacute","ecircumflex","edieresis",
		"egrave","iacute","icircumflex","idieresis","igrave","ntilde","oacute","ocircumflex","odieresis","ograve",
		"otilde","scaron","uacute","ucircumflex","udieresis","ugrave","yacute","ydieresis","zcaron","exclamsmall",
		"Hungarumlautsmall","dollaroldstyle","dollarsuperior","ampersandsmall","Acutesmall","parenleftsuperior","parenrightsuperior","twodotenleader","onedotenleader","zerooldstyle",
		"oneoldstyle","twooldstyle","threeoldstyle","fouroldstyle","fiveoldstyle","sixoldstyle","sevenoldstyle","eightoldstyle","nineoldstyle","commasuperior",
		"threequartersemdash","periodsuperior","questionsmall","asuperior","bsuperior","centsuperior","dsuperior","esuperior","isuperior","lsuperior",
		"msuperior","nsuperior","osuperior","rsuperior","ssuperior","tsuperior","ff","ffi","ffl","parenleftinferior",
		"parenrightinferior","Circumflexsmall","hyphensuperior","Gravesmall","Asmall","Bsmall","Csmall","Dsmall","Esmall","Fsmall",
		"Gsmall","Hsmall","Ismall","Jsmall","Ksmall","Lsmall","Msmall","Nsmall","Osmall","Psmall",
		"Qsmall","Rsmall","Ssmall","Tsmall","Usmall","Vsmall","Wsmall","Xsmall","Ysmall","Zsmall",
		"colonmonetary","onefitted","rupiah","Tildesmall","exclamdownsmall","centoldstyle","Lslashsmall","Scaronsmall","Zcaronsmall","Dieresissmall",
		"Brevesmall","Caronsmall","Dotaccentsmall","Macronsmall","figuredash","hypheninferior","Ogoneksmall","Ringsmall","Cedillasmall","questiondownsmall",
		"oneeighth","threeeighths","fiveeighths","seveneighths","onethird","twothirds","zerosuperior","foursuperior","fivesuperior","sixsuperior",
		"sevensuperior","eightsuperior","ninesuperior","zeroinferior","oneinferior","twoinferior","threeinferior","fourinferior","fiveinferior","sixinferior",
		"seveninferior","eightinferior","nineinferior","centinferior","dollarinferior","periodinferior","commainferior","Agravesmall","Aacutesmall","Acircumflexsmall",
		"Atildesmall","Adieresissmall","Aringsmall","AEsmall","Ccedillasmall","Egravesmall","Eacutesmall","Ecircumflexsmall","Edieresissmall","Igravesmall",
		"Iacutesmall","Icircumflexsmall","Idieresissmall","Ethsmall","Ntildesmall","Ogravesmall","Oacutesmall","Ocircumflexsmall","Otildesmall","Odieresissmall",
		"OEsmall","Oslashsmall","Ugravesmall","Uacutesmall","Ucircumflexsmall","Udieresissmall","Yacutesmall","Thornsmall","Ydieresissmall","001.000",
		"001.001","001.002","001.003","Black","Bold","Book","Light","Medium","Regular","Roman",
		"Semibold"
};
#define CHARSET_ISOADOBE_SIZE 228
	static const unsigned short scCharsetIsoadobeSids[CHARSET_ISOADOBE_SIZE] =
	{
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
		41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,
		78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,
		111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,
		138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,
		165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,
		192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,
		219,220,221,222,223,224,225,226,227,228
	};
#define CHARSET_EXPERT_SIZE 165
	static const unsigned short scCharsetExpertSids[CHARSET_EXPERT_SIZE] =
	{
		1,229,230,231,232,233,234,235,236,237,238,13,14,15,99,239,240,241,242,243,244,245,246,247,248,27,28,
		249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,109,110,267,268,269,270,271,272,
		273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297,298,
		299,300,301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,158,155,163,319,320,321,
		322,323,324,325,326,150,164,169,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,
		345,346,347,348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,
		370,371,372,373,374,375,376,377,378
	};
#define CHARSET_EXPERT_SUBSET_SIZE 86
	static const unsigned short scCharsetExpertSubsetSids[CHARSET_EXPERT_SUBSET_SIZE] =
	{
		1,231,232,235,236,237,238,13,14,15,99,239,240,241,242,243,244,245,246,247,248,27,28,
		249,250,251,253,254,255,256,257,258,259,260,261,262,263,264,265,266,109,110,267,268,
		269,270,272,300,301,302,305,314,315,158,155,163,320,321,322,323,324,325,326,150,164,
		169,327,328,329,330,331,332,333,334,335,336,337,338,339,340,341,342,343,344,345,346
	};
	static const unsigned short* scDefaultCharsets[3] =
	{
		scCharsetIsoadobeSids,
		scCharsetExpertSids,
		scCharsetExpertSubsetSids
	};
	static const unsigned short scDefaultCharsetsSizes[3] =
	{
		CHARSET_ISOADOBE_SIZE,
		CHARSET_EXPERT_SIZE,
		CHARSET_EXPERT_SUBSET_SIZE
	};

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
		typedef std::vector<CharSetInfo*> CharSetInfoVector;
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
		typedef std::vector<EncodingsInfo*> EncodingsInfoVector;
		struct StringLess
		{
			bool operator() (const char* left, const char* right) const
			{
				return strcmp(left ,right) < 0;
			}
		};
		typedef std::map<const char*, unsigned short, StringLess> CharPToUShortMap;
		typedef std::map<long long, CharStrings*> LongFilePositionTypeToCharStringsMap;
		typedef std::map<long long, CharSetInfo*> LongFilePositionTypeToCharSetInfoMap;
		typedef std::map<long long, EncodingsInfo*> LongFilePositionTypeToEncodingsInfoMap;

		long long mCFFOffset;

		CFFHeader mHeader;
		unsigned short mFontsCount;
		std::list<std::string> mName;
		TopDictInfo* mTopDictIndex; // count is same as fonts count
		char** mStrings;
		unsigned short mStringsCount;
		long long mStringIndexPosition;
		long long mGlobalSubrsPosition;
		PrivateDictInfo* mPrivateDicts;

		CharStrings mGlobalSubrs;
		CharStrings* mCharStrings; // count is same as fonts count
		LongFilePositionTypeToCharStringsMap mLocalSubrs; // count is NOT the same as fonts count [some may be shared, plus there might be more because of CID usage]
		CharSetInfoVector mCharSets;// count is NOT the same as fonts count [some charsets may be shared]. consult the top dict charset pointer for the right charset
		EncodingsInfoVector mEncodings; // count is NOT the same as fonts count [some encodinds may be shared].

		CMemoryStream* mPrimitivesReader; // внешний, освобождать не надо
		StringToUShort mNameToIndex;
		long long mNameIndexPosition;
		long long mTopDictIndexPosition;
		CharPToUShortMap mStringToSID;
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
		bool ReadStringIndex();
		bool ReadGlobalSubrs();
		bool ReadSubrsFromIndex(unsigned short& outSubrsCount, CharStringsIndex* outSubrsIndex);
		bool ReadCharStrings();
		static const unsigned short scCharStringsKey = 17;
		long long GetCharStringsPosition(unsigned short inFontIndex);
		long GetSingleIntegerValue(unsigned short inFontIndex, unsigned short inKey, long inDefault);
		long GetSingleIntegerValueFromDict(const UShortToDictOperandListMap& inDict, unsigned short inKey, long inDefault);
		static const unsigned short scCharstringType = 0x0C06;
		long GetCharStringType(unsigned short inFontIndex);
		bool ReadPrivateDicts();
		static const unsigned short scPrivate = 18;
		bool ReadPrivateDict(const UShortToDictOperandListMap& inReferencingDict, PrivateDictInfo* outPrivateDict);
		bool ReadLocalSubrs();
		static const unsigned short scSubrs = 19;
		bool ReadLocalSubrsForPrivateDict(PrivateDictInfo* inPrivateDict, BYTE inCharStringType);
		static const unsigned short scROS = 0xC1E;
		bool ReadCharsets();
		bool ReadEncodings();
		void ReadEncoding(EncodingsInfo* inEncoding, long long inEncodingPosition);
		void SetupSIDToGlyphMapWithStandard(const unsigned short* inStandardCharSet, unsigned short inStandardCharSetLength, UShortToCharStringMap& ioCharMap, const CharStrings& inCharStrings);
		bool ReadFormat0Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		bool ReadFormat1Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		bool ReadFormat2Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		static const unsigned short scCharset = 15;
		long long GetCharsetPosition(unsigned short inFontIndex);
		static const unsigned short scEncoding = 16;
		long long GetEncodingPosition(unsigned short inFontIndex);
		bool ReadCIDInformation();
		bool ReadFDArray(unsigned short inFontIndex);
		static const unsigned short scFDArray = 0xC24;
		long long GetFDArrayPosition(unsigned short inFontIndex);
		bool ReadFDSelect(unsigned short inFontIndex);
		static const unsigned short scFDSelect = 0xC25;
		long long GetFDSelectPosition(unsigned short inFontIndex);
	};
	CCFFReader::CCFFReader()
	{
		mTopDictIndex = NULL;
		mStrings = NULL;
		mGlobalSubrs.mCharStringsIndex = NULL;
		mCharStrings = NULL;
		mPrivateDicts = NULL;
	}
	CCFFReader::~CCFFReader()
	{
		FreeData();
	}
	void CCFFReader::FreeData()
	{
		mName.clear();
		mNameToIndex.clear();
		if (mTopDictIndex != NULL)
		{
			for (unsigned short i = 0; i < mFontsCount; ++i)
			{
				delete[] mTopDictIndex[i].mFDArray;
				delete[] mTopDictIndex[i].mFDSelect;
			}
			RELEASEARRAYOBJECTS(mTopDictIndex);
		}
		if (mStrings != NULL)
		{
			for (unsigned short i = 0; i < mStringsCount; ++i)
				delete[] mStrings[i];
			RELEASEARRAYOBJECTS(mStrings);
		}
		mStringToSID.clear();
		RELEASEARRAYOBJECTS(mGlobalSubrs.mCharStringsIndex);
		if (mCharStrings != NULL)
		{
			for (unsigned short i = 0; i < mFontsCount; ++i)
				delete[] mCharStrings[i].mCharStringsIndex;
			RELEASEARRAYOBJECTS(mCharStrings);
		}
		RELEASEARRAYOBJECTS(mPrivateDicts);

		LongFilePositionTypeToCharStringsMap::iterator itLocalSubrs = mLocalSubrs.begin();
		for(; itLocalSubrs != mLocalSubrs.end(); ++itLocalSubrs)
		{
			delete[] itLocalSubrs->second->mCharStringsIndex;
			delete itLocalSubrs->second;
		}

		CharSetInfoVector::iterator itCharSets = mCharSets.begin();
		for(; itCharSets != mCharSets.end(); ++itCharSets)
		{
			delete[] (*itCharSets)->mSIDs;
			(*itCharSets)->mSIDToGlyphMap.clear();
			delete (*itCharSets);
		}
		mCharSets.clear();

		EncodingsInfoVector::iterator itEncodings = mEncodings.begin();
		for(; itEncodings != mEncodings.end(); ++itEncodings)
		{
			delete[] (*itEncodings)->mEncoding;
			delete (*itEncodings);
		}
		mEncodings.clear();

		RELEASEOBJECT(mPrimitivesReader);
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
			outValue = mPrimitivesReader->ReadUShort();
			if (mPrimitivesReader->IsEof())
				return false;
		}
		else if (29 == byte0)
		{
			outValue = mPrimitivesReader->ReadUInt();
			if (mPrimitivesReader->IsEof())
				return false;
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

		mCFFOffset = inCFFFile->Tell();
		mPrimitivesReader = new CMemoryStream(inCFFFile->Size() - inCFFFile->Tell());
		mPrimitivesReader->Write(inCFFFile->GetCurBuffer(), inCFFFile->Size() - inCFFFile->Tell());
		mPrimitivesReader->Seek(0, SeekSet);

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

		status = ReadStringIndex();
		if (!status)
			return false;

		status = ReadGlobalSubrs();
		if (!status)
			return false;

		status = ReadCharStrings();
		if (!status)
			return false;

		status = ReadPrivateDicts();
		if (!status)
			return false;

		status = ReadLocalSubrs();
		if (!status)
			return false;

		status = ReadCharsets();
		if (!status)
			return false;

		status = ReadEncodings();
		if (!status)
			return false;

		status = ReadCIDInformation();
		if (!status)
			return false;

		return true;
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
		if (mPrimitivesReader->IsEof())
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
			if (mPrimitivesReader->IsEof())
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
	bool CCFFReader::ReadStringIndex()
	{
		mStringIndexPosition = mPrimitivesReader->Tell();

		unsigned long* offsets;
		bool status = ReadIndexHeader(&offsets,mStringsCount);
		if (!status)
			return false;

		if (0 == mStringsCount)
		{
			mStrings = NULL;
			return true;
		}

		if (offsets[0] != 1)
			mPrimitivesReader->Seek(offsets[0] - 1, SeekCur);

		mStrings = new char*[mStringsCount];

		unsigned long i;
		for (i = 0; i < mStringsCount && status; ++i)
		{
			unsigned int nLength = offsets[i + 1] - offsets[i];
			mStrings[i] = new char[nLength + 1];
			mPrimitivesReader->Read((BYTE*)mStrings[i], &nLength);
			if (mPrimitivesReader->IsEof())
			{
				status = false;
				break;
			}
			mStrings[i][nLength] = 0;
		}

		// failure case, null all the rest of the strings for later delete to not perofrm errors
		if (!status)
		{
			for(; i < mStringsCount; ++i)
				mStrings[i] = NULL;
		}

		// now create the string to SID map
		for ( i = 0; i < N_STD_STRINGS; ++i)
			mStringToSID.insert(CharPToUShortMap::value_type(scStandardStrings[i], i));
		for (; i < N_STD_STRINGS + mStringsCount; ++i)
			mStringToSID.insert(CharPToUShortMap::value_type(mStrings[i - N_STD_STRINGS], i));

		delete[] offsets;
		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadGlobalSubrs()
	{
		mGlobalSubrsPosition = mPrimitivesReader->Tell();
		mGlobalSubrs.mCharStringsType = 2; // always 2
		return ReadSubrsFromIndex(mGlobalSubrs.mCharStringsCount, &(mGlobalSubrs.mCharStringsIndex));
	}
	bool CCFFReader::ReadSubrsFromIndex(unsigned short& outSubrsCount, CharStringsIndex* outSubrsIndex)
	{
		unsigned long* offsets;
		bool status = ReadIndexHeader(&offsets, outSubrsCount);
		if (!status)
			return false;

		if (0 == outSubrsCount)
		{
			*outSubrsIndex = NULL;
			return true;
		}

		// just taking a snapshot of positions here
		*outSubrsIndex = new CharString[outSubrsCount];

		long long dataStartPosition = mPrimitivesReader->Tell();

		for (unsigned short i = 0; i < outSubrsCount; ++i)
		{
			(*outSubrsIndex)[i].mStartPosition = dataStartPosition + offsets[i] - 1;
			(*outSubrsIndex)[i].mEndPosition = dataStartPosition + offsets[i+1] - 1;
			(*outSubrsIndex)[i].mIndex = i;
		}

		// for good faith put the pointer at the end now (if someone wants to take a snapshot)
		mPrimitivesReader->Seek(offsets[outSubrsCount] - 1, SeekCur);

		delete[] offsets;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadCharStrings()
	{
		// scan all charstrings of all included fonts
		mCharStrings = new CharStrings[mFontsCount];
		bool status = true;

		for (unsigned short i = 0; i < mFontsCount && status; ++i)
		{
			long long  charStringsPosition = GetCharStringsPosition(i);
			mCharStrings[i].mCharStringsType = (BYTE)GetCharStringType(i);
			if (0 == charStringsPosition)
			{
				mCharStrings[i].mCharStringsCount = 0;
				mCharStrings[i].mCharStringsIndex = NULL;
			}
			else
			{
				mPrimitivesReader->Seek(charStringsPosition, SeekSet);
				status = ReadSubrsFromIndex(mCharStrings[i].mCharStringsCount, &(mCharStrings[i].mCharStringsIndex));
			}
		}

		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	long long CCFFReader::GetCharStringsPosition(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scCharStringsKey, 0);
	}
	long CCFFReader::GetSingleIntegerValue(unsigned short inFontIndex, unsigned short inKey, long inDefault)
	{
		return GetSingleIntegerValueFromDict(mTopDictIndex[inFontIndex].mTopDict, inKey, inDefault);
	}
	long CCFFReader::GetSingleIntegerValueFromDict(const UShortToDictOperandListMap& inDict, unsigned short inKey, long inDefault)
	{
		UShortToDictOperandListMap::const_iterator it = inDict.find(inKey);
		if (it != inDict.end())
			return it->second.front().IntegerValue;
		return inDefault;
	}
	long CCFFReader::GetCharStringType(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scCharstringType, 2);
	}
	bool CCFFReader::ReadPrivateDicts()
	{
		mPrivateDicts = new PrivateDictInfo[mFontsCount];
		bool status = true;

		for (unsigned short i = 0; i < mFontsCount && status; ++i)
			status = ReadPrivateDict(mTopDictIndex[i].mTopDict, mPrivateDicts + i);

		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadPrivateDict(const UShortToDictOperandListMap& inReferencingDict, PrivateDictInfo* outPrivateDict)
	{
		bool status = true;
		UShortToDictOperandListMap::const_iterator it = inReferencingDict.find(scPrivate);

		outPrivateDict->mLocalSubrs = NULL;
		if (it == inReferencingDict.end())
		{
			outPrivateDict->mPrivateDictStart = 0;
			outPrivateDict->mPrivateDictEnd   = 0;
		}
		else
		{
			outPrivateDict->mPrivateDictStart = (long long)it->second.back().IntegerValue;
			outPrivateDict->mPrivateDictEnd = (long long)(it->second.back().IntegerValue + it->second.front().IntegerValue);

			mPrimitivesReader->Seek(it->second.back().IntegerValue, SeekSet);
			status = ReadDict(it->second.front().IntegerValue, outPrivateDict->mPrivateDict);
		}
		return status;
	}
	bool CCFFReader::ReadLocalSubrs()
	{
		// scan all subrs of all included fonts
		bool status = true;

		for (unsigned short i=0; i < mFontsCount && status; ++i)
			status = ReadLocalSubrsForPrivateDict(mPrivateDicts + i, (BYTE)GetCharStringType(i));

		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadLocalSubrsForPrivateDict(PrivateDictInfo* inPrivateDict, BYTE inCharStringType)
	{
		bool status = true;
		long long subrsPosition = GetSingleIntegerValueFromDict(inPrivateDict->mPrivateDict, scSubrs, 0);

		if (0 == subrsPosition)
		{
			inPrivateDict->mLocalSubrs = NULL;
		}
		else
		{
			LongFilePositionTypeToCharStringsMap::iterator	it = mLocalSubrs.find(inPrivateDict->mPrivateDictStart + subrsPosition);
			if (it == mLocalSubrs.end())
			{
				CharStrings* charStrings = new CharStrings();
				charStrings->mCharStringsType = inCharStringType;
				mPrimitivesReader->Seek(inPrivateDict->mPrivateDictStart + subrsPosition, SeekSet);
				status = ReadSubrsFromIndex(charStrings->mCharStringsCount, &(charStrings->mCharStringsIndex));
				if (status)
					it = mLocalSubrs.insert(LongFilePositionTypeToCharStringsMap::value_type(inPrivateDict->mPrivateDictStart + subrsPosition,charStrings)).first;
			}
			inPrivateDict->mLocalSubrs = it->second;
		}
		return status;
	}
	bool CCFFReader::ReadCharsets()
	{
		// read all charsets
		bool status = true;
		LongFilePositionTypeToCharSetInfoMap offsetToIndex;
		LongFilePositionTypeToCharSetInfoMap::iterator it;

		for (unsigned short i = 0; i < mFontsCount && status; ++i)
		{
			long long charsetPosition = GetCharsetPosition(i);
			it = offsetToIndex.find(charsetPosition);
			if (it == offsetToIndex.end())
			{
				CharSetInfo* charSet = new CharSetInfo();
				bool isCID = mTopDictIndex[i].mTopDict.find(scROS) != mTopDictIndex[i].mTopDict.end();
				if (charsetPosition <= 2)
				{
					charSet->mType = (ECharSetType)charsetPosition;
					charSet->mSIDs = NULL;
					if (!isCID) // collect SID->Glyph map only if not CID, in which case SIDs are CIDs...and what i'm using the map for is irrelevant
						SetupSIDToGlyphMapWithStandard(scDefaultCharsets[charsetPosition], scDefaultCharsetsSizes[charsetPosition], charSet->mSIDToGlyphMap, mCharStrings[i]);
				}
				else
				{
					BYTE charsetFormat;
					charSet->mType = eCharSetCustom;
					mPrimitivesReader->Seek(charsetPosition, SeekSet);
					charsetFormat = mPrimitivesReader->ReadUChar();

					if (0 == charsetFormat)
						status = ReadFormat0Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
					else if (1 == charsetFormat)
						status = ReadFormat1Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
					else // 2 charset format
						status = ReadFormat2Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
				}
				mCharSets.push_back(charSet);
				it = offsetToIndex.insert(LongFilePositionTypeToCharSetInfoMap::value_type(charsetPosition,charSet)).first;
			}
			mTopDictIndex[i].mCharSet = it->second;
		}

		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadEncodings()
	{
		// read all encodings positions
		bool status = true;
		LongFilePositionTypeToEncodingsInfoMap offsetToEncoding;
		LongFilePositionTypeToEncodingsInfoMap::iterator it;

		for (unsigned short i = 0; i < mFontsCount && status; ++i)
		{
			long long encodingPosition = GetEncodingPosition(i);
			it = offsetToEncoding.find(encodingPosition);
			if (it == offsetToEncoding.end())
			{
				EncodingsInfo* encoding = new EncodingsInfo();
				ReadEncoding(encoding,encodingPosition);
				mEncodings.push_back(encoding);
				it = offsetToEncoding.insert(LongFilePositionTypeToEncodingsInfoMap::value_type(encodingPosition,encoding)).first;
			}
			mTopDictIndex[i].mEncoding = it->second;
		}

		if (!status)
			return status;
		else
			return !mPrimitivesReader->IsEof();
	}
	void CCFFReader::ReadEncoding(EncodingsInfo* inEncoding, long long inEncodingPosition)
	{
		if (inEncodingPosition <= 1)
		{
			inEncoding->mEncodingStart = inEncoding->mEncodingEnd = inEncodingPosition;
			inEncoding->mType = (EEncodingType)inEncodingPosition;
			return;
		}
		inEncoding->mType = eEncodingCustom;
		inEncoding->mEncodingStart = inEncodingPosition;
		mPrimitivesReader->Seek(inEncodingPosition, SeekSet);
		BYTE encodingFormat = mPrimitivesReader->ReadUChar();

		if (0 == (encodingFormat & 0x1))
		{
			inEncoding->mEncodingsCount = mPrimitivesReader->ReadUChar();
			if (inEncoding->mEncodingsCount > 0)
			{
				inEncoding->mEncoding = new BYTE[inEncoding->mEncodingsCount];
				for (BYTE i = 0; i < inEncoding->mEncodingsCount; ++i)
					inEncoding->mEncoding[i] = mPrimitivesReader->ReadUChar();
			}
		}
		else // format = 1
		{
			BYTE rangesCount = mPrimitivesReader->ReadUChar();
			if (rangesCount > 0)
			{
				BYTE firstCode;
				BYTE left;

				inEncoding->mEncodingsCount = 0;
				// get the encoding count (yap, reading twice here)
				for (BYTE i = 0; i < rangesCount; ++i)
				{
					firstCode = mPrimitivesReader->ReadUChar();
					left = mPrimitivesReader->ReadUChar();
					inEncoding->mEncodingsCount += left;
				}
				inEncoding->mEncoding = new BYTE[inEncoding->mEncodingsCount];
				mPrimitivesReader->Seek(inEncodingPosition + 2, SeekSet); // reset encoding to beginning of range reading

				// now read the encoding array
				BYTE encodingIndex = 0;
				for (BYTE i = 0; i < rangesCount; ++i)
				{
					firstCode = mPrimitivesReader->ReadUChar();
					left = mPrimitivesReader->ReadUChar();
					for (BYTE j = 0;j < left; ++j)
						inEncoding->mEncoding[encodingIndex + j] = firstCode + j;
					encodingIndex += left;
				}
			}
		}
		if ((encodingFormat & 0x80) !=  0) // supplaments exist, need to add to encoding end
		{
			mPrimitivesReader->Seek(inEncoding->mEncodingEnd, SeekSet); // set position to end of encoding, and start of supplamental, so that can read their count
			BYTE supplamentalsCount = mPrimitivesReader->ReadUChar();
			if (supplamentalsCount > 0)
			{
				BYTE encoding;
				unsigned short SID;
				for (BYTE i = 0; i < supplamentalsCount; ++i)
				{
					encoding = mPrimitivesReader->ReadUChar();
					SID = mPrimitivesReader->ReadUShort();

					UShortToByteList::iterator it = inEncoding->mSupplements.find(SID);
					if (it == inEncoding->mSupplements.end())
						it = inEncoding->mSupplements.insert(UShortToByteList::value_type(SID, ByteList())).first;
					it->second.push_back(encoding);
				}
			}
		}
		inEncoding->mEncodingEnd = mPrimitivesReader->Tell();
	}
	void CCFFReader::SetupSIDToGlyphMapWithStandard(const unsigned short* inStandardCharSet, unsigned short inStandardCharSetLength, UShortToCharStringMap& ioCharMap, const CharStrings& inCharStrings)
	{
		ioCharMap.insert(UShortToCharStringMap::value_type(0, inCharStrings.mCharStringsIndex));
		for (unsigned short i = 1; i < inCharStrings.mCharStringsCount && i < inStandardCharSetLength;++i)
			ioCharMap.insert(UShortToCharStringMap::value_type(inStandardCharSet[i], inCharStrings.mCharStringsIndex + i));
	}
	bool CCFFReader::ReadFormat0Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings)
	{
		// for CIDs don't bother filling up the SID->glyph map. it ain't SIDs
		if (!inIsCID)
			ioGlyphMap.insert(UShortToCharStringMap::value_type(0, inCharStrings.mCharStringsIndex));
		*inSIDArray = new unsigned short[inCharStrings.mCharStringsCount];
		(*inSIDArray)[0] = 0;

		if (inIsCID)
		{
			for (unsigned short i = 1; i < inCharStrings.mCharStringsCount; ++i)
				(*inSIDArray)[i] = mPrimitivesReader->ReadUShort();
		}
		else
		{
			for (unsigned short i = 1; i < inCharStrings.mCharStringsCount; ++i)
			{
				unsigned short sid = mPrimitivesReader->ReadUShort();
				(*inSIDArray)[i] = sid;

				ioGlyphMap.insert(UShortToCharStringMap::value_type(sid, inCharStrings.mCharStringsIndex + i));
			}
		}
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadFormat1Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings)
	{
		if(!inIsCID)
			ioGlyphMap.insert(UShortToCharStringMap::value_type(0, inCharStrings.mCharStringsIndex));
		*inSIDArray = new unsigned short[inCharStrings.mCharStringsCount];
		(*inSIDArray)[0] = 0;
		unsigned long glyphIndex = 1;
		unsigned short sid;
		BYTE left;

		if (inIsCID)
		{
			while (glyphIndex < inCharStrings.mCharStringsCount)
			{
				sid = mPrimitivesReader->ReadUShort();
				left = mPrimitivesReader->ReadUChar();
				for (BYTE i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i, ++glyphIndex)
					(*inSIDArray)[glyphIndex] = sid + i;
			}
		}
		else
		{
			while (glyphIndex < inCharStrings.mCharStringsCount)
			{
				sid = mPrimitivesReader->ReadUShort();
				left = mPrimitivesReader->ReadUChar();
				for (BYTE i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i, ++glyphIndex)
				{
					ioGlyphMap.insert(UShortToCharStringMap::value_type(sid + i, inCharStrings.mCharStringsIndex + glyphIndex));
					(*inSIDArray)[glyphIndex] = sid + i;
				}
			}
		}
		return !mPrimitivesReader->IsEof();
	}
	bool CCFFReader::ReadFormat2Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings)
	{
		if(!inIsCID)
			ioGlyphMap.insert(UShortToCharStringMap::value_type(0, inCharStrings.mCharStringsIndex));
		*inSIDArray = new unsigned short[inCharStrings.mCharStringsCount];
		(*inSIDArray)[0] = 0;
		unsigned short glyphIndex = 1;
		unsigned short sid;
		unsigned short left;

		if (inIsCID)
		{
			while (glyphIndex < inCharStrings.mCharStringsCount)
			{
				sid = mPrimitivesReader->ReadUShort();
				left = mPrimitivesReader->ReadUShort();
				for (unsigned short i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i, ++glyphIndex)
					(*inSIDArray)[glyphIndex] = sid + i;
			}
		}
		else
		{
			while (glyphIndex < inCharStrings.mCharStringsCount)
			{
				sid = mPrimitivesReader->ReadUShort();
				left = mPrimitivesReader->ReadUShort();
				for (unsigned short i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i ,++glyphIndex)
				{
					ioGlyphMap.insert(UShortToCharStringMap::value_type(sid + i, inCharStrings.mCharStringsIndex + glyphIndex));
					(*inSIDArray)[glyphIndex] = sid + i;
				}
			}
		}
		return !mPrimitivesReader->IsEof();
	}
	long long CCFFReader::GetCharsetPosition(unsigned short inFontIndex)
	{
		return (long long)GetSingleIntegerValue(inFontIndex, scCharset, 0);
	}
	long long CCFFReader::GetEncodingPosition(unsigned short inFontIndex)
	{
		return (long long)GetSingleIntegerValue(inFontIndex, scEncoding, 0);
	}
	bool CCFFReader::ReadCIDInformation()
	{
		bool status = true;

		for (unsigned short i = 0; i < mFontsCount && status; ++i)
		{
			// CID font will be identified by the existance of the ROS entry
			if (mTopDictIndex[i].mTopDict.find(scROS) != mTopDictIndex[i].mTopDict.end())
			{
				status = ReadFDArray(i);
				if (!status)
					break;

				status = ReadFDSelect(i);
				if (!status)
					break;
			}
		}

		return status;
	}
	bool CCFFReader::ReadFDArray(unsigned short inFontIndex)
	{
		long long fdArrayLocation = GetFDArrayPosition(inFontIndex);

		// supposed to get here only for CIDs. and they must have an FDArray...so if it doesn't - fail
		if (0 == fdArrayLocation)
			return false;

		mPrimitivesReader->Seek(fdArrayLocation, SeekSet);

		unsigned long* offsets;
		unsigned short dictionariesCount;
		unsigned short i;
		bool status = ReadIndexHeader(&offsets, dictionariesCount);
		if (!status)
			return false;

		if (offsets[0] != 1)
			mPrimitivesReader->Seek(offsets[0] - 1, SeekCur);

		mTopDictIndex[inFontIndex].mFDArray = new FontDictInfo[dictionariesCount];

		for (i = 0; i < dictionariesCount && status; ++i)
		{
			mTopDictIndex[inFontIndex].mFDArray[i].mFontDictStart = mPrimitivesReader->Tell();
			status = ReadDict(offsets[i + 1] - offsets[i], mTopDictIndex[inFontIndex].mFDArray[i].mFontDict);
			if (!status)
				break;
			mTopDictIndex[inFontIndex].mFDArray[i].mFontDictEnd = mPrimitivesReader->Tell();
		}

		// another loop for reading the privates [should be one per font dict]. make sure to get their font subrs reference right
		for (i = 0; i < dictionariesCount && status; ++i)
		{
			status = ReadPrivateDict(mTopDictIndex[inFontIndex].mFDArray[i].mFontDict, &(mTopDictIndex[inFontIndex].mFDArray[i].mPrivateDict));
			if (status)
				status = ReadLocalSubrsForPrivateDict(&(mTopDictIndex[inFontIndex].mFDArray[i].mPrivateDict), (BYTE)GetCharStringType(inFontIndex));
		}

		delete[] offsets;
		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	long long CCFFReader::GetFDArrayPosition(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scFDArray, 0);
	}
	bool CCFFReader::ReadFDSelect(unsigned short inFontIndex)
	{
		long long fdSelectLocation = GetFDSelectPosition(inFontIndex);
		unsigned short glyphCount = mCharStrings[inFontIndex].mCharStringsCount;
		bool status = true;

		// supposed to get here only for CIDs. and they must have an FDSelect...so if it doesn't - fail
		if (0 == fdSelectLocation)
			return false;

		mTopDictIndex[inFontIndex].mFDSelect = new FontDictInfo*[glyphCount];
		mPrimitivesReader->Seek(fdSelectLocation, SeekSet);

		BYTE format = mPrimitivesReader->ReadUChar();
		if (0 == format)
		{
			BYTE fdIndex;

			for (unsigned short i = 0; i < glyphCount && !mPrimitivesReader->IsEof(); ++i)
			{
				fdIndex = mPrimitivesReader->ReadUChar();
				if (!mPrimitivesReader->IsEof())
					mTopDictIndex[inFontIndex].mFDSelect[i] = mTopDictIndex[inFontIndex].mFDArray + fdIndex;
			}
		}
		else // format 3
		{
			unsigned short rangesCount;
			unsigned short firstGlyphIndex;
			unsigned short nextRangeGlyphIndex;
			BYTE fdIndex;

			rangesCount = mPrimitivesReader->ReadUShort();
			if (!mPrimitivesReader->IsEof())
			{
				firstGlyphIndex = mPrimitivesReader->ReadUShort();
				for (unsigned short i = 0; i < rangesCount && !mPrimitivesReader->IsEof(); ++i)
				{
					fdIndex = mPrimitivesReader->ReadUChar();
					nextRangeGlyphIndex = mPrimitivesReader->ReadUShort();
					if (!mPrimitivesReader->IsEof())
						for (unsigned short j = firstGlyphIndex; j < nextRangeGlyphIndex; ++j)
							mTopDictIndex[inFontIndex].mFDSelect[j] = mTopDictIndex[inFontIndex].mFDArray + fdIndex;
					firstGlyphIndex = nextRangeGlyphIndex;
				}
			}
		}

		if (!status)
			return status;
		return !mPrimitivesReader->IsEof();
	}
	long long CCFFReader::GetFDSelectPosition(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scFDSelect, 0);
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
