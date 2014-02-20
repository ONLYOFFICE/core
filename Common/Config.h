#pragma once

#ifndef BUILD_CONFIG_OPENSOURCE_VERSION
	#define BUILD_CONFIG_FULL_VERSION 
#endif

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