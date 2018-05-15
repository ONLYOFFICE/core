/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef BINARY_COMMON_READER
#define BINARY_COMMON_READER

#include "../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"
#include "../../XlsxSerializerCom/Common/BinReaderWriterDefines.h"

namespace BinXlsxRW {

	#define READ_TABLE_DEF(res, fReadFunction, arg) {\
		res = m_oBufferedStream.Peek(4) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
		if (c_oSerConstants::ReadOk == res) {\
			long stLen = m_oBufferedStream.GetLong();\
			res = m_oBufferedStream.Peek(stLen) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
			if (c_oSerConstants::ReadOk == res) {\
				READ1_DEF(stLen, res, fReadFunction, arg);\
			}\
		}\
	}
	#define READ1_DEF(stLen, res, fReadFunction, arg) {\
		long stCurPos = 0;\
		long start_pos = m_oBufferedStream.GetPos();\
		while(stCurPos < stLen)\
		{\
			BYTE type = m_oBufferedStream.GetUChar();\
			ULONG length = m_oBufferedStream.GetULong();\
			if (length + stCurPos > stLen)\
			{\
				m_oBufferedStream.Seek(start_pos + stLen);\
				res = c_oSerConstants::ReadOk;\
				break;\
			}\
			res = fReadFunction(type, length, arg);\
			if(res == c_oSerConstants::ReadUnknown)\
			{\
				m_oBufferedStream.GetPointer(length);\
				res = c_oSerConstants::ReadOk;\
			}\
			else if(res != c_oSerConstants::ReadOk)\
				break;\
			stCurPos += length + 5;\
		}\
	}

	#define READ2_DEF(stLen, res, fReadFunction, arg) {\
		long stCurPos = 0;\
		while(stCurPos < stLen)\
		{\
			BYTE type = m_oBufferedStream.GetUChar();\
			long lenType =  m_oBufferedStream.GetUChar();\
			int nCurPosShift = 2;\
			int nRealLen;\
			switch(lenType)\
			{\
			case c_oSerPropLenType::Null: nRealLen = 0;break;\
			case c_oSerPropLenType::Byte: nRealLen = 1;break;\
			case c_oSerPropLenType::Short: nRealLen = 2;break;\
			case c_oSerPropLenType::Three: nRealLen = 3;break;\
			case c_oSerPropLenType::Long: nRealLen = 4;break;\
			case c_oSerPropLenType::Double: nRealLen = 8;break;\
			case c_oSerPropLenType::Variable:\
				nRealLen = m_oBufferedStream.GetLong();\
				nCurPosShift += 4;\
				break;\
			default:res = c_oSerConstants::ErrorUnknown;break;\
			}\
			if(res == c_oSerConstants::ReadOk)\
				res = fReadFunction(type, nRealLen, arg);\
			if(res == c_oSerConstants::ReadUnknown)\
			{\
				m_oBufferedStream.GetPointer(nRealLen);\
				res = c_oSerConstants::ReadOk;\
			}\
			else if(res != c_oSerConstants::ReadOk)\
				break;\
			stCurPos += nRealLen + nCurPosShift;\
		}\
	}

	class Binary_CommonReader
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
	public:
		Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_oBufferedStream(poBufferedStream)
		{
		}
	};
}
#endif	// #ifndef BINARY_COMMON_READER
