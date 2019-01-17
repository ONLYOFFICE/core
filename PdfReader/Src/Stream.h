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
#ifndef _PDF_READER_STREAM_H
#define _PDF_READER_STREAM_H

#include <stdio.h>
#include "Object.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

namespace PdfReader
{
	class BaseStream;

	//------------------------------------------------------------------------

	enum StreamType
	{
		strFile,
		strASCIIHex,
		strASCII85,
		strLZW,
		strRunLength,
		strCCITTFax,
		strDCT,
		strFlate,
		strJBIG2,
		strJPX,
		strWeird			// internal-use stream types
	};

	enum StreamColorSpaceMode
	{
		streamCSNone,
		streamCSDeviceGray,
		streamCSDeviceRGB,
		streamCSDeviceCMYK
	};

	//------------------------------------------------------------------------

	enum CryptAlgorithm
	{
		cryptRC4,
		cryptAES128,
		cryptAES256
	};

	//------------------------------------------------------------------------
	// Stream (основной класс)
	//------------------------------------------------------------------------

	class Stream
	{
	public:

		Stream();

		virtual ~Stream();

		// Счетчик ссылок.
		int AddRef()
		{
			return ++m_nRef;
		}
		int Release()
		{
			return --m_nRef;
		}

		// Тип потока.
		virtual StreamType GetType() = 0;

		// Сбрасываем все параметры(к начальным).
		virtual void Reset() = 0;

		// Закрываем поток.
		virtual void Close();

		// Передвигаемся к следующему символу в потоке.
		virtual int GetChar() = 0;

		// Смотрим на следующий символ в потоке.
		virtual int LookChar() = 0;

		// GetChar буз использования Predictor.
		// Используется только в StreamPredictor.
		virtual int GetRawChar();

		// Следующая строка в потоке.
		virtual char *GetLine(char *sBuffer, int nSize);

		// Текущая позиция в потоке.
		virtual int GetPos() = 0;

		// Устанавливаем текущую позицию в потоке. Если nDirection - 
		// отрицательно, тогда позицию отсчитваем от конца потока, а 
		// если положительно, тогда от начала.
		virtual void SetPos(unsigned int unPos, int nDirection = 0) = 0;

		// PostScript'ое название фильтра.
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);

		// Может ли поток содержать непечатыемые символы?
		virtual bool IsBinary(bool bLast = true) = 0;

		virtual BaseStream *GetBaseStream() = 0;

		// Поток после последнего декодирования (это может быть BaseStream 
		// или DecryptStream).
		virtual Stream *GetUndecodedStream() = 0;

		// Словарь связанный с данным потоком.
		virtual Dict *GetDict() = 0;

		// Encoding filter?
		virtual bool IsEncoder()
		{
			return false;
		}

		// Получить параметры изображения, определенного содержимым данного потока.
		virtual void GetImageParams(int *pnBitsPerComponent, StreamColorSpaceMode *peCSMode)
		{
		}

		// Возвращется следующий поток из стека.
		virtual Stream *GetNextStream()
		{
			return NULL;
		}

		// Применяем фильтры к данному потоку, описанные в параметрах словаря.
		// Возвращается новый поток.
		Stream *AddFilters(Object *pDict);

	private:

		Stream *ApplyFilter(char *sName, Stream *pStream, Object *pParams);

	private:

