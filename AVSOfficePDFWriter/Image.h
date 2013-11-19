#ifndef _IMAGE_H
#define _IMAGE_H

#include "Objects.h"
#include "Utils.h"

static const char *c_sColorCMYK = "DeviceCMYK";
static const char *c_sColorRGB  = "DeviceRGB";
static const char *c_sColorGray = "DeviceGray";

//------------ Image вспомогательные функции -----------------------------------------------------------
static unsigned long LoadJpegHeader(ImageDict pImage, Stream pStream)
{
    unsigned short nTag = 0;
    unsigned short nHeight = 0;
    unsigned short nWidth  = 0; 
    BYTE nPrecision;
    BYTE nNumComponents;
    const char *sColorSpaceName;
	unsigned int nLen = 2;
	unsigned long nRet = 0;

	Array oArray;

    if ( OK != StreamRead( pStream, (BYTE *)&nTag, &nLen ) )
		return ErrorGetCode( pStream->oError );

    UInt16Swap(&nTag);

	//проверяем сигнатуру: JPEG всегда начинается с FFD8 и кончается FFD9
    if ( 0xFFD8 != nTag )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_JPEG_DATA;

    /* find SOF record */
    for (;;) 
	{
        unsigned short nSize = 0;
        nLen = 2;

        if ( OK != StreamRead( pStream, (BYTE *)&nTag,  &nLen) )
			return ErrorGetCode( pStream->oError );

        UInt16Swap(&nTag);

		nLen = 2;
        if ( OK != StreamRead( pStream, (BYTE *)&nSize,  &nLen) )
			return ErrorGetCode( pStream->oError );


        UInt16Swap(&nSize);

        if ( 0xFFC0 == nTag || 0xFFC1 == nTag || 0xFFC2 == nTag || 0xFFC9 == nTag ) 
		{

            nLen = 1;
            if ( OK != StreamRead( pStream, (BYTE *)&nPrecision, &nLen) )
				return ErrorGetCode( pStream->oError );

            nLen = 2;
            if ( OK != StreamRead( pStream, (BYTE *)&nHeight, &nLen) )
				return ErrorGetCode( pStream->oError );

            UInt16Swap(&nHeight);

            nLen = 2;
            if ( OK != StreamRead( pStream, (BYTE *)&nWidth, &nLen) )
				return ErrorGetCode( pStream->oError );

            UInt16Swap(&nWidth);

            nLen = 1;
            if ( OK != StreamRead( pStream, (BYTE *)&nNumComponents, &nLen) )
				return ErrorGetCode( pStream->oError );

            break;
        } 
		else if ( ( nTag | 0x00FF ) != 0xFFFF )
            /* lost marker */
			return SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT, 0);

        if ( OK != StreamSeek( pStream, nSize - 2, SeekCur ) )
				return ErrorGetCode( pStream->oError );
    }

    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
		return ErrorGetCode( pStream->oError );

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
		return ErrorGetCode( pStream->oError );

    // Классификация RGB и CMYK не совершенная, т.к. 
    // YCbCr или YCCK классифицируются как RGB или CMYK.
    // Необходимо прочитать данные APP14 для точного определения
	// цветового пространства.
    switch ( nNumComponents ) 
	{
        case 1:
            sColorSpaceName = c_sColorGray;
            break;
        case 3:
            sColorSpaceName = c_sColorRGB;
            break;
        case 4:
			oArray = ArrayNew( pImage->oMMgr );
            if ( !oArray )
				return ErrorGetCode( pStream->oError );

            nRet = DictAdd( pImage, "Decode", oArray );
            if ( OK != nRet )
				return ErrorGetCode( pStream->oError );

			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 1 ) );
			nRet += ArrayAdd( oArray, NumberNew( pImage->oMMgr, 0 ) );

            if ( OK != nRet )
				return ErrorGetCode( pStream->oError );

            sColorSpaceName = c_sColorCMYK;

            break;
        default:
			return SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_UNSUPPORTED_JPEG_FORMAT, 0);
    }

	if ( OK != DictAdd( pImage, "ColorSpace", NameNew( pImage->oMMgr, sColorSpaceName ) ) )
		return ErrorGetCode( pStream->oError );

	if ( OK != DictAdd( pImage, "BitsPerComponent", NumberNew( pImage->oMMgr, nPrecision ) ) )
		return ErrorGetCode( pStream->oError );

    return OK;
}

