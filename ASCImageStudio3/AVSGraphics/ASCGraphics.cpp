// ASCGraphics.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

//#include "Objects\ASCGraphics.h"
//#include "Objects\ASCEdgeText.h"
//#include "Objects\ASCShadow.h"
//#include "Objects\ASCStringFormat.h"
#include "Objects\ASCFontManager.h"
#include "Objects\ASCWinFonts.h"
//#include "Objects\ASCImage.h"

#include "Objects\VMLPainter.h"
#include "Objects\TextPainter.h"
#include "Objects\EffectTransform.h"

#include "Interfaces\ASCMetafile.h"
#include "Interfaces\ASCDocumentRenderer.h"
#include "Interfaces\ASCGdiplusRenderer.h"
#include "Interfaces\ASCGraphicsRenderer.h"
#include "Interfaces\ASCRenderer.h"
#include "Interfaces\ASCDocumentPainter.h"
#include "Interfaces\ASCEffectPainter.h"
#include "Interfaces\ASCGraphicsSimpleConverter.h"
#include "Interfaces\SVGRenderer.h"

// for animation

#include "Objects\AnimationLayer.h"
#include "Objects\AnimationTransformer.h"

// viewer
#include "DocumentViewer\DocumentViewer.h"
#include "DocumentViewer\ViewerThumbnails.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{AB5F2678-2773-44c4-A637-33DFBC51233B}", 
		 name = "ASCGraphics", 
		 helpstring = "ASCGraphics 1.0 Type Library",
		 resource_name = "IDR_ASCGRAPHICS") ];
