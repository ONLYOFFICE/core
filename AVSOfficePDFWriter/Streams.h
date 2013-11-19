#ifndef _STREAMS_H
#define _STREAMS_H

#include <errno.h>

// пока временно откажемся от deflate
//#define NOZLIB

#ifndef NOZLIB
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib, "Zlib\\zlib.lib")
#endif /* NOZLIB */

#include "Consts.h"
#include "Utils.h"
#include "List.h"
#include "Encrypt.h"

static unsigned int  DocGetTempFile         (Doc pPDF, CString *wsTempFileName);

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

typedef struct TStreamRec  *Stream;

typedef unsigned long (*StreamWriteFunc) (Stream pStream, const BYTE *pBuffer, unsigned int nSize);
typedef unsigned long (*StreamReadFunc)  (Stream pStream, BYTE *pBuffer, unsigned int *pnSize);
typedef unsigned long (*StreamSeekFunc)  (Stream pStream, int nPos, WhenceMode oMode);
typedef int           (*StreamTellFunc)  (Stream pStream);
typedef void          (*StreamFreeFunc)  (Stream pStream);
typedef unsigned int  (*StreamSizeFunc)  (Stream pStream);

typedef struct TMemStreamAttrRec  *MemStreamAttr;

typedef struct TMemStreamAttrRec 
{
    List         pBuf;
    unsigned int nBufSize;
    unsigned int nWPos;
    BYTE        *pWPtr;
    unsigned int nRPtrIndex;
    unsigned int nRPos;
    BYTE        *pRPtr;
} MemStreamAttrRec;

typedef struct TTempFileStreamAttrRec *TempFileStreamAttr;

typedef struct TTempFileStreamAttrRec
{
	unsigned long  nReaderPos;          // Текущая позиция для чтения
	unsigned long  nWriterPos;          // Текущая позиция для записи
	unsigned long  nCurPos;             // Текущая позиция ( для функций Tell и Seek )
	wchar_t        wsFilePath[MAX_PATH]; // Путь к файлу
} TempFileStreamAttrRec;

typedef struct TStreamRec 
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
} StreamRec;

// additional functions

unsigned long MemStreamInWrite      (Stream  pStream, const BYTE  **ppBuffer, unsigned int *pnCount)
{
	MemStreamAttr pAttr  = (MemStreamAttr)pStream->pAttr;
	unsigned int  nRSize = pAttr->nBufSize - pAttr->nWPos;

    if ( *pnCount <= 0 )
        return OK;

    if ( nRSize >= *pnCount ) 
	{
		UtilsMemCpy( pAttr->pWPtr, *ppBuffer, *pnCount);
		pAttr->pWPtr += *pnCount;
		pAttr->nWPos += *pnCount;
        *pnCount = 0;
    } 
	else 
	{
        if (nRSize > 0) 
		{
			UtilsMemCpy( pAttr->pWPtr, *ppBuffer, nRSize);
            *ppBuffer += nRSize;
            *pnCount  -= nRSize;
        }

		pAttr->pWPtr = (BYTE*)GetMem(pStream->oMMgr, pAttr->nBufSize);
		if (NULL == pAttr->pWPtr)
			return ErrorGetCode(pStream->oError);

		if (OK != ListAdd(pAttr->pBuf, pAttr->pWPtr)) 
		{
			FreeMem(pStream->oMMgr, pAttr->pWPtr);
			pAttr->pWPtr = NULL;

			return ErrorGetCode(pStream->oError);
        }

		pAttr->nWPos = 0;
    }
    return OK;
}

unsigned long MemStreamWriteFunc    (Stream  pStream, const BYTE *pBuffer, unsigned int nSize)
{
    unsigned int nWSize = nSize;

	if ( ErrorGetCode( pStream->oError ) != 0 )
        return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;

    while ( nWSize > 0 ) 
	{
        unsigned long nRet = MemStreamInWrite( pStream, &pBuffer, &nWSize );
        if ( OK != nRet )
            return nRet;
    }

    return OK;
}


unsigned long MemStreamSeekFunc     (Stream  pStream, int nPos, WhenceMode  eMode)
{
	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;

    if ( SeekCur == eMode ) 
	{
		nPos += (pAttr->nRPtrIndex * pAttr->nBufSize );
		nPos += pAttr->nRPos;
    } 
	else if ( SeekEnd == eMode )
		nPos = pStream->nSize - nPos;

	if ( nPos > pStream->nSize || 0 == pStream->nSize )
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF, 0);

	pAttr->nRPtrIndex	=	nPos / pAttr->nBufSize;
	pAttr->nRPos		=	nPos % pAttr->nBufSize;
	pAttr->pRPtr		=	(BYTE*)ListItemAt(pAttr->pBuf, pAttr->nRPtrIndex);

	if (eMode == SeekWrite)
	{
		pAttr->nWPos	=	nPos % pAttr->nBufSize;
		pAttr->pWPtr	=	(BYTE*)ListItemAt(pAttr->pBuf, pAttr->nRPtrIndex);

		if (pAttr->nRPtrIndex < pAttr->pBuf->nCount)
		{
			int baseInd = pAttr->nRPtrIndex + 1;
			for (int i = baseInd; i < (int)pAttr->pBuf->nCount; ++i)
			{
				ListRemoveByIndex(pAttr->pBuf, baseInd);
			}
		}

		if (NULL == pAttr->pWPtr) 
		{
			SetError(pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
			return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
		}

		pAttr->pWPtr	+=	pAttr->nRPos;
		pStream->nSize	=	nPos;
	}
    
	if (NULL == pAttr->pRPtr) 
	{
		SetError(pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    } 
	else
	{
		pAttr->pRPtr += pAttr->nRPos;
	}

    return OK;
}


BYTE*         MemStreamGetBufPtr    (Stream  pStream, unsigned int nIndex, unsigned int *pnLength)
{
    BYTE *pRet = NULL;
    MemStreamAttr pAttr;

	if ( StreamMemory != pStream->eType ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return NULL;
    }

	pAttr = (MemStreamAttr)pStream->pAttr;

	pRet = (BYTE *)ListItemAt( pAttr->pBuf, nIndex );
    
	if ( NULL == pRet ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);
        *pnLength = 0;
        return NULL;
    }

	*pnLength = ( pAttr->pBuf->nCount - 1 == nIndex) ? pAttr->nWPos : pAttr->nBufSize;
    return pRet;
}

