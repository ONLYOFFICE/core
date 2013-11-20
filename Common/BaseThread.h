#pragma once

//
class CBaseThread
{
public:
	CBaseThread(DWORD_PTR dwAffinityMask)
		: m_bRunThread(FALSE)
		, m_hThread(NULL)
		, m_lPercents(0)
		, m_lError(0)
		, m_dwAffinityMask(dwAffinityMask)
		, m_bSuspend(FALSE)
	{
		if (0==m_dwAffinityMask)
		{
			DWORD_PTR dwProcessAffinityMask = 0;
			DWORD_PTR dwSystemAffinityMask = 0;
			if (GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask))
				m_dwAffinityMask = dwProcessAffinityMask;
		}
	}
	virtual ~CBaseThread()
	{
		Stop();
	}
	virtual void Start(long lPriority)
	{
		if (m_bRunThread)
			return;
		m_lError = 0;
		m_bSuspend = FALSE;
		StartThread();
		SetThreadPriority(m_hThread, lPriority);
		m_lThreadPriority = lPriority;
	}
	virtual void Suspend()
	{
		m_bSuspend = TRUE;
	}
	virtual void Resume()
	{
		m_bSuspend = FALSE;
	}
	virtual void Stop()
	{
		if (!m_bRunThread)
			return;
		m_bRunThread = FALSE;
		if (NULL!=m_hThread)
			WaitForSingleObject(m_hThread,INFINITE);
		RELEASEHANDLE(m_hThread);
	}
	inline BOOL IsSuspended() const
	{		
		return m_bSuspend;
	}
	inline BOOL IsRunned() const
	{		
		return m_bRunThread;
	}
	inline long GetPercents() const
	{
		return m_lPercents;
	}

	inline long GetError() const
	{
		return m_lError;
	}
	inline HANDLE GetHandle()
	{
		return m_hThread;
	}
	inline int GetPriority()
	{
		return GetThreadPriority(m_hThread);
	}
protected:
	HANDLE m_hThread;
	BOOL m_bRunThread;
	BOOL m_bSuspend;
	DWORD_PTR m_dwAffinityMask;
	long m_lPercents;

	long m_lError;
	long m_lThreadPriority;


	void StartThread()
	{
		DWORD dwTemp;
		m_bRunThread = TRUE;
		m_hThread = CreateThread(NULL, 0, &_ThreadProc, (void*)this, 0, &dwTemp);
		SetThreadAffinityMask(m_hThread, m_dwAffinityMask);
	}
	void CheckSuspend()
	{
		while ((m_bSuspend)&&(m_bRunThread))
			Sleep(10);
	}
	static DWORD WINAPI _ThreadProc(void* pv)
	{
		CBaseThread *pThis = (CBaseThread *)pv;
		return pThis->ThreadProc();
	}
	virtual DWORD ThreadProc() = 0;
};