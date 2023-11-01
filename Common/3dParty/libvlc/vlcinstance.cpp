#include "vlcinstance.h"
#include <algorithm>

#include <iostream>

unsigned CVlcInstance::m_nVerbose = 0;

CVlcInstance::CVlcInstance()
{
	const char *argv[2];
	int argc = (m_nVerbose == 0 ? 1 : 2);
	char verbose[2];

	if (m_nVerbose > 0)
	{
		argv[0] = "--verbose";
		sprintf(verbose, "%d", m_nVerbose);
		argv[1] = verbose;
	}
	else
	{
		argv[0] = "--quiet";
	}

	m_pVlcInstance = libvlc_new(argc, argv);

	// error if libvlc instantiation was unsuccessful
	if (m_pVlcInstance == nullptr)
	{
		std::cerr << "Could not init libVLC" << std::endl;
		exit(1);
	}
}

CVlcInstance::~CVlcInstance()
{
	libvlc_release(m_pVlcInstance);
}

void CVlcInstance::setVerbosityLevel(unsigned nVerbose)
{
	// verbosity level can be only 0, 1 or 2
	m_nVerbose = std::min(nVerbose, 2u);
}

CVlcInstance& CVlcInstance::get()
{
	static CVlcInstance oVlcInstance;
	return oVlcInstance;
}

libvlc_instance_t* CVlcInstance::getVlcInstance()
{
	return m_pVlcInstance;
}

libvlc_instance_t* GetVlcInstance()
{
	return CVlcInstance::get().getVlcInstance();
}