unsigned long MemStreamReadFunc     (Stream  pStream, BYTE *pBuffer, unsigned int *pnSize)
{
	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;
    unsigned int nBufSize = 0;
    unsigned int nRLen = *pnSize;

	*pnSize = 0;

	unsigned int offset = 0;

    while (nRLen > 0) 
	{
        unsigned int nTmpLen;

		if ( 0 == pAttr->pBuf->nCount )
            return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;

		if ( pAttr->pBuf->nCount - 1 > pAttr->nRPtrIndex )
			nTmpLen = pAttr->nBufSize - pAttr->nRPos;
		else if ( pAttr->pBuf->nCount - 1 == pAttr->nRPtrIndex )
			nTmpLen = pAttr->nWPos - pAttr->nRPos;
        else
            return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;

		if ( !pAttr->pRPtr )
			pAttr->pRPtr = (BYTE*)ListItemAt( pAttr->pBuf, pAttr->nRPtrIndex );

        if (nTmpLen >= nRLen) 
		{
			UtilsMemCpy (pBuffer + offset, pAttr->pRPtr, nRLen);
			
			pAttr->nRPos	+=	nRLen;
            *pnSize			+=	nRLen;
			pAttr->pRPtr	+=	nRLen;
          
			return OK;
        } 
		else 
		{
			UtilsMemCpy (pBuffer + offset, pAttr->pRPtr, nTmpLen);
			offset += nTmpLen;
           
			nRLen -= nTmpLen;
            *pnSize += nTmpLen;

			if (pAttr->nRPtrIndex == pAttr->pBuf->nCount - 1) 
			{
				pAttr->pRPtr += nTmpLen;
				pAttr->nRPos += nTmpLen;
               
				return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
            }

			pAttr->nRPtrIndex++;
			pAttr->nRPos = 0;
			pAttr->pRPtr = MemStreamGetBufPtr(pStream, pAttr->nRPtrIndex, &nBufSize);
        }
    }

    return OK;
}


int           MemStreamTellFunc     (Stream  pStream)
{
    int nRet = 0;
    
	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;

    nRet  = pAttr->nRPtrIndex * pAttr->nBufSize;
	nRet += pAttr->nRPos;

    return nRet;
}


unsigned int  MemStreamSizeFunc     (Stream  pStream)
{
    return pStream->nSize;
}


void          MemStreamFreeData     (Stream  pStream)
{
	if ( !pStream || StreamMemory != pStream->eType )
        return;

	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;

	for (unsigned int nIndex = 0; nIndex < pAttr->pBuf->nCount; nIndex++)
		FreeMem( pStream->oMMgr, ListItemAt( pAttr->pBuf, nIndex ) );

	ListClear( pAttr->pBuf );

	pStream->nSize = 0;
	pAttr->nWPos = pAttr->nBufSize;
	pAttr->pWPtr = NULL;
	pAttr->nRPtrIndex = 0;
	pAttr->nRPos = 0;
}
void          MemStreamFreeFunc     (Stream  pStream)
{
    MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;
    MemStreamFreeData(pStream);
    ListFree( pAttr->pBuf );
    FreeMem ( pStream->oMMgr, pAttr);
    pStream->pAttr = NULL;
}

/*
 *  FileReaderReadFunc
 *
 *  Reading data function for FileReader.
 *
 */