static BOOL          ReadJpxBoxHeader   (Stream pStream, unsigned int *punBoxType, unsigned int *punBoxLen, unsigned int *punDataLen) 
{
	unsigned int unLen, unLenH;
	unsigned int unReadLen = 4;

	if ( OK != StreamReadULong( pStream,  &unLen ) || OK != StreamReadULong( pStream, punBoxType ) ) 
	{
		return FALSE;
	}
	if ( 1 == unLen ) 
	{
		if ( OK != StreamReadULong( pStream,  &unLenH ) || OK != StreamReadULong( pStream,  &unLen ) ) 
		{
			return FALSE;
		}
		if ( unLenH ) 
		{
			// TO DO: Error "JPX stream contains a box larger than 2^32 bytes"
			return FALSE;
		}
		*punBoxLen  = unLen;
		*punDataLen = unLen - 16;
	} 
	else if ( 0 == unLen ) 
	{
		*punBoxLen  = 0;
		*punDataLen = 0;
	} 
	else 
	{
		*punBoxLen  = unLen;
		*punDataLen = unLen - 8;
	}
	return TRUE;
}

static int           ReadJpxMarkerHeader(Stream pStream, int *pnSegmentType, unsigned int *pnSegmentLen) 
{
	int nChar = 0;

	do 
	{
		do 
		{
			unsigned int nReadLen = 1;
			if ( OK != StreamRead( pStream, (BYTE*)&nChar, &nReadLen ) )
				return FALSE;

			if ( nChar == EOF ) 
			{
				return FALSE;
			}
		} while ( nChar != 0xff );
		do 
		{
			unsigned int nReadLen = 1;
			if ( OK != StreamRead( pStream, (BYTE*)&nChar, &nReadLen ) )
				return FALSE;

			if ( nChar == EOF)  
			{
				return FALSE;
			}
		} while ( nChar == 0xff );
	} while ( nChar == 0x00 );
	*pnSegmentType = nChar;
	if ( ( nChar >= 0x30 && nChar <= 0x3f ) || nChar == 0x4f || nChar == 0x92 || nChar == 0x93 || nChar == 0xd9 ) 
	{
		*pnSegmentLen = 0;
		return TRUE;
	}

	if ( OK != StreamReadUShort( pStream, pnSegmentLen ) )
		return FALSE;

	return TRUE;
}

