// Txt.h : Declaration of the CTxt

#pragma once
#include "resource.h"       // main symbols
#include <comsvcs.h>


// ITxt
[
	object,
	uuid("15FE2E8B-1809-4A95-9F17-086356BCDB6E"),
	dual,	helpstring("ITxt Interface"),
	pointer_default(unique)
]
__interface ITxt : IDispatch
{
};


// CTxt

[
	coclass,
	default(ITxt),
	threading(single),
	aggregatable(never),
	vi_progid("AVSOdtFile.Txt"),
	progid("AVSOdtFile.Txt.1"),
	version(1.0),
	uuid("63CA47BB-7D3B-489D-A412-9886783D41C5"),
	helpstring("Txt Class")
]
class ATL_NO_VTABLE CTxt :
	public ITxt
{
public:
	CTxt()
	{
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}



// ITxt
public:
};