unsigned long FileReaderReadFunc    (Stream  pStream, BYTE *pBuffer, unsigned int *pnSize)
{
	FILE* pFile = (FILE*)pStream->pAttr;
    
	unsigned int nRSize = 0;

    UtilsMemSet( pBuffer, 0, *pnSize );
    
	nRSize = fread( pBuffer, 1, *pnSize, pFile);

    if ( nRSize != *pnSize ) 
	{
        if ( feof( pFile ) ) 
		{

            *pnSize = nRSize;

            return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
        }

		return SetError (pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
    }

    return OK;
}



/*
 *  FileReaderSeekFunc
 *
 *  Seeking data function for FileReader.
 *
 *  pStream     : Указатель на объект Stream.
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
unsigned long FileReaderSeekFunc    (Stream  pStream, int nPos, WhenceMode  eMode)
{
    FILE* pFile = (FILE*)pStream->pAttr;
    
	int nWhence = 0;

    switch ( eMode ) 
	{
        case SeekCur:
            nWhence = SeekCur;
            break;
        case SeekEnd:
            nWhence = SeekEnd;
            break;
        default:
            nWhence = SeekSet;
    }

    if ( 0 != fseek( pFile, nPos, nWhence) ) 
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
    }

    return OK;
}




int           FileStreamTellFunc    (Stream  pStream)
{
    int nRet = 0;
    
	FILE* pFile = (FILE*)pStream->pAttr;

    if ( ( nRet = ftell( pFile ) ) < 0 ) 
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR,  ferror(pFile) );
    }

    return nRet;
}


unsigned int  FileStreamSizeFunc    (Stream  pStream)
{
    int nSize = 0;
    int nPtr = 0;
    
	FILE* pFile = (FILE*)pStream->pAttr;

    // сохраняем текущий указатель файла
    if ( ( nPtr = ftell( pFile ) ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
        return 0;
    }

    // двигаем указатель в конец файла 
    if ( fseek( pFile, 0, SeekEnd ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
        return 0;
    }

    // получаем указатель конца файла
    if ( ( nSize = ftell( pFile ) ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
        return 0;
    }

    // воостанавливаем текущий указатель файла
    if ( fseek( pFile, nPtr, SeekSet) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
        return 0;
    }

    return (unsigned int)nSize;
}


unsigned long FileWriterWriteFunc   (Stream  pStream, const BYTE *pBuffer, unsigned int nSize )
{
    FILE* pFile = (FILE*)pStream->pAttr;;
    unsigned int nRet = fwrite( pBuffer, 1, nSize, pFile);

    if ( nRet != nSize) 
	{
        return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
    }
	::fflush( pFile );

    return OK;
}

void          FileStreamFreeFunc    (Stream  pStream)
{
	FILE* pFile = (FILE*)pStream->pAttr;

    if ( pFile )
        fclose( pFile );

	pStream->pAttr = NULL;
}




unsigned long TempFileStreamWriteFunc(Stream  pStream, const BYTE *pBuffer, unsigned int nSize )
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	FILE *pFile = ::_wfopen( pAttr->wsFilePath, _T("rb+") );
	if ( !pFile )
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno );
	}

	// Смещаемся к текущей позиции для записи
	::fseek( pFile, pAttr->nWriterPos, SEEK_SET );

	unsigned int nRet = ::fwrite( pBuffer, 1, nSize, pFile );

    if ( nRet != nSize) 
	{
		int nErrorNo = SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
		::fclose( pFile );
        return nErrorNo;
    }

	// Запоминаем новую текущую позицию для записи
	pAttr->nWriterPos = ::ftell( pFile );
	pAttr->nCurPos    = pAttr->nWriterPos;
	::fclose( pFile );

    return OK;
}
unsigned long TempFileStreamReadFunc(Stream  pStream, BYTE *pBuffer, unsigned int *pnSize)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	FILE *pFile = ::_wfopen( pAttr->wsFilePath, _T("rb+") );
	if ( !pFile )
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno );
	}

	// Смещаемся к текущей позиции для чтения
	::fseek( pFile, pAttr->nReaderPos, SEEK_SET );

	unsigned int nRSize = 0;

    UtilsMemSet( pBuffer, 0, *pnSize );
    
	nRSize = fread( pBuffer, 1, *pnSize, pFile );

    if ( nRSize != *pnSize ) 
	{
        if ( feof( pFile ) ) 
		{

            *pnSize = nRSize;
			::fclose( pFile );
            return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
        }

		::fclose( pFile );
		return SetError (pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, errno );
    }

	// Запоминаем новую текущую позицию для чтения
	pAttr->nReaderPos = ::ftell( pFile );
	pAttr->nCurPos    = pAttr->nReaderPos;
	::fclose( pFile );

    return OK;

}
unsigned long TempFileStreamSeekFunc(Stream  pStream, int nPos, WhenceMode  eMode)
{
	// Этой функцией лучше не пользоваться (лучше отдельно использовать TempFileStreamReaderSeek и TempFileStreamWriterSeek)
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;


	if ( SeekSet == eMode )
	{
		if ( pAttr->nCurPos == pAttr->nReaderPos )
			pAttr->nReaderPos = nPos;
		else if ( pAttr->nCurPos == pAttr->nWriterPos )
			pAttr->nWriterPos = nPos;

		pAttr->nCurPos = nPos;
	}
	else if ( SeekEnd == eMode )
	{
		unsigned int nSize = pStream->pSizeFn( pStream );
		int nSetPos = nSize - nPos;

		if ( pAttr->nCurPos == pAttr->nReaderPos )
			pAttr->nReaderPos = nSetPos;
		else if ( pAttr->nCurPos == pAttr->nWriterPos )
			pAttr->nWriterPos = nSetPos;

		pAttr->nCurPos = nSetPos;
	}
	else if ( SeekCur == eMode )
	{
		if ( pAttr->nCurPos == pAttr->nReaderPos )
			pAttr->nReaderPos += nPos;
		else if ( pAttr->nCurPos == pAttr->nWriterPos )
			pAttr->nWriterPos += nPos;

		pAttr->nCurPos += nPos;
	}

    return OK;
}




int           TempFileStreamTellFunc(Stream  pStream)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	return pAttr->nCurPos;
}


unsigned int  TempFileStreamSizeFunc(Stream  pStream)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	FILE *pFile = ::_wfopen( pAttr->wsFilePath, _T("rb+") );
	if ( !pFile )
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno );
	}

    int nSize = 0;
    int nPtr = 0;

    // сохраняем текущий указатель файла
    if ( ( nPtr = ftell( pFile ) ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
		::fclose( pFile );
        return 0;
    }

    // двигаем указатель в конец файла 
    if ( fseek( pFile, 0, SeekEnd ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
		::fclose( pFile );
        return 0;
    }

    // получаем указатель конца файла
    if ( ( nSize = ftell( pFile ) ) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
		::fclose( pFile );
        return 0;
    }

    // воостанавливаем текущий указатель файла
    if ( fseek( pFile, nPtr, SeekSet) < 0 ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, ferror( pFile ) );
		::fclose( pFile );
        return 0;
    }

	::fclose( pFile );
    return (unsigned int)nSize;
}


void          TempFileStreamFreeFunc(Stream  pStream)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;
	::_wunlink( pAttr->wsFilePath );
	FreeMem( pStream->oMMgr, pAttr );
	pStream->pAttr = NULL;
}
unsigned long TempFileStreamReaderSeek(Stream pStream, int nPos, WhenceMode eMode)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	FILE *pFile = ::_wfopen( pAttr->wsFilePath, _T("rb+") );
	if ( !pFile )
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno );
	}

	if ( 0 != ::fseek( pFile, pAttr->nReaderPos, SeekSet ) )
	{
		::fclose( pFile );
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, errno );
	}

	int nWhence = 0;

    switch ( eMode ) 
	{
        case SeekCur:
            nWhence = SeekCur;
            break;
        case SeekEnd:
            nWhence = SeekEnd;
            break;
        default:
            nWhence = SeekSet;
    }

	if ( 0 != ::fseek( pFile, nPos, nWhence ) ) 
	{
		::fclose( pFile );
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, errno );
    }

	int nCurPos = ::ftell( pFile );
	pAttr->nReaderPos  = nCurPos;
	pAttr->nCurPos     = nCurPos;

	::fclose( pFile );
	return OK;
}
unsigned long TempFileStreamWriterSeek(Stream pStream, int nPos, WhenceMode eMode)
{
	TempFileStreamAttr pAttr = (TempFileStreamAttr)pStream->pAttr;

	FILE *pFile = ::_wfopen( pAttr->wsFilePath, _T("rb+") );
	if ( !pFile )
	{
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno );
	}

	if ( 0 != ::fseek( pFile, pAttr->nWriterPos, SeekSet ) )
	{
		::fclose( pFile );
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, errno );
	}

	int nWhence = 0;

    switch ( eMode ) 
	{
        case SeekCur:
            nWhence = SeekCur;
            break;
        case SeekEnd:
            nWhence = SeekEnd;
            break;
        default:
            nWhence = SeekSet;
    }

	if ( 0 != ::fseek( pFile, nPos, nWhence ) ) 
	{
		::fclose( pFile );
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_IO_ERROR, errno );
    }

	int nCurPos = ::ftell( pFile );
	pAttr->nWriterPos  = nCurPos;
	pAttr->nCurPos     = nCurPos;

	::fclose( pFile );
	return OK;
}
// main functions

Stream        MemStreamNew          (MMgr    oMMgr, unsigned int nBufSize)
{
    Stream pStream;

    // Создаем новый объект Stream 
    pStream = (Stream)GetMem( oMMgr, sizeof( StreamRec) );

    if ( pStream ) 
	{
        // Создаем струкутуру атрибутов
        MemStreamAttr pAttr = (MemStreamAttr)GetMem( oMMgr, sizeof(MemStreamAttrRec) );

        if ( !pAttr ) 
		{
            FreeMem( oMMgr, pStream);
            return NULL;
        }

        UtilsMemSet( pStream, 0, sizeof(StreamRec) );
        UtilsMemSet( pAttr, 0, sizeof(MemStreamAttrRec) );

		pAttr->pBuf = ListNew( oMMgr, DEF_ITEMS_PER_BLOCK );
        
		if ( !pAttr->pBuf ) 
		{
            FreeMem( oMMgr, pStream );
            FreeMem( oMMgr, pAttr );
            return NULL;
        }

		pStream->nSigBytes = STREAM_SIG_BYTES;
		pStream->eType     = StreamMemory;
		pStream->oError    = oMMgr->oError;
		pStream->oMMgr     = oMMgr;
		pStream->pAttr     = pAttr;
		pAttr->nBufSize = (nBufSize > 0) ? nBufSize : STREAM_BUF_SIZ;
		pAttr->nWPos    = pAttr->nBufSize;

		pStream->pWriteFn = MemStreamWriteFunc;
		pStream->pReadFn  = (StreamReadFunc)MemStreamReadFunc;
		pStream->pSeekFn  = MemStreamSeekFunc;
		pStream->pTellFn  = MemStreamTellFunc;
		pStream->pSizeFn  = MemStreamSizeFunc;
		pStream->pFreeFn  = MemStreamFreeFunc;
    }

    return pStream;
}

unsigned int  MemStreamGetBufSize   (Stream  pStream)
{
	if ( !pStream || StreamMemory != pStream->eType )
        return 0;

	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;
	return pAttr->nBufSize;
}

unsigned int  MemStreamGetBufCount  (Stream  pStream)
{
	if ( !pStream || StreamMemory != pStream->eType )
        return 0;

	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;

	return pAttr->pBuf->nCount;
}


unsigned long MemStreamRewrite      (Stream  pStream, BYTE *pBuf, unsigned int nSize)
{
	MemStreamAttr pAttr = (MemStreamAttr)pStream->pAttr;
    unsigned int nBufSize = 0;
    unsigned int nRLen = nSize;

	while ( nRLen > 0 ) 
	{
        unsigned int nTmpLen = 0;

		if ( pAttr->pBuf->nCount <= pAttr->nRPtrIndex ) 
		{
            unsigned long nRet = MemStreamWriteFunc( pStream, pBuf, nRLen );
			pAttr->nRPtrIndex = pAttr->pBuf->nCount;
			pAttr->nRPos = pAttr->nWPos;
			pAttr->pRPtr = pAttr->pWPtr;
            return nRet;
        } 
		else if ( pAttr->pBuf->nCount == pAttr->nRPtrIndex )
			nTmpLen = pAttr->nWPos - pAttr->nRPos;
        else
			nTmpLen = pAttr->nBufSize - pAttr->nRPos;

        if ( nTmpLen >= nRLen ) 
		{
			UtilsMemCpy( pAttr->pRPtr, pBuf, nRLen );
			pAttr->nRPos += nRLen;
			pAttr->pRPtr += nRLen;
            return OK;
        } 
		else 
		{
			UtilsMemCpy( pAttr->pRPtr, pBuf, nTmpLen );
            pBuf  += nTmpLen;
            nRLen -= nTmpLen;
			pAttr->nRPtrIndex++;

			if ( pAttr->pBuf->nCount > pAttr->nRPtrIndex ) 
			{
				pAttr->nRPos = 0;
				pAttr->pRPtr = MemStreamGetBufPtr( pStream, pAttr->nRPtrIndex, &nBufSize );
            }
        }
    }
    return OK;
}

unsigned int  StreamSize            (Stream  pStream)
{
	if ( pStream->pWriteFn )
		return pStream->nSize;

	if ( !( pStream->pSizeFn ) ) 
	{
		SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);
        return 0;
    }

	if ( 0 != ErrorGetCode(pStream->oError) )
        return 0;

	return pStream->pSizeFn( pStream );
}

unsigned long StreamSeek            (Stream  pStream, int nPos, WhenceMode  eMode)
{
    if ( !( pStream->pSeekFn ) )
		return SetError ( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

	if ( 0 != ErrorGetCode( pStream->oError ) )
        return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;

	return pStream->pSeekFn( pStream, nPos, eMode);
}


/*
 *  StreamRead
 *
 *  pStream : Указатель на объект Stream.
 *  pBuffer : Указатель на буфер данных.
 *  pnSize  : Указатель на переменную, которая хранит размер буфера.
 *
 *  Возращает OK в случае успеха. В противном случае, возвращает код
 *  ошибки, который возвращает функция чтения данного потока.
 *
 */
unsigned long StreamRead            (Stream  pStream, BYTE *pBuffer, unsigned int *pnSize)
{
    if ( !( pStream->pReadFn ) )
        return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

    //
    //if ( ErrorGetCode( pStream->oError) != NOERROR )
    //    return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;
    //

	return pStream->pReadFn( pStream, pBuffer, pnSize );
}


/*
 *  StreamReadLn
 *
 *  pStream : Указатель на объект Stream.
 *  sBuffer : Указатель на буфер данных.
 *  pnSize  : Указатель на переменную, которая хранит размер буфера.
 *
 *  Читаем из потока, до тех пор пока либо буфер не закончится, либо 
 *  не наткнемся на перенос строки.
 *
 */
unsigned long StreamReadLn          (Stream  pStream, char *sBuffer, unsigned int *nSize)
{
    char sBuf[STREAM_BUF_SIZ];

    unsigned int nRSize = *nSize;
    unsigned int nReadSize = STREAM_BUF_SIZ;

    if ( !pStream || !sBuffer || 0 == *nSize )
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0 );

	if ( !( pStream->pSeekFn ) || !( pStream->pReadFn ) )
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0 );

    if ( nRSize < STREAM_BUF_SIZ )
        nReadSize = nRSize;

    *nSize = 0;

    while ( nRSize > 1 ) 
	{
        char *pBuf = sBuf;

        unsigned long nRet = StreamRead( pStream, (BYTE*)sBuf, &nReadSize );

        if ( OK != nRet && 0 == nReadSize )
            return nRet;

        nRSize -= nReadSize;

        while (nReadSize > 0) 
		{
            if ( *pBuf == 0x0A || *pBuf == 0x0D ) 
			{
                *sBuffer = 0;
                nReadSize--;

                // отлавливаем переносы строк 
                if (*pBuf == 0x0D || nReadSize > 1) 
				{
                    pBuf++;

                    if (*pBuf == 0x0A)
                        nReadSize--;
                }

                if (nReadSize > 0)
                    return StreamSeek( pStream, 0 - nReadSize, SeekCur);
                else
                    return OK;
            }

            *sBuffer++ = *pBuf++;
            nReadSize--;
            (*nSize)++;
        }

        if (nRSize < STREAM_BUF_SIZ)
            nReadSize = nRSize;
        else
            nReadSize = STREAM_BUF_SIZ;

        if (nRet == AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF)
            return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
    }

    *sBuffer = 0;

    return AVS_OFFICEPDFWRITER_ERROR_STREAM_READLN_CONTINUE;
}


