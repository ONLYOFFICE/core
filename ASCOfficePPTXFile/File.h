#pragma once

#include "Settings.h"
#include <windows.h>

namespace FileSystem {
	class File {
	public:
        static bool Exists(LPCTSTR path);
        static bool Exists(const String& path);
		
		static void Create(LPCTSTR path);
        static void Create(const String& path);
	};
}