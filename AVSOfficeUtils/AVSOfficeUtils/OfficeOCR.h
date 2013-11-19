#pragma once
#include "resource.h"
#include "IOfficeOCR.h"
#include "../../../AVSVideoStudio3/Common/AVSATLError.h"

#define OUTPUT_FORMAT_TYPE_XML	0
#define OUTPUT_FORMAT_TYPE_TEXT	1

[ coclass, uuid("5E4BC6BB-26B4-4237-894E-2C872842A8EE"), threading(apartment), vi_progid("AVSOfficeUtils.OfficeOCR"), progid("AVSOfficeUtils.OfficeOCR.1"), version(1.0), support_error_info(IOfficeOCR) ]
class ATL_NO_VTABLE COfficeOCR : public IOfficeOCR, public CAVSATLError
{
public :

	COfficeOCR();

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();
	void FinalRelease();

public :

	STDMETHOD ( Recognize )( IUnknown *Image, BSTR* Text );					// Получает изображение, возвращает текст

	STDMETHOD ( put_OutputFormat )( LONG Type );							// Выставляет формат выходного текста ( по умолчанию XML )
	STDMETHOD ( get_OutputFormat )( LONG *Type );							// Возвращает формат выходного текста ( по умолчанию XML )

	STDMETHOD ( put_GrayLevel )( LONG Type );								// Выставляет уровень серого ( по умолчанию 0 - автоматически )
	STDMETHOD ( get_GrayLevel )( LONG *Type );								// Возвращает уровень серого ( по умолчанию 0 - автоматически )

	STDMETHOD ( put_DustSize )( LONG Type );								// Выставляет размер шума ( по умолчанию -1 - автоматически )
	STDMETHOD ( get_DustSize )( LONG *Type );								// Возвращает размер шума ( по умолчанию -1 - автоматически )

	STDMETHOD ( put_SpaceWidthDots )( LONG Type );							// Выставляет ширину пробелов/точек ( по умолчанию 0 - автоматически )
	STDMETHOD ( get_SpaceWidthDots )( LONG *Type );							// Возвращает ширину пробелов/точек ( по умолчанию 0 - автоматически )

	STDMETHOD ( put_Certainty )( LONG Type );								// Выставляет процент достоверности объекта ( по умолчанию 95 )
	STDMETHOD ( get_Certainty )( LONG *Type );								// Возвращает процент достоверности объекта ( по умолчанию 95 )

	STDMETHOD ( SetAdditionalParam )( BSTR ParamName, VARIANT ParamValue );	// Установка дополнительных параметров
	STDMETHOD ( GetAdditionalParam )( BSTR ParamName, VARIANT *ParamValue );// Получение дополнительных параметров

private :

	LONG m_lOutputFormatType;												// Формат выходного текста ( по умолчанию XML )
	LONG m_lGrayLevel;														// Уровень серого ( по умолчанию 0 - автоматически )
	LONG m_lDustSize;														// Размер шума ( по умолчанию -1 - автоматически )
	LONG m_lSpaceWidthDots;													// Ширина пробелов/точек ( по умолчанию 0 - автоматически )
	LONG m_lCertainty;														// Процент достоверности объекта ( по умолчанию 95 )
};