#pragma once
#include "IAVSMatrix.h"

const long c_ag_CompositingModeSourceOver				= 0;
const long c_ag_CompositingModeSourceCopy				= 1;

const long c_ag_CompositingQualityInvalid				= -1;
const long c_ag_CompositingQualityDefault				= 0;
const long c_ag_CompositingQualityHighSpeed				= 1;
const long c_ag_CompositingQualityHighQuality			= 2;
const long c_ag_CompositingQualityGammaCorrected		= 3;
const long c_ag_CompositingQualityAssumeLinear			= 4;

const long c_ag_InterpolationModeInvalid				= -1;
const long c_ag_InterpolationModeDefault				= 0;
const long c_ag_InterpolationModeLow					= 1;
const long c_ag_InterpolationModeHigh					= 2;
const long c_ag_InterpolationModeBilinear				= 3;
const long c_ag_InterpolationModeBicubic				= 4;
const long c_ag_InterpolationModeNearestNeighbor		= 5;
const long c_ag_InterpolationModeHighQualityBilinear	= 6;
const long c_ag_InterpolationModeHighQualityBicubic		= 7;

const long c_ag_PixelOffsetModeInvalid					= -1;
const long c_ag_PixelOffsetModeDefault					= 0;
const long c_ag_PixelOffsetModeHighSpeed				= 1;
const long c_ag_PixelOffsetModeHighQuality				= 2;
const long c_ag_PixelOffsetModeNone						= 3;
const long c_ag_PixelOffsetModeHalf						= 4;

const long c_ag_SmoothingModeInvalid					= -1;
const long c_ag_SmoothingModeDefault					= 0;
const long c_ag_SmoothingModeHighSpeed					= 1;
const long c_ag_SmoothingModeHighQuality				= 2;
const long c_ag_SmoothingModeNone						= 3;
const long c_ag_SmoothingModeAntiAlias					= 4;

const long c_ag_TextRenderingHintSystemDefault					= 0;
const long c_ag_TextRenderingHintSingleBitPerPixelGridFit		= 1;
const long c_ag_TextRenderingHintSingleBitPerPixel				= 2;
const long c_ag_TextRenderingHintAntiAliasGridFit				= 3;
const long c_ag_TextRenderingHintAntiAlias						= 4;
const long c_ag_TextRenderingHintClearTypeGridFit				= 5;

[object, uuid("49427C80-016B-48bc-ADA1-16A887AB6629"), dual, pointer_default(unique)]
__interface IAVSGraphicsState : IAVSGraphicsBase
{
	[id(001)]			HRESULT LoadFromXml([in] BSTR bstrXML);
	
	// general ------------------------------------------------------------
	[id(100), propget]	HRESULT BaseMatrix([out, retval] IAVSMatrix** ppMatrix);
	[id(100), propput]	HRESULT BaseMatrix([in] IAVSMatrix* pMatrix);

	[id(101), propget]	HRESULT Matrix([out, retval] IAVSMatrix** ppMatrix);
	[id(101), propput]	HRESULT Matrix([in] IAVSMatrix* pMatrix);

	[id(201), propget]	HRESULT DpiX([out, retval] double* dDpiX);
	[id(201), propput]	HRESULT DpiX([in] double dDpiX);

	[id(202), propget]	HRESULT DpiY([out, retval] double* dDpiY);
	[id(202), propput]	HRESULT DpiY([in]  double dDpiY);

	[id(301), propget]	HRESULT SmoothingMode([out, retval] LONG* lMode);
	[id(301), propput]	HRESULT SmoothingMode([in] LONG lMode);

	[id(302), propget]	HRESULT TextRenderingHint([out, retval] LONG* lMode);
	[id(302), propput]	HRESULT TextRenderingHint([in] LONG lMode);

	[id(303), propget]	HRESULT CompositingMode([out, retval] LONG* lMode);
	[id(303), propput]	HRESULT CompositingMode([in] LONG lMode);

	[id(304), propget]	HRESULT CompositingQuality([out, retval] LONG* lMode);
	[id(304), propput]	HRESULT CompositingQuality([in] LONG lMode);

	[id(305), propget]	HRESULT InterpolationMode([out, retval] LONG* lMode);
	[id(305), propput]	HRESULT InterpolationMode([in] LONG lMode);

	[id(306), propget]	HRESULT PixelOffsetMode([out, retval] LONG* lMode);
	[id(306), propput]	HRESULT PixelOffsetMode([in] LONG lMode);
};