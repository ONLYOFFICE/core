#ifndef VLC_INSTANCE_H
#define VLC_INSTANCE_H

#include "libvlc_base.h"

// Singleton wrapper around libvlc_instance_t
class CVlcInstance
{
private:
	CVlcInstance();
	~CVlcInstance();

public:
	/* NOTE:
	 * setVerbosityLevel() must be called before first get().
	 * If get() was already called once, all subsequent setVerbosityLevel() won't do anything.
	 */
	static void setVerbosityLevel(unsigned nVerbose);
	static CVlcInstance& get();
	libvlc_instance_t* getVlcInstance();

public:
	static unsigned m_nVerbose;

private:
	libvlc_instance_t* m_pVlcInstance;
};

// shorthand for CVlcInstance::get().getVlcInstance()
libvlc_instance_t* GetVlcInstance();

#endif // VLC_INSTANCE_H
