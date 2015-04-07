#ifndef _IMAGE_H
#define _IMAGE_H

#pragma once 

#include "Objects.h"
#include "Utils.h"

//static const char *c_sColorCMYK = "DeviceCMYK";
//static const char *c_sColorRGB  = "DeviceRGB";
//static const char *c_sColorGray = "DeviceGray";

////------------ Image вспомогательные функции -----------------------------------------------------------
//static unsigned long LoadJpegHeader(ImageDict pImage, StreamRecPtr pStream)
//{
//    unsigned short nTag = 0;
//    unsigned short nHeight = 0;
//    unsigned short nWidth  = 0; 
//    BYTE nPrecision;
//    BYTE nNumComponents;
//    const char *sColorSpaceName;
//	unsigned int nLen = 2;
//	unsigned long nRet = 0;
//
//	Array oArray;
//
//    if ( OK != StreamRead( pStream, (BYTE *)&nTag, &nLen ) )
//		return ErrorGetCode( pStream->oError );
//
//    UInt16Swap(&nTag);
//
//	//проверяем сигнатуру: JPEG всегда начинается с FFD8 и кончается FFD9
//    if ( 0xFFD8 != nTag )
//        return AVS_OFFICEPDFWRITER_ERROR_INVALID_JPEG_DATA;
//
//    /* find SOF record */
//    for (;;) 
//	{
//        unsigned short nSize = 0;
//        nLen = 2;
//
//        if ( OK != StreamRead( pStream, (BYTE *)&nTag,  &nLen) )
//			return ErrorGetCode( pStream->oError );
//
//        UInt16Swap(&nTag);
//
//		nLen = 2;
//        if ( OK != StreamRead( pStream, (BYTE *)&nSize,  &nLen) )
//			return ErrorGetCode( pStream->oError );
//
//
//        UInt16Swap(&nSize);
//
//        if ( 0xFFC0 == nTag || 0xFFC1 == nTag || 0xFFC2 == nTag || 0xFFC9 == nTag ) 
//		{
//
//            nLen = 1;
//            if ( OK != StreamRead( pStream, (BYTE *)&nPrecision, &nLen) )
//				return ErrorGetCode( pStream->oError );
//
//            nLen = 2;
//            if ( OK != StreamRead( pStream, (BYTE *)&nHeight, &nLen) )
//				return ErrorGetCode( pStream->oError );
//
//            UInt16Swap(&nHeight);
//
//            nLen = 2;
//            if ( OK != StreamRead( pStream, (BYTE *)&nWidth, &nLen) )
//				return ErrorGetCode( pStream->oError );
//
//            UInt16Swap(&nWidth);
//
//            nLen = 1;
//            if ( OK != StreamRead( pStream, (BYTE *)&nNumComponents, &nLen) )
//				return ErrorGetCode( pStream->oError );
//
//            break;
//        } 
//		else if ( ( nTag | 0x00FF ) != 0xFFFF )
//            /* lost marker */
//			return SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT, 0);
//
//        if ( OK != StreamSeek( pStream, nSize - 2, SeekCur ) )
//				return ErrorGetCode( pStream->oError );
//    }
//
//    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
//		return ErrorGetCode( pStream->oError );
//
//    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
//		return ErrorGetCode( pStream->oError );
//
//    // Классификация RGB и CMYK не совершенная, т.к. 
//    // YCbCr или YCCK классифицируются как RGB или CMYK.
//    // Необходимо прочитать данные APP14 для точного определения
//	// цветового пространства.
//    switch ( nNumComponents ) 
//	{
//        case 1:
//            sColorSpaceName = c_sColorGray;
//            break;
//        case 3:
//            sColorSpaceName = c_sColorRGB;
//            break;
//        case 4:
//			oArray = ArrayNew( pImage->oMMgr );
//            if ( !oArray )
//				return ErrorGetCode( pStream->oError );
//
//            nRet = DictAdd( pImage, "Decode", oArray );
//            if ( OK != nRet )
//				return ErrorGetCode( pStream->oError );
//
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
//			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
//
//            if ( OK != nRet )
//				return ErrorGetCode( pStream->oError );
//
//            sColorSpaceName = c_sColorCMYK;
//
//            break;
//        default:
//			return SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT, 0);
//    }
//
//	if ( OK != DictAdd( pImage, "ColorSpace", NameNew( pImage->oMMgr, sColorSpaceName ) ) )
//		return ErrorGetCode( pStream->oError );
//
//	if ( OK != DictAdd( pImage, "BitsPerComponent", NumberNew( pImage->oMMgr, nPrecision ) ) )
//		return ErrorGetCode( pStream->oError );
//
//    return OK;
//}
//
//static BOOL          ReadJpxBoxHeader   (StreamRecPtr pStream, unsigned int *punBoxType, unsigned int *punBoxLen, unsigned int *punDataLen) 
//{
//	unsigned int unLen, unLenH;
//	unsigned int unReadLen = 4;
//
//	if ( OK != StreamReadULong( pStream,  &unLen ) || OK != StreamReadULong( pStream, punBoxType ) ) 
//	{
//		return FALSE;
//	}
//	if ( 1 == unLen ) 
//	{
//		if ( OK != StreamReadULong( pStream,  &unLenH ) || OK != StreamReadULong( pStream,  &unLen ) ) 
//		{
//			return FALSE;
//		}
//		if ( unLenH ) 
//		{
//			// TO DO: Error "JPX stream contains a box larger than 2^32 bytes"
//			return FALSE;
//		}
//		*punBoxLen  = unLen;
//		*punDataLen = unLen - 16;
//	} 
//	else if ( 0 == unLen ) 
//	{
//		*punBoxLen  = 0;
//		*punDataLen = 0;
//	} 
//	else 
//	{
//		*punBoxLen  = unLen;
//		*punDataLen = unLen - 8;
//	}
//	return TRUE;
//}
//
//static int           ReadJpxMarkerHeader(StreamRecPtr pStream, int *pnSegmentType, unsigned int *pnSegmentLen) 
//{
//	int nChar = 0;
//
//	do 
//	{
//		do 
//		{
//			unsigned int nReadLen = 1;
//			if ( OK != StreamRead( pStream, (BYTE*)&nChar, &nReadLen ) )
//				return FALSE;
//
//			if ( nChar == EOF ) 
//			{
//				return FALSE;
//			}
//		} while ( nChar != 0xff );
//		do 
//		{
//			unsigned int nReadLen = 1;
//			if ( OK != StreamRead( pStream, (BYTE*)&nChar, &nReadLen ) )
//				return FALSE;
//
//			if ( nChar == EOF)  
//			{
//				return FALSE;
//			}
//		} while ( nChar == 0xff );
//	} while ( nChar == 0x00 );
//	*pnSegmentType = nChar;
//	if ( ( nChar >= 0x30 && nChar <= 0x3f ) || nChar == 0x4f || nChar == 0x92 || nChar == 0x93 || nChar == 0xd9 ) 
//	{
//		*pnSegmentLen = 0;
//		return TRUE;
//	}
//
//	if ( OK != StreamReadUShort( pStream, pnSegmentLen ) )
//		return FALSE;
//
//	return TRUE;
//}
//
//static const char*   ReadJpxCodestream  (StreamRecPtr pStream, unsigned int *pnBitsPerComponent, unsigned int *pnWidth, unsigned int *pnHeight)
//{
//	int nSegmentType;
//	unsigned int nSegmentLen;
//
//	while ( ReadJpxMarkerHeader( pStream, &nSegmentType, & nSegmentLen ) )
//	{
//		if ( 0x51 == nSegmentType )
//		{
//			unsigned int nTemp, nComponentsCount, nBPC, nXsiz, nYsiz, nXOsiz, nYOsiz;
//			if ( OK == StreamReadUShort( pStream, &nTemp   ) && OK == StreamReadULong( pStream, &nXsiz  ) && OK == StreamReadULong( pStream, &nYsiz ) &&
//				 OK == StreamReadULong ( pStream, &nXOsiz  ) && OK == StreamReadULong( pStream, &nYOsiz ) && OK == StreamReadULong( pStream, &nTemp ) &&
//				 OK == StreamReadULong ( pStream, &nTemp   ) && OK == StreamReadULong( pStream, &nTemp  ) && OK == StreamReadULong( pStream, &nTemp ) &&
//				 OK == StreamReadUShort( pStream, &nComponentsCount ) && OK == StreamReadUByte( pStream, &nBPC  ) ) 
//			{
//				*pnBitsPerComponent = (nBPC & 0x7f) + 1;
//				*pnWidth  = nXsiz - nXOsiz;
//				*pnHeight = nYsiz - nYOsiz;
//
//				if ( nComponentsCount == 1 ) 
//				{
//					return c_sColorGray;
//				} 
//				else if ( nComponentsCount == 3 ) 
//				{
//					return c_sColorRGB;
//				} 
//				else if ( nComponentsCount == 4 ) 
//				{
//					return c_sColorRGB;
//				}
//			}
//			break;
//		}
//	}
//	return "";
//}
//static unsigned long LoadJpxHeader (ImageDict pImage, StreamRecPtr pStream, long nOpacityFlag = 0)
//{
//	unsigned int nLen  = 1;
//	int nChar = 10;
//
//	if ( OK != StreamRead( pStream, (BYTE *)&nChar, &nLen ) )
//		return ErrorGetCode( pStream->oError );
//
//	StreamSeek( pStream, 0, SeekSet );
//
//	BOOL bHaveBPC = FALSE, bHaveModeCS = FALSE;
//	unsigned int nBitsPerComponent = 0;
//	const char *sColorSpaceName = "";
//	unsigned int nCSPrec = 0;
//	unsigned int nWidth = 0, nHeight = 0;
//
//	if ( 0xff == nChar )
//	{
//		sColorSpaceName = ReadJpxCodestream( pStream, &nBitsPerComponent, &nWidth, &nHeight );
//	}
//	else
//	{
//		unsigned int nBoxType, nBoxLen, nDataLen;
//		while( ReadJpxBoxHeader( pStream, &nBoxType, &nBoxLen, &nDataLen  ) )
//		{
//			if ( nBoxType == 0x6a703268 ) // JP2 header
//			{
//				// ничего не делаем
//			} 
//			else if ( nBoxType == 0x69686472 ) // Image header
//			{
//				unsigned int nBPC, nTemp;
//				if ( OK == StreamReadULong( pStream, &nTemp ) && OK == StreamReadULong( pStream, &nTemp ) && OK == StreamReadUShort( pStream, &nTemp ) &&
//					 OK == StreamReadUByte( pStream, &nBPC  ) && OK == StreamReadUByte( pStream, &nTemp ) && OK == StreamReadUByte(  pStream, &nTemp ) &&
//					 OK == StreamReadUByte( pStream, &nTemp ) ) 
//				{
//					nBitsPerComponent = nBPC + 1;
//					bHaveBPC = TRUE;
//				}
//			} 
//			else if ( nBoxType == 0x636F6C72 ) // Color specification
//			{
//				int nMethodCS, nPrecedenceCS, nTemp;
//				if ( OK == StreamReadByte( pStream, &nMethodCS ) && OK == StreamReadByte( pStream, &nPrecedenceCS ) && OK == StreamReadByte( pStream, &nTemp ) ) 
//				{
//					if ( nMethodCS == 1 ) 
//					{
//						unsigned int nEnumCS;
//						if ( OK == StreamReadULong( pStream, &nEnumCS ) ) 
//						{
//							const char *sTempCS = "";
//							if ( nEnumCS == 0 || nEnumCS == 17 ) 
//							{
//								sTempCS = c_sColorGray;
//							} 
//							else if ( nEnumCS == 12 ) 
//							{
//								sTempCS = c_sColorCMYK;
//							} 
//							else if ( nEnumCS == 16 || nEnumCS == 20 || nEnumCS == 21 ) 
//							{
//								sTempCS = c_sColorRGB;
//							}
//
//							if ( strlen( sTempCS ) > 0 && ( !bHaveModeCS || nPrecedenceCS > nCSPrec ) ) 
//							{
//								sColorSpaceName = sTempCS;
//								nCSPrec = nPrecedenceCS;
//								bHaveModeCS = TRUE;
//							}
//
//							StreamSeek( pStream, nDataLen - 7, SeekCur );
//						}
//					} 
//					else 
//					{
//						StreamSeek( pStream, nDataLen - 3, SeekCur );
//					}
//				}
//			} 
//			else if ( nBoxType == 0x6A703263 ) // Codestream
//			{
//				const char* sTempCS = ReadJpxCodestream( pStream, &nBitsPerComponent, &nWidth, &nHeight );
//				if ( strlen( sColorSpaceName ) <= 0 )
//					sColorSpaceName = sTempCS;
//
//			} 
//			else 
//			{
//				StreamSeek( pStream, nDataLen, SeekCur );
//			}
//		}
//	}
//
//    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
//		return ErrorGetCode( pStream->oError );
//
//    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
//		return ErrorGetCode( pStream->oError );
//
//	// По спецификации, ColorSpace и BitsPerComponent можно опустить, но тогда даже AcrobatReader не всегда правильно читает данные изображения
//	if ( OK != DictAdd( pImage, "ColorSpace", NameNew( pImage->oMMgr, sColorSpaceName ) ) )
//		return ErrorGetCode( pStream->oError );
//
//	if ( OK != DictAdd( pImage, "BitsPerComponent", NumberNew( pImage->oMMgr, nBitsPerComponent ) ) )
//		return ErrorGetCode( pStream->oError );
//
//	// Предполагаем, что значения nOpacityFlag могут быть 0, 1 или 2
//	if ( OK != DictAdd( pImage, "SMaskInData", NumberNew( pImage->oMMgr, nOpacityFlag ) ) )
//		return ErrorGetCode( pStream->oError );
//
//	return OK;
//
//}
//------------ Image основные функции ------------------------------------------------------------------
ImageDict ImageLoadJpegImage       (MMgr oMMgr, StreamRecPtr pJpegData,          Xref pXref);
ImageDict ImageLoadJpxImage        (MMgr oMMgr, StreamRecPtr pJpxData,           Xref pXref, long nOpacityFlag);
ImageDict ImageLoadRawImage        (MMgr oMMgr, StreamRecPtr pRawData,           Xref pXref, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace);
ImageDict ImageLoadAlphaFromMem    (MMgr oMMgr, const BYTE *pBuffer,       Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int nBitsPerComponent);
ImageDict ImageLoadRawImageFromMem (MMgr oMMgr, const BYTE *pBuffer,       Xref pXref, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace, unsigned int nBitsPerComponent, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL);
ImageDict ImageLoadJBig2Image      (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum);
ImageDict ImageLoadJpxImage        (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const wchar_t *wsAlphaPath = NULL, unsigned int unAlphaCheckSum = 0);
ImageDict ImageLoadJpegImage       (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const wchar_t *wsAlphaPath = NULL, unsigned int unAlphaCheckSum = 0);
BOOL          ImageValidate           (ImageDict pImage);
TPoint        ImageGetSize            (ImageDict pImage);
unsigned long ImageGetSize2           (ImageDict pImage, TPoint *poSize);
unsigned int  ImageGetBitsPerComponent(ImageDict pImage);
const char*   ImageGetColorSpace      (ImageDict pImage);
EColorSpace   ImageGetColorSpace2     (ImageDict pImage);
unsigned int  ImageGetWidth           (ImageDict pImage);
unsigned int  ImageGetHeight          (ImageDict pImage);
ImageDict     ImageGetSMask           (ImageDict pImage);
unsigned long ImageSetMask            (ImageDict pImage, BOOL bMask);
unsigned long ImageSetMaskImage       (ImageDict pImage, ImageDict pMaskImage);
unsigned long ImageSetColorMask       (ImageDict pImage, unsigned int nRMin, unsigned int nRMax, unsigned int nGMin, unsigned int nGMax, unsigned int nBMin, unsigned int nBMax);
#endif /* _IMAGE_H */

