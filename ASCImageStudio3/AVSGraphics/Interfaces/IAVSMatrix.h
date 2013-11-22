#pragma once

const long c_ag_MatrixOrderAppend			= 0;
const long c_ag_MatrixOrderPrepend			= 1;

#include "IAVSGraphicsBase.h"

[object, uuid("5DB462B5-941B-46e7-B80F-9C9B38FF4BE6"), dual, pointer_default(unique)]
__interface IAVSMatrix : IAVSGraphicsBase
{
	[id(100)] HRESULT LoadFromXml([in] BSTR bstrXML);

	[id(101)] HRESULT SetElements([in]	double  m11, [in]  double  m12, [in]  double  m21, [in]  double  m22, [in]  double  dx, [in]  double  dy);
	[id(102)] HRESULT GetElements([out] double* m11, [out] double* m12, [out] double* m21, [out] double* m22, [out] double* dx, [out] double* dy);

	[id(201)] HRESULT Multiply([in] IAVSMatrix* pMatrix, [in] LONG lMatrixOrder);
	
	[id(211)] HRESULT Rotate([in] double dAngle, [in] LONG lMatrixOrder);
	[id(212)] HRESULT RotateAt([in] double dAngle, [in] double dPointX, [in] double dPointY, [in] LONG lMatrixOrder);

	[id(221)] HRESULT Scale([in] double dScaleX, [in] double dScaleY, [in] LONG lMatrixOrder);

	[id(231)] HRESULT Translate([in] double dOffsetX, [in] double dOffsetY, [in] LONG lMatrixOrder);

	[id(301)] HRESULT Transpose();
	[id(302)] HRESULT Invert();
	[id(303)] HRESULT Reset();
};