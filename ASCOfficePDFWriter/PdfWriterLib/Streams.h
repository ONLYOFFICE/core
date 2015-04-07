#ifndef _STREAMS_H
#define _STREAMS_H

#pragma once

#include <zlib.h>

#include "Utils.h"
#include "List.h"
#include "Encrypt.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

unsigned int  DocGetTempFile         (Doc pPDF, CString *wsTempFileName);

#define STREAM_SIG_BYTES  0x5354524DL

// тип потока

typedef enum EStreamType 
{
    StreamUnknown = 0,
    StreamCallBack,
    StreamFile,
    StreamMemory,
	StreamTempFile
} StreamType;

#define STREAM_FILTER_NONE          0x0000
#define STREAM_FILTER_ASCIIHEX      0x0100
#define STREAM_FILTER_ASCII85       0x0200
#define STREAM_FILTER_FLATE_DECODE  0x0400
#define STREAM_FILTER_DCT_DECODE    0x0800
#define STREAM_FILTER_JPX_DECODE    0x1000
#define STREAM_FILTER_JBIG2_DECODE  0x2000
#define STREAM_FILTER_LZW_DECODE    0x4000

#define STREAM_PREDICTOR_NONE       0x0000
#define STREAM_PREDICTOR_FLATE_TIFF 0x0001

typedef enum EWhenceMode 
{
    SeekSet = 0,
    SeekCur,
    SeekEnd,
	SeekWrite
} WhenceMode;

typedef struct StreamRec*	StreamRecPtr;

typedef unsigned long (*StreamWriteFunc) (StreamRecPtr pStream, const BYTE *pBuffer, unsigned int nSize);
typedef unsigned long (*StreamReadFunc)  (StreamRecPtr pStream, BYTE *pBuffer, unsigned int *pnSize);
typedef unsigned long (*StreamSeekFunc)  (StreamRecPtr pStream, int nPos, WhenceMode oMode);
typedef int           (*StreamTellFunc)  (StreamRecPtr pStream);
typedef void          (*StreamFreeFunc)  (StreamRecPtr pStream);
typedef unsigned int  (*StreamSizeFunc)  (StreamRecPtr pStream);

struct StreamRec 
{
    unsigned int    nSigBytes;
    StreamType      eType;
    MMgr            oMMgr;
    Error           oError;
    unsigned int    nSize;
    StreamWriteFunc pWriteFn;
    StreamReadFunc  pReadFn;
    StreamSeekFunc  pSeekFn;
    StreamFreeFunc  pFreeFn;
    StreamTellFunc  pTellFn;
    StreamSizeFunc  pSizeFn;
    void*           pAttr;
	unsigned int    nCheckSum; // Чексумма CRC32 вычисленная по последнему вызову StreamWrite
};

struct MemStreamAttrRec 
{
    List         pBuf;
    unsigned int nBufSize;
    unsigned int nWPos;
    BYTE        *pWPtr;
    unsigned int nRPtrIndex;
    unsigned int nRPos;
    BYTE        *pRPtr;
};
typedef MemStreamAttrRec*	MemStreamAttrRecPtr;

#ifndef MAX_PATH
    #define MAX_PATH        260 // windef.h
#endif

struct TempFileStreamAttrRec
{
	unsigned long  nReaderPos;          // Текущая позиция для чтения
	unsigned long  nWriterPos;          // Текущая позиция для записи
	unsigned long  nCurPos;             // Текущая позиция ( для функций Tell и Seek )
    wchar_t        wsFilePath[MAX_PATH];// Путь к файлу
};
typedef TempFileStreamAttrRec* TempFileStreamAttrRecPtr;



// additional functions

