#pragma once
#include "../ASCOfficeFile/OfficeEventsTemplate.h"

template <class T, class Reciever>
class COfficeFileConverterEvents : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeFileConverterEvents(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnProgress( LONG nID, LONG nPercent)
	{
		m_pReciever->_OnProgress( nID, nPercent );
		return S_OK;
	}
	HRESULT OnProgressEx( LONG nID, LONG nPercent, SHORT* Cancel )
	{
		m_pReciever->_OnProgressEx( nID, nPercent, Cancel );
		return S_OK;
	}
    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		CTemporaryCS oCS(&m_csThread);
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
class COfficeFileEvents : public COfficeEventTemplate<T,Reciever>
{
public:
	COfficeFileEvents(Reciever *pReciever):COfficeEventTemplate(pReciever)
	{
		m_cnt = 0;
		m_dwAdvise = 0;
	};
	HRESULT OnError( LONG lError )
	{
		m_pReciever->_OnError( lError );
		return S_OK;
	}
	HRESULT OnCompleteParseFile( )
	{
		m_pReciever->_OnCompleteParseFile();
		return S_OK;
	}
	HRESULT OnProgressParseFile( LONG nID, LONG lProgressPage, SHORT* Cancel )
	{
		m_pReciever->_OnProgressParseFile( nID, lProgressPage, Cancel, this );
		return S_OK;
	}
	HRESULT OnProgressParsePage( LONG lProgress )
	{
		m_pReciever->_OnProgressParsePage( lProgress );
		return S_OK;
	}
	HRESULT OnCompleteParsePage( )
	{
		m_pReciever->_OnCompleteParsePage();
		return S_OK;
	}

    STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
		VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
	{ 
		CTemporaryCS oCS(&m_csThread);
		HRESULT hr;
		VARIANTARG varg0;
		VARIANTARG varg1;
		VARIANTARG varg2;
		switch(dispIdMember) {
		case 0:
			VariantInit(&varg0);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			OnError( varg0.lVal );
			return(S_OK);
			break;
		case 1:
			OnCompleteParseFile();
			return(S_OK);
			break;
		case 5:
			VariantInit(&varg0);
			VariantInit(&varg1);
			VariantInit(&varg2);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			hr = DispGetParam(pDispParams,1,VT_I4,&varg1,puArgErr);
			hr = DispGetParam(pDispParams,2,VT_I2,&varg2,puArgErr);		
			OnProgressParseFile( varg0.lVal, varg1.lVal, &varg2.iVal );
			if (pDispParams->rgvarg[0].vt & VT_BYREF)	// implemented now for last VT_I2 arg only
			{
				SHORT* p = (SHORT*)pDispParams->rgvarg[0].byref;
				if (p) *p = varg2.iVal;
			}
			return(S_OK);
			break;
		case 6:
			VariantInit(&varg0);
			hr = DispGetParam(pDispParams,0,VT_I4,&varg0,puArgErr);
			OnProgressParsePage( varg0.lVal );
			return(S_OK);
			break;
		case 7:
			OnCompleteParsePage();
			return(S_OK);
			break;
		default:
			return(E_NOTIMPL); 
		}
	}
};