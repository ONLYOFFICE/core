#pragma once

//------------------------------------------------------------------------------------------------------
// IOfficeOCR
//------------------------------------------------------------------------------------------------------
// ѕолучает изображение, отдает текст ( если он есть ), который находитс€ на изображении
//------------------------------------------------------------------------------------------------------

[ object, uuid("804C1FF6-7020-477E-AD9F-980E9201C237"), dual, pointer_default(unique) ]
__interface IOfficeOCR : IDispatch
{
	[id(1)]			HRESULT Recognize([in] IUnknown *Image, [out, retval] BSTR* Text);					// ѕолучает изображение, возвращает текст

	[id(2), propput]HRESULT OutputFormat([in] LONG Type);												// ¬ыставл€ет формат выходного текста ( по умолчанию XML )
	[id(2), propget]HRESULT OutputFormat([ out, retval ] LONG *Type);									// ¬озвращает формат выходного текста ( по умолчанию XML )

	[id(3), propput]HRESULT GrayLevel([in] LONG Type);													// ¬ыставл€ет уровень серого ( по умолчанию 0 - автоматически )
	[id(3), propget]HRESULT GrayLevel([ out, retval ] LONG *Type);										// ¬озвращает уровень серого ( по умолчанию 0 - автоматически )

	[id(4), propput]HRESULT DustSize([in] LONG Type);													// ¬ыставл€ет размер шума ( по умолчанию -1 - автоматически )
	[id(4), propget]HRESULT DustSize([ out, retval ] LONG *Type);										// ¬озвращает размер шума ( по умолчанию -1 - автоматически )

	[id(5), propput]HRESULT SpaceWidthDots([in] LONG Type);												// ¬ыставл€ет ширину пробелов/точек ( по умолчанию 0 - автоматически )
	[id(5), propget]HRESULT SpaceWidthDots([ out, retval ] LONG *Type);									// ¬озвращает ширину пробелов/точек ( по умолчанию 0 - автоматически )

	[id(6), propput]HRESULT Certainty([in] LONG Type);													// ¬ыставл€ет процент достоверности объекта ( по умолчанию 95 )
	[id(6), propget]HRESULT Certainty([ out, retval ] LONG *Type);										// ¬озвращает процент достоверности объекта ( по умолчанию 95 )

	[id(100000)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);			// ”становка дополнительных параметров
	[id(100001)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);	// ѕолучение дополнительных параметров
};