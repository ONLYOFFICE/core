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

#include "Timer.h"
#include "time.h"

#if defined(_WIN32) || defined (_WIN64)
//#define _CAN_USE_COM_THREADS
#endif

namespace NSTimers
{    
	// CLOCK_MONOTONIC defined ONLY since macOS 10.12!!! (crash on earlier version)
	DWORD GetTickCount()
	{
#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)
		return ::GetTickCount();
#else
#if defined(CLOCK_MONOTONIC) && !defined(_MAC)
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

	CTimer::CTimer() : NSThreads::CBaseThread()
	{
		m_dwInterval = 40;
		m_bIsCOMNeed = FALSE;
	}
	CTimer::~CTimer()
	{
	}

	void CTimer::SetInterval(const DWORD& dwInterval) { m_dwInterval = dwInterval; }
	void CTimer::SetCOMNeed(const INT& bIsCOM) { m_bIsCOMNeed = bIsCOM; }

	DWORD CTimer::ThreadProc()
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
}
