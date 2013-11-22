// AVSGraphics.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"

//#include "Objects\AVSGraphics.h"
//#include "Objects\AVSEdgeText.h"
//#include "Objects\AVSShadow.h"
//#include "Objects\AVSStringFormat.h"
#include "Objects\AVSFontManager.h"
#include "Objects\AVSWinFonts.h"
//#include "Objects\AVSImage.h"

#include "Objects\VMLPainter.h"
#include "Objects\TextPainter.h"
#include "Objects\EffectTransform.h"

#include "Interfaces\AVSMetafile.h"
#include "Interfaces\AVSDocumentRenderer.h"
#include "Interfaces\AVSGdiplusRenderer.h"
#include "Interfaces\AVSGraphicsRenderer.h"
#include "Interfaces\AVSRenderer.h"
#include "Interfaces\AVSDocumentPainter.h"
#include "Interfaces\AVSEffectPainter.h"
#include "Interfaces\AVSGraphicsSimpleConverter.h"
#include "Interfaces\SVGRenderer.h"

// for animation

#include "Objects\AnimationLayer.h"
#include "Objects\AnimationTransformer.h"

// viewer
#include "DocumentViewer\DocumentViewer.h"
#include "DocumentViewer\ViewerThumbnails.h"

// The module attribute causes DllMain, DllRegisterServer and DllUnregisterServer to be automatically implemented for you
[ module(dll, uuid = "{AB5F2678-2773-44c4-A637-33DFBC51233B}", 
		 name = "AVSGraphics", 
		 helpstring = "AVSGraphics 1.0 Type Library",
		 resource_name = "IDR_AVSGRAPHICS") ];
