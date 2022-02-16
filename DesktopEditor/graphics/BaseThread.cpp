/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#if defined(__ANDROID__) && !defined (NOT_USE_PTHREAD_CANCEL)
#include <pthread_setcanceltype.h>
#endif

#include "./BaseThread.h"

#if defined(_WIN32) || defined(_WIN64) ||defined(_WIN32_WCE)

#include <winbase.h>

#else

#include <signal.h>
#include "time.h"

#endif

namespace NSThreads
{
    class CThreadDescriptor
    {
    public:
        CThreadDescriptor()
        {
        }
        virtual ~CThreadDescriptor()
        {
        }
    };

    ASC_THREAD_ID GetCurrentThreadId()
    {
#if defined(_WIN32) || defined (_WIN64)
        return ::GetCurrentThreadId();
#else
        return pthread_self();
#endif
    }

    void Sleep(int nMilliseconds)
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

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
    DWORD WINAPI CBaseThread::__ThreadProc(void* pv)
    {
        CBaseThread* pThis = (CBaseThread*)pv;
        DWORD value = pThis->ThreadProc();
        if (pThis->m_bIsNeedDestroy)
            delete pThis;
        return value;
    }

    class __native_thread : public CThreadDescriptor
    {
        friend class CBaseThread;
    private:
        HANDLE m_thread = nullptr;

    public:
        __native_thread() : CThreadDescriptor()
        {
        }
        virtual ~__native_thread()
        {
            if (m_thread)
            {
                CloseHandle(m_thread);
                m_thread = NULL;
            }
        }
    };
#else
    void* CBaseThread::__ThreadProc(void* pv)
    {
#ifndef NOT_USE_PTHREAD_CANCEL
        int old_thread_type;
        pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &old_thread_type);
#endif

        CBaseThread* pThis = (CBaseThread*)pv;
        pThis->ThreadProc();

        if (pThis->m_bIsNeedDestroy)
            delete pThis;

        return NULL;
    }
    class __native_thread : public CThreadDescriptor
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

    CBaseThread::CBaseThread()
    {
        m_hThread		= NULL;
        m_bRunThread	= FALSE;
        m_bSuspend		= FALSE;

        m_lError			= 0;
        m_lThreadPriority	= 0;

        m_bIsNeedDestroy = false;
    }
    CBaseThread::~CBaseThread()
    {
        Stop();
    }
    void CBaseThread::Start(int lPriority)
    {
        if (m_bRunThread)
            return;
        m_lError = 0;
        m_bSuspend = FALSE;

        m_hThread = new __native_thread();

        m_bRunThread = TRUE;

        m_bIsExit.store(false);
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
        DWORD dwTemp;
        ((__native_thread*)m_hThread)->m_thread = CreateThread(NULL, 0, &__ThreadProc, (void*)this, 0, &dwTemp);
        SetThreadPriority(((__native_thread*)m_hThread)->m_thread, lPriority);
#else
        pthread_create(&((__native_thread*)m_hThread)->m_thread, 0, &__ThreadProc, (void*)this);
#endif
        m_lThreadPriority = lPriority;
    }
    void CBaseThread::Suspend()
    {
        m_bSuspend = TRUE;
    }
    void CBaseThread::Resume()
    {
        m_bSuspend = FALSE;
    }
    void CBaseThread::Stop()
    {
        if (!m_bRunThread)
            return;

        m_bIsExit.store(true);
        m_bRunThread = FALSE;

        Join();
        RELEASEOBJECT(m_hThread);
    }
    void CBaseThread::StopNoJoin()
    {
        m_bRunThread = FALSE;
        m_bIsExit.store(true);
        RELEASEOBJECT(m_hThread);
    }
    void CBaseThread::DestroyOnFinish()
    {
        m_bIsNeedDestroy = true;
    }

    INT CBaseThread::IsSuspended() { return m_bSuspend; }
    INT CBaseThread::IsRunned() { return m_bRunThread; }
    int CBaseThread::GetError() { return m_lError; }
    bool CBaseThread::isAborted() {return m_bIsExit && m_bIsExit.load();}

    CThreadDescriptor* CBaseThread::GetDescriptor() { return m_hThread; }
    int CBaseThread::GetPriority() { return m_lThreadPriority; }

    void CBaseThread::CheckSuspend()
    {
        while (m_bSuspend && m_bRunThread)
            NSThreads::Sleep(10);
    }

    void CBaseThread::Join()
    {
        if (NULL == m_hThread)
            return;

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
        WaitForSingleObject(((__native_thread*)m_hThread)->m_thread, INFINITE);
#else
        pthread_join(((__native_thread*)m_hThread)->m_thread, 0);
#endif
    }

    void CBaseThread::Cancel()
    {
        if (NULL == m_hThread)
            return;

         m_bIsExit.store(true);

         m_bRunThread = FALSE;

         Join();
         RELEASEOBJECT(m_hThread);
    }
}
