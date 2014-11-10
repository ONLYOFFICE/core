#ifndef _WMF_FILE_H
#define _WMF_FILE_H

#include <stdio.h>

#include "WmfTypes.h"
#include "WmfColor.h"
#include "WmfUtils.h"
#include "WmfRegion.h"
#include "WmfOutputDevice.h"
#include "WmfMemory.h"
#ifdef DESKTOP_EDITOR_GRAPHICS
#include "../../../../../DesktopEditor/fontengine/FontManager.h"
#include "../../../../../DesktopEditor/fontengine/ApplicationFonts.h"
#else
#include "..\..\ASCFontManager.h"
#include "..\..\Font\FontUtils.h"
#endif

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif

#ifdef _DEBUG
//#define DebugWriteXml
#endif

class CWmfFile
{
public:

	CWmfFile()
	{
		m_bWindow = false;
		m_eError = wmf_error_None;
		
		m_pOutput = NULL;

		m_pMetaHeader.pPlaceableMetaHeader = &(m_pPlaceableMetaHeader);
		m_pMetaHeader.pHeader              = &(m_pHead);
		m_pFile                            = &(m_pMetaHeader);

		m_pFile->pFileIn = NULL; 

		m_pBufferData = NULL;
		m_nBufferType = WMF_NOT_OPEN;

		WriteBegin("C:\\WmfTest.xml");

		m_oStore.pAttrlist = NULL;
        m_oStore.ulMax     = 0;
		m_oStore.ulCount   = 0;

		m_oColorData.SetErrorHandler( &m_eError );

		m_pPlayerData = (TWmfPlayer *)m_oMemoryManager.Malloc( sizeof (TWmfPlayer), _T( "Player" ) );
		if ( NULL == m_pPlayerData )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return;
		}
		::memset( m_pPlayerData, 0, sizeof(TWmfPlayer) );

		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;
		pPlayer->ulFlags = 0;

		m_pFontManager = NULL;
		m_bDeleteFontManager = true;
	}

	~CWmfFile()
	{
		WriteEnd();
		Close();

		if ( m_pBufferData )
			delete m_pBufferData;
#ifdef DESKTOP_EDITOR_GRAPHICS
		if(m_bDeleteFontManager)
			RELEASEOBJECT( m_pFontManager );
#else
		RELEASEINTERFACE( m_pFontManager );
#endif

		if ( m_pPlayerData )
		{
			m_oMemoryManager.Free( m_pPlayerData );
		}		
	}
#ifdef DESKTOP_EDITOR_GRAPHICS
	BOOL OpenFromFile(const wchar_t *wsFilePath)
#else
	BOOL OpenFromFile(wchar_t *wsFilePath)
#endif
	{
		if ( m_pBufferData )
			delete m_pBufferData;

		FILE *pFile = NULL;
#ifdef _WIN32
		if ( NULL == ( pFile = _wfopen( wsFilePath, _T("rb") ) ) )
#else
		std::string sFilePath = U_TO_UTF8(wsFilePath);
        if ( NULL == ( pFile = fopen( sFilePath.c_str(), "rb" ) ) )
#endif
		{
			m_eError = wmf_error_BadFile;
			return FALSE;
		}

		m_pBufferData = new TFileInfo;
		if ( !m_pBufferData )
		{
			m_eError = wmf_error_NotEnoughMemory;
			::fclose( pFile );
			return FALSE;
		}

		TFileInfo *pData = (TFileInfo *)m_pBufferData;
		pData->pFile = pFile;

		m_nBufferType = WMF_OPEN_FROM_FILE;

		return TRUE;
	}
#ifdef DESKTOP_EDITOR_GRAPHICS
	void SetFontManager(CFontManager* pManager = NULL)
	{
		if(m_bDeleteFontManager)
			RELEASEOBJECT(m_pFontManager);
		if (NULL == pManager)
		{
			m_pFontManager = new CFontManager();
			m_pFontManager->Initialize();
			m_bDeleteFontManager = true;
		}
		else
		{
			m_pFontManager = pManager;
			m_bDeleteFontManager = false;
		}
	}

	inline CFontManager* GetFontManager()
#else
	void SetFontManager(IASCFontManager* pManager = NULL)
	{
		if (NULL == pManager)
		{
			::CoCreateInstance( __uuidof(CASCFontManager), NULL, CLSCTX_INPROC, __uuidof(IASCFontManager), (void **)&m_pFontManager );

			if ( m_pFontManager )
			{
				m_pFontManager->Initialize( L"" );
				m_pFontManager->SetDefaultFont( L"Arial Unicode MS" );
			}
		}
		else
		{
			m_pFontManager = pManager;
			ADDREFINTERFACE(m_pFontManager);
		}
	}

	inline IASCFontManager* GetFontManager()
#endif
	{
		return m_pFontManager;
	}

	BOOL OpenFromMemory(unsigned char *pMemory, long lLength)
	{
		if ( m_pBufferData )
			delete m_pBufferData;

		if ( ( NULL == pMemory ) || ( lLength <= 0 ) )
		{	
			m_eError = wmf_error_BadFile;
			return FALSE;
		}


		m_pBufferData = new TMemoryInfo;
		if ( !m_pBufferData )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return FALSE;
		}

		TMemoryInfo *pData = (TMemoryInfo *)m_pBufferData;
		pData->pMemory  = pMemory;
		pData->pPointer = pMemory;

		pData->lPos    = 0;
		pData->lLength = lLength;

		m_nBufferType = WMF_OPEN_FROM_MEMORY;

		return TRUE;
	}


	void Close()
	{
		m_eError = wmf_error_None;

		if ( m_pPlayerData )
		{
			if ( m_pPlayerData->pObjects )
			{
				m_oMemoryManager.Free( m_pPlayerData->pObjects );
				m_pPlayerData->pObjects = NULL;
			}

			if ( m_pPlayerData->pParameters )
			{
				m_oMemoryManager.Free( m_pPlayerData->pParameters );
				m_pPlayerData->pParameters = NULL;
			}
		}


		if ( WMF_NOT_OPEN == m_nBufferType )
		{
			return;
		}
		else if ( WMF_OPEN_FROM_FILE == m_nBufferType )
		{
			TFileInfo *pData = (TFileInfo *)m_pBufferData;
			if ( !pData )
				return;

			::fclose( pData->pFile );

			delete m_pBufferData;
			m_pBufferData = NULL;
			m_nBufferType = WMF_NOT_OPEN;

			return;
		}
		else if ( WMF_OPEN_FROM_MEMORY == m_nBufferType )
		{
			TMemoryInfo *pData = (TMemoryInfo *)m_pBufferData;
			if ( !pData )
				return;

			delete m_pBufferData;
			m_pBufferData = NULL;
			m_nBufferType = WMF_NOT_OPEN;

			return;
		}
	}





	void SetOutputDevice(CWmfOutputDevice *pOutput)
	{
		m_pOutput = pOutput;
	}
	void Scan(TWmfRectF *pDeviceRect)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;

		if ( pPlayer->ulFlags & PLAYER_SCANNED )
		{
			return;
		}

        pPlayer->nDCStackMaxLen = 0;
		pPlayer->ppDCStack      = 0;

		pPlayer->pObjects		= NULL;
		pPlayer->pParameters	= NULL;
		pPlayer->pDC			= NULL;

		pPlayer->oDisplayTL.fX = 0;
		pPlayer->oDisplayTL.fY = 0;
		pPlayer->oDisplayBR.fX = 0;
		pPlayer->oDisplayBR.fY = 0;

		pPlayer->oBoundBR.fX = 0;
		pPlayer->oBoundBR.fY = 0;
		pPlayer->oBoundTL.fX = 0;
		pPlayer->oBoundTL.fY = 0;

		pDeviceRect->oTL = pPlayer->oDisplayTL;
		pDeviceRect->oBR = pPlayer->oDisplayBR;

		pPlayer->ulFlags &= ~PLAYER_PLAY; // Set Scan Mode

		ReadHeader();

		if ( CheckError() )
			return;

		if ( m_pFile->pHeader->ushNumberOfObjects > 0 )
		{
			pPlayer->pObjects = (TWmfObject *)m_oMemoryManager.Malloc( m_pFile->pHeader->ushNumberOfObjects * sizeof(TWmfObject), _T("Player->pObjects") );
			if ( NULL == pPlayer->pObjects )
			{
				m_eError = wmf_error_NotEnoughMemory;
				return;
			}
		}

        //pPlayer->pParameters = (unsigned char *)m_oMemoryManager.Malloc( (m_pFile->pHeader->unMaxRecordSize - 3) * 2 * sizeof(unsigned char) );
        pPlayer->pParameters = (unsigned char *)m_oMemoryManager.Malloc( m_pFile->pHeader->unMaxRecordSize * 2 * sizeof(unsigned char), _T("Player->pParamters") );

		if ( NULL == pPlayer->pParameters )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return;
		}

		PlayMetaFile();

		if ( CheckError() )
			return;

		pDeviceRect->oTL = pPlayer->oDisplayTL;
		pDeviceRect->oBR = pPlayer->oDisplayBR;

		pPlayer->ulFlags |= PLAYER_SCANNED;
	}


	void GetSize(float *pWidth, float *pHeight)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		if ( CheckError() )
			return;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_SCANNED ) )
		{
			m_eError = wmf_error_Internal;
			return;
		}

		//m_pFile->pPlaceableMetaHeader->shRight = 100;
		//m_pFile->pPlaceableMetaHeader->shBottom = 100;

		short shDefaultWidth  = abs( m_pFile->pPlaceableMetaHeader->shRight - m_pFile->pPlaceableMetaHeader->shLeft   );
		short shDefaultHeight = abs( m_pFile->pPlaceableMetaHeader->shTop   - m_pFile->pPlaceableMetaHeader->shBottom );

		if ( shDefaultWidth && shDefaultHeight )
		{	
			(*pWidth)  = (float)shDefaultWidth;
			(*pHeight) = (float)shDefaultHeight;
		}
		else
		{	
			(*pWidth)  = pPlayer->oDisplayBR.fX - pPlayer->oDisplayTL.fX;
			(*pHeight) = pPlayer->oDisplayBR.fY - pPlayer->oDisplayTL.fY;
		}
	}
	void GetDisplaySize(double *pdWidth, double *pdHeight, double dResX, double dResY)
	{	
		float fEstimatedWidth;
		float fEstimatedHeight;

		GetSize( &fEstimatedWidth, &fEstimatedHeight );

		if ( CheckError() )
			return;

		unsigned int unUnitsPerInch = 1440;

		if ( m_pFile->nPlaceable )
		{	
			unUnitsPerInch = m_pFile->pPlaceableMetaHeader->ushInch;
		}
		else if ( (fEstimatedWidth * fEstimatedHeight) < ( 1024 * 1024 ) )
		{	
			unUnitsPerInch = 72;
		}

		double dDisplayWidth  = ((double) fEstimatedWidth  ) * dResX / (double) unUnitsPerInch;
		double dDisplayHeight = ((double) fEstimatedHeight ) * dResY / (double) unUnitsPerInch;

		if ( pdWidth   ) 
			*pdWidth  = dDisplayWidth;

		if ( pdHeight  ) 
			*pdHeight = dDisplayHeight;
	}
	void GetDpi(double *pdDpiX, double *pdDpiY)
	{
		float fEstimatedWidth;
		float fEstimatedHeight;

		GetSize( &fEstimatedWidth, &fEstimatedHeight );

		if ( CheckError() )
			return;

		unsigned int unUnitsPerInch = 1440;

		if ( m_pFile->nPlaceable )
		{	
			unUnitsPerInch = m_pFile->pPlaceableMetaHeader->ushInch;
		}
		else if ( (fEstimatedWidth * fEstimatedHeight) < ( 1024 * 1024 ) )
		{	
			unUnitsPerInch = 72;
		}

		*pdDpiX = (double)unUnitsPerInch;
		*pdDpiY = (double)unUnitsPerInch;
	}

	TWmfRectF GetBounds()
	{
		//TWmfCoordF oTL, oBR;

		TWmfPlayer* pPlayer = (TWmfPlayer*)m_pPlayerData;;

		TWmfRectF oRect;
		
		oRect.oBR = pPlayer->oBoundBR;
		oRect.oTL = pPlayer->oBoundTL;

		double dDisplayWidth  = pPlayer->oDisplayBR.fX - pPlayer->oDisplayTL.fX;
		double dDisplayHeight = pPlayer->oDisplayBR.fY - pPlayer->oDisplayTL.fY;

		double dBoundWidth  = pPlayer->oBoundBR.fX - pPlayer->oBoundTL.fX;
		double dBoundHeight = pPlayer->oBoundBR.fY - pPlayer->oBoundTL.fY;

		
		//// Если рисунок не убирается в границы заданные  в файле, тогда используем границы самого рисунка
		//if ( dDisplayWidth > dBoundWidth || dDisplayHeight > dBoundHeight )
		//{
		//	oRect.oBR = pPlayer->oBoundBR;
		//	oRect.oTL = pPlayer->oBoundTL;
		//}


		if ( pPlayer->oBoundTL.fX > pPlayer->oDisplayTL.fX || pPlayer->oBoundTL.fY > pPlayer->oDisplayTL.fY )
		{
			oRect.oTL = pPlayer->oDisplayTL;

			oRect.oBR.fX = (float)(pPlayer->oDisplayTL.fX + dBoundWidth);
			oRect.oBR.fY = (float)(pPlayer->oDisplayTL.fY + dBoundHeight);
		}

		//if ( !m_bWindow )
		//{
		//	oRect.oTL = pPlayer->oDisplayTL;
		//	oRect.oBR = pPlayer->oDisplayBR;
		//}

		return oRect;
	}
	void Play(TWmfRectF *pDeviceRect)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		if ( CheckError() )
			return;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_SCANNED ) )
		{
			m_eError = wmf_error_Internal;
			return;
		}

		pPlayer->ulFlags |= PLAYER_PLAY; // Set play mode

		PlayMetaFile();

		pDeviceRect->oTL = pPlayer->oDisplayTL;
		pDeviceRect->oBR = pPlayer->oDisplayBR;
	}
	bool CheckError()
	{
		if ( wmf_error_None == m_eError ) 
			return false;

		return true;
	}
	bool UnSupportedWmf()
	{
		if ( wmf_error_UnSupported == m_eError )
			return true;

		return false;
	}
private:

//------------------------------------------------------------------------------------------------------------------------
	int  Read()
	{
		if ( WMF_NOT_OPEN == m_nBufferType )
		{
			return EOF;
		}
		else if ( WMF_OPEN_FROM_FILE == m_nBufferType )
		{
			TFileInfo *pData = (TFileInfo *)m_pBufferData;
			if ( !pData )
				return EOF;

			return ( ::fgetc( pData->pFile ) );
		}
		else if ( WMF_OPEN_FROM_MEMORY == m_nBufferType )
		{
			int nByte = EOF;

			TMemoryInfo *pData = (TMemoryInfo *)m_pBufferData;
			if ( !pData )
				return nByte;

			if ( pData->lPos < pData->lLength )
			{
				nByte = (int)(*(pData->pPointer));

				pData->pPointer++;
				pData->lPos++;
			}

			return nByte;
		}

		return EOF;
	}

	int  Seek(long lPos)
	{
		if ( WMF_NOT_OPEN == m_nBufferType )
		{
			return -1;
		}
		else if ( WMF_OPEN_FROM_FILE == m_nBufferType )
		{
			TFileInfo *pData = (TFileInfo *)m_pBufferData;
			if ( !pData )
				return EOF;

			return ( ::fseek( pData->pFile, lPos, SEEK_SET ) );
		}
		else if ( WMF_OPEN_FROM_MEMORY == m_nBufferType )
		{
			TMemoryInfo* pMemoryInfo = (TMemoryInfo*)m_pBufferData;

			if ( ( lPos < 0 ) || ( lPos >= pMemoryInfo->lLength ) ) 
				return (-1);

			pMemoryInfo->pPointer = pMemoryInfo->pMemory + lPos;
			pMemoryInfo->lPos     = lPos;

			return (0);
		}

		return -1;
	}

	long Tell()
	{
		if ( WMF_NOT_OPEN == m_nBufferType )
		{
			return -1;
		}
		else if ( WMF_OPEN_FROM_FILE == m_nBufferType )
		{
			TFileInfo *pData = (TFileInfo *)m_pBufferData;
			if ( !pData )
				return -1;

			return ( ::ftell( pData->pFile ) );
		}
		else if ( WMF_OPEN_FROM_MEMORY == m_nBufferType )
		{
			TMemoryInfo* pMemoryInfo = (TMemoryInfo*)m_pBufferData;

			return (pMemoryInfo->lPos);
		}

		return -1;
	}
