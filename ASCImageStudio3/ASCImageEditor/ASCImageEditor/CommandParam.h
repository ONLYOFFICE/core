// CommandParam.h : Declaration of the CCommandParam

#pragma once
#include "resource.h"       // main symbols
#include "comutil.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

// ICommandParam
[
	object,
	uuid("C0B26C7F-9340-4939-A9B4-7375775900C5"),
	dual,	helpstring("ICommandParam Interface"),
	pointer_default(unique)
]
__interface ICommandParam : IDispatch
{
	[id(100)] HRESULT GetName([out, retval] BSTR* pVal);
	[id(101)] HRESULT GetDescription([out, retval] BSTR* pVal);
	[propget, id(102), helpstring("property Value")] HRESULT Value([out, retval] VARIANT* pVal);
	[propput, id(102), helpstring("property Value")] HRESULT Value([in] VARIANT newVal);
	[id(103)] HRESULT GetMinValue([out, retval] VARIANT* pVal);
	[id(104)] HRESULT GetMaxValue([out, retval] VARIANT* pVal);
};

// _ICommandParamEvents
[
	dispinterface,
	uuid("3C1B9B88-7584-4b8e-8362-F0813446674E"),
	helpstring("_ICommandParamEvents Interface")
]
__interface _ICommandParamEvents
{
	[id(1), helpstring("method OnChange")]    HRESULT OnChange();
};



struct CommandParameter
{
	CString    m_name;
	CString    m_description;
	VARIANT    m_value;
	VARIANT    m_min_value;
	VARIANT    m_max_value;
};

// CCommandParam

[
	coclass,
	default(ICommandParam, _ICommandParamEvents),
	threading(apartment),
	event_source(com),
	vi_progid("AVSImageEditor.CommandParam"),
	progid("AVSImageEditor.CommandParam.1"),
	version(1.0),
	uuid("1F24AB89-C0A7-410B-9F27-4A38D268188B"),
	helpstring("CommandParam Class")
]
class ATL_NO_VTABLE CCommandParam :
	public ICommandParam
{
protected:
	int                          m_parameter_index;
	CommandParameter*            m_parameter_struct;
	IParameterChangeCallback*    m_callback; 
		
public:
	CCommandParam():m_parameter_index(-1),m_parameter_struct(NULL),m_callback(NULL)
	{
	}
	~CCommandParam()
	{
	}

	__event __interface _ICommandParamEvents;

	void   SetCommandParameterPtr(CommandParameter*   paramPtr, int paramIndex) 
	{
		m_parameter_struct = paramPtr;
		m_parameter_index = paramIndex;
	}

	void   SetCallback(IParameterChangeCallback* cllbck) 
	{
		m_callback = cllbck;
	}

	void   ChangeParameterEvent()
	{
		OnChange();
	}

public:


	STDMETHOD(GetName)( BSTR* pVal)
	{
		CString tmpS("Unknown name");
		if (m_parameter_struct)
			tmpS = m_parameter_struct->m_name;
		*pVal = tmpS.AllocSysString();
		return S_OK;
	}
	STDMETHOD(GetDescription)( BSTR* pVal)
	{
		CString tmpS("Unknown Desc");
		if (m_parameter_struct)
			tmpS = m_parameter_struct->m_description;
		*pVal = tmpS.AllocSysString();
		return S_OK;
	}
	STDMETHOD(get_Value)( VARIANT* pVal)
	{
		if (m_parameter_struct)
			*pVal = m_parameter_struct->m_value;
		else
		{
			VARIANT tempVar;
			VariantInit(&tempVar);
			*pVal = tempVar;
		}
		return S_OK;
	}
	STDMETHOD(put_Value)( VARIANT newVal)
	{
		if (m_parameter_struct)
		{
			m_parameter_struct->m_value = newVal;
			if (m_callback && m_parameter_index>-1)
				m_callback->OnChangeParameter(m_parameter_index);
		}
		return S_OK;
	}
	STDMETHOD(GetMinValue)( VARIANT* pVal)
	{
		if (m_parameter_struct)
			*pVal = m_parameter_struct->m_min_value;
		else
		{
			VARIANT tempVar;
			VariantInit(&tempVar);
			*pVal = tempVar;
		}
		return S_OK;
	}
	STDMETHOD(GetMaxValue)( VARIANT* pVal)
	{
		if (m_parameter_struct)
			*pVal = m_parameter_struct->m_max_value;
		else
		{
			VARIANT tempVar;
			VariantInit(&tempVar);
			*pVal = tempVar;
		}
		return S_OK;
	}
};