		int m_nRef; // счетчик ссылок

	};

	//------------------------------------------------------------------------
	// BaseStream
	//
	// Это основной класс для всех потоков и чтения из файла.
	//------------------------------------------------------------------------

	class BaseStream : public Stream
	{
	public:

		BaseStream(Object *pDict);
		virtual ~BaseStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict) = 0;
		virtual void SetPos(unsigned int unPos, int nDirection = 0) = 0;
		virtual bool IsBinary(bool bLast = true)
		{
			return bLast;
		}
		virtual BaseStream *GetBaseStream()
		{
			return this;
		}
		virtual Stream *GetUndecodedStream()
		{
			return this;
		}
		virtual Dict *GetDict()
		{
			return m_pDict.GetDict();
		}
		virtual StringExt *GetFileName()
		{
			return NULL;
		}

		// Запрашиваем/устанавливаем позицию первого байта в потоке файла.
		virtual unsigned int GetStartPos() = 0;
		virtual void         SetStartPos(int nDelta) = 0;

	private:

		Object m_pDict;
	};

	//------------------------------------------------------------------------
	// FilterStream
	//
	// Это основной класс для всех потоков, к которым примене фильтр.
	//------------------------------------------------------------------------

	class FilterStream : public Stream
	{
	public:

		FilterStream(Stream *pStream);
		virtual ~FilterStream();
		virtual void Close();
		virtual int GetPos()
		{
			return m_pStream->GetPos();
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual BaseStream *GetBaseStream()
		{
			return m_pStream->GetBaseStream();
		}
		virtual Stream *GetUndecodedStream()
		{
			return m_pStream->GetUndecodedStream();
		}
		virtual Dict *GetDict()
		{
			return m_pStream->GetDict();
		}
		virtual Stream *GetNextStream()
		{
			return m_pStream;
		}

	protected:

		Stream *m_pStream;
	};

	//------------------------------------------------------------------------
	// ImageStream
	//------------------------------------------------------------------------

	class ImageStream
	{
	public:

		// Создаем Image object по картинке с заданными параметрами( тут имеются ввиду 
		// реальные параметры картинки, а не те что могут прийти в Predictor).
		ImageStream(Stream *pStream, int nWidth, int nComponents, int nBitsPerComponent);

		~ImageStream();

		void Reset();

		// Считываем следующий пиксел из потока. pPixel должно указывать на место достаточное,
		// для хранения как минимум m_nComponentsPerPixel элементов.
		bool GetPixel(unsigned char *pPixel);

		unsigned char *GetNextLine();

		void SkipLine();

	private:

		Stream        *m_pStream;             // Основной поток
		int            m_nWidth;              // Количество пикселей в строке
		int            m_nComponentsPerPixel; // Количество компонент на один пиксел
		int            m_nBitsPerComponent;   // Количество бит в компоненте
		int            m_nComponentsPerLine;  // Количество компонент в одной строке
		unsigned char *m_pLineBuffer;         // Буффер для одной строки изображения
		int            m_nLinePos;            // Текущая позиция в m_pLineBuffer
	};

	//------------------------------------------------------------------------
	// StreamPredictor (Predictor - дополнительный фильтр для картинок)
	//------------------------------------------------------------------------

	class StreamPredictor
	{
	public:

		// Создаем Predictor object. Параметры, которые задаются здесь, могут  
		// не совпадать с реальными параметрами картинки.
		StreamPredictor(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);

		~StreamPredictor();

		bool CheckValidate()
		{
			return m_bSuccess;
		}


		int LookChar();
		int GetChar();

	private:

		bool GetNextLine();

	private:

		Stream *m_pStream;             // Основной поток
		int     m_nPredictor;          // Predictor
		int     m_nWidth;              // Количество пикселей в строке
		int     m_nComponentsPerPixel; // Количество компонент на один пиксел
		int     m_nBitsPerComponent;   // Количество бит в компоненте
		int     m_nComponentsPerLine;  // Количество компонент в одной строке
		int     m_nBytesPerPixel;      // Количество байт в одном пикселе
		int     m_nBytesPerLine;       // Количество байт в строке
		unsigned char *m_pLineBuffer;  // Буффер для одной строки изображения
		int     m_nLinePos;            // Текущая позиция в m_pLineBuffer

		bool    m_bSuccess;
	};

	//------------------------------------------------------------------------
	// FileStream
	//------------------------------------------------------------------------

#define FileStreamBufferSize 1024

	class FileStream : public BaseStream
	{
	public:

		FileStream(FILE *pFile, unsigned int unStart, bool bLimited, unsigned int nLength, Object *pDict);
		virtual ~FileStream();

		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int nLength, Object *pDict);
		virtual StreamType GetType()
		{
			return strFile;
		}
		virtual void Reset();
		virtual void Close();
		inline virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		inline virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		inline virtual int GetPos()
		{
			return m_unBufferPos + (m_pBufferPointer - m_sBuffer);
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos()
		{
			return m_unStart;
		}
		virtual void SetStartPos(int nDelta);

	private:

		bool FillBuffer();

	private:

		FILE        *m_pFile;
		unsigned int m_unStart;
		bool         m_bLimited;
		unsigned int m_unLength;
		char         m_sBuffer[FileStreamBufferSize];
		char        *m_pBufferPointer;
		char        *m_pBufferEnd;
		unsigned int m_unBufferPos;
		int          m_nSavePos;
		bool         m_bSaved;
	};
	//------------------------------------------------------------------------
	// MemoryStream
	//------------------------------------------------------------------------

	class MemoryStream : public BaseStream
	{
	public:

		MemoryStream(char *sBuffer, unsigned int unStart, unsigned int unLength, Object *pDict);
		virtual ~MemoryStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict);
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual void Close();
		virtual int GetChar()
		{
			return (m_pBufferPointer < m_pBufferEnd) ? (*m_pBufferPointer++ & 0xff) : EOF;
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer < m_pBufferEnd) ? (*m_pBufferPointer & 0xff) : EOF;
		}
		virtual int GetPos()
		{
			return (int)(m_pBufferPointer - m_sBuffer);
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos()
		{
			return m_unStart;
		}
		virtual void SetStartPos(int delta);

		char* getCurrent();
		unsigned int getCurrentLength();

	private:

		char        *m_sBuffer;
		unsigned int m_unStart;
		unsigned int m_unLength;
		char        *m_pBufferEnd;
		char        *m_pBufferPointer;
		bool         m_bNeedFree;
	};

	//------------------------------------------------------------------------
	// EmbedStream
	//
	// Специальный тип потока, используемый для внутренныих(внедренных) потоков
	// (Inline images). Читаем напрямую из Base stream -- после удаления 
	// EmbedStream, чтение из Base stream будет совершаться для  оставшейся части
	// потока. Это соверешнно подругому, чем просто создать новый поток
	// FileStream (используя MakeSubStream).
	//------------------------------------------------------------------------

	class EmbedStream : public BaseStream
	{
	public:

		EmbedStream(Stream *pStream, Object *pDict, bool bLimited, unsigned int unLength);
		virtual ~EmbedStream();
		virtual Stream *MakeSubStream(unsigned int unStart, bool bLimited, unsigned int unLength, Object *pDict);
		virtual StreamType GetType()
		{
			return m_pStream->GetType();
		}
		virtual void Reset()
		{
		}
        virtual void Close(){}
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetPos()
		{
			return m_pStream->GetPos();
		}
		virtual void SetPos(unsigned int unPos, int nDirection = 0);
		virtual unsigned int GetStartPos();
		virtual void SetStartPos(int nDelta);

	private:

		Stream      *m_pStream;
		bool         m_bLimited;
		unsigned int m_unLength;
	};

	//------------------------------------------------------------------------
	// ASCIIHexStream
	//------------------------------------------------------------------------

	class ASCIIHexStream : public FilterStream
	{
	public:

		ASCIIHexStream(Stream *pStream);
		virtual ~ASCIIHexStream();
		virtual StreamType GetType()
		{
			return strASCIIHex;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			m_nBuffer = EOF;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		int  m_nBuffer;
		bool m_bEOF;
	};

	//------------------------------------------------------------------------
	// ASCII85Stream
	//------------------------------------------------------------------------

	class ASCII85Stream : public FilterStream
	{
	public:

		ASCII85Stream(Stream *pStream);
		virtual ~ASCII85Stream();
		virtual StreamType GetType()
		{
			return strASCII85;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			++m_nIndex;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		int  m_arrC[5]; // Имена массивов C и B взяты из спецификации
		int  m_arrB[4];
		int  m_nIndex;
		int  m_nCount;
		bool m_bEOF;
	};

	//------------------------------------------------------------------------
	// LZWStream
	//------------------------------------------------------------------------

	class LZWStream : public FilterStream
	{
	public:

		LZWStream(Stream *pStream, int nPredictor, int nColumns, int nColors, int nBitPerPixel, int nEarlyChange);
		virtual ~LZWStream();
		virtual StreamType GetType()
		{
			return strLZW;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		bool ProcessNextCode();
		void ClearTable();
		int  GetCode();

	private:

		StreamPredictor *m_pPredictor;         // Predictor (еще один дополнительный фильтр)
		int              m_nEarlyChange;       // Параметр и словаря для LZW фильтра
		bool             m_bEOF;               // Конец потока?
		int              m_nInputBuffer;       // Input buffer
		int              m_nInputBits;         // Количество бит в буффере
		struct
		{
			int           nLength;
			int           nHead;
			unsigned char unTail;
		}  m_pTable[4097];       // Таблица для декодирования
		int              m_nNextCode;          // Следующий код
		int              m_nNextBits;          // Число бит в следующем коде
		int              m_nPrevCode;          // Предыдущий код в потоке
		int              m_nNewChar;           // Новый символ, который мы добавим в таблицу
		unsigned char    m_arrCurBuffer[4097]; // Буффер для текущей последовательности
		int              m_nCurLength;         // Длина текущей последовательности
		int              m_nCurPos;            // Позиция в текущей последовательности
		bool             m_bFirst;             // Является ли данный код первым после очищения таблицы
	};

	//------------------------------------------------------------------------
	// RunLengthStream
	//------------------------------------------------------------------------

	class RunLengthStream : public FilterStream
	{
	public:

		RunLengthStream(Stream *pStream);
		virtual ~RunLengthStream();
		virtual StreamType GetType()
		{
			return strRunLength;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pEndOfBuffer && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pEndOfBuffer && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[128];   // Буфер
		char *m_pBufferPointer; // Указатель на следующий символ в буфере
		char *m_pEndOfBuffer;   // Указатель на конец буфера
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// CCITTFaxStream
	//------------------------------------------------------------------------

	struct CCITTCodeTable;

	class CCITTFaxStream : public FilterStream
	{
	public:

		CCITTFaxStream(Stream *pStream, int nK, bool bEndOfLine, bool bByteAlign, int nColumns, int nRows, bool bEndOfBlock, bool bBlackIs1);
		virtual ~CCITTFaxStream();
		virtual StreamType GetType()
		{
			return strCCITTFax;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			int nChar = LookChar();
			m_nCharBuffer = EOF;
			return nChar;
		}
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		short Get2DCode();
		short GetWhiteCode();
		short GetBlackCode();
		short LookBits(int nCount);
		void  SkipBits(int nCount)
		{
			if ((m_nInputBits -= nCount) < 0)
				m_nInputBits = 0;
		}

	private:

		int    m_nK;           // Параметр 'K', определяющий тип кодировки
		bool   m_bEndOfLine;   // Параметр 'EndOfLine'
		bool   m_bByteAlign;   // Параметр 'EncodedByteAlign'
		int    m_nColumns;     // Параметр 'Columns'
		int    m_nRows;        // Параметр 'Rows'
		bool   m_bEndOfBlock;  // Параметр 'EndOfBlock'
		bool   m_bBlackIs1;    // Параметр 'BlackIs1'

		bool   m_bEOF;         // 
		bool   m_bNextLine2D;  // True, если следующая строка использует кодировку 2D
		int    m_mCurRow;      // Текущая строка
		int    m_nInputBuffer; // Input buffer
		int    m_nInputBits;   // Количество бит в Input buffer
		short *m_pRefLine;     // Ссылочная строка, меняющую элементы
		int    m_nCurPosRL;    // Текущая позиция в m_pRefLine
		short *m_pCodingLine;  // Кодирующая строка, меняющая элементы
		int    m_nCurPosCL;    // Текущая позиция в m_pCodingLine 
		int    m_nOutputBits;  //
		int    m_nCharBuffer;  // 
	};

	//------------------------------------------------------------------------
	// DCTStream
	//------------------------------------------------------------------------

	struct DCTCompInfo
	{
		int nID;            // ID данного элемента
		int nXResolution;   // Разрешение по горизонтали
		int nYResolution;   // Разрешение по вертикали
		int nQuantTableNum; // Номер таблицы квантования
		int nPrevDC;        // 
	};

	struct DCTScanInfo
	{
		bool arrbComponent[4];  // arrbComponent[i] = true, если i-ая компонента включена в даннуб структуру
		int  nComponentsCount;  // количество компонент в данной структуре
		int  arrDCHuffTable[4];	// номера таблиц DC Huffman
		int  arrACHuffTable[4]; // номера таблиц AC Huffman
		int  nFirstKoef;        // первый DCT коэффициент
		int  nLastKoef;         // последний DCT коэффициент
		int  nApproxH;
		int  nApproxL;
	};

	struct DCTHuffTable
	{
		unsigned char  arrunFirstSymbol[17]; // Первый символ в данной группе
		unsigned short arrunFirstCode[17];   // Первый код в данной группе
		unsigned short arrunCodesCount[17];  // Количество кодов в данной группе
		unsigned char  arrunSymbols[256];    // Сами символы
	};

	class DCTStream : public FilterStream
	{
	public:

		DCTStream(Stream *pStream, int nColorTransform);
		virtual ~DCTStream();
		virtual StreamType GetType()
		{
			return strDCT;
		}
		virtual void Reset();
		virtual void Close();
		virtual int GetChar();
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);
		Stream *GetRawStream()
		{
			return m_pStream;
		}

	private:

		void Restart();
		bool ReadMCURow();
		void ReadScan();
		bool ReadDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64]);
		bool ReadProgressiveDataUnit(DCTHuffTable *pDCHuffTable, DCTHuffTable *pACHuffTable, int *pnPrevDC, int arrData[64]);
		void DecodeImage();
		void TransformDataUnit(unsigned short *pQuantTable, int arrDataIn[64], unsigned char arrDataOut[64]);
		int  ReadHuffSymbol(DCTHuffTable *pTable);
		int  ReadAmp(int nSize);
		int  ReadBit();
		bool ReadHeader();
		bool ReadBaselineSOF();
		bool ReadProgressiveSOF();
		bool ReadScanInfo();
		bool ReadQuantTables();
		bool ReadHuffmanTables();
		bool ReadRestartInterval();
		bool ReadJFIFMarker();
		bool ReadAdobeMarker();
		bool ReadTrailer();
		int  ReadMarker();
		int  Read16();

	private:

		bool           m_bProgressive;          // True, если мод progressive
		bool           m_bInterleaved;          // True, если мод interleaved
		int            m_nWidth;                // ширина изображения
		int            m_nHeight;               // высота изображения
		int            m_nMCUWidth;             // ширина min coding unit(MCU)
		int            m_nMCUHeight;            // высота min coding unit(MCU)
		int            m_nBufferWidth;          // ширина FrameBuffer
		int            m_nBufferHeight;         // высота FrameBuffer
		DCTCompInfo    m_arrCompInfo[4];	      // Структура Info для каждой компоненты
		DCTScanInfo    m_oCurScanInfo;          // Структура Info для текущего Scan
		int            m_nComponentsCount;      // Количество компонент изображения
		int            m_nColorTransform;       // Преобразования пространства цветов:
		// -1 = непоределено
		//  0 = нет преобразования
		//  1 = YUV/YUVK -> RGB/CMYK
		bool           m_bJFIFMarker;           // True, если есть маркер APP0 JFIF 
		bool           m_bAdobeMarker;          // True, если есть маркер APP14 Adobe 
		int            m_nRestartInterval;      // в MCU
		unsigned short m_arrQuantTables[4][64]; // Таблицы квантования
		int            m_nQuantTablesCount;     // количество таблицы квантования
		DCTHuffTable   m_arrDCHuffTables[4];    // Таблицы DC Huffman
		DCTHuffTable   m_arrACHuffTables[4];    // Таблицы AC Huffman
		int            m_nDCHuffTablesCount;    // Количество таблиц DC Huffman
		int            m_nACHuffTablesCount;    // Количество таблиц AC Huffman
		unsigned char *m_pppRowBuffer[4][32];   // Буфер для MCU (non-progressive mode)
		int           *m_ppFrameBuffer[4];      // Буфер для фрэйма (progressive mode)

		int            m_nCurComponent;         // Текущие параметры для кртинки/MCU
		int            m_nX;                    //
		int            m_nY;                    //
		int            m_nDY;                   //

		int            m_nRestartCtr;           // количество оставишхся MCU до рестарта
		int            m_nRestartMarker;        //
		int            m_nEOBRun;               // количество оставшихся EOB(end-of-block) в текущей группе

		int            m_nInputBuffer;          // Буфер для кодов разной длины
		int            m_nInputBits;            // Число корректных бит в Input buffer

	};

	//------------------------------------------------------------------------
	// FlateZlibStream
	//------------------------------------------------------------------------

#define flateZlibWindow             32768   // Размер буфера
#define flateZlibMask               32767   // (flateZlibWindow - 1)

	class FlateZlibStream : public FilterStream
	{
	public:

		FlateZlibStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);
		virtual ~FlateZlibStream();
		virtual StreamType GetType()
		{
			return strFlate;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		void ReadSome();

	private:

        CInflate          m_oZStream;                     // Zlib поток
		StreamPredictor  *m_pPredictor;                   // Predictor
		unsigned int      m_nInSize;                      // Размер входного буффера
		unsigned char     m_arrInBuffer[flateZlibWindow]; // Входной буффер
		unsigned char     m_arrBuffer[flateZlibWindow];   // Буфер для результата декодирования
		int               m_nBufferCurPos;                // Текущая позиция в буфере
		int               m_nRemain;                      // Число корректных байт в буфере
		bool              m_bEndOfBlock;                  // True, когда достигли конца блока
		bool              m_bEOF;                         // True, когда достигли конца потока
	};


	//------------------------------------------------------------------------
	// FlateStream
	//------------------------------------------------------------------------

#define flateWindow             32768   // Размер буфера
#define flateMask               (flateWindow-1)
#define flateMaxHuffman         15      // макимальная длина кода Huffman
#define flateMaxCodeLenCodes    19      // max # code length codes
#define flateMaxLitCodes        288     // max # literal codes
#define flateMaxDistCodes       30      // max # distance codes

	struct FlateHuffmanCode
	{
		unsigned short nLength;   // длина кода в битах
		unsigned short nValue;    // значение данного коду
	};

	struct FlateHuffmanTable
	{
		FlateHuffmanCode *pCodes;  // таблица кодов
		int               nMaxLen; // макимальная длина кода
	};

	struct FlateDecode
	{
		int nExtraBitsCount; // количество дополнительных бит
		int nFirst;          // первая длина
	};

	class FlateStream : public FilterStream
	{
	public:

		FlateStream(Stream *pStream, int nPredictor, int nWidth, int nComponents, int nBitsPerComponent);
		virtual ~FlateStream();
		virtual StreamType GetType()
		{
			return strFlate;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual int GetRawChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent);
		virtual bool IsBinary(bool bLast = true);

	private:

		void ReadSome();
		bool StartBlock();
		void LoadFixedCodes();
		bool ReadDynamicCodes();
		void ConvertHuffmanCodes(int *pLengths, int nCount, FlateHuffmanTable *pTable);
		int  GetHuffmanCodeWord(FlateHuffmanTable *pTable);
		int  GetCodeWord(int nBits);

	private:

		StreamPredictor  *m_pPredictor;              // Predictor
		unsigned char     m_arrBuffer[flateWindow];  // Буфер для результата декодирования
		int               m_nBufferCurPos;           // Текущая позиция в буфере
		int               m_nRemain;                 // Число корректных байт в буфере
		int               m_nCodeBuffer;             // Бефер исходных кодов
		int               m_nCodeSize;               // Число бит в m_nCodeBuffer
		int				  m_arrCodeLengths[flateMaxLitCodes + flateMaxDistCodes]; // Длины кодов(и для самих слов и для расстояний)
		FlateHuffmanTable m_oLiteratCodeTable;       // Таблица буквенных кодов
		FlateHuffmanTable m_oDistanceCodeTable;      // Таблица кодов-расстояний
		bool              m_bCompressedBlock;        // True, если читаем сжатый блок
		int               m_nUncompBlockLen;         // Оставшаяся длина несжатого блока
		bool              m_bEndOfBlock;             // True, когда достигли конца блока
		bool              m_bEOF;                    // True, когда достигли конца потока

		static int               m_arrCodeLenCodeMap[flateMaxCodeLenCodes]; // 
		static FlateDecode       m_arrLengthDecode[flateMaxLitCodes - 257];   // 
		static FlateDecode       m_arrDistanceDecode[flateMaxDistCodes];    // 
		static FlateHuffmanTable m_oFixedLiteralCodeTable;   // 
		static FlateHuffmanTable m_oFixedDistanceCodeTable;  // 
	};

	//------------------------------------------------------------------------
	// EOFStream
	//------------------------------------------------------------------------

	class EOFStream : public FilterStream
	{
	public:

		EOFStream(Stream *pStream);
		virtual ~EOFStream();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset()
		{
		}
		virtual int GetChar()
		{
			return EOF;
		}
		virtual int LookChar()
		{
			return EOF;
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
	};

	//------------------------------------------------------------------------
	// FixedLengthEncoder
	//------------------------------------------------------------------------

	class FixedLengthEncoder : public FilterStream
	{
	public:

		FixedLengthEncoder(Stream *pStream, int nLength);
		~FixedLengthEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar();
		virtual int LookChar();
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true);
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		int m_nLength;
		int m_nCount;
	};

	//------------------------------------------------------------------------
	// ASCIIHexEncoder
	//------------------------------------------------------------------------

	class ASCIIHexEncoder : public FilterStream
	{
	public:

		ASCIIHexEncoder(Stream *pStream);
		virtual ~ASCIIHexEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[4];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		int   m_nLineLength;
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// ASCII85Encoder
	//------------------------------------------------------------------------

	class ASCII85Encoder : public FilterStream
	{
	public:

		ASCII85Encoder(Stream *pStream);
		virtual ~ASCII85Encoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return false;
		}
		virtual bool IsEncoder()
		{
			return true;
		}
	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[8];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		int   m_nLineLength;
		bool  m_bEOF;
	};

	//------------------------------------------------------------------------
	// RunLengthEncoder
	//------------------------------------------------------------------------

	class RunLengthEncoder : public FilterStream
	{
	public:

		RunLengthEncoder(Stream *pStream);
		virtual ~RunLengthEncoder();
		virtual StreamType GetType()
		{
			return strWeird;
		}
		virtual void Reset();
		virtual int GetChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer++ & 0xff);
		}
		virtual int LookChar()
		{
			return (m_pBufferPointer >= m_pBufferEnd && !FillBuffer()) ? EOF : (*m_pBufferPointer & 0xff);
		}
		virtual StringExt *GetPSFilter(int nPSLevel, char *sIndent)
		{
			return NULL;
		}
		virtual bool IsBinary(bool bLast = true)
		{
			return true;
		}
		virtual bool IsEncoder()
		{
			return true;
		}

	private:

		bool FillBuffer();

	private:

		char  m_sBuffer[131];
		char *m_pBufferPointer;
		char *m_pBufferEnd;
		char *m_pNextEnd;
		bool  m_bEOF;
	};
}

#endif // _PDF_READER_STREAM_H
