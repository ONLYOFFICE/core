#pragma once

// IAnimationLayer
[
	object,
	uuid("B865893C-5ED9-49D7-BEED-2B85D1496B1C"),
	dual,	helpstring("IAnimationLayer Interface"),
	pointer_default(unique)
]

__interface IAnimationLayer : IDispatch
{
	[id(10000)] HRESULT LoadFromXml([in] BSTR Xml);
	[id(20201)] HRESULT AddSubLayer([in]IUnknown* pUnk);

	[id(20501), propget] HRESULT BeginTime([out, retval] double* dTime);
	[id(20501), propput] HRESULT BeginTime([in] double dSize);
	
	[id(20502), propget] HRESULT EndTime([out, retval] double* dTime);
	[id(20502), propput] HRESULT EndTime([in] double dTime);
	
	[id(20600), propget] HRESULT Duration([out, retval] double* dTime);

	[id(30100)] HRESULT Draw([in]IAVSRenderer* pRenderer,[in]double dTime);
	[id(30200)] HRESULT DrawOnMediaData([in]IUnknown* punkFrame,[in]double dTime);

	[id(40100), propget] HRESULT IsStatic([out, retval] VARIANT_BOOL* bStatic);
	[id(40200)] HRESULT ClearCache();

	[id(50201)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT  ParamValue);
	[id(50202)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out]VARIANT* ParamValue);
};
