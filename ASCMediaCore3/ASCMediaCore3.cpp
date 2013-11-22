// ASCMediaCore3.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{F0A0AAE5-06FE-4ABC-A9F1-26BFFEE2A11F}", 
		 name = "ASCMediaCore3", 
		 helpstring = "ASCMediaCore3 1.0 Type Library",
		 resource_name = "IDR_ASCMEDIACORE3") ];

#include "ASCMediaData.h"
#include "ASCAudioData.h"
#include "ASCAudioDataEx.h"
#include "ASCUncompressedVideoFrame.h"
#include "ASCCompressedVideoFrame.h"
#include "ASCVideoFrameColorTransform.h"
#include "ASCDVDSubpictureFrame.h"
#include "ASCVideoFramePainter.h"
#include "ASCVideoFramePainter2.h"
#include "ASCSubtitleData.h"
#include "ASCPacketsAudioData.h"