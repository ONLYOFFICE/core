#pragma once

class CTimeMeasurer
{
public:
	CTimeMeasurer()
	{
		QueryPerformanceFrequency(&m_liFreq);	
		QueryPerformanceCounter(&m_liStart);
	}
	virtual ~CTimeMeasurer()
	{
	}

	void Reset()
	{
		QueryPerformanceCounter(&m_liStart);
	}
	//интервал в секундах
	virtual float GetTimeInterval()
	{
		LARGE_INTEGER liFinish;
		QueryPerformanceCounter(&liFinish);
		return (float)((liFinish.QuadPart-m_liStart.QuadPart)/(float)m_liFreq.LowPart);
	}
protected:
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liFreq;	
};

class CTimeMeasurerExt
	: public CTimeMeasurer
{
public:
	CTimeMeasurerExt()
		: m_bIsSuspended(FALSE)
	{
		m_liSuspendedTime.QuadPart = 0;
	}
	virtual ~CTimeMeasurerExt()
	{
	}
	void Suspend()
	{
		if (m_bIsSuspended)
			return;
		m_bIsSuspended = TRUE;
		QueryPerformanceCounter(&m_liStartSuspend);
	}
	void Resume()
	{
		if (!m_bIsSuspended)
			return;
		LARGE_INTEGER liTemp;
		QueryPerformanceCounter(&liTemp);
		m_liSuspendedTime.QuadPart += liTemp.QuadPart - m_liStartSuspend.QuadPart;
		m_bIsSuspended = FALSE;
	}
	//интервал в секундах
	virtual float GetTimeInterval()
	{
		if (m_bIsSuspended)
			return (float)((m_liStartSuspend.QuadPart - m_liStart.QuadPart - m_liSuspendedTime.QuadPart)/(float)m_liFreq.LowPart);

		LARGE_INTEGER liFinish;
		QueryPerformanceCounter(&liFinish);
		return (float)((liFinish.QuadPart - m_liStart.QuadPart - m_liSuspendedTime.QuadPart)/(float)m_liFreq.LowPart);
	}
protected:
	LARGE_INTEGER m_liStartSuspend;
	LARGE_INTEGER m_liSuspendedTime;
	BOOL m_bIsSuspended;
};

class CTraceTimer
{
public:
	CTraceTimer(const CString &sTitle)
		: m_sTitle(sTitle)
	{
	}
	~CTraceTimer()
	{
		ATLTRACE("%s %0.3f\n",m_sTitle,m_oTimer.GetTimeInterval());
	}
	void Trace(const CString &sSubTitle)
	{
		ATLTRACE("%s::%s %0.3f\n",m_sTitle,sSubTitle,m_oTimer.GetTimeInterval());
	}
protected:
	CTimeMeasurer m_oTimer;
	CString m_sTitle;
};
