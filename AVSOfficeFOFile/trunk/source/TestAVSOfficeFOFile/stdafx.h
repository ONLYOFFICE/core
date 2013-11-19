// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <atlbase.h>
#include <atlstr.h>

#include <string>
#include <iostream>

//#include "../oox2foutils/oox2foutils.h"
#include <boost/format.hpp>



#ifdef _DEBUG
//#pragma comment(lib, "../bin/oox2foutilsD.lib")
/*#pragma comment(lib,"../../../Common/AvsDocxFormat/Lib/Debug/libboost_filesystem-vc90-mt-gd-1_38.lib")
#pragma comment(lib,"../../../Common/AvsDocxFormat/Lib/Debug/libboost_system-vc90-mt-gd-1_38.lib")*/
#else
//#pragma comment(lib, "../bin/oox2foutils.lib")
/*#pragma comment(lib,"../../../Common/AvsDocxFormat/Lib/Release/libboost_filesystem-vc90-mt-1_38.lib")
#pragma comment(lib,"../../../Common/AvsDocxFormat/Lib/Debug/libboost_system-vc90-mt-1_38.lib")*/
#endif
/*
#include <boost/filesystem/path.hpp>
*/




#include <msxml2.h>
#include <conio.h>

#import "../../bin/AVSOfficeFOFile.dll" rename_namespace("AVSOffice")

// TODO: reference additional headers your program requires here
