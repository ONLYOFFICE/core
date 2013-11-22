#pragma once

//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------

[ object, uuid("97A181B3-AE3F-4ca1-8962-8BB0812F31E9"), dual, pointer_default(unique) ]
__interface ITrackController : public IDispatch
{
	[id(1000), propget]	HRESULT	PointsCount	( [out, retval] LONG* pValue );
	[id(2000), propget]	HRESULT	PointX		( [in] LONG Index, [out, retval] DOUBLE* pValue );
	[id(2000), propput]	HRESULT	PointX		( [in] LONG Index, [in] DOUBLE Value );
	[id(3000), propget]	HRESULT	PointY		( [in] LONG Index, [out, retval] DOUBLE* pValue );
	[id(3000), propput]	HRESULT	PointY		( [in] LONG Index, [in] DOUBLE Value );

	[id(99000)]	HRESULT	SetAdditionalParam 	( [in] BSTR ParamName, [in] VARIANT ParamValue );
	[id(99900)] HRESULT	GetAdditionalParam 	( [in] BSTR ParamName, [out, retval] VARIANT* ParamValue );
};