unsigned long MemStreamInWrite      (StreamRecPtr  pStream, const BYTE  **ppBuffer, unsigned int *pnCount);
unsigned long MemStreamWriteFunc    (StreamRecPtr  pStream, const BYTE *pBuffer, unsigned int nSize);
unsigned long MemStreamSeekFunc     (StreamRecPtr  pStream, int nPos, WhenceMode  eMode);
BYTE*         MemStreamGetBufPtr    (StreamRecPtr  pStream, unsigned int nIndex, unsigned int *pnLength);
unsigned long MemStreamReadFunc     (StreamRecPtr  pStream, BYTE *pBuffer, unsigned int *pnSize);
int           MemStreamTellFunc     (StreamRecPtr  pStream);
unsigned int  MemStreamSizeFunc     (StreamRecPtr  pStream);
void          MemStreamFreeData     (StreamRecPtr  pStream);
void          MemStreamFreeFunc     (StreamRecPtr  pStream);
/*
 *  FileReaderReadFunc
 *
 *  Reading data function for FileReader.
 *
 */
unsigned long FileReaderReadFunc    (StreamRecPtr  pStream, BYTE *pBuffer, unsigned int *pnSize);
/*
 *  FileReaderSeekFunc
 *
 *  Seeking data function for FileReader.
 *
 *  pStream     : Указатель на объект StreamRecPtr.
 *  nPos        : Новая позиция потока.
 *  eWhenceMode : Seeking mode
 *                     SeekSet : По отношению к началу файла
 *                     SeekCur : По отношению к текующей позиции в файле
 *                     SeekEnd : По отношению к концу файла
 *
 *  В случае успеха возвращает OK.
 *  В противном случае, возвращает AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR и Error_GetCode2() 
 *
 */
unsigned long FileReaderSeekFunc    (StreamRecPtr  pStream, int nPos, WhenceMode  eMode);
int           FileStreamTellFunc    (StreamRecPtr  pStream);
unsigned int  FileStreamSizeFunc    (StreamRecPtr  pStream);
unsigned long FileWriterWriteFunc   (StreamRecPtr  pStream, const BYTE *pBuffer, unsigned int nSize );
void          FileStreamFreeFunc    (StreamRecPtr  pStream);
unsigned long TempFileStreamWriteFunc(StreamRecPtr  pStream, const BYTE *pBuffer, unsigned int nSize );
unsigned long TempFileStreamReadFunc(StreamRecPtr  pStream, BYTE *pBuffer, unsigned int *pnSize);
unsigned long TempFileStreamSeekFunc(StreamRecPtr  pStream, int nPos, WhenceMode  eMode);
int           TempFileStreamTellFunc(StreamRecPtr  pStream);
unsigned int  TempFileStreamSizeFunc(StreamRecPtr  pStream);
void          TempFileStreamFreeFunc(StreamRecPtr  pStream);
unsigned long TempFileStreamReaderSeek(StreamRecPtr pStream, int nPos, WhenceMode eMode);
unsigned long TempFileStreamWriterSeek(StreamRecPtr pStream, int nPos, WhenceMode eMode);
// main functions

StreamRecPtr        MemStreamNew          (MMgr    oMMgr, unsigned int nBufSize);
unsigned int  MemStreamGetBufSize   (StreamRecPtr  pStream);
unsigned int  MemStreamGetBufCount  (StreamRecPtr  pStream);
unsigned long MemStreamRewrite      (StreamRecPtr  pStream, BYTE *pBuf, unsigned int nSize);
unsigned int  StreamSize            (StreamRecPtr  pStream);
unsigned long StreamSeek            (StreamRecPtr  pStream, int nPos, WhenceMode  eMode);
/*
 *  StreamRead
 *
 *  pStream : Указатель на объект StreamRecPtr.
 *  pBuffer : Указатель на буфер данных.
 *  pnSize  : Указатель на переменную, которая хранит размер буфера.
 *
 *  Возращает OK в случае успеха. В противном случае, возвращает код
 *  ошибки, который возвращает функция чтения данного потока.
 *
 */
