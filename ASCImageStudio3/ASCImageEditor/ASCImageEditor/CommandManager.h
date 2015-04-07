#pragma once
#include "resource.h"       // main symbols
#include "comutil.h"

#include <atlcoll.h>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include "CommandParam.h"

#include <math.h>

// ICommand
[
	object,
	uuid("8637D1E2-FCA3-4404-8FBE-E4B461019C2B"),
	dual,	helpstring("ICommand Interface"),
	pointer_default(unique)
]
__interface ICommand : IDispatch
{
	[id(100), helpstring("method GetParamsCount")] HRESULT GetParamsCount([out,retval] LONG* nCount);
	[id(101), helpstring("method GetParam")] HRESULT GetParam([in] LONG nParam, [out,retval] IUnknown** Param);
	[id(102), helpstring("method StartChangingParamsGroup")] HRESULT StartChangingParamsGroup();
	[id(103), helpstring("method FinishChangingParamsGroup")] HRESULT FinishChangingParamsGroup();
};


// _ICommandEvents
[
	dispinterface,
	uuid("9C7629CB-3E7C-41FE-ABD0-2E6C73213113"),
	helpstring("_ICommandEvents Interface")
]
__interface _ICommandEvents
{
	[id(1), helpstring("method OnProgress")] HRESULT OnProgress([in] LONG nProgress);
	[id(2), helpstring("method OnFinish")]   HRESULT OnFinish([in] VARIANT_BOOL bSucceesed);
	[id(3), helpstring("method NeedRestart")] HRESULT NeedRestart([in] VARIANT_BOOL goodStop);
	[id(4)]                                   HRESULT OnSwitchViewToSlow();
	[id(5)]                                   HRESULT OnSwitchViewToFast();
};

class CCommand;
// CCommandDescriptor

[
	coclass,
	default(ICommand, _ICommandEvents),
	threading(apartment),
	support_error_info("ICommand"),
	event_source(com),
	vi_progid("AVSImageEditor.Command"),
	progid("AVSImageEditor.Command.1"),
	version(1.0),
	uuid("E395F4F3-5670-453F-862E-54296C38F9C9"),
	helpstring("CommandDescriptor Class")
]
class ATL_NO_VTABLE CCommandDescriptor :
	public ICommand
{
public:
	CCommandDescriptor():m_commandPtr(NULL)
	{
	}

	__event __interface _ICommandEvents;

	void   SetCommandPtr(CCommand* cmndPtr)   {m_commandPtr = cmndPtr;}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(GetParamsCount)(LONG* nCount);
	STDMETHOD(GetParam)(LONG nParam, IUnknown** Param);
	STDMETHOD(StartChangingParamsGroup)();
	STDMETHOD(FinishChangingParamsGroup)();
private:
	CCommand*   m_commandPtr;
};

#define    INVALID_GRAPHICS     0x0001
#define    INVALID_LAYER        0x0002

class CCommand : public IParameterChangeCallback, public ITimerHandler
{
public:
	CCommand(IAppManager* app_manager):m_app_manager(app_manager)
			 ,m_invalidate_flag(INVALID_GRAPHICS), m_changes_after_finish(true)
	{
		if (!app_manager)
			throw "Failed";

		if (SUCCEEDED(m_command_desriptor.CoCreateInstance(__uuidof(CCommandDescriptor), NULL, CLSCTX_INPROC)))
		{
			CCommandDescriptor*  pComDesc = static_cast<CCommandDescriptor*>((void*)m_command_desriptor);
			if(pComDesc)
				pComDesc->SetCommandPtr(this);
		}

	}
	virtual ~CCommand() 
	{
		for (size_t i=0;i<m_params.GetCount();i++)
		{
			delete m_params[i];
		}
		m_params.RemoveAll();
		m_params_IDisp.RemoveAll();
	}
public:

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect) {  }
	virtual bool DrawOnLayer() { return false; }
	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	{	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	{	}
	virtual void OnMouseMove(LONG nX, LONG nY, LONG nButton)	{	}
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam)	{	}
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam)	{	}
	virtual void OnMouseLeave()	{	}
	virtual void OnTimer(void)	{	}
	virtual void Undo(int stepsCount)	{	}
	virtual void Redo(int stepsCount)	{	}
	virtual void StartChangingParamsGroup() {  }
	virtual void FinishChangingParamsGroup() {  }	
	virtual void SetAllParamsFromXML(VARIANT vXml)	{	}
	
	void GetCommandManagerIDisp(ICommand** pVal)	
	{
		if (m_command_desriptor)
			((ICommand*)m_command_desriptor)->AddRef();
		*pVal = m_command_desriptor;	
	}

	long GetParamsCount() const  
	{
		return (int)(m_params_IDisp.GetCount());
	}
	ICommandParam* GetParam(long indx)
	{
		if (indx<0 || indx>=(long)m_params_IDisp.GetCount())
			return NULL;
		if (m_params_IDisp[indx])
			((ICommandParam*)m_params_IDisp[indx])->AddRef();
		return m_params_IDisp[indx];
	}

	
	virtual const bool IsInvalidGraphics() const 
	{
		return (m_invalidate_flag & INVALID_GRAPHICS)?true:false;
	}
	virtual const bool IsInvalidLayer() const  
	{
		return (m_invalidate_flag & INVALID_LAYER)?true:false;
	};

	
	void RestartEvent(bool goodRestart) 
	{
		if (goodRestart) 
			NeedRestartEvent(VARIANT_TRUE); 
		else  
			NeedRestartEvent(VARIANT_FALSE);
	}

	void SwitchViewToSlow() 
	{
		CCommandDescriptor*  pComDesc = static_cast<CCommandDescriptor*>((void*)m_command_desriptor);
		if(pComDesc)
			pComDesc->OnSwitchViewToSlow();
	}
	void SwitchViewToFast() 
	{
		CCommandDescriptor*  pComDesc = static_cast<CCommandDescriptor*>((void*)m_command_desriptor);
		if(pComDesc)
			pComDesc->OnSwitchViewToFast();
	}
