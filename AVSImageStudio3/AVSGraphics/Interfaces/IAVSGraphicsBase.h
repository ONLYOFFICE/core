#pragma once
#include "..\stdafx.h"

const double c_ag_Inch_to_MM					= 25.4;
const double c_ag_Inch_to_Point					= 72;

const long c_ag_CombineModeReplace				= 0;
const long c_ag_CombineModeIntersect			= 1;
const long c_ag_CombineModeUnion				= 2;
const long c_ag_CombineModeXor					= 3;
const long c_ag_CombineModeExclude				= 4;
const long c_ag_CombineModeComplement			= 5;

const long c_ag_PageUnitPixel					= 0;
const long c_ag_PageUnitPoint					= 1;
const long c_ag_PageUnitInch					= 2;
const long c_ag_PageUnitMillimeter				= 3;

[object, uuid("01CD8265-7CF2-4af6-819D-73B27542D8CA"), dual, pointer_default(unique)]
__interface IAVSGraphicsBase : IDispatch
{
	[id(1001)]			HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]			HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);

	[id(1003)]			HRESULT CreateDublicate([out, retval] IAVSGraphicsBase** ppGraphicsBase);
};