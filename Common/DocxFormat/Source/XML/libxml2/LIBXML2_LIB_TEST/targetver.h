#pragma once

// Следующие макросы определяют минимальные требования к версии платформы. Минимальные требования к версии платформы
// - это самая ранняя версия Windows, Internet Explorer и т. д., имеющая все необходимые функции для выполнения 
// вашего приложения. В результате выполнения макроса включаются все функции, доступные для версий платформы вплоть до указанной и 
// включая указанную версию.

// Измените следующие определения, если версия вашей платформы более ранняя, чем указанная ниже.
// Последние данные о соответствующих значениях для различных платформ см. в MSDN.
#ifndef _WIN32_WINNT            // Указывает, что минимально необходимой версией платформы является Windows Vista.
#define _WIN32_WINNT 0x0600     // Выберите значение, указывающее на другие версии Windows.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _CRT_SECURE_NO_DEPRECATE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS
#include <windows.h>

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlcoll.h>
#include <comutil.h>

using namespace ATL;
