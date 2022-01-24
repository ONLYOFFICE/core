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
#ifndef _BUILD_BASETHREAD_H_
#define _BUILD_BASETHREAD_H_

#include "../common/Types.h"
#include <atomic>

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
#include <windows.h>
#else
#include <pthread.h>
#endif

#if defined(_WIN32) || defined (_WIN64)
typedef DWORD ASC_THREAD_ID;
#else
typedef pthread_t ASC_THREAD_ID;
#endif

#include "../../Common/kernel_config.h"

namespace NSThreads
{
    KERNEL_DECL ASC_THREAD_ID GetCurrentThreadId();

    KERNEL_DECL void Sleep(int nMilliseconds);

    class CThreadDescriptor;
    class KERNEL_DECL CBaseThread
	{
	protected:
        CThreadDescriptor*  m_hThread;
        INT                 m_bRunThread;
        INT                 m_bSuspend;

        int                 m_lError;
        int                 m_lThreadPriority;

        bool                m_bIsNeedDestroy;
        std::atomic<bool>   m_bIsExit{false};

	public:
        CBaseThread();
        virtual ~CBaseThread();
	public:
        virtual void Start(int lPriority);
        virtual void Suspend();
        virtual void Resume();
        virtual void Stop();
        virtual void StopNoJoin();
        virtual void DestroyOnFinish();
        virtual void Cancel();

        INT IsSuspended();
        INT IsRunned();
        bool isAborted();
        int GetError();

        CThreadDescriptor* GetDescriptor();
        int GetPriority();
		
        virtual void CheckSuspend();

	protected:
        virtual void Join();
		virtual DWORD ThreadProc() = 0;

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
        static DWORD WINAPI __ThreadProc(void* pv);
#else
        static void* __ThreadProc(void* pv);
#endif
    };
}

#endif // _BUILD_BASETHREAD_H_
