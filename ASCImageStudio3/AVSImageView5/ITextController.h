#pragma once

//---------------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------------

[ object, uuid("DA00DAC7-63F5-4572-BC45-0F8A8BB031DC"), dual, pointer_default(unique) ]
__interface ITextController : public IDispatch
{
	[id(0x2000)]		HRESULT		BeginEdit	( [out, retval] VARIANT_BOOL* STATUS );
	[id(0x2001)]		HRESULT		EndEdit		( [out, retval] VARIANT_BOOL* STATUS );
	
	[id(0x5000)]		HRESULT		Command 	( [in] LONG COMMAND, [in] VARIANT VALUE, [out] VARIANT* OUT, [out, retval] VARIANT_BOOL* STATUS );

	[id(0x6000)]		HRESULT		SetParam 	( [in] BSTR NAME, [in] VARIANT SET );
	[id(0x6001)]		HRESULT		GetParam 	( [in] BSTR NAME, [out, retval] VARIANT* GET );
};

[ dispinterface, uuid("872B8BE7-B129-4473-A503-07C19A18E4B7") ]
__interface _ITextControllerEvents
{
	[id(10000)] HRESULT Event ( LONG ID, LONG VALUE );
};