unsigned long StreamRead            (StreamRecPtr  pStream, BYTE *pBuffer, unsigned int *pnSize);
/*
 *  StreamReadLn
 *
 *  pStream : Указатель на объект StreamRecPtr.
 *  sBuffer : Указатель на буфер данных.
 *  pnSize  : Указатель на переменную, которая хранит размер буфера.
 *
 *  Читаем из потока, до тех пор пока либо буфер не закончится, либо 
 *  не наткнемся на перенос строки.
 *
 */
unsigned long StreamReadLn          (StreamRecPtr  pStream, char *sBuffer, unsigned int *nSize);
unsigned long StreamReadULong       (StreamRecPtr  pStream, unsigned int *pnValue);
unsigned long StreamReadUByte       (StreamRecPtr  pStream, unsigned int *pnValue);
unsigned long StreamReadByte        (StreamRecPtr  pStream, int *pnValue);
unsigned long StreamReadUShort      (StreamRecPtr  pStream, unsigned int *pnValue);
/*
 * StreamWrite
 *
 *  pStream : Указатель на объект StreamRecPtr.
 *  pBuffer : Указатель на буфер для записи.
 *  nSize   : Размер буфера для записи.
 *
 *  Возращает OK в случае успеха. В противном случае, возвращает код
 *  ошибки, который возвращает функция записи данного потока.
 *
 */
unsigned long StreamWrite           (StreamRecPtr  pStream, const BYTE *pBuffer, unsigned int nSize, bool bCalcCheckSum = false);
unsigned long StreamWriteToStreamWithDeflate( StreamRecPtr  pSrc, StreamRecPtr  pDst, EncryptRecPtr pEncrypt);
unsigned long StreamWriteToStream   (StreamRecPtr  pSrc, StreamRecPtr pDst, unsigned int nFilter, EncryptRecPtr pEncrypt);
//unsigned long StreamWriteToStream   (StreamRecPtr  pSrc, StreamRecPtr pDst, unsigned int nFilter/*, EncryptRecPtr  e*/)
//{
//    unsigned long nRet = 0;
//    
//	BYTE pBuf[STREAM_BUF_SIZ];
//    BYTE pEBuf[STREAM_BUF_SIZ];
//    
//	BOOL bFlag = FALSE;
//
//	if ( !pDst || ! ( pDst->pWriteFn ) ) 
//	{
//		SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
//        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
//    }
//
//	if ( NOERROR != ErrorGetCode( pSrc->oError ) || NOERROR != ErrorGetCode( pDst->oError ) )
//        return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;
//
//    // инициализируем входящий поток 
//    
//	if ( 0 == StreamSize( pSrc ) )
//        return OK;
//
//    nRet = StreamSeek( pSrc, 0, SeekSet );
//    if ( OK != nRet )
//        return nRet;
//
//    bFlag = FALSE;
//    for (;;) 
//	{
//        unsigned int nSize = STREAM_BUF_SIZ;
//
//        nRet = StreamRead( pSrc, pBuf, &nSize );
//
//        if ( OK != nRet ) 
//		{
//            if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
//			{
//                bFlag = TRUE;
//                if ( 0 == nSize )
//                    break;
//            } 
//			else 
//			{
//                return nRet;
//            }
//        }
//
//  //      if ( e ) 
//		//{
//  //          EncryptCryptBuf (e, pBuf, pEBuf, nSize);
//  //          nRet = StreamWrite( pDst, pEBuf, nSize);
//  //      } 
//		//else 
//		//{
//            nRet = StreamWrite( pDst, pBuf, nSize);
//        //}
//
//        if ( OK != nRet )
//            return nRet;
//
//        if ( bFlag )
//            break;
//    }
//
//    return OK;
//}
//
StreamRecPtr	FileReaderNew         (MMgr    oMMgr, std::wstring & wsFileName);
StreamRecPtr	FileWriterNew         (MMgr    oMMgr, std::wstring & wsFileName, bool create = false);
StreamRecPtr	TempFileStreamNew     (MMgr    oMMgr, std::wstring wsTempFilePath = _T(""), unsigned int unCheckSum = 0);
/*
 *  CallbackReaderNew
 *
 *  Конструктор для CallbackReader.
 *
 *  oMMgr   : Указатель на объект MMgr.
 *  pReadFn : Указатель на функию пользователя для чтения данных.
 *  pSeekFn : Указатель на функию пользователя для поиска данных.
 *  pTellFn : Указатель на функию пользователя для определения текущего указателя файла.
 *  pSizeFn : Указатель на функию пользователя для определения размера файла.
 *  pData   : Указатель на данные определенные пользователем.
 *
 *  return: В случае успеха возвращает указатель на новый объект StreamRecPtr,
 *          в случае неудачи, возвращает NULL.
 *
 */
