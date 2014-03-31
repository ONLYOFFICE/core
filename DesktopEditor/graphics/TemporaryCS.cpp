#include "TemporaryCS.h"
#include "../common/Types.h"

#if defined(WIN32) || defined(_WIN32_WCE)

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
