// AVSMediaFormatSettings3.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "AVSAudioFormat.h"
#include "AVSVideoFormat.h"
#include "AVSMPEGsVideoFormat.h"
#include "AVSMPEG2VideoFormat.h"
#include "AVSMPEG4VideoFormat.h"
#include "AVSH263VideoFormat.h"
#include "AVSH264VideoFormat.h"
#include "AVSH264ExtVideoFormat.h"
#include "AVSWMVVideoFormat.h"
#include "AVSRMVideoFormat.h"
#include "AVSRMAudioFormat.h"
#include "AVSWMAAudioFormat.h"
#include "AVSMPEG4AudioFormat.h"
#include "AVSMPEG3AudioFormat.h"
#include "AVSExternalVideoFormat.h"
#include "AVSOggVorbisAudioFormat.h"
#include "AVSSubPictureFormat.h"
#include "AVSVPXVideoFormat.h"

#include "AVSMediaFormatSerialize.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{12481495-4D84-409F-B52E-E728AE3D3BC9}", 
		name = "AVSMediaFormatSettings3", 
		helpstring = "AVSMediaFormatSettings3 1.0 Type Library",
		resource_name = "IDR_AVSMEDIAFORMATSETTINGS3")];
