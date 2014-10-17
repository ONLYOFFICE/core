#include <windows.h>
#include <string>

struct ProgressCallback;

HRESULT ConvertXls2Xlsx(const std::wstring &ext, const std::wstring & srcFile, const std::wstring & dstPath, bool bOnlyPresentation, const ProgressCallback* CallBack);