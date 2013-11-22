// AVSMediaCore3.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{F0A0AAE5-06FE-4ABC-A9F1-26BFFEE2A11F}", 
		 name = "AVSMediaCore3", 
		 helpstring = "AVSMediaCore3 1.0 Type Library",
		 resource_name = "IDR_AVSMEDIACORE3") ];

#include "AVSMediaData.h"
#include "AVSAudioData.h"
#include "AVSAudioDataEx.h"
#include "AVSUncompressedVideoFrame.h"
#include "AVSCompressedVideoFrame.h"
#include "AVSVideoFrameColorTransform.h"
#include "AVSDVDSubpictureFrame.h"
#include "AVSVideoFramePainter.h"
#include "AVSVideoFramePainter2.h"
#include "AVSSubtitleData.h"
#include "AVSPacketsAudioData.h"