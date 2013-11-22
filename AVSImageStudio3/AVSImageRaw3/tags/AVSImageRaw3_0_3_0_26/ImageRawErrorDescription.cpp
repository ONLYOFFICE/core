/*!	
 *	\file	ImageRawErrorDescription.cpp
 *	\brief	Строковые описания ошибок компоненты AVSImageRaw
 *			и функция их получения по кодам ошибок
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */


#include "stdafx.h"
#include "comutil.h"
#include "ImageRawErrorDescription.h"

/*
 *	Строковое описание ошибок
 *
 *	Переменную cо строковым описанием можно получить с использованием макроса E_IMAGERAW_DESC(ErrorCode)
 */

#define __L(S)	OLESTR(S)
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_UNEXPECTED_ERROR)				= __L("Unexpected error");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_OPENFILE_ERROR)				= __L("Open file error");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR)	= __L("Dcraw internal exception");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_UNSUPPORTERDFORMAT_ERROR)		= __L("Unsupported file format");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_DECODE_ERROR)					= __L("Cannot decode file");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_NOTHUMBNAIL_ERROR)				= __L("Image has no thumbnail");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR)	= __L("Unsupported thumb format");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_READING_NEFCURVE_ERROR)		= __L("Error reading NEF curve");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_NOCAMWB_ERROR)					= __L("Cannot use camera white balance");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR)		= __L("Internal memory operation error");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_FILE_NOT_OPENED_ERROR)			= __L("File not opened");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_LOAD_THUMBNAIL_ERROR)			= __L("Cannot load thumbnail");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_DECODE_THUMBNAIL_ERROR)		= __L("Cannot decode thumbnail image");
ERRORDESC_STR_	E_IMAGERAW_DESC(IMAGERAW_MEMORY_ERROR)					= __L("Memory manipulation error");


HRESULT GetStringErrorDesc(LONG lErrorCode,BSTR * bsErrorDesc)
{
	if (!bsErrorDesc)
		return E_POINTER;

	switch(lErrorCode)
	{
		case IMAGERAW_UNEXPECTED_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_UNEXPECTED_ERROR)).copy(); break;
		
		case IMAGERAW_OPENFILE_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_OPENFILE_ERROR)).copy(); break;
		
		case IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR)).copy(); break;
		
		case IMAGERAW_UNSUPPORTERDFORMAT_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_UNSUPPORTERDFORMAT_ERROR)).copy(); break;
		
		case IMAGERAW_DECODE_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_DECODE_ERROR)).copy(); break;

		case IMAGERAW_NOTHUMBNAIL_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_NOTHUMBNAIL_ERROR)).copy(); break;

		case IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR)).copy(); break;

		case IMAGERAW_READING_NEFCURVE_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_READING_NEFCURVE_ERROR)).copy(); break;

		case IMAGERAW_NOCAMWB_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_NOCAMWB_ERROR)).copy(); break;

		case IMAGERAW_DCRAW_OUTOFMEMORY_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR)).copy(); break;

		case IMAGERAW_FILE_NOT_OPENED_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_FILE_NOT_OPENED_ERROR)).copy(); break;

		case IMAGERAW_LOAD_THUMBNAIL_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_LOAD_THUMBNAIL_ERROR)).copy(); break;

		case IMAGERAW_DECODE_THUMBNAIL_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_DECODE_THUMBNAIL_ERROR)).copy(); break;

		case IMAGERAW_MEMORY_ERROR: 
			*bsErrorDesc = _bstr_t(E_IMAGERAW_DESC(IMAGERAW_MEMORY_ERROR)).copy(); break;


		default:
			*bsErrorDesc = NULL;
			return E_INVALIDARG;
	};
	return S_OK;
}


#undef __L

