#pragma once
#include "TemporaryCS.h"

//class CVideoFile3;
template <class T, class Reciever>
class COfficeEventTemplate : public T
{
protected: CRITICAL_SECTION m_csThread;
public:
	COfficeEventTemplate(Reciever *pReciever)
		: m_pReciever(pReciever)
	{
		InitializeCriticalSection(&m_csThread);
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	virtual ~COfficeEventTemplate(void)
	{
		DeleteCriticalSection(&m_csThread);
	}
    STDMETHOD(GetTypeInfoCount)(UINT*) { return E_NOTIMPL; }
    STDMETHOD(GetTypeInfo)(UINT, LCID, ITypeInfo**) { return E_NOTIMPL; }
    STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, UINT, LCID, DISPID*) { return E_NOTIMPL; }

    STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppv) { 
        if ((iid == __uuidof(T)) ||
        (iid == __uuidof(IDispatch)) ||
        (iid == __uuidof(IUnknown)))
            *ppv = this;
        else {
            *ppv = 0;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    STDMETHOD_(ULONG,AddRef)() { 
		return InterlockedIncrement(&m_cnt); 
	}

    STDMETHOD_(ULONG,Release)() { 
	    InterlockedDecrement(&m_cnt);
		if (m_cnt!=0) return m_cnt;
		delete this;
		return 0;
    }

	BOOL Advise(IUnknown *pFile)
	{
		CTemporaryCS oCS(&m_csThread);
		if (NULL==pFile)
			return FALSE;
		IConnectionPointContainer* pContainer;
		IConnectionPoint* pCP;
		HRESULT hr = pFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
		if (NULL!=pContainer)
		{
			pContainer->FindConnectionPoint(__uuidof(T), &pCP);	
			if (NULL!=pCP)
			{
				pCP->Advise(this, &m_dwAdvise);
				pCP->Release();
				pCP = NULL;
			}
			pContainer->Release();
			pContainer = NULL;
		}
		return TRUE;
	}
	BOOL UnAdvise(IUnknown *pFile)
	{
		CTemporaryCS oCS(&m_csThread);
		if ((NULL==pFile)||(0==m_dwAdvise))
			return FALSE;

		IConnectionPointContainer* pContainer;
		IConnectionPoint* pCP;

		HRESULT hr = pFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
		if (NULL!=pContainer)
		{
			hr = pContainer->FindConnectionPoint(__uuidof(T), &pCP);	
			if (NULL!=pCP)
			{
				hr = pCP->Unadvise(m_dwAdvise);
				pCP->Release();
				pCP = NULL;
			}
			pContainer->Release();
			pContainer = NULL;
		}
		return TRUE;
	}

	Reciever* m_pReciever;
protected:
	LONG  m_cnt;
	DWORD m_dwAdvise;
};

template <class T, class Reciever>
class CEventPdf : public COfficeEventTemplate<T,Reciever>
{
public:
	CEventPdf(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgress( LONG nID, LONG nPercent)
	{
		m_pReciever->OnProgressPdf( nPercent );
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		switch(dispIdMember) {
		case 1:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			OnProgress( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};