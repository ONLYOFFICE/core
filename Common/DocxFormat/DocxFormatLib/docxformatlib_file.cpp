#if defined(_WIN32) || defined (_WIN64)
#include "../Source/SystemUtility/FileSystem/Directory.cpp"
#include "../Source/SystemUtility/FileSystem/File.cpp"
#else
#include "../Source/SystemUtility/FileSystem/DirectoryPosix.cpp"
#include "../Source/SystemUtility/FileSystem/FilePosix.cpp"
#endif

#include "../Source/XlsxFormat/Common.cpp"
#include "../Source/XML/libxml2/libxml2.cpp"
#include "../Source/XML/stringcommon.cpp"
#include "../Source/SystemUtility/SystemUtility.cpp"