static const char*   ReadJpxCodestream  (Stream pStream, unsigned int *pnBitsPerComponent, unsigned int *pnWidth, unsigned int *pnHeight)
{
	int nSegmentType;
	unsigned int nSegmentLen;

	while ( ReadJpxMarkerHeader( pStream, &nSegmentType, & nSegmentLen ) )
	{
		if ( 0x51 == nSegmentType )
		{
			unsigned int nTemp, nComponentsCount, nBPC, nXsiz, nYsiz, nXOsiz, nYOsiz;
			if ( OK == StreamReadUShort( pStream, &nTemp   ) && OK == StreamReadULong( pStream, &nXsiz  ) && OK == StreamReadULong( pStream, &nYsiz ) &&
				 OK == StreamReadULong ( pStream, &nXOsiz  ) && OK == StreamReadULong( pStream, &nYOsiz ) && OK == StreamReadULong( pStream, &nTemp ) &&
				 OK == StreamReadULong ( pStream, &nTemp   ) && OK == StreamReadULong( pStream, &nTemp  ) && OK == StreamReadULong( pStream, &nTemp ) &&
				 OK == StreamReadUShort( pStream, &nComponentsCount ) && OK == StreamReadUByte( pStream, &nBPC  ) ) 
			{
				*pnBitsPerComponent = (nBPC & 0x7f) + 1;
				*pnWidth  = nXsiz - nXOsiz;
				*pnHeight = nYsiz - nYOsiz;

				if ( nComponentsCount == 1 ) 
				{
					return c_sColorGray;
				} 
				else if ( nComponentsCount == 3 ) 
				{
					return c_sColorRGB;
				} 
				else if ( nComponentsCount == 4 ) 
				{
					return c_sColorRGB;
				}
			}
			break;
		}
	}
	return "";
}
static unsigned long LoadJpxHeader (ImageDict pImage, Stream pStream, long nOpacityFlag = 0)
{
	unsigned int nLen  = 1;
	int nChar = 10;

	if ( OK != StreamRead( pStream, (BYTE *)&nChar, &nLen ) )
		return ErrorGetCode( pStream->oError );

	StreamSeek( pStream, 0, SeekSet );

	BOOL bHaveBPC = FALSE, bHaveModeCS = FALSE;
	unsigned int nBitsPerComponent = 0;
	const char *sColorSpaceName = "";
	unsigned int nCSPrec = 0;
	unsigned int nWidth = 0, nHeight = 0;

	if ( 0xff == nChar )
	{
		sColorSpaceName = ReadJpxCodestream( pStream, &nBitsPerComponent, &nWidth, &nHeight );
	}
	else
	{
		unsigned int nBoxType, nBoxLen, nDataLen;
		while( ReadJpxBoxHeader( pStream, &nBoxType, &nBoxLen, &nDataLen  ) )
		{
			if ( nBoxType == 0x6a703268 ) // JP2 header
			{
				// ничего не делаем
			} 
			else if ( nBoxType == 0x69686472 ) // Image header
			{
				unsigned int nBPC, nTemp;
				if ( OK == StreamReadULong( pStream, &nTemp ) && OK == StreamReadULong( pStream, &nTemp ) && OK == StreamReadUShort( pStream, &nTemp ) &&
					 OK == StreamReadUByte( pStream, &nBPC  ) && OK == StreamReadUByte( pStream, &nTemp ) && OK == StreamReadUByte(  pStream, &nTemp ) &&
					 OK == StreamReadUByte( pStream, &nTemp ) ) 
				{
					nBitsPerComponent = nBPC + 1;
					bHaveBPC = TRUE;
				}
			} 
			else if ( nBoxType == 0x636F6C72 ) // Color specification
			{
				int nMethodCS, nPrecedenceCS, nTemp;
				if ( OK == StreamReadByte( pStream, &nMethodCS ) && OK == StreamReadByte( pStream, &nPrecedenceCS ) && OK == StreamReadByte( pStream, &nTemp ) ) 
				{
					if ( nMethodCS == 1 ) 
					{
						unsigned int nEnumCS;
						if ( OK == StreamReadULong( pStream, &nEnumCS ) ) 
						{
							const char *sTempCS = "";
							if ( nEnumCS == 0 || nEnumCS == 17 ) 
							{
								sTempCS = c_sColorGray;
							} 
							else if ( nEnumCS == 12 ) 
							{
								sTempCS = c_sColorCMYK;
							} 
							else if ( nEnumCS == 16 || nEnumCS == 20 || nEnumCS == 21 ) 
							{
								sTempCS = c_sColorRGB;
							}

							if ( strlen( sTempCS ) > 0 && ( !bHaveModeCS || nPrecedenceCS > nCSPrec ) ) 
							{
								sColorSpaceName = sTempCS;
								nCSPrec = nPrecedenceCS;
								bHaveModeCS = TRUE;
							}

							StreamSeek( pStream, nDataLen - 7, SeekCur );
						}
					} 
					else 
					{
						StreamSeek( pStream, nDataLen - 3, SeekCur );
					}
				}
			} 
			else if ( nBoxType == 0x6A703263 ) // Codestream
			{
				const char* sTempCS = ReadJpxCodestream( pStream, &nBitsPerComponent, &nWidth, &nHeight );
				if ( strlen( sColorSpaceName ) <= 0 )
					sColorSpaceName = sTempCS;

			} 
			else 
			{
				StreamSeek( pStream, nDataLen, SeekCur );
			}
		}
	}

    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
		return ErrorGetCode( pStream->oError );

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
		return ErrorGetCode( pStream->oError );

	// По спецификации, ColorSpace и BitsPerComponent можно опустить, но тогда даже AcrobatReader не всегда правильно читает данные изображения
	if ( OK != DictAdd( pImage, "ColorSpace", NameNew( pImage->oMMgr, sColorSpaceName ) ) )
		return ErrorGetCode( pStream->oError );

	if ( OK != DictAdd( pImage, "BitsPerComponent", NumberNew( pImage->oMMgr, nBitsPerComponent ) ) )
		return ErrorGetCode( pStream->oError );

	// Предполагаем, что значения nOpacityFlag могут быть 0, 1 или 2
	if ( OK != DictAdd( pImage, "SMaskInData", NumberNew( pImage->oMMgr, nOpacityFlag ) ) )
		return ErrorGetCode( pStream->oError );

	return OK;

}
//------------ Image основные функции ------------------------------------------------------------------
ImageDict ImageLoadJpegImage       (MMgr oMMgr, Stream pJpegData,          Xref pXref)
{
    Dict pImage;
    unsigned long nRet = OK;

    pImage = DictStreamNew( oMMgr, pXref );
    if ( !pImage )
        return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;

    // Добавляем необходимые элементы
	pImage->nFilter = STREAM_FILTER_DCT_DECODE;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");

	if ( OK != nRet )
        return NULL;

    if ( OK != LoadJpegHeader( pImage, pJpegData ) )
        return NULL;

    if ( OK != StreamSeek( pJpegData, 0, SeekSet ) )
        return NULL;

    for (;;) 
	{
        BYTE pBuffer[STREAM_BUF_SIZ];
        unsigned int nLen = STREAM_BUF_SIZ;
        unsigned long nRet = StreamRead( pJpegData, pBuffer, &nLen );

        if ( OK != nRet ) 
		{
            if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
			{
                if ( nLen > 0 ) 
				{
					nRet = StreamWrite( pImage->pStream, pBuffer, nLen );
                    if ( OK != nRet )
                        return NULL;
                }
                break;
            } 
			else
                return NULL;
        }

		if ( OK != StreamWrite( pImage->pStream, pBuffer, nLen ) )
            return NULL;
    }

    return pImage;
}

