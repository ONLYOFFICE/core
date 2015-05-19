#ifndef _TIMER_H_
#define _TIMER_H_

//////////////////////////////////////////////////////////////////////////////////////////////////
// CTimer
template <class Derived, class T, const IID* piid>
class CTimer
{
public:

	CTimer()
	{
		m_bCOMTimerOn = FALSE;
		m_bAPITimerOn = FALSE;
	}

	HRESULT APITimerOn(DWORD dwTimerInterval)
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

	HRESULT COMTimerOn(DWORD dwTimerInterval)
	{
		HRESULT hRes;
		Derived* pDerived = ((Derived*)this);

		m_dwCOMTimerInterval = dwTimerInterval;
		if (m_bCOMTimerOn) // already on, just change interval
			return S_OK;

		m_bCOMTimerOn = TRUE;
		m_pStream = NULL;


		hRes = CoMarshalInterThreadInterfaceInStream(*piid, (T*)pDerived, &m_pStream);

		m_hCOMThread = CreateThread(NULL, 0, &_COMApartment, (void*)this, 0, &m_dwCOMThreadID);

		return S_OK;
	}

	void APITimerOff()
	{
		if (m_bAPITimerOn) 
		{
			m_bAPITimerOn = FALSE;
			if (NULL!=m_hAPIThread)
				WaitForSingleObject(m_hAPIThread, INFINITE);
		}
		RELEASEHANDLE(m_hAPIThread);	
	}

	void COMTimerOff()
	{
		if (m_bCOMTimerOn) 
		{
			m_bCOMTimerOn = FALSE;
			if (NULL!=m_hCOMThread)
				WaitForSingleObject(m_hCOMThread,INFINITE);
		}
		RELEASEHANDLE(m_hCOMThread);
	}


// Implementation
private:
	static DWORD WINAPI _APIApartment(void* pv)
	{
		CTimer<Derived, T, piid>* pThis = (CTimer<Derived, T, piid>*) pv;
		pThis->APIApartment();
		return 0;
	}

	static DWORD WINAPI _COMApartment(void* pv)
	{
		CTimer<Derived, T, piid>* pThis = (CTimer<Derived, T, piid>*) pv;
		pThis->COMApartment();
		return 0;
	}

	DWORD APIApartment()
	{
		DWORD m_startTime, m_curTime;
		m_startTime = GetTickCount();

		CoInitialize(NULL);

		while(m_bAPITimerOn)
		{
			m_curTime = GetTickCount();
			while(m_curTime - m_startTime < m_dwAPITimerInterval)
			{
				Sleep(10);
				if(!m_bAPITimerOn) break;
				m_curTime = GetTickCount();
			}

			m_startTime = GetTickCount();
			
			OnTimer();
			
		}
		CoUninitialize();
		return 0;
	}

	DWORD COMApartment()
	{
		DWORD m_startTime, m_curTime;
		m_startTime = GetTickCount();
		CoInitialize(NULL);
		HRESULT hRes;

		m_spT.Release();

		if (m_pStream)
			hRes = CoGetInterfaceAndReleaseStream(m_pStream, *piid, (void**)&m_spT);

		while(m_bCOMTimerOn)
		{
			m_curTime = GetTickCount();
			while( m_curTime - m_startTime < m_dwCOMTimerInterval)
			{
				Sleep(10);
				if (!m_bCOMTimerOn) break;
				m_curTime = GetTickCount();
			}

			m_startTime = GetTickCount();
			m_spT->_OnTimer();
		}
		m_spT.Release();


		CoUninitialize();
		return 0;
	}

// Attributes
public:
	DWORD	m_dwCOMTimerInterval;
	DWORD	m_dwAPITimerInterval;

virtual	void OnTimer (void)
		{
			return;
		}

// Implementation
private:
	HANDLE	m_hAPIThread;
	DWORD	m_dwAPIThreadID;
	BOOL	m_bAPITimerOn;
	
	HANDLE	m_hCOMThread;
	DWORD	m_dwCOMThreadID;
	BOOL	m_bCOMTimerOn;

	LPSTREAM m_pStream;
	CComPtr<T> m_spT;
};

#endif