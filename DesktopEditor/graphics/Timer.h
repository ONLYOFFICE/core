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
