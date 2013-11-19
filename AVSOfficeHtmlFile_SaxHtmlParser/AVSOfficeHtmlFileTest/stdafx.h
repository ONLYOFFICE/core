// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#ifdef DEBUG
#import "..\bin\Debug\AVSOfficeHtmlFile.dll"							rename_namespace("AVSOfficeHtmlFile")
#else
#import "..\bin\Release\AVSOfficeHtmlFile.dll"							rename_namespace("AVSOfficeHtmlFile")
#endif