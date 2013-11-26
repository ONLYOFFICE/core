#pragma once

//#define BUILD_CONFIG_OPENSOURCE_VERSION
#define BUILD_CONFIG_FULL_VERSION

#ifdef BUILD_CONFIG_OPENSOURCE_VERSION

class CRegistratorClient
{
public:
	static BOOL IsRegistered()
	{
		return true;
	}
};

#endif