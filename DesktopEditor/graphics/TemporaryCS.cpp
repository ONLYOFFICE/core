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
#include "TemporaryCS.h"
#include "../common/Types.h"

namespace NSCriticalSection
{
    class CRITICAL_SECTION_NATIVE
    {
    public:
        CRITICAL_SECTION_NATIVE()
        {
        }
        virtual ~CRITICAL_SECTION_NATIVE()
        {
        }

        virtual void Enter() = 0;
        virtual void Leave() = 0;
    };
}

#if defined(_WIN32) || defined(_WIN64) || defined(_WIN32_WCE)

#include "windows.h"
#include "winbase.h"

class __critical_section : public NSCriticalSection::CRITICAL_SECTION_NATIVE
{
private:
	::CRITICAL_SECTION m_cs;

public:
	virtual void Enter()
	{
		EnterCriticalSection(&m_cs);
	}
	virtual void Leave()
	{
		LeaveCriticalSection(&m_cs);
	}

public:
	static void InitializeCriticalSection(NSCriticalSection::CRITICAL_SECTION_NATIVE* pNative)
	{
		__critical_section* pCS = (__critical_section*)pNative;
		::InitializeCriticalSection(&pCS->m_cs);
	}
	static void DeleteCriticalSection(NSCriticalSection::CRITICAL_SECTION_NATIVE* pNative)
	{
		__critical_section* pCS = (__critical_section*)pNative;
		::DeleteCriticalSection(&pCS->m_cs);
	}
};

#else

#include <pthread.h>

#ifndef PTHREAD_MUTEX_RECURSIVE_NP
#define PTHREAD_MUTEX_RECURSIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

class __critical_section : public NSCriticalSection::CRITICAL_SECTION_NATIVE
{
private:
	pthread_mutex_t m_cs;

public:
	virtual void Enter()
	{
		pthread_mutex_lock(&m_cs);
	}
	virtual void Leave()
	{
		pthread_mutex_unlock(&m_cs);
	}

public:
	static void InitializeCriticalSection(NSCriticalSection::CRITICAL_SECTION_NATIVE* pNative)
	{
		__critical_section* pCS = (__critical_section*)pNative;
		
		pthread_mutexattr_t _attr;
        pthread_mutexattr_init(&_attr);
        pthread_mutexattr_settype(&_attr, PTHREAD_MUTEX_RECURSIVE_NP);
        pthread_mutex_init(&pCS->m_cs, &_attr);
		pthread_mutexattr_destroy(&_attr);
	}
	static void DeleteCriticalSection(NSCriticalSection::CRITICAL_SECTION_NATIVE* pNative)
	{
		__critical_section* pCS = (__critical_section*)pNative;
        pthread_mutex_destroy(&pCS->m_cs);
	}
};

#endif

namespace NSCriticalSection
{
	CRITICAL_SECTION::CRITICAL_SECTION()
	{
		m_pCS = new __critical_section();
	}
	CRITICAL_SECTION::~CRITICAL_SECTION()
	{
		RELEASEOBJECT(m_pCS);
	}

	void CRITICAL_SECTION::InitializeCriticalSection()
	{
		__critical_section::InitializeCriticalSection(m_pCS);
	}

	void CRITICAL_SECTION::DeleteCriticalSection()
	{
		__critical_section::DeleteCriticalSection(m_pCS);
	}

	void CRITICAL_SECTION::Enter()
	{
		if (NULL != m_pCS)
			m_pCS->Enter();
	}
	void CRITICAL_SECTION::Leave()
	{
		if (NULL != m_pCS)
			m_pCS->Leave();
	}

    CRITICAL_SECTION_SMART::CRITICAL_SECTION_SMART()
    {
        m_pCS = new CRITICAL_SECTION();
        m_pCS->InitializeCriticalSection();
    }
    CRITICAL_SECTION_SMART::~CRITICAL_SECTION_SMART()
    {
        m_pCS->DeleteCriticalSection();
        RELEASEOBJECT(m_pCS);
    }
    void CRITICAL_SECTION_SMART::Enter()
    {
        if (NULL != m_pCS)
            m_pCS->Enter();
    }
    void CRITICAL_SECTION_SMART::Leave()
    {
        if (NULL != m_pCS)
            m_pCS->Leave();
    }
}

CTemporaryCS::CTemporaryCS(NSCriticalSection::CRITICAL_SECTION* cs)
{
	cs->Enter();
	m_cs = cs;
}
CTemporaryCS::~CTemporaryCS()
{
	LeaveCS();
}
void CTemporaryCS::LeaveCS()
{
	if (NULL != m_cs)
		m_cs->Leave();
	m_cs = NULL;
}
void CTemporaryCS::EnterCS(NSCriticalSection::CRITICAL_SECTION* cs)
{
	LeaveCS();
	cs->Enter();
	m_cs = cs;
}