StreamRecPtr	CallbackReaderNew     (MMgr    oMMgr, StreamReadFunc  pReadFn, StreamSeekFunc  pSeekFn, StreamTellFunc pTellFn, StreamSizeFunc pSizeFn, void* pData);
/*
 *  CallbackWriterNew
 *
 *  Конструктор для CallbackWriter.
 *
 *  oMMgr    : Указатель на объект MMgr.
 *  pWriteFn : Указатель на функию пользователя для записи данных.
 *  pData    : Указатель на данные определенные пользователем.
 *
 *  return: В случае успеха возвращает указатель на новый объект StreamRecPtr,
 *          в случае неудачи, возвращает NULL.
 *
 */
StreamRecPtr	CallbackWriterNew     (MMgr    oMMgr, StreamWriteFunc pWriteFn, void* pData);
void          StreamFree            (StreamRecPtr  pStream);
unsigned long StreamWriteChar       (StreamRecPtr  pStream, char cValue);
unsigned long StreamWriteStr        (StreamRecPtr  pStream, const char *sValue);
unsigned long StreamWriteUChar      (StreamRecPtr  pStream, BYTE nValue);
unsigned long StreamWriteInt        (StreamRecPtr  pStream, int nValue);
unsigned long StreamWriteUInt       (StreamRecPtr  pStream, unsigned int nValue);
unsigned long StreamWriteHex        (StreamRecPtr  pStream, int nValue, int nLen);
unsigned long StreamWriteReal       (StreamRecPtr  pStream, float fValue);
int           StreamTell            (StreamRecPtr  pStream);
BOOL          StreamEOF             (StreamRecPtr  pStream);
unsigned long StreamFlush           (StreamRecPtr  pStream);
unsigned long StreamWriteEscapeName (StreamRecPtr  pStream, const char *sValue);
unsigned long StreamWriteEscapeText2(StreamRecPtr  pStream, const BYTE *sText, unsigned int nLen);
unsigned long StreamWriteEscapeText (StreamRecPtr  pStream, const BYTE *sText, unsigned int nLen);
unsigned long StreamWriteBinary (StreamRecPtr  pStream, const BYTE *pData, unsigned int nLen, EncryptRecPtr pEncrypt);
unsigned long StreamValidate        (StreamRecPtr  pStream);
namespace MemoryUtils
{
	class ZipBuffer
	{
	public:
		ZipBuffer() : buffer(NULL), size(0)
		{

		}
		ZipBuffer(const CString& source) : buffer(NULL), size(0)
		{
			if (source.GetLength())
				DeflateString(source);
		}
		~ZipBuffer()
		{
			RELEASEOBJECT(buffer);
		}

		inline const unsigned char* GetData() const
		{
			return buffer;
		}
		inline long GetSize() const 
		{
			return size;
		}

	private:

		inline BOOL DeflateString(const CString& source)
		{
//#ifdef NOZLIB

			size	=	source.GetLength();
			buffer	=	new unsigned char[size];
			if (buffer)
			{
				memcpy(buffer,source.GetString(), size); 
				return TRUE;
			}
//#else

//#endif
			return FALSE;
		}

	private:

		unsigned char*	buffer;
		long			size;
	};
};

#endif /* _STREAMS_H */
