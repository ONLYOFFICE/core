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
#ifndef BINARY_COMMON_READER
#define BINARY_COMMON_READER

#include "../../../Binary/Presentation/BinaryFileReaderWriter.h"
#include "../../Sheets/Common/BinReaderWriterDefines.h"

namespace BinXlsxRW {

#define READ_TABLE_DEF(res, fReadFunction, arg) {\
		res = m_oBufferedStream.Peek(4) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
		if (c_oSerConstants::ReadOk == res) {\
			long readtabledefLen = m_oBufferedStream.GetLong();\
			res = m_oBufferedStream.Peek(readtabledefLen) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;\
			if (c_oSerConstants::ReadOk == res) {\
				READ1_DEF(readtabledefLen, res, fReadFunction, arg);\
			}\
		}\
	}

#define READ1_DEF(stLen, res, fReadFunction, arg) {\
	long read1defCurPos = 0;\
	long read1defstart_pos = m_oBufferedStream.GetPos();\
	while(read1defCurPos < (long)stLen)\
	{\
		BYTE read1defType = 0;\
		if (false == m_oBufferedStream.GetUCharWithResult(&read1defType))\
			break;\
		ULONG read1defLength = m_oBufferedStream.GetULong();\
		if (read1defLength + read1defCurPos > (ULONG)stLen)\
		{\
			m_oBufferedStream.Seek(read1defstart_pos + stLen);\
			res = c_oSerConstants::ReadOk;\
			break;\
		}\
		res = fReadFunction(read1defType, read1defLength, arg);\
		if(res == c_oSerConstants::ReadUnknown)\
		{\
			m_oBufferedStream.GetPointer(read1defLength);\
			res = c_oSerConstants::ReadOk;\
		}\
		else if(res != c_oSerConstants::ReadOk)\
			break;\
		read1defCurPos += read1defLength + 5;\
	}\
}

#define READ2_DEF_SPREADSHEET(stLen, res, fReadFunction, arg) {\
	long read2defCurPos = 0;\
	while(read2defCurPos < stLen)\
	{\
		BYTE read2defType = 0;\
		if (false == m_oBufferedStream.GetUCharWithResult(&read2defType))\
			break;\
		long read2defLenType =  m_oBufferedStream.GetUChar();\
		int read2defCurPosShift = 2;\
		int read2defRealLen;\
		switch(read2defLenType)\
		{\
		case c_oSerPropLenType::Null: read2defRealLen = 0;break;\
		case c_oSerPropLenType::Byte: read2defRealLen = 1;break;\
		case c_oSerPropLenType::Short: read2defRealLen = 2;break;\
		case c_oSerPropLenType::Three: read2defRealLen = 3;break;\
		case c_oSerPropLenType::Long: read2defRealLen = 4;break;\
		case c_oSerPropLenType::Double: read2defRealLen = 8;break;\
		case c_oSerPropLenType::Variable:\
			read2defRealLen = m_oBufferedStream.GetLong();\
			read2defCurPosShift += 4;\
			break;\
		default:res = c_oSerConstants::ErrorUnknown;break;\
		}\
		if(res == c_oSerConstants::ReadOk)\
			res = fReadFunction(read2defType, read2defRealLen, arg);\
		if(res == c_oSerConstants::ReadUnknown)\
		{\
			m_oBufferedStream.GetPointer(read2defRealLen);\
			res = c_oSerConstants::ReadOk;\
		}\
		else if(res != c_oSerConstants::ReadOk)\
			break;\
		read2defCurPos += read2defRealLen + read2defCurPosShift;\
	}\
}

	class Binary_CommonReader
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
	public:
		Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream) : m_oBufferedStream(poBufferedStream)
		{
		}
	};
}
#endif	// #ifndef BINARY_COMMON_READER
