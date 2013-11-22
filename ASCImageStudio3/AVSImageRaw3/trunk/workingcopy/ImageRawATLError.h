/*!	
 *	\file	ImageRawATLError.h
 *	\brief	Содержит класс для генераций ErrorDescription
 *
 *	\author	Nikolay.Pogorskiy@avsmedia.net
 */

#pragma once
#include <windows.h>
#include "ImageRawErrorDescription.h"
#include "atlcom.h"

/*!
 *	\brief	Вспомогательный макрос для заполнения полей данных
 */
#define _IMAGERAW_SETERRORVAR(ErrorCode) do { hr = E_IMAGERAW(ErrorCode); GetStringErrorDesc(ErrorCode,&strErrorDesc);}while(0);

/*!
 *	\brief	Вспомогательный макрос для более компактной записи конструкции switch
 */
#define _IMAGERAW_ERRORCASE(ErrorCode) \
	case ErrorCode: \
		 _IMAGERAW_SETERRORVAR(ErrorCode) \
		break;


/*! \class		CImageRawATLError ImageRawATLError.h "ImageRawATLError.h"
 *	\brief		Класс содержит метод, возращающий HRESULT и, при необходимости, устанавливающий значения для интерфейса IErrorInfo
 *	\tparam T	Шаблонный параметр — кокласс, для которого реализуем описание ошибок
 */
template<class T>
class CImageRawATLError
{
public:
	/*!
	 *	\brief	Установить значения для интерфейса IErrorInfo и вернуть HRESULT
	 *
	 *	\param[in]	hrCode	Код ошибки
	 *	\param[in]	iid		IID интерфейса, для которого определена ошибка
	 */
	HRESULT Error(HRESULT hrCode,IID iid)
	{
		HRESULT hr;
		BSTR	strErrorDesc;

		if (FAILED(hrCode) && (hrCode > E_IMAGERAW_BASE) && (hrCode & ((unsigned long)(FACILITY_ITF)<<16)))
		{
			LONG Code = hrCode - E_IMAGERAW_BASE;
			switch(Code)
			{
			case NOERROR:
				return S_OK;
				break;
			
			_IMAGERAW_ERRORCASE(IMAGERAW_UNEXPECTED_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_OPENFILE_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_DCRAWINTERNALEXCEPTION_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_UNSUPPORTERDFORMAT_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_DECODE_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_NOTHUMBNAIL_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_UNSUPPORTEDTHUMBFORMAT_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_READING_NEFCURVE_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_NOCAMWB_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_DCRAW_OUTOFMEMORY_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_FILE_NOT_OPENED_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_LOAD_THUMBNAIL_ERROR)
			_IMAGERAW_ERRORCASE(IMAGERAW_DECODE_THUMBNAIL_ERROR)


			default:
				_IMAGERAW_SETERRORVAR(IMAGERAW_UNEXPECTED_ERROR);
				break;
			}
		}
		else
			return hrCode;

		// Sets up the IErrorInfo interface to provide error information to clients of the object
		AtlReportError(__uuidof(T), strErrorDesc, iid);
		::SysFreeString(strErrorDesc);
		return hr;
	}
};
