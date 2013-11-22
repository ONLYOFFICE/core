#pragma once

class CTimeMeasurer
{
public:
	CTimeMeasurer()
	{
		QueryPerformanceFrequency(&m_liFreq);	
		QueryPerformanceCounter(&m_liStart);
	}

	void Reset()
	{
		QueryPerformanceCounter(&m_liStart);
	}
	//интервал в секундах
	double GetTimeInterval()
	{
		LARGE_INTEGER liFinish;
		QueryPerformanceCounter(&liFinish);
		return (double)((liFinish.QuadPart-m_liStart.QuadPart)/(double)m_liFreq.LowPart);
	}
protected:
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liFreq;	
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
		ATLTRACE("%s : %f\n",m_sTitle,m_oTimer.GetTimeInterval());
	}
	void Trace(const CString &sSubTitle)
	{
		ATLTRACE("%s::%s %f\n",m_sTitle,sSubTitle,m_oTimer.GetTimeInterval() * 1000.0 );
	}
protected:
	CTimeMeasurer m_oTimer;
	CString m_sTitle;
};
