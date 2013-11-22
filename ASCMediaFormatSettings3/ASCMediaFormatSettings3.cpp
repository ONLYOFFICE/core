// ASCMediaFormatSettings3.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "ASCAudioFormat.h"
#include "ASCVideoFormat.h"
#include "ASCMPEGsVideoFormat.h"
#include "ASCMPEG2VideoFormat.h"
#include "ASCMPEG4VideoFormat.h"
#include "ASCH263VideoFormat.h"
#include "ASCH264VideoFormat.h"
#include "ASCH264ExtVideoFormat.h"
#include "ASCWMVVideoFormat.h"
#include "ASCRMVideoFormat.h"
#include "ASCRMAudioFormat.h"
#include "ASCWMAAudioFormat.h"
#include "ASCMPEG4AudioFormat.h"
#include "ASCMPEG3AudioFormat.h"
#include "ASCExternalVideoFormat.h"
#include "ASCOggVorbisAudioFormat.h"
#include "ASCSubPictureFormat.h"
#include "ASCVPXVideoFormat.h"

#include "ASCMediaFormatSerialize.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{12481495-4D84-409F-B52E-E728AE3D3BC9}", 
		name = "ASCMediaFormatSettings3", 
		helpstring = "ASCMediaFormatSettings3 1.0 Type Library",
		resource_name = "IDR_ASCMEDIAFORMATSETTINGS3")];
