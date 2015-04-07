#pragma once

// IViewSettings
[
	object,
	uuid("32945C3F-0EEF-4423-BABC-F33BDDC5383D"),
	dual,	helpstring("IViewSettings Interface"),
	pointer_default(unique)
]
__interface IViewSettings : IDispatch
{
	[id(10000)]											HRESULT Refresh ();
	
	[id(11000), propget]								HRESULT ZoomToWindow ( [out, retval] VARIANT_BOOL* Value );
	[id(11000), propput]								HRESULT ZoomToWindow ( [in] VARIANT_BOOL Value );
	[id(12000), propget]								HRESULT Zoom ( [out, retval] double* Value );
	[id(12000), propput]								HRESULT Zoom ( [in] double Value );

	[id(30000), helpstring("Set you view property")]	HRESULT SetPropertyN ( [in] long ID, [in] VARIANT Value );
	[id(40000), helpstring("Get you view property")]	HRESULT GetPropertyN ( [in] long ID, [out, retval] VARIANT* Value );
	[id(50000), helpstring("Set you view property")]	HRESULT SetProperty ( [in] BSTR Name, [in] VARIANT Value );
	[id(60000), helpstring("Get you view property")]	HRESULT GetProperty ( [in] BSTR Name, [out, retval] VARIANT* Value );
};