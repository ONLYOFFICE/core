#pragma once

class CTemporaryCS
{
public:
	CTemporaryCS(CRITICAL_SECTION *cs)
	{
		EnterCriticalSection(cs);
		m_cs = cs;
	}
	~CTemporaryCS()
	{
		LeaveCS();
	}
	void LeaveCS()
	{
		if (NULL!=m_cs)
			LeaveCriticalSection(m_cs);
		m_cs = NULL;
	}
	void EnterCS(CRITICAL_SECTION *cs)
	{
		LeaveCS();
		EnterCriticalSection(cs);
		m_cs = cs;
	}
protected:
	CRITICAL_SECTION *m_cs;
};