unsigned long StreamReadULong       (Stream  pStream, unsigned int *pnValue)
{
	unsigned long nRet = OK;

	int nChar0 = 0, nChar1 = 0, nChar2 = 0, nChar3 = 0;
	unsigned int unReadLen = 1;

	if ( OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar0, &unReadLen ) ) || OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar1, &unReadLen ) ) ||
		 OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar2, &unReadLen ) ) || OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar3, &unReadLen ) ) )  
	{
		return nRet;
	}

	if ( nChar0 == EOF || nChar1 == EOF || nChar2 == EOF || nChar3 == EOF ) 
	{
		return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
	}
	*pnValue = (unsigned int)((nChar0 << 24) | (nChar1 << 16) | (nChar2 << 8) | nChar3);

	return OK;
}
unsigned long StreamReadUByte       (Stream  pStream, unsigned int *pnValue) 
{
	unsigned long nRet = OK;
	unsigned int nReadLen = 1;
	int nChar0 = 0;

	if ( OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar0, &nReadLen ) ) )
	{
		return nRet;
	}

	if ( nChar0 == EOF ) 
	{
		return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
	}
	*pnValue = (unsigned int)nChar0;
	return OK;
}
unsigned long StreamReadByte        (Stream  pStream, int *pnValue) 
{
	unsigned long nRet = OK;
	unsigned int nReadLen = 1;

	int nChar = 0;

	if ( OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar, &nReadLen ) ) )
	{
		return nRet;
	}

	if ( nChar == EOF ) 
	{
		return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
	}
	
	*pnValue = nChar;
	if ( nChar & 0x80 ) 
	{
		*pnValue |= -1 - 0xff;
	}
	return OK;
}
unsigned long StreamReadUShort      (Stream  pStream, unsigned int *pnValue) 
{
	unsigned long nRet = OK;
	unsigned int nReadLen = 1;
	int nChar0 = 0, nChar1 = 0;

	if ( OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar0, &nReadLen ) ) || OK != ( nRet = StreamRead( pStream, (BYTE *)&nChar1, &nReadLen ) ) )
	{
		return nRet;
	}

	if ( nChar0 == EOF || nChar1 == EOF ) 
	{
		return AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF;
	}
	*pnValue = (unsigned int)((nChar0 << 8) | nChar1);
	return OK;
}


