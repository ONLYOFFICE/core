#ifndef _BUILD_TIMER_H_
#define _BUILD_TIMER_H_

#include "BaseThread.h"

namespace NSTimers
{
	static DWORD GetTickCount()
	{
#if defined(WIN32) || defined(_WIN32_WCE)
		return ::GetTickCount();
#else
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

        return (ts.tv_sec * 1000 + (DWORD)(ts.tv_nsec / 100000));
#endif
	}

	class CTimer : public NSThreads::CBaseThread
	{
	private:
		DWORD	m_dwInterval;
		BOOL	m_bIsCOMNeed;

	public:
		CTimer() : NSThreads::CBaseThread()
		{
			m_dwInterval = 40;
			m_bIsCOMNeed = FALSE;
		}
		virtual ~CTimer()
		{
		}

		inline void SetInterval(const DWORD& dwInterval) { m_dwInterval = dwInterval; }
		inline void SetCOMNeed(const BOOL& bIsCOM) { m_bIsCOMNeed = bIsCOM; }

	protected:
		virtual DWORD ThreadProc()
		{
#if defined(WIN32) || defined(_WIN32_WCE)
			if (m_bIsCOMNeed)
				CoInitialize(NULL);
#endif
			DWORD m_startTime, m_curTime;
			m_startTime = NSTimers::GetTickCount();

			while (m_bRunThread)
			{
				m_curTime = NSTimers::GetTickCount();
				while (m_curTime - m_startTime < m_dwInterval)
				{
					NSThreads::Sleep(10);
					if (!m_bRunThread)
						break;
					m_curTime = NSTimers::GetTickCount();
				}

				m_startTime = NSTimers::GetTickCount();				
				OnTimer();				
			}

#if defined(WIN32) || defined(_WIN32_WCE)
			if (m_bIsCOMNeed)
				CoUninitialize();
#endif
			return 0;
		}

		virtual void OnTimer() = 0;
	};
}

#endif