protected:

	virtual void OnChangeParameter(int paramInd) {}

	CAtlArray<CommandParameter*>    m_params;

	void SetInvalidateFlag(UINT invFlag, bool updateData = true)
	{
		m_invalidate_flag = invFlag;
		m_app_manager->GetView()->InvalidateView(updateData);
	};

	void NeedRestartEvent(VARIANT_BOOL goodStop)
	{
		CCommandDescriptor*  pComDesc = static_cast<CCommandDescriptor*>((void*)m_command_desriptor);
		if(pComDesc)
			pComDesc->NeedRestart(goodStop);
	}

	IAppManager* GetAppManager() 
	{
		return m_app_manager;
	};


	/************************************************************************/
	/* PARAMETERS                                                           */
	/************************************************************************/
	void  AddParameter(const CString& paramName, const CString& paramDesc, int paramVal, int paramMin, int paramMax)  
   {
	   CommandParameter*  tmpParam = new CommandParameter(); 
		tmpParam->m_name = paramName; 
		tmpParam->m_description = paramDesc; 
		VariantInit(&tmpParam->m_value); 
		tmpParam->m_value.vt = VT_I4;
		tmpParam->m_value.lVal = paramVal;
		VariantInit(&tmpParam->m_min_value); 
		tmpParam->m_min_value.vt = VT_I4;
		tmpParam->m_min_value.lVal = paramMin;
		VariantInit(&tmpParam->m_max_value); 
		tmpParam->m_max_value.vt = VT_I4;
		tmpParam->m_max_value.lVal = paramMax;
		AddCommandParameter(tmpParam);
   }


	void  AddParameter(const CString& paramName, const CString& paramDesc, float paramVal, float paramMin, float paramMax)  
   {
	   CommandParameter*  tmpParam = new CommandParameter(); 
		tmpParam->m_name = paramName; 
		tmpParam->m_description = paramDesc; 
		VariantInit(&tmpParam->m_value); 
		tmpParam->m_value.vt = VT_R8;
		tmpParam->m_value.dblVal = paramVal;
		VariantInit(&tmpParam->m_min_value); 
		tmpParam->m_min_value.vt = VT_R8;
		tmpParam->m_min_value.dblVal = paramMin;
		VariantInit(&tmpParam->m_max_value); 
		tmpParam->m_max_value.vt = VT_R8;
		tmpParam->m_max_value.dblVal = paramMax;
		AddCommandParameter(tmpParam);
   }

	void  AddParameter(const CString& paramName, const CString& paramDesc, const CString& paramVal)			 
   {
		CommandParameter*  tmpParam = new CommandParameter(); 
		tmpParam->m_name = paramName; 
		tmpParam->m_description = paramDesc; 
		VariantInit(&tmpParam->m_value); 
		tmpParam->m_value.vt = VT_BSTR;
		tmpParam->m_value.bstrVal = paramVal.AllocSysString();
		VariantInit(&tmpParam->m_min_value); 
		tmpParam->m_min_value.vt = VT_BSTR;
		CString tmpS(""); 
		tmpParam->m_min_value.bstrVal = tmpS.AllocSysString();
		VariantInit(&tmpParam->m_max_value); 
		tmpParam->m_max_value.vt = VT_BSTR;
		CString tmpS2(""); 
		tmpParam->m_max_value.bstrVal = tmpS2.AllocSysString();
		AddCommandParameter(tmpParam);
   }

	void  AddParameter(const CString& paramName, const CString& paramDesc, long* paramArray, unsigned long paramArraySize, long paramMin, long paramMax)  
	{
		CommandParameter*  tmpParam = new CommandParameter(); 
		tmpParam->m_name = paramName; 
		tmpParam->m_description = paramDesc; 
		VariantInit(&tmpParam->m_value);   
		tmpParam->m_value.vt = VT_ARRAY | VT_I4;   
		SAFEARRAYBOUND bounds[1]; 
		bounds[0].cElements = paramArraySize; 
		bounds[0].lLbound = 0; 
		SAFEARRAY *psa = SafeArrayCreate(VT_I4,1,bounds); 
		long * psadata = NULL; 
		SafeArrayAccessData(psa,reinterpret_cast<void **>(&psadata)); 
		memcpy(psadata, paramArray, paramArraySize*sizeof(long));    
		SafeArrayUnaccessData(psa);   
		tmpParam->m_value.parray = psa;   
		VariantInit(&tmpParam->m_min_value); 
		tmpParam->m_min_value.vt = VT_I4;
		tmpParam->m_min_value.lVal = paramMin;
		VariantInit(&tmpParam->m_max_value); 
		tmpParam->m_max_value.vt = VT_I4;
		tmpParam->m_max_value.lVal = paramMax;
		AddCommandParameter(tmpParam);
	}