/*
 * StreamWrite
 *
 *  pStream : Указатель на объект Stream.
 *  pBuffer : Указатель на буфер для записи.
 *  nSize   : Размер буфера для записи.
 *
 *  Возращает OK в случае успеха. В противном случае, возвращает код
 *  ошибки, который возвращает функция записи данного потока.
 *
 */
unsigned long StreamWrite           (Stream  pStream, const BYTE *pBuffer, unsigned int nSize, bool bCalcCheckSum = false)
{
    unsigned long nRet = 0;

	if ( !( pStream->pWriteFn ) )
		return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

 	//if ( NOERROR != ErrorGetCode( pStream->oError ) )
	//    return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;
 
	nRet = pStream->pWriteFn( pStream, pBuffer, nSize );

    if ( OK != nRet )
        return nRet;

	pStream->nSize += nSize;

	if ( bCalcCheckSum )
	{
		CRC32 oCRC;
		oCRC.ProcessCRC( (void*)pBuffer, nSize );
		pStream->nCheckSum = oCRC.m_nCRC32;
	}

    return OK;
}


unsigned long StreamWriteToStreamWithDeflate( Stream  pSrc, Stream  pDst, Encrypt pEncrypt)
{
#ifndef NOZLIB

#define DEFLATE_BUF_SIZ  ((int)(STREAM_BUF_SIZ * 1.1) + 13)

    unsigned long nRet = OK;
    BOOL bFlag = FALSE;

    z_stream pZStream;
    Bytef inbuf[STREAM_BUF_SIZ];
    Bytef otbuf[DEFLATE_BUF_SIZ];
    BYTE ebuf[DEFLATE_BUF_SIZ];

    /* initialize input stream */
    nRet = StreamSeek (pSrc, 0, SeekSet);
    if ( OK != nRet )
        return nRet;

    /* initialize decompression stream. */
    UtilsMemSet( &pZStream, 0x00, sizeof(z_stream) );
    pZStream.next_out  = otbuf;
    pZStream.avail_out = DEFLATE_BUF_SIZ;

    nRet = deflateInit_(&pZStream, Z_DEFAULT_COMPRESSION, ZLIB_VERSION, sizeof(z_stream) );
    if ( Z_OK != nRet )
		return SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR, nRet );

    pZStream.next_in  = inbuf;
    pZStream.avail_in = 0;

    for (;;) 
	{
        unsigned int nSize = STREAM_BUF_SIZ;

        nRet = StreamRead( pSrc, inbuf, &nSize);

        pZStream.next_in  = inbuf;
        pZStream.avail_in = nSize;

        if ( OK != nRet ) 
		{
            if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
			{
                bFlag = TRUE;
                if ( 0 == nSize )
                    break;
            } 
			else 
			{
                deflateEnd(&pZStream);
                return nRet;
            }
        }

        while ( pZStream.avail_in > 0 ) 
		{
            nRet = deflate(&pZStream, Z_NO_FLUSH);
            if ( Z_OK != nRet && Z_STREAM_END != nRet ) 
			{
                deflateEnd(&pZStream);
				return SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR, nRet );
            }

            if (pZStream.avail_out == 0) 
			{
                if ( pEncrypt ) 
				{
                    EncryptCryptBuf( pEncrypt, otbuf, ebuf, DEFLATE_BUF_SIZ);
                    nRet = StreamWrite( pDst, ebuf, DEFLATE_BUF_SIZ);
                }
				else
                    nRet = StreamWrite( pDst, otbuf, DEFLATE_BUF_SIZ);
                if ( OK != nRet ) 
				{
                    deflateEnd(&pZStream);
					return SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR, nRet );
                }

                pZStream.next_out  = otbuf;
                pZStream.avail_out = DEFLATE_BUF_SIZ;
            }
        }

        if ( bFlag )
            break;
    }

    bFlag = FALSE;
    for (;;) 
	{
        nRet = deflate(&pZStream, Z_FINISH );
        if ( Z_OK != nRet && Z_STREAM_END != nRet ) 
		{
            deflateEnd(&pZStream);
			return SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR, nRet);
        }

        if ( Z_STREAM_END == nRet )
            bFlag = TRUE;

        if ( pZStream.avail_out < DEFLATE_BUF_SIZ ) 
		{
            unsigned int osize = DEFLATE_BUF_SIZ - pZStream.avail_out;
            if ( pEncrypt ) 
			{
                EncryptCryptBuf( pEncrypt, otbuf, ebuf, osize);
                nRet = StreamWrite( pDst, ebuf, osize);
            } 
			else
                nRet = StreamWrite( pDst, otbuf, osize);
            if ( OK != nRet )
			{
                deflateEnd(&pZStream);
				return SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_ZLIB_ERROR, nRet );
            }

            pZStream.next_out  = otbuf;
            pZStream.avail_out = DEFLATE_BUF_SIZ;
        }

        if ( bFlag )
            break;
    }

    deflateEnd(&pZStream);
    return OK;
