#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////
// CTimer
template <class Derived, class T, const IID* piid>
class CEventSource
{
protected:HANDLE m_hNeedEvent;
public:

	CEventSource()
	{
		m_bAPITimerOn = FALSE;
	}

	HRESULT APIEventsOn( DWORD dwTimerInterval  )
	{
		Derived* pDerived = ((Derived*)this);

		m_dwAPITimerInterval = dwTimerInterval;
		if (m_bAPITimerOn) // already on, just change interval
			return S_OK;

		m_bAPITimerOn = TRUE;

		// Create thread and pass the thread proc the this ptr
		m_hAPIThread = CreateThread(NULL, 0, &_APIApartment, (void*)this, 0, &m_dwAPIThreadID);

		return S_OK;
	}

	void APIEventsOff()
	{
		if (m_bAPITimerOn) 
		{
			m_bAPITimerOn = FALSE;
			Derived* pDerived = ((Derived*)this);
			SetEvent( pDerived->m_hNeedEvent );
			if (NULL!=m_hAPIThread)
				WaitForSingleObject(m_hAPIThread, INFINITE);
		}
		RELEASEHANDLE(m_hAPIThread);	
	}


// Implementation
private:
	static DWORD WINAPI _APIApartment(void* pv)
	{
		CEventSource<Derived, T, piid>* pThis = (CEventSource<Derived, T, piid>*) pv;
		pThis->APIApartment();
		return 0;
	}

	DWORD APIApartment()
	{
		DWORD dwStartTime, dwCurTime;
		dwStartTime = GetTickCount();

		CoInitialize(NULL);

		while( m_bAPITimerOn )
		{
			Derived* pDerived = ((Derived*)this);
			WaitForSingleObject( pDerived->m_hNeedEvent, INFINITE );

			dwCurTime = GetTickCount();
			while(dwCurTime - dwStartTime < m_dwAPITimerInterval)
			{
				Sleep(100);
				if(!m_bAPITimerOn) break;
				dwCurTime = GetTickCount();
			}

			dwStartTime = GetTickCount();

			if( TRUE == m_bAPITimerOn )
				OnEvent();
		}

		CoUninitialize();
		return 0;
	}
virtual	void OnEvent (void)
		{
			return;
		}
// Implementation
private:
	DWORD	m_dwAPITimerInterval;

	HANDLE	m_hAPIThread;
	DWORD	m_dwAPIThreadID;
	BOOL	m_bAPITimerOn;
};