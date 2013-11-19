#pragma once

#include "AVSUtils.h"
#include "FileTransform.h"

// IOfficePPTToPPTX
[
	object,
	uuid("20D985D5-7BBC-4074-B3C6-DF671DD28016"),
	dual, 
	pointer_default(unique)
]

__interface IOfficePPTToPPTX :  IDispatch
{	
	[id(100)] HRESULT TransformFile([in] BSTR bstrPPT, [in] BSTR bstrPPTX, [in] BSTR bstrOptions);

	[id(800)] HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(900)] HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// IOfficePPTToPPTXEvents
/*
[
	dispinterface,	
	uuid("432BA54D-2937-4ff8-A7A6-B7F969C19BF0")
]
*/

__interface IOfficePPTToPPTXEvents
{
	[id(1)] HRESULT Process([in] LONG Percent);
};

// COfficePPTToPPTX
[
	coclass, 
	uuid("D1DA3069-9691-48a1-9C3F-D8FE143BCE57"),
	//event_source(com), 
	threading(apartment),
	vi_progid("AVSOfficePPTFile.OfficePPTToPPTX"),
	progid("AVSOfficePPTFile.OfficePPTToPPTX.1"),
	version(1.0)
]

class ATL_NO_VTABLE COfficePPTToPPTX : public IOfficePPTToPPTX
{
public:

	COfficePPTToPPTX () : m_hSynchMutex(NULL)
	{
	}

	//__event __interface IOfficePPTToPPTXEvents;

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{	
		m_hSynchMutex	=	CreateMutex(NULL, FALSE, NULL);

		return S_OK;
	}

	void FinalRelease()
	{
		RELEASEHANDLE(m_hSynchMutex);
	}

public:

	STDMETHOD(TransformFile)(BSTR bstrPPT, BSTR bstrPPTX, BSTR bstrOptions)
	{
		CSynchAccess oSynchAccess(m_hSynchMutex);

		if (SUCCEEDED(m_oTransformer.LoadFromFile(bstrPPT, bstrOptions)))
			return m_oTransformer.SaveToFile(bstrPPTX, bstrOptions);

		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR bstrName, VARIANT parValue)
	{

		return S_OK;
	}

	STDMETHOD(GetAdditionalParam)(BSTR bstrName, VARIANT* parValue)
	{

		return S_OK;
	}

private:

	HANDLE			m_hSynchMutex;
	CFileTransform	m_oTransformer;
};