#else /* NOZLIB */
    return AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_FUNC;
#endif /* NOZLIB */
}


unsigned long StreamWriteToStream   (Stream  pSrc, Stream pDst, unsigned int nFilter, Encrypt pEncrypt)
{
	if ( 0 == StreamSize( pSrc ) )
        return OK;

	unsigned long nRet = 0;
    
	BYTE pBuf[STREAM_BUF_SIZ];
    BYTE pEBuf[STREAM_BUF_SIZ];
    
	BOOL bFlag = FALSE;

	if ( !pDst || ! ( pDst->pWriteFn ) ) 
	{
		SetError( pSrc->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT, 0);
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_OBJECT;
    }

	if ( NOERROR != ErrorGetCode( pSrc->oError ) || NOERROR != ErrorGetCode( pDst->oError ) )
        return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;

#ifndef NOZLIB
#ifndef _DEBUG

    if ( nFilter & STREAM_FILTER_FLATE_DECODE)
        return StreamWriteToStreamWithDeflate( pSrc, pDst, pEncrypt);
#endif
#endif /* NOZLIB */

    // инициализируем входящий поток 
    
    nRet = StreamSeek( pSrc, 0, SeekSet );
    if ( OK != nRet )
        return nRet;

    bFlag = FALSE;
    for (;;) 
	{
        unsigned int nSize = STREAM_BUF_SIZ;

        nRet = StreamRead( pSrc, pBuf, &nSize );

        if ( OK != nRet ) 
		{
            if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
			{
                bFlag = TRUE;
                if ( 0 == nSize )
                    break;
            } 
			else 
			{
                return nRet;
            }
        }

        if ( pEncrypt ) 
		{
            EncryptCryptBuf( pEncrypt, pBuf, pEBuf, nSize);
            nRet = StreamWrite( pDst, pEBuf, nSize);
        } 
		else 
		{
            nRet = StreamWrite( pDst, pBuf, nSize);
        }

        if ( OK != nRet )
            return nRet;

        if ( bFlag )
            break;
    }

    return OK;
}

//unsigned long StreamWriteToStream   (Stream  pSrc, Stream pDst, unsigned int nFilter/*, Encrypt  e*/)
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
Stream        FileReaderNew         (MMgr    oMMgr, const wchar_t *wsFileName)
{
    Stream pStream;
    
	FILE* pFile = _wfopen( wsFileName, _T("rb") );

    if ( !pFile ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno);
        return NULL;
    }

    pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec) );

    if ( pStream ) 
	{
        UtilsMemSet( pStream, 0, sizeof(StreamRec) );
		pStream->nSigBytes = STREAM_SIG_BYTES;
		pStream->eType     = StreamFile;
		pStream->oError    = oMMgr->oError;
		pStream->oMMgr     = oMMgr;
		pStream->pReadFn   = FileReaderReadFunc;
		pStream->pSeekFn   = FileReaderSeekFunc;
		pStream->pTellFn   = FileStreamTellFunc;
		pStream->pSizeFn   = FileStreamSizeFunc;
		pStream->pFreeFn   = FileStreamFreeFunc;
		pStream->pAttr     = pFile;
    }

    return pStream;
}

Stream        FileWriterNew         (MMgr    oMMgr, const wchar_t *wsFileName)
{
    Stream pStream;
    
	FILE* pFile = _wfopen( wsFileName, _T("wb") );

    if ( !pFile ) 
	{
		SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno);
        return NULL;
    }

    pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec ) );

    if (pStream) 
	{
        UtilsMemSet( pStream, 0, sizeof(StreamRec) );
		pStream->nSigBytes = STREAM_SIG_BYTES;
		pStream->oError    = oMMgr->oError;
		pStream->oMMgr     = oMMgr;

		pStream->pReadFn   = FileReaderReadFunc;
		pStream->pWriteFn  = FileWriterWriteFunc;
		pStream->pFreeFn   = FileStreamFreeFunc;
		pStream->pTellFn   = FileStreamTellFunc;
		pStream->pAttr     = pFile;
		pStream->eType     = StreamFile;
    }

    return pStream;
}




Stream        TempFileStreamNew     (MMgr    oMMgr, const wchar_t *wsTempFilePath = NULL, unsigned int unCheckSum = 0)
{
	if ( NULL == wsTempFilePath )
	{
		CString wsTempFile;
		if ( 0 == DocGetTempFile( oMMgr->pDoc, &wsTempFile ) )
		{
			SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno);
			return NULL;
		}

		// Создаем временный файл (он создается с типом "wb+" как нам и надо)
		FILE *pFile = ::_wfopen( wsTempFile.GetBuffer(), _T("wb+") );
		if ( !pFile ) 
		{
			SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno);
			return NULL;
		}
		// Закрываем файл, мы будем хранить путь к файлу, а не указатель на структуру pFile
		::fclose( pFile );

		Stream pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec ) );
		if ( pStream ) 
		{
			TempFileStreamAttr pAttr = (TempFileStreamAttr)GetMem( oMMgr, sizeof( TempFileStreamAttrRec ) );
			if ( !pAttr )
			{
				FreeMem( oMMgr, pStream );
				return NULL;
			}

			UtilsMemSet( pStream, 0, sizeof(StreamRec) );
			UtilsMemSet( pAttr, 0, sizeof(TempFileStreamAttrRec) );

			pAttr->nCurPos    = 0;
			pAttr->nReaderPos = 0;
			pAttr->nWriterPos = 0;
			wcscpy( pAttr->wsFilePath, wsTempFile.GetBuffer() );

			pStream->nSigBytes = STREAM_SIG_BYTES;
			pStream->eType     = StreamTempFile;
			pStream->oError    = oMMgr->oError;
			pStream->oMMgr     = oMMgr;
			pStream->pAttr     = pAttr;

			pStream->pWriteFn = TempFileStreamWriteFunc;
			pStream->pReadFn  = TempFileStreamReadFunc;
			pStream->pSeekFn  = TempFileStreamSeekFunc;
			pStream->pTellFn  = TempFileStreamTellFunc;
			pStream->pSizeFn  = TempFileStreamSizeFunc;
			pStream->pFreeFn  = TempFileStreamFreeFunc;
		}
		else
		{
			::_wunlink( wsTempFile.GetBuffer() );
		}

		return pStream;
	}
	else
	{
		// Проверяем существует ли файл
		FILE *pFile = ::_wfopen( wsTempFilePath, _T("rb") );
		if ( !pFile ) 
		{
			SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_OPEN_ERROR, errno);
			return NULL;
		}

		// определяем размер файла
		::fseek( pFile, 0, SEEK_END );
		long lFileSize = ::ftell( pFile );
		// Закрываем файл, мы будем хранить путь к файлу, а не указатель на структуру pFile
		::fclose( pFile );

		Stream pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec ) );
		if ( pStream ) 
		{
			TempFileStreamAttr pAttr = (TempFileStreamAttr)GetMem( oMMgr, sizeof( TempFileStreamAttrRec ) );
			if ( !pAttr )
			{
				FreeMem( oMMgr, pStream );
				return NULL;
			}

			UtilsMemSet( pStream, 0, sizeof(StreamRec) );
			UtilsMemSet( pAttr, 0, sizeof(TempFileStreamAttrRec) );

			pAttr->nCurPos    = 0;
			pAttr->nReaderPos = 0;
			pAttr->nWriterPos = 0;
			wcscpy( pAttr->wsFilePath, wsTempFilePath );

			pStream->nSigBytes = STREAM_SIG_BYTES;
			pStream->eType     = StreamTempFile;
			pStream->oError    = oMMgr->oError;
			pStream->oMMgr     = oMMgr;
			pStream->pAttr     = pAttr;

			pStream->pWriteFn = TempFileStreamWriteFunc;
			pStream->pReadFn  = TempFileStreamReadFunc;
			pStream->pSeekFn  = TempFileStreamSeekFunc;
			pStream->pTellFn  = TempFileStreamTellFunc;
			pStream->pSizeFn  = TempFileStreamSizeFunc;
			pStream->pFreeFn  = TempFileStreamFreeFunc;

			pStream->nSize     = lFileSize;
			pStream->nCheckSum = unCheckSum;
		}
		else
		{
			::_wunlink( wsTempFilePath );
		}

		return pStream;
	}
}
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
 *  return: В случае успеха возвращает указатель на новый объект Stream,
 *          в случае неудачи, возвращает NULL.
 *
 */
Stream        CallbackReaderNew     (MMgr    oMMgr, StreamReadFunc  pReadFn, StreamSeekFunc  pSeekFn, StreamTellFunc pTellFn, StreamSizeFunc pSizeFn, void* pData)
{
    Stream pStream;

    pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec) );

    if ( pStream ) 
	{
        UtilsMemSet( pStream, 0, sizeof(StreamRec) );
        pStream->nSigBytes = STREAM_SIG_BYTES;
		pStream->oError    = oMMgr->oError;
        pStream->oMMgr     = oMMgr;
        pStream->pReadFn   = pReadFn;
        pStream->pSeekFn   = pSeekFn;
        pStream->pTellFn   = pTellFn;
        pStream->pSizeFn   = pSizeFn;
        pStream->pAttr     = pData;
        pStream->eType     = StreamCallBack;
    }

    return pStream;
}

/*
 *  CallbackWriterNew
 *
 *  Конструктор для CallbackWriter.
 *
 *  oMMgr    : Указатель на объект MMgr.
 *  pWriteFn : Указатель на функию пользователя для записи данных.
 *  pData    : Указатель на данные определенные пользователем.
 *
 *  return: В случае успеха возвращает указатель на новый объект Stream,
 *          в случае неудачи, возвращает NULL.
 *
 */
Stream        CallbackWriterNew     (MMgr    oMMgr, StreamWriteFunc pWriteFn, void* pData)
{
    Stream pStream;

    pStream = (Stream)GetMem( oMMgr, sizeof(StreamRec) );

    if ( pStream ) 
	{
        UtilsMemSet( pStream, 0, sizeof(StreamRec) );
		pStream->nSigBytes = STREAM_SIG_BYTES;
		pStream->oError    = oMMgr->oError;
		pStream->oMMgr     = oMMgr;
		pStream->pWriteFn  = pWriteFn;
		pStream->pAttr     = pData;
		pStream->eType     = StreamCallBack;
    }

    return pStream;
}





void          StreamFree            (Stream  pStream)
{
    if (! pStream )
        return;

    if ( pStream->pFreeFn )
        pStream->pFreeFn( pStream );

	pStream->nSigBytes = 0;

	FreeMem( pStream->oMMgr, pStream );
}

unsigned long StreamWriteChar       (Stream  pStream, char cValue)
{
    return StreamWrite( pStream, (BYTE*)&cValue, sizeof(char) );
}




unsigned long StreamWriteStr        (Stream  pStream, const char *sValue)
{
    unsigned int nLen = UtilsStrLen( sValue, -1 );

    return StreamWrite( pStream, (BYTE*)sValue, nLen );
}



unsigned long StreamWriteUChar      (Stream  pStream, BYTE nValue)
{
    return StreamWrite( pStream, &nValue, sizeof(BYTE) );
}

unsigned long StreamWriteInt        (Stream  pStream, int nValue)
{
    char sBuf[INT_LEN + 1];

    char* cCh = UtilsIToA( sBuf, nValue, sBuf + INT_LEN );

    return StreamWrite( pStream, (BYTE*)sBuf, (unsigned int)(cCh - sBuf));
}

unsigned long StreamWriteUInt       (Stream  pStream, unsigned int nValue)
{
    return StreamWriteInt( pStream, (int)nValue);
}

