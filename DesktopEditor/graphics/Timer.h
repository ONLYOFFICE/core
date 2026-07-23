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
#ifndef _BUILD_TIMER_H_
#define _BUILD_TIMER_H_

#include "BaseThread.h"

#ifdef _MAC
#include "mach/mach.h"
#include "mach/mach_time.h"
#endif

namespace NSTimers
{
	KERNEL_DECL DWORD GetTickCount();

	class KERNEL_DECL CTimer : public NSThreads::CBaseThread
	{
	private:
		DWORD	m_dwInterval;
		INT     m_bIsCOMNeed;

	public:
		CTimer();
		virtual ~CTimer();

		void SetInterval(const DWORD& dwInterval);
		void SetCOMNeed(const INT& bIsCOM);

	protected:
		virtual DWORD ThreadProc();

		virtual void OnTimer() = 0;
	};
	
	inline static unsigned long getUptimeInMilliseconds()
	{
#if defined(_IOS) || defined(_MAC)
		const int64_t kOneMillion = 1000 * 1000;
		static mach_timebase_info_data_t s_timebase_info;
		
		if (s_timebase_info.denom == 0) {
			(void) mach_timebase_info(&s_timebase_info);
		}
		
		// mach_absolute_time() returns billionth of seconds,
		// so divide by one million to get milliseconds
		return (unsigned long)((mach_absolute_time() * s_timebase_info.numer) / (kOneMillion * s_timebase_info.denom));
#endif
		
#ifdef __ANDROID__
		struct timespec ts;
		clock_gettime(CLOCK_MONOTONIC, &ts);
		
		return (ts.tv_sec * 1000 + (DWORD)(ts.tv_nsec / 1000000));
#endif
		
		return 0;
	}
}

#endif
