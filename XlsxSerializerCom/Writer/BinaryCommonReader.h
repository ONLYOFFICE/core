#ifndef BINARY_COMMON_READER
#define BINARY_COMMON_READER

#include "../../ASCOfficePPTXFile/Editor/BinaryFileReaderWriter.h"
#include "../../XlsxSerializerCom/Common/BinReaderWriterDefines.h"

namespace BinXlsxRW {
	template <typename CallbackType >
	class Binary_CommonReader
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_oBufferedStream;
		typedef int (CallbackType::*funcArg)(BYTE type, long length, void* arg);
	public:
		Binary_CommonReader(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_oBufferedStream(poBufferedStream)
		{
		}
		int ReadTable(funcArg fReadFunction, void* poFuncObj, void* arg = NULL)
		{
			int res = c_oSerConstants::ReadOk;
			//stLen
			res = m_oBufferedStream.Peek(4) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if(c_oSerConstants::ReadOk != res)
				return res;
			long stLen = m_oBufferedStream.GetLong();
			//Смотрим есть ли данные под всю таблицу в дальнейшем спокойно пользуемся get функциями
			res = m_oBufferedStream.Peek(stLen) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
			if(c_oSerConstants::ReadOk != res)
				return res;
			return Read1(stLen, fReadFunction, poFuncObj, arg);
		}
		int Read1(long stLen, funcArg fReadFunction, void* poFuncObj, void* arg = NULL)
		{
			int res = c_oSerConstants::ReadOk;
			long stCurPos = 0;

			long start_pos = m_oBufferedStream.GetPos();
			while(stCurPos < stLen)
			{
				//stItem
				BYTE type = m_oBufferedStream.GetUChar();

                ULONG length = m_oBufferedStream.GetULong();

				if (length + stCurPos > stLen)
				{
					//ОШИБКА ЧТЕНИЯ БИНАРНИКА - пропускаем элемент
					m_oBufferedStream.Seek(start_pos + stLen);
					return c_oSerConstants::ReadOk;
				} 
				res = (((CallbackType*)poFuncObj)->*fReadFunction)(type, length, arg);
                if(res == c_oSerConstants::ReadUnknown && length > 0)
				{
					m_oBufferedStream.GetPointer(length);
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
				BYTE type = m_oBufferedStream.GetUChar();
				long lenType =  m_oBufferedStream.GetUChar();
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
					nRealLen = m_oBufferedStream.GetLong();
					nCurPosShift += 4;
					break;
				default:return c_oSerConstants::ErrorUnknown;
				}
				res = (((CallbackType*)poFuncObj)->*fReadFunction)(type, nRealLen, arg);
				if(res == c_oSerConstants::ReadUnknown)
				{
					m_oBufferedStream.GetPointer(nRealLen);
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
#endif	// #ifndef BINARY_COMMON_READER
