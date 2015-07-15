#include <string>

struct ProgressCallback;

long ConvertXls2Xlsx(const std::wstring & srcFile, const std::wstring & dstPath, const ProgressCallback* CallBack);