#include <windows.h>
#include <string>

struct ProgressCallback;

HRESULT ConvertOO2OOX(const std::wstring &ext, const std::wstring & srcPath, const std::wstring & dstPath, bool bOnlyPresentation, const ProgressCallback* CallBack);