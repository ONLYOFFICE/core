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
		InitializeCriticalSection (&m_CS);
	}

	~ASCOfficeCriticalSection()
	{
		DeleteCriticalSection (&m_CS);		
#ifdef _DBG_CS	
		++m_dbgCount;
		ATLTRACE (_T("delCS : %d\n"), m_dbgCount); 
#endif
	}

	inline void Enter()
	{
		EnterCriticalSection (&m_CS);
#ifdef _DBG_CS	
		++m_dbgCount;
		ATLTRACE (_T("dbgCS : %d\n"), m_dbgCount);
#endif
	}

	inline void Leave()
	{
		LeaveCriticalSection (&m_CS);
#ifdef _DBG_CS	
		--m_dbgCount;
		ATLTRACE (_T("dbgCS : %d\n"), m_dbgCount);
#endif
	}

private:

	CRITICAL_SECTION m_CS;
#ifdef _DBG_CS	
	LONG	m_dbgCount;
#endif
};