//------------------------------------------------------------------------------------------------------------------------
    AVSINLINE void WriteHeader (long lHeaderStart, long lHeaderEnd)
	{
#ifndef DebugWriteXml
		return;
#endif
		long lHeaderSize = lHeaderEnd - lHeaderStart;

		TWmfAttributes pAttrlist;

		if ( NULL == m_pWriteData ) 
			return;

		if ( lHeaderSize <= 0 )
		{	
			m_eError = wmf_error_Internal;
			return;
		}

		unsigned char *sHeader = (unsigned char *) m_oMemoryManager.Malloc( lHeaderSize, _T("WriteHeader function") );
		if ( NULL == sHeader )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return;
		}

		//AttrNew( &pAttrlist );
		if ( CheckError() ) 
			return;

		Seek( lHeaderStart );

		for ( long lIndex = 0; lIndex < lHeaderSize; lIndex++ )
		{	
			int nByte = Read();
			if ( EOF == nByte )
			{
				m_eError = wmf_error_Internal;
				break;
			}
			sHeader[lIndex] = (unsigned char)(nByte & 0xff);
		}

		if ( CheckError() )
			return;

		Write( 0, 0, "Header", pAttrlist.psAttrs, sHeader, lHeaderSize );
		//AttrFree( &pAttrlist );
		m_oMemoryManager.Free( sHeader );
	}

    AVSINLINE void Write(unsigned long ulSize, unsigned int unFunction, const char *sName, char **psAttrs, const unsigned char *sBuffer, unsigned long ulLength)
	{
#ifndef DebugWriteXml
		return;
#endif
		char **psAttr = 0;
		TWmfWriteFileInfo *pInfo = (TWmfWriteFileInfo *)m_pWriteData;

		if ( NULL == pInfo ) 
			return;

		unsigned char sPrefix[6]; 

		sPrefix[0] = (unsigned char) ( ulSize        & 0xff);
		sPrefix[1] = (unsigned char) ((ulSize >>  8) & 0xff);
		sPrefix[2] = (unsigned char) ((ulSize >> 16) & 0xff);
		sPrefix[3] = (unsigned char) ((ulSize >> 24) & 0xff);

		sPrefix[4] = (unsigned char) ( unFunction       & 0xff);
		sPrefix[5] = (unsigned char) ((unFunction >> 8) & 0xff);

		if ( wmf_write_WMF == pInfo->eType )
		{	
			if ( strcmp( sName, "Header" ) ) 
				fwrite( sPrefix, 1, 6, pInfo->pFile );
			if ( sBuffer && ulLength ) 
				fwrite ( sBuffer, 1, ulLength, pInfo->pFile );
			return;
		}

		if ( 0 == sName ) 
			return;

        fprintf( pInfo->pFile, " <%s", sName );

		if ( psAttrs )
		{	
			psAttr = psAttrs;
			while ( *psAttr )
			{	
                fprintf( pInfo->pFile, " %s=\"", *psAttr++ );
				if ( *psAttr ) 
					fputs ( *psAttr, pInfo->pFile );
				fputs ( "\"", pInfo->pFile );
				psAttr++;
			}
		}

		fputs( ">\n", pInfo->pFile );

		if ( strcmp( sName, "Header" ) ) 
			WriteB64( sPrefix, 6 );

		if ( sBuffer && ulLength ) 
			WriteB64( sBuffer, ulLength );

        fprintf( pInfo->pFile, " </%s>\n", sName );
	}

    AVSINLINE void WriteBegin(const char *sFileName)
	{
#ifndef DebugWriteXml
		return;
#endif

		TWmfWriteFileInfo *pInfo = (TWmfWriteFileInfo *)m_oMemoryManager.Malloc( sizeof(TWmfWriteFileInfo), _T("WriteBegin function") );

		if ( NULL == pInfo )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return;
		}

		if ( NULL == sFileName )
		{
			m_eError = wmf_error_Internal;
			return;
		}

		int nLength = (int)strlen( sFileName );

		if ( nLength < 5 )
		{	
			m_eError = wmf_error_BadFile;
			return;
		}

		if ( ( strcmp( sFileName + nLength - 4, ".wmf" ) == 0 ) || ( strcmp( sFileName + nLength - 4, ".WMF" ) == 0 ) )
		{	
			pInfo->eType = wmf_write_WMF;
		}
		else if ( ( strcmp( sFileName + nLength - 4, ".xml" ) == 0 ) || ( strcmp( sFileName + nLength - 4, ".XML" ) == 0 ) )
		{	
			pInfo->eType = wmf_write_XML;
		}
		else
		{	
			m_eError = wmf_error_BadFile;
			return;
		}

		if ( pInfo->eType == wmf_write_WMF ) 
			pInfo->pFile = fopen( sFileName, "wb" );
		if ( pInfo->eType == wmf_write_XML ) 
			pInfo->pFile = fopen( sFileName, "w" );

		if ( NULL == pInfo->pFile )
		{	
			m_eError = wmf_error_BadFile;
			return;
		}

		m_pWriteData = (void *)pInfo;

		if ( pInfo->eType == wmf_write_XML )
		{	
//			flags |= API_ENABLE_EDITING;
//
			WriteStr("<?xml version=\"1.0\"?>\n");
			WriteStr("<wmfxml>\n");
		}
	}
    AVSINLINE void WriteEnd()
	{
#ifndef DebugWriteXml
		return;
#endif
		TWmfWriteFileInfo *pInfo = (TWmfWriteFileInfo *)m_pWriteData;

		if ( 0 == pInfo )
			return;

		if ( wmf_write_XML == pInfo->eType ) 
			WriteStr("</wmfxml>\n");

		fclose( pInfo->pFile );
		m_oMemoryManager.Free( m_pWriteData );
		m_pWriteData = 0;
	}

    AVSINLINE void WriteStr (const char *sString)
	{
#ifndef DebugWriteXml
		return;
#endif
		TWmfWriteFileInfo *pInfo = (TWmfWriteFileInfo *)m_pWriteData;

		if ( NULL == pInfo )
			return;
		
		if ( wmf_write_WMF == pInfo->eType ) 
			return;

		if ( sString == 0) 
			return;
		if ( *sString == 0 ) 
			return;

		fputs( sString, pInfo->pFile );
	}
    AVSINLINE void WriteB64(const unsigned char *sBuffer, unsigned long ulLength)
	{	
#ifndef DebugWriteXml
		return;
#endif
		static char c_sB64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

		unsigned long i;
		unsigned long b32;
		unsigned long remaining = ulLength;

		const unsigned char * bufptr = sBuffer;

		char buf[74];
		char * ptr = 0;

		TWmfWriteFileInfo *WFI = (TWmfWriteFileInfo *)m_pWriteData;

		if (WFI == 0) return;
		if (WFI->eType == wmf_write_WMF) return;

		if (ulLength == 0) return;

		if (sBuffer == 0)
		{	
			m_eError = wmf_error_Internal;
			return;
		}

		buf[72] = '\n';
		buf[73] = 0;
		while (remaining >= 54)
		{	ptr = buf;
		for (i = 0; i < 18; i++)
		{	b32 = (unsigned long) *bufptr++;
		b32 = (b32 << 8) | (unsigned long) *bufptr++;
		b32 = (b32 << 8) | (unsigned long) *bufptr++;
		*ptr++ = c_sB64[(b32 >> 18)       ];
		*ptr++ = c_sB64[(b32 >> 12) & 0x3f];
		*ptr++ = c_sB64[(b32 >>  6) & 0x3f];
		*ptr++ = c_sB64[ b32        & 0x3f];
		}
		fputs (buf, WFI->pFile);
		remaining -= 54;
		}

		ptr = buf;
		while (remaining >= 3)
		{	b32 = (unsigned long) *bufptr++;
		b32 = (b32 << 8) | (unsigned long) *bufptr++;
		b32 = (b32 << 8) | (unsigned long) *bufptr++;
		*ptr++ = c_sB64[(b32 >> 18)       ];
		*ptr++ = c_sB64[(b32 >> 12) & 0x3f];
		*ptr++ = c_sB64[(b32 >>  6) & 0x3f];
		*ptr++ = c_sB64[ b32        & 0x3f];
		remaining -= 3;
		}
		if (remaining == 2)
		{	b32 = (unsigned long) *bufptr++;
		b32 = (b32 << 8) | (unsigned long) *bufptr++;
		b32 = (b32 << 8);
		*ptr++ = c_sB64[(b32 >> 18)       ];
		*ptr++ = c_sB64[(b32 >> 12) & 0x3f];
		*ptr++ = c_sB64[(b32 >>  6) & 0x3f];
		remaining -= 2;
		}
		if (remaining == 1)
		{	b32 = (unsigned long) *bufptr++;
		b32 = (b32 << 16);
		*ptr++ = c_sB64[(b32 >> 18)       ];
		*ptr++ = c_sB64[(b32 >> 12) & 0x3f];
		remaining -= 1;
		}

		*ptr++ = '\n';
		*ptr++ = 0;
		fputs (buf, WFI->pFile);
	}

//------------------------------------------------------------------------------------------------------------------------
	//void        AttrNew  (TWmfAttributes *pList)
	//{	
	//	if ( NULL == pList ) 
	//		return;

	//	pList->sName    = 0;

	//	pList->sBuffer  = 0;
	//	pList->ulLength = 0;
	//	pList->ulOffset = 0;

	//	pList->ulCount = 0;
    //	pList->ulmax   = 0;
	//	pList->psAttrs = (char **)m_oMemoryManager.Malloc( 2 * (8 + 1) * sizeof (char *), _T("AttrNew function") );

	//	if ( NULL == pList->psAttrs )
	//	{
	//		m_eError = wmf_error_NotEnoughMemory;
	//		return;
	//	}

    //	pList->ulmax = 8;
	//	pList->psAttrs[pList->ulCount] = 0;
	//}

	//void        AttrClear(TWmfAttributes *pList)
	//{	
	//	if ( NULL == pList ) 
	//		return;

	//	for ( unsigned long ulIndex = 0; ulIndex < (2 * pList->ulCount); ulIndex++) 
	//		m_oMemoryManager.Free( pList->psAttrs[ulIndex] );

	//	pList->ulCount = 0;
	//	pList->psAttrs[pList->ulCount] = 0;

	//	if ( pList->sName )
	//	{	
	//		m_oMemoryManager.Free( pList->sName );
	//		pList->sName = 0;
	//	}

	//	pList->sBuffer  = 0;
	//	pList->ulLength = 0;
	//}

	//void        AttrFree (TWmfAttributes *pList)
	//{
	//	if ( NULL == pList )
	//		return;

	//	AttrClear( pList );

	//	if ( pList->psAttrs )
	//	{	
	//		m_oMemoryManager.Free( pList->psAttrs );
	//		pList->psAttrs = 0;
	//	}
	//	
    //	pList->ulmax = 0;
	//}

	//const char *AttrAdd  (TWmfAttributes *pList, const char *sName, const char *sValue)
	//{	
	//	if ( NULL == pList ) 
	//		return NULL;
	//	if ( NULL == pList->psAttrs ) 
	//		return NULL;

	//	char *sCopyValue = WmfStrDup( sValue );
	//	if ( NULL == sCopyValue )
	//	{
	//		m_eError = wmf_error_NotEnoughMemory;
	//		return NULL;
	//	}

	//	char *sCopyName = NULL;
	//	for ( unsigned long ulIndex = 0; ulIndex < (2 * pList->ulCount); ulIndex += 2 )
	//	{	
	//		if ( strcmp( pList->psAttrs[ulIndex], sName ) == 0 )
	//		{	
	//			sCopyName = pList->psAttrs[ulIndex];
	//			m_oMemoryManager.Free( pList->psAttrs[ulIndex + 1] );
	//			pList->psAttrs[ulIndex + 1] = sCopyValue;
	//			break;
	//		}
	//	}

	//	if ( sCopyName ) 
	//		return sCopyValue;

	//	sCopyName = WmfStrDup( sName );
	//	if ( NULL == sCopyName )
	//	{
	//		m_eError = wmf_error_NotEnoughMemory;
	//		return NULL;
	//	}

    //	if ( pList->ulCount == pList->ulmax )
	//	{	
    //		char **psMore = (char **)m_oMemoryManager.Realloc( pList->psAttrs, 2 * (pList->ulmax + 8 + 1) * sizeof (char *) );

	//		if ( NULL == psMore )
	//		{
	//			m_eError = wmf_error_NotEnoughMemory;
	//			return NULL;
	//		}

	//		pList->psAttrs = psMore;
    //		pList->ulmax  += 8;
	//	}
	//	pList->psAttrs[pList->ulCount++] = sCopyName;
	//	pList->psAttrs[pList->ulCount++] = sCopyValue;

	//	pList->psAttrs[pList->ulCount] = 0;

	//	return sCopyValue;
	//}

	//const char *AttrQuery(TWmfAttributes *pList, const char *sName)
	//{	
	//	const char *sValue = NULL;

	//	if ( NULL == pList ) 
	//		return NULL;
	//	if ( NULL == pList->psAttrs ) 
	//		return NULL;

	//	for ( unsigned long ulIndex = 0; ulIndex < (2 * pList->ulCount); ulIndex += 2)
	//	{	
	//		if ( strcmp( pList->psAttrs[ulIndex], sName ) == 0 )
	//		{	
	//			sValue = pList->psAttrs[ulIndex + 1];
	//			break;
	//		}
	//	}
	//	return sValue;
	//}

//------------------------------------------------------------------------------------------------------------------------
	void SetDefaults(TWmfPen *pPen, TWmfBrush *pBrush, TWmfFont *pFont)
	{	
		pPen->oColor   = c_oColorBlack;
		pPen->dWidth   = 0; // 0 - это 1 при любом Dpi
		pPen->dHeight  = 1;
		pPen->ushStyle = (pPen->ushStyle & ~PS_STYLE_MASK ) | (PS_SOLID & PS_STYLE_MASK );
		pPen->ushStyle = (pPen->ushStyle & ~PS_ENDCAP_MASK) | (PS_ENDCAP_ROUND & PS_ENDCAP_MASK);
		pPen->ushStyle = (pPen->ushStyle & ~PS_JOIN_MASK  ) | (PS_JOIN_ROUND & PS_JOIN_MASK  );
		pPen->ushStyle = (pPen->ushStyle & ~PS_TYPE_MASK  ) | (PS_COSMETIC & PS_TYPE_MASK  );

		TWmfBMP oBitmap;
		oBitmap.ushWidth  = 0;
		oBitmap.ushHeight = 0;
		oBitmap.pData     = NULL;

		pBrush->oColor   = c_oColorBlack;
		pBrush->ushStyle = BS_NULL;
		pBrush->ushHatch = HS_HORIZONTAL;
		pBrush->oBitmap  = oBitmap;

		pFont->ushHeight        = 12;
		pFont->ushWidth         = 12;
		pFont->shEscapement     = 0;
		pFont->shOrientation    = 0;
		pFont->ushWeight        = 0;
		pFont->unItalic         = 0;
		pFont->unUnderline      = 0;
		pFont->unStrikeOut      = 0;
		pFont->unCharSet        = 0;
		pFont->unOutPrecision   = 0;
		pFont->unClipPrecision  = 0;
		pFont->unQuality        = 0;
		pFont->unPitchAndFamily = 0;
		pFont->sFaceName        = "Times New Roman";
		pFont->pUserData        = NULL;
	}
//------------------------------------------------------------------------------------------------------------------------
	TWmfDC *CopyDC(TWmfDC *pDC)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfRegion *pClip;
		TWmfRegion *pClipNew;

		TWmfDC *pNewDC = (TWmfDC *)m_oMemoryManager.Malloc( sizeof(TWmfDC), _T("CopyDC function: pNewDC") );
		if ( NULL == pNewDC )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return NULL;
		}

		pNewDC->pClip = (TWmfRegion *)m_oMemoryManager.Malloc( sizeof( TWmfRegion ), _T("CopyDC function: pNewDC->pClip") );
		if ( NULL == pNewDC->pClip )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return NULL;
		}

		pClipNew = (TWmfRegion*)pNewDC->pClip;

		pClipNew->unNumRects = 0;
		pClipNew->unSize     = 8;
		pClipNew->pRects     = (TWmfRectF*)m_oMemoryManager.Malloc( pClipNew->unSize * sizeof (TWmfRectF), _T("CopyDC function: pNewDC->pRects") );
		if ( NULL == pClipNew->pRects )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return NULL;
		}

		pNewDC->oBGColor   = c_oColorWhite;
		pNewDC->oTextColor = c_oColorBlack;

		pNewDC->ushBGMode       = OPAQUE;
		pNewDC->ushPolyFillMode = ALTERNATE;
		pNewDC->ushROPMode      = R2_COPYPEN;
 
		pNewDC->ushTextAlign    = TA_LEFT;
		pNewDC->ushCharExtra    = 0;
		pNewDC->ushBreakExtra   = 0;

		if ( pDC )
		{	
			pNewDC->pBrush = pDC->pBrush;
			pNewDC->pPen   = pDC->pPen;
			pNewDC->pFont  = pDC->pFont;

			pClip = (TWmfRegion*)pDC->pClip;

			REGION_CopyRegion( pClipNew, pClip );

			pNewDC->oWindow.nX      = pDC->oWindow.nX;
			pNewDC->oWindow.nY      = pDC->oWindow.nY;
			pNewDC->oWindow.nWidth  = pDC->oWindow.nWidth;
			pNewDC->oWindow.nHeight = pDC->oWindow.nHeight;

			pNewDC->dPixelHeight  = pDC->dPixelHeight;
			pNewDC->dPixelWidth   = pDC->dPixelWidth;

			pNewDC->ushMapMode = pDC->ushMapMode;
		}
		else
		{	
			SetDefaults( &(pPlayer->oDefaultPen), &(pPlayer->oDefaultBrush), &(pPlayer->oDefaultFont) );

			pNewDC->pBrush = &(pPlayer->oDefaultBrush);
			pNewDC->pPen   = &(pPlayer->oDefaultPen);
			pNewDC->pFont  = &(pPlayer->oDefaultFont);

			pNewDC->oWindow.nX      = 0;
			pNewDC->oWindow.nY      = 0;
			pNewDC->oWindow.nWidth  = 1024;
			pNewDC->oWindow.nHeight = 1024;

			pNewDC->dPixelHeight  = 1;
			pNewDC->dPixelWidth   = 1;

			pNewDC->ushMapMode    = MM_TEXT;
		}

		return pNewDC;
	}

	void    DCStackPush(TWmfDC *pDC)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		if ( NULL == pDC )
		{	
			m_eError = wmf_error_Internal;
			return;
		}

		if ( NULL == pPlayer->ppDCStack )
		{	
			pPlayer->ppDCStack = (TWmfDC**)m_oMemoryManager.Malloc( 8 * sizeof (TWmfDC*), _T("DCStackPush function") );
			if ( NULL == pPlayer->ppDCStack )
			{
				m_eError = wmf_error_NotEnoughMemory;
				return;
			}
            pPlayer->nDCStackMaxLen = 8;
		}

        if ( pPlayer->nDCStackLength == pPlayer->nDCStackMaxLen )
		{
            TWmfDC **ppMore = (TWmfDC**)m_oMemoryManager.Realloc( pPlayer->ppDCStack, (pPlayer->nDCStackMaxLen + 8) * sizeof (TWmfDC*));
			if ( NULL == ppMore )
			{
				m_eError = wmf_error_NotEnoughMemory;
				return;
			}
			pPlayer->ppDCStack = ppMore;
            pPlayer->nDCStackMaxLen += 8;
		}

		pPlayer->ppDCStack[pPlayer->nDCStackLength] = pDC;
		pPlayer->nDCStackLength++;
	}
	TWmfDC *DCStackPop ()
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;
		
		if ( 0 == pPlayer->nDCStackLength  )
		{
			m_eError = wmf_error_Internal;
			return NULL;
		}
		pPlayer->nDCStackLength--;
		
		return pPlayer->ppDCStack[pPlayer->nDCStackLength];
	}
	void    DCStackFree()
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		while ( pPlayer->nDCStackLength )
		{
			pPlayer->nDCStackLength--;
			m_oMemoryManager.Free( pPlayer->ppDCStack[pPlayer->nDCStackLength]);
		}

		m_oMemoryManager.Free( pPlayer->ppDCStack );

		pPlayer->ppDCStack      = NULL;
        pPlayer->nDCStackMaxLen = 0;
	}


