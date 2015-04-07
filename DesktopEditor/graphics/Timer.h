#ifndef _BUILD_TIMER_H_
#define _BUILD_TIMER_H_

#include "BaseThread.h"
#include "time.h"

#ifdef _MAC
#include "mach/mach.h"
#include "mach/mach_time.h"
#endif

#if defined(_WIN32) || defined (_WIN64)
//#define _CAN_USE_COM_THREADS
#endif

#if defined(_WIN32) || defined (_WIN64)
typedef DWORD ASC_THREAD_ID;
#else
typedef pthread_t ASC_THREAD_ID;
#endif

namespace NSThreads
{
    static ASC_THREAD_ID GetCurrentThreadId()
    {
#if defined(_WIN32) || defined (_WIN64)
        return ::GetCurrentThreadId();
#else
        return pthread_self();
#endif
    }
}

namespace NSTimers
{
#ifdef _MAC
    static DWORD getUptimeInMilliseconds()
    {
        const int64_t kOneMillion = 1000 * 1000;
        static mach_timebase_info_data_t s_timebase_info;
        
        if (s_timebase_info.denom == 0) {
            (void) mach_timebase_info(&s_timebase_info);
        }
        
        // mach_absolute_time() returns billionth of seconds,
        // so divide by one million to get milliseconds
        return (DWORD)((mach_absolute_time() * s_timebase_info.numer) / (kOneMillion * s_timebase_info.denom));
    }
#endif
    
	static DWORD GetTickCount()
	{
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
		return ::GetTickCount();
#else
#ifdef CLOCK_MONOTONIC
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);

        return (ts.tv_sec * 1000 + (DWORD)(ts.tv_nsec / 1000000));
#else
        //uint64_t nano = mach_absolute_time();
        //return nano / 1000000;
        return getUptimeInMilliseconds();
#endif
#endif
	}

	class CTimer : public NSThreads::CBaseThread
	{
	private:
		DWORD	m_dwInterval;
		INT	m_bIsCOMNeed;

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
		inline void SetCOMNeed(const INT& bIsCOM) { m_bIsCOMNeed = bIsCOM; }

	protected:
		virtual DWORD ThreadProc()
		{
#ifdef _CAN_USE_COM_THREADS
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

#ifdef _CAN_USE_COM_THREADS
			if (m_bIsCOMNeed)
				CoUninitialize();
#endif
			return 0;
		}

		virtual void OnTimer() = 0;
	};
}

#endif
