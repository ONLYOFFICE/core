#pragma once

class AVSOfficeCriticalSection
{
  private:
    CRITICAL_SECTION CriticalSection;

  public:
	AVSOfficeCriticalSection()
    {
	  InitializeCriticalSection( &(this->CriticalSection) );
    }

	~AVSOfficeCriticalSection()
	{
	  DeleteCriticalSection( &(this->CriticalSection) );
	}

	void Enter()
	{
	  EnterCriticalSection( &(this->CriticalSection) );
	}

	void Leave()
	{
	  LeaveCriticalSection( &(this->CriticalSection) );
	}
};