//------------------------------------------------------------------------------------------------------------------------
	unsigned  short Record_GetUShortPar(TWmfRecord *pRecord, unsigned long ulIndex)
	{
		if ( ulIndex >= pRecord->ulSize )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		ulIndex <<= 1;
		unsigned short ushPar1 = pRecord->sParameter[ulIndex];
		ulIndex++;
		unsigned short ushPar2 = pRecord->sParameter[ulIndex];

		return ( ( ushPar2 << 8 ) | ushPar1 );
	}

	int             Record_SetUShortPar(TWmfRecord *pRecord, unsigned long ulIndex, unsigned short ushPar)
	{	
		int nChanged = 0;

		unsigned char unChar1 = (unsigned char) ( ushPar       & 0xff);
		unsigned char unChar2 = (unsigned char) ((ushPar >> 8) & 0xff);

		if ( ulIndex >= pRecord->ulSize )
		{
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		ulIndex <<= 1;
		if ( pRecord->sParameter[ulIndex] != unChar1 )
		{	
			pRecord->sParameter[ulIndex] = unChar1;
			nChanged = 1;
		}

		ulIndex++;
		if ( pRecord->sParameter[ulIndex] != unChar2 )
		{	
			pRecord->sParameter[ulIndex] = unChar2;
			nChanged = 1;
		}

		return nChanged;
	}

	short           Record_GetShortPar (TWmfRecord *pRecord, unsigned long ulIndex)
	{	
		unsigned short ushPar = Record_GetUShortPar( pRecord, ulIndex );

		return UShort_2_Short( ushPar );
	}

	int             Record_GetIntPar   (TWmfRecord *pRecord, unsigned long ulIndex)
	{	
		unsigned short ushPar = Record_GetUShortPar( pRecord, ulIndex );

		return UShort_2_Long( ushPar );
	}
	long            Record_GetLongPar  (TWmfRecord *pRecord, unsigned long ulIndex1, unsigned long ulIndex2)
	{
		unsigned short ushPar1 = Record_GetUShortPar( pRecord, ulIndex1 );
		unsigned short ushPar2 = Record_GetUShortPar( pRecord, ulIndex2 );

		return (long)( ((long)(ushPar1 << 16)) | ((long)(ushPar2)) );
	}

	TWmfRecord      Record_OffsetRecord(TWmfRecord *pRecord, unsigned long ulIndex)
	{
		TWmfRecord oNewRecord;

		oNewRecord.unFunction = pRecord->unFunction;
		oNewRecord.ulSize     = 0;
		oNewRecord.sParameter = 0;

		if ( ulIndex > pRecord->ulSize )
		{	
			m_eError = wmf_error_BadFormat;
			return oNewRecord;
		}

		oNewRecord.ulSize     = pRecord->ulSize - ulIndex;
		ulIndex <<= 1;
		oNewRecord.sParameter = pRecord->sParameter + ulIndex;
		oNewRecord.lPosition  = pRecord->lPosition + ulIndex;

		return oNewRecord;
	}


//------------------------------------------------------------------------------------------------------------------------
	int Meta_MapMode      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		unsigned short ushPar = Record_GetUShortPar( pRecord, 0 );

		SetMapMode( ushPar );

		return 0;
	}

	int Meta_OrgExt       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		int nParX = Record_GetIntPar( pRecord, 1 );
		int nParY = Record_GetIntPar( pRecord, 0 );

		switch ( pRecord->unFunction )
		{
		case META_SETWINDOWORG: // 2.3.5.31
			pPlayer->pDC->oWindow.nX = nParX;
			pPlayer->pDC->oWindow.nY = nParY;

			pPlayer->oWindow.nX = nParX;
			pPlayer->oWindow.nY = nParY;

			{
				float fW = pPlayer->oBoundBR.fX - pPlayer->oBoundTL.fX;
				float fH = pPlayer->oBoundBR.fY - pPlayer->oBoundTL.fY;

				TWmfCoord oOrigin = { nParX, nParY };
				TWmfCoordF oOriginF = TranslateCoordF( oOrigin );

				pPlayer->oBoundTL.fX = oOriginF.fX;
				pPlayer->oBoundTL.fY = oOriginF.fY;
				pPlayer->oBoundBR.fX = pPlayer->oBoundTL.fX + fW;
				pPlayer->oBoundBR.fY = pPlayer->oBoundTL.fY + fH;
			}

			break;

		case META_SETVIEWPORTORG: // 2.3.5.29
			pPlayer->oViewportOrigin.fX = (float) ((double) nParX * pPlayer->pDC->dPixelWidth  );
			pPlayer->oViewportOrigin.fY = (float) ((double) nParY * pPlayer->pDC->dPixelHeight );
			break;

		case META_SETVIEWPORTEXT: // 2.3.5.28
			pPlayer->unViewportWidth  = nParX;
			pPlayer->unViewportHeight = nParY;
			GetPixelWidth();  // пересчитываем Dpi 
			GetPixelHeight(); //
			break;

		case META_SETWINDOWEXT: // 2.3.5.30
			pPlayer->pDC->oWindow.nWidth  = nParX;
			pPlayer->pDC->oWindow.nHeight = nParY;

			pPlayer->oWindow.nWidth  = nParX;
			pPlayer->oWindow.nHeight = nParY;

			GetPixelWidth();  // пересчитываем Dpi 
			GetPixelHeight(); //

			{
				TWmfCoord oOrigin = { nParX, nParY };
				TWmfCoordF oOriginF = TranslateCoordF( oOrigin );

				pPlayer->oBoundBR.fX = pPlayer->oBoundTL.fX + oOriginF.fX;
				pPlayer->oBoundBR.fY = pPlayer->oBoundTL.fY + oOriginF.fY;
			}

			m_bWindow = true;
			break;

		case META_OFFSETWINDOWORG: // 2.3.5.7
			pPlayer->pDC->oWindow.nX += nParX;
			pPlayer->pDC->oWindow.nY += nParY;

			pPlayer->oWindow.nX += nParX;
			pPlayer->oWindow.nY += nParY;
			break;

		case META_OFFSETVIEWPORTORG: // 2.3.5.6
			pPlayer->oViewportOrigin.fX += (float) ((double) nParX * pPlayer->pDC->dPixelWidth  );
			pPlayer->oViewportOrigin.fY += (float) ((double) nParY * pPlayer->pDC->dPixelHeight );
			break;

		default:

			m_eError = wmf_error_Internal;
			break;
		}

		return 0;
	}

	int Meta_Scale        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		double dNumX   = (double) Record_GetIntPar( pRecord, 3 ); // числитель по Х
		double dDenomX = (double) Record_GetIntPar( pRecord, 2 ); // знаменатель по Х
		double dNumY   = (double) Record_GetIntPar( pRecord, 1 ); // числитель по У
		double dDenomY = (double) Record_GetIntPar( pRecord, 0 ); // знаменатель по У

		if ( ( dDenomX == 0 ) || ( dDenomY == 0 ) )
		{
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		switch ( pRecord->unFunction )
		{
		case META_SCALEWINDOWEXT: // 2.3.5.13
			pPlayer->pDC->oWindow.nWidth  = (int) (((double) pPlayer->pDC->oWindow.nWidth  * dNumX ) / dDenomX );
			pPlayer->pDC->oWindow.nHeight = (int) (((double) pPlayer->pDC->oWindow.nHeight * dNumY ) / dDenomY );

			pPlayer->oWindow.nWidth  = pPlayer->pDC->oWindow.nWidth;
			pPlayer->oWindow.nHeight = pPlayer->pDC->oWindow.nHeight;
			break;

		case META_SCALEVIEWPORTEXT: // 2.3.5.12
			pPlayer->unViewportWidth  = (int) (((double) pPlayer->unViewportWidth  * dNumX ) / dDenomX );
			pPlayer->unViewportHeight = (int) (((double) pPlayer->unViewportHeight * dNumY ) / dDenomY );
			break;

		default:

			m_eError = wmf_error_Internal;
			break;
		}

		GetPixelWidth();  // Пересчитываем Dpi
		GetPixelHeight(); //

		return 0;
	}
	int Meta_MoveTo       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 0 );

		pPlayer->oCurrent = WmfCoord( ushX,ushY );

		return 0;
	}

	int Meta_Flood        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfCoord oPoint;

		TWmfFlood oFlood;

		unsigned short ushX    = Record_GetUShortPar( pRecord, 4 );
		unsigned short ushY    = Record_GetUShortPar( pRecord, 3 );
		unsigned short ushB    = Record_GetUShortPar( pRecord, 2 );
		unsigned short ushRG   = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushMode = Record_GetUShortPar( pRecord, 0 );

		oPoint = WmfCoord( ushX, ushY );

		oFlood.oPoint = TranslateCoordF( oPoint );
		oFlood.oColor = CWmfColor::LongToRGB( ushRG, ushB );

		// При сканировании мы добавляем цвет и точку
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			m_oColorData.Add( &(oFlood.oColor) );
			CoordFRegister( oFlood.oPoint, 0 );
			return 0;
		}

		oFlood.pDC = pPlayer->pDC;

		oFlood.ushType = ushMode;

		oFlood.dPixelWidth  = abs( pPlayer->pDC->dPixelWidth  );
		oFlood.dPixelHeight = abs( pPlayer->pDC->dPixelHeight );

		switch ( pRecord->unFunction )
		{
		case META_FLOODFILL: // 2.3.3.7
			if ( m_pOutput ) 
				m_pOutput->Flood_Interior( &oFlood );
			break;

		case META_EXTFLOODFILL: // 2.3.3.4
			if ( m_pOutput ) 
				m_pOutput->Flood_Exterior( &oFlood );
			break;

		default:

			m_eError = wmf_error_Internal;
			break;
		}

		return 0;
	}
	int Meta_Pixel        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX  = Record_GetUShortPar( pRecord, 3 );
		unsigned short ushY  = Record_GetUShortPar( pRecord, 2 );
		unsigned short ushB  = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushRG = Record_GetUShortPar( pRecord, 0 );

		TWmfCoord oPoint = WmfCoord( ushX, ushY );
		TWmfDrawPixel oDrawPixel;

		oDrawPixel.oPoint       = TranslateCoordF( oPoint );
		oDrawPixel.oColor       = CWmfColor::LongToRGB( ushRG, ushB );
		oDrawPixel.dPixelWidth  = abs( pPlayer->pDC->dPixelWidth  );
		oDrawPixel.dPixelHeight = abs( pPlayer->pDC->dPixelHeight );

		// При сканировании мы добавляем цвет и точку
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			m_oColorData.Add( &(oDrawPixel.oColor) );
            float fScope = (float)(std::max)( oDrawPixel.dPixelWidth, oDrawPixel.dPixelHeight );
			CoordFRegister( oDrawPixel.oPoint, fScope );
			return 0;
		}

		oDrawPixel.pDC = pPlayer->pDC;

		if ( m_pOutput ) 
			m_pOutput->Draw_Pixel( &oDrawPixel );

		return 0;
	}
	int Meta_Arc          (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		float fScope = 0;

		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = Record_GetUShortPar( pRecord, 7 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 6 );

		TWmfCoord oTempPoint = WmfCoord( ushX, ushY );
		TWmfDrawArc oDrawArc;
		oDrawArc.oTL = TranslateCoordF( oTempPoint );

		ushX = Record_GetUShortPar( pRecord, 5 );
		ushY = Record_GetUShortPar( pRecord, 4 );

		oTempPoint = WmfCoord( ushX, ushY );
		oDrawArc.oBR = TranslateCoordF( oTempPoint );

		ushX = Record_GetUShortPar( pRecord, 3 );
		ushY = Record_GetUShortPar( pRecord, 2 );

		oTempPoint = WmfCoord( ushX, ushY );
		oDrawArc.oEnd = TranslateCoordF( oTempPoint );

		unsigned short ushEndX = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushEndY = Record_GetUShortPar( pRecord, 0 );

		if ( ( ushEndX == ushX ) && ( ushEndY == ushY ) )
		{
			// TO DO: Начальная точка совпадает с конечной: возможно тут надо рисовать эллипс
		}

		oTempPoint = WmfCoord( ushEndX, ushEndY );
		oDrawArc.oStart = TranslateCoordF( oTempPoint );

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float)( (std::max)( pPen->dWidth, pPen->dHeight ) ) / 2;

			CoordFRegister( oDrawArc.oEnd, fScope );
			CoordFRegister( oDrawArc.oStart, fScope );
		}

		TWmfCoordF oPointC;
		oPointC.fX = (oDrawArc.oTL.fX + oDrawArc.oBR.fX) / 2;
		oPointC.fY = (oDrawArc.oTL.fY + oDrawArc.oBR.fY) / 2;

		// Для определения четверти, в котороый находятся начальная и конечная точки
		oDrawArc.oStart.fX -= oPointC.fX;
		oDrawArc.oStart.fY -= oPointC.fY;
		oDrawArc.oEnd.fX   -= oPointC.fX;
		oDrawArc.oEnd.fY   -= oPointC.fY;

		TWmfCoordF oPointF;
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			char nQuardStart, nQuardEnd;

			if  ( ( oDrawArc.oStart.fX > 0) && ( oDrawArc.oStart.fY >= 0 ) ) 
				nQuardStart = '1';
			else if ( ( oDrawArc.oStart.fX <= 0) && ( oDrawArc.oStart.fY > 0)) 
				nQuardStart = '2';
			else if ( ( oDrawArc.oStart.fX < 0) && ( oDrawArc.oStart.fY <= 0)) 
				nQuardStart = '3';
			else 
				nQuardStart = '4';

			if ( ( oDrawArc.oEnd.fX > 0 ) && ( oDrawArc.oEnd.fY >= 0 ) ) 
				nQuardEnd = '1';
			else if ( ( oDrawArc.oEnd.fX <= 0 ) && ( oDrawArc.oEnd.fY > 0 ) ) 
				nQuardEnd = '2';
			else if ( ( oDrawArc.oEnd.fX < 0 ) && ( oDrawArc.oEnd.fY <= 0 ) )
				nQuardEnd = '3';
			else 
				nQuardEnd = '4';

			switch (nQuardStart)
			{
			case '1':
				switch (nQuardEnd)
				{
				case '1':
					if ( ( oDrawArc.oEnd.fX < oDrawArc.oStart.fX ) || ( oDrawArc.oEnd.fY < oDrawArc.oStart.fY ) ) 
						break;
					oPointF.fX = oDrawArc.oBR.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '4':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oBR.fY;
					CoordFRegister( oPointF, fScope );
				case '3':
					oPointF.fX = oDrawArc.oTL.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '2':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oTL.fY;
					CoordFRegister( oPointF, fScope );
					break;

				default:
					m_eError = wmf_error_Internal;
					break;
				}
				break;

			case '2':
				switch (nQuardEnd)
				{
				case '2':
					if ( ( oDrawArc.oEnd.fX < oDrawArc.oStart.fX ) || ( oDrawArc.oEnd.fY > oDrawArc.oStart.fY ) ) 
						break;
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oTL.fY;
					CoordFRegister( oPointF, fScope );
				case '1':
					oPointF.fX = oDrawArc.oBR.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '4':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oBR.fY;
					CoordFRegister( oPointF, fScope );
				case '3':
					oPointF.fX = oDrawArc.oTL.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
					break;

				default:
					m_eError = wmf_error_Internal;
					break;
				}
				break;

			case '3':
				switch (nQuardEnd)
				{
				case '3':
					if ( ( oDrawArc.oEnd.fX > oDrawArc.oStart.fX ) || ( oDrawArc.oEnd.fY > oDrawArc.oStart.fY ) ) 
						break;
					oPointF.fX = oDrawArc.oTL.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '2':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oTL.fY;
					CoordFRegister( oPointF, fScope );
				case '1':
					oPointF.fX = oDrawArc.oBR.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '4':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oBR.fY;
					CoordFRegister( oPointF, fScope );
					break;

				default:
					m_eError = wmf_error_Internal;
					break;
				}
				break;

			case '4':
				switch (nQuardEnd)
				{
				case '4':
					if ( ( oDrawArc.oEnd.fX > oDrawArc.oStart.fX ) || ( oDrawArc.oEnd.fY < oDrawArc.oStart.fY ) ) 
						break;
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oBR.fY;
					CoordFRegister( oPointF, fScope );
				case '3':
					oPointF.fX = oDrawArc.oTL.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
				case '2':
					oPointF.fX = oPointC.fX;
					oPointF.fY = oDrawArc.oTL.fY;
					CoordFRegister( oPointF, fScope );
				case '1':
					oPointF.fX = oDrawArc.oBR.fX;
					oPointF.fY = oPointC.fY;
					CoordFRegister( oPointF, fScope );
					break;

				default:
					m_eError = wmf_error_Internal;
					break;
				}
				break;

			default:
				m_eError = wmf_error_Internal;
				break;
			}

			return 0;
		}

		oPointF.fX = (oDrawArc.oBR.fX - oDrawArc.oTL.fX) / 2; // Половина ширины эллипса
		oPointF.fY = (oDrawArc.oBR.fY - oDrawArc.oTL.fY) / 2; // Половина высоты эллипса

		if ( ( oDrawArc.oStart.fX == 0 ) && ( oDrawArc.oStart.fY == 0 ) ) 
			oDrawArc.oStart.fX = oPointF.fX;

		if ( oDrawArc.oStart.fX >   oPointF.fX  ) oDrawArc.oStart.fX =   oPointF.fX;
		if ( oDrawArc.oStart.fX < (-oPointF.fX) ) oDrawArc.oStart.fX = - oPointF.fX;
		if ( oDrawArc.oStart.fY >   oPointF.fY  ) oDrawArc.oStart.fY =   oPointF.fY;
		if ( oDrawArc.oStart.fY < (-oPointF.fY) ) oDrawArc.oStart.fY = - oPointF.fY;

		if ( ( oDrawArc.oEnd.fX == 0 ) && ( oDrawArc.oEnd.fY == 0 ) ) 
			oDrawArc.oEnd.fX = oPointF.fX;

		if ( oDrawArc.oEnd.fX >   oPointF.fX  ) oDrawArc.oEnd.fX =   oPointF.fX;
		if ( oDrawArc.oEnd.fX < (-oPointF.fX) ) oDrawArc.oEnd.fX = - oPointF.fX;
		if ( oDrawArc.oEnd.fY >   oPointF.fY  ) oDrawArc.oEnd.fY =   oPointF.fY;
		if ( oDrawArc.oEnd.fY < (-oPointF.fY) ) oDrawArc.oEnd.fY = - oPointF.fY;

		oDrawArc.pDC = pPlayer->pDC;

		switch (pRecord->unFunction)
		{
		case META_PIE:
			if ( m_pOutput )
				m_pOutput->Draw_Pie( &oDrawArc );
			break;

		case META_CHORD:
			if ( m_pOutput )
				m_pOutput->Draw_Chord( &oDrawArc );
			break;

		case META_ARC:
			if ( m_pOutput )
				m_pOutput->Draw_Arc( &oDrawArc );
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		return 0;
	}

	int Meta_Ellipse      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = Record_GetUShortPar( pRecord, 3 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 2 );

		TWmfCoord oTempPoint = WmfCoord( ushX, ushY );
		TWmfDrawArc oDrawArc;
		oDrawArc.oTL = TranslateCoordF( oTempPoint );

		ushX = Record_GetUShortPar( pRecord, 1 );
		ushY = Record_GetUShortPar( pRecord, 0 );

		oTempPoint = WmfCoord( ushX, ushY );
		oDrawArc.oBR = TranslateCoordF( oTempPoint );

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ))
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dWidth, pPen->dHeight ) ) / 2;
			CoordFRegister( oDrawArc.oTL, fScope );
			CoordFRegister( oDrawArc.oBR, fScope );

			return 0;
		}

		oDrawArc.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Draw_Ellipse( &oDrawArc );

		return 0;
	}

	int Meta_Line         (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfDrawLine oDrawLine;
		oDrawLine.oFrom = TranslateCoordF( pPlayer->oCurrent );

		unsigned short ushX = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 0 );

		TWmfCoord oPoint = WmfCoord( ushX, ushY );
		oDrawLine.oTo = TranslateCoordF( oPoint );

		pPlayer->oCurrent = oPoint;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dWidth, pPen->dHeight ) ) / 2;
			CoordFRegister( oDrawLine.oFrom, fScope );
			CoordFRegister( oDrawLine.oTo, fScope );
			return 0;
		}

		oDrawLine.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Draw_Line( &oDrawLine );

		return 0;
	}
	int Meta_Lines        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned long ulParamIndex;

		TWmfPolyLine oPolyLine;
		oPolyLine.ushCount = Record_GetUShortPar( pRecord, 0 );

		// Во время сканирования регистрируем все точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dHeight, pPen->dWidth ) ) / 2;

			ulParamIndex = 1;
			for ( unsigned short ushPointIndex = 0; ushPointIndex < oPolyLine.ushCount; ushPointIndex++ )
			{	
				unsigned short ushX = Record_GetUShortPar( pRecord, ulParamIndex );
				ulParamIndex++;
				unsigned short ushY = Record_GetUShortPar( pRecord, ulParamIndex );
				ulParamIndex++;

				TWmfCoord  oPoint  = WmfCoord( ushX, ushY );
				TWmfCoordF oPointF = TranslateCoordF( oPoint );
				CoordFRegister( oPointF, fScope );
			}
			return 0;
		}

		oPolyLine.pPoints = (TWmfCoordF *)m_oMemoryManager.Malloc( oPolyLine.ushCount * sizeof (TWmfCoordF), _T("Meta_Lines function") );

		if ( NULL == oPolyLine.pPoints )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		ulParamIndex = 1;
		for ( unsigned short ushPointIndex = 0; ushPointIndex < oPolyLine.ushCount; ushPointIndex++ )
		{	
			unsigned short ushX = Record_GetUShortPar( pRecord, ulParamIndex );
			ulParamIndex++;
			unsigned short ushY = Record_GetUShortPar( pRecord, ulParamIndex );
			ulParamIndex++;

			TWmfCoord oPoint = WmfCoord( ushX, ushY );
			oPolyLine.pPoints[ushPointIndex] = TranslateCoordF( oPoint );
		}

		oPolyLine.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Poly_Line( &oPolyLine );

		m_oMemoryManager.Free( oPolyLine.pPoints );

		return 0;
	}

	int Meta_Polygon      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned long ulParamIndex;

		TWmfPolyLine oPolyLine;
		oPolyLine.ushCount = Record_GetUShortPar( pRecord, 0 );

		// Во время сканирования регистрируем все точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dWidth, pPen->dHeight ) ) / 2;

			ulParamIndex = 1;
			for ( unsigned short ushPointIndex = 0; ushPointIndex < oPolyLine.ushCount; ushPointIndex++ )
			{	
				unsigned short ushX = Record_GetUShortPar( pRecord, ulParamIndex );
				ulParamIndex++;
				unsigned short ushY = Record_GetUShortPar( pRecord, ulParamIndex);
				ulParamIndex++;

				TWmfCoord  oPoint  = WmfCoord( ushX, ushY );
				TWmfCoordF oPointF = TranslateCoordF( oPoint );
				CoordFRegister( oPointF, fScope );
			}
			return 0;
		}

		oPolyLine.pPoints = (TWmfCoordF*)m_oMemoryManager.Malloc( oPolyLine.ushCount * sizeof (TWmfCoordF), _T("Meta_Polygon function") );

		if ( NULL == oPolyLine.pPoints )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		ulParamIndex = 1;
		for ( unsigned short ushPointIndex = 0; ushPointIndex < oPolyLine.ushCount; ushPointIndex++ )
		{	
			unsigned short ushX = Record_GetUShortPar( pRecord, ulParamIndex );
			ulParamIndex++;
			unsigned short ushY = Record_GetUShortPar( pRecord, ulParamIndex);
			ulParamIndex++;

			TWmfCoord oPoint = WmfCoord( ushX, ushY );
			oPolyLine.pPoints[ushPointIndex] = TranslateCoordF( oPoint );
		}

		oPolyLine.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Draw_Polygon( &oPolyLine );

		m_oMemoryManager.Free( oPolyLine.pPoints );

		return 0;
	}
	int Meta_Polygons     (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfPolyPoly oPolyPoly;
		oPolyPoly.ushPolyCount = Record_GetUShortPar( pRecord, 0 );

		if ( 0 == oPolyPoly.ushPolyCount ) 
			return 0;

		oPolyPoly.ppPoints = (TWmfCoordF**)m_oMemoryManager.Malloc( oPolyPoly.ushPolyCount * sizeof(TWmfCoordF*), _T("Meta_Polygons function") );

		if ( NULL == oPolyPoly.ppPoints )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyPoly.pCount = (unsigned short *)m_oMemoryManager.Malloc( oPolyPoly.ushPolyCount * sizeof(unsigned short), _T("Meta_Polygons") );

		if ( NULL == oPolyPoly.pCount )
		{	
			m_oMemoryManager.Free( oPolyPoly.ppPoints );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		unsigned short ushCount   = 0; // Суммарное количество точек в PolyLine
		unsigned short ushNumPars = 0; // Суммарное количество точек в PolyPoly
		int nSkipRecord = 0;
		for ( unsigned short ushPolyIndex = 0; ushPolyIndex < oPolyPoly.ushPolyCount; ushPolyIndex++ )
		{	
			oPolyPoly.pCount[ushPolyIndex] = Record_GetUShortPar( pRecord, (unsigned long) (1 + ushPolyIndex) );
			ushCount   += oPolyPoly.pCount[ushPolyIndex] + 2; // Для преобразования: PolyPoly -> PolyLine
			ushNumPars += oPolyPoly.pCount[ushPolyIndex];

			if ( ( oPolyPoly.pCount[ushPolyIndex] < 3 ) && ( 0 == nSkipRecord ) )
			{
				// Странный полигон из 2-х или менее точек
				nSkipRecord = 1;
			}
			if ( nSkipRecord )
			{	
				oPolyPoly.ppPoints[ushPolyIndex] = NULL;
			}
			else
			{	
				(oPolyPoly.ppPoints[ushPolyIndex]) = (TWmfCoordF *)m_oMemoryManager.Malloc( oPolyPoly.pCount[ushPolyIndex] * sizeof (TWmfCoordF), _T("Meta_Polygons") );
				if ( NULL == oPolyPoly.ppPoints[ushPolyIndex] )
				{
					m_eError = wmf_error_NotEnoughMemory;
					break;
				}
			}
		}

		if ( nSkipRecord )
		{
			// Тут можно выдать ошибку m_eError = wmf_error_BadFormat, но тут не так критично
			for ( unsigned short ushPolyIndex = 0; ushPolyIndex < oPolyPoly.ushPolyCount; ushPolyIndex++ )
			{	
				if ( oPolyPoly.ppPoints[ushPolyIndex] ) 
					m_oMemoryManager.Free( oPolyPoly.ppPoints[ushPolyIndex] );
			}
			m_oMemoryManager.Free( oPolyPoly.ppPoints );
			m_oMemoryManager.Free( oPolyPoly.pCount );
			return 0;
		}

		if ( CheckError() )
			return 0;

		unsigned long ulParamIndex;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dWidth, pPen->dHeight ) ) / 2;

			ulParamIndex = 1 + oPolyPoly.ushPolyCount;
			for ( unsigned short ushPointCount = 0; ushPointCount < ushNumPars; ushPointCount++ )
			{	
				unsigned short ushX = Record_GetUShortPar( pRecord, ulParamIndex );
				ulParamIndex++;
				unsigned short ushY = Record_GetUShortPar( pRecord, ulParamIndex );
				ulParamIndex++;

				TWmfCoord  oPoint  = WmfCoord( ushX, ushY );
				TWmfCoordF oPointF = TranslateCoordF( oPoint );
				CoordFRegister( oPointF, fScope );
			}
			return 0;
		}

		oPolyPoly.pDC = pPlayer->pDC;

		TWmfRecord oPolygon = Record_OffsetRecord( pRecord, (unsigned long)(1 + oPolyPoly.ushPolyCount) );
		TWmfPolyLine oPolyLine;

		for ( unsigned short ushPolyCount = 0; ushPolyCount < oPolyPoly.ushPolyCount; ushPolyCount++ )
		{	
			oPolyLine.ushCount = oPolyPoly.pCount[ushPolyCount];
			ulParamIndex = 0;

			for ( unsigned short ushPointIndex = 0; ushPointIndex < oPolyLine.ushCount; ushPointIndex++ )
			{	
				unsigned short ushX = Record_GetUShortPar( &oPolygon, ulParamIndex );
				ulParamIndex++;
				unsigned short ushY = Record_GetUShortPar( &oPolygon, ulParamIndex );
				ulParamIndex++;

				TWmfCoord oPoint = WmfCoord( ushX, ushY );
				oPolyPoly.ppPoints[ushPolyCount][ushPointIndex] = TranslateCoordF( oPoint );
			}
			oPolygon = Record_OffsetRecord( &oPolygon, ulParamIndex );
		}

		if ( m_pOutput )
		{
			if ( m_pOutput->IsSupportPolypolygon()  )
			{	
				m_pOutput->Draw_Polypolygon( &oPolyPoly );
			}
			else
			{	
				if ( BS_NULL != oPolyPoly.pDC->pBrush->ushStyle )
				{	
					unsigned short ushStyle = oPolyPoly.pDC->pPen->ushStyle;
					oPolyPoly.pDC->pPen->ushStyle = PS_NULL;

					oPolyLine.pDC = oPolyPoly.pDC;
					oPolyLine.pPoints = (TWmfCoordF*)m_oMemoryManager.Malloc( ushCount * sizeof (TWmfCoordF), _T("Meta_Polygons") );
					oPolyLine.ushCount = 0;

					if ( NULL == oPolyLine.pPoints )
					{	
						m_eError = wmf_error_NotEnoughMemory;
						return 0;
					}

					PolyPoly_To_PolyLine( &oPolyPoly, &oPolyLine, 0 );

					if ( oPolyLine.ushCount > 2 ) 
						m_pOutput->Draw_Polygon( &oPolyLine );

					m_oMemoryManager.Free( oPolyLine.pPoints );

					oPolyPoly.pDC->pPen->ushStyle = ushStyle;
				}
				if ( PS_NULL != ( oPolyPoly.pDC->pPen->ushStyle & PS_STYLE_MASK ) )
				{	
					unsigned short ushStyle = oPolyPoly.pDC->pBrush->ushStyle;
					oPolyPoly.pDC->pBrush->ushStyle = BS_NULL;

					for ( unsigned short ushPolyIndex = 0; ushPolyIndex < oPolyPoly.ushPolyCount; ushPolyIndex++ )
					{	
						oPolyLine.pDC      = oPolyPoly.pDC;
						oPolyLine.pPoints  = oPolyPoly.ppPoints[ushPolyIndex];
						oPolyLine.ushCount = oPolyPoly.pCount[ushPolyIndex];

						if ( ( oPolyLine.ushCount > 2 ) && oPolyLine.pPoints )
						{	
							m_pOutput->Draw_Polygon( &oPolyLine );
						}
					}
					oPolyPoly.pDC->pBrush->ushStyle = ushStyle;
				}
			}
		}

		for ( unsigned short ushPolyIndex = 0; ushPolyIndex < oPolyPoly.ushPolyCount; ushPolyIndex++ )
		{	
			if ( oPolyPoly.ppPoints[ushPolyIndex] )
				m_oMemoryManager.Free( oPolyPoly.ppPoints[ushPolyIndex] );
		}
		m_oMemoryManager.Free( oPolyPoly.ppPoints );
		m_oMemoryManager.Free( oPolyPoly.pCount );

		return 0;
	}


	int Meta_Round        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = Record_GetUShortPar( pRecord, 5 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 4 );

		TWmfCoord oPoint = WmfCoord( ushX, ushY );
		TWmfDrawRectangle oDrawRect;
		oDrawRect.oTL = TranslateCoordF( oPoint );

		ushX = Record_GetUShortPar( pRecord, 3 );
		ushY = Record_GetUShortPar( pRecord, 2 );
		oPoint = WmfCoord( ushX, ushY );
		oDrawRect.oBR = TranslateCoordF( oPoint );

		// Во время сканирования регистрируем крайние точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max) ( pPen->dWidth, pPen->dHeight ) ) / 2;
			CoordFRegister( oDrawRect.oTL, fScope );
			CoordFRegister( oDrawRect.oBR, fScope );
			return 0;
		}

		ushX = Record_GetUShortPar( pRecord, 1 );
		ushY = Record_GetUShortPar( pRecord, 0 );

		oDrawRect.fWidth  = (float) ( (double) ushX * abs( pPlayer->pDC->dPixelWidth  ) );
		oDrawRect.fHeight = (float) ( (double) ushY * abs( pPlayer->pDC->dPixelHeight ) );

		oDrawRect.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Draw_Rectangle( &oDrawRect );

		return 0;
	}

	int Meta_Rect         (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = Record_GetUShortPar( pRecord, 3 );
		unsigned short ushY = Record_GetUShortPar( pRecord, 2 );

		TWmfCoord oPoint = WmfCoord( ushX, ushY );
		TWmfDrawRectangle oDrawRect;
		oDrawRect.oTL = TranslateCoordF( oPoint );

		ushX = Record_GetUShortPar( pRecord, 1 );
		ushY = Record_GetUShortPar( pRecord, 0 );

		oPoint = WmfCoord( ushX, ushY );
		oDrawRect.oBR = TranslateCoordF( oPoint );

		// Во время сканирования регистрируем крайние точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfPen *pPen = pPlayer->pDC->pPen;

            float fScope = (float) ( (std::max)( pPen->dHeight, pPen->dWidth ) ) / 2;

			CoordFRegister( oDrawRect.oTL, fScope );
			CoordFRegister( oDrawRect.oBR, fScope );
			return 0;
		}

		oDrawRect.fHeight = 0;
		oDrawRect.fWidth  = 0;

		oDrawRect.pDC = pPlayer->pDC;

		if ( m_pOutput )
			m_pOutput->Draw_Rectangle( &oDrawRect );

		return 0;
	}

	int Meta_RegionBrush  (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectID_Region = Record_GetUShortPar( pRecord, 0 );
		unsigned short ushObjectID_Brush  = Record_GetUShortPar( pRecord, 1 );

		if ( ( ushObjectID_Region >= m_pFile->pHeader->ushNumberOfObjects ) || ( ushObjectID_Brush >= m_pFile->pHeader->ushNumberOfObjects ) )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		TWmfObject *pObject_Region = pObjects + ushObjectID_Region;
		TWmfObject *pObject_Brush  = pObjects + ushObjectID_Brush;

		if ( ( OBJ_REGION != pObject_Region->nType ) || ( OBJ_BRUSH != pObject_Brush->nType ) )
		{	
			m_eError = wmf_error_Internal;
			return 0;
		}

		TWmfRegion *pRegion = &(pObject_Region->uObject.oRegion);
		TWmfBrush  *pBrush  = &(pObject_Brush->uObject.oBrush);

		float fWidth;
		float fHeight;

		if ( pRecord->unFunction == META_FRAMEREGION )
		{	
			unsigned short ushW = Record_GetUShortPar( pRecord, 3 );
			unsigned short ushH = Record_GetUShortPar( pRecord, 2 );

			fWidth  = (float) ( (double) ushW * abs( pPlayer->pDC->dPixelWidth  ) );
			fHeight = (float) ( (double) ushH * abs( pPlayer->pDC->dPixelHeight ) );
		}
		else
		{	
			fWidth  = 0;
			fHeight = 0;
		}

		// Во время сканирования регистрируем крайние точки региона
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfCoordF oPoint = pRegion->oExtents.oTL;
			oPoint.fX -= fWidth;
			oPoint.fY -= fHeight;
			CoordFRegister( oPoint, 0 );

			oPoint = pRegion->oExtents.oBR;
			oPoint.fX += fWidth;
			oPoint.fY += fHeight;
			CoordFRegister( oPoint, 0 );

			return 0;
		}

		TWmfPolyRectangle oPolyRect;
		oPolyRect.pDC = pPlayer->pDC;

		oPolyRect.pTL = NULL;
		oPolyRect.pBR = NULL;

		oPolyRect.unCount = 0;

		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect ); // Реально ничего не делается

		TWmfRegion *pClip = (TWmfRegion*)pPlayer->pDC->pClip;

        oPolyRect.unCount = (std::max)( pClip->unNumRects, pRegion->unNumRects + 1 );
		oPolyRect.pTL = (TWmfCoordF *)m_oMemoryManager.Malloc( oPolyRect.unCount * sizeof(TWmfCoordF), _T("Meta_RegionBrush") );

		if ( NULL == oPolyRect.pTL )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.pBR = (TWmfCoordF* )m_oMemoryManager.Malloc( oPolyRect.unCount * sizeof(TWmfCoordF), _T("Meta_RegionBrush") );

		if ( NULL == oPolyRect.pBR )
		{	
			m_oMemoryManager.Free( oPolyRect.pTL );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.unCount = pRegion->unNumRects;
		for (unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pRegion->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pRegion->pRects[unIndex].oBR;
		}
		oPolyRect.pTL[oPolyRect.unCount] = pRegion->oExtents.oTL;
		oPolyRect.pBR[oPolyRect.unCount] = pRegion->oExtents.oBR;

		oPolyRect.fWidth  = fWidth;
		oPolyRect.fHeight = fHeight;

		switch (pRecord->unFunction)
		{
		case META_FRAMEREGION:
			if ( m_pOutput )
			{	
				TWmfBrush *pTempBrush = oPolyRect.pDC->pBrush;
				oPolyRect.pDC->pBrush = pBrush;
				
				m_pOutput->Region_Frame( &oPolyRect );

				oPolyRect.pDC->pBrush = pTempBrush;
			}
			break;

		case META_FILLREGION:
			if ( m_pOutput )
			{	
				TWmfBrush *pTempBrush = oPolyRect.pDC->pBrush;
				oPolyRect.pDC->pBrush = pBrush;

				m_pOutput->Region_Paint( &oPolyRect );

				oPolyRect.pDC->pBrush = pTempBrush;			
			}
			break;

		default:

			m_eError = wmf_error_Internal;
			break;
		}

		oPolyRect.unCount = pClip->unNumRects;
		for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pClip->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pClip->pRects[unIndex].oBR;
		}

		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect );

		m_oMemoryManager.Free( oPolyRect.pTL );
		m_oMemoryManager.Free( oPolyRect.pBR );

		return 0;
	}
	int Meta_RegionPaint  (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectID_Region = Record_GetUShortPar( pRecord, 0 );

		if ( ushObjectID_Region >= m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		TWmfObject *pObject_Region = pObjects + ushObjectID_Region;

		if ( OBJ_REGION != pObject_Region->nType )
		{	
			m_eError = wmf_error_Internal;
			return 0;
		}

		TWmfRegion *pRegion = &(pObject_Region->uObject.oRegion);

		// Во время сканирования регистрируем крайние точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			TWmfCoordF oPoint = pRegion->oExtents.oTL;
			CoordFRegister( oPoint, 0 );

			oPoint = pRegion->oExtents.oBR;
			CoordFRegister( oPoint, 0 );

			return 0;
		}

		TWmfPolyRectangle oPolyRect;
		oPolyRect.pDC = pPlayer->pDC;

		oPolyRect.pTL     = NULL;
		oPolyRect.pBR     = NULL;
		oPolyRect.unCount = 0;
		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect ); // Ничего не делается

		TWmfRegion *pClip = (TWmfRegion*)pPlayer->pDC->pClip;

        oPolyRect.unCount = (std::max)( pClip->unNumRects, pRegion->unNumRects + 1 );

		oPolyRect.pTL = (TWmfCoordF *)m_oMemoryManager.Malloc( oPolyRect.unCount * sizeof(TWmfCoordF), _T("Meta_RegionPaint") );
		if ( NULL == oPolyRect.pTL )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.pBR = (TWmfCoordF *)m_oMemoryManager.Malloc( oPolyRect.unCount * sizeof(TWmfCoordF), _T("Meta_RegionPaint") );
		if ( NULL == oPolyRect.pBR )
		{	
			m_oMemoryManager.Free( oPolyRect.pTL );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.unCount = pRegion->unNumRects;
		for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pRegion->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pRegion->pRects[unIndex].oBR;
		}
		oPolyRect.pTL[oPolyRect.unCount] = pRegion->oExtents.oTL;
		oPolyRect.pBR[oPolyRect.unCount] = pRegion->oExtents.oBR;

		switch (pRecord->unFunction)
		{
		case META_INVERTREGION:
			if ( m_pOutput )
			{	
				unsigned short ushTempROP = oPolyRect.pDC->ushROPMode;
				oPolyRect.pDC->ushROPMode = R2_NOT;

				m_pOutput->Region_Paint( &oPolyRect );

				oPolyRect.pDC->ushROPMode = ushTempROP;
			}
			break;

		case META_PAINTREGION:
			if ( m_pOutput )
				m_pOutput->Region_Paint( &oPolyRect );
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		oPolyRect.unCount = pClip->unNumRects;
		for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pClip->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pClip->pRects[unIndex].oBR;
		}

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect );

		m_oMemoryManager.Free( oPolyRect.pTL );
		m_oMemoryManager.Free( oPolyRect.pBR );

		return 0;
	}

	int Meta_RegionCreate (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectIndex = 0;
		while ( pObjects[ushObjectIndex].nType && ( ushObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) ) 
			ushObjectIndex++;

		if ( ushObjectIndex == m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		unsigned short ushObjectID_Region = ushObjectIndex;
		TWmfObject *pObject_Region = pObjects + ushObjectID_Region;

		pObject_Region->nType = OBJ_REGION;

		TWmfRegion *pRegion = &(pObject_Region->uObject.oRegion);

		pRegion->pRects = (TWmfRectF *)m_oMemoryManager.Malloc( 8 * sizeof(TWmfRectF), _T("Meta_RegionCreate") );
		pRegion->unSize = 8;

		if ( NULL == pRegion->pRects )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		WmfSetRectRegion( pRegion, NULL );

		unsigned short ushScanCount = Record_GetUShortPar( pRecord, 5 );

		if ( 0 == ushScanCount ) 
			return 0;

		TWmfRegion oTempRegion;
		oTempRegion.pRects = (TWmfRectF*)m_oMemoryManager.Malloc( 8 * sizeof(TWmfRectF), _T("Meta_RegionCreate") );
		oTempRegion.unSize = 8;

		if ( NULL == oTempRegion.pRects )
		{	
			m_oMemoryManager.Free( pRegion->pRects );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		WmfSetRectRegion( &oTempRegion, NULL );

		TWmfRecord oEnd = Record_OffsetRecord( pRecord, 10 );
		
        unsigned long ulmaxIndex = 10;
		for ( unsigned short ushScanIndex = 0; ushScanIndex < ushScanCount; ushScanIndex++ )
		{	
            ulmaxIndex++;

			TWmfRecord oStart = Record_OffsetRecord( &oEnd, 1 );
			unsigned short ushCount = Record_GetUShortPar( &oStart, 0 );

			// Проверяем значение ushCount на четность
			if ( ushCount & 1 )
			{	
				m_oMemoryManager.Free( pRegion->pRects );
				m_oMemoryManager.Free( oTempRegion.pRects );
				m_eError = wmf_error_BadFormat;
				break;
			}

			unsigned ushPairCount = ushCount >> 1;
            ulmaxIndex += ushCount + 3;

			oEnd = Record_OffsetRecord( &oStart, (unsigned long)( ushCount + 3 ) );

			// В соответствии со спецификацией проверяем совпадение параметров count и count2
			if ( Record_GetUShortPar( &oEnd, 0 ) != ushCount )
			{	
				m_oMemoryManager.Free( pRegion->pRects );
				m_oMemoryManager.Free( oTempRegion.pRects );
				m_eError = wmf_error_BadFormat;
				break;
			}

			unsigned short ushTop    = Record_GetUShortPar( &oStart, 1 );
			unsigned short ushBottom = Record_GetUShortPar( &oStart, 2 );

			for ( unsigned short ushIndex = 0; ushIndex < ushPairCount; ushIndex++ )
			{	
				unsigned short ushLeft  = Record_GetUShortPar( &oStart, (unsigned long) ( 3 + 2 * ushIndex ) );
				unsigned short ushRight = Record_GetUShortPar( &oStart, (unsigned long) ( 4 + 2 * ushIndex ) );

				TWmfRectF oRect;
				GetRectF( &oRect, ushLeft, ushTop, ushRight, ushBottom );

				WmfSetRectRegion( &oTempRegion, &oRect );
				WmfCombineRegion( pRegion, pRegion, &oTempRegion, RGN_OR );
			}
		}

		m_oMemoryManager.Free( oTempRegion.pRects );

		return 0;
	}

	int Meta_ClipSelect   (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectID_Region = Record_GetUShortPar( pRecord, 0 );

		if ( ushObjectID_Region >= m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		TWmfObject *pObject_Region = pObjects + ushObjectID_Region;

		if ( OBJ_REGION != pObject_Region->nType )
		{	
			// Некоторые метафайлы используют данную запись, даже когда ни одного
			// региона не определено в файле. Возможно, в таких случаях область
			// клипа мы должны сделать во весь файл. В любом случае данная ошибка
			// не критична.

			return 0;
		}

		TWmfRegion *pRegion = &(pObject_Region->uObject.oRegion);
		TWmfRegion *pClip   = (TWmfRegion*)pPlayer->pDC->pClip;

		WmfCombineRegion( pClip, pRegion, 0, RGN_COPY );

		return 0;
	}

	int Meta_ClipOffset   (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfRegion *pClip = (TWmfRegion*)pPlayer->pDC->pClip;

		unsigned short ushOffsetX = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushOffsetY = Record_GetUShortPar( pRecord, 0 );

		TWmfCoord oPoint = WmfCoord( ushOffsetX, ushOffsetY );

		for ( unsigned int unIndex = 0; unIndex < pClip->unNumRects; unIndex++ )
		{	
			pClip->pRects[unIndex].oTL.fX += oPoint.unX;
			pClip->pRects[unIndex].oTL.fY += oPoint.unY;
			pClip->pRects[unIndex].oBR.fX += oPoint.unX;
			pClip->pRects[unIndex].oBR.fY += oPoint.unY;
		}
		pClip->oExtents.oTL.fX += oPoint.unX;
		pClip->oExtents.oTL.fY += oPoint.unY;
		pClip->oExtents.oBR.fX += oPoint.unX;
		pClip->oExtents.oBR.fY += oPoint.unY;

		// Во время сканирования в данной функции нам больше ничего не надо делать
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) ) 
			return 0;

		TWmfPolyRectangle oPolyRect;
		oPolyRect.pTL = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_ClipOffset") );

		if ( NULL == oPolyRect.pTL )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.pBR = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_ClipOffset") );

		if ( NULL == oPolyRect.pBR )
		{	
			m_oMemoryManager.Free( oPolyRect.pTL );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.unCount = pClip->unNumRects;
		for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pClip->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pClip->pRects[unIndex].oBR;
		}

		oPolyRect.pDC = pPlayer->pDC;

		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect );

		m_oMemoryManager.Free( oPolyRect.pTL );
		m_oMemoryManager.Free( oPolyRect.pBR );

		return 0;
	}

	int Meta_ClipCombine  (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfRegion *pVisible = &(pPlayer->oVisible);

		unsigned short ushLeft   = Record_GetUShortPar( pRecord, 3 );
		unsigned short ushTop    = Record_GetUShortPar( pRecord, 2 );
		unsigned short ushRight  = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushBottom = Record_GetUShortPar( pRecord, 0 );

		TWmfRectF oRect;
		GetRectF( &oRect, ushLeft, ushTop, ushRight, ushBottom );

		TWmfRegion *pClip = (TWmfRegion*)pPlayer->pDC->pClip;

		switch (pRecord->unFunction)
		{
		case META_EXCLUDECLIPRECT:
			Clipping( pClip, pVisible, &oRect, CLIP_EXCLUDE );
			break;

		case META_INTERSECTCLIPRECT:
			Clipping( pClip, pVisible, &oRect, CLIP_INTERSECT );
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		if ( CheckError() )
			return 0;

		// Во время сканирования в данной функции больше ничего не делаем
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) ) 
			return 0;

		TWmfPolyRectangle oPolyRect;
		oPolyRect.pTL = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_ClipCombine") );

		if ( NULL == oPolyRect.pTL )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.pBR = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_ClipCombine") );

		if ( NULL == oPolyRect.pBR )
		{	
			m_oMemoryManager.Free( oPolyRect.pTL );
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		oPolyRect.unCount = pClip->unNumRects;
		for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
		{	
			oPolyRect.pTL[unIndex] = pClip->pRects[unIndex].oTL;
			oPolyRect.pBR[unIndex] = pClip->pRects[unIndex].oBR;
		}

		oPolyRect.pDC     = pPlayer->pDC;
		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect );

		m_oMemoryManager.Free( oPolyRect.pTL );
		m_oMemoryManager.Free( oPolyRect.pBR );

		return 0;
	}
	int Meta_DIBDraw      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;

		unsigned short ushX = 0;
		unsigned short ushY = 0;
		unsigned short ushW = 0;
		unsigned short ushH = 0;

		if ( ( pRecord->unFunction == META_DIBBITBLT ) && ( ( pRecord->ulSize ) == 9 ) ) // Специальный случай: 2.3.1.2.2
		{	
			return Meta_ROPDraw( pRecord, pAttrlist );
		}


		TWmfBMPRead oBmpRead;
		TWmfBMPDraw oBmpDraw;
		TWmfRecord oBmpRecord;

		switch (pRecord->unFunction)
		{
		case META_SETDIBTODEV:

			ushX = Record_GetUShortPar( pRecord, 8 );
			ushY = Record_GetUShortPar( pRecord, 7 );
			ushW = Record_GetUShortPar( pRecord, 6 );
			ushH = Record_GetUShortPar( pRecord, 5 );

			oBmpDraw.oCrop.ushW = ushW;
			oBmpDraw.oCrop.ushH = ushH;
			oBmpDraw.oCrop.ushX = Record_GetUShortPar( pRecord, 4 );
			oBmpDraw.oCrop.ushY = Record_GetUShortPar( pRecord, 3 );

			oBmpRead.ushWidth  = Record_GetUShortPar( pRecord, 2 ); // Не уверен насчет этого :)
			oBmpRead.ushHeight = Record_GetUShortPar( pRecord, 1 );

			oBmpDraw.unType = SRCCOPY;

			oBmpRecord = Record_OffsetRecord( pRecord, 9 );
			break;

		case META_STRETCHDIB:

			ushX = Record_GetUShortPar( pRecord, 10 );
			ushY = Record_GetUShortPar( pRecord, 9  );
			ushW = Record_GetUShortPar( pRecord, 8  );
			ushH = Record_GetUShortPar( pRecord, 7  );

			oBmpDraw.oCrop.ushX = Record_GetUShortPar( pRecord, 6 );
			oBmpDraw.oCrop.ushY = Record_GetUShortPar( pRecord, 5 );
			oBmpDraw.oCrop.ushW = Record_GetUShortPar( pRecord, 4 );
			oBmpDraw.oCrop.ushH = Record_GetUShortPar( pRecord, 3 );

			oBmpRead.ushWidth  = 0;
			oBmpRead.ushHeight = 0;

			oBmpDraw.unType = (unsigned int)Record_GetUShortPar( pRecord, 0 ) + (((unsigned int)Record_GetUShortPar( pRecord, 1 ) ) << 16);

			oBmpRecord = Record_OffsetRecord( pRecord, 11 );
			break;

		case META_DIBSTRETCHBLT:

			ushX = Record_GetUShortPar( pRecord, 9 );
			ushY = Record_GetUShortPar( pRecord, 8 );
			ushW = Record_GetUShortPar( pRecord, 7 );
			ushH = Record_GetUShortPar( pRecord, 6 );

			oBmpDraw.oCrop.ushX = Record_GetUShortPar( pRecord, 5 );
			oBmpDraw.oCrop.ushY = Record_GetUShortPar( pRecord, 4 );
			oBmpDraw.oCrop.ushW = Record_GetUShortPar( pRecord, 3 );
			oBmpDraw.oCrop.ushH = Record_GetUShortPar( pRecord, 2 );

			oBmpRead.ushWidth  = 0;
			oBmpRead.ushHeight = 0;

			oBmpDraw.unType = (unsigned int)Record_GetUShortPar( pRecord, 0 ) + (((unsigned int)Record_GetUShortPar( pRecord, 1 ) ) << 16);

			oBmpRecord = Record_OffsetRecord( pRecord, 10 );
			break;

		case META_DIBBITBLT: 

			ushX = Record_GetUShortPar( pRecord, 7 );
			ushY = Record_GetUShortPar( pRecord, 6 );
			ushW = Record_GetUShortPar( pRecord, 5 );
			ushH = Record_GetUShortPar( pRecord, 4 );

			oBmpDraw.oCrop.ushX = Record_GetUShortPar( pRecord, 3 );
			oBmpDraw.oCrop.ushY = Record_GetUShortPar( pRecord, 2 );
			oBmpDraw.oCrop.ushW = ushW;
			oBmpDraw.oCrop.ushH = ushH;

			oBmpRead.ushWidth  = 0;
			oBmpRead.ushHeight = 0;

			oBmpDraw.unType = (unsigned int)Record_GetUShortPar( pRecord, 0 ) + (((unsigned int)Record_GetUShortPar( pRecord, 1 ) ) << 16);

			oBmpRecord = Record_OffsetRecord( pRecord, 8 );
			break;

		default:

			m_eError = wmf_error_Internal;
			break;
		}

		if ( CheckError() )
			return 0;

		if ( ( 0 == ushW ) || ( 0 == ushH ) || ( 0 == oBmpDraw.oCrop.ushW ) || ( 0 == oBmpDraw.oCrop.ushH ) )
		{	
			return 0;
		}

		TWmfCoord oPointTL = WmfCoord( ushX, ushY );
		oBmpDraw.oPoint = TranslateCoordF( oPointTL );

		TWmfCoord oPoint = WmfCoord( ushW, ushH );

		int nWidth  = abs( (int)oPoint.unX );
		int nHeight = abs( (int)oPoint.unY );

		// Во время сканирования регистрируем граничные точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			CoordFRegister( oBmpDraw.oPoint, 0 );

			oPoint.unX = oPointTL.unX + nWidth;
			oPoint.unY = oPointTL.unY + nHeight;
			
			TWmfCoordF oPointBR = TranslateCoordF( oPoint );
			CoordFRegister( oPointBR, 0 );

			return 0;
		}

		long lCurPos = Tell();
 
		if ( lCurPos < 0 )
		{	
			m_eError = wmf_error_BadFile;
			return 0;
		}

		oBmpRead.lOffset = oBmpRecord.lPosition;
		oBmpRead.pBuffer = oBmpRecord.sParameter;
		oBmpRead.lLength = (long)(oBmpRecord.ulSize) * 2;

		oBmpRead.oBitmap.pData = NULL;

		if ( m_pOutput )
			m_pOutput->Bmp_Read( &oBmpRead );

		if ( CheckError() || ( NULL == oBmpRead.oBitmap.pData ) )
		{	
			m_eError = wmf_error_Internal;
			return 0;
		}

		if ( (-1) == Seek( lCurPos ) )
		{	
			m_eError = wmf_error_BadFile;
			return 0;
		}

		oBmpDraw.pDC     = pPlayer->pDC;
		oBmpDraw.oBitmap = oBmpRead.oBitmap;

		if ( oBmpDraw.oCrop.ushX >= oBmpRead.oBitmap.ushWidth  ) 
			oBmpDraw.oCrop.ushX = 0;
		if ( oBmpDraw.oCrop.ushY >= oBmpRead.oBitmap.ushHeight ) 
			oBmpDraw.oCrop.ushY = 0;

		if ( oBmpDraw.oCrop.ushX + oBmpDraw.oCrop.ushW >= oBmpRead.oBitmap.ushWidth  )
		{	
			oBmpDraw.oCrop.ushW = oBmpRead.oBitmap.ushWidth - oBmpDraw.oCrop.ushX;
		}
		if ( oBmpDraw.oCrop.ushY + oBmpDraw.oCrop.ushH >= oBmpRead.oBitmap.ushHeight )
		{	
			oBmpDraw.oCrop.ushH = oBmpRead.oBitmap.ushHeight - oBmpDraw.oCrop.ushY;
		}

		double dStretchX = (double) ushW / (double) oBmpDraw.oCrop.ushW;
		double dStretchY = (double) ushH / (double) oBmpDraw.oCrop.ushH;

		oBmpDraw.dPixelWidth  = abs( pPlayer->pDC->dPixelWidth  ) * dStretchX;
		oBmpDraw.dPixelHeight = abs( pPlayer->pDC->dPixelHeight ) * dStretchY;

		if ( m_pOutput )
		{
			m_pOutput->Bmp_Draw( &oBmpDraw );
			m_pOutput->Bmp_Free( &(oBmpDraw.oBitmap) );
		}

		return 0;
	}

	int Meta_DIBBrush     (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned int unObjectIndex = 0;
		while ( pObjects[unObjectIndex].nType && ( unObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) )
			unObjectIndex++;

		if ( m_pFile->pHeader->ushNumberOfObjects == unObjectIndex )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		unsigned short ushObjectID_Brush = unObjectIndex;
		TWmfObject *pObject_Brush = pObjects + ushObjectID_Brush;

		pObject_Brush->nType = OBJ_BRUSH;
		TWmfBrush *pBrush = &(pObject_Brush->uObject.oBrush);

		TWmfRecord oBmpRecord = Record_OffsetRecord( pRecord, 2 );

		long lCurPos = Tell();
		if ( lCurPos < 0 )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}
		
		TWmfBMPRead oBmpRead;
		oBmpRead.lOffset   = oBmpRecord.lPosition;
		oBmpRead.pBuffer   = oBmpRecord.sParameter;
		oBmpRead.lLength   = (long)(oBmpRecord.ulSize) * 2;
		oBmpRead.ushWidth  = 0;
		oBmpRead.ushHeight = 0;

		oBmpRead.oBitmap.ushWidth  = 0;
		oBmpRead.oBitmap.ushHeight = 0;
		oBmpRead.oBitmap.pData     = 0;

		if ( ( pPlayer->ulFlags & PLAYER_PLAY ) && m_pOutput )
			m_pOutput->Bmp_Read( &oBmpRead );

		if ( CheckError() )
			return 0;

		if ( (-1) == Seek( lCurPos ) )
		{	
			m_eError = wmf_error_BadFile;
			return 0;
		}

		pBrush->ushStyle = BS_DIBPATTERN;
		pBrush->oColor   = c_oColorBlack;
		pBrush->oBitmap  = oBmpRead.oBitmap;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
			m_oColorData.Add( &(pBrush->oColor) );

		pPlayer->pDC->pBrush = pBrush;

		return 0;
	}

	int Meta_ROPDraw      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushX = 0;
		unsigned short ushY = 0;
		unsigned short ushW = 0;
		unsigned short ushH = 0;

		TWmfROPDraw oROPDraw;

		switch ( pRecord->unFunction )
		{
		case META_DIBBITBLT: // META_DIBBITBLT: Специальный случай: Size = 9
			ushX = Record_GetUShortPar( pRecord, 8 );
			ushY = Record_GetUShortPar( pRecord, 7 );

			ushW = Record_GetUShortPar( pRecord, 6 );
			ushH = Record_GetUShortPar( pRecord, 5 );

			oROPDraw.unROP = (unsigned int)Record_GetUShortPar( pRecord, 0 ) + (((unsigned int)Record_GetUShortPar( pRecord, 1 ) ) << 16);
			break;

		case META_PATBLT:

			ushX = Record_GetUShortPar( pRecord, 5 );
			ushY = Record_GetUShortPar( pRecord, 4 );

			ushW = Record_GetUShortPar( pRecord, 3 );
			ushH = Record_GetUShortPar( pRecord, 2 );

			oROPDraw.unROP = (unsigned int)Record_GetUShortPar( pRecord, 0 ) + (((unsigned int)Record_GetUShortPar( pRecord, 1 ) ) << 16);
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		if ( CheckError() )
			return 0;

		TWmfCoord oPointTL = WmfCoord( ushX, ushY );
		oROPDraw.oTL = TranslateCoordF( oPointTL );

		TWmfCoord oPoint = WmfCoord( ushW, ushH );

		int nWidth  = abs( (int)oPoint.unX );
		int nHeight = abs( (int)oPoint.unY );

		oPoint.unX = oPointTL.unX + nWidth;
		oPoint.unY = oPointTL.unY + nHeight;

		oROPDraw.oBR = TranslateCoordF( oPoint );

		// Во время сканирования регистрируем граничные точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			CoordFRegister( oROPDraw.oTL, 0 );
			CoordFRegister( oROPDraw.oBR, 0 );
			return 0;
		}

		oROPDraw.pDC          = pPlayer->pDC;
		oROPDraw.dPixelWidth  = abs( pPlayer->pDC->dPixelWidth  );
		oROPDraw.dPixelHeight = abs( pPlayer->pDC->dPixelHeight );

		if ( m_pOutput )
			m_pOutput->Rop_Draw( &oROPDraw );

		return 0;
	}
	int Meta_DCSet        (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushParameter = Record_GetUShortPar( pRecord, 0 );

		switch (pRecord->unFunction)
		{
		case META_SETROP2: // 2.3.5.22
			pPlayer->pDC->ushROPMode = ushParameter;
			break;

		case META_SETTEXTJUSTIFICATION: // 2.3.5.27
			// TO DO: Здесь должно считываться два параметра
			pPlayer->pDC->ushBreakExtra = ushParameter;
			break;

		case META_SETTEXTCHAREXTRA: // 2.3.5.25
			pPlayer->pDC->ushCharExtra = ushParameter;
			break;

		case META_SETPOLYFILLMODE: // 2.3.5.20
			pPlayer->pDC->ushPolyFillMode = ushParameter;
			break;

		case META_SETTEXTALIGN: // 2.3.5.24
			pPlayer->pDC->ushTextAlign = ushParameter;
			break;

		case META_SETBKMODE: // 2.3.5.15
			if ( TRANSPARENT == ushParameter )
			{	
				pPlayer->pDC->ushBGMode = TRANSPARENT;
			}
			else
			{	
				pPlayer->pDC->ushBGMode = OPAQUE;
				if ( OPAQUE != ushParameter )
				{
					// TO DO: Такого быть не должно, но данная ошибка не критичная
				}
			}
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		return 0;
	}
	int Meta_DCColor      (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushB  = Record_GetUShortPar( pRecord, 1 );
		unsigned short ushRG = Record_GetUShortPar( pRecord, 0 );

		TWmfRGB oColor = CWmfColor::LongToRGB( ushRG, ushB );

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) ) 
			m_oColorData.Add( &oColor );

		switch (pRecord->unFunction)
		{
		case META_SETTEXTCOLOR: // 2.3.5.26
			pPlayer->pDC->oTextColor = oColor;
			break;

		case META_SETBKCOLOR: // 2.3.5.14
			pPlayer->pDC->oBGColor = oColor;
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		return 0;
	}
	int Meta_DCSelect     (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectID = Record_GetUShortPar( pRecord, 0 );

		if ( ushObjectID >= m_pFile->pHeader->ushNumberOfObjects )
		{
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		TWmfObject *pObject = pObjects + ushObjectID;

		switch ( pObject->nType )
		{
		case OBJ_BRUSH:
			pPlayer->pDC->pBrush = &(pObject->uObject.oBrush);
			break;

		case OBJ_PEN:
			pPlayer->pDC->pPen = &(pObject->uObject.oPen);
			break;

		case OBJ_FONT:
			pPlayer->pDC->pFont = &(pObject->uObject.oFont);
			break;

		default:

			// Согласно спецификации, смена палитры данной функцией не осуществляется
			break;
		}

		return 0;
	}

	int Meta_DCSave       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist) /* complete ?? */
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		DCStackPush( pPlayer->pDC );

		pPlayer->pDC = CopyDC( pPlayer->pDC );

		return 0;
	}

	int Meta_DCRestore    (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfUserData oUserData;
		oUserData.pDC   = pPlayer->pDC;

		TWmfRegion *pClip = (TWmfRegion*)pPlayer->pDC->pClip;

		m_oMemoryManager.Free( pClip->pRects );
		m_oMemoryManager.Free( pPlayer->pDC->pClip );
		m_oMemoryManager.Free( pPlayer->pDC );

		pPlayer->pDC = DCStackPop();

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
			return 0;

		oUserData.pDC   = pPlayer->pDC;

		pClip = (TWmfRegion*)pPlayer->pDC->pClip;

		TWmfPolyRectangle oPolyRect;
		oPolyRect.pDC     = pPlayer->pDC;
		oPolyRect.fWidth  = 0;
		oPolyRect.fHeight = 0;

		if ( pClip->unNumRects )
		{	
			oPolyRect.pTL = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_DCRestore") );

			if ( NULL == oPolyRect.pTL )
			{	
				m_eError = wmf_error_NotEnoughMemory;
				return 0;
			}

			oPolyRect.pBR = (TWmfCoordF *)m_oMemoryManager.Malloc( pClip->unNumRects * sizeof(TWmfCoordF), _T("Meta_DCRestore") );

			if ( NULL == oPolyRect.pBR )
			{	
				m_oMemoryManager.Free( oPolyRect.pTL );
				m_eError = wmf_error_NotEnoughMemory;
				return 0;
			}

			oPolyRect.unCount = pClip->unNumRects;
			for ( unsigned int unIndex = 0; unIndex < oPolyRect.unCount; unIndex++ )
			{	
				oPolyRect.pTL[unIndex] = pClip->pRects[unIndex].oTL;
				oPolyRect.pBR[unIndex] = pClip->pRects[unIndex].oBR;
			}

			m_oMemoryManager.Free( oPolyRect.pTL );
			m_oMemoryManager.Free( oPolyRect.pBR );
		}
		else
		{	
			oPolyRect.pTL = NULL;
			oPolyRect.pBR = NULL;
			oPolyRect.unCount = 0;
		}

		if ( m_pOutput )
			m_pOutput->Region_Clip( &oPolyRect );

		return 0;
	}

	int Meta_Text         (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		unsigned short ushLength = 0;

		TWmfCoord oPoint;
		unsigned short ushStartX;
		unsigned short ushStartY;
		TWmfDrawText oDrawText;

		TWmfRecord oStringRecord;

		unsigned short ushBBoxInfo = 0;

		switch ( pRecord->unFunction )
		{
		case META_TEXTOUT:

			if ( pPlayer->pDC->ushTextAlign & TA_UPDATECP )
			{	
				if ( ( pRecord->ulSize ) < (unsigned long)( 1 + (ushLength + 1) / 2 ) )
				{	
					m_eError = wmf_error_BadFormat;
					break;
				}

				oPoint = pPlayer->oCurrent;
			}
			else
			{	
				if ( ( pRecord->ulSize ) < (unsigned long)( 3 + (ushLength + 1) / 2 ) )
				{	
					m_eError = wmf_error_BadFormat;
					break;
				}

				ushStartX = Record_GetUShortPar( pRecord, pRecord->ulSize - 1 );
				ushStartY = Record_GetUShortPar( pRecord, pRecord->ulSize - 2 );

				oPoint = WmfCoord( ushStartX, ushStartY );
			}

			oDrawText.oPoint = TranslateCoordF( oPoint );

			ushLength = Record_GetUShortPar( pRecord, 0 );

			if ( 0 == ushLength ) 
				break;

			oDrawText.oTL.fX = 0;
			oDrawText.oTL.fY = 0;
			oDrawText.oBR.fX = 0;
			oDrawText.oBR.fY = 0;

			oStringRecord = Record_OffsetRecord( pRecord, 1 );
			break;

		case META_EXTTEXTOUT:

			if ( pPlayer->pDC->ushTextAlign & TA_UPDATECP )
			{	
				oPoint = pPlayer->oCurrent;
			}
			else
			{	
				ushStartX = Record_GetUShortPar( pRecord, 1 );
				ushStartY = Record_GetUShortPar( pRecord, 0 );

				oPoint = WmfCoord( ushStartX, ushStartY );
			}

			oDrawText.oPoint = TranslateCoordF( oPoint );

			ushLength = Record_GetUShortPar( pRecord, 2 );

			if ( 0 == ushLength ) 
				break;

			ushBBoxInfo = Record_GetUShortPar( pRecord, 3 );
			if ( ushBBoxInfo )
			{	
				ushStartX = Record_GetUShortPar( pRecord, 4 );
				ushStartY = Record_GetUShortPar( pRecord, 5 );

				oPoint = WmfCoord( ushStartX, ushStartY );

				oDrawText.oTL = TranslateCoordF( oPoint );

				ushStartX  = Record_GetUShortPar( pRecord, 6 );
				ushStartY  = Record_GetUShortPar( pRecord, 7 );

				oPoint = WmfCoord( ushStartX, ushStartY );

				oDrawText.oBR = TranslateCoordF( oPoint );

				if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
				{	
					CoordFRegister( oDrawText.oTL, 0 );
					CoordFRegister( oDrawText.oBR, 0 );
				}

				oStringRecord = Record_OffsetRecord( pRecord, 8 );
			}
			else
			{	
				oDrawText.oTL.fX = 0;
				oDrawText.oTL.fY = 0;
				oDrawText.oBR.fX = 0;
				oDrawText.oBR.fY = 0;

				oStringRecord = Record_OffsetRecord( pRecord, 4 );
			}
			break;

		default:
			m_eError = wmf_error_Internal;
			break;
		}

		if ( CheckError() || 0 == ushLength ) 
			return 0;

		oDrawText.sText = (char*)m_oMemoryManager.Malloc( ushLength + 1, _T("Meta_DrawText") );

		if ( NULL == oDrawText.sText )
		{
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		TWmfFont *pFont = pPlayer->pDC->pFont;

		// TO DO: Возможна отрицательная высота шрифта
		oDrawText.dFontHeight = (double)pFont->ushHeight * abs( pPlayer->pDC->dPixelHeight );

		// TO DO: Ширина и высота шрифта не обязательно имеют одинаковый коэффициент растяжения/сжатия
		if ( 0 == pFont->ushHeight )
		{
			m_oMemoryManager.Free( oDrawText.sText );
			m_eError = wmf_error_Internal;
			return 0;
		}
		oDrawText.dFontRatio  = (double)pFont->ushWidth / (double)pFont->ushHeight;

		unsigned short ushChar = 0;
		for (unsigned short ushIndex = 0; ushIndex < ushLength; ushIndex++ )
		{	
			if ( ushIndex & 1 )
			{	
				oDrawText.sText[ushIndex] =  (ushChar >> 8) & 0xff;
			}
			else
			{	
				ushChar = Record_GetUShortPar( &oStringRecord, ushIndex / 2 );
				oDrawText.sText[ushIndex] = ushChar & 0xff;
			}
		}
		oDrawText.sText[ushLength] = '\0';
#ifndef DESKTOP_EDITOR_GRAPHICS
		USES_CONVERSION;
#endif


		float fWidth = 0;
		if ( m_pFontManager )
		{
			long lStyle = ( pFont->ushWeight > 550 ? 1 : 0 ) + ( pFont->unItalic ? 2 : 0 );
			//m_pFontManager->LoadFontByName( A2W(pPlayer->pDC->pFont->sFaceName), (float)oDrawText.dFontHeight, lStyle, 72.0f, 72.0f );
			//m_pFontManager->LoadStringW( A2W(oDrawText.sText), 0, 0 );

			float fTempX, fTempY, fTempW, fTempH;
#ifdef DESKTOP_EDITOR_GRAPHICS
			m_pFontManager->LoadFontByName( ascii_to_unicode(pPlayer->pDC->pFont->sFaceName), (float)(oDrawText.dFontHeight * 72 / 25.4), lStyle, 72.0f, 72.0f );
			m_pFontManager->LoadString1( ascii_to_unicode(oDrawText.sText), 0, 0 );

			TBBox oBox = m_pFontManager->MeasureString2();
			fTempX = oBox.fMinX;
			fTempY = oBox.fMinY;
            fTempW = oBox.fMaxX - oBox.fMinX;
            fTempH = oBox.fMaxX - oBox.fMinX;
#else
			m_pFontManager->LoadFontByName( A2W(pPlayer->pDC->pFont->sFaceName), (float)(oDrawText.dFontHeight * 72 / 25.4), lStyle, 72.0f, 72.0f );
			m_pFontManager->LoadStringW( A2W(oDrawText.sText), 0, 0 );

			m_pFontManager->MeasureString2( &fTempX, &fTempY, &fTempW, &fTempH );
#endif

			fWidth = fTempW * 25.4f / 72.0f;	
		}


		fWidth = (float) ( (double) fWidth * oDrawText.dFontRatio );

		oDrawText.oOrigin = oDrawText.oPoint;

		TWmfRecord oDxRecord = Record_OffsetRecord( &oStringRecord, ( ushLength + 1 ) / 2 );
		unsigned short *pushDx = NULL;
		unsigned short ushWidth;

		TWmfCoordF oPointT;
		TWmfCoordF oPointD;

		oDrawText.bUseDx = false;
		if ( ( META_EXTTEXTOUT == pRecord->unFunction ) && ( ( oDxRecord.ulSize ) >= ushLength ) )
		{	
			pushDx = (unsigned short *)m_oMemoryManager.Malloc( ushLength * sizeof(unsigned short), _T("Meta_DrawText function") );
			
			if ( NULL == pushDx )
			{	
				m_oMemoryManager.Free( oDrawText.sText );
				m_eError = wmf_error_NotEnoughMemory;
				return 0;
			}

			ushWidth = 0;
			for ( unsigned short ushIndex = 0; ushIndex < ushLength; ushIndex++ )
			{	
				pushDx[ushIndex] = Record_GetUShortPar( &oDxRecord, ushIndex );
				ushWidth += pushDx[ushIndex];
			}

			oPoint = WmfCoord( 0, 0 );
			oPointT = TranslateCoordF( oPoint );

			oPoint = WmfCoord( ushWidth, 0 );
			oPointD = TranslateCoordF( oPoint );

			fWidth = oPointD.fX - oPointT.fX;

			oDrawText.bUseDx = true;
		}

		double dTheta = - ( ( ( (double)pFont->shEscapement ) / 10 ) * M_PI / 180 );

		float fCosTheta = (float)cos( dTheta );
		float fSinTheta = (float)sin( dTheta );

		if ( pPlayer->pDC->ushTextAlign & TA_BASELINE )
		{	
			// Ничего не делаем
		}
		else if ( pPlayer->pDC->ushTextAlign & TA_BOTTOM )
		{	
			oPointD.fY = (float) ( -oDrawText.dFontHeight / 3 ); // Аппроксимация
			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;
			oDrawText.oPoint.fX += oPointT.fX;
			oDrawText.oPoint.fY += oPointT.fY;
		}
		else // if ( pPlayer->pDC->ushTextAlign & TA_TOP )
		{	
			oPointD.fY = (float) oDrawText.dFontHeight; // Аппроксимация
			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;
			oDrawText.oPoint.fX += oPointT.fX;
			oDrawText.oPoint.fY += oPointT.fY;
		}

		if ( pPlayer->pDC->ushTextAlign & TA_CENTER )
		{	
			oPointD.fX = fWidth / 2;
			oPointD.fY = (float) ( -oDrawText.dFontHeight * 0.77 );
			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oTR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oTR.fY = oDrawText.oPoint.fY + oPointT.fY;

			CoordFRegister( oDrawText.oBBox.oTR, 0 );

			oDrawText.oBBox.oBL.fX = oDrawText.oPoint.fX - oPointT.fX;
			oDrawText.oBBox.oBL.fY = oDrawText.oPoint.fY - oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBL, 0 );

			oPointD.fX = fWidth / 2;
			oPointD.fY = (float) ( oDrawText.dFontHeight * 0.23 ); 
			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oBR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oBR.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBR, 0 );

			oDrawText.oBBox.oTL.fX = oDrawText.oPoint.fX - oPointT.fX;
			oDrawText.oBBox.oTL.fY = oDrawText.oPoint.fY - oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oTL, 0 );

			oPointD.fX = - fWidth / 2;
			oPointT.fX = oPointD.fX * fCosTheta;
			oPointT.fY = oPointD.fX * fSinTheta;
			oDrawText.oPoint.fX += oPointT.fX;
			oDrawText.oPoint.fY += oPointT.fY;
		}
		else if ( pPlayer->pDC->ushTextAlign & TA_RIGHT )
		{	
			oPointD.fY = (float)( -oDrawText.dFontHeight * 0.77 );

			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;

			oDrawText.oBBox.oTR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oTR.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oTR, 0 );

			oPointD.fY = (float)( oDrawText.dFontHeight * 0.23 );

			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;

			oDrawText.oBBox.oBR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oBR.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBR, 0 );

			oPointD.fX = -fWidth;
			oPointD.fY = (float)( -oDrawText.dFontHeight * 0.77 );

			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oTL.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oTL.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oTL, 0 );

			oPointD.fX = -fWidth;
			oPointD.fY = (float)( oDrawText.dFontHeight * 0.23 );

			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oBL.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oBL.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBL, 0 );

			oPointD.fX = -fWidth;

			oPointT.fX = oPointD.fX * fCosTheta;
			oPointT.fY = oPointD.fX * fSinTheta;
			oPointD.fX = oDrawText.oPoint.fX + oPointT.fX;
			oPointD.fY = oDrawText.oPoint.fY + oPointT.fY;

			oDrawText.oPoint = oPointD;

			if ( pPlayer->pDC->ushTextAlign & TA_UPDATECP )
			{	
				pPlayer->oCurrent = TranslateCoord( oDrawText.oPoint );
			}
		}
		else // if ( pPlayer->pDC->ushTextAlign & TA_LEFT )
		{	
			oPointD.fY = (float)( -oDrawText.dFontHeight * 0.77 );

			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;

			oDrawText.oBBox.oTL.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oTL.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oTL, 0 );

			oPointD.fY = (float)( oDrawText.dFontHeight * 0.23 );

			oPointT.fX = -oPointD.fY * fSinTheta;
			oPointT.fY =  oPointD.fY * fCosTheta;

			oDrawText.oBBox.oBL.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oBL.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBL, 0 );

			oPointD.fX = fWidth;
			oPointD.fY = (float)( -oDrawText.dFontHeight * 0.77 );

			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oTR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oTR.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oTR, 0 );

			oPointD.fX = fWidth;
			oPointD.fY = (float)( oDrawText.dFontHeight * 0.23 );

			oPointT.fX = oPointD.fX * fCosTheta - oPointD.fY * fSinTheta;
			oPointT.fY = oPointD.fX * fSinTheta + oPointD.fY * fCosTheta;

			oDrawText.oBBox.oBR.fX = oDrawText.oPoint.fX + oPointT.fX;
			oDrawText.oBBox.oBR.fY = oDrawText.oPoint.fY + oPointT.fY;
			CoordFRegister( oDrawText.oBBox.oBR, 0 );

			if ( pPlayer->pDC->ushTextAlign & TA_UPDATECP )
			{	
				oPointD.fX = fWidth;
				oPointT.fX = oPointD.fX * fCosTheta;
				oPointT.fY = oPointD.fX * fSinTheta;
				oPointD.fX = oDrawText.oPoint.fX + oPointT.fX;
				oPointD.fY = oDrawText.oPoint.fY + oPointT.fY;

				pPlayer->oCurrent = TranslateCoord( oDrawText.oPoint );
			}
		}

		// Во время сканирования мы только регистриуем граничные точки
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
		{	
			m_oMemoryManager.Free( oDrawText.sText );
			
			if ( pushDx ) 
				m_oMemoryManager.Free( pushDx );

			return 0;
		}

		oDrawText.pDC      = pPlayer->pDC;
		oDrawText.ushFlags = ushBBoxInfo;

		TWmfCoordF oPointO;
		if ( pushDx )
		{	
			char *sStringSave = oDrawText.sText;
			char sBuffer[2];
			
			oDrawText.sText = sBuffer;
			oPointT = oDrawText.oPoint;

			double dRatio = oDrawText.dFontRatio;
			
			ushWidth = 0;
			
			for ( unsigned short ushIndex = 0; ushIndex < ushLength; ushIndex++ )
			{	
				sBuffer[0] = sStringSave[ushIndex];
				sBuffer[1] = 0;

				oPoint  = WmfCoord( 0, 0 );
				oPointO = TranslateCoordF( oPoint );

				oPoint  = WmfCoord( ushWidth, 0 );
				oPointD = TranslateCoordF( oPoint );

				oPointD.fX -= oPointO.fX;
				oPointD.fY -= oPointO.fY;
				
				oDrawText.oPoint.fX = oPointT.fX + oPointD.fX * fCosTheta;
				oDrawText.oPoint.fY = oPointT.fY + oPointD.fX * fSinTheta;

				oPoint  = WmfCoord( 0, 0 );
				oPointO = TranslateCoordF( oPoint );

				oPoint  = WmfCoord( pushDx[ushIndex], 0 );
				oPointD = TranslateCoordF( oPoint );
				oPointD.fX -= oPointO.fX;
				oPointD.fY -= oPointO.fY;

				fWidth = 100; // Временно
				//fWidth = FD->stringwidth (API,font,drawtext.str);
				fWidth = (float) ( (double)fWidth * oDrawText.dFontHeight * dRatio );

				if ( oPointD.fX < fWidth )
				{	
					oDrawText.dFontRatio = dRatio * (oPointD.fX / fWidth);
				}
				else
				{	
					oDrawText.dFontRatio = dRatio;
				}

				if ( m_pOutput )
					m_pOutput->Draw_Text( &oDrawText );

				ushWidth += pushDx[ushIndex];

				oDrawText.oBBox.oTL.fX = 0;
				oDrawText.oBBox.oTL.fY = 0;
				oDrawText.oBBox.oTR.fX = 0;
				oDrawText.oBBox.oTR.fY = 0;
				oDrawText.oBBox.oBL.fX = 0;
				oDrawText.oBBox.oBL.fY = 0;
				oDrawText.oBBox.oBR.fX = 0;
				oDrawText.oBBox.oBR.fY = 0;
			}
			
			m_oMemoryManager.Free( pushDx );

			oDrawText.sText = sStringSave;
		}
		else
		{	
			if ( m_pOutput )
				m_pOutput->Draw_Text( &oDrawText );
		}

		m_oMemoryManager.Free( oDrawText.sText );

		return 0;
	}

	int Meta_PenCreate    (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectIndex = 0;
		while ( pObjects[ushObjectIndex].nType && ( ushObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) )
			ushObjectIndex++;

		if ( m_pFile->pHeader->ushNumberOfObjects == ushObjectIndex )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		unsigned short ushObjectID_Pen = ushObjectIndex;
		TWmfObject *pObject_Pen = pObjects + ushObjectID_Pen;

		pObject_Pen->nType = OBJ_PEN;
		TWmfPen *pPen = &(pObject_Pen->uObject.oPen);

		unsigned short ushStyle = Record_GetUShortPar( pRecord, 0 );
		
		pPen->ushStyle = ( pPen->ushStyle & ~PS_STYLE_MASK  ) | ( ushStyle & PS_STYLE_MASK  );
		pPen->ushStyle = ( pPen->ushStyle & ~PS_ENDCAP_MASK ) | ( ushStyle & PS_ENDCAP_MASK );
		pPen->ushStyle = ( pPen->ushStyle & ~PS_JOIN_MASK   ) | ( ushStyle & PS_JOIN_MASK   );
		pPen->ushStyle = ( pPen->ushStyle & ~PS_TYPE_MASK   ) | ( ushStyle & PS_TYPE_MASK   );

		unsigned short ushWidth = Record_GetUShortPar( pRecord, 1 );
        ushWidth = (std::max)( (unsigned short) 1, ushWidth );

		pPen->dWidth  = (double) ushWidth * abs( pPlayer->pDC->dPixelWidth  );
		pPen->dHeight = (double) ushWidth * abs( pPlayer->pDC->dPixelHeight );

		unsigned short ushB  = Record_GetUShortPar( pRecord, 4 );
		unsigned short ushRG = Record_GetUShortPar( pRecord, 3 );

		TWmfRGB oColor = CWmfColor::LongToRGB( ushRG, ushB );
		pPen->oColor = oColor;

		// Во время сканирования регистрируем цвет
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
			m_oColorData.Add( &oColor );

		pPlayer->pDC->pPen = pPen;

		return 0;
	}
	int Meta_BrushCreate  (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectIndex = 0;
		while ( pObjects[ushObjectIndex].nType && ( ushObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) ) 
			ushObjectIndex++;

		if ( m_pFile->pHeader->ushNumberOfObjects == ushObjectIndex )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		unsigned short ushObjectID_Brush = ushObjectIndex;
		TWmfObject *pObject_Brush = pObjects + ushObjectID_Brush;

		pObject_Brush->nType = OBJ_BRUSH;
		TWmfBrush *pBrush = &(pObject_Brush->uObject.oBrush);

		pBrush->ushStyle = Record_GetUShortPar( pRecord, 0 );

		unsigned short ushB  = Record_GetUShortPar( pRecord, 2 );
		unsigned short ushRG = Record_GetUShortPar( pRecord, 1 );

		TWmfRGB oColor = CWmfColor::LongToRGB( ushRG, ushB );

		pBrush->oColor   = oColor;
		pBrush->ushHatch = Record_GetUShortPar( pRecord, 3 );

		TWmfBMP oBitmap;
		oBitmap.ushWidth  = 0;
		oBitmap.ushHeight = 0;
		oBitmap.pData     = NULL;

		pBrush->oBitmap = oBitmap;

		// Во время сканирования регистрируем цвет
		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) )
			m_oColorData.Add( &oColor );

		pPlayer->pDC->pBrush = pBrush;

		return 0;
	}

	int Meta_FontCreate   (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;
		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectIndex = 0;
		while ( pObjects[ushObjectIndex].nType && ( ushObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) ) 
			ushObjectIndex++;

		if ( ushObjectIndex == m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		unsigned short ushObjectID_Font = ushObjectIndex;
		TWmfObject *pObject_Font = pObjects + ushObjectID_Font;

		pObject_Font->nType = OBJ_FONT;
		TWmfFont *pFont = &(pObject_Font->uObject.oFont);

		short shWidth  = Record_GetShortPar( pRecord, 1 );
		short shHeight = Record_GetShortPar( pRecord, 0 );

		pFont->ushWidth  = abs( shWidth  );
		pFont->ushHeight = abs( shHeight );

		pFont->shEscapement  = Record_GetShortPar( pRecord, 2 ); // Угол текса
		pFont->shOrientation = Record_GetShortPar( pRecord, 3 );

		pFont->ushWeight = Record_GetUShortPar( pRecord, 4 );

		unsigned short ushParam;
		ushParam = Record_GetUShortPar( pRecord, 5 );
		pFont->unItalic    = ushParam & 0xff;
		pFont->unUnderline = (ushParam >> 8) & 0xff;

		ushParam = Record_GetUShortPar( pRecord, 6 );
		pFont->unStrikeOut = ushParam & 0xff;
		pFont->unCharSet   = (ushParam >> 8) & 0xff;

		// В спецификации Wmf сказано, что значение DEFAULT_CHARSET может означать, что
		// шрифт ищется только по имени и размеру.
#ifdef DESKTOP_EDITOR_GRAPHICS
		//todo
		//if ( DEFAULT_CHARSET == pFont->unCharSet )
		//	pFont->unCharSet = GetDefaultCharset( FALSE );
#else
		if ( DEFAULT_CHARSET == pFont->unCharSet )
			pFont->unCharSet = GetDefaultCharset( FALSE );
#endif

		ushParam = Record_GetUShortPar( pRecord, 7 );
		pFont->unOutPrecision  = ushParam & 0xff;
		pFont->unClipPrecision = (ushParam >> 8) & 0xff;

		ushParam = Record_GetUShortPar( pRecord, 8 );
		pFont->unQuality        = ushParam & 0xff;
		pFont->unPitchAndFamily = (ushParam >> 8) & 0xff;

		if ( 0 == pFont->ushWidth )
		{	
			pFont->ushWidth = pFont->ushHeight;
		}

		TWmfRecord oNameRecord = Record_OffsetRecord( pRecord, 9 );
		unsigned long lLength = (oNameRecord.ulSize) * 2;

		char *sFontName = (char*)m_oMemoryManager.Malloc( lLength + 1, _T("Meta_FontCreate function") );

		if ( NULL == sFontName )
		{	
			m_eError = wmf_error_NotEnoughMemory;
			return 0;
		}

		unsigned short ushChar = 0;
		for ( unsigned long ulIndex = 0; ulIndex < lLength; ulIndex++ )
		{	
			if ( ulIndex & 1 )
			{	
				sFontName[ulIndex] = (ushChar >> 8) & 0xff;
			}
			else
			{	
				ushChar = Record_GetUShortPar( &oNameRecord, ulIndex / 2 );
				sFontName[ulIndex] = ushChar & 0xff;
			}
		}
		sFontName[lLength] = '\0';

		pFont->sFaceName = sFontName;

		if ( m_pFontManager )
		{
			// Проверим есть ли такой шрифт в системе, если запишем имя ближайшайшего шрифта
#ifdef DESKTOP_EDITOR_GRAPHICS
			std::wstring bsFontName = ascii_to_unicode( pFont->sFaceName );
#else
			BSTR bsFontName = CString( pFont->sFaceName ).AllocSysString();
#endif

			BOOL bNeedFindByParams = FALSE;
#ifdef DESKTOP_EDITOR_GRAPHICS
			if ( TRUE == m_pFontManager->LoadFontByName( bsFontName, 1, 0, 0, 0 ) )
			{
				unsigned long ulBit = 0;
				unsigned int unIndex = 0;
				//todo
				//if ( UNKNOWN_CHARSET != pFont->unCharSet )
				//{
				//	GetCodePageByCharset( pFont->unCharSet, &ulBit, &unIndex );
				//	VARIANT_BOOL vbSuccess;
				//	m_pFontManager->IsUnicodeRangeAvailable( ulBit, unIndex, &vbSuccess );
				//	if ( VARIANT_TRUE != vbSuccess )
				//		bNeedFindByParams = TRUE;
				//}
#else
			if ( S_OK == m_pFontManager->LoadFontByName( bsFontName, 1, 0, 0, 0 ) )
			{
				unsigned long ulBit = 0;
				unsigned int unIndex = 0;
				if ( UNKNOWN_CHARSET != pFont->unCharSet )
				{
					GetCodePageByCharset( pFont->unCharSet, &ulBit, &unIndex );
					VARIANT_BOOL vbSuccess;
					m_pFontManager->IsUnicodeRangeAvailable( ulBit, unIndex, &vbSuccess );
					if ( VARIANT_TRUE != vbSuccess )
						bNeedFindByParams = TRUE;
				}
#endif
			}
			else
				bNeedFindByParams = TRUE;
			
			if ( bNeedFindByParams )
			{
#ifdef DESKTOP_EDITOR_GRAPHICS
				CFontSelectFormat oFontSelectFormat;
                oFontSelectFormat.wsName = new std::wstring(bsFontName);
				oFontSelectFormat.unCharset = new BYTE;
				*oFontSelectFormat.unCharset = pFont->unCharSet;
				oFontSelectFormat.usWeight = new USHORT;
				*oFontSelectFormat.usWeight = pFont->ushWeight;
				
				CFontInfo* pFontInfo = m_pFontManager->GetFontInfoByParams(oFontSelectFormat);
				if ( NULL != pFontInfo)
				{
					long lStyle = ( pFontInfo->m_bBold ? 1 : 0 ) + ( pFontInfo->m_bItalic ? 2 : 0 );
					if ( TRUE == m_pFontManager->LoadFontByName( pFontInfo->m_wsFontName, 11, lStyle, 96, 96 ) )
					{
						free( pFont->sFaceName );
						std::string sNewName = unicode_to_ascii( pFontInfo->m_wsFontName.c_str());
						int nLen = sNewName.length();
						pFont->sFaceName = (char*)m_oMemoryManager.Malloc( nLen + 1, _T("Meta_FontCreate function") );
						if ( !pFont->sFaceName )
							m_eError = wmf_error_NotEnoughMemory;
						else
						{
							pFont->sFaceName[nLen] = '\0';
							memcpy( pFont->sFaceName, sNewName.c_str(), nLen );
						}
					}
				}
			}
#else
				USES_CONVERSION;
				::SysFreeString( bsFontName );
				CString sXml;
				sXml.Format( _T("<FontProperties><Name value='%ls'/><Charset value='%X'/><Weight value='%d'/></FontProperties>"), A2W(pFont->sFaceName), pFont->unCharSet, pFont->ushWeight);
				BSTR bsXml = sXml.AllocSysString();
				BSTR bsPath; LONG lIndex = 0;

				m_pFontManager->GetWinFontByParams( bsXml, &bsPath, &lIndex );
				::SysFreeString( bsXml );

				if ( S_OK == m_pFontManager->LoadFontFromFile( bsPath, 1, 0, 0, lIndex ) )
				{
					m_pFontManager->GetFamilyName( &bsFontName );

					if ( S_OK == m_pFontManager->LoadFontByName( bsFontName, 1, 0, 0, 0 ) )
					{
						free( pFont->sFaceName );
						char *sNewName = W2A( bsFontName );
						int nLen = (int)strlen( sNewName );
						pFont->sFaceName = (char*)m_oMemoryManager.Malloc( nLen + 1, _T("Meta_FontCreate function") );
						if ( !pFont->sFaceName )
							m_eError = wmf_error_NotEnoughMemory;
						else
						{
							pFont->sFaceName[nLen] = '\0';
							memcpy( pFont->sFaceName, sNewName, nLen );
						}
					}
					::SysFreeString( bsFontName );
				}

				::SysFreeString( bsPath );
			}
			else
			{
				::SysFreeString( bsFontName );
			}
#endif
		}

		pFont->pUserData = NULL;

		pPlayer->pDC->pFont = pFont;

		return 0;
	}
	int Meta_PaletteCreate(TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;

		unsigned short ushObjectIndex = 0;
		while ( pObjects[ushObjectIndex].nType && ( ushObjectIndex < m_pFile->pHeader->ushNumberOfObjects ) ) 
			ushObjectIndex++;

		if ( ushObjectIndex == m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		pObjects[ushObjectIndex].nType = OBJ_PAL;

		return 0;
	}
	int Meta_Delete       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;

		TWmfObject *pObjects = pPlayer->pObjects;
		unsigned short ushObectID = Record_GetUShortPar( pRecord, 0 );

		if ( ushObectID >= m_pFile->pHeader->ushNumberOfObjects )
		{	
			m_eError = wmf_error_BadFormat;
			return 0;
		}

		TWmfObject *pObject = pObjects + ushObectID;

		if ( OBJ_BRUSH == pObject->nType )
		{	
			if ( BS_DIBPATTERN == pObject->uObject.oBrush.ushStyle )
			{	
				if ( ( pPlayer->ulFlags & PLAYER_PLAY ) && m_pOutput ) 
					m_pOutput->Bmp_Free( &(pObject->uObject.oBrush.oBitmap ) );
			}
		}
		else if ( OBJ_REGION == pObject->nType )
		{	
			m_oMemoryManager.Free( pObject->uObject.oRegion.pRects );
		}

		pObject->nType = 0;

		return 0;
	}
	int Meta_Escape       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;

		unsigned short ushEscapeFunction = Record_GetUShortPar( pRecord, 0 );
		TWmfRecord oEscapeRecord = Record_OffsetRecord( pRecord, 1 );

		switch(ushEscapeFunction)
		{
		case 0x0019:/*DRAW_PATTERNRECT*/

			Meta_Escape_DrawRect( &oEscapeRecord );

			break;

		case 0x000F: /* */

			//m_eError = wmf_error_UnSupported;
			return 0;

		default:

			Meta_Escape_UnUsed( &oEscapeRecord );

			return 0;
		}

		return 0;
	}
	int Meta_UnUsed       (TWmfRecord *pRecord, TWmfAttributes *pAttrlist)
	{
		//http://msdn.microsoft.com/en-us/library/ms969909.aspx
		/* META_SETSTRETCHBLTMODE: not all that important really */

		/* META_SETMAPPERFLAGS:
		extract from http://www.melander.dk/lib/windows/gdi/fontmap.htm
		{
		Windows Font Mapping

		Ron Gery 
		Microsoft Developer Network Technology Group 

		Created: June 8, 1992 

		Filters

		An application can, to some extent, filter which physical fonts are examined by the font mapper. 
		Aspect-ratio filtering, which is available in both Windows versions 3.0 and 3.1, allows an 
		application to specify that only fonts designed for the device's aspect ratio should be considered 
		by the font mapper. An application enables and disables this filter by using the SetMapperFlags 
		function. Because nonscaling raster fonts are designed with a certain aspect ratio in mind, it is 
		sometimes desirable to ensure that only fonts actually designed for the device are used. When this 
		filter is enabled, the font mapper does not consider any physical fonts whose design aspect ratio 
		does not match that of the device. Aspect-ratio filtering does not affect TrueType fonts because 
		they can scale to match any aspect ratio. This filter affects all font selections to the DC until 
		the filter is turned off. Aspect-ratio filtering is a holdover from earlier times and is not a 
		recommended approach in today's font world. 
		}

		So we're going to ignore this call entirely */

		/* META_REALIZEPALETTE:
		META_SELECTPALETTE:
		META_SETPALENTRIES:
		as these set and fiddle with the palette i don't think
		they have much relevence to our converter, we will
		do our own color management elsewhere (if we do it
		at all), so i think we can safely ignore these things. */

		return 0;
	}
	int Meta_Escape_UnUsed     (TWmfRecord *pRecord)
	{
		return 0;
	}
	int Meta_Escape_DrawRect   (TWmfRecord *pRecord)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer *)m_pPlayerData;

		unsigned short ushByteCount = Record_GetUShortPar( pRecord, 0 );

		if ( 0x0014 != ushByteCount )
			return -1; // Неправильная запись 

		long lX = Record_GetLongPar( pRecord, 1, 2 );
		long lY = Record_GetLongPar( pRecord, 3, 4 );

		long lW = Record_GetLongPar( pRecord, 5, 6 );
		long lH = Record_GetLongPar( pRecord, 7, 8 );

		unsigned short ushStyle   = Record_GetUShortPar( pRecord, 9 );
		unsigned short ushPattern = Record_GetUShortPar( pRecord, 10 );

		return 0;
	}

//------------------------------------------------------------------------------------------------------------------------
	void SetViewportOrigin(TWmfCoordF oPointFt)
	{
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;
		pPlayer->oViewportOrigin = oPointFt;
	}
	void GetRectF(TWmfRectF *pRect, unsigned short ushX1, unsigned short ushY1, unsigned short ushX2, unsigned short ushY2)
	{	
		TWmfCoord oPoint1;
		TWmfCoord oPoint2;

		TWmfCoordF oPointF1;
		TWmfCoordF oPointF2;

		oPoint1.unX = UShort_2_Long( ushX1 );
		oPoint1.unY = UShort_2_Long( ushY1 );
		oPoint2.unX = UShort_2_Long( ushX2 );
		oPoint2.unY = UShort_2_Long( ushY2 );

		oPointF1 = TranslateCoordF( oPoint1 );
		oPointF2 = TranslateCoordF( oPoint2 );

		if ( oPointF1.fX <= oPointF2.fX )
		{	
			pRect->oTL.fX = oPointF1.fX;
			pRect->oBR.fX = oPointF2.fX;
		}
		else
		{	
			pRect->oTL.fX = oPointF2.fX;
			pRect->oBR.fX = oPointF1.fX;
		}

		if ( oPointF1.fY <= oPointF2.fY )
		{	
			pRect->oTL.fY = oPointF1.fY;
			pRect->oBR.fY = oPointF2.fY;
		}
		else
		{	
			pRect->oTL.fY = oPointF2.fY;
			pRect->oBR.fY = oPointF1.fY;
		}
	}
	void CoordFRegister(TWmfCoordF oPointF, float fScope)
	{
		TWmfPlayer* pPlayer = (TWmfPlayer*)m_pPlayerData;

		if ( 0 == ( pPlayer->ulFlags & PLAYER_TLBR_D_SET ) )
		{
			pPlayer->oDisplayTL = oPointF;
			pPlayer->oDisplayBR = oPointF;

			pPlayer->ulFlags |= PLAYER_TLBR_D_SET;
		}

		fScope = abs( fScope );

		if ( pPlayer->oDisplayTL.fX > ( oPointF.fX - fScope ) ) pPlayer->oDisplayTL.fX = oPointF.fX - fScope;
		if ( pPlayer->oDisplayTL.fY > ( oPointF.fY - fScope ) ) pPlayer->oDisplayTL.fY = oPointF.fY - fScope;
		if ( pPlayer->oDisplayBR.fX < ( oPointF.fX + fScope ) ) pPlayer->oDisplayBR.fX = oPointF.fX + fScope;
		if ( pPlayer->oDisplayBR.fY < ( oPointF.fY + fScope ) ) pPlayer->oDisplayBR.fY = oPointF.fY + fScope;


		double dPixW = GetPixelWidth();
		double dPixH = GetPixelHeight();

		int nX = pPlayer->pDC->oWindow.nX;
		int nY = pPlayer->pDC->oWindow.nY;
		int nW = pPlayer->pDC->oWindow.nWidth;
		int nH = pPlayer->pDC->oWindow.nHeight;

		if ( nH < 0 )
		{
			nY = nY + nH;
			nH = -nH;
		}
		if ( nW < 0 )
		{
			nX = nX + nW;
			nW = -nW;
		}


		float fX = (float) ((double) nX * dPixW );
		float fY = (float) ((double) nY * dPixH );

		if ( pPlayer->oBoundTL.fX > fX ) pPlayer->oBoundTL.fX = fX;
		if ( pPlayer->oBoundTL.fY > fY ) pPlayer->oBoundTL.fY = fY;
		if ( pPlayer->oBoundBR.fX < fX ) pPlayer->oBoundBR.fX = fX;
		if ( pPlayer->oBoundBR.fY < fY ) pPlayer->oBoundBR.fY = fY;

		fX = (float) ((double) (nX + nW) * dPixW );
		fY = (float) ((double) (nY + nH) * dPixH );

		if ( pPlayer->oBoundTL.fX > fX ) pPlayer->oBoundTL.fX = fX;
		if ( pPlayer->oBoundTL.fY > fY ) pPlayer->oBoundTL.fY = fY;
		if ( pPlayer->oBoundBR.fX < fX ) pPlayer->oBoundBR.fX = fX;
		if ( pPlayer->oBoundBR.fY < fY ) pPlayer->oBoundBR.fY = fY;

		if ( pPlayer->oBoundTL.fX > fX ) pPlayer->oBoundTL.fX = fX;
		if ( pPlayer->oBoundTL.fY > fY ) pPlayer->oBoundTL.fY = fY;
		if ( pPlayer->oBoundBR.fX < fX ) pPlayer->oBoundBR.fX = fX;
		if ( pPlayer->oBoundBR.fY < fY ) pPlayer->oBoundBR.fY = fY;

		//if ( m_pFile->nPlaceable )
		//{
		//	pPlayer->oBoundTL.fX = m_pFile->pPlaceableMetaHeader->shLeft   * GetPixelWidth();
		//	pPlayer->oBoundTL.fY = m_pFile->pPlaceableMetaHeader->shTop    * GetPixelHeight();
		//	pPlayer->oBoundBR.fX = m_pFile->pPlaceableMetaHeader->shRight  * GetPixelWidth();
		//	pPlayer->oBoundBR.fY = m_pFile->pPlaceableMetaHeader->shBottom * GetPixelHeight();
		//}

	}

	TWmfCoord  TranslateCoord (TWmfCoordF oPointF)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfCoord oPoint;

		oPointF.fX += pPlayer->oViewportOrigin.fX;
		oPointF.fY += pPlayer->oViewportOrigin.fY;

		oPoint.unX = (unsigned int) ((double) oPointF.fX / GetPixelWidth());
		oPoint.unY = (unsigned int) ((double) oPointF.fY / GetPixelHeight());

		oPoint.unX += pPlayer->pDC->oWindow.nX;
		oPoint.unY += pPlayer->pDC->oWindow.nY;

		return oPoint;
	}

	TWmfCoordF TranslateCoordF(TWmfCoord  oPoint)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfCoordF oPointF;

		oPointF.fX = (float) ( ((int)oPoint.unX) * GetPixelWidth() );
		oPointF.fY = (float) ( ((int)oPoint.unY) * GetPixelHeight() );

		oPointF.fX -= pPlayer->oViewportOrigin.fX;
		oPointF.fY -= pPlayer->oViewportOrigin.fY;

		return oPointF;
	}
	void       SetMapMode     (unsigned short ushMapMode)
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		pPlayer->pDC->ushMapMode = ushMapMode;

		switch (ushMapMode)
		{
		case (MM_TEXT): // 1 unit = 1pt
			pPlayer->pDC->dPixelWidth  = 1;
			pPlayer->pDC->dPixelHeight = 1;
			break;
		case (MM_LOMETRIC): // 1 unit = 0.1mm
			pPlayer->pDC->dPixelWidth  = INCH_TO_POINT (MM_TO_INCH (0.1));
			pPlayer->pDC->dPixelHeight = INCH_TO_POINT (MM_TO_INCH (0.1));
			break;
		case (MM_HIMETRIC): // 1 unit = 0.01mm
			pPlayer->pDC->dPixelWidth  = INCH_TO_POINT (MM_TO_INCH (0.01));
			pPlayer->pDC->dPixelHeight = INCH_TO_POINT (MM_TO_INCH (0.01));
			break;
		case (MM_LOENGLISH): // 1 unit = 0.01 inch
			pPlayer->pDC->dPixelWidth  = INCH_TO_POINT (0.01);
			pPlayer->pDC->dPixelHeight = INCH_TO_POINT (0.01);
			break;
		case (MM_HIENGLISH): // 1 unit = 0.001 inch
			pPlayer->pDC->dPixelWidth  = INCH_TO_POINT (0.001);
			pPlayer->pDC->dPixelHeight = INCH_TO_POINT (0.001);
			break;
		case (MM_TWIPS): // 1 unit = 1/1440 inch
			pPlayer->pDC->dPixelWidth  = 0.05;
			pPlayer->pDC->dPixelHeight = 0.05;
			break;
		case (MM_ISOTROPIC):
		case (MM_ANISOTROPIC): // scale here depends on window & viewport extents
			GetPixelWidth();
			GetPixelHeight();
			break;
		default:
			if ( m_pFile->nPlaceable )
			{
				pPlayer->pDC->dPixelWidth  = INCH_TO_POINT ((double) 1 / (double) m_pFile->pPlaceableMetaHeader->ushInch);
				pPlayer->pDC->dPixelHeight = INCH_TO_POINT ((double) 1 / (double) m_pFile->pPlaceableMetaHeader->ushInch);

				ushMapMode = MM_DPI; /* [fjf] added this - uncertainly */
			}
			else
			{	
				m_eError = wmf_error_BadFormat;
			}
			break;
		}

		//pPlayer->pDC->ushMapMode = ushMapMode;
	}

	double     GetPixelWidth  ()
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		switch ( pPlayer->pDC->ushMapMode )
		{
		case (MM_ISOTROPIC): // scale here depends on window & viewport extents
		//case (MM_ANISOTROPIC):
			if ( pPlayer->pDC->oWindow.nWidth == 0 )
			{	
				m_eError = wmf_error_BadFormat;
				break;
			}
			if ( pPlayer->unViewportWidth == 0 )
			{
				m_eError = wmf_error_BadFormat;
				break;
			}
			pPlayer->pDC->dPixelWidth  = (double) pPlayer->unViewportWidth  / (double) pPlayer->pDC->oWindow.nWidth;

			break;

		case (MM_ANISOTROPIC):
			pPlayer->pDC->dPixelWidth  = (double) pPlayer->unViewportWidth;
			break;

		case (MM_TEXT):      // 1 unit = 1pt
		case (MM_LOMETRIC):  // 1 unit = 0.1mm
		case (MM_HIMETRIC):  // 1 unit = 0.01mm
		case (MM_LOENGLISH): // 1 unit = 0.01 inch
		case (MM_HIENGLISH): // 1 unit = 0.001 inch
		case (MM_TWIPS):     // 1 unit = 1/1440 inch
		case (MM_DPI):       // isotropic; placeable meta file

			break;

		default:

			break;
		}

		if ( CheckError() )
			return 1;

		return pPlayer->pDC->dPixelWidth;
	}
	double     GetPixelHeight ()
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		switch ( pPlayer->pDC->ushMapMode )
		{
		case (MM_ISOTROPIC): // scale here depends on window & viewport extents
		//case (MM_ANISOTROPIC):
			if ( pPlayer->pDC->oWindow.nHeight == 0 )
			{	
				m_eError = wmf_error_BadFormat;
				break;
			}
			if ( pPlayer->unViewportHeight == 0 )
			{	
				m_eError = wmf_error_BadFormat;
				break;
			}
			pPlayer->pDC->dPixelHeight  = (double) pPlayer->unViewportHeight / (double) pPlayer->pDC->oWindow.nHeight;

			break;

		case (MM_ANISOTROPIC):
			pPlayer->pDC->dPixelHeight  = (double) pPlayer->unViewportHeight;
			break;

		case (MM_TEXT):      // 1 unit = 1pt
		case (MM_LOMETRIC):  // 1 unit = 0.1mm
		case (MM_HIMETRIC):  // 1 unit = 0.01mm
		case (MM_LOENGLISH): // 1 unit = 0.01 inch
		case (MM_HIENGLISH): // 1 unit = 0.001 inch
		case (MM_TWIPS):     // 1 unit = 1/1440 inch
		case (MM_DPI):       // isotropic; placeable meta file

			break;

		default:

			break;
		}

		if ( CheckError() )
			return 1;

		return pPlayer->pDC->dPixelHeight;
	}

//------------------------------------------------------------------------------------------------------------------------
	unsigned short ReadShort()
	{	
		unsigned short ushResult = 0;

		int nByte1 = Read();
		int nByte2 = Read();

		if ( ( EOF == nByte1 ) || ( EOF == nByte2 ) )
		{	
			m_eError = wmf_error_EOF;
		}
		else 
			ushResult = (((unsigned short) nByte2) << 8) + ((unsigned short) nByte1);

		return ushResult;
	}

	unsigned int   ReadLong(unsigned short *pushByte1, unsigned short *pushByte2)
	{
		unsigned short ushShort1 = ReadShort();
		unsigned short ushShort2 = ReadShort();

		if ( pushByte1 ) 
			(*pushByte1) = ushShort1;
		if ( pushByte2 ) 
			(*pushByte2) = ushShort2;

		unsigned int unResult = ((((unsigned int) (ushShort2)) << 16) + (unsigned int) (ushShort1));

		return unResult;
	}



	unsigned short AldusChecksum ()
	{
		unsigned short ushChecksum = 0;

		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->unKey & 0x0000FFFFUL;
		ushChecksum ^= (m_pFile->pPlaceableMetaHeader->unKey & 0xFFFF0000UL) >> 16;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->ushHandle;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->shLeft;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->shTop;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->shRight;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->shBottom;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->ushInch;
		ushChecksum ^=  m_pFile->pPlaceableMetaHeader->unReserved & 0x0000FFFFUL;
		ushChecksum ^= (m_pFile->pPlaceableMetaHeader->unReserved & 0xFFFF0000UL) >> 16;

		return (ushChecksum);
	} 

	void           ReadHeader()
	{
		unsigned short uShort1;
		unsigned short uShort2;

		long lHeaderStart = Tell();
		long lHeaderEnd   = 0;

		if ( 0x9ac6cdd7 == ( m_pFile->pPlaceableMetaHeader->unKey = ReadLong( &uShort1, &uShort2 ) ) )
		{	
			m_pFile->nPlaceable = 1;

			m_pFile->pPlaceableMetaHeader->ushHandle = ReadShort();

			uShort1 = ReadShort(); m_pFile->pPlaceableMetaHeader->shLeft   = UShort_2_Short(uShort1);
			uShort1 = ReadShort(); m_pFile->pPlaceableMetaHeader->shTop    = UShort_2_Short(uShort1);
			uShort1 = ReadShort(); m_pFile->pPlaceableMetaHeader->shRight  = UShort_2_Short(uShort1);
			uShort1 = ReadShort(); m_pFile->pPlaceableMetaHeader->shBottom = UShort_2_Short(uShort1);

			m_pFile->pPlaceableMetaHeader->ushInch     = ReadShort();
			m_pFile->pPlaceableMetaHeader->unReserved  = ReadLong(NULL, NULL);
			m_pFile->pPlaceableMetaHeader->ushChecksum = ReadShort();

			m_pFile->pHeader->ushFileType   = ReadShort();
			m_pFile->pHeader->ushHeaderSize = ReadShort();
		}
		else
		{
			m_pFile->nPlaceable = 0;

			m_pFile->pPlaceableMetaHeader->unKey = 0;

			m_pFile->pPlaceableMetaHeader->ushHandle   = 0;
			m_pFile->pPlaceableMetaHeader->shLeft      = 0;
			m_pFile->pPlaceableMetaHeader->shTop       = 0;
			m_pFile->pPlaceableMetaHeader->shRight     = 0;
			m_pFile->pPlaceableMetaHeader->shBottom    = 0;
			m_pFile->pPlaceableMetaHeader->ushInch     = 0;
			m_pFile->pPlaceableMetaHeader->unReserved  = 0;
			m_pFile->pPlaceableMetaHeader->ushChecksum = 0;

			m_pFile->pHeader->ushFileType   = uShort1;
			m_pFile->pHeader->ushHeaderSize = uShort2;
		}

		if ( 9 == m_pFile->pHeader->ushHeaderSize )
		{	
			m_pFile->pHeader->ushVersion         = ReadShort();
			m_pFile->pHeader->unFileSize         = ReadLong(NULL, NULL);
			m_pFile->pHeader->ushNumberOfObjects = ReadShort();
            m_pFile->pHeader->unMaxRecordSize    = ReadLong(NULL, NULL);
			m_pFile->pHeader->ushNumberOfMembers = ReadShort();

			m_pFile->lPos = Tell();
			lHeaderEnd = m_pFile->lPos;
			WriteHeader( lHeaderStart, lHeaderEnd );
		}
		else
		{			
			m_eError = wmf_error_BadFormat;
		}
	}

	void           PlayMetaFile()
	{	
		TWmfPlayer *pPlayer = (TWmfPlayer*)m_pPlayerData;

		TWmfAttributes  oAttrlist;
		TWmfAttributes *pAtts = NULL;

		pPlayer->nDCStackLength = 0;

		TWmfRegion *pVisible = &(pPlayer->oVisible);
		pVisible->unNumRects = 0;
		pVisible->pRects     = NULL;
		pVisible->unSize     = 0;

		pPlayer->oCurrent.unX = 0;
		pPlayer->oCurrent.unY = 0;

		pPlayer->oViewportOrigin.fX = 0;
		pPlayer->oViewportOrigin.fY = 0;

		pPlayer->unViewportWidth  = 1024;
		pPlayer->unViewportHeight = 1024;

		pPlayer->pDC = CopyDC(NULL);

		SetMapMode( (unsigned short)(m_pFile->nPlaceable ? MM_DPI : MM_TEXT));

		if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) && m_pOutput )
		{	
			m_pOutput->Begin();
		}

		unsigned char *pParameters = pPlayer->pParameters;

		TWmfObject *pObjects = pPlayer->pObjects;

		for ( unsigned short ushIndex = 0; ushIndex < m_pFile->pHeader->ushNumberOfObjects; ushIndex++ ) 
			pObjects[ushIndex].nType = 0;

		Seek( m_pFile->lPos );

		//AttrNew( &oAttrlist );
		if ( CheckError() )
			return;

		unsigned long ulSize;
		unsigned int  unFunction;

		unsigned long ulNumber = 0;

		do
		{	
			if ( ++ulNumber < m_oStore.ulCount )
			{	
				pAtts = m_oStore.pAttrlist + ulNumber;
			}
			else
			{	
				pAtts = &oAttrlist;
				//AttrClear( pAtts );
			}

			ulSize     = ReadLong( NULL, NULL );
			unFunction = ReadShort();

			if ( (3 == ulSize ) && ( /*META_EOF*/0 == unFunction ) ) // Проверяем на последнюю запись в метафайле
			{	
				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) ) 
					Write( ulSize, unFunction, "empty", pAtts->psAttrs, 0, 0 );
				break;
			}

            //if ( ( ulSize > m_pFile->pHeader->unMaxRecordSize ) || ( ulSize < 3 ) )
            if ( ( (ulSize - 3) > m_pFile->pHeader->unMaxRecordSize ) || ( ulSize < 3 ) )
			{	
				m_eError = wmf_error_BadFormat;
				break;
			}

			long lPosParams = Tell();

			if ( lPosParams < 0 )
			{
				m_eError = wmf_error_BadFile;
				break;
			}

			for ( int nIndex = 0; nIndex < (int)( ( ulSize - 3 ) * 2 ); nIndex++ )
			{	
				int nByte = Read();
				if ( EOF == nByte )
				{	
					m_eError = wmf_error_EOF;
					break;
				}
				pParameters[nIndex] = (unsigned char)nByte;
			}

			TWmfRecord oRecord;
			oRecord.ulSize     = ulSize - 3;
			oRecord.unFunction = unFunction;
			oRecord.sParameter = pParameters;
			oRecord.lPosition  = lPosParams;

			switch (unFunction)
			{
			case META_SETMAPMODE: // 2.3.5.17
				
				Meta_MapMode( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) ) 
					Write( ulSize, unFunction, "setmapmode", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2 );

				break;

			case META_SETWINDOWORG: // 2.3.5.31

				Meta_OrgExt( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setwindoworg", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2 );

				break;

			case META_SETVIEWPORTORG:  // 2.3.5.29
				
				Meta_OrgExt( &oRecord, pAtts );
				
				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setviewportorg", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);
				break;

			case META_SETVIEWPORTEXT: // 2.3.5.28

				Meta_OrgExt( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setviewportext", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);
				break;

			case META_SETWINDOWEXT: // 2.3.5.30

				Meta_OrgExt( &oRecord, pAtts );
				
				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setwindowext", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_OFFSETWINDOWORG: // 2.3.5.7

				Meta_OrgExt( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "offsetwindoworg", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_OFFSETVIEWPORTORG: // 2.3.5.6

				Meta_OrgExt( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "offsetviewportorg", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SCALEWINDOWEXT: // 2.3.5.13

				Meta_Scale( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "scalewindowext", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SCALEVIEWPORTEXT: // 2.3.5.12

				Meta_Scale( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "scaleviewportext", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//	
			//	(a) basic draw
			//	(b) REGION calls
			//	(c) BMP & ROP stuff
			//	(d) DC set
			//	(e) text & font
			//	(f) palette
			//	(g) create & delete; save & restore; ...
			//	(h) ==== other ====
			//	

			//-----------------------------------------------------------------------------
			// (a) basic draw
			//-----------------------------------------------------------------------------
			case META_MOVETO: // 2.3.5.4

				Meta_MoveTo( &oRecord, pAtts );
				
				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "moveto", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_FLOODFILL: // 2.3.3.7

				Meta_Flood( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "floodfill", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_EXTFLOODFILL: // 2.3.3.4

				Meta_Flood( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "extfloodfill", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETPIXEL: // 2.3.3.19

				Meta_Pixel( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setpixel", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_PIE: // 2.3.3.13

				Meta_Arc( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "pie", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_CHORD: // 2.3.3.2

				Meta_Arc( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "chord", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_ARC: // 2.3.3.1
				
				Meta_Arc( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "arc", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_ELLIPSE: // 2.3.3.3
				
				Meta_Ellipse( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "ellipse", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_LINETO: // 2.3.3.10

				Meta_Line( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "lineto", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_POLYLINE: // 2.3.3.14

				Meta_Lines( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "polyline", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_POLYGON: // 2.3.3.15
				
				Meta_Polygon( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "polygon", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_POLYPOLYGON: // 2.3.3.16

				Meta_Polygons( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "polypolygon", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_ROUNDRECT: // 2.3.3.18

				Meta_Round( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "roundrect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_RECTANGLE: // 2.3.3.17

				Meta_Rect( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "rectangle", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (b) REGION calls
			//-----------------------------------------------------------------------------
			case META_FRAMEREGION: // 2.3.3.8

				Meta_RegionBrush( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "frameregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_FILLREGION: // 2.3.3.6

				Meta_RegionBrush( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "fillregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_INVERTREGION: // 2.3.3.9
				
				Meta_RegionPaint( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "invertregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_PAINTREGION: // 2.3.3.11

				Meta_RegionPaint( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "paintregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_CREATEREGION: // 2.3.4.6

				Meta_RegionCreate( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "createregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SELECTCLIPREGION: // 2.3.4.9

				Meta_ClipSelect( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "selectclipregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_OFFSETCLIPRGN: // 2.3.5.5

				Meta_ClipOffset( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "offsetclipregion", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_EXCLUDECLIPRECT: // 2.3.5.2

				Meta_ClipCombine( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "excludecliprect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_INTERSECTCLIPRECT: // 2.3.5.3

				Meta_ClipCombine( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "intersectcliprect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (c) BMP & ROP stuff
			// Ширина и высота картинок может быть отрицательной
			//-----------------------------------------------------------------------------
			case META_SETDIBTODEV: // 2.3.1.4

				Meta_DIBDraw( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setdibtodev", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_STRETCHDIB: // 2.3.1.6

				Meta_DIBDraw( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "stretchdib", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_DIBSTRETCHBLT: // 2.3.1.3

				Meta_DIBDraw( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "dibstretchblt", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_DIBBITBLT: // 2.3.1.2

				Meta_DIBDraw( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "dibbitblt", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_DIBCREATEPATTERNBRUSH: // 2.3.4.8

				Meta_DIBBrush( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "dibcreatepatternbrush", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_PATBLT: // 2.3.3.12

				Meta_ROPDraw( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "patblt", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (d) DC set
			//-----------------------------------------------------------------------------
			case META_SETROP2: // 2.3.5.22
				
				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setrop2", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETTEXTJUSTIFICATION: // 2.3.5.27

				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "settextjustification", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETTEXTCHAREXTRA: // 2.3.5.25

				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "settextcharextra", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETPOLYFILLMODE: // 2.3.5.20

				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setpolyfillmode", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETSTRETCHBLTMODE: // 2.3.5.23

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setstretchbltmode", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETTEXTALIGN: // 2.3.5.24

				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "settextalign", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETTEXTCOLOR: // 2.3.5.26

				Meta_DCColor( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "settextcolour", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETBKCOLOR: // 2.3.5.14

				Meta_DCColor( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setbkcolor", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETBKMODE: // 2.3.5.15

				Meta_DCSet( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setbkmode", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SELECTOBJECT: // 2.3.4.10

				Meta_DCSelect( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "selectobject", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (e) text & font
			//-----------------------------------------------------------------------------
			case META_TEXTOUT: // 2.3.3.20

				Meta_Text( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "textout", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_EXTTEXTOUT: // 2.3.3.5

				Meta_Text( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "exttextout", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_CREATEFONTINDIRECT: // 2.3.4.2

				Meta_FontCreate( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "createfontindirect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETMAPPERFLAGS: // 2.3.5.18

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setmapperflags", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (f) palette
			//-----------------------------------------------------------------------------
			case META_CREATEPALETTE: // 2.3.4.3

				Meta_PaletteCreate( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "createpalette", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_REALIZEPALETTE: // 2.3.5.8

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "realizepalette", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SELECTPALETTE: // 2.3.4.11

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "selectpalette", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_SETPALENTRIES: // 2.3.5.19

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "setpalentries", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (g) create & delete; save & restore; ...
			//-----------------------------------------------------------------------------
			case META_SAVEDC: // 2.3.5.11

				Meta_DCSave( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "savedc", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_RESTOREDC: // 2.3.5.10

				Meta_DCRestore( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "restoredc", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_CREATEPENINDIRECT: // 2.3.4.5

				Meta_PenCreate( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "createpenindirect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_CREATEBRUSHINDIRECT: // 2.3.4.1

				Meta_BrushCreate( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "createbrushindirect", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			case META_DELETEOBJECT: // 2.3.4.7

				Meta_Delete( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "deleteobject", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			//-----------------------------------------------------------------------------
			// (h) ==== other ====
			//-----------------------------------------------------------------------------
			case META_ESCAPE: // 2.3.6

				Meta_Escape( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "escape", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;

			default:

				Meta_UnUsed( &oRecord, pAtts );

				if ( 0 == (pPlayer->ulFlags & PLAYER_PLAY) )  
					Write( ulSize, unFunction, "unknown", pAtts->psAttrs, oRecord.sParameter, oRecord.ulSize * 2);

				break;
			}

		} while ( wmf_error_None == m_eError );

		if ( 0 == ulSize )
		{	
			if ( m_eError == wmf_error_EOF )
			{	
				m_eError = wmf_error_None;
			}
		}

		if ( CheckError() )
			return;

		if ( ( pPlayer->ulFlags & PLAYER_PLAY ) && m_pOutput ) 
			m_pOutput->End();

		//AttrFree( &oAttrlist );

		if ( 0 == ( pPlayer->ulFlags & PLAYER_PLAY ) ) 
			WriteEnd();

		while ( pPlayer->nDCStackLength )
		{	
			if ( pPlayer->pDC )
			{
				if ( pPlayer->pDC->pClip )
				{
					if ( pPlayer->pDC->pClip->pRects )
						m_oMemoryManager.Free( pPlayer->pDC->pClip->pRects );
					m_oMemoryManager.Free( pPlayer->pDC->pClip );
				}
				m_oMemoryManager.Free( pPlayer->pDC );
			}
			
			pPlayer->pDC = DCStackPop();
		}

		if ( pPlayer->pDC )
		{	
			if ( pPlayer->pDC->pClip )
			{
				if ( pPlayer->pDC->pClip->pRects )
					m_oMemoryManager.Free( pPlayer->pDC->pClip->pRects );
				m_oMemoryManager.Free( pPlayer->pDC->pClip );
			}
			m_oMemoryManager.Free( pPlayer->pDC );
		}

		DCStackFree();

		for ( unsigned short ushIndex = 0; ushIndex < m_pFile->pHeader->ushNumberOfObjects; ushIndex++ )
		{	
			if ( OBJ_BRUSH == pObjects[ushIndex].nType )
			{	
				if ( BS_DIBPATTERN == pObjects[ushIndex].uObject.oBrush.ushStyle )
				{	
					if ( pObjects[ushIndex].uObject.oBrush.oBitmap.pData && m_pOutput )
					{	
						m_pOutput->Bmp_Free( &(pObjects[ushIndex].uObject.oBrush.oBitmap));
					}
				}
			}
			else if ( OBJ_REGION == pObjects[ushIndex].nType )
			{	
				m_oMemoryManager.Free( pObjects[ushIndex].uObject.oRegion.pRects );
			}
			else if ( OBJ_FONT == pObjects[ushIndex].nType )
			{	
				m_oMemoryManager.Free( pObjects[ushIndex].uObject.oFont.sFaceName );
			}
		}
	}

//------------------------------------------------------------------------------------------------------------------------
public:
private:

	void                   *m_pBufferData;          // Буфер
	int                     m_nBufferType;          // Что храним в буфере: 0 - указатель на TFileInfo, 1 - указатель на TMemoryInfo, -1 - файл не открыт
	EWmfError               m_eError;               // Ошибка

	TWmfHead                m_pHead;                //
	TWmfPlaceableMetaHeader m_pPlaceableMetaHeader; // Заголовки метафайла
	TWmfMetaHeader          m_pMetaHeader;          // 
	TWmfFile                m_pFile;                //

	void                   *m_pWriteData;           // Запись Xml в дебаге
	TWmfAttributeStore      m_oStore;               // Хранение Xml

	TWmfPlayer             *m_pPlayerData;          // Проигрыватель Wmf файла
#ifdef DESKTOP_EDITOR_GRAPHICS
	CFontManager        *m_pFontManager;         // Интерефейс для работы с шрифтами
	bool m_bDeleteFontManager;
#else
	IASCFontManager        *m_pFontManager;         // Интерефейс для работы с шрифтами
#endif
	CWmfColor               m_oColorData;           // Паллитра
	CWmfOutputDevice       *m_pOutput;              // Выходное устройство, для изображения данного метафайла

	bool                    m_bWindow;              // Были ли заданы размеры картинки командой SETWINDOWEXT

	CWmfMemoryManager       m_oMemoryManager;
	friend class CRendererOutput;
};

#endif /* _WMF_FILE_H */
