#ifndef _DOC_H
#define _DOC_H

#pragma once

#define SIG_BYTES 0x41504446L

#include "Catalog.h"
#include "Image.h"
#include "Pages.h"
#include "Outline.h"
#include "GraphicState.h"
#include "Utils.h"
#include "EncryptDictionary.h"
#include "Destination.h"
#include "MetaData.h"
#include "LZWEncoder.h"
#include "Pattern.h"

static const char *VERSION_STR[5] = 
{
	"%PDF-1.2\012%ASC\012",
	"%PDF-1.3\012%ASC\012",
	"%PDF-1.4\012%ASC\012",
	"%PDF-1.5\012%ASC\012",
	"%PDF-1.6\012%ASC\012"
};


#define VER_DEFAULT  PDF_1_2

typedef struct TDocRec 
{
    unsigned int    nSigBytes;
    PDFVer          ePdfVersion;

    MMgr            oMMgr;
    Catalog         pCatalog;
    Outline         pOutlines;
    Xref            pXref;
    Pages           pRootPages;
    Pages           pCurPages;
    Page            pCurPage;
    List            pPageList;
    ErrorRec        oError;
    Dict            pInfo;
    Dict            pTrailer;

	Dict			resources;

    List            pFontManager;
    BYTE            aTTFontTag[6];

    // список для загрузки fontdefs
    List            pFontDefList;

    // список для загрузки кодировок
    List            pEncoderList;
    EncoderRecPtr         pCurEncoder;

	// список для загрузки ToUnicode
	List            pToUnicodeList;         // Весь список отображений ToUnicode
	unsigned int    nToUnicodeStreamCount;  // Количество ToUnicode типа StreamRecPtr
	unsigned int    nToUnicodeBuiltCount;   // Количество ToUnicode типа Built
	PToUnicode      pCurToUnicode;          // Текущий объект ToUnicode
	PToUnicode      pLastToUnicodeBuilt;    // Последний объект ToUnicode типа Built

	// Список ExtGState
	List            pExtGStateList;
	ExtGState       pCurExtGState;

    // default compression mode 
    unsigned short  nCompressionMode;

    BOOL            bEncryptOn;
    EncryptDict     pEncryptDict;

    EncoderRecPtr         pDefEncoder;

    unsigned int    nPagePerPages;
    unsigned int    nCurPageNum;
	
	// Счетчик CMap кодировок
	unsigned int    nCMapCount;
	EncoderRecPtr         pLastCMapEncoder;

	// Темповая папка, куда скидываются все временный файлы
	wchar_t         wsTempDir[MAX_PATH];

    // буфер для сохранения в потоке
    StreamRecPtr          pStream;

} DocRec;

typedef struct TDocRec *Doc;

#include "PageLabel.h"

//----------------- определения вспомогательных функций ------------------------------------------