ImageDict ImageLoadJpxImage        (MMgr oMMgr, Stream pJpxData,           Xref pXref, long nOpacityFlag)
{
	Dict pImage = NULL;
	unsigned long nRet = OK;

	pImage = DictStreamNew( oMMgr, pXref );
	if ( !pImage )
		return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
    // Добавляем необходимые элементы
	pImage->nFilter = STREAM_FILTER_JPX_DECODE;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");

	if ( OK != nRet )
        return NULL;

	if ( OK != LoadJpxHeader( pImage, pJpxData, nOpacityFlag ) )
		return NULL;

    if ( OK != StreamSeek( pJpxData, 0, SeekSet ) )
        return NULL;

    for (;;) 
	{
        BYTE pBuffer[STREAM_BUF_SIZ];
        unsigned int  nLen = STREAM_BUF_SIZ;
        unsigned long nRet = StreamRead( pJpxData, pBuffer, &nLen );

        if ( OK != nRet ) 
		{
            if ( AVS_OFFICEPDFWRITER_ERROR_STREAM_EOF == nRet ) 
			{
                if ( nLen > 0 ) 
				{
					nRet = StreamWrite( pImage->pStream, pBuffer, nLen );
                    if ( OK != nRet )
                        return NULL;
                }
                break;
            } 
			else
                return NULL;
        }

		if ( OK != StreamWrite( pImage->pStream, pBuffer, nLen ) )
            return NULL;
    }

	return pImage;
}
ImageDict ImageLoadRawImage        (MMgr oMMgr, Stream pRawData,           Xref pXref, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace)
{
    unsigned long nRet = OK;
    unsigned int nSize = 0;

    if ( CSDeviceGray != eColorSpace && CSDeviceRGB != eColorSpace && CSDeviceCMYK != eColorSpace ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE, 0);
        return NULL;
    }

    Dict pImage = DictStreamNew( oMMgr, pXref );
    if ( !pImage )
        return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");
    if ( OK != nRet )
        return NULL;

    if ( CSDeviceGray == eColorSpace ) 
	{
        nSize = nWidth * nHeight;
        nRet = DictAddName( pImage, "ColorSpace", c_sColorGray );
	} 
	else if ( CSDeviceCMYK == eColorSpace ) 
	{
		nSize = nWidth * nHeight * 4;
		nRet = DictAddName( pImage, "ColorSpace", c_sColorCMYK );
    } 
	else 
	{
        nSize = nWidth * nHeight * 3;
        nRet = DictAddName( pImage, "ColorSpace", c_sColorRGB );
    }

    if ( OK != nRet )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "BitsPerComponent", 8 ) )
        return NULL;

	if ( OK != StreamWriteToStream( pRawData, pImage->pStream, 0, NULL ) )
        return NULL;

	if ( nSize != pImage->pStream->nSize ) 
	{
        SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE, 0);
        return NULL;
    }

    return pImage;
}
ImageDict ImageLoadAlphaFromMem    (MMgr oMMgr, const BYTE *pBuffer,       Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int nBitsPerComponent)
{
    unsigned long nRet = OK;
    unsigned int nSize = 0;

    if ( 1 != nBitsPerComponent && 2 != nBitsPerComponent && 4 != nBitsPerComponent && 8 != nBitsPerComponent ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE, 0);
        return NULL;
    }

    Dict pImage = DictStreamNew( oMMgr, pXref, FALSE );
    if ( !pImage )
        return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");
    if ( OK != nRet )
        return NULL;

    nSize = (double)nWidth * nHeight / ( 8 / nBitsPerComponent ) + 0.876;
    nRet = DictAddName( pImage, "ColorSpace", c_sColorGray );

    if ( OK != nRet )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "BitsPerComponent", nBitsPerComponent ) )
        return NULL;


	if ( OK != StreamWrite( pImage->pStream, pBuffer, nSize, true ) )
        return NULL;

    return pImage;
}
ImageDict ImageLoadRawImageFromMem (MMgr oMMgr, const BYTE *pBuffer,       Xref pXref, unsigned int nWidth, unsigned int nHeight, ColorSpace eColorSpace, unsigned int nBitsPerComponent, BOOL bAlpha = FALSE, const BYTE *pAlphaBuffer = NULL)
{
    unsigned long nRet = OK;
    unsigned int nSize = 0;

    if ( CSDeviceGray != eColorSpace && CSDeviceRGB != eColorSpace ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE, 0);
        return NULL;
    }

    if ( 1 != nBitsPerComponent && 2 != nBitsPerComponent && 4 != nBitsPerComponent && 8 != nBitsPerComponent ) 
	{
        SetError( oMMgr->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE, 0);
        return NULL;
    }

    Dict pImage = DictStreamNew( oMMgr, pXref, FALSE );
    if ( !pImage )
        return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");
    if ( OK != nRet )
        return NULL;

    if ( CSDeviceGray  == eColorSpace ) 
	{
        nSize = (double)nWidth * nHeight / ( 8 / nBitsPerComponent ) + 0.876;
        nRet = DictAddName( pImage, "ColorSpace", c_sColorGray );
    } 
	else 
	{
        nSize = (double)nWidth * nHeight / ( 8 / nBitsPerComponent ) + 0.876;
        nSize *= 3;
        nRet = DictAddName( pImage, "ColorSpace", c_sColorRGB );
    }

    if ( OK != nRet )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
        return NULL;

    if ( OK != DictAddNumber( pImage, "BitsPerComponent", nBitsPerComponent ) )
        return NULL;

	if ( bAlpha )
	{
		ImageDict pImageSMask = ImageLoadAlphaFromMem( oMMgr, pAlphaBuffer, pXref, nWidth, nHeight, nBitsPerComponent );
		if ( !pImageSMask )
			return NULL;
		if ( OK != DictAdd( pImage, "SMask", pImageSMask ) )
			return NULL;
#ifndef _DEBUG
		pImageSMask->nFilter = STREAM_FILTER_FLATE_DECODE;
#endif
	}

	if ( OK != StreamWrite( pImage->pStream, pBuffer, nSize, true ) )
        return NULL;

    return pImage;
}
ImageDict ImageLoadJBig2Image      (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum)
{
	Dict pImage = NULL;
	unsigned long nRet = OK;

	pImage = DictStreamNew( oMMgr, pXref, FALSE, wsTempFile, unImageCheckSum );
	if ( !pImage )
		return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
    // Добавляем необходимые элементы
	pImage->nFilter = STREAM_FILTER_JBIG2_DECODE;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");

	if ( OK != nRet )
        return NULL;

	if ( OK != DictAddNumber( pImage, "Height", nHeight ) )
		return NULL;

    if ( OK != DictAddNumber( pImage, "Width", nWidth ) )
		return NULL;

	// По спецификации, ColorSpace и BitsPerComponent можно опустить, но тогда даже AcrobatReader не всегда правильно читает данные изображения
	if ( OK != DictAdd( pImage, "ColorSpace", NameNew( pImage->oMMgr, c_sColorGray ) ) )
		return NULL;

	if ( OK != DictAdd( pImage, "BitsPerComponent", NumberNew( pImage->oMMgr, 1 ) ) )
		return NULL;

	return pImage;
}
ImageDict ImageLoadJpxImage        (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const wchar_t *wsAlphaPath = NULL, unsigned int unAlphaCheckSum = 0)
{
	Dict pImage = NULL;
	unsigned long nRet = OK;

	pImage = DictStreamNew( oMMgr, pXref, FALSE, wsTempFile, unImageCheckSum );
	if ( !pImage )
		return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;

	// Добавляем необходимые элементы
	pImage->nFilter = STREAM_FILTER_JPX_DECODE;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");
	nRet += DictAddNumber( pImage, "Height", nHeight );
	nRet += DictAddNumber( pImage, "Width",  nWidth  );
	nRet += DictAddName( pImage, "ColorSpace", c_sColorRGB );
	nRet += DictAddNumber( pImage, "BitsPerComponent", 8 );

	if ( OK != nRet )
        return NULL;

	if ( bAlpha )
	{
		Dict pImageSMask = DictStreamNew( oMMgr, pXref, FALSE, wsAlphaPath, unAlphaCheckSum );
		if ( !pImageSMask )
			return NULL;

		pImageSMask->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
		pImageSMask->nFilter = STREAM_FILTER_JPX_DECODE;
		nRet += DictAddName( pImageSMask, "Type", "XObject");
		nRet += DictAddName( pImageSMask, "Subtype", "Image");
		nRet += DictAddName( pImageSMask, "ColorSpace", c_sColorGray );
		nRet += DictAddNumber( pImageSMask, "Width", nWidth );
		nRet += DictAddNumber( pImageSMask, "Height", nHeight );
		nRet += DictAddNumber( pImageSMask, "BitsPerComponent", 8 );

		if ( OK != nRet )
			return NULL;

		if ( OK != DictAdd( pImage, "SMask", pImageSMask ) )
			return NULL;
#ifndef _DEBUG
		pImageSMask->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif
	}
	else
	{
		if ( OK != DictAdd( pImage, "SMaskInData", NumberNew( pImage->oMMgr, 0 ) ) )
			return NULL;
	}

	return pImage;
}
ImageDict ImageLoadJpegImage       (MMgr oMMgr, const wchar_t *wsTempFile, Xref pXref, unsigned int nWidth, unsigned int nHeight, unsigned int unImageCheckSum, BOOL bAlpha = FALSE, const wchar_t *wsAlphaPath = NULL, unsigned int unAlphaCheckSum = 0)
{
	Dict pImage = NULL;
	unsigned long nRet = OK;

	pImage = DictStreamNew( oMMgr, pXref, FALSE, wsTempFile, unImageCheckSum );
	if ( !pImage )
		return NULL;

	pImage->pHeader.nObjClass |= OSUBCLASS_XOBJECT;

	// Добавляем необходимые элементы
	pImage->nFilter = STREAM_FILTER_DCT_DECODE;
    nRet += DictAddName( pImage, "Type", "XObject");
    nRet += DictAddName( pImage, "Subtype", "Image");
	nRet += DictAddNumber( pImage, "Height", nHeight );
	nRet += DictAddNumber( pImage, "Width",  nWidth  );
	nRet += DictAddName( pImage, "ColorSpace", c_sColorRGB );
	nRet += DictAddNumber( pImage, "BitsPerComponent", 8 );

	if ( OK != nRet )
        return NULL;

	if ( bAlpha )
	{
		Dict pImageSMask = DictStreamNew( oMMgr, pXref, FALSE, wsAlphaPath, unAlphaCheckSum );
		if ( !pImageSMask )
			return NULL;

		pImageSMask->pHeader.nObjClass |= OSUBCLASS_XOBJECT;
		pImageSMask->nFilter = STREAM_FILTER_LZW_DECODE;
		nRet += DictAddName( pImageSMask, "Type", "XObject");
		nRet += DictAddName( pImageSMask, "Subtype", "Image");
		nRet += DictAddName( pImageSMask, "ColorSpace", c_sColorGray );
		nRet += DictAddNumber( pImageSMask, "Width", nWidth );
		nRet += DictAddNumber( pImageSMask, "Height", nHeight );
		nRet += DictAddNumber( pImageSMask, "BitsPerComponent", 8 );

		if ( OK != nRet )
			return NULL;

		if ( OK != DictAdd( pImage, "SMask", pImageSMask ) )
			return NULL;
#ifndef _DEBUG
		pImageSMask->nFilter |= STREAM_FILTER_FLATE_DECODE;
#endif
	}

	return pImage;
}
BOOL          ImageValidate           (ImageDict pImage)
{
    if ( !pImage )
        return FALSE;

    if ( pImage->pHeader.nObjClass != (OSUBCLASS_XOBJECT | OCLASS_DICT) ) 
	{
        RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE, 0);
        return FALSE;
    }

    Name pSubtype = (Name)DictGetItem( pImage, "Subtype", OCLASS_NAME );
    if ( !pSubtype || 0 != UtilsStrCmp( pSubtype->sValue, "Image" ) ) 
	{
        RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE, 0);
        return FALSE;
    }

    return TRUE;
}
TPoint        ImageGetSize            (ImageDict pImage)
{
    TPoint oRet = {0, 0};

    if ( !ImageValidate( pImage ) )
        return oRet;

    Number oWidth  = (Number)DictGetItem( pImage, "Width",  OCLASS_NUMBER );
    Number oHeight = (Number)DictGetItem( pImage, "Height", OCLASS_NUMBER );

    if (oWidth && oHeight) 
	{
		oRet.fX = oWidth->nValue;
		oRet.fY = oHeight->nValue;
    }

    return oRet;
}
unsigned long ImageGetSize2           (ImageDict pImage, TPoint *poSize)
{
	poSize->fX = 0;
	poSize->fY = 0;

    if ( !ImageValidate( pImage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;

    Number oWidth  = (Number)DictGetItem( pImage, "Width",  OCLASS_NUMBER );
    Number oHeight = (Number)DictGetItem( pImage, "Height", OCLASS_NUMBER );

    if (oWidth && oHeight) 
	{
		poSize->fX = oWidth->nValue;
		poSize->fY = oHeight->nValue;
    }

    return OK;
}
unsigned int  ImageGetBitsPerComponent(ImageDict pImage)
{
    if ( !ImageValidate( pImage ) )
        return 0;

    Number oNumber = (Number)DictGetItem( pImage, "BitsPerComponent", OCLASS_NUMBER);

    if (!oNumber)
        return 0;

	return oNumber->nValue;
}
const char*   ImageGetColorSpace      (ImageDict pImage)
{
    Name oName = (Name)DictGetItem( pImage, "ColorSpace", OCLASS_NAME);

    if ( !oName ) 
	{
        CheckError( pImage->oError );
        return NULL;
    }

    return oName->sValue;
}
EColorSpace   ImageGetColorSpace2     (ImageDict pImage)
{
	const char* sColorSpace = ImageGetColorSpace( pImage );

	if ( 0 == UtilsStrCmp( sColorSpace, c_sColorCMYK ) )
		return CSDeviceCMYK;
	else if ( 0 == UtilsStrCmp( sColorSpace, c_sColorRGB )  )
		return CSDeviceRGB;
	else if ( 0 == UtilsStrCmp( sColorSpace, c_sColorGray ) )
		return CSDeviceGray;

	RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE, 0 );
	return CSDeviceGray;
}
unsigned int  ImageGetWidth           (ImageDict pImage)
{
    return ImageGetSize( pImage ).fX;
}
unsigned int  ImageGetHeight          (ImageDict pImage)
{
    return ImageGetSize( pImage ).fY;
}
ImageDict     ImageGetSMask           (ImageDict pImage)
{
	ImageDict oSMask = (ImageDict)DictGetItem( pImage, "SMask", OCLASS_DICT );

	return oSMask;
}
unsigned long ImageSetMask            (ImageDict pImage, BOOL bMask)
{
    if ( !ImageValidate( pImage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;

    if ( bMask && 1 != ImageGetBitsPerComponent( pImage ) )
        return SetError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_BIT_PER_COMPONENT, 0);

    Boolean oImageMask = (Boolean)DictGetItem( pImage, "ImageMask", OCLASS_BOOLEAN);
    
	if ( !oImageMask ) 
	{
        unsigned long nRet = OK;
		oImageMask = BooleanNew( pImage->oMMgr, FALSE);
        if ( OK != ( nRet = DictAdd( pImage, "ImageMask", oImageMask ) ) )
            return nRet;
    }

	oImageMask->bValue = bMask;
    return OK;
}
unsigned long ImageSetMaskImage       (ImageDict pImage, ImageDict pMaskImage)
{
    if ( !ImageValidate( pImage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;

    if ( !ImageValidate( pMaskImage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;

    if ( OK != ImageSetMask( pMaskImage, TRUE ) )
        return CheckError( pImage->oError );

    return DictAdd( pImage, "Mask", pMaskImage );
}
unsigned long ImageSetColorMask       (ImageDict pImage, unsigned int nRMin, unsigned int nRMax, unsigned int nGMin, unsigned int nGMax, unsigned int nBMin, unsigned int nBMax)
{
    unsigned long nRet = OK;

    if ( ImageValidate( pImage ) )
        return AVS_OFFICEPDFWRITER_ERROR_INVALID_IMAGE;

    if ( DictGetItem( pImage, "ImageMask", OCLASS_BOOLEAN ) )
		return RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_OPERATION, 0);

    if ( 8 != ImageGetBitsPerComponent( pImage ) )
		return RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_BIT_PER_COMPONENT, 0);

    const char *sName = ImageGetColorSpace( pImage );
    if ( !sName || 0 != UtilsStrCmp( c_sColorRGB, sName ) )
		return RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_COLOR_SPACE, 0);

    // Любое число должно лежать в отрезке от 0 до 2^BitsPerComponent - 1 
    if ( nRMax > 255 || nGMax > 255 || nBMax > 255)
		return RaiseError( pImage->oError, AVS_OFFICEPDFWRITER_ERROR_INVALID_PARAMETER, 0);

	Array oArray = ArrayNew( pImage->oMMgr );
    if ( !oArray )
		return CheckError( pImage->oError );

    nRet += DictAdd( pImage, "Mask", oArray);
    nRet += ArrayAddNumber( oArray, nRMin);
    nRet += ArrayAddNumber( oArray, nRMax);
    nRet += ArrayAddNumber( oArray, nGMin);
    nRet += ArrayAddNumber( oArray, nGMax);
    nRet += ArrayAddNumber( oArray, nBMin);
    nRet += ArrayAddNumber( oArray, nBMax);

    if ( OK != nRet )
		return CheckError( pImage->oError );

    return OK;
}
#endif /* _IMAGE_H */

