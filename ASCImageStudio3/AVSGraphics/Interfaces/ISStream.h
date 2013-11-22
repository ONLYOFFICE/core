#pragma once

// IStream
[
	object,
	uuid("578D1CDC-286B-4bca-A2C9-EBB35393270D"),
	dual,	helpstring("IStream Interface"),
	pointer_default(unique)
]

__interface ISStream : IDispatch
{
	[id(10000)] HRESULT LoadFromXml([in] BSTR Xml);

	[id(12000)] HRESULT GetFrame([out]IUnknown** ppUnk);
	[id(13000)] HRESULT Seek([in] double nVal);
	
	[id(50201)]	HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT  ParamValue);
	[id(50202)]	HRESULT GetAdditionalParam([in] BSTR ParamName, [out]VARIANT* ParamValue);
};
