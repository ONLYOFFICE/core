#pragma once

#include "Settings.h"

#if defined(_WIN32) || defined (_WIN64)
	#include <windows.h>
#else
	#include "../../Base/ASCString.h"
#endif

namespace FileSystem {
	class File {
	public:
        static bool Exists(LPCTSTR path);
        static bool Exists(const String& path);
		
		static void Create(LPCTSTR path);
        static void Create(const String& path);
	};
}
