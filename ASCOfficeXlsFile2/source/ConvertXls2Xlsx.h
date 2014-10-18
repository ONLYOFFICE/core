#include <windows.h>
#include <string>

struct ProgressCallback;

HRESULT ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* CallBack);