public:
	void FireChangeParameter(size_t paramInd, int newParamVal)
	{
		if (paramInd<m_params_IDisp.GetCount() && 
			paramInd<m_params.GetCount())
		{
			CCommandParam*  pComParam = static_cast<CCommandParam*>((void*)m_params_IDisp[paramInd]);
			VARTYPE aaa = m_params[paramInd]->m_value.vt;
			if (m_params[paramInd]->m_value.vt == VT_I4)
			{
				m_params[paramInd]->m_value.lVal = newParamVal;
				pComParam->ChangeParameterEvent();

				DEV_LOG( "New value = %i ", newParamVal );
			}
		}
	}

	void FireChangeParameter(size_t paramInd, float newParamVal)
	{
		if (paramInd<m_params_IDisp.GetCount() && 
			paramInd<m_params.GetCount())
		{
			CCommandParam*  pComParam = static_cast<CCommandParam*>((void*)m_params_IDisp[paramInd]);
			if (m_params[paramInd]->m_value.vt == VT_R8)
			{
				m_params[paramInd]->m_value.dblVal = newParamVal;
				pComParam->ChangeParameterEvent();
				
				DEV_LOG( "New value = %f ", newParamVal );
			}
		}
	}

	void FireChangeParameter(size_t paramInd, long* newParamVals, size_t arrSize)
	{
		if (paramInd<m_params_IDisp.GetCount() && 
			paramInd<m_params.GetCount())
		{
			CCommandParam*  pComParam = static_cast<CCommandParam*>((void*)m_params_IDisp[paramInd]);
			//if (m_params[paramInd]->m_value.vt == VT_ARRAY | VT_I4) // original
			if (m_params[paramInd]->m_value.vt == (VT_ARRAY | VT_I4)) // fixed
			{
				SAFEARRAY *psa = m_params[paramInd]->m_value.parray; 
				long * psadata = NULL; 
				SafeArrayAccessData(psa,reinterpret_cast<void **>(&psadata)); 
				memcpy(psadata, newParamVals, arrSize*sizeof(long));   
				SafeArrayUnaccessData(psa);   
				pComParam->ChangeParameterEvent();
			}
		}
	}

private:
	CComPtr<ICommand>   m_command_desriptor;
	CAtlArray< CComPtr<ICommandParam> >   m_params_IDisp;
	
	UINT                m_invalidate_flag;

	IAppManager*        m_app_manager;

	void AddCommandParameter(CommandParameter* cmnd_param)
	{
		CComPtr<ICommandParam>   tempParam;
		if (SUCCEEDED(tempParam.CoCreateInstance(__uuidof(CCommandParam), NULL, CLSCTX_INPROC)))
		{
			CCommandParam*  pComParam = static_cast<CCommandParam*>((void*)tempParam);
			if(pComParam)
			{
				m_params.Add(cmnd_param);
				pComParam->SetCommandParameterPtr(m_params.GetAt(m_params.GetCount()-1), (int)m_params.GetCount()-1);
				pComParam->SetCallback(this);
				m_params_IDisp.Add(tempParam);
			}
		}
	}
protected:
	bool m_changes_after_finish;

public:
	bool IsApplyChangesAfterFinish() 
	{
		return m_changes_after_finish;
	};
};

/************************************************************************/
/* TEST COMMAND   -----   ID = 0                                       */
/************************************************************************/

class CTestCommand : public CCommand
{
public:
	CTestCommand(IAppManager* app_manager):CCommand(app_manager)
	{
	}
	virtual ~CTestCommand() {}
public:

	virtual void OnMouseDown(LONG nX, LONG nY, LONG nButton)	
	{
		
	}
	virtual void OnMouseUp(LONG nX, LONG nY, LONG nButton)	
	{
		
	}
	virtual void OnMouseMove(LONG nX, LONG nY)	
	{
		
	}
	

	virtual void OnChangeParameter(int paramInd) 
	{
	}

	virtual void DrawOnGraphics(Gdiplus::Graphics* pGr, const CRect& rect)
	{

	}

	virtual bool DrawOnLayer()	
	{	
		return true;
	}


private:
};

