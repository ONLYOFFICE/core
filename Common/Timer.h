#ifndef _TIMER_H_
#define _TIMER_H_

#include "ATLDefine.h"
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

		m_hAPIThread = NULL;
		m_dwAPIThreadID = 0;

		m_hCOMThread = NULL;
		m_dwCOMThreadID = 0;

		m_pStream = NULL;
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
			if (NULL != m_hCOMThread)
			{
				WaitWithMessageLoop(m_hCOMThread);	// STA requirements
			}
		}
		RELEASEHANDLE(m_hCOMThread);
	}


// Implementation
private:
	// Pump messages while waiting for event.
	BOOL WaitWithMessageLoop(HANDLE hEvent)
	{
		while(1)
		{
			const DWORD dwRet = MsgWaitForMultipleObjects(1, &hEvent, FALSE, INFINITE, QS_ALLINPUT);

			if (WAIT_OBJECT_0 == dwRet)
				return TRUE;    // The event was signaled

			if (dwRet != WAIT_OBJECT_0 + 1)
				break;          // Something else happened

			MSG msg;

			// There is one or more window message available. Dispatch them
			while (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage (&msg);
				DispatchMessage (&msg);
				if (WAIT_OBJECT_0 == WaitForSingleObject(hEvent, 0))
					return TRUE; // Event is now signaled.
			}
		}
		return FALSE;
	}

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
		DWORD m_startTime (0), m_curTime (0);
		m_startTime = GetTickCount();
		HRESULT hRes = CoInitialize(NULL);

		T* piObj = NULL;

		if (m_pStream)
			hRes = CoGetInterfaceAndReleaseStream(m_pStream, *piid, (void**) &piObj);

		while(m_bCOMTimerOn)
		{
			m_curTime = GetTickCount();
			while (m_curTime - m_startTime < m_dwCOMTimerInterval)
			{
				Sleep(10);
				if (!m_bCOMTimerOn) break;
				m_curTime = GetTickCount();
			}

			m_startTime = GetTickCount();
			if (piObj)
				piObj->_OnTimer();
		}
		RELEASEINTERFACE (piObj);
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
};

#endif