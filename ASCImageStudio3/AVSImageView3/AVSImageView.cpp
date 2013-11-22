// AVSImageView.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[module(dll,
		uuid = "{8B0D4DF9-37A8-4a37-906E-1A4295762E4E}", 
		name = "AVSImageView3", 
		helpstring = "AVSImageView 3",
		resource_name = "IDR_AVSIMAGEVIEW") ];


#include "ImageView.h"
#include "ImageViewLight.h"
#include "ImageViewQuick.h"