#pragma once

#include "ASCOfficeCriticalSection.h"

// This class helps to lock/unlock critical section and defend any using code from deadlocks due to unhandled exceptions
class CSLocker
{
public:
	CSLocker(AVSOfficeCriticalSection &critical_section) : cs(critical_section)
	{
		cs.Enter();
	}

	~CSLocker(void)
	{
		cs.Leave();
	}

private:
	AVSOfficeCriticalSection &cs;
};
