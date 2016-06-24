/*
 * (c) Copyright Ascensio System SIA 2010-2016
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#ifndef _BUILD_BASETHREAD_H_
#define _BUILD_BASETHREAD_H_

#include "../common/Types.h"

#if defined(_WIN32) || defined(_WIN64) ||defined(_WIN32_WCE)

#include "windows.h"
#include "winbase.h"

#else

#include <pthread.h>
#include <signal.h>
#include "time.h"

#endif

namespace NSThreads
{
	class CThreadDescriptor
	{
	public:
		CThreadDescriptor(){}
		virtual ~CThreadDescriptor(){}
	};

	static void Sleep(int nMilliseconds)
	{
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
		::Sleep((DWORD)nMilliseconds);
#else
		struct timespec tim, tim2;
        tim.tv_sec = nMilliseconds / 1000;
        tim.tv_nsec = (nMilliseconds % 1000) * 1000000;

		::nanosleep(&tim , &tim2);
#endif
	}

	class CBaseThread
	{
	protected:
		CThreadDescriptor*	m_hThread;
		INT				m_bRunThread;
		INT				m_bSuspend;

		int				m_lError;
		int				m_lThreadPriority;

	public:
		CBaseThread()
		{
			m_hThread		= NULL;
			m_bRunThread	= FALSE;
			m_bSuspend		= FALSE;

			m_lError			= 0;
			m_lThreadPriority	= 0;
		}
		virtual ~CBaseThread()
		{
			Stop();
		}
	public:
		virtual void Start(int lPriority)
		{
			if (m_bRunThread)
				return;
			m_lError = 0;
			m_bSuspend = FALSE;

			m_hThread = new __native_thread();

            m_bRunThread = TRUE;
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
			DWORD dwTemp;			
			((__native_thread*)m_hThread)->m_thread = CreateThread(NULL, 0, &__ThreadProc, (void*)this, 0, &dwTemp);
			SetThreadPriority(((__native_thread*)m_hThread)->m_thread, lPriority);
#else
            pthread_create(&((__native_thread*)m_hThread)->m_thread, 0, &__ThreadProc, (void*)this);
#endif
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
			
			Join();
			RELEASEOBJECT(m_hThread);
		}
		
		inline INT IsSuspended() { return m_bSuspend; }
		inline INT IsRunned() { return m_bRunThread; }
		inline int GetError() { return m_lError; }

		inline CThreadDescriptor* GetDescriptor() { return m_hThread; }
		inline int GetPriority() { return m_lThreadPriority; }
		
		virtual void CheckSuspend()
		{
			while (m_bSuspend && m_bRunThread)
				NSThreads::Sleep(10);
		}

	protected:
		virtual void Join()
		{
			if (NULL == m_hThread)
				return;

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
			WaitForSingleObject(((__native_thread*)m_hThread)->m_thread, INFINITE);
#else
			pthread_join(((__native_thread*)m_hThread)->m_thread, 0);
#endif
		}
		virtual DWORD ThreadProc() = 0;

	private:

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
		static DWORD WINAPI __ThreadProc(void* pv)
		{
			CBaseThread* pThis = (CBaseThread*)pv;
			return pThis->ThreadProc();
		}

		class __native_thread : public NSThreads::CThreadDescriptor
		{
			friend class CBaseThread;
		private:
			HANDLE m_thread;

		public:
			__native_thread() : CThreadDescriptor()
			{
				m_thread = NULL;
			}
			virtual ~__native_thread()
			{
				if (m_thread != NULL)
				{
					CloseHandle(m_thread);
					m_thread = NULL;
				}				
			}
		};
#else
        static void* __ThreadProc(void* pv)
		{
			int old_thread_type;
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_thread_type);

			CBaseThread* pThis = (CBaseThread*)pv;
			pThis->ThreadProc();
            return NULL;
		}
		class __native_thread : public NSThreads::CThreadDescriptor
		{
			friend class CBaseThread;
		private:
			pthread_t m_thread;

		public:
			__native_thread() : CThreadDescriptor()
			{
				m_thread = NULL;
			}
			virtual ~__native_thread()
			{
			}
		};
#endif
	};	
}

#endif // _BUILD_BASETHREAD_H_
