#pragma once
#include "../../ASCOfficeDocxFile2/BinWriter/StreamUtils.h"

namespace BinXlsxRW {
	template <typename CallbackType >
	class Binary_CommonReader
	{
	protected:
		Streams::CBufferedStream& m_oBufferedStream;
		typedef int (CallbackType::*funcArg)(BYTE type, long length, void* arg);
	public:
		Binary_CommonReader(Streams::CBufferedStream& poBufferedStream):m_oBufferedStream(poBufferedStream)
		{
		}
		int ReadTable(funcArg fReadFunction, void* poFuncObj, void* arg = NULL)
		{
			int res = c_oSerConstants::ReadOk;
			//stLen
			res = m_oBufferedStream.Peek(4) == FALSE ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if(c_oSerConstants::ReadOk != res)
				return res;
			long stLen = m_oBufferedStream.ReadLong();
			//Смотрим есть ли данные под всю таблицу в дальнейшем спокойно пользуемся get функциями
			res = m_oBufferedStream.Peek(stLen) == FALSE ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if(c_oSerConstants::ReadOk != res)
				return res;
			return Read1(stLen, fReadFunction, poFuncObj, arg);
		}
		int Read1(long stLen, funcArg fReadFunction, void* poFuncObj, void* arg = NULL)
		{
			int res = c_oSerConstants::ReadOk;
			long stCurPos = 0;
			while(stCurPos < stLen)
			{
				//stItem
				BYTE type = m_oBufferedStream.ReadByte();
				long length =  m_oBufferedStream.ReadLong();
				res = (((CallbackType*)poFuncObj)->*fReadFunction)(type, length, arg);
				if(res == c_oSerConstants::ReadUnknown)
				{
					m_oBufferedStream.ReadPointer(length);
					res = c_oSerConstants::ReadOk;
				}
				else if(res != c_oSerConstants::ReadOk)
					return res;
				stCurPos += length + 5;
			}
			return res;
		}
		int Read2(long stLen, funcArg fReadFunction, void* poFuncObj, void* arg = NULL)
		{
			int res = c_oSerConstants::ReadOk;
			long stCurPos = 0;
			while(stCurPos < stLen)
			{
				//stItem
				BYTE type = m_oBufferedStream.ReadByte();
				long lenType =  m_oBufferedStream.ReadByte();
				int nCurPosShift = 2;
				int nRealLen;
				switch(lenType)
				{
				case c_oSerPropLenType::Null: nRealLen = 0;break;
				case c_oSerPropLenType::Byte: nRealLen = 1;break;
				case c_oSerPropLenType::Short: nRealLen = 2;break;
				case c_oSerPropLenType::Three: nRealLen = 3;break;
				case c_oSerPropLenType::Long: nRealLen = 4;break;
				case c_oSerPropLenType::Double: nRealLen = 8;break;
				case c_oSerPropLenType::Variable:
					nRealLen = m_oBufferedStream.ReadLong();
					nCurPosShift += 4;
					break;
				default:return c_oSerConstants::ErrorUnknown;
				}
				res = (((CallbackType*)poFuncObj)->*fReadFunction)(type, nRealLen, arg);
				if(res == c_oSerConstants::ReadUnknown)
				{
					m_oBufferedStream.ReadPointer(nRealLen);
					res = c_oSerConstants::ReadOk;
				}
				else if(res != c_oSerConstants::ReadOk)
					return res;
				stCurPos += nRealLen + nCurPosShift;
			}
			return res;
		}
	};
}