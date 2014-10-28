#pragma once
#include "../../DesktopEditor/graphics/TemporaryCS.h"
class AVSOfficeCriticalSection
{
  private:
    NSCriticalSection::CRITICAL_SECTION CriticalSection;

  public:
	AVSOfficeCriticalSection()
    {
		CriticalSection.InitializeCriticalSection();
    }

	~AVSOfficeCriticalSection()
	{
		CriticalSection.DeleteCriticalSection();
	}

	void Enter()
	{
		CriticalSection.Enter();
	}

	void Leave()
	{
		CriticalSection.Leave();
	}
};