unsigned long StreamWriteHex        (Stream  pStream, int nValue, int nLen)
{
	// Пишем заданное число в шестандцатиричной форме, параметр nLen отвечает за длину числа, (т.е. если надо дописываем нули перед числом)
	char sBuffer[10];
	if ( 2 == nLen )
		_snprintf( sBuffer, sizeof(sBuffer), "%.2X", nValue );
	else // 4 == nLen
		_snprintf( sBuffer, sizeof(sBuffer), "%.4X", nValue );
	return StreamWriteStr( pStream, sBuffer );
}

unsigned long StreamWriteReal       (Stream  pStream, float fValue)
{
    char sBuf[REAL_LEN + 1];

    char* cCh = UtilsFToA( sBuf, fValue, sBuf + REAL_LEN);

    return StreamWrite( pStream, (BYTE*)sBuf, (unsigned int)( cCh - sBuf ) );
}

int           StreamTell            (Stream  pStream)
{
    if ( !( pStream->pTellFn ) )
        return SetError( pStream->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0 );

    if ( 0 != ErrorGetCode(pStream->oError) )
        return AVS_OFFICEPDFWRITER_ERROR_THIS_FUNC_WAS_SKIPPED;

    return pStream->pTellFn( pStream );
}


BOOL          StreamEOF             (Stream  pStream);
unsigned long StreamFlush           (Stream  pStream);
unsigned long StreamWriteEscapeName (Stream  pStream, const char *sValue)
{
    char sTmpChar[LIMIT_MAX_NAME_LEN * 3 + 2];

	unsigned int nLen = 0;

    const BYTE* pPos1;
          char* pPos2;

    nLen = UtilsStrLen( sValue, LIMIT_MAX_NAME_LEN );
    pPos1 = (BYTE*)sValue;
    pPos2 = sTmpChar;

    *pPos2++ = '/';

    for ( int nIndex = 0; nIndex < (int)nLen; nIndex++) 
	{
        BYTE nChar = *pPos1++;
        if ( NEEDS_ESCAPE( nChar ) ) 
		{
            *pPos2++ = '#';
            *pPos2 = (char)(nChar >> 4);

            if ( *pPos2 <= 9 )
                *pPos2 += 0x30;
            else
                *pPos2 += 0x41 - 10;

            pPos2++;

            *pPos2 = (char)(nChar & 0x0f);

            if ( *pPos2 <= 9 )
                *pPos2 += 0x30;
            else
                *pPos2 += 0x41 - 10;

            pPos2++;
        } 
		else
            *pPos2++ = nChar;
    }

    *pPos2 = 0;

    return StreamWrite( pStream, (BYTE*)sTmpChar, UtilsStrLen( sTmpChar, -1 ) );
}

unsigned long StreamWriteEscapeText2(Stream  pStream, const BYTE *sText, unsigned int nLen)
{
    char sBuf[TEXT_DEFAULT_LEN];

	unsigned int nIndex = 0;

    const BYTE* sTxt = sText;
   unsigned long nRet = 0;

    if ( !nLen )
        return OK;

    sBuf[nIndex++] = '(';

    for (int nCounter = 0; nCounter < nLen; nCounter++) 
	{
        BYTE nChar = (BYTE)*sTxt++;
        
		if ( NEEDS_ESCAPE(nChar) ) 
		{
            sBuf[nIndex++] = '\\';

            sBuf[nIndex]    = nChar >> 6;
            sBuf[nIndex]   += 0x30;
            nIndex++;
            sBuf[nIndex]    = (nChar & 0x38) >> 3;
            sBuf[nIndex]   += 0x30;
            nIndex++;
            sBuf[nIndex]    = (nChar & 0x07);
            sBuf[nIndex]   += 0x30;
            nIndex++;
        }
        else
            sBuf[nIndex++] = nChar;

        if ( nIndex > TEXT_DEFAULT_LEN - 4) 
		{
            nRet = StreamWrite( pStream, (BYTE*)sBuf, nIndex );
            if ( OK != nRet )
                return nRet;
            nIndex = 0;
        }
    }
    sBuf[nIndex++] = ')';

    nRet = StreamWrite( pStream, (BYTE*)sBuf, nIndex );

    return nRet;
}

unsigned long StreamWriteEscapeText (Stream  pStream, const BYTE *sText, unsigned int nLen)
{
	if ( NULL == sText ) nLen = 0;

    return StreamWriteEscapeText2( pStream, sText, nLen );
}


unsigned long StreamWriteBinary     (Stream  pStream, const BYTE *pData, unsigned int nLen, Encrypt pEncrypt)
{
    char sBuf[TEXT_DEFAULT_LEN];

    BYTE pEBuf[TEXT_DEFAULT_LEN];
    BYTE *pBuf = NULL;
    BOOL bFlag = FALSE;
    unsigned int nIndex = 0;

	const BYTE* pBuffer = NULL;
    unsigned long nRet = OK;

    if ( pEncrypt ) 
	{
        if ( nLen <= TEXT_DEFAULT_LEN )
            pBuf = pEBuf;
        else 
		{
			pBuf = (BYTE *)GetMem( pStream->oMMgr, nLen);
            bFlag = TRUE;
        }

        EncryptCryptBuf( pEncrypt, pData, pBuf, nLen);
        pBuffer = pBuf;
    } 
	else 
	{
        pBuffer = pData;
    }

    for ( int nCounter = 0; nCounter < nLen; nCounter++, pBuffer++) 
	{
        char nChar = *pBuffer >> 4;

        if ( nChar <= 9 )
            nChar += 0x30;
        else
            nChar += 0x41 - 10;
        sBuf[nIndex++] = nChar;

        nChar = *pBuffer & 0x0f;
        if ( nChar <= 9 )
            nChar += 0x30;
        else
            nChar += 0x41 - 10;
        sBuf[nIndex++] = nChar;

        if (nIndex > TEXT_DEFAULT_LEN - 2) 
		{
            nRet = StreamWrite( pStream, (BYTE*)sBuf, nIndex );
            if ( OK != nRet ) 
			{
                if ( bFlag )
					FreeMem( pStream->oMMgr, pBuf);
                return nRet;
            }
            nIndex = 0;
        }
    }

    if ( nIndex > 0) 
	{
        nRet = StreamWrite( pStream, (BYTE*)sBuf, nIndex );
    }

    if ( bFlag )
        FreeMem( pStream->oMMgr, pBuf);

    return nRet;
}

unsigned long StreamValidate        (Stream  pStream)
{
    if ( !pStream || STREAM_SIG_BYTES != pStream->nSigBytes )
        return FALSE;
    else
        return TRUE;
}

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
