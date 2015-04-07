// AVSImageView.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module ( dll, uuid = "{20A99195-A346-42cf-9684-4212462A13A0}", name = "AVSImageView5", helpstring = "AVSImageView5", resource_name = "IDR_AVSIMAGEVIEW5") ];

#include "ImageView.h"
#include "TrackController.h"
#include "TextController.h"
