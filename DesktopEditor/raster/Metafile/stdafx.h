#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../../SDK/lib/win_64/DEBUG/graphics.lib")
#pragma comment(lib, "../../../SDK/lib/win_64/DEBUG/ASCOfficeUtilsLib.lib")
#else
#pragma comment(lib, "../../../SDK/lib/win_64/graphics.lib")
#pragma comment(lib, "../../../SDK/lib/win_64/ASCOfficeUtilsLib.lib")
#endif
