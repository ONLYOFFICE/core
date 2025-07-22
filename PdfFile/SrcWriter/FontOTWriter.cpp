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
#include <set>

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

	static const unsigned short scCharset = 15;
	static const unsigned short scEncoding = 16;
	static const unsigned short scCharStrings = 17;
	static const unsigned short scPrivate = 18;
	static const unsigned short scSubrs = 19;
	static const unsigned short scFDArray = 0xC24;
	static const unsigned short scFDSelect = 0xC25;
	static const unsigned short scROS = 0xC1E;

	struct DictOperand
	{
		bool IsInteger;
		long IntegerValue;
		double RealValue;
		long RealValueFractalEnd;
	};
	typedef std::list<DictOperand> DictOperandList;
	typedef std::map<unsigned short, DictOperandList> UShortToDictOperandListMap;
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
		EncodingsInfo() { mEncoding = NULL; }

		long long mEncodingStart;
		long long mEncodingEnd;

		EEncodingType mType;
		BYTE mEncodingsCount;
		BYTE* mEncoding;
		UShortToByteList mSupplements;
	};
	struct IndexElement
	{
		IndexElement() { mStartPosition = 0; mEndPosition = 0; mIndex = 0; }

		long long mStartPosition;
		long long mEndPosition;
		unsigned short mIndex;
	};
	typedef IndexElement CharString;
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
	typedef std::set<unsigned short> UShortSet;
	struct CharString2Dependencies
	{
		UShortSet mCharCodes; // from seac-like endchar operator
		UShortSet mGlobalSubrs; // from callgsubr
		UShortSet mLocalSubrs; // from callsubr
	};
	struct CharStringOperand
	{
		bool IsInteger;
		long IntegerValue;
		double RealValue;
	};
	typedef std::list<CharStringOperand> CharStringOperandList;
	typedef std::vector<CharStringOperand> CharStringOperandVector;

	namespace FSType
	{
	bool CanEmbed(unsigned short mFSTypeValue)
	{
		return (mFSTypeValue != 0x2) && (mFSTypeValue != 0x0200) && (mFSTypeValue != 0x0202);
	}
	}
	//----------------------------------------------------------------------------------------
	// CPrimitiveWriter
	//----------------------------------------------------------------------------------------
	struct CPrimitiveWriter
	{
		BYTE mCurrentOffsize;
		CStream* mStream;
	public:
		CPrimitiveWriter(CStream* pStream);
		~CPrimitiveWriter();

		void SetOffSize(BYTE inOffSize);
		bool WriteByte(BYTE inValue);
		bool WriteCard8(BYTE inValue);
		bool WriteCard16(unsigned short inValue);
		bool WriteOffSize(BYTE inValue);
		bool WriteOffset(unsigned long inValue);
		bool Write3ByteUnsigned(unsigned long inValue);
		bool Write4ByteUnsigned(unsigned long inValue);
		bool Write(const BYTE* inBuffer, unsigned long inBufferSize);
		bool WriteDictOperand(const DictOperand& inOperand);
		bool WriteDictItems(unsigned short inOperator, const DictOperandList& inOperands);
		bool WriteIntegerOperand(long inValue);
		bool WriteRealOperand(double inValue, long inFractalLength);
		bool Write5ByteDictInteger(long inValue);
		bool WriteIntegerOfReal(double inIntegerValue, BYTE& ioBuffer, bool& ioUsedFirst);
		bool SetOrWriteNibble(BYTE inValue, BYTE& ioBuffer, bool& ioUsedFirst);
		bool WriteDictOperator(unsigned short inOperator);
		bool Pad5Bytes();
		bool WriteSID(unsigned short inValue);
	};
	CPrimitiveWriter::CPrimitiveWriter(CStream* pStream)
	{
		mCurrentOffsize = 1;
		mStream = pStream;
	}
	CPrimitiveWriter::~CPrimitiveWriter()
	{
		mCurrentOffsize = 1;
		mStream = NULL;
	}
	void CPrimitiveWriter::SetOffSize(BYTE inOffSize)
	{
		mCurrentOffsize = inOffSize;
	}
	bool CPrimitiveWriter::WriteByte(BYTE inValue)
	{
		mStream->WriteUChar(inValue);
		return true;
	}
	bool CPrimitiveWriter::WriteCard8(BYTE inValue)
	{
		return WriteByte(inValue);
	}
	bool CPrimitiveWriter::WriteCard16(unsigned short inValue)
	{
		mStream->WriteUChar((inValue >> 8) & 0xff);
		mStream->WriteUChar(inValue & 0xff);
		return true;
	}
	bool CPrimitiveWriter::WriteOffSize(BYTE inValue)
	{
		return WriteCard8(inValue);
	}
	bool CPrimitiveWriter::WriteOffset(unsigned long inValue)
	{
		switch (mCurrentOffsize)
		{
		case 1:
			WriteCard8((BYTE)inValue);
			break;
		case 2:
			WriteCard16((unsigned short)inValue);
			break;
		case 3:
			Write3ByteUnsigned(inValue);
			break;
		case 4:
			Write4ByteUnsigned(inValue);
			break;
		}
		return true;
	}
	bool CPrimitiveWriter::Write3ByteUnsigned(unsigned long inValue)
	{
		mStream->WriteUChar((inValue >> 16) & 0xff);
		mStream->WriteUChar((inValue >> 8) & 0xff);
		mStream->WriteUChar(inValue & 0xff);
		return true;
	}
	bool CPrimitiveWriter::Write4ByteUnsigned(unsigned long inValue)
	{
		mStream->WriteUChar((inValue >> 24) & 0xff);
		mStream->WriteUChar((inValue >> 16) & 0xff);
		mStream->WriteUChar((inValue >> 8) & 0xff);
		mStream->WriteUChar(inValue & 0xff);
		return true;
	}
	bool CPrimitiveWriter::Write(const BYTE* inBuffer, unsigned long inBufferSize)
	{
		mStream->Write(inBuffer, inBufferSize);
		return true;
	}
	bool CPrimitiveWriter::WriteDictOperand(const DictOperand& inOperand)
	{
		if (inOperand.IsInteger)
			return WriteIntegerOperand(inOperand.IntegerValue);
		else
			return WriteRealOperand(inOperand.RealValue, inOperand.RealValueFractalEnd);
	}
	bool CPrimitiveWriter::WriteDictItems(unsigned short inOperator, const DictOperandList& inOperands)
	{
		bool status = true;
		DictOperandList::const_iterator it = inOperands.begin();

		for (; it != inOperands.end() && status; ++it)
			status = WriteDictOperand(*it);
		if (status)
			status = WriteDictOperator(inOperator);

		return status;
	}
	bool CPrimitiveWriter::WriteIntegerOperand(long inValue)
	{
		if (-107 <= inValue && inValue <= 107)
			return WriteByte((BYTE)(inValue + 139));
		else if (108 <= inValue && inValue <= 1131)
		{
			inValue -= 108;
			WriteByte(((inValue >> 8) & 0xff) + 247);
			WriteByte(inValue & 0xff);
		}
		else if (-1131 <= inValue && inValue <= -108)
		{
			inValue = -(inValue + 108);
			WriteByte(((inValue >> 8) & 0xff) + 251);
			WriteByte(inValue & 0xff);
		}
		else if (-32768 <= inValue && inValue<= 32767)
		{
			WriteByte(28);
			WriteByte((inValue >> 8) & 0xff);
			WriteByte(inValue & 0xff);
		}
		else //  -2^31 <= inValue <= 2^31 - 1
			return Write5ByteDictInteger(inValue);
		return true;
	}
	bool CPrimitiveWriter::WriteRealOperand(double inValue, long inFractalLength)
	{
		// first, calculate the proper formatting

		bool minusSign = inValue < 0;
		bool minusExponent = false;
		bool plusExponent = false;
		unsigned short exponentSize = 0;

		if (minusSign)
			inValue = -inValue;

		double integerValue = floor(inValue);
		double fractalValue = inValue - integerValue;

		if (0 == fractalValue)
		{
			if (long(integerValue) % 1000 == 0 && integerValue >= 1000) // bother only if > 1000
			{
				plusExponent = true;
				while (long(integerValue) % 10 == 0)
				{
					++exponentSize;
					integerValue = integerValue / 10;
				}
			}
		}
		else if (0 == integerValue)
		{
			if (fractalValue <= 0.001) // bother only if < 0.001
			{
				minusExponent = true;
				while (fractalValue < 0.1)
				{
					++exponentSize;
					fractalValue = fractalValue * 10;
				}
			}
		}

		// now let's get to work
		if (!WriteByte(30))
			return false;

		// first, take care of minus sign
		BYTE buffer = minusSign ? 0xe0 : 0;
		bool usedFirst = minusSign;

		// Integer part
		if (integerValue != 0)
		{
			if (!WriteIntegerOfReal(integerValue, buffer, usedFirst))
				return false;
		}
		else
		{
			if (!SetOrWriteNibble(0, buffer, usedFirst))
				return false;
		}

		// Fractal part (if there was an integer or not)
		if (fractalValue != 0 && inFractalLength > 0)
		{
			if (!SetOrWriteNibble(0xa, buffer, usedFirst))
				return false;

			while (fractalValue != 0 && inFractalLength > 0)
			{
				if (!SetOrWriteNibble((BYTE)floor(fractalValue * 10), buffer, usedFirst))
					return false;
				fractalValue = fractalValue * 10 - floor(fractalValue * 10);
				--inFractalLength;
			}
		}

		// now, if there's any exponent, write it
		if (minusExponent)
		{
			if (!SetOrWriteNibble(0xc, buffer, usedFirst))
				return false;
			if (!WriteIntegerOfReal(exponentSize, buffer, usedFirst))
				return false;
		}
		if (plusExponent)
		{
			if (!SetOrWriteNibble(0xb, buffer, usedFirst))
				return false;
			if (!WriteIntegerOfReal(exponentSize, buffer, usedFirst))
				return false;
		}

		// final f or ff
		if (usedFirst)
			return SetOrWriteNibble(0xf, buffer, usedFirst);
		else
			return WriteByte(0xff);
	}
	bool CPrimitiveWriter::Write5ByteDictInteger(long inValue)
	{
		WriteByte(29);
		WriteByte((inValue >> 24) & 0xff);
		WriteByte((inValue >> 16)& 0xff);
		WriteByte((inValue >> 8) & 0xff);
		WriteByte(inValue & 0xff);
		return true;
	}
	bool CPrimitiveWriter::WriteIntegerOfReal(double inIntegerValue, BYTE& ioBuffer, bool& ioUsedFirst)
	{
		if (0 == inIntegerValue)
			return true;

		bool status = WriteIntegerOfReal(floor(inIntegerValue / 10), ioBuffer, ioUsedFirst);
		if (!status)
			return false;

		return SetOrWriteNibble((BYTE)(long(inIntegerValue) % 10), ioBuffer, ioUsedFirst);
	}
	bool CPrimitiveWriter::SetOrWriteNibble(BYTE inValue, BYTE& ioBuffer, bool& ioUsedFirst)
	{
		bool status = true;
		if (ioUsedFirst)
		{
			ioBuffer |= inValue;
			status = WriteByte(ioBuffer);
			ioBuffer = 0;
			ioUsedFirst = false;
		}
		else
		{
			ioBuffer = (inValue << 4) & 0xf0;
			ioUsedFirst = true;
		}
		return status;
	}
	bool CPrimitiveWriter::WriteDictOperator(unsigned short inOperator)
	{
		if (((inOperator >> 8)  & 0xff) == 12)
			return WriteCard16(inOperator);
		else
			return WriteCard8((BYTE)(inOperator & 0xff));
	}
	bool CPrimitiveWriter::Pad5Bytes()
	{
		BYTE BytesPad5[5] = {'0','0','0','0','0'};
		return Write(BytesPad5, 5);
	}
	bool CPrimitiveWriter::WriteSID(unsigned short inValue)
	{
		return WriteCard16(inValue);
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
		typedef std::map<std::string, unsigned short> StringToUShort;
		enum ECharSetType
		{
			eCharSetISOAdobe = 0,
			eCharSetExpert,
			eCharSetExpertSubset,
			eCharSetCustom
		};
		typedef std::map<unsigned short, CharString*> UShortToCharStringMap;
		struct CharSetInfo
		{
			CharSetInfo() {mSIDs = NULL;}

			ECharSetType mType;
			UShortToCharStringMap mSIDToGlyphMap;
			unsigned short* mSIDs; // count is like glyphs count
		};
		typedef std::vector<CharSetInfo*> CharSetInfoVector;
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
				return strcmp(left, right) < 0;
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

		CharString2Dependencies* mCurrentDependencies;
		CharStrings* mCurrentLocalSubrs;
		CharSetInfo* mCurrentCharsetInfo;
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
		bool ReadIndexHeader(unsigned long** outOffsets, unsigned short& outItemsCount);
		bool ReadNameIndex();
		bool ReadTopDictIndex();
		bool ReadDict(unsigned long inReadAmount, UShortToDictOperandListMap& outDict);
		bool ReadStringIndex();
		bool ReadGlobalSubrs();
		bool ReadSubrsFromIndex(unsigned short& outSubrsCount, CharStringsIndex* outSubrsIndex);
		bool ReadCharStrings();

		long long GetCharStringsPosition(unsigned short inFontIndex);
		long GetSingleIntegerValue(unsigned short inFontIndex, unsigned short inKey, long inDefault);
		long GetSingleIntegerValueFromDict(const UShortToDictOperandListMap& inDict, unsigned short inKey, long inDefault);
		static const unsigned short scCharstringType = 0x0C06;
		long GetCharStringType(unsigned short inFontIndex);
		bool ReadPrivateDicts();
		bool ReadPrivateDict(const UShortToDictOperandListMap& inReferencingDict, PrivateDictInfo* outPrivateDict);
		bool ReadLocalSubrs();
		bool ReadLocalSubrsForPrivateDict(PrivateDictInfo* inPrivateDict, BYTE inCharStringType);
		bool ReadCharsets();
		bool ReadEncodings();
		void ReadEncoding(EncodingsInfo* inEncoding, long long inEncodingPosition);
		void SetupSIDToGlyphMapWithStandard(const unsigned short* inStandardCharSet, unsigned short inStandardCharSetLength, UShortToCharStringMap& ioCharMap, const CharStrings& inCharStrings);
		bool ReadFormat0Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		bool ReadFormat1Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		bool ReadFormat2Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings);
		long long GetCharsetPosition(unsigned short inFontIndex);
		long long GetEncodingPosition(unsigned short inFontIndex);
		bool ReadCIDInformation();
		bool ReadFDArray(unsigned short inFontIndex);
		long long GetFDArrayPosition(unsigned short inFontIndex);
		bool ReadFDSelect(unsigned short inFontIndex);
		long long GetFDSelectPosition(unsigned short inFontIndex);
		unsigned short GetGlyphSID(unsigned short inFontIndex, unsigned short inGlyphIndex);
		bool PrepareForGlyphIntepretation(unsigned short inFontIndex, unsigned short inCharStringIndex);
		CharString* GetGlyphCharString(unsigned short inFontIndex, unsigned short inCharStringIndex);
		bool ReadCharString(long long inCharStringStart, long long inCharStringEnd, BYTE** outCharString);
		CharString* GetLocalSubr(long inSubrIndex);
		unsigned short GetBiasedIndex(unsigned short inSubroutineCollectionSize, long inSubroutineIndex);
		CharString* GetGlobalSubr(long inSubrIndex);
	};
	CCFFReader::CCFFReader()
	{
		mTopDictIndex = NULL;
		mStrings = NULL;
		mGlobalSubrs.mCharStringsIndex = NULL;
		mCharStrings = NULL;
		mPrivateDicts = NULL;
		mPrimitivesReader = NULL;
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
			for (unsigned int i = 0; i < mFontsCount; ++i)
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
			for (unsigned int i = 0; i < mFontsCount; ++i)
				delete[] mCharStrings[i].mCharStringsIndex;
			RELEASEARRAYOBJECTS(mCharStrings);
		}
		RELEASEARRAYOBJECTS(mPrivateDicts);

		LongFilePositionTypeToCharStringsMap::iterator itLocalSubrs = mLocalSubrs.begin();
		for (; itLocalSubrs != mLocalSubrs.end(); ++itLocalSubrs)
		{
			delete[] itLocalSubrs->second->mCharStringsIndex;
			delete itLocalSubrs->second;
		}

		CharSetInfoVector::iterator itCharSets = mCharSets.begin();
		for (; itCharSets != mCharSets.end(); ++itCharSets)
		{
			delete[] (*itCharSets)->mSIDs;
			(*itCharSets)->mSIDToGlyphMap.clear();
			delete (*itCharSets);
		}
		mCharSets.clear();

		EncodingsInfoVector::iterator itEncodings = mEncodings.begin();
		for (; itEncodings != mEncodings.end(); ++itEncodings)
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
			if (mPrimitivesReader->IsEof())
				return false;
			byte1 = mPrimitivesReader->ReadUChar();
			outValue = (byte0 - 247) * 256 + byte1 + 108;
		}
		else if (byte0 >= 251 && byte0 <= 254)
		{
			if (mPrimitivesReader->IsEof())
				return false;
			byte1 = mPrimitivesReader->ReadUChar();
			outValue = -(long)((long)byte0 - 251) * 256 - byte1 - 108;
		}
		else if (28 == byte0)
		{
			if (mPrimitivesReader->IsEof())
				return false;
			outValue = mPrimitivesReader->ReadUShort();
		}
		else if (29 == byte0)
		{
			if (mPrimitivesReader->IsEof())
				return false;
			outValue = mPrimitivesReader->ReadUInt();
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
			if (mPrimitivesReader->IsEof())
				return false;
			buffer = mPrimitivesReader->ReadUChar();

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
					else if (hasFraction)
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
			result = integerPart + fractionPart / fractionDecimal;
			if (hasNegativePower || hasPositivePower)
				result = result * pow(10, hasNegativePower ? -powerPart : powerPart);
			if (hasNegative)
				result = -result;
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
			if (!mPrimitivesReader->IsEof())
			{
				BYTE buffer = mPrimitivesReader->ReadUChar();
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
		mPrimitivesReader = new CMemoryStream(inCFFFile->GetCurBuffer(), inCFFFile->Size() - inCFFFile->Tell());
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
		return true;
	}
	bool CCFFReader::ReadIndexHeader(unsigned long** outOffsets, unsigned short& outItemsCount)
	{
		outItemsCount = mPrimitivesReader->ReadUShort();
		if (0 == outItemsCount)
		{
			*outOffsets = NULL;
			return true;
		}

		if (mPrimitivesReader->IsEof())
			return false;
		BYTE offSizeForIndex = mPrimitivesReader->ReadUChar();
		*outOffsets = new unsigned long[outItemsCount + 1];

		for (unsigned int i = 0; i <= outItemsCount && !mPrimitivesReader->IsEof(); ++i)
			(*outOffsets)[i] = mPrimitivesReader->ReadOffset(offSizeForIndex);

		return true;
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
		for (unsigned int i = 0; i < mFontsCount; ++i)
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
		return true;
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

		for (unsigned int i = 0; i < dictionariesCount && status; ++i)
			status = ReadDict(offsets[i + 1] - offsets[i], mTopDictIndex[i].mTopDict);

		delete[] offsets;
		return status;
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
		bool status = ReadIndexHeader(&offsets, mStringsCount);
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
			if (mPrimitivesReader->IsEof())
			{
				status = false;
				break;
			}
			mPrimitivesReader->Read((BYTE*)mStrings[i], &nLength);
			mStrings[i][nLength] = 0;
		}

		// failure case, null all the rest of the strings for later delete to not perofrm errors
		if (!status)
		{
			for (; i < mStringsCount; ++i)
				mStrings[i] = NULL;
		}

		// now create the string to SID map
		for (i = 0; i < N_STD_STRINGS; ++i)
			mStringToSID.insert(CharPToUShortMap::value_type(scStandardStrings[i], i));
		for (; i < N_STD_STRINGS + mStringsCount; ++i)
			mStringToSID.insert(CharPToUShortMap::value_type(mStrings[i - N_STD_STRINGS], i));

		delete[] offsets;
		return status;
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

		for (unsigned int i = 0; i < outSubrsCount; ++i)
		{
			(*outSubrsIndex)[i].mStartPosition = dataStartPosition + offsets[i] - 1;
			(*outSubrsIndex)[i].mEndPosition = dataStartPosition + offsets[i + 1] - 1;
			(*outSubrsIndex)[i].mIndex = i;
		}

		// for good faith put the pointer at the end now (if someone wants to take a snapshot)
		mPrimitivesReader->Seek(offsets[outSubrsCount] - 1, SeekCur);

		delete[] offsets;
		return true;
	}
	bool CCFFReader::ReadCharStrings()
	{
		// scan all charstrings of all included fonts
		mCharStrings = new CharStrings[mFontsCount];
		bool status = true;

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
		{
			long long charStringsPosition = GetCharStringsPosition(i);
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

		return status;
	}
	long long CCFFReader::GetCharStringsPosition(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scCharStrings, 0);
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

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
			status = ReadPrivateDict(mTopDictIndex[i].mTopDict, mPrivateDicts + i);

		return status;
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

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
			status = ReadLocalSubrsForPrivateDict(mPrivateDicts + i, (BYTE)GetCharStringType(i));

		return status;
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
					it = mLocalSubrs.insert(LongFilePositionTypeToCharStringsMap::value_type(inPrivateDict->mPrivateDictStart + subrsPosition, charStrings)).first;
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

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
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
					mPrimitivesReader->Seek((int)charsetPosition, SeekSet);
					charsetFormat = mPrimitivesReader->ReadUChar();

					if (0 == charsetFormat)
						status = ReadFormat0Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
					else if (1 == charsetFormat)
						status = ReadFormat1Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
					else // 2 charset format
						status = ReadFormat2Charset(isCID, charSet->mSIDToGlyphMap, &charSet->mSIDs, mCharStrings[i]);
				}
				mCharSets.push_back(charSet);
				it = offsetToIndex.insert(LongFilePositionTypeToCharSetInfoMap::value_type(charsetPosition, charSet)).first;
			}
			mTopDictIndex[i].mCharSet = it->second;
		}

		return status;
	}
	bool CCFFReader::ReadEncodings()
	{
		// read all encodings positions
		bool status = true;
		LongFilePositionTypeToEncodingsInfoMap offsetToEncoding;
		LongFilePositionTypeToEncodingsInfoMap::iterator it;

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
		{
			long long encodingPosition = GetEncodingPosition(i);
			it = offsetToEncoding.find(encodingPosition);
			if (it == offsetToEncoding.end())
			{
				EncodingsInfo* encoding = new EncodingsInfo();
				ReadEncoding(encoding, encodingPosition);
				mEncodings.push_back(encoding);
				it = offsetToEncoding.insert(LongFilePositionTypeToEncodingsInfoMap::value_type(encodingPosition, encoding)).first;
			}
			mTopDictIndex[i].mEncoding = it->second;
		}

		return status;
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
		for (unsigned int i = 1; i < inCharStrings.mCharStringsCount && i < inStandardCharSetLength;++i)
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
			for (unsigned int i = 1; i < inCharStrings.mCharStringsCount; ++i)
				(*inSIDArray)[i] = mPrimitivesReader->ReadUShort();
		}
		else
		{
			for (unsigned int i = 1; i < inCharStrings.mCharStringsCount; ++i)
			{
				unsigned short sid = mPrimitivesReader->ReadUShort();
				(*inSIDArray)[i] = sid;

				ioGlyphMap.insert(UShortToCharStringMap::value_type(sid, inCharStrings.mCharStringsIndex + i));
			}
		}
		return true;
	}
	bool CCFFReader::ReadFormat1Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings)
	{
		if (!inIsCID)
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
		return true;
	}
	bool CCFFReader::ReadFormat2Charset(bool inIsCID, UShortToCharStringMap& ioGlyphMap, unsigned short** inSIDArray, const CharStrings& inCharStrings)
	{
		if (!inIsCID)
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
				for (unsigned int i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i, ++glyphIndex)
					(*inSIDArray)[glyphIndex] = sid + i;
			}
		}
		else
		{
			while (glyphIndex < inCharStrings.mCharStringsCount)
			{
				sid = mPrimitivesReader->ReadUShort();
				left = mPrimitivesReader->ReadUShort();
				for (unsigned int i = 0; i <= left && glyphIndex < inCharStrings.mCharStringsCount; ++i ,++glyphIndex)
				{
					ioGlyphMap.insert(UShortToCharStringMap::value_type(sid + i, inCharStrings.mCharStringsIndex + glyphIndex));
					(*inSIDArray)[glyphIndex] = sid + i;
				}
			}
		}
		return true;
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

		for (unsigned int i = 0; i < mFontsCount && status; ++i)
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
		return status;
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

			for (unsigned int i = 0; i < glyphCount && !mPrimitivesReader->IsEof(); ++i)
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
				for (unsigned int i = 0; i < rangesCount && !mPrimitivesReader->IsEof(); ++i)
				{
					fdIndex = mPrimitivesReader->ReadUChar();
					nextRangeGlyphIndex = mPrimitivesReader->ReadUShort();
					if (!mPrimitivesReader->IsEof())
						for (unsigned int j = firstGlyphIndex; j < nextRangeGlyphIndex; ++j)
							mTopDictIndex[inFontIndex].mFDSelect[j] = mTopDictIndex[inFontIndex].mFDArray + fdIndex;
					firstGlyphIndex = nextRangeGlyphIndex;
				}
			}
		}

		return status;
	}
	long long CCFFReader::GetFDSelectPosition(unsigned short inFontIndex)
	{
		return GetSingleIntegerValue(inFontIndex, scFDSelect, 0);
	}
	unsigned short CCFFReader::GetGlyphSID(unsigned short inFontIndex, unsigned short inGlyphIndex)
	{
		if (inFontIndex >= mFontsCount || inGlyphIndex >= mCharStrings[inFontIndex].mCharStringsCount)
		{
			return 0;
		}
		else
		{
			unsigned short sid;
			if (0 == inGlyphIndex)
			{
				sid = 0;
			}
			else
			{
				if (eCharSetCustom == mTopDictIndex[inFontIndex].mCharSet->mType)
				{
					sid = mTopDictIndex[inFontIndex].mCharSet->mSIDs[inGlyphIndex];
				}
				else
				{
					// SID 0 is omitted for the default charsets
					sid = scDefaultCharsets[(BYTE)mTopDictIndex[inFontIndex].mCharSet->mType][inGlyphIndex - 1];
				}
			}
			return sid;
		}
	}
	bool CCFFReader::PrepareForGlyphIntepretation(unsigned short inFontIndex, unsigned short inCharStringIndex)
	{
		if (inFontIndex >= mFontsCount)
			return false;

		if (mCharStrings[inFontIndex].mCharStringsCount <= inCharStringIndex)
			return false;

		if (2 == mCharStrings[inFontIndex].mCharStringsType)
		{
			if (mTopDictIndex[inFontIndex].mFDSelect) // CIDs have FDSelect
			{
				mCurrentLocalSubrs = mTopDictIndex[inFontIndex].mFDSelect[inCharStringIndex]->mPrivateDict.mLocalSubrs;
				mCurrentCharsetInfo = mTopDictIndex[inFontIndex].mCharSet;
				mCurrentDependencies = NULL;
			}
			else
			{
				mCurrentLocalSubrs = mPrivateDicts[inFontIndex].mLocalSubrs;
				mCurrentCharsetInfo = mTopDictIndex[inFontIndex].mCharSet;
				mCurrentDependencies = NULL;
			}
			return true;
		}
		return false;
	}
	CharString* CCFFReader::GetGlyphCharString(unsigned short inFontIndex, unsigned short inCharStringIndex)
	{
		if (inFontIndex >= mFontsCount)
			return NULL;
		if (mCharStrings[inFontIndex].mCharStringsCount <= inCharStringIndex)
			return NULL;
		return mCharStrings[inFontIndex].mCharStringsIndex + inCharStringIndex;
	}
	bool CCFFReader::ReadCharString(long long inCharStringStart, long long inCharStringEnd, BYTE** outCharString)
	{
		bool status = true;
		mPrimitivesReader->Seek(inCharStringStart, SeekSet);
		*outCharString = NULL;

		unsigned int nReadLength = inCharStringEnd - inCharStringStart;
		*outCharString = new BYTE[nReadLength];

		mPrimitivesReader->Read(*outCharString, &nReadLength);
		if (nReadLength != inCharStringEnd - inCharStringStart)
			status = false;

		if (!status && *outCharString)
			delete[] *outCharString;

		return status;
	}
	CharString* CCFFReader::GetLocalSubr(long inSubrIndex)
	{
		// locate local subr and return. also - push it to the dependendecy stack to start calculating dependencies for it
		// also - record dependency on this subr.
		unsigned short biasedIndex = GetBiasedIndex(mCurrentLocalSubrs->mCharStringsCount, inSubrIndex);

		if (biasedIndex < mCurrentLocalSubrs->mCharStringsCount)
		{
			CharString* returnValue = mCurrentLocalSubrs->mCharStringsIndex + biasedIndex;
			if (mCurrentDependencies)
				mCurrentDependencies->mLocalSubrs.insert(biasedIndex);
			return returnValue;
		}
		return NULL;
	}
	unsigned short CCFFReader::GetBiasedIndex(unsigned short inSubroutineCollectionSize, long inSubroutineIndex)
	{
		if (inSubroutineCollectionSize < 1240)
			return (unsigned short)(107 + inSubroutineIndex);
		else if (inSubroutineCollectionSize < 33900)
			return (unsigned short)(1131 + inSubroutineIndex);
		return (unsigned short)(32768 + inSubroutineIndex);
	}
	CharString* CCFFReader::GetGlobalSubr(long inSubrIndex)
	{
		unsigned short biasedIndex = GetBiasedIndex(mGlobalSubrs.mCharStringsCount, inSubrIndex);

		if (biasedIndex < mGlobalSubrs.mCharStringsCount)
		{
			CharString* returnValue = mGlobalSubrs.mCharStringsIndex + biasedIndex;
			if (mCurrentDependencies)
				mCurrentDependencies->mGlobalSubrs.insert(biasedIndex);
			return returnValue;
		}
		return NULL;
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
		for (; it != mActualGlyphs.end(); ++it)
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

		mPrimitivesReader = new CMemoryStream(pData, nDataLength);
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
		mPrimitivesReader->ReadUInt(); // sfntVersion
		mTablesCount = mPrimitivesReader->ReadUShort();
		// skip the next 6
		mPrimitivesReader->Seek(6, SeekCur);

		for (unsigned int i = 0; i < mTablesCount; ++i)
		{
			tableTag = mPrimitivesReader->ReadUInt();
			tableEntry.CheckSum = mPrimitivesReader->ReadUInt();
			tableEntry.Offset = mPrimitivesReader->ReadUInt();
			tableEntry.Length = mPrimitivesReader->ReadUInt();
			tableEntry.Offset += mTableOffset;
			mTables.insert(UIntToTableEntryMap::value_type(tableTag, tableEntry));
		}

		return true;
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
			mPrimitivesReader->ReadUInt(); // ttcVersion
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
		(void) rdata_len;
		// verify that the header is composed as expected
		BYTE head[16], head2[16];

		mPrimitivesReader->Seek(mHeaderOffset, SeekSet);

		for (unsigned short i = 0; i < 16; ++i)
			head[i] = mPrimitivesReader->ReadUChar();

		rdata_pos = ( head[0] << 24 )  | ( head[1] << 16 )  | ( head[2] <<  8 )  | head[3] ;
		map_pos   = ( head[4] << 24 )  | ( head[5] << 16 )  | ( head[6] <<  8 )  | head[7] ;
		rdata_len = ( head[8] << 24 )  | ( head[9] << 16 )  | ( head[10] << 8 )  | head[11];

		mPrimitivesReader->Seek(map_pos, SeekSet);

		for (unsigned short i = 0; i < 16; ++i)
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

		map_offset = map_pos + type_list;

		mPrimitivesReader->Seek(map_offset, SeekSet);

		// read the resource type list

		unsigned short cnt = mPrimitivesReader->ReadUShort();

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
					unsigned short res_id;
					unsigned int temp, res_offset;
					res_id = mPrimitivesReader->ReadUShort();
					if (mPrimitivesReader->IsEof())
						break;
					mPrimitivesReader->ReadUShort(); // res_name
					if (mPrimitivesReader->IsEof())
						break;
					temp = mPrimitivesReader->ReadUInt();
					if (mPrimitivesReader->IsEof())
						break;
					mPrimitivesReader->ReadUInt(); // mbz
					if (mPrimitivesReader->IsEof())
						break;
					res_offset = temp & 0xFFFFFFL;
					resOffsetsMap.insert(std::pair<unsigned short, unsigned long>(res_id, rdata_pos + res_offset));
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

		for (; i < strlen(inTagName); ++i)
			buffer[i] = (BYTE)inTagName[i];
		for (; i < 4; ++i)
			buffer[i] = 0x20;

		return ((unsigned long)buffer[0] << 24) + ((unsigned long)buffer[1] << 16) +
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

		return true;
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
		return true;
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

		return true;
	}
	bool COpenTypeReader::ReadHMtx()
	{
		UIntToTableEntryMap::iterator it = mTables.find(GetTag("hmtx"));
		if (it == mTables.end())
			return false;

		mPrimitivesReader->Seek(it->second.Offset, SeekSet);

		mHMtx = new HMtxTableEntry[mMaxp.NumGlyphs];

		unsigned int i = 0;
		for (; i < mHHea.NumberOfHMetrics; ++i)
		{
			mHMtx[i].AdvanceWidth    = mPrimitivesReader->ReadUShort();
			mHMtx[i].LeftSideBearing = mPrimitivesReader->ReadUShort();
		}

		for (; i < mMaxp.NumGlyphs; ++i)
		{
			mHMtx[i].AdvanceWidth = mHMtx[mHHea.NumberOfHMetrics - 1].AdvanceWidth;
			mHMtx[i].LeftSideBearing = mPrimitivesReader->ReadUShort();
		}

		return true;
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
		return true;
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

		return true;
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
		return true;
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

		return true;
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
	// IType2InterpreterImplementation
	//----------------------------------------------------------------------------------------
	struct IType2InterpreterImplementation
	{
		virtual bool ReadCharString(long long inCharStringStart, long long inCharStringEnd, BYTE** outCharString) = 0;
		virtual bool Type2InterpretNumber(const CharStringOperand& inOperand) = 0;
		virtual bool Type2Hstem(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vstem(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vmoveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Rlineto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hlineto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vlineto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2RRCurveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Return(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Endchar(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hstemhm(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hintmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit) = 0;
		virtual bool Type2Cntrmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit) = 0;
		virtual bool Type2Rmoveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hmoveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vstemhm(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Rcurveline(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Rlinecurve(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vvcurveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hvcurveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hhcurveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Vhcurveto(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hflex(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Hflex1(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Flex(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Flex1(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2And(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Or(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Not(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Abs(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Add(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Sub(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Div(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Neg(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Eq(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Drop(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Put(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Get(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Ifelse(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Random(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Mul(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Sqrt(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Dup(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Exch(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Index(const CharStringOperandList& inOperandList) = 0;
		virtual bool Type2Roll(const CharStringOperandList& inOperandList) = 0;
		virtual CharString* GetLocalSubr(long inSubrIndex) = 0;
		virtual CharString* GetGlobalSubr(long inSubrIndex) = 0;
	};
	//----------------------------------------------------------------------------------------
	// CharStringType2Interpreter
	//----------------------------------------------------------------------------------------
	struct CharStringType2Interpreter
	{
		#define MAX_ARGUMENTS_STACK_SIZE 48
		#define MAX_STEM_HINTS_SIZE 96
		#define MAX_SUBR_NESTING_STACK_SIZE 10

		CharStringOperandList mOperandStack;
		unsigned short mStemsCount;
		IType2InterpreterImplementation* mImplementationHelper;
		bool mGotEndChar;
		CharStringOperandVector mStorage;
		bool mCheckedWidth;
		unsigned short mSubrsNesting;

	public:
		CharStringType2Interpreter();
		~CharStringType2Interpreter();

		bool Intepret(const CharString& inCharStringToIntepret, IType2InterpreterImplementation* inImplementationHelper);
		bool ProcessCharString(BYTE* inCharString, long long inCharStringLength);
		bool IsOperator(BYTE inCurrentByte);
		BYTE* InterpretNumber(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretOperator(BYTE* inProgramCounter, bool& outGotEndExecutionCommand, long long inReadLimit);
		void CheckWidth();
		bool AddStemsCount(unsigned short inBy);
		BYTE* InterpretHStem(BYTE* inProgramCounter, long long inReadLimit);
		void ClearStack();
		BYTE* InterpretVStem(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretVMoveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRLineto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHLineto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretVLineto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRRCurveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretCallSubr(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretReturn(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretEndChar(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHStemHM(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHintMask(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretCntrMask(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRMoveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHMoveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretVStemHM(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRCurveLine(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRLineCurve(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretVVCurveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHHCurveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretCallGSubr(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretVHCurveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHVCurveto(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretAnd(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretOr(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretNot(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretAbs(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretAdd(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretSub(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretDiv(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretNeg(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretEq(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretDrop(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretPut(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretGet(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretIfelse(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRandom(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretMul(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretSqrt(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretDup(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretExch(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretIndex(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretRoll(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHFlex(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretFlex(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretHFlex1(BYTE* inProgramCounter, long long inReadLimit);
		BYTE* InterpretFlex1(BYTE* inProgramCounter, long long inReadLimit);
	};
	CharStringType2Interpreter::CharStringType2Interpreter()
	{
		mImplementationHelper = NULL;
	}
	CharStringType2Interpreter::~CharStringType2Interpreter()
	{
	}
	bool CharStringType2Interpreter::Intepret(const CharString& inCharStringToIntepret, IType2InterpreterImplementation* inImplementationHelper)
	{
		BYTE* charString = NULL;
		bool status = false;

		mImplementationHelper = inImplementationHelper;
		mGotEndChar = false;
		mStemsCount = 0;
		mCheckedWidth = false;
		mSubrsNesting = 0;
		if (!inImplementationHelper)
			return false;

		status = mImplementationHelper->ReadCharString(inCharStringToIntepret.mStartPosition, inCharStringToIntepret.mEndPosition, &charString);
		if (!status)
			return false;

		status = ProcessCharString(charString, inCharStringToIntepret.mEndPosition - inCharStringToIntepret.mStartPosition);

		delete charString;

		return status;
	}
	bool CharStringType2Interpreter::ProcessCharString(BYTE* inCharString, long long inCharStringLength)
	{
		bool status = true;
		BYTE* pointer = inCharString;
		bool gotEndExecutionOperator = false;

		while (pointer - inCharString < inCharStringLength && status && !gotEndExecutionOperator && !mGotEndChar)
		{
			long long readLimit = inCharStringLength - (pointer - inCharString); // should be at least 1

			if (IsOperator(*pointer))
			{
				pointer = InterpretOperator(pointer, gotEndExecutionOperator, readLimit);
				if (!pointer)
					status = false;
			}
			else
			{
				pointer = InterpretNumber(pointer, readLimit);
				if (!pointer)
					status = false;

				if (mOperandStack.size() > MAX_ARGUMENTS_STACK_SIZE)
					status = false;
			}
		}
		return status;
	}
	bool CharStringType2Interpreter::IsOperator(BYTE inCurrentByte)
	{
		return ((inCurrentByte) <= 27) || (29 <= (inCurrentByte) && (inCurrentByte) <= 31);
	}
	BYTE* CharStringType2Interpreter::InterpretNumber(BYTE* inProgramCounter, long long inReadLimit)
	{
		CharStringOperand operand;
		BYTE* newPosition = inProgramCounter;

		if (inReadLimit < 1)
			return NULL; // error, cant read a single byte

		if (28 == *newPosition && inReadLimit >= 3)
		{
			operand.IsInteger = true;
			operand.IntegerValue = (short)(((unsigned short)(*(newPosition + 1)) << 8) + (*(newPosition + 2)));
			newPosition += 3;
		}
		else if (32 <= *newPosition && *newPosition <= 246)
		{
			operand.IsInteger = true;
			operand.IntegerValue = (short)*newPosition - 139;
			++newPosition;
		}
		else if (247 <= *newPosition && *newPosition <= 250  && inReadLimit >= 2)
		{
			operand.IsInteger = true;
			operand.IntegerValue = (*newPosition - 247) * 256 + *(newPosition + 1) + 108;
			newPosition += 2;
		}
		else if (251 <= *newPosition && *newPosition <= 254  && inReadLimit >= 2)
		{
			operand.IsInteger = true;
			operand.IntegerValue = -(short)(*newPosition - 251) * 256 - *(newPosition + 1) - 108;
			newPosition += 2;
		}
		else if (255 == *newPosition  && inReadLimit >= 5)
		{
			operand.IsInteger = false;
			operand.RealValue = (short)(((unsigned short)(*(newPosition + 1)) << 8) + (*(newPosition + 2)));
			if (operand.RealValue > 0)
				operand.RealValue += (double)(((unsigned short)(*(newPosition + 3)) << 8) + (*(newPosition + 4))) / (1 << 16);
			else
				operand.RealValue -= (double)(((unsigned short)(*(newPosition + 3)) << 8) + (*(newPosition + 4))) / (1 << 16);
			newPosition += 5;
		}
		else
			newPosition = NULL; // error

		if (newPosition)
		{
			mOperandStack.push_back(operand);
			bool status = mImplementationHelper->Type2InterpretNumber(operand);
			if (!status)
				return NULL;
		}

		return newPosition;
	}
	BYTE* CharStringType2Interpreter::InterpretOperator(BYTE* inProgramCounter, bool& outGotEndExecutionCommand, long long inReadLimit)
	{
		unsigned short operatorValue;
		BYTE* newPosition = inProgramCounter;
		outGotEndExecutionCommand = false;

		if (inReadLimit < 1)
			return NULL; // error, cant read a single byte

		if (12 == *newPosition)
		{
			if (inReadLimit < 2)
				return NULL;

			operatorValue = 0x0c00 + *(newPosition + 1);
			newPosition += 2;
			inReadLimit -= 2;
		}
		else
		{
			operatorValue = *newPosition;
			++newPosition;
			--inReadLimit;
		}

		switch (operatorValue)
		{
			case 1: // hstem
				CheckWidth();
				newPosition = InterpretHStem(newPosition, inReadLimit);
				break;
			case 3: // vstem
				CheckWidth();
				newPosition = InterpretVStem(newPosition, inReadLimit);
				break;
			case 4: // vmoveto
				CheckWidth();
				newPosition = InterpretVMoveto(newPosition, inReadLimit);
				break;
			case 5: // rlineto
				newPosition = InterpretRLineto(newPosition, inReadLimit);
				break;
			case 6: // hlineto
				newPosition = InterpretHLineto(newPosition, inReadLimit);
				break;
			case 7: // vlineto
				newPosition = InterpretVLineto(newPosition, inReadLimit);
				break;
			case 8: // rrcurveto
				newPosition = InterpretRRCurveto(newPosition, inReadLimit);
				break;
			case 10: // callsubr
				newPosition = InterpretCallSubr(newPosition, inReadLimit);
				break;
			case 11: // return
				newPosition = InterpretReturn(newPosition, inReadLimit);
				outGotEndExecutionCommand = true;
				break;
			case 14: // endchar
				CheckWidth();
				newPosition = InterpretEndChar(newPosition, inReadLimit);
				break;
			case 18: // hstemhm
				CheckWidth();
				newPosition = InterpretHStemHM(newPosition, inReadLimit);
				break;
			case 19: // hintmask
				CheckWidth();
				newPosition = InterpretHintMask(newPosition, inReadLimit);
				break;
			case 20: // cntrmask
				CheckWidth();
				newPosition = InterpretCntrMask(newPosition, inReadLimit);
				break;
			case 21: // rmoveto
				CheckWidth();
				newPosition = InterpretRMoveto(newPosition, inReadLimit);
				break;
			case 22: // hmoveto
				CheckWidth();
				newPosition = InterpretHMoveto(newPosition, inReadLimit);
				break;
			case 23: // vstemhm
				CheckWidth();
				newPosition = InterpretVStemHM(newPosition, inReadLimit);
				break;
			case 24: // rcurveline
				newPosition = InterpretRCurveLine(newPosition, inReadLimit);
				break;
			case 25: // rlinecurve
				newPosition = InterpretRLineCurve(newPosition, inReadLimit);
				break;
			case 26: // vvcurveto
				newPosition = InterpretVVCurveto(newPosition, inReadLimit);
				break;
			case 27: // hhcurveto
				newPosition = InterpretHHCurveto(newPosition, inReadLimit);
				break;
			case 29: // callgsubr
				newPosition = InterpretCallGSubr(newPosition, inReadLimit);
				break;
			case 30: // vhcurveto
				newPosition = InterpretVHCurveto(newPosition, inReadLimit);
				break;
			case 31: // hvcurveto
				newPosition = InterpretHVCurveto(newPosition, inReadLimit);
				break;
			case 0x0c00: // dotsection, depracated
				// ignore
				break;
			case 0x0c03: // and
				newPosition = InterpretAnd(newPosition, inReadLimit);
				break;
			case 0x0c04: // or
				newPosition = InterpretOr(newPosition, inReadLimit);
				break;
			case 0x0c05: // not
				newPosition = InterpretNot(newPosition, inReadLimit);
				break;
			case 0x0c09: // abs
				newPosition = InterpretAbs(newPosition, inReadLimit);
				break;
			case 0x0c0a: // add
				newPosition = InterpretAdd(newPosition, inReadLimit);
				break;
			case 0x0c0b: // sub
				newPosition = InterpretSub(newPosition, inReadLimit);
				break;
			case 0x0c0c: // div
				newPosition = InterpretDiv(newPosition, inReadLimit);
				break;
			case 0x0c0e: // neg
				newPosition = InterpretNeg(newPosition, inReadLimit);
				break;
			case 0x0c0f: // eq
				newPosition = InterpretEq(newPosition, inReadLimit);
				break;
			case 0x0c12: // drop
				newPosition = InterpretDrop(newPosition, inReadLimit);
				break;
			case 0x0c14: // put
				newPosition = InterpretPut(newPosition, inReadLimit);
				break;
			case 0x0c15: // get
				newPosition = InterpretGet(newPosition, inReadLimit);
				break;
			case 0x0c16: // ifelse
				newPosition = InterpretIfelse(newPosition, inReadLimit);
				break;
			case 0x0c17: // random
				newPosition = InterpretRandom(newPosition, inReadLimit);
				break;
			case 0x0c18: // mul
				newPosition = InterpretMul(newPosition, inReadLimit);
				break;
			case 0x0c1a: // sqrt
				newPosition = InterpretSqrt(newPosition, inReadLimit);
				break;
			case 0x0c1b: // dup
				newPosition = InterpretDup(newPosition, inReadLimit);
				break;
			case 0x0c1c: // exch
				newPosition = InterpretExch(newPosition, inReadLimit);
				break;
			case 0x0c1d: // index
				newPosition = InterpretIndex(newPosition, inReadLimit);
				break;
			case 0x0c1e: // roll
				newPosition = InterpretRoll(newPosition, inReadLimit);
				break;
			case 0x0c22: // hflex
				newPosition = InterpretHFlex(newPosition, inReadLimit);
				break;
			case 0x0c23: // flex
				newPosition = InterpretFlex(newPosition, inReadLimit);
				break;
			case 0x0c24: // hflex1
				newPosition = InterpretHFlex1(newPosition, inReadLimit);
				break;
			case 0x0c25: // flex1
				newPosition = InterpretFlex1(newPosition, inReadLimit);
				break;
		}
		return newPosition;
	}
	void CharStringType2Interpreter::CheckWidth()
	{
		if (!mCheckedWidth)
		{
			if (mOperandStack.size() % 2 != 0) // has width
				mOperandStack.pop_front();
			mCheckedWidth = true;
		}
	}
	bool CharStringType2Interpreter::AddStemsCount(unsigned short inBy)
	{
		if (mStemsCount + inBy > MAX_STEM_HINTS_SIZE)
			return false;
		mStemsCount += inBy;
		return true;
	}
	BYTE* CharStringType2Interpreter::InterpretHStem(BYTE* inProgramCounter, long long)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2));
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Hstem(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	void CharStringType2Interpreter::ClearStack()
	{
		mOperandStack.clear();
	}
	BYTE* CharStringType2Interpreter::InterpretVStem(BYTE* inProgramCounter, long long)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2));
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Vstem(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretVMoveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Vmoveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRLineto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Rlineto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHLineto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hlineto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretVLineto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Vlineto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRRCurveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2RRCurveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretCallSubr(BYTE* inProgramCounter, long long)
	{
		CharString* aCharString = NULL;
		if (mOperandStack.size() < 1)
			return NULL;

		aCharString = mImplementationHelper->GetLocalSubr(mOperandStack.back().IntegerValue);
		mOperandStack.pop_back();

		if (aCharString != NULL)
		{
			BYTE* charString = NULL;
			bool status = mImplementationHelper->ReadCharString(aCharString->mStartPosition, aCharString->mEndPosition, &charString);
			if (!status)
			{
				delete charString;
				return NULL;
			}

			++mSubrsNesting;

			if (mSubrsNesting > MAX_SUBR_NESTING_STACK_SIZE)
			{
				delete charString;
				return NULL;
			}

			status = ProcessCharString(charString, aCharString->mEndPosition - aCharString->mStartPosition);
			--mSubrsNesting;

			delete charString;
			if (!status)
				return NULL;
			return inProgramCounter;
		}
		return NULL;
	}
	BYTE* CharStringType2Interpreter::InterpretReturn(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Return(mOperandStack);
		if (!status)
			return NULL;

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretEndChar(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Endchar(mOperandStack);
		if (!status)
			return NULL;

		mGotEndChar = true;
		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHStemHM(BYTE* inProgramCounter, long long)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2));
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Hstemhm(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHintMask(BYTE* inProgramCounter, long long inReadLimit)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2)); // assuming this is a shortcut of dropping vstem if got arguments
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Hintmask(mOperandStack, inProgramCounter, inReadLimit);
		if (!status)
			return NULL;

		ClearStack();
		long long programCounterStemReadSize = (mStemsCount / 8 + (mStemsCount % 8 != 0 ? 1 : 0));
		if (programCounterStemReadSize > inReadLimit)
			return NULL;
		return inProgramCounter + programCounterStemReadSize;
	}
	BYTE* CharStringType2Interpreter::InterpretCntrMask(BYTE* inProgramCounter, long long inReadLimit)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2)); // assuming this is a shortcut of dropping vstem if got arguments
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Cntrmask(mOperandStack, inProgramCounter, inReadLimit);
		if (!status)
			return NULL;

		ClearStack();
		long long programCounterStemReadSize = (mStemsCount / 8 + (mStemsCount % 8 != 0 ? 1 : 0));
		if (programCounterStemReadSize > inReadLimit)
			return NULL;
		return inProgramCounter + programCounterStemReadSize;
	}
	BYTE* CharStringType2Interpreter::InterpretRMoveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Rmoveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHMoveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hmoveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretVStemHM(BYTE* inProgramCounter, long long)
	{
		bool status = AddStemsCount((unsigned short)(mOperandStack.size() / 2));
		if (!status)
			return NULL;

		status = mImplementationHelper->Type2Vstemhm(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRCurveLine(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Rcurveline(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRLineCurve(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Rlinecurve(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretVVCurveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Vvcurveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHHCurveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hhcurveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretCallGSubr(BYTE* inProgramCounter, long long)
	{
		CharString* aCharString = NULL;
		if (mOperandStack.size() < 1)
			return NULL;

		aCharString = mImplementationHelper->GetGlobalSubr(mOperandStack.back().IntegerValue);
		mOperandStack.pop_back();

		if (aCharString != NULL)
		{
			BYTE* charString = NULL;
			bool status = mImplementationHelper->ReadCharString(aCharString->mStartPosition, aCharString->mEndPosition, &charString);
			if (!status)
			{
				delete charString;
				return NULL;
			}

			++mSubrsNesting;

			if (mSubrsNesting > MAX_SUBR_NESTING_STACK_SIZE)
			{
				delete charString;
				return NULL;
			}

			status = ProcessCharString(charString, aCharString->mEndPosition - aCharString->mStartPosition);
			--mSubrsNesting;

			delete charString;
			if (!status)
				return NULL;
			return inProgramCounter;
		}
		return NULL;
	}
	BYTE* CharStringType2Interpreter::InterpretVHCurveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Vhcurveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHVCurveto(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hvcurveto(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretAnd(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2And(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;
		newOperand.IsInteger = true;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		newOperand.IntegerValue = (
			(valueB.IsInteger ? valueB.IntegerValue : valueB.RealValue) &&
			(valueA.IsInteger ? valueA.IntegerValue : valueA.RealValue)
			) ? 1 : 0;
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretOr(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Or(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;
		newOperand.IsInteger = true;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		newOperand.IntegerValue = (
			(valueB.IsInteger ? valueB.IntegerValue : valueB.RealValue) ||
			(valueA.IsInteger ? valueA.IntegerValue : valueA.RealValue)
			) ? 1:0;	mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretNot(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Not(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;
		CharStringOperand newOperand;
		newOperand.IsInteger = true;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		mOperandStack.pop_back();

		newOperand.IntegerValue = (value.IsInteger ? value.IntegerValue : value.RealValue)  ? 1 : 0;
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretAbs(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Abs(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		newOperand.IsInteger = value.IsInteger;
		mOperandStack.pop_back();

		if (value.IsInteger)
			newOperand.IntegerValue = labs(value.IntegerValue);
		else
			newOperand.RealValue = fabs(value.RealValue);
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretAdd(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Add(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		if (!valueA.IsInteger || !valueB.IsInteger)
		{
			newOperand.IsInteger = false;
			newOperand.RealValue =
				(valueA.IsInteger ? (double)valueA.IntegerValue : valueA.RealValue)
				+
				(valueB.IsInteger ? (double)valueB.IntegerValue : valueB.RealValue);
		}
		else
		{
			newOperand.IsInteger = true;
			newOperand.IntegerValue = valueA.IntegerValue + valueB.IntegerValue;
		}
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretSub(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Sub(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		if (!valueA.IsInteger || !valueB.IsInteger)
		{
			newOperand.IsInteger = false;
			newOperand.RealValue =
				(valueA.IsInteger ? (double)valueA.IntegerValue : valueA.RealValue)
				-
				(valueB.IsInteger ? (double)valueB.IntegerValue : valueB.RealValue);
		}
		else
		{
			newOperand.IsInteger = true;
			newOperand.IntegerValue = valueA.IntegerValue - valueB.IntegerValue;
		}
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretDiv(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Div(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;


		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		if (!valueA.IsInteger || !valueB.IsInteger)
		{
			newOperand.IsInteger = false;
			newOperand.RealValue =
				(valueA.IsInteger ? (double)valueA.IntegerValue : valueA.RealValue)
				/
				(valueB.IsInteger ? (double)valueB.IntegerValue : valueB.RealValue);
		}
		else
		{
			newOperand.IsInteger = true;
			newOperand.IntegerValue = valueA.IntegerValue / valueB.IntegerValue;
		}
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretNeg(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Neg(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		newOperand.IsInteger = value.IsInteger;
		mOperandStack.pop_back();

		if (value.IsInteger)
			newOperand.IntegerValue = -value.IntegerValue;
		else
			newOperand.RealValue = -value.RealValue;
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretEq(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Eq(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();


		newOperand.IsInteger = true;
		newOperand.IntegerValue = (
		(valueB.IsInteger ? valueB.IntegerValue : valueB.RealValue) ==
		(valueA.IsInteger ? valueA.IntegerValue : valueA.RealValue)
		) ? 1 : 0;
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretDrop(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Drop(mOperandStack);
		if (!status)
			return NULL;

		if (mOperandStack.size() < 1)
			return NULL;

		mOperandStack.pop_back();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretPut(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Put(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		mStorage[(valueB.IsInteger ? valueB.IntegerValue : (long)valueB.RealValue)] = valueA;

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretGet(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Get(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		mOperandStack.pop_back();
		long index = (value.IsInteger ? value.IntegerValue : (long)value.RealValue);

		if ((mStorage.size() > (unsigned long)index) && (index >= 0))
		{
			mOperandStack.push_back(mStorage[index]);
			return inProgramCounter;
		}
		return NULL;
	}
	BYTE* CharStringType2Interpreter::InterpretIfelse(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Ifelse(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand valueC;
		CharStringOperand valueD;

		if (mOperandStack.size() < 4)
			return NULL;

		valueD = mOperandStack.back();
		mOperandStack.pop_back();
		valueC = mOperandStack.back();
		mOperandStack.pop_back();
		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		if (!valueC.IsInteger || !valueD.IsInteger)
		{
			if ((valueC.IsInteger ? (double)valueC.IntegerValue : valueC.RealValue) >
				(valueD.IsInteger ? (double)valueD.IntegerValue : valueD.RealValue))
				mOperandStack.push_back(valueB);
			else
				mOperandStack.push_back(valueA);
		}
		else
		{
			if (valueC.IntegerValue > valueD.IntegerValue)
				mOperandStack.push_back(valueB);
			else
				mOperandStack.push_back(valueA);
		}

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRandom(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Random(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand newOperand;

		newOperand.IsInteger = false;
		newOperand.RealValue = ((double)rand() + 1) / ((double)RAND_MAX + 1);

		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretMul(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Mul(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		if (!valueA.IsInteger || !valueB.IsInteger)
		{
			newOperand.IsInteger = false;
			newOperand.RealValue =
				(valueA.IsInteger ? (double)valueA.IntegerValue : valueA.RealValue)
				*
				(valueB.IsInteger ? (double)valueB.IntegerValue : valueB.RealValue);
		}
		else
		{
			newOperand.IsInteger = true;
			newOperand.IntegerValue = valueA.IntegerValue * valueB.IntegerValue;
		}
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretSqrt(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Sqrt(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;
		CharStringOperand newOperand;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		mOperandStack.pop_back();

		newOperand.IsInteger = false;
		newOperand.RealValue = sqrt(value.IsInteger ? value.IntegerValue:value.RealValue);
		mOperandStack.push_back(newOperand);
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretDup(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Dup(mOperandStack);
		if (!status)
			return NULL;

		if (mOperandStack.size() < 1)
			return NULL;

		mOperandStack.push_back(mOperandStack.back());
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretExch(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Exch(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;

		if (mOperandStack.size() < 2)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		mOperandStack.push_back(valueB);
		mOperandStack.push_back(valueA);

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretIndex(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Index(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand value;

		if (mOperandStack.size() < 1)
			return NULL;

		value = mOperandStack.back();
		mOperandStack.pop_back();
		long index = (value.IsInteger ? value.IntegerValue : (long)value.RealValue);
		CharStringOperandList::reverse_iterator it = mOperandStack.rbegin();

		while (index > 0 && it != mOperandStack.rend())
			++it;
		mOperandStack.push_back(*it);

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretRoll(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Roll(mOperandStack);
		if (!status)
			return NULL;

		CharStringOperand valueA;
		CharStringOperand valueB;

		if (mOperandStack.size() < 1)
			return NULL;

		valueB = mOperandStack.back();
		mOperandStack.pop_back();
		valueA = mOperandStack.back();
		mOperandStack.pop_back();

		long shiftAmount = (valueB.IsInteger ? valueB.IntegerValue : (long)valueB.RealValue);
		long itemsCount  = (valueA.IsInteger ? valueA.IntegerValue : (long)valueA.RealValue);

		if (itemsCount > 0)
		{
			CharStringOperandList groupToShift;

			for (long i = 0; i < itemsCount && mOperandStack.size() > 0; ++i)
			{
				groupToShift.push_front(mOperandStack.back());
				mOperandStack.pop_back();
			}

			if (shiftAmount > 0)
			{
				for (long j = 0; j < shiftAmount; ++j)
				{
					groupToShift.push_front(groupToShift.back());
					groupToShift.pop_back();
				}
			}
			else
			{
				for (long j = 0; j < -shiftAmount; ++j)
				{
					groupToShift.push_back(groupToShift.front());
					groupToShift.pop_front();
				}
			}

			// put back the rolled group
			for (long i = 0; i < itemsCount; ++i)
			{
				mOperandStack.push_back(groupToShift.front());
				groupToShift.pop_front();
			}
		}

		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHFlex(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hflex(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretFlex(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Flex(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretHFlex1(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Hflex1(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	BYTE* CharStringType2Interpreter::InterpretFlex1(BYTE* inProgramCounter, long long)
	{
		bool status = mImplementationHelper->Type2Flex1(mOperandStack);
		if (!status)
			return NULL;

		ClearStack();
		return inProgramCounter;
	}
	//----------------------------------------------------------------------------------------
	// CharStringType2Flattener
	//----------------------------------------------------------------------------------------
	struct CharStringType2Flattener : public IType2InterpreterImplementation
	{
		CMemoryStream* mWriter;
		CCFFReader* mHelper;
		unsigned short mStemsCount;
		CharStringOperandList mOperandsToWrite;
	public:
		CharStringType2Flattener();
		~CharStringType2Flattener();

		// will write a font program to another stream, flattening the references to subrs and gsubrs, so that
		// the charstring becomes independent (with possible references to other charachters through seac-like endchar)
		bool WriteFlattenedGlyphProgram(unsigned short inFontIndex, unsigned short inGlyphIndex, CCFFReader* inCFFFileInput, CMemoryStream* inWriter);
		virtual bool ReadCharString(long long inCharStringStart, long long inCharStringEnd, BYTE** outCharString) override;
		virtual bool Type2InterpretNumber(const CharStringOperand& inOperand) override;
		virtual bool Type2Hstem(const CharStringOperandList& inOperandList) override;
		bool WriteRegularOperator(unsigned short inOperatorCode);
		bool WriteCharStringOperand(const CharStringOperand& inOperand);
		bool WriteCharStringOperator(unsigned short inOperatorCode);
		bool WriteByte(BYTE inValue);
		virtual bool Type2Vstem(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Vmoveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Rlineto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hlineto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Vlineto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2RRCurveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Return(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Endchar(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hstemhm(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hintmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit) override;
		bool WriteStemMask(BYTE* inProgramCounter, long long inReadLimit);
		bool WriteSubrOperator(unsigned short inOperatorCode);
		virtual bool Type2Cntrmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit) override;
		virtual bool Type2Rmoveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hmoveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Vstemhm(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Rcurveline(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Rlinecurve(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Vvcurveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hvcurveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hhcurveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Vhcurveto(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hflex(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Hflex1(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Flex(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Flex1(const CharStringOperandList& inOperandList) override;
		virtual bool Type2And(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Or(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Not(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Abs(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Add(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Sub(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Div(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Neg(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Eq(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Drop(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Put(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Get(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Ifelse(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Random(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Mul(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Sqrt(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Dup(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Exch(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Index(const CharStringOperandList& inOperandList) override;
		virtual bool Type2Roll(const CharStringOperandList& inOperandList) override;
		virtual CharString* GetLocalSubr(long inSubrIndex) override;
		virtual CharString* GetGlobalSubr(long inSubrIndex) override;
	};
	CharStringType2Flattener::CharStringType2Flattener()
	{
	}
	CharStringType2Flattener::~CharStringType2Flattener()
	{
	}
	bool CharStringType2Flattener::WriteFlattenedGlyphProgram(unsigned short inFontIndex, unsigned short inGlyphIndex, CCFFReader* inCFFFileInput, CMemoryStream* inWriter)
	{
		CharStringType2Interpreter interpreter;
		bool status = inCFFFileInput->PrepareForGlyphIntepretation(inFontIndex, inGlyphIndex);

		mWriter = inWriter;
		mHelper = inCFFFileInput;
		mOperandsToWrite.clear();
		mStemsCount = 0;

		if (!status)
			return false;

		CharString* charString = inCFFFileInput->GetGlyphCharString(inFontIndex, inGlyphIndex);
		if (!charString)
			return false;

		status = interpreter.Intepret(*charString, this);
		/*
			The alrogithm for writing a flattened charstring is as follows:
			1. enumerator, through interpretation, the charstring
			2. hit an operand? accumulate.
			3. hit an operator? if it's not callgsubr or callsubr just write the operand stack, and continue.
								if it is callgsubr/callsubr pop the last element on the operand stack and write it, then continue.
			4. an exception would be when callgsubr/callsubr follow an operator, in which case their index operand is already written. just call drop.

		*/
		return status;
	}
	bool CharStringType2Flattener::ReadCharString(long long inCharStringStart, long long inCharStringEnd, BYTE** outCharString)
	{
		return mHelper->ReadCharString(inCharStringStart, inCharStringEnd, outCharString);
	}
	bool CharStringType2Flattener::Type2InterpretNumber(const CharStringOperand& inOperand)
	{
		mOperandsToWrite.push_back(inOperand);
		return true;
	}
	bool CharStringType2Flattener::Type2Hstem(const CharStringOperandList& inOperandList)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);
		return WriteRegularOperator(1);
	}
	bool CharStringType2Flattener::WriteRegularOperator(unsigned short inOperatorCode)
	{
		CharStringOperandList::iterator it = mOperandsToWrite.begin();
		bool status = true;

		for (; it != mOperandsToWrite.end() && status; ++it)
			status = WriteCharStringOperand(*it);
		if (status)
			status = WriteCharStringOperator(inOperatorCode);

		mOperandsToWrite.clear();

		return status;
	}
	bool CharStringType2Flattener::WriteCharStringOperand(const CharStringOperand& inOperand)
	{
		if (inOperand.IsInteger)
		{
			long value = inOperand.IntegerValue;

			if (-107 <= value && value <= 107)
			{
				return WriteByte((BYTE)(value + 139));
			}
			else if (108 <= value && value <= 1131)
			{
				value -= 108;
				WriteByte(((value >> 8) & 0xff) + 247);
				WriteByte(value & 0xff);
			}
			else if (-1131 <= value && value <= -108)
			{
				value = -(value + 108);
				WriteByte(((value >> 8) & 0xff) + 251);
				WriteByte(value & 0xff);
			}
			else if (-32768 <= value && value<= 32767)
			{
				WriteByte(28);
				WriteByte((value >> 8) & 0xff);
				WriteByte(value & 0xff);
			}
			else
				return false;
		}
		else
		{
			double value = inOperand.RealValue;
			bool sign = inOperand.RealValue < 0;
			if (sign)
				value = -value;
			long integerPart = (long)floor(value);
			long realPart = (long)((value - floor(value)) * 65536);
			if (sign)
				integerPart = -integerPart;

			WriteByte(BYTE(0xff));
			WriteByte(BYTE((integerPart >> 8) & 0xff));
			WriteByte(BYTE(integerPart & 0xff));
			WriteByte(BYTE((realPart >> 8) & 0xff));
			WriteByte(BYTE(realPart & 0xff));
		}
		return true;
	}
	bool CharStringType2Flattener::WriteCharStringOperator(unsigned short inOperatorCode)
	{
		if ((inOperatorCode & 0xff00) == 0x0c00)
		{
			if (!WriteByte(0x0c))
				return false;
		}
		return WriteByte(BYTE(inOperatorCode & 0xff));
	}
	bool CharStringType2Flattener::WriteByte(BYTE inValue)
	{
		mWriter->Write(&inValue, 1);
		return true;
	}
	bool CharStringType2Flattener::Type2Vstem(const CharStringOperandList& inOperandList)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);
		return WriteRegularOperator(3);
	}
	bool CharStringType2Flattener::Type2Vmoveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(4);
	}
	bool CharStringType2Flattener::Type2Rlineto(const CharStringOperandList&)
	{
		return WriteRegularOperator(5);
	}
	bool CharStringType2Flattener::Type2Hlineto(const CharStringOperandList&)
	{
		return WriteRegularOperator(6);
	}
	bool CharStringType2Flattener::Type2Vlineto(const CharStringOperandList&)
	{
		return WriteRegularOperator(7);
	}
	bool CharStringType2Flattener::Type2RRCurveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(8);
	}
	bool CharStringType2Flattener::Type2Return(const CharStringOperandList&)
	{
		// ignore returns
		return true;
	}
	bool CharStringType2Flattener::Type2Endchar(const CharStringOperandList&)
	{
		return WriteRegularOperator(14);
	}
	bool CharStringType2Flattener::Type2Hstemhm(const CharStringOperandList& inOperandList)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);
		return WriteRegularOperator(18);
	}
	bool CharStringType2Flattener::Type2Hintmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);

		if (!WriteRegularOperator(19))
			return false;

		return WriteStemMask(inProgramCounter, inReadLimit);
	}
	bool CharStringType2Flattener::WriteStemMask(BYTE* inProgramCounter, long long inReadLimit)
	{
		unsigned short maskSize = mStemsCount / 8 + (mStemsCount % 8 != 0 ? 1 : 0);

		if (maskSize > inReadLimit)
			return false;

		mWriter->Write(inProgramCounter, maskSize);
		return true;
	}
	bool CharStringType2Flattener::WriteSubrOperator(unsigned short)
	{
		if (mOperandsToWrite.size() > 0)
		{
			bool status = true;
			mOperandsToWrite.pop_back(); // pop back parameter, which is the subr index

			// now continue writing all operands
			CharStringOperandList::iterator it = mOperandsToWrite.begin();

			for (; it != mOperandsToWrite.end() && status; ++it)
				status = WriteCharStringOperand(*it);

			mOperandsToWrite.clear();
			return status;
		}
		else // no current operands. either result of calculation or just multiple operators one of the other
			return WriteCharStringOperator(0x0c12); // write a "drop" command for the subr index
	}
	bool CharStringType2Flattener::Type2Cntrmask(const CharStringOperandList& inOperandList, BYTE* inProgramCounter, long long inReadLimit)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);

		if (!WriteRegularOperator(20))
			return false;

		return WriteStemMask(inProgramCounter, inReadLimit);
	}
	bool CharStringType2Flattener::Type2Rmoveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(21);
	}
	bool CharStringType2Flattener::Type2Hmoveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(22);
	}
	bool CharStringType2Flattener::Type2Vstemhm(const CharStringOperandList& inOperandList)
	{
		mStemsCount += (unsigned short)(inOperandList.size() / 2);

		return WriteRegularOperator(23);
	}
	bool CharStringType2Flattener::Type2Rcurveline(const CharStringOperandList&)
	{
		return WriteRegularOperator(24);
	}
	bool CharStringType2Flattener::Type2Rlinecurve(const CharStringOperandList&)
	{
		return WriteRegularOperator(25);
	}
	bool CharStringType2Flattener::Type2Vvcurveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(26);
	}
	bool CharStringType2Flattener::Type2Hvcurveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(31);
	}
	bool CharStringType2Flattener::Type2Hhcurveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(27);
	}
	bool CharStringType2Flattener::Type2Vhcurveto(const CharStringOperandList&)
	{
		return WriteRegularOperator(30);
	}
	bool CharStringType2Flattener::Type2Hflex(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c22);
	}
	bool CharStringType2Flattener::Type2Hflex1(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c24);
	}
	bool CharStringType2Flattener::Type2Flex(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c23);
	}
	bool CharStringType2Flattener::Type2Flex1(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c25);
	}
	bool CharStringType2Flattener::Type2And(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c03);
	}
	bool CharStringType2Flattener::Type2Or(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c04);
	}
	bool CharStringType2Flattener::Type2Not(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c05);
	}
	bool CharStringType2Flattener::Type2Abs(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c09);
	}
	bool CharStringType2Flattener::Type2Add(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c0a);
	}
	bool CharStringType2Flattener::Type2Sub(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c0b);
	}
	bool CharStringType2Flattener::Type2Div(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c0c);
	}
	bool CharStringType2Flattener::Type2Neg(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c0e);
	}
	bool CharStringType2Flattener::Type2Eq(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c0f);
	}
	bool CharStringType2Flattener::Type2Drop(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c12);
	}
	bool CharStringType2Flattener::Type2Put(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c14);
	}
	bool CharStringType2Flattener::Type2Get(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c15);
	}
	bool CharStringType2Flattener::Type2Ifelse(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c16);
	}
	bool CharStringType2Flattener::Type2Random(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c17);
	}
	bool CharStringType2Flattener::Type2Mul(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c18);
	}
	bool CharStringType2Flattener::Type2Sqrt(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c1a);
	}
	bool CharStringType2Flattener::Type2Dup(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c1b);
	}
	bool CharStringType2Flattener::Type2Exch(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c1c);
	}
	bool CharStringType2Flattener::Type2Index(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c1d);
	}
	bool CharStringType2Flattener::Type2Roll(const CharStringOperandList&)
	{
		return WriteRegularOperator(0x0c1e);
	}
	CharString* CharStringType2Flattener::GetLocalSubr(long inSubrIndex)
	{
		if (!WriteSubrOperator(10))
			return NULL;

		return mHelper->GetLocalSubr(inSubrIndex);
	}
	CharString* CharStringType2Flattener::GetGlobalSubr(long inSubrIndex)
	{
		if (!WriteSubrOperator(29))
			return NULL;

		return mHelper->GetGlobalSubr(inSubrIndex);
	}
	//----------------------------------------------------------------------------------------
	// CCFFWriter
	//----------------------------------------------------------------------------------------
	const unsigned short scEmbeddedPostscript = 0xC15;
	struct CCFFWriter
	{
		typedef std::pair<BYTE, unsigned short> ByteAndUShort;
		typedef std::list<ByteAndUShort> ByteAndUShortList;
		typedef std::map<FontDictInfo*, BYTE> FontDictInfoToByteMap;
		typedef std::set<FontDictInfo*> FontDictInfoSet;
		typedef std::pair<long long, long long> LongFilePositionTypePair;
		typedef std::map<FontDictInfo*, LongFilePositionTypePair> FontDictInfoToLongFilePositionTypePairMap;

		BYTE* mFile;
		COpenTypeReader mOpenTypeInput;
		CPrimitiveWriter* mPrimitivesWriter;
		CStream* mFontFileStream;
		bool mIsCID;
		std::string mOptionalEmbeddedPostscript;
		unsigned short mSubsetFontGlyphsCount;

		int mCharsetPlaceHolderPosition;
		int mEncodingPlaceHolderPosition;
		int mCharstringsPlaceHolderPosition;
		int mPrivatePlaceHolderPosition;
		int mFDArrayPlaceHolderPosition;
		int mFDSelectPlaceHolderPosition;

		long long mEncodingPosition;
		long long mCharsetPosition;
		long long mCharStringPosition;
		long long mPrivatePosition;
		long long mPrivateSize;
		long long mFDArrayPosition;
		long long mFDSelectPosition;

	public:
		CCFFWriter();
		~CCFFWriter();

		bool CreateCFFSubset(BYTE* pFile, unsigned int nLen, unsigned short unFontIndex, const std::string& inSubsetFontName, CStream* pOutputStream, unsigned short* pCodeToGID, unsigned int unCodesCount);
		bool WriteCFFHeader();
		bool WriteName(const std::string& inSubsetFontName);
		BYTE GetMostCompressedOffsetSize(unsigned long inOffset);
		bool WriteTopIndex();
		bool WriteTopDictSegment(CMemoryStream* ioTopDictSegment);
		bool WriteStringIndex();
		bool WriteGlobalSubrsIndex();
		bool WriteEncodings(const std::vector<unsigned int>& inSubsetGlyphIDs);
		bool WriteCharsets(const std::vector<unsigned int>& inSubsetGlyphIDs, std::vector<unsigned short>* inCIDMapping);
		void DetermineFDArrayIndexes(const std::vector<unsigned int>& inSubsetGlyphIDs, FontDictInfoToByteMap& outNewFontDictsIndexes);
		bool WriteFDSelect(const std::vector<unsigned int>& inSubsetGlyphIDs, const FontDictInfoToByteMap& inNewFontDictsIndexes);
		bool WriteCharStrings(const std::vector<unsigned int>& inSubsetGlyphIDs);
		bool WritePrivateDictionary();
		void WritePrivateDictionaryBody(const PrivateDictInfo& inPrivateDictionary, long long& outWriteSize, long long& outWritePosition);
		bool WriteFDArray(const std::vector<unsigned int>& inSubsetGlyphIDs, const FontDictInfoToByteMap& inNewFontDictsIndexes);
		bool UpdateIndexesAtTopDict();
	};
	CCFFWriter::CCFFWriter()
	{
		mPrimitivesWriter = NULL;
	}
	CCFFWriter::~CCFFWriter()
	{
		RELEASEOBJECT(mPrimitivesWriter);
	}
	bool CCFFWriter::CreateCFFSubset(BYTE* pFile, unsigned int nLen, unsigned short unFontIndex, const std::string& inSubsetFontName, CStream* pOutputStream, unsigned short* pCodeToGID, unsigned int unCodesCount)
	{
		mFile = pFile;

		bool status = mOpenTypeInput.ReadOpenTypeFile(pFile, nLen, unFontIndex);
		if (!status)
			return false;

		if (mOpenTypeInput.GetOpenTypeFontType() != COpenTypeReader::EOpenTypeInputType::EOpenTypeCFF)
			return false;

		// see if font may be embedded
		if (mOpenTypeInput.mOS2Exists && !FSType::CanEmbed(mOpenTypeInput.mOS2.fsType))
			return true;

		std::vector<unsigned int> subsetGlyphIDs;
		for (unsigned long i = 0; i < unCodesCount; ++i)
			subsetGlyphIDs.push_back(pCodeToGID[i]);
		if (subsetGlyphIDs.empty())
			subsetGlyphIDs.push_back(0);

		// Добавить зависимые глифы
		// Они есть в m_vCodeToGid из pCodeToGID. В pUseGlyfs они тоже есть из m_mGlyphs, но только в m_mGlyphs, они имеют false

		mSubsetFontGlyphsCount = subsetGlyphIDs.size(); // == unCodesCount

		mIsCID = mOpenTypeInput.mCFF.mTopDictIndex[0].mTopDict.find(scROS) != mOpenTypeInput.mCFF.mTopDictIndex[0].mTopDict.end();

		mFontFileStream = pOutputStream;
		mPrimitivesWriter = new CPrimitiveWriter(pOutputStream);

		status = WriteCFFHeader();
		if (!status)
			return false;

		status = WriteName(inSubsetFontName);
		if (!status)
			return false;

		status = WriteTopIndex();
		if (!status)
			return false;

		status = WriteStringIndex();
		if (!status)
			return false;

		status = WriteGlobalSubrsIndex();
		if (!status)
			return false;

		status = WriteEncodings(subsetGlyphIDs);
		if (!status)
			return false;

		std::vector<unsigned short>* inCIDMapping = NULL;
		status = WriteCharsets(subsetGlyphIDs, inCIDMapping);
		if (!status)
			return false;

		FontDictInfoToByteMap newFDIndexes;
		if (mIsCID)
		{
			DetermineFDArrayIndexes(subsetGlyphIDs, newFDIndexes);
			status = WriteFDSelect(subsetGlyphIDs, newFDIndexes);
			if (!status)
				return false;
		}

		status = WriteCharStrings(subsetGlyphIDs);
		if (!status)
			return false;

		status = WritePrivateDictionary();
		if (!status)
			return false;

		if (mIsCID)
		{
			status = WriteFDArray(subsetGlyphIDs, newFDIndexes);
			if (!status)
				return false;
		}

		status = UpdateIndexesAtTopDict();
		if (!status)
			return false;

		return true;
	}
	bool CCFFWriter::WriteCFFHeader()
	{
		// i'm just gonna copy the header of the original CFF
		// content.
		// One thing i never got - OffSize does not seem to be important.
		// all offeet references to (0) are dictionary items (like in Top Dict),
		// and reading them follows the Integer operand rules. so why signify their size.
		// it's probably even not true, cause i guess font writers write integers using the most
		// compressed method, so if the number is small they'll use less bytes, and if large more.
		// so i don't get it. hope it won't screw up my implementation. in any case, for the sake of a single pass.
		// i'll probably just set it to something.

		mPrimitivesWriter->Write(mFile + mOpenTypeInput.mCFF.mCFFOffset, mOpenTypeInput.mCFF.mHeader.hdrSize);
		return true;
	}
	bool CCFFWriter::WriteName(const std::string& inSubsetFontName)
	{
		// get the first name from the name table, and write it here

		std::string fontName = inSubsetFontName.size() == 0 ? mOpenTypeInput.mCFF.mName.front() : inSubsetFontName;

		BYTE sizeOfOffset = GetMostCompressedOffsetSize((unsigned long)fontName.size() + 1);

		mPrimitivesWriter->WriteCard16(1);
		mPrimitivesWriter->WriteOffSize(sizeOfOffset);
		mPrimitivesWriter->SetOffSize(sizeOfOffset);
		mPrimitivesWriter->WriteOffset(1);
		mPrimitivesWriter->WriteOffset((unsigned long)fontName.size() + 1);
		mPrimitivesWriter->Write((const BYTE*)fontName.c_str(), fontName.size());

		return true;
	}
	BYTE CCFFWriter::GetMostCompressedOffsetSize(unsigned long inOffset)
	{
		if (inOffset < 256)
			return 1;
		if (inOffset < 65536)
			return 2;
		if (inOffset < 1 << 24)
			return 3;
		return 4;
	}
	bool CCFFWriter::WriteTopIndex()
	{
		/*
			what do i have to do:
			- write the top dictionary to a separate segment
				- make sure to write the ROS variable first, if one exists.
				- make sure to avoid writing any of the offset variables.
				- leave placeholders for any of the offset variables. make them maximum size. note
					to leave items for fdarray and fdselect only if required being a CID
				- be aware of the placeholders locations relative to the beginning of the segment
			- calculate the size of the segment
			- write the appropriate index header
			- write the segment
			- adjust the placeholders offset relative to the beginning of the file.
		*/

		bool status = true;
		CMemoryStream* topDictSegment = new CMemoryStream();

		status = WriteTopDictSegment(topDictSegment);
		if (!status)
		{
			RELEASEOBJECT(topDictSegment);
			return status;
		}

		// write index section
		BYTE sizeOfOffset = GetMostCompressedOffsetSize((unsigned long)topDictSegment->Tell() + 1);

		mPrimitivesWriter->WriteCard16(1);
		mPrimitivesWriter->WriteOffSize(sizeOfOffset);
		mPrimitivesWriter->SetOffSize(sizeOfOffset);
		mPrimitivesWriter->WriteOffset(1);
		mPrimitivesWriter->WriteOffset((unsigned long)topDictSegment->Tell() + 1);

		topDictSegment->Seek(0, SeekSet);

		int topDictDataOffset = mFontFileStream->Tell();

		// Write data
		mFontFileStream->WriteStream(topDictSegment, 0, NULL);

		// Adjust position locators for important placeholders
		mCharsetPlaceHolderPosition     += topDictDataOffset;
		mEncodingPlaceHolderPosition    += topDictDataOffset;
		mCharstringsPlaceHolderPosition += topDictDataOffset;
		mPrivatePlaceHolderPosition     += topDictDataOffset;
		mFDArrayPlaceHolderPosition     += topDictDataOffset;
		mFDSelectPlaceHolderPosition    += topDictDataOffset;

		return status;
	}
	bool CCFFWriter::WriteTopDictSegment(CMemoryStream* ioTopDictSegment)
	{
		CPrimitiveWriter dictPrimitiveWriter(ioTopDictSegment);
		UShortToDictOperandListMap::iterator itROS;
		UShortToDictOperandListMap::iterator it;

		UShortToDictOperandListMap& originalTopDictRef = mOpenTypeInput.mCFF.mTopDictIndex[0].mTopDict;

		itROS = originalTopDictRef.find(scROS);

		// make sure to write ROS first, if one exists
		if (mIsCID)
			dictPrimitiveWriter.WriteDictItems(itROS->first, itROS->second);

		// write all keys, excluding those that we want to write on our own
		for (it = originalTopDictRef.begin(); it != originalTopDictRef.end(); ++it)
		{
			if (it->first != scROS &&
				it->first != scCharset &&
				it->first != scEncoding &&
				it->first != scCharStrings &&
				it->first != scPrivate &&
				it->first != scFDArray &&
				it->first != scFDSelect)
				dictPrimitiveWriter.WriteDictItems(it->first, it->second);
		}
		// check if it had an embedded postscript (which would normally be the FSType implementation).
		// if not...create one to implement the FSType
		if (originalTopDictRef.find(scEmbeddedPostscript) == originalTopDictRef.end() && mOpenTypeInput.mOS2Exists)
		{
			// no need for sophistication here...you can consider this as the only string to be added.
			// so can be sure that its index would be the current count
			mOptionalEmbeddedPostscript = "/FSType " + std::to_string(mOpenTypeInput.mOS2.fsType) + " def";
			dictPrimitiveWriter.WriteIntegerOperand(mOpenTypeInput.mCFF.mStringsCount + N_STD_STRINGS);
			dictPrimitiveWriter.WriteDictOperator(scEmbeddedPostscript);
		}
		else
			mOptionalEmbeddedPostscript = "";

		// now leave placeholders, record their positions
		mCharsetPlaceHolderPosition = ioTopDictSegment->Tell();
		dictPrimitiveWriter.Pad5Bytes();
		dictPrimitiveWriter.WriteDictOperator(scCharset);
		mCharstringsPlaceHolderPosition = ioTopDictSegment->Tell();
		dictPrimitiveWriter.Pad5Bytes();
		dictPrimitiveWriter.WriteDictOperator(scCharStrings);
		if (mOpenTypeInput.mCFF.mPrivateDicts[0].mPrivateDictStart != 0)
		{
			mPrivatePlaceHolderPosition = ioTopDictSegment->Tell();
			dictPrimitiveWriter.Pad5Bytes(); // for private it's two places - size and position
			dictPrimitiveWriter.Pad5Bytes();
			dictPrimitiveWriter.WriteDictOperator(scPrivate);
		}
		else
		{
			mPrivatePlaceHolderPosition = 0;
		}
		if (mIsCID)
		{
			mEncodingPlaceHolderPosition = 0;
			mFDArrayPlaceHolderPosition = ioTopDictSegment->Tell();
			dictPrimitiveWriter.Pad5Bytes();
			dictPrimitiveWriter.WriteDictOperator(scFDArray);
			mFDSelectPlaceHolderPosition = ioTopDictSegment->Tell();
			dictPrimitiveWriter.Pad5Bytes();
			dictPrimitiveWriter.WriteDictOperator(scFDSelect);
		}
		else
		{
			mEncodingPlaceHolderPosition = ioTopDictSegment->Tell();
			dictPrimitiveWriter.Pad5Bytes();
			dictPrimitiveWriter.WriteDictOperator(scEncoding);
			mFDArrayPlaceHolderPosition = 0;
			mFDSelectPlaceHolderPosition = 0;
		}
		return true;
	}
	bool CCFFWriter::WriteStringIndex()
	{
		// if added a new string...needs to work hard, otherwise just copy the strings.
		if (mOptionalEmbeddedPostscript.size() == 0)
		{
			// copy as is from the original file. note that the global subroutines
			// starting position is equal to the strings end position. hence length is...

			mFontFileStream->Write(mFile + mOpenTypeInput.mCFF.mCFFOffset + mOpenTypeInput.mCFF.mStringIndexPosition,
									 mOpenTypeInput.mCFF.mGlobalSubrsPosition - mOpenTypeInput.mCFF.mStringIndexPosition);
		}
		else
		{
			// need to write the bloody strings...[remember that i'm adding one more string at the end]
			mPrimitivesWriter->WriteCard16(mOpenTypeInput.mCFF.mStringsCount + 1);

			// calculate the total data size to determine the required offset size
			unsigned long totalSize = 0;
			for (int i = 0; i < mOpenTypeInput.mCFF.mStringsCount; ++i)
				totalSize += (unsigned long)strlen(mOpenTypeInput.mCFF.mStrings[i]);
			totalSize += (unsigned long)mOptionalEmbeddedPostscript.size();

			BYTE sizeOfOffset = GetMostCompressedOffsetSize(totalSize + 1);
			mPrimitivesWriter->WriteOffSize(sizeOfOffset);
			mPrimitivesWriter->SetOffSize(sizeOfOffset);

			unsigned long currentOffset = 1;

			// write the offsets
			for (int i = 0; i < mOpenTypeInput.mCFF.mStringsCount; ++i)
			{
				mPrimitivesWriter->WriteOffset(currentOffset);
				currentOffset += (unsigned long)strlen(mOpenTypeInput.mCFF.mStrings[i]);
			}
			mPrimitivesWriter->WriteOffset(currentOffset);
			currentOffset += (unsigned long)mOptionalEmbeddedPostscript.size();
			mPrimitivesWriter->WriteOffset(currentOffset);

			// write the data
			for (int i = 0; i < mOpenTypeInput.mCFF.mStringsCount; ++i)
			{
				mFontFileStream->Write((const BYTE*)(mOpenTypeInput.mCFF.mStrings[i]), strlen(mOpenTypeInput.mCFF.mStrings[i]));
			}
			mFontFileStream->Write((const BYTE*)(mOptionalEmbeddedPostscript.c_str()), mOptionalEmbeddedPostscript.size());
		}
		return true;
	}
	bool CCFFWriter::WriteGlobalSubrsIndex()
	{
		// global subrs index is empty!. no subrs in my CFF outputs. all charstrings are flattened

		return mPrimitivesWriter->WriteCard16(0);
	}
	bool CCFFWriter::WriteEncodings(const std::vector<unsigned int>& inSubsetGlyphIDs)
	{
		// if it's a CID. don't bother with encodings (marks as 0)
		if (mIsCID)
		{
			mEncodingPosition = 0;
			return true;
		}

		// not CID, write encoding, according to encoding values from the original font
		EncodingsInfo* encodingInfo = mOpenTypeInput.mCFF.mTopDictIndex[0].mEncoding;
		if (encodingInfo->mEncodingStart <= 1)
		{
			mEncodingPosition = encodingInfo->mEncodingStart;
			return true;
		}
		else
		{
			// original font had custom encoding, let's subset it according to just the glyphs we
			// actually have. but cause i'm lazy i'll just do the first format.

			// figure out if we got supplements
			std::vector<unsigned int>::const_iterator it = inSubsetGlyphIDs.begin();
			ByteAndUShortList supplements;

			for (; it != inSubsetGlyphIDs.end();++it)
			{
				// don't be confused! the supplements is by SID! not GID!
				unsigned short sid = mOpenTypeInput.mCFF.GetGlyphSID(0, *it);

				UShortToByteList::iterator itSupplements = encodingInfo->mSupplements.find(sid);
				if (itSupplements != encodingInfo->mSupplements.end())
				{
					ByteList::iterator itMoreEncoding = itSupplements->second.begin();
					for (; itMoreEncoding != itSupplements->second.end(); ++itMoreEncoding)
						supplements.push_back(ByteAndUShort(*itMoreEncoding, sid));
				}
			}

			mEncodingPosition = mFontFileStream->Tell();

			if (supplements.size() > 0)
				mPrimitivesWriter->WriteCard8(0x80);
			else
				mPrimitivesWriter->WriteCard8(0);

			// assuming that 0 is in the subset glyphs IDs, which does not require encoding
			// get the encodings count
			BYTE encodingGlyphsCount = std::min((BYTE)(inSubsetGlyphIDs.size() - 1), encodingInfo->mEncodingsCount);

			mPrimitivesWriter->WriteCard8(encodingGlyphsCount);
			for (BYTE i = 0; i < encodingGlyphsCount; ++i)
			{
				if (inSubsetGlyphIDs[i + 1] < encodingInfo->mEncodingsCount)
					mPrimitivesWriter->WriteCard8(encodingInfo->mEncoding[inSubsetGlyphIDs[i + 1] - 1]);
				else
					mPrimitivesWriter->WriteCard8(0);
			}

			if (supplements.size() > 0)
			{
				mPrimitivesWriter->WriteCard8(BYTE(supplements.size()));
				ByteAndUShortList::iterator itCollectedSupplements = supplements.begin();

				for (; itCollectedSupplements != supplements.end(); ++itCollectedSupplements)
				{
					mPrimitivesWriter->WriteCard8(itCollectedSupplements->first);
					mPrimitivesWriter->WriteCard16(itCollectedSupplements->second);
				}
			}
		}

		return true;
	}
	bool CCFFWriter::WriteCharsets(const std::vector<unsigned int>& inSubsetGlyphIDs, std::vector<unsigned short>* inCIDMapping)
	{
		// since this is a subset the chances that i'll get a defult charset are 0.
		// hence i'll always do some charset. and using format 0 !!1
		std::vector<unsigned int>::const_iterator it = inSubsetGlyphIDs.begin();
		++it; // skip the 0

		mCharsetPosition = mFontFileStream->Tell();

		mPrimitivesWriter->WriteCard8(0);
		if (mIsCID)
		{
			int i = 1;
			for (; it != inSubsetGlyphIDs.end(); ++it, ++i)
				mPrimitivesWriter->WriteSID(inCIDMapping ? (*inCIDMapping)[i] : i);

		}
		else
		{
			// note that this also works for CIDs! cause in this case the SIDs are actually
			// CIDs
			for (; it != inSubsetGlyphIDs.end(); ++it)
				mPrimitivesWriter->WriteSID(mOpenTypeInput.mCFF.GetGlyphSID(0, *it));
		}
		return true;
	}
	void CCFFWriter::DetermineFDArrayIndexes(const std::vector<unsigned int>& inSubsetGlyphIDs, FontDictInfoToByteMap& outNewFontDictsIndexes)
	{
		std::vector<unsigned int>::const_iterator itGlyphs = inSubsetGlyphIDs.begin();
		FontDictInfoSet fontDictInfos;

		for (; itGlyphs != inSubsetGlyphIDs.end(); ++itGlyphs)
			if (mOpenTypeInput.mCFF.mTopDictIndex[0].mFDSelect[*itGlyphs])
				fontDictInfos.insert(mOpenTypeInput.mCFF.mTopDictIndex[0].mFDSelect[*itGlyphs]);

		FontDictInfoSet::iterator itFontInfos;
		BYTE i = 0;

		for (itFontInfos = fontDictInfos.begin(); itFontInfos != fontDictInfos.end(); ++itFontInfos, ++i)
			outNewFontDictsIndexes.insert(FontDictInfoToByteMap::value_type(*itFontInfos, i));
	}
	bool CCFFWriter::WriteFDSelect(const std::vector<unsigned int>& inSubsetGlyphIDs, const FontDictInfoToByteMap& inNewFontDictsIndexes)
	{
		// always write format 3. cause at most cases the FD dicts count will be so low that it'd
		// take a bloody mircale for no repeats to occur.
		std::vector<unsigned int>::const_iterator itGlyphs = inSubsetGlyphIDs.begin();

		mFDSelectPosition = mFontFileStream->Tell();
		mPrimitivesWriter->WriteCard8(3);

		long long rangesCountPosition = mFontFileStream->Tell();
		mPrimitivesWriter->WriteCard16(1); // temporary. will get back to this later

		unsigned short rangesCount = 1;
		BYTE currentFD, newFD;
		unsigned short glyphIndex = 1;
		FontDictInfoToByteMap::const_iterator itNewIndex = inNewFontDictsIndexes.find(mOpenTypeInput.mCFF.mTopDictIndex[0].mFDSelect[*itGlyphs]);

		// k. seems like i probably just imagine exceptions here. i guess there must
		// be a proper FDSelect with FDs for all...so i'm defaulting to some 0
		currentFD = (itNewIndex == inNewFontDictsIndexes.end() ? 0 : itNewIndex->second);
		mPrimitivesWriter->WriteCard16(0);
		mPrimitivesWriter->WriteCard8(currentFD);
		++itGlyphs;

		for (; itGlyphs != inSubsetGlyphIDs.end(); ++itGlyphs, ++glyphIndex)
		{
			itNewIndex = inNewFontDictsIndexes.find(mOpenTypeInput.mCFF.mTopDictIndex[0].mFDSelect[*itGlyphs]);
			newFD = (itNewIndex == inNewFontDictsIndexes.end() ? 0 : itNewIndex->second);
			if (newFD != currentFD)
			{
				currentFD = newFD;
				mPrimitivesWriter->WriteCard16(glyphIndex);
				mPrimitivesWriter->WriteCard8(currentFD);
				++rangesCount;
			}
		}
		mPrimitivesWriter->WriteCard16((unsigned short)inSubsetGlyphIDs.size());
		// go back to ranges count if not equal to what's already written
		if (rangesCount != 1)
		{
			long long currentPosition = mFontFileStream->Tell();
			mFontFileStream->Seek(rangesCountPosition, SeekSet);
			mPrimitivesWriter->WriteCard16(rangesCount);
			mFontFileStream->Seek(currentPosition, SeekSet);
		}
		return true;
	}
	bool CCFFWriter::WriteCharStrings(const std::vector<unsigned int>& inSubsetGlyphIDs)
	{
		/*
			1. build the charstrings data, looping the glyphs charstrings and writing a flattened
			   version of each charstring
			2. write the charstring index based on offsets inside the data (size should be according to the max)
			3. copy the data into the stream
		*/

		unsigned long* offsets = new unsigned long[inSubsetGlyphIDs.size() + 1];
		// CMemoryStream charStringsData;
		CMemoryStream charStringsDataWriteStream;
		CharStringType2Flattener charStringFlattener;
		std::vector<unsigned int>::const_iterator itGlyphs = inSubsetGlyphIDs.begin();
		bool status = true;

		unsigned short i = 0;
		for (; itGlyphs != inSubsetGlyphIDs.end() && status; ++itGlyphs, ++i)
		{
			offsets[i] = (unsigned long)charStringsDataWriteStream.Tell();
			status = charStringFlattener.WriteFlattenedGlyphProgram(0, *itGlyphs, &(mOpenTypeInput.mCFF), &charStringsDataWriteStream);
		}
		if (!status)
		{
			delete[] offsets;
			return false;
		}

		offsets[i] = (unsigned long)charStringsDataWriteStream.Tell();

		charStringsDataWriteStream.Seek(0, SeekSet);

		// write index section
		mCharStringPosition = mFontFileStream->Tell();
		BYTE sizeOfOffset = GetMostCompressedOffsetSize(offsets[i] + 1);
		mPrimitivesWriter->WriteCard16((unsigned short)inSubsetGlyphIDs.size());
		mPrimitivesWriter->WriteOffSize(sizeOfOffset);
		mPrimitivesWriter->SetOffSize(sizeOfOffset);
		for (i = 0; i <= inSubsetGlyphIDs.size(); ++i)
			mPrimitivesWriter->WriteOffset(offsets[i] + 1);

		// Write data
		mFontFileStream->WriteStream(&charStringsDataWriteStream, 0, NULL);

		delete[] offsets;
		return status;
	}
	bool CCFFWriter::WritePrivateDictionary()
	{
		WritePrivateDictionaryBody(mOpenTypeInput.mCFF.mPrivateDicts[0], mPrivateSize, mPrivatePosition);
		return true;
	}
	void CCFFWriter::WritePrivateDictionaryBody(const PrivateDictInfo& inPrivateDictionary, long long& outWriteSize, long long& outWritePosition)
	{
		// just copy the private dict, without the subrs reference
		if (inPrivateDictionary.mPrivateDictStart != 0)
		{
			UShortToDictOperandListMap::const_iterator it = inPrivateDictionary.mPrivateDict.begin();

			outWritePosition = mFontFileStream->Tell();
			for (; it != inPrivateDictionary.mPrivateDict.end(); ++it)
				if (it->first != scSubrs) // should get me a nice little pattern for this some time..a filter thing
					mPrimitivesWriter->WriteDictItems(it->first, it->second);

			outWriteSize = mFontFileStream->Tell() - outWritePosition;
			return;
		}
		outWritePosition = 0;
		outWriteSize = 0;
	}
	bool CCFFWriter::WriteFDArray(const std::vector<unsigned int>&, const FontDictInfoToByteMap& inNewFontDictsIndexes)
	{
		// loop the glyphs IDs, for each get their respective dictionary. put them in a set.
		// now itereate them, and write each private dictionary [no need for index]. save the private dictionary position.
		// now write the FDArray. remember it's an index, so first write into a separate, maintain the offsets and only then write the actual buffer.
		// save a mapping between the original pointer and a new index.

		FontDictInfoToLongFilePositionTypePairMap privateDictionaries;
		bool status = true;
		unsigned long* offsets = NULL;

		if (inNewFontDictsIndexes.size() == 0)
		{
			// if no valid font infos, write an empty index and finish
			mFDArrayPosition = mFontFileStream->Tell();
			mPrimitivesWriter->WriteCard16(0);
			return true;
		}

		// loop the font infos, and write the private dictionaries
		long long privatePosition, privateSize;
		FontDictInfoToByteMap::const_iterator itFontInfos = inNewFontDictsIndexes.begin();
		for (; itFontInfos != inNewFontDictsIndexes.end(); ++itFontInfos)
		{
			WritePrivateDictionaryBody(itFontInfos->first->mPrivateDict, privateSize, privatePosition);
			privateDictionaries.insert(
				FontDictInfoToLongFilePositionTypePairMap::value_type(itFontInfos->first, LongFilePositionTypePair(privateSize, privatePosition)));
		}

		// write FDArray segment
		offsets = new unsigned long[inNewFontDictsIndexes.size() + 1];
		CMemoryStream fontDictDataWriteStream;
		CPrimitiveWriter fontDictPrimitiveWriter(&fontDictDataWriteStream);
		BYTE i = 0;

		for (itFontInfos = inNewFontDictsIndexes.begin(); itFontInfos != inNewFontDictsIndexes.end() && status; ++itFontInfos, ++i)
		{
			offsets[i] = (unsigned long)fontDictDataWriteStream.Tell();

			UShortToDictOperandListMap::const_iterator itDict = itFontInfos->first->mFontDict.begin();

			for (; itDict != itFontInfos->first->mFontDict.end() && status; ++itDict)
				if (itDict->first != scPrivate) // should get me a nice little pattern for this some time..a filter thing
					status = fontDictPrimitiveWriter.WriteDictItems(itDict->first, itDict->second);

			// now add the private key
			if (status && privateDictionaries[itFontInfos->first].first != 0)
			{
				fontDictPrimitiveWriter.WriteIntegerOperand(long(privateDictionaries[itFontInfos->first].first));
				fontDictPrimitiveWriter.WriteIntegerOperand(long(privateDictionaries[itFontInfos->first].second));
				fontDictPrimitiveWriter.WriteDictOperator(scPrivate);
			}
		}
		if (!status)
		{
			delete[] offsets;
			return false;
		}

		offsets[i] = (unsigned long)fontDictDataWriteStream.Tell();

		fontDictDataWriteStream.Seek(0, SeekSet);

		// write index section
		mFDArrayPosition = mFontFileStream->Tell();
		BYTE sizeOfOffset = GetMostCompressedOffsetSize(offsets[i] + 1);
		mPrimitivesWriter->WriteCard16((unsigned short)inNewFontDictsIndexes.size());
		mPrimitivesWriter->WriteOffSize(sizeOfOffset);
		mPrimitivesWriter->SetOffSize(sizeOfOffset);
		for (i = 0; i <= inNewFontDictsIndexes.size(); ++i)
			mPrimitivesWriter->WriteOffset(offsets[i] + 1);

		// Write data
		mFontFileStream->WriteStream(&fontDictDataWriteStream, 0, NULL);

		delete[] offsets;
		return status;
	}
	bool CCFFWriter::UpdateIndexesAtTopDict()
	{
		mFontFileStream->Seek(mCharsetPlaceHolderPosition, SeekSet);
		mPrimitivesWriter->Write5ByteDictInteger((long)mCharsetPosition);

		mFontFileStream->Seek(mCharstringsPlaceHolderPosition, SeekSet);
		mPrimitivesWriter->Write5ByteDictInteger((long)mCharStringPosition);

		if (mOpenTypeInput.mCFF.mPrivateDicts[0].mPrivateDictStart != 0)
		{
			mFontFileStream->Seek(mPrivatePlaceHolderPosition, SeekSet);
			mPrimitivesWriter->Write5ByteDictInteger((long)mPrivateSize);
			mPrimitivesWriter->Write5ByteDictInteger((long)mPrivatePosition);

		}

		if (mIsCID)
		{
			mFontFileStream->Seek(mFDArrayPlaceHolderPosition, SeekSet);
			mPrimitivesWriter->Write5ByteDictInteger((long)mFDArrayPosition);
			mFontFileStream->Seek(mFDSelectPlaceHolderPosition, SeekSet);
			mPrimitivesWriter->Write5ByteDictInteger((long)mFDSelectPosition);
		}
		else
		{
			mFontFileStream->Seek(mEncodingPlaceHolderPosition, SeekSet);
			mPrimitivesWriter->Write5ByteDictInteger((long)mEncodingPosition);
		}

		return true;
	}
	//----------------------------------------------------------------------------------------
	// CFontFileTrueType
	//----------------------------------------------------------------------------------------
	void CFontFileTrueType::WriteCIDFontType0C(CStream* pOutputStream, unsigned short* pCodeToGID, unsigned int unCodesCount)
	{
		if (!m_bOpenTypeCFF)
		{
			// Если шрифт не является OpenType CFF, завершаем
			return;
		}

		CCFFWriter pWriter;
		pWriter.CreateCFFSubset(m_sFile, m_nLen, m_unFontIndex, m_sName, pOutputStream, pCodeToGID, unCodesCount);
	}
}
