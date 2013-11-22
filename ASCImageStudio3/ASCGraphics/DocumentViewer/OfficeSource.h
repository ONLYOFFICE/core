#pragma once 
#include "..\stdafx.h"
#include "ViewerCommon.h"

#define AVS_OFFICE_SOURCE_COMPLETEPAGE_EVENT	7
#define AVS_OFFICE_SOURCE_COMPLETE_EVENT		1
#define AVS_OFFICE_SOURCE_WATERMARK_EVENT		9

#define AVS_COUNT_PAGES_UPDATE_SOURCE			10

class CEventsSource
{
public:
	virtual void OnCompletePage()
	{
	}
	virtual void OnUpdateWatermark()
	{
	}
};

class COfficeSource
{
	class COfficeSourceEvents : public DocConverter::_IAVSXMLOfficeSourceEvents
	{
	public:
		COfficeSourceEvents(CEventsSource* pSource)
		{
			m_cnt = 0;
			m_dwAdvise = 0;

			m_pSource = pSource;
			m_nCountPages = 0;
		}
		virtual ~COfficeSourceEvents(void)
		{
		}

		STDMETHOD(GetTypeInfoCount)(UINT*) { return E_NOTIMPL; }
		STDMETHOD(GetTypeInfo)(UINT, LCID, ITypeInfo**) { return E_NOTIMPL; }
		STDMETHOD(GetIDsOfNames)(REFIID, LPOLESTR*, UINT, LCID, DISPID*) { return E_NOTIMPL; }

		STDMETHOD(Invoke)(DISPID dispIdMember, 	REFIID	riid, LCID	lcid, WORD	wFlags, DISPPARAMS*	pDispParams, 
			VARIANT*	pVarResult, EXCEPINFO*	pExcepInfo, UINT*	puArgErr)
		{ 
			switch(dispIdMember) 
			{
			case AVS_OFFICE_SOURCE_COMPLETEPAGE_EVENT:
				{
					++m_nCountPages;
					if (AVS_COUNT_PAGES_UPDATE_SOURCE <= m_nCountPages)
					{
						m_nCountPages = 0;
						if (NULL != m_pSource)
						{
							m_pSource->OnCompletePage();
						}
					}
					return S_OK;
				}
			case AVS_OFFICE_SOURCE_COMPLETE_EVENT:
				{
					m_nCountPages = 0;
					if (NULL != m_pSource)
					{
						m_pSource->OnCompletePage();
					}
					return S_OK;
				}
			case AVS_OFFICE_SOURCE_WATERMARK_EVENT:
				{
					if (NULL != m_pSource)
					{
						m_pSource->OnUpdateWatermark();
					}
					return S_OK;
				}
			default:
				return E_NOTIMPL; 
			}
		}

		STDMETHOD(QueryInterface)(REFIID iid, LPVOID* ppv) { 
			if ((iid == __uuidof(DocConverter::_IAVSXMLOfficeSourceEvents)) ||
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
			m_nCountPages = 0;
			
			if (NULL == pFile)
				return FALSE;
			IConnectionPointContainer* pContainer;
			IConnectionPoint* pCP;
			HRESULT hr = pFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
			if (NULL!=pContainer)
			{
				pContainer->FindConnectionPoint(__uuidof(DocConverter::_IAVSXMLOfficeSourceEvents), &pCP);	
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
			if ((NULL == pFile) || (0 == m_dwAdvise) || (NULL == m_pSource))
				return FALSE;

			IConnectionPointContainer* pContainer;
			IConnectionPoint* pCP;

			HRESULT hr = pFile->QueryInterface(IID_IConnectionPointContainer, (void**)&pContainer);
			if (NULL!=pContainer)
			{
				hr = pContainer->FindConnectionPoint(__uuidof(DocConverter::_IAVSXMLOfficeSourceEvents), &pCP);	
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

		
	public:
		CEventsSource* m_pSource;
	protected:
		LONG  m_cnt;
		DWORD m_dwAdvise;

		int m_nCountPages;  // это для того, чтобы не слишком часто подменялся сорс, а каждые 10 страниц
	};

public:
	DocConverter::IAVSXMLOfficeSource* m_pSource;
	COfficeSourceEvents m_oEvents;

public:
	IUnknown* GetSource()
	{
		if (NULL == m_pSource)
			return NULL;

		IUnknown* punk = NULL;
		m_pSource->QueryInterface(IID_IUnknown, (void**)&punk);
		return punk;		
	}
	void SetSource(IUnknown* pSource)
	{
		m_oEvents.UnAdvise(m_pSource);
		RELEASEINTERFACE(m_pSource);

		if (NULL != pSource)
		{
			pSource->QueryInterface(DocConverter::IID_IAVSXMLOfficeSource, (void**)&m_pSource);
		}

		m_oEvents.Advise(m_pSource);
		m_oEvents.AddRef();
	}
	
public:
	COfficeSource(CEventsSource* pSource) : m_oEvents(pSource)
	{
		m_pSource = NULL;
	}
	~COfficeSource()
	{
		SetSource(NULL);
	}
	
};