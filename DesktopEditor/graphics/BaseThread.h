/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

#if defined(_WIN32) || defined(_WIN64)
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
		CThreadDescriptor* m_hThread;
		INT m_bRunThread;
		INT m_bSuspend;

		int m_lError;
		int m_lThreadPriority;

		bool m_bIsNeedDestroy;
		std::atomic<bool> m_bIsExit{false};

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
