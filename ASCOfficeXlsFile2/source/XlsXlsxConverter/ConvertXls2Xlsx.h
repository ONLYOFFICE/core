#include <string>

struct ProgressCallback;

long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const std::wstring & password, const std::wstring& fontsPath, const ProgressCallback* CallBack);