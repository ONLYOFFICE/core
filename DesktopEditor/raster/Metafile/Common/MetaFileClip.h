#ifndef _METAFILE_COMMON_METAFILECLIP_H
#define _METAFILE_COMMON_METAFILECLIP_H

#include "IOutputDevice.h"

namespace MetaFile
{
	class CClip
	{
	public:
		CClip(){}
		virtual ~CClip(){}
		virtual void ClipOnRenderer(IOutputDevice* pOutput) = 0;
	};
}

#endif _METAFILE_COMMON_METAFILECLIP_H