unsigned long WriteHeader            (Doc pPDF, StreamRecPtr pStream);
unsigned long PrepareTrailer         (Doc pPDF);
unsigned long InternalSaveToStream   (Doc pPDF, StreamRecPtr pStream);
unsigned int  DocGetTempFile         (Doc pPDF, CString *pwsTempFileName);
unsigned long InitCMapEncoding       (EncoderRecPtr pEncoder);
//----------------- определения основных функций --------------------------------------------------
//----------------- Функции для работы с ToUnicode -------------------------------------------------
PToUnicode      DocFindToUnicode      (Doc pPDF, const char *sToUnicodeName);
PToUnicode      DocFindToUnicode      (Doc pPDF, unsigned short *pUnicode, unsigned int unSize);
PToUnicode      DocFindToUnicode      (Doc pPDF, BYTE *pBuffer, unsigned int unSize);
PToUnicode      GetToUnicode          (Doc pPDF, const char *sToUnicodeName);
PToUnicode      GetToUnicode          (Doc pPDF, unsigned short *pUnicode, unsigned int unSize);
PToUnicode      GetToUnicode          (Doc pPDF, BYTE *pBuffer, unsigned int unSize);
PToUnicode      DocGetCurrentToUnicode(Doc pPDF);
void            FreeToUnicodeList     (Doc pPDF);
PToUnicode      FindToUnicodeForString(Doc pPDF, std::wstring sSrc);
//----------------- Функции для работы с кодировками -----------------------------------------------
EncoderRecPtr   DocFindEncoder          (Doc pPDF, const char *sEncodingName, unsigned short *pUnicode = NULL, int nCount = 0);
unsigned long   DocRegisterEncoder      (Doc pPDF, EncoderRecPtr pEncoder);
EncoderRecPtr   GetEncoder              (Doc pPDF, const char *sEncodingName, unsigned short *pUnicode = NULL, int nCount = 0);
EncoderRecPtr   DocGetCurrentEncoder    (Doc pPDF);
unsigned long   DocSetCurrentEncoder    (Doc pPDF, const char *sEncodingName);
void            FreeEncoderList         (Doc pPDF);
EncoderRecPtr   FindEncoderForString    (Doc pPDF, CString sSrc, BOOL *pbNew);
//----------------- Функции для работы с фонтами ---------------------------------------------------
void            FreeFontDefList         (Doc pPDF);
void            CleanupFontDefList      (Doc pPDF);
FontDef         DocFindFontDef          (Doc pPDF, const char *sFontName, const char *sToUnicodeName = NULL);
unsigned long   DocRegisterFontDef      (Doc pPDF, FontDef pFontDef);
FontDef         GetFontDef              (Doc pPDF, const char *sFontName, const char *sToUnicode = NULL);
//----------------- Font ----------------------------------------------------------------------------
FontDict        DocFindFont             (Doc pPDF, const char *sFontName, const char *sEncodingName, const char *sToUnicodeName = NULL);
FontDict        GetFont                 (Doc pPDF, const char *sFontName, const char *sEncodingName, const char *sToUnicodeName = NULL, unsigned short *pUnicode = NULL, int nCount = 0);
const char*     LoadType1FontFromFile   (Doc pPDF, std::wstring & wsAFMFileName, std::wstring & wsDataFileName, short nType = 0);
const char*     LoadType1FontFromStream (Doc pPDF, StreamRecPtr pAFMData, StreamRecPtr pPFMData, short nType);
const char*     LoadTTFontFromFile      (Doc pPDF, std::wstring & wsFileName, BOOL bEmbedding, const char* sEncodingName = NULL, const char *sPrefix = NULL);
const char*     LoadTTFontFromStream    (Doc pPDF, BOOL bEmbedding, std::wstring & wsFileName, const char *sEncodingName, const char *sPrefix);
const char*     LoadTTFontFromFile2     (Doc pPDF, std::wstring & wsFileName, unsigned int nIndex, BOOL bEmbedding);
const char*     LoadTTFontFromStream2   (Doc pPDF, unsigned int nIndex, BOOL bEmbedding, std::wstring & wsFileName);
////----------------- Images --------------------------------------------------------------------------
//ImageDict LoadRawImageFromFile (Doc pPDF, const wchar_t *wsFileName, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace)
//{
//    ImageDict pImage = NULL;
//
//    if ( !HasDoc( pPDF ) )
//        return NULL;
//
//    // создаем файловый поток
//	StreamRecPtr pImageData = FileReaderNew( pPDF->oMMgr, wsFileName );
//
//    if ( StreamValidate( pImageData ) )
//        pImage = ImageLoadRawImage( pPDF->oMMgr, pImageData, pPDF->pXref, nWidth, nHeight, eColorSpace);
//    else
//        pImage = NULL;
//
//    // удаялем поток
//    StreamFree( pImageData );
//
//    if ( !pImage )
//        CheckError( &pPDF->oError );
//
//#ifdef FILTER_FLATE_DECODE_ENABLED
//    if ( pPDF->nCompressionMode & COMP_IMAGE )
//        pImage->nFilter = STREAM_FILTER_FLATE_DECODE;
//#endif
//
//    return pImage;
//}
//
//ImageDict LoadRawImageFromMem  (Doc pPDF, const BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace, unsigned int nBitsPerComponent, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL)
//{
//    if ( !HasDoc( pPDF ))
//        return NULL;
//
//    ImageDict pImage = ImageLoadRawImageFromMem( pPDF->oMMgr, pBuffer, pPDF->pXref, nWidth, nHeight, eColorSpace, nBitsPerComponent, bAlpha, pAlphaBuffer );
//
//    if ( !pImage )
//        CheckError( &pPDF->oError );
//
//#ifdef FILTER_FLATE_DECODE_ENABLED
//    if ( pPDF->nCompressionMode & COMP_IMAGE )
//        pImage->nFilter = STREAM_FILTER_FLATE_DECODE;
//#endif
//
//    return pImage;
//}
ImageDict LoadJpxImageFromFile (Doc pPDF, std::wstring wsFileName, long nOpacityFlag);
ImageDict LoadJbig2ImageFromMem(Doc pPDF, BYTE *pBuffer, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum);

