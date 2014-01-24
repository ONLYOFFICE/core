

[ object, uuid("63848500-ab65-11df-94e2-0800200c9a66"), dual, pointer_default(unique)]
__interface IAVSOfficeMenu
{
	[id(10)] HRESULT Open( [in] BSTR XMLString );
	[id(20)] HRESULT FireEvent( [in] BSTR XMLString );
};
[
	dispinterface,
	uuid("28780cc0-ab6f-11df-94e2-0800200c9a66"),
	helpstring("_IAVSOfficeFileTemplateEvents Interface")
]
__interface _IAVSOfficeMenuEvents
{
	[id(0)] HRESULT Fire([in] BSTR XMLString);
};

[coclass, uuid("402b3fe0-ab6f-11df-94e2-0800200c9a66"), threading(apartment), event_source(com), vi_progid("AVSUniversalConverter.AVSXMLSingle"), progid("AVSUniversalConverter.AVSXMLSingle.1"), version(1.0)]
class ATL_NO_VTABLE CAVSOfficeMenu 
	: public IAVSOfficeMenu
{
public:
	__event __interface _IAVSOfficeMenuEvents;
public:
	CAVSOfficeMenu()
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
public:
	STDMETHOD(Open)( BSTR XMLString )
	{
		return S_OK;
	}
	STDMETHOD(FireEvent)( BSTR XMLString )
	{
		Fire( XMLString );
		return S_OK;
	}
};