#pragma once

#ifdef _DEBUG
//	#define _DBG_CS
#endif

class ASCOfficeCriticalSection
{
public:
	ASCOfficeCriticalSection()
	{
#ifdef _DBG_CS	
		m_dbgCount	=	0L;
#endif
#if defined(_WIN32) || defined(_WIN64)
		InitializeCriticalSection (&m_CS);
#endif
	}

	~ASCOfficeCriticalSection()
	{
#if defined(_WIN32) || defined(_WIN64)
	DeleteCriticalSection (&m_CS);
#endif
#ifdef _DBG_CS	
		++m_dbgCount;
		ATLTRACE (_T("delCS : %d\n"), m_dbgCount); 
#endif
	}

	inline void Enter()
	{
#if defined(_WIN32) || defined(_WIN64)
		EnterCriticalSection (&m_CS);
#endif
#ifdef _DBG_CS	
		++m_dbgCount;
		ATLTRACE (_T("dbgCS : %d\n"), m_dbgCount);
#endif
	}

	inline void Leave()
	{
#ifdef _WIN32
		LeaveCriticalSection (&m_CS);
#endif
#ifdef _DBG_CS	
		--m_dbgCount;
		ATLTRACE (_T("dbgCS : %d\n"), m_dbgCount);
#endif
	}

private:

#if defined(_WIN32) || defined(_WIN64)
	CRITICAL_SECTION m_CS;
#endif
#ifdef _DBG_CS	
	LONG	m_dbgCount;
#endif
};