ImageDict LoadJpegImageFromFile(Doc pPDF, std::wstring wsFileName);
ImageDict LoadJpegImageFromMem (Doc pPDF, BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL, unsigned int unAlphaCheckSum = 0);

//ImageDict LoadJpxImageFromMem  (Doc pPDF, const BYTE *pBuffer,   unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL, unsigned int unAlphaCheckSum = 0)
//{
//#ifdef BUILD_CONFIG_OPENSOURCE_VERSION
//	return NULL;
//#else
//    if ( !HasDoc( pPDF ))
//        return NULL;
//
//	// Переводим Image -> IAVSUncompressedVideoFrame
//	MediaCore::IAVSUncompressedVideoFrame *pInterface = NULL;
//	HRESULT hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
//	if ( hRes != S_OK )
//		return NULL;
//
//	int nBufferSize = 4 * nWidth * nHeight;
//	if ( nBufferSize < 1 )
//		return NULL;
//
//	pInterface->put_ColorSpace( 1 << 6 ); // CPS_BGRA
//	pInterface->put_Width( nWidth );
//	pInterface->put_Height( nHeight );
//	pInterface->put_AspectRatioX( nWidth );
//	pInterface->put_AspectRatioY( nHeight );
//	pInterface->put_Interlaced( VARIANT_FALSE );
//	pInterface->put_Stride( 0, 4 * nWidth );
//	pInterface->AllocateBuffer( nBufferSize );
//
//	unsigned char *pBufferPtr = NULL;
//	long nCreatedBufferSize = 0;
//
//	pInterface->get_Buffer( &pBufferPtr );
//	pInterface->get_BufferSize( &nCreatedBufferSize );
//	pInterface->put_Plane( 0, pBufferPtr );
//
//	if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
//		return NULL;
//
//	// Пишем данные в pBufferPtr
//
//	for ( int nIndex = 0; nIndex < nWidth * nHeight; nIndex++, pBufferPtr += 4, pBuffer += 3 ) 
//	{
//		pBufferPtr[0] = pBuffer[ 2 ];
//		pBufferPtr[1] = pBuffer[ 1 ];
//		pBufferPtr[2] = pBuffer[ 0 ];
//		pBufferPtr[3] = 255;
//	}
//
//	// Создаем темповый файл для записи туда Jpx
//	CString wsTempFile, wsTempAlpha;
//	if ( 0 == DocGetTempFile( pPDF, &wsTempFile ) )
//	{
//		SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
//		return NULL;
//	}
//
//	// Записываем во временный файл картинку в формате Jpx
//	Jpx::IJ2kFile *pJpx = NULL;
//	::CoCreateInstance( __uuidof( Jpx::CJ2kFile ), NULL, CLSCTX_ALL, __uuidof( Jpx::IJ2kFile ), (void**)&pJpx );
//
//	if ( !pJpx )
//		return NULL;
//
//	CString sOptions;
//	sOptions.Format( _T("<Jpeg2000-Options><SaveOptions><ComponentsCount value='3'/><Format value='1' /></SaveOptions></Jpeg2000-Options>" ), nWidth, nHeight );
//
//	BSTR bsTempFile = ::SysAllocString( wsTempFile );
//	BSTR bsOptions  = sOptions.AllocSysString();
//	pJpx->InterfaceToJ2k( (IUnknown **)&pInterface, bsTempFile, bsOptions );
//
//	::SysFreeString( bsTempFile );
//	::SysFreeString( bsOptions );
//
//	RELEASEINTERFACE( pInterface );
//	RELEASEINTERFACE( pJpx );
//
//	if ( bAlpha )
//	{
//		// Переводим Image -> IAVSUncompressedVideoFrame
//		pInterface = NULL;
//		hRes = CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pInterface );
//		if ( hRes != S_OK )
//			return NULL;
//
//		pInterface->put_ColorSpace( ( 1 << 6 ) ); // CPS_BGRA
//		pInterface->put_Width( nWidth );
//		pInterface->put_Height( nHeight );
//		pInterface->put_AspectRatioX( nWidth );
//		pInterface->put_AspectRatioY( nHeight );
//		pInterface->put_Interlaced( VARIANT_FALSE );
//		pInterface->put_Stride( 0, 4 * nWidth );
//		pInterface->AllocateBuffer( nBufferSize );
//
//		pBufferPtr = NULL;
//		nCreatedBufferSize = 0;
//
//		pInterface->get_Buffer( &pBufferPtr );
//		pInterface->get_BufferSize( &nCreatedBufferSize );
//		pInterface->put_Plane( 0, pBufferPtr );
//
//		if ( !pBufferPtr || nCreatedBufferSize != nBufferSize )
//			return NULL;
//
//		// Пишем данные в pBufferPtr
//
//		for ( int nIndex = 0; nIndex < nWidth * nHeight; nIndex++, pBufferPtr += 4 ) 
//		{
//			pBufferPtr[0] = pAlphaBuffer[ nIndex ];
//			pBufferPtr[1] = pAlphaBuffer[ nIndex ];
//			pBufferPtr[2] = pAlphaBuffer[ nIndex ];
//			pBufferPtr[3] = 255;
//		}
//
//		if ( 0 == DocGetTempFile( pPDF, &wsTempAlpha ) )
//		{
//			SetError( pPDF->oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_FILE_WRITE_ERROR, errno );
//			return NULL;
//		}
//
//
//		// Записываем во временный файл картинку в формате Jpx
//		Jpx::IJ2kFile *pJpx = NULL;
//		::CoCreateInstance( __uuidof( Jpx::CJ2kFile ), NULL, CLSCTX_ALL, __uuidof( Jpx::IJ2kFile ), (void**)&pJpx );
//
//		if ( !pJpx )
//			return NULL;
//
//		CString sOptions;
//		sOptions.Format( _T("<Jpeg2000-Options><SaveOptions><ComponentsCount value='1'/><Format value='0' /><Tile width='%d' height='%d' /></SaveOptions></Jpeg2000-Options>" ), nWidth, nHeight );
//
//		BSTR bsTempFile = ::SysAllocString( wsTempAlpha );
//		BSTR bsOptions  = sOptions.AllocSysString();
//		pJpx->InterfaceToJ2k( (IUnknown **)&pInterface, bsTempFile, bsOptions );
//
//		::SysFreeString( bsTempFile );
//		::SysFreeString( bsOptions );
//
//		RELEASEINTERFACE( pInterface );
//		RELEASEINTERFACE( pJpx );
//	}
//
//	ImageDict pImage = ImageLoadJpxImage ( pPDF->oMMgr, wsTempFile.GetBuffer(), pPDF->pXref, nWidth, nHeight, unImageCheckSum, bAlpha, wsTempAlpha.GetBuffer(), unAlphaCheckSum );
//
//    if ( !pImage )
//        CheckError( &pPDF->oError );
//
//#ifdef FILTER_FLATE_DECODE_ENABLED
//    if ( pPDF->nCompressionMode & COMP_IMAGE )
//        pImage->nFilter |= STREAM_FILTER_FLATE_DECODE;
//#endif
//
//    return pImage;
//#endif
//}

//----------------- Catalog -------------------------------------------------------------------------
PageLayout    GetPageLayout        (Doc pPDF);
unsigned long SetPageLayout        (Doc pPDF, PageLayout eLayout);
PageMode      GetPageMode          (Doc pPDF);
unsigned long SetPageMode          (Doc pPDF, PageMode eMode);
unsigned long SetOpenAction        (Doc pPDF, Destination pOpenAction);
unsigned int  GetViewerPreference  (Doc pPDF);
unsigned long SetViewerPreference  (Doc pPDF, unsigned int nValue);
unsigned long AddPageLabel         (Doc pPDF, unsigned int nPageNum, PageNumStyle eStyle, unsigned int nFirstPage, const char *sPrefix);
//----------------- Info -----------------------------------------------------------------------------
Dict   GetInfo            (Doc pPDF);
unsigned long SetInfoAttr        (Doc pPDF, InfoType eType, const char *sValue);
const char*   GetInfoAttr        (Doc pPDF, InfoType eType);
unsigned long SetInfoDateAttr    (Doc pPDF, InfoType eType, TDate oDate);
Outline       CreateOutline      (Doc pPDF, Outline pParent, const char *sTitle, EncoderRecPtr pEncoder);
ExtGState     CreateExtGState    (Doc pPDF);
ExtGState     FindExtGState      (Doc pPDF, float fAlphaStroke = -1, float fAlphaFill = -1, BlendMode eMode = BMEOF, int nStrokeAdjustment = -1);
ExtGState     GetExtGState       (Doc pPDF, float fAlphaStroke = -1, float fAlphaFill = -1, BlendMode eMode = BMEOF, int nStrokeAdjustment = -1);
unsigned long SetCompressionMode (Doc pPDF, unsigned int nMode);
unsigned long GetError           (Doc pPDF);
unsigned long GetErrorDetail     (Doc pPDF);
void          ResetError         (Doc pPDF);
//----------------- Основные функции -----------------------------------------------------------------
BOOL          DocValidate            (Doc pPDF);
BOOL          HasDoc                 (Doc pPDF);
void          FreeDoc                (Doc pPDF);
void          FreeDocAll             (Doc pPDF);
void          Free                   (Doc pPDF);
unsigned long NewDoc                 (Doc pPDF);
Doc           NewEx                  (ErrorHandler pUserErrorFn, AllocFunc pUserAllocFn, FreeFunc pUserFreeFn, unsigned int nMemPoolBufSize, void *pUserData);
Doc           New                    (ErrorHandler pUserErrorFn, void *pUserData);
Doc           New                    (void);
unsigned long SetPagesConfiguration  (Doc pPDF, unsigned int nPagePerPages);
unsigned long DocSetEncryptOn        (Doc pPDF);
unsigned long SetPassword            (Doc pPDF, const char *sOwnerPassword, const char *sUserPassword);
unsigned long SetPermission          (Doc pPDF, unsigned int nPermission);
unsigned long SetEncryptionMode      (Doc pPDF, EncryptMode eMode, unsigned int nKeyLen);
unsigned long DocSetEncryptOff       (Doc pPDF);
unsigned long DocPrepareEncryption   (Doc pPDF);
unsigned long SaveToStream           (Doc pPDF);
unsigned int  GetStreamSize          (Doc pPDF);
unsigned long ReadFromStream         (Doc pPDF, BYTE *pBuf, unsigned int *pnSize);
unsigned long ResetStream            (Doc pPDF);
unsigned long SaveDocToFile          (Doc pPDF, std::wstring & wsFileName);
Page          GetCurrentPage         (Doc pPDF);
Page          GetPageByIndex         (Doc pPDF, unsigned int nIndex);
Pages         DocGetCurrentPages     (Doc pPDF);
unsigned long DocSetCurrentPages     (Doc pPDF, Pages pPages);
unsigned long DocSetCurrentPage      (Doc pPDF, Page pPage);
unsigned long DocSetTempDir          (Doc pPDF, wchar_t *wsTempDir);
Page          AddPage                (Doc pPDF);
Pages DocAddPagesTo (Doc pPDF, Pages pParent);
Page          InsertPage             (Doc pPDF, Page pTarget);
unsigned long SetErrorHandler        (Doc pPDF, ErrorHandler pUserErrorFn);
#endif /* _DOC_H */

