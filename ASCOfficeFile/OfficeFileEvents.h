#pragma once
#include "OfficeEventsTemplate.h"

template <class T, class Reciever>
class COfficeEventDocuments : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeEventDocuments(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgress( LONG nID, LONG nPercent)
	{
		m_pReciever->OnProgressDocument( nID, nPercent );
		return S_OK;
	}
	HRESULT OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel)
	{
		m_pReciever->OnProgressDocumentEx( nID, nPercent, Cancel );
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		VARIANTARG varg2;
		switch(dispIdMember) {
		case 1:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			OnProgress( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		case 2:
			VariantInit(&varg0);
			VariantInit(&varg1);
			VariantInit(&varg2);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			hr = DispGetParam(pDispParams,2,VT_I2,&varg2,puArgErr);		
			OnProgressEx( varg0.lVal, varg1.lVal, &varg2.iVal );
			if (pDispParams->rgvarg[0].vt & VT_BYREF)	// implemented now for last VT_I2 arg only
			{
				SHORT* p = (SHORT*)pDispParams->rgvarg[0].byref;
				if (p) *p = varg2.iVal;
			}
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};

template <class T, class Reciever>
class COfficeEventViewer : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeEventViewer(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnError(LONG lError)//[id(0)] 
	{
		m_pReciever->OnErrorViewer( lError );
		return S_OK;
	}
	HRESULT OnStop()//[id(1)] 
	{
		m_pReciever->OnStopViewer();
		return S_OK;
	}
	HRESULT OnProgress(LONG lProgressPage)//[id(5)] 
	{
		m_pReciever->OnProgressViewer(  lProgressPage );
		return S_OK;
	}
	HRESULT OnProgressParce(LONG lType, LONG lProgress)//[id(6)] 
	{
		m_pReciever->OnProgressParse( lType, lProgress );
		return S_OK;
	}
	HRESULT OnNewPage(double dWidthMm, double dHeightMm)//[id(10)] 
	{
		m_pReciever->OnNewPageViewer(dWidthMm, dHeightMm);
		return S_OK;
	}
	HRESULT OnCompletePage() //[id(11)] 
	{
		m_pReciever->OnCompletePageViewer();
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		switch(dispIdMember) {
		case 0:
			VariantInit(&varg0);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			OnError( varg0.lVal);
			return(S_OK);
			break;
		case 1:
			OnStop( );
			return(S_OK);
			break;
		case 5:
			VariantInit(&varg0);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			OnProgress( varg0.lVal );
			return(S_OK);
			break;
		case 6:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_R8,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_R8,&varg1,puArgErr);
			OnProgressParce( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		case 10:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_R8,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_R8,&varg1,puArgErr);
			OnNewPage( varg0.dblVal, varg1.dblVal );
			return(S_OK);
			break;
		case 11:
			OnCompletePage();
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};
template <class T, class Reciever>
class COfficeFileConvEvents : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeFileConvEvents(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel)
	{
		m_pReciever->OnProgressDocumentEx( nID, nPercent, Cancel );
		return S_OK;
	}
	HRESULT OnProgress( LONG nID, LONG nPercent)
	{
		m_pReciever->OnProgressDocument( nID, nPercent );
		return S_OK;
	}
	HRESULT OnCompletePage()
	{
		m_pReciever->OnCompletePageDocument();
		return S_OK;
	}
    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		VARIANTARG varg2;
		switch(dispIdMember) {
		case 1:
			VariantInit(&varg0);
			VariantInit(&varg1);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			OnProgress( varg0.lVal, varg1.lVal );
			return(S_OK);
			break;
		case 2:
			VariantInit(&varg0);
			VariantInit(&varg1);
			VariantInit(&varg2);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			hr = DispGetParam(pDispParams,2,VT_I2,&varg2,puArgErr);		
			OnProgressEx( varg0.lVal, varg1.lVal, &varg2.iVal );
			if (pDispParams->rgvarg[0].vt & VT_BYREF)	// implemented now for last VT_I2 arg only
			{
				SHORT* p = (SHORT*)pDispParams->rgvarg[0].byref;
				if (p) *p = varg2.iVal;
			}
			return(S_OK);
			break;
		case 10:
			OnCompletePage();
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};
template <class T, class Reciever>
class CCommandRendererEvents : public COfficeEventTemplate<T,Reciever>
{
public:
	CCommandRendererEvents(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnCompletePageDocument()
	{
		m_pReciever->OnCompletePageDocument();
		return S_OK;
	}
    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		//CTemporaryCS oCS(&m_csThread);
		switch(dispIdMember) {
		case 100:
			return(S_OK);
			break;
		case 101:
			OnCompletePageDocument();
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};