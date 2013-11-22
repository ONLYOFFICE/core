// ASCGraphics.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

#include "Objects\ASCFontManager.h"
#include "Objects\ASCWinFonts.h"

#include "Interfaces\ASCGdiplusRenderer.h"
#include "Interfaces\ASCGraphicsRenderer.h"
#include "Interfaces\ASCRenderer.h"
#include "Interfaces\ASCGraphicsSimpleConverter.h"
#include "Interfaces\SVGRenderer.h"
#include "Interfaces\ASCMetafile.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{AB5F2678-2773-44c4-A637-33DFBC51233B}", 
		 name = "ASCGraphics", 
		 helpstring = "ASCGraphics 1.0 Type Library",
		 resource_name = "IDR_ASCGRAPHICS") ];
