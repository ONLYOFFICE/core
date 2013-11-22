#pragma once
#include "..\stdafx.h"

static LPCTSTR g_cpszXMLMultiSource				= _T("MultiSource");
static LPCTSTR g_cpszXMLSingleSource			= _T("SingleSource");
static LPCTSTR g_cpszXMLVideoSources			= _T("VideoSources");
static LPCTSTR g_cpszXMLVideoSource				= _T("VideoSource");
static LPCTSTR g_cpszXMLColorSource 			= _T("ColorSource");
static LPCTSTR g_cpszXMLImageSource 			= _T("ImageSource");
static LPCTSTR g_cpszXMLVideoTransforms     	= _T("VideoTransforms");
static LPCTSTR g_cpszXMLVideoCompose			= _T("VideoCompose");

static LPCTSTR g_cpszXMLEffectText              = _T("ImagePaint-DrawTextEx");
static LPCTSTR g_cpszXMLPath		            = _T("ImagePaint-DrawGraphicPath");
static LPCTSTR g_cpszXMLImage	                = _T("ImagePaint-DrawImageFromFile");

#define OFFICEEDITOR_PROGRESS_TYPE_ERROR	        0
#define OFFICEEDITOR_PROGRESS_TYPE_STOP		        1
#define OFFICEEDITOR_PROGRESS_TYPE_PAGE				2
#define OFFICEEDITOR_PROGRESS_TYPE_ELEMENT			3

class CEventReceiver
{
public:
	virtual void OnSendEvent(LONG lType, LONG lParam1, LONG lParam2) = 0;
};
