#ifndef _METAFILE_COMMON_METAFILECLIP_H
#define _METAFILE_COMMON_METAFILECLIP_H

#include "IOutputDevice.h"

namespace MetaFile
{
	class IClip
	{
	public:
		IClip(){}
		virtual ~IClip(){}
		virtual void ClipOnRenderer(IOutputDevice* pOutput) = 0;
	};
}

#endif _METAFILE_COMMON_